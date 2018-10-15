/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>

#include "server.hpp"

DataFeed::DataFeed(QIODevice *dev): QObject(dev) {
	m_dev = dev;
	connect(m_dev, &QIODevice::readyRead, this, &DataFeed::handleInit);
}

QSharedPointer<ProcessData> DataFeed::procData() {
	return m_procData;
}

void DataFeed::handleInit() {
	auto doc = QJsonDocument::fromJson(m_dev->readLine());
	if (doc.isObject()) {
		auto msg = doc.object();
		if (msg["type"].toString() == "Init") {
			disconnect(m_dev, &QIODevice::readyRead, this, &DataFeed::handleInit);
			connect(m_dev, &QIODevice::readyRead, this, &DataFeed::read);
		}
	}
}

void DataFeed::read() {
	while (m_dev->bytesAvailable()) {
		auto doc = QJsonDocument::fromJson(m_dev->readLine());
		if (m_procData) {
			m_procData->traceEvents.push_back(doc.object());
		}
	}
}


LogServer::LogServer() {
	m_server->listen(QHostAddress::LocalHost, 5590);
	connect(m_server, &QTcpServer::newConnection, this, &LogServer::handleConnection);
}

void LogServer::handleConnection() {
	auto conn = m_server->nextPendingConnection();
	connect(conn, &QAbstractSocket::disconnected, conn, &QObject::deleteLater);
	connect(this, &QObject::destroyed, conn, &QObject::deleteLater);
	emit newDataFeed(new DataFeed(conn));
}
