/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QListWidget>
#include <QSettings>
#include <QTableView>

#include "traceview.hpp"

TraceView::TraceView(QWidget *parent): QWidget(parent) {
	auto lyt = new QHBoxLayout;
	setLayout(lyt);

	m_splitter = new QSplitter(Qt::Vertical, this);
	lyt->addWidget(m_splitter);

	m_eventTable = new QTableView(this);
	m_model = new TraceEventModel;
	m_eventTable->setModel(m_model);
	m_eventTable->horizontalHeader()->setStretchLastSection(true);
	m_eventTable->verticalHeader()->hide();
	m_eventTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_eventTable->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(m_eventTable->selectionModel(), &QItemSelectionModel::selectionChanged,
	        [this](const QItemSelection &selected, const QItemSelection &deselected) {
		auto indexes = selected.indexes();
		if (indexes.size()) {
			auto row = indexes[0].row();
			const auto te = m_model->traceEvent(row);
			m_callStackModel->setTraceEvent(te);
		}
	});
	m_splitter->addWidget(m_eventTable);

	m_callStack = new QTableView(this);
	m_callStackModel = new CallStackModel;
	m_callStack->setModel(m_callStackModel);
	m_callStack->horizontalHeader()->setStretchLastSection(true);
	m_callStack->verticalHeader()->hide();
	m_callStack->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_callStack->setSelectionMode(QAbstractItemView::SingleSelection);
	m_splitter->addWidget(m_callStack);

	m_splitter->setStretchFactor(0, 2);

	readState();
}

TraceView::~TraceView() {
	writeState();
}

void TraceView::readState() {
	QSettings settings;
	settings.beginGroup("TraceView");
	m_eventTable->horizontalHeader()->restoreState(settings.value("eventTableState").toByteArray());
	m_eventTable->horizontalHeader()->restoreGeometry(settings.value("eventTableGeometry").toByteArray());
	m_callStack->horizontalHeader()->restoreState(settings.value("callStackTableState").toByteArray());
	m_callStack->horizontalHeader()->restoreGeometry(settings.value("callStackTableGeometry").toByteArray());
	m_splitter->restoreState(settings.value("splitterState").toByteArray());
	settings.endGroup();
}

void TraceView::writeState() {
	QSettings settings;
	settings.beginGroup("TraceView");
	settings.setValue("eventTableState", m_eventTable->horizontalHeader()->saveState());
	settings.setValue("eventTableGeometry", m_eventTable->horizontalHeader()->saveGeometry());
	settings.setValue("callStackTableState", m_callStack->horizontalHeader()->saveState());
	settings.setValue("callStackTableGeometry", m_callStack->horizontalHeader()->saveGeometry());
	settings.setValue("splitterState", m_splitter->saveState());
	settings.endGroup();
}

void TraceView::setProcessData(ProcessData *data) {
	m_model->setProcessData(data);
	m_callStackModel->clear();
}
