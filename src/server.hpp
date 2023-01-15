/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include <QJsonDocument>
#include <QPointer>
#include <QSharedPointer>
#include <QTcpServer>
#include <QTcpSocket>

#include "processdata.hpp"

class DataFeed: public QObject {
	Q_OBJECT

	private:
		QSharedPointer<ProcessData> m_procData = QSharedPointer<ProcessData>(new ProcessData);
		QIODevice *m_dev = nullptr;

	public:
		/**
		 * Constructor
		 * @param dev typically a TCP connection, but could be any QIODevice
		 * @param skipInit indicates that the feed should not expect an init message
		 */
		explicit DataFeed(QIODevice *dev, bool skipInit = false);

		const QSharedPointer<ProcessData> &procData();

		void handleInit();

		void read();

	private:
		void handleMcTraceEvent();

		void endFeed();

		void addTraceEvent(TraceEvent teSrc);

	signals:
		void feedEnd(QIODevice*);

};

class LogServer: public QObject {
	Q_OBJECT

	private:
		QPointer<QTcpServer> m_server = new QTcpServer(this);

	public:
		LogServer();

		void handleConnection();

		void setupDataFeed(QIODevice *conn);

	signals:
		void newDataFeed(DataFeed*);

};
