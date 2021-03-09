/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "processdata.hpp"

#include <QAbstractTableModel>


class CallStackModel: public QAbstractTableModel {
	Q_OBJECT

	public:
		enum Column {
			Function = 0,
			Source,
			End
		};

	private:
		QVector<Frame> m_frames;

	public:
		int rowCount(const QModelIndex &parent = QModelIndex()) const override ;

		int columnCount(const QModelIndex &parent = QModelIndex()) const override;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

		void clear();

	public slots:
		void setTraceEvent(const TraceEvent &event);

};
