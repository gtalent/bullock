/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>

#include "processselector.hpp"

ProcessSelector::ProcessSelector(QWidget *parent): QWidget(parent) {
	const auto MostRecent = tr("Most Recent");
	auto lyt = new QVBoxLayout(this);
	lyt->addWidget(new QLabel(tr("Processes"), this));
	lyt->addWidget(m_list);
	m_list->addItem(MostRecent);
	m_list->setCurrentRow(RowLatest);
	connect(m_list, &QListWidget::currentTextChanged, [this, MostRecent](QString txt) {
		if (txt == MostRecent && m_list->count() > 1) {
			txt = m_list->item(1)->text();
		}
		emit this->selectionChanged(txt);
	});
}

void ProcessSelector::addProcess(QString procKey) {
	m_list->insertItem(1, procKey);
	if (m_list->currentRow() == RowLatest) {
		emit selectionChanged(procKey);
	}
}
