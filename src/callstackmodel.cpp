/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>

#include "callstackmodel.hpp"

int CallStackModel::rowCount(const QModelIndex &parent) const {
	return m_frames.size();
}

int CallStackModel::columnCount(const QModelIndex &parent) const {
	return Column::End;
}

QVariant CallStackModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	if (orientation == Qt::Horizontal) {
		switch (section) {
			case Column::Function:
				return tr("Function");
			case Column::Source:
				return tr("Source");
			default:
				return QVariant();
		}
	}
	return QVariant();
}

QVariant CallStackModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}

	if (index.row() >= m_frames.size() || index.row() < 0) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		const auto &f = m_frames[index.row()];

		switch (index.column()) {
			case Column::Function:
				return f.function;
			case Column::Source:
				return QString("%1:%2").arg(f.file).arg(f.line);
			default:
				return {};
		}
	} else if (role == Qt::ToolTipRole) {
		const auto &f = m_frames[index.row()];

		switch (index.column()) {
			case Column::Function:
				return f.function;
			case Column::Source:
				return QString("%1:%2").arg(f.file).arg(f.line);
			default:
				return {};
		}
	}
	return QVariant();
}

void CallStackModel::clear() {
	beginResetModel();
	m_frames.clear();
	endResetModel();
}

void CallStackModel::setTraceEvent(const TraceEvent &event) {
	beginResetModel();
	m_frames = event.frames;
	endResetModel();
}
