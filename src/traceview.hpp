/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QTableView>
#include <QWidget>

#include "traceeventmodel.hpp"

#include "processdata.hpp"

class TraceView: public QWidget {
	Q_OBJECT

	private:
		QTableView *m_eventTable;
		TraceEventModel *m_model = nullptr;

	public:
		TraceView(QWidget *parent = nullptr);

		~TraceView();

		void setProcessData(ProcessData *data);

	private:
		void readState();

		void writeState();

};
