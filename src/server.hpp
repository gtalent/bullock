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
		QIODevice *m_dev = nullptr;

	public:
		DataFeed(QIODevice *dev);

		const QSharedPointer<ProcessData> &procData();

	public slots:
		void handleInit();

		void read();
};

class LogServer: public QObject {
	Q_OBJECT

	private:
		QPointer<QTcpServer> m_server = new QTcpServer(this);

	public:
		LogServer();

	public slots:
		void handleConnection();

	signals:
		void newDataFeed(DataFeed*);
};
