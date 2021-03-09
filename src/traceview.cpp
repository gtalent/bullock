/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
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

static QTreeWidgetItem *treeItem(const Field &field, QTreeWidget *treeWidget, QTreeWidgetItem *parent = nullptr) {
	QTreeWidgetItem *item = nullptr;
	if (parent) {
		item = new QTreeWidgetItem(parent);
		parent->addChild(item);
	} else {
		item = new QTreeWidgetItem(treeWidget);
		//treeWidget->addChild(item);
	}
	item->setData(0, Qt::DisplayRole, field.name);
	item->setData(1, Qt::DisplayRole, field.type);
	item->setData(2, Qt::DisplayRole, field.value);
	for (const auto &f : field.fields) {
		treeItem(f, treeWidget, item);
	}
	return item;
}

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
		m_frameTableModel->clear();
		m_fieldView->clear();
		auto indexes = selected.indexes();
		if (indexes.size()) {
			m_selectedEvent = indexes[0].row();
			const auto te = m_model->traceEvent(m_selectedEvent);
			m_frameTableModel->setTraceEvent(te);
		}
	});
	m_splitter->addWidget(m_eventTable);

	// setup lower pane
	m_lowerSplitter = new QSplitter(this);

	// setup stack trace viewer
	m_frameTable = new QTableView(this);
	m_frameTableModel = new CallStackModel;
	m_frameTable->setModel(m_frameTableModel);
	m_frameTable->horizontalHeader()->setStretchLastSection(true);
	m_frameTable->verticalHeader()->hide();
	m_frameTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_frameTable->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(m_frameTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TraceView::handleFrameSelection);
	m_lowerSplitter->addWidget(m_frameTable);

	// setup field viewer
	m_fieldView = new QTreeWidget(this);
	m_fieldView->header()->setStretchLastSection(true);
	m_fieldView->setSelectionMode(QAbstractItemView::NoSelection);
	m_fieldView->setHeaderLabels({tr("Name"), tr("Type"), tr("Value")});
	m_fieldView->setAlternatingRowColors(true);
	m_lowerSplitter->addWidget(m_fieldView);


	m_splitter->addWidget(m_lowerSplitter);
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
	m_frameTable->horizontalHeader()->restoreState(settings.value("frameTableState").toByteArray());
	m_frameTable->horizontalHeader()->restoreGeometry(settings.value("frameTableGeometry").toByteArray());
	m_fieldView->header()->restoreState(settings.value("fieldViewState").toByteArray());
	m_fieldView->header()->restoreGeometry(settings.value("fieldViewGeometry").toByteArray());
	m_splitter->restoreState(settings.value("splitterState").toByteArray());
	m_lowerSplitter->restoreState(settings.value("lowerSplitterState").toByteArray());
	settings.endGroup();
}

void TraceView::writeState() {
	QSettings settings;
	settings.beginGroup("TraceView");
	settings.setValue("eventTableState", m_eventTable->horizontalHeader()->saveState());
	settings.setValue("eventTableGeometry", m_eventTable->horizontalHeader()->saveGeometry());
	settings.setValue("frameTableState", m_frameTable->horizontalHeader()->saveState());
	settings.setValue("frameTableGeometry", m_frameTable->horizontalHeader()->saveGeometry());
	settings.setValue("fieldViewState", m_fieldView->header()->saveState());
	settings.setValue("fieldViewGeometry", m_fieldView->header()->saveGeometry());
	settings.setValue("splitterState", m_splitter->saveState());
	settings.setValue("lowerSplitterState", m_lowerSplitter->saveState());
	settings.endGroup();
}

void TraceView::setProcessData(ProcessData *data) {
	m_model->setProcessData(data);
	m_frameTableModel->clear();
	m_fieldView->clear();
}

void TraceView::handleFrameSelection(const QItemSelection &selected, const QItemSelection &deselected) {
	auto indexes = selected.indexes();
	m_fieldView->clear();
	if (indexes.size()) {
		auto row = indexes[0].row();
		const auto te = m_model->traceEvent(m_selectedEvent);
		for (const auto &field : te.frames[row].fields) {
			treeItem(field, m_fieldView);
		}
	}
}
