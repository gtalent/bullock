/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>

#include "channelview.hpp"
#include "traceeventmodel.hpp"

TraceEventModel::TraceEventModel(ChannelView *cv) {
	m_channelView = cv;
}

int TraceEventModel::rowCount(const QModelIndex&) const {
	return m_visibleTraceEvents.size();
}

int TraceEventModel::columnCount(const QModelIndex&) const {
	return Column::End;
}

QVariant TraceEventModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	if (orientation == Qt::Horizontal) {
		switch (section) {
			case Column::Channel:
				return tr("Channel");
			case Column::Source:
				return tr("Source");
			case Column::Message:
				return tr("Message");
			default:
				return QVariant();
		}
	}
	return QVariant();
}

QVariant TraceEventModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}

	if (index.row() >= m_visibleTraceEvents.size() || index.row() < 0) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		const auto eventId = m_visibleTraceEvents[index.row()];
		const auto &te = m_procData->traceEvents[eventId];
		switch (index.column()) {
			case Column::Channel:
				return te.channel();
			case Column::Source:
				return QString("%1:%2").arg(te.file()).arg(te.line());
			case Column::Message:
				return te.logMsg;
			default:
				return {};
		}
	}
	return QVariant();
}

void TraceEventModel::setProcessData(ProcessData *data) {
	if (m_procData) {
		disconnect(m_procData, &ProcessData::traceEvent, this, &TraceEventModel::addEvent);
		disconnect(m_procData, &ProcessData::channelToggled, this, &TraceEventModel::resetChannels);
	}
	m_procData = data;
	if (m_procData) {
		connect(m_procData, &ProcessData::traceEvent, this, &TraceEventModel::addEvent);
		connect(m_procData, &ProcessData::channelToggled, this, &TraceEventModel::resetChannels);
		resetChannels();
	}
}

const TraceEvent &TraceEventModel::traceEvent(int row) {
	return m_procData->traceEvents[m_visibleTraceEvents[row]];
}

void TraceEventModel::addEvent(std::size_t idx) {
	const auto &te = m_procData->traceEvents[idx];
	const auto &ch = m_procData->channels[te._channel];
	if (ch.channel->enabled()) {
		addVisibleEvent(idx);
	}
}

void TraceEventModel::addVisibleEvent(std::size_t idx) {
	auto index = m_procData->traceEvents.size();
	beginInsertRows(QModelIndex(), index, index);
	m_visibleTraceEvents.push_back(idx);
	endInsertRows();
}

void TraceEventModel::clearVisibleEvents() {
	m_visibleTraceEvents.clear();
}

void TraceEventModel::resetChannels() {
	beginResetModel();
	m_visibleTraceEvents.clear();
	for (auto i = 0l; i < m_procData->traceEvents.size(); ++i) {
		const auto &te = m_procData->traceEvents[i];
		const auto &ch = m_procData->channels[te._channel];
		if (ch.channel->enabled()) {
			m_visibleTraceEvents.push_back(i);
		}
	}
	endResetModel();
}

