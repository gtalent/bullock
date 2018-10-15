/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QMainWindow>

#include "procselector.hpp"
#include "server.hpp"

class MainWindow: public QMainWindow {
	Q_OBJECT

	private:
		QVector<QSharedPointer<ProcessData>> m_procData;
		ProcessSelector *m_procSelector = nullptr;

	public:
		MainWindow();

		~MainWindow();

	public slots:
		void addDataFeed(DataFeed*);

	private:
		void setupMenu();

};
