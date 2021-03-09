/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QSplitter>
#include <QTableView>
#include <QTreeWidget>
#include <QWidget>

#include "callstackmodel.hpp"
#include "traceeventmodel.hpp"

#include "processdata.hpp"

class TraceView: public QWidget {
	Q_OBJECT

	private:
		int m_selectedEvent = 0;
		QTableView *m_eventTable = nullptr;
		QTableView *m_frameTable = nullptr;
		QTreeWidget *m_fieldView = nullptr;
		QSplitter *m_splitter = nullptr;
		QSplitter *m_lowerSplitter = nullptr;
		CallStackModel *m_frameTableModel = nullptr;
		TraceEventModel *m_model = nullptr;

	public:
		TraceView(QWidget *parent = nullptr);

		~TraceView() override;

		void setProcessData(ProcessData *data);

	private:
		void readState();

		void writeState();

	private slots:
		void handleFrameSelection(const QItemSelection &selected, const QItemSelection &deselected);

};
