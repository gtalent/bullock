/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QMainWindow>
#include <QSplitter>

#include "processselector.hpp"
#include "traceview.hpp"

#include "server.hpp"

class MainWindow: public QMainWindow {
	Q_OBJECT

	private:
		QHash<QString, QSharedPointer<ProcessData>> m_procData;
		QSplitter *m_splitter = nullptr;
		ProcessData *m_currentProc = nullptr;
		ProcessSelector *m_procSelector = nullptr;
		TraceView *m_traceView = nullptr;
		ChannelView *m_channelView = nullptr;

	public:
		MainWindow();

		~MainWindow();

	public slots:
		void addDataFeed(DataFeed*);

	private:
		void readState();

		void writeState();

	private slots:
		void setProcess(QString procKey);

	private:
		void setupMenu();

};
