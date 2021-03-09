/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QListWidget>
#include <QWidget>

class ProcessSelector: public QWidget {
	Q_OBJECT

	private:
		enum Rows {
			RowLatest = 0
		};

		QListWidget *m_list = new QListWidget(this);

	public:
		ProcessSelector(QWidget *parent = nullptr);

	public slots:
		void addProcess(QString procKey);

	signals:
		void selectionChanged(QString procKey);

};
