/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>

#include "traceeventmodel.hpp"

int TraceEventModel::rowCount(const QModelIndex &parent) const {
	return m_traceEvents.size();
}

int TraceEventModel::columnCount(const QModelIndex &parent) const {
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

	if (index.row() >= m_traceEvents.size() || index.row() < 0) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		const auto &te = m_traceEvents[index.row()];

		switch (index.column()) {
			case Column::Channel:
				return te.channel;
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
	beginResetModel();
	m_traceEvents.clear();
	if (m_procData) {
		disconnect(m_procData, &ProcessData::traceEvent, this, &TraceEventModel::addEvent);
	}
	m_procData = data;
	if (m_procData) {
		for (const auto &te : m_procData->traceEvents) {
			m_traceEvents.push_back(te);
		}
		connect(m_procData, &ProcessData::traceEvent, this, &TraceEventModel::addEvent);
	}
	endResetModel();
}

TraceEvent TraceEventModel::traceEvent(int row) {
	if (m_procData) {
		return m_procData->traceEvents[row];
	}
	return {};
}

void TraceEventModel::addEvent(const TraceEvent &event) {
	auto index = m_traceEvents.size();
	beginInsertRows(QModelIndex(), index, index);
	m_traceEvents.push_back(event);
	endInsertRows();
}
