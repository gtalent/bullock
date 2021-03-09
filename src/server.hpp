/*
 * Copyright 2018 gtalent2@gmail.com
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
		std::unique_ptr<QIODevice> m_dev;

	public:
		DataFeed(QIODevice *dev, bool skipInit = false);

		const QSharedPointer<ProcessData> &procData();

	public slots:
		void handleInit();

		void read();

	signals:
		void feedEnd(QIODevice*);

};

class LogServer: public QObject {
	Q_OBJECT

	private:
		QPointer<QTcpServer> m_server = new QTcpServer(this);

	public:
		LogServer();

	public slots:
		void handleConnection();

		void setupDataFeed(QIODevice *conn);

	signals:
		void newDataFeed(DataFeed*);

};
