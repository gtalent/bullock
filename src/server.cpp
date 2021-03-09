/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>

#include "server.hpp"

DataFeed::DataFeed(QIODevice *dev, bool skipInit): QObject(dev) {
	m_dev = std::unique_ptr<QIODevice>(dev);
	if (!skipInit) {
		connect(m_dev.get(), &QIODevice::readyRead, this, &DataFeed::handleInit);
	} else {
		connect(m_dev.get(), &QIODevice::readyRead, this, &DataFeed::read);
	}
}

const QSharedPointer<ProcessData> &DataFeed::procData() {
	return m_procData;
}

void DataFeed::handleInit() {
	auto doc = QJsonDocument::fromJson(m_dev->readLine());
	if (doc.isObject()) {
		auto msg = doc.object();
		if (msg["type"].toString() == "Init") {
			disconnect(m_dev.get(), &QIODevice::readyRead, this, &DataFeed::handleInit);
			connect(m_dev.get(), &QIODevice::readyRead, this, &DataFeed::read);
		}
	}
}

void DataFeed::read() {
	while (m_dev && m_dev->bytesAvailable()) {
		const auto json = m_dev->readLine();
		const auto doc = QJsonDocument::fromJson(json);
		if (m_procData) {
			const auto msg = doc.object();
			if (msg["type"] == "TraceEvent") {
				m_procData->traceEvents.push_back(msg["data"].toObject());
				emit m_procData->traceEvent(m_procData->traceEvents.last());
			} else if (msg["type"] == "Init") {
				emit feedEnd(m_dev.get());
				m_dev.release();
			} else {
				qDebug().noquote() << "Bad message:" << json;
			}
		}
	}
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
