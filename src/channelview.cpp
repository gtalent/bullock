/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QHeaderView>
#include <QLabel>
#include <QTreeView>
#include <QSettings>
#include <QVBoxLayout>

#include "channelview.hpp"

ChannelView::ChannelView(QWidget *parent): QWidget(parent) {
	const auto lyt = new QVBoxLayout(this);
	m_tree = new QTreeView(this);
	m_model = new ChannelModel;
	m_tree->setModel(m_model);
	lyt->addWidget(new QLabel(tr("Channels"), this));
	lyt->addWidget(m_tree);
	m_tree->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	m_tree->setTreePosition(ChannelColumn::ColName);
	readState();
}

ChannelView::~ChannelView() {
	writeState();
}

void ChannelView::logChEntry(ChId chId) {
	++m_channels[chId]->msgCnt;
}

bool ChannelView::channelOn(ChId chId) const {
	return m_channels[chId]->on;
}

Channel *ChannelView::findChannel(const QStringList &path, const ox::Vector<ox::UPtr<Channel>> &channels, int it) {
	if (it >= path.size()) {
		return nullptr;
	}
	const auto &name = path[it];
	for (const auto &ch : channels) {
		if (name == ch->fullName()) {
			if (it == path.size() - 1) {
				return ch.get();
			} else {
				return findChannel(path, ch->children, it + 1);
			}
		}
	}
	return nullptr;
}

void ChannelView::setProcessData(ProcessData *data) {
	m_model->setProcessData(data);
	//m_frameTableModel->clear();
	//m_fieldView->clear();
}

void ChannelView::readState() {
	QSettings settings;
	settings.beginGroup("Logger.ChannelView");
	m_tree->header()->restoreState(settings.value("headers").toByteArray());
	settings.endGroup();
}

void ChannelView::writeState() {
	QSettings settings;
	settings.beginGroup("Logger.ChannelView");
	settings.setValue("headers", m_tree->header()->saveState());
	settings.endGroup();
}
