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
#include <QSettings>

#include "traceview.hpp"

TraceView::TraceView(QWidget *parent): QWidget(parent) {
	auto lyt = new QHBoxLayout;
	setLayout(lyt);
	m_eventTable = new QTableView(this);
	m_model = new TraceEventModel;
	m_eventTable->setModel(m_model);
	m_eventTable->horizontalHeader()->setStretchLastSection(true);
	m_eventTable->verticalHeader()->hide();
	m_eventTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	lyt->addWidget(m_eventTable);
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
	settings.endGroup();
}

void TraceView::writeState() {
	QSettings settings;
	settings.beginGroup("TraceView");
	settings.setValue("eventTableState", m_eventTable->horizontalHeader()->saveState());
	settings.setValue("eventTableGeometry", m_eventTable->horizontalHeader()->saveGeometry());
	settings.endGroup();
}

void TraceView::setProcessData(ProcessData *data) {
	m_model->setProcessData(data);
}
