/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>

#include <ox/mc/read.hpp>
#include <ox/std/trace.hpp>

#include "server.hpp"

DataFeed::DataFeed(QIODevice *dev, bool skipInit): QObject(dev) {
	m_dev = dev;
	if (!skipInit) {
		connect(m_dev, &QIODevice::readyRead, this, &DataFeed::handleInit);
	} else {
		connect(m_dev, &QIODevice::readyRead, this, &DataFeed::read);
	}
}

const QSharedPointer<ProcessData> &DataFeed::procData() {
	return m_procData;
}

void DataFeed::handleInit() {
	const auto init = [&] {
		disconnect(m_dev, &QIODevice::readyRead, this, &DataFeed::handleInit);
		connect(m_dev, &QIODevice::readyRead, this, &DataFeed::read);
	};
	ox::trace::MsgId peekChar;
	m_dev->peek(reinterpret_cast<char*>(&peekChar), 1);
	if (peekChar == ox::trace::MsgId::Json) {
		auto doc = QJsonDocument::fromJson(m_dev->readLine());
		if (doc.isObject()) {
			auto msg = doc.object();
			if (msg["type"].toString() == "Init") {
				init();
			}
		}
	} else if (peekChar == ox::trace::MsgId::Init) {
		ox::Array<char, 5> hdrBuff;
		m_dev->peek(hdrBuff.data(), hdrBuff.size());
		const auto msgSize = *reinterpret_cast<uint32_t*>(&hdrBuff[1]);
		while (m_dev->bytesAvailable() < msgSize && m_dev->isOpen()) {
			m_dev->waitForBytesWritten(1);
		}
		m_dev->skip(5);
		auto msgBuff = ox_malloca(msgSize, char);
		m_dev->read(msgBuff.get(), msgSize);
		const auto [msg, err] = ox::readMC<ox::trace::InitTraceMsgRcv>(msgBuff.get(), msgSize);
		if (err) [[unlikely]] {
			qDebug().noquote() << "Bad message";
			return;
		}
		init();
	}
}

void DataFeed::read() {
	while (m_dev && m_dev->bytesAvailable()) {
		ox::trace::MsgId msgId;
		m_dev->peek(reinterpret_cast<char*>(&msgId), 1);
		if (msgId == ox::trace::MsgId::Init && !m_dev->isOpen()) {
			qInfo() << "Connection closed";
			break;
		} else if (msgId == ox::trace::MsgId::TraceEvent) {
			if (m_dev->bytesAvailable() > 5) {
				if (!handleMcTraceEvent()) {
					break;
				}
			}
		} else if (msgId == ox::trace::MsgId::Json) {
			const auto json = m_dev->readLine();
			const auto doc = QJsonDocument::fromJson(json);
			if (m_procData) {
				const auto msg = doc.object();
				if (msg["type"] == "TraceEvent") {
					addTraceEvent(msg["data"].toObject());
				} else if (msg["type"] == "Init") {
					qInfo() << "Connection closed";
					endFeed();
					break;
				} else {
					qDebug().noquote() << "Bad message:" << json;
				}
			}
		} else {
			qDebug().noquote() << "Bad message id:" << static_cast<int>(msgId);
			qDebug() << "Connection is in invalid state, ending.";
			m_dev->close();
			m_dev->deleteLater();
		}
	}
}

bool DataFeed::handleMcTraceEvent() {
	ox::Array<char, 5> hdrBuff;
	m_dev->peek(hdrBuff.data(), hdrBuff.size());
	const auto msgSize = *reinterpret_cast<uint32_t*>(&hdrBuff[1]);
	if (m_dev->bytesAvailable() < msgSize) {
		return false;
	}
	m_dev->skip(5);
	auto msgBuff = ox_malloca(msgSize, char);
	m_dev->read(msgBuff.get(), msgSize);
	const auto [msg, err] = ox::readMC<ox::trace::TraceMsgRcv>(msgBuff.get(), msgSize);
	if (err) [[unlikely]] {
		qDebug().noquote() << "Bad message";
		return true;
	}
	addTraceEvent(msg);
	return true;
}

void DataFeed::endFeed() {
	disconnect(m_dev, &QIODevice::readyRead, this, &DataFeed::read);
	emit feedEnd(m_dev);
	m_dev = nullptr;
}

static ProcessData::ChannelEntry &chEntry(auto *list, ChId id) {
	if (id >= list->size()) {
		list->resize(id + 1);
	}
	return (*list)[id];
}

void DataFeed::addTraceEvent(TraceEvent teSrc) {
	const auto &te = m_procData->traceEvents.emplace_back(std::move(teSrc));
	auto &ce = chEntry(&m_procData->channels, te._channel);
	if (!ce.present) {
		ce.present = true;
		emit m_procData->channelAdded(te._channel);
	}
	emit m_procData->channelInc(te._channel);
	emit m_procData->traceEvent(m_procData->traceEvents.size() - 1);
}


LogServer::LogServer() {
	m_server->listen(QHostAddress::Any, 5590);
	connect(m_server, &QTcpServer::newConnection, this, &LogServer::handleConnection);
}

void LogServer::handleConnection() {
	auto conn = m_server->nextPendingConnection();
	connect(conn, &QAbstractSocket::disconnected, conn, &QObject::deleteLater);
	connect(this, &QObject::destroyed, conn, &QObject::deleteLater);
	auto feed = new DataFeed(conn);
	connect(feed, &DataFeed::feedEnd, this, &LogServer::setupDataFeed);
	emit newDataFeed(feed);
}

void LogServer::setupDataFeed(QIODevice *conn) {
	auto feed = new DataFeed(conn, true);
	connect(feed, &DataFeed::feedEnd, this, &LogServer::setupDataFeed);
	emit newDataFeed(feed);
}
