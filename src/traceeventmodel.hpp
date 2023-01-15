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


class TraceEventModel: public QAbstractTableModel {
	Q_OBJECT

	public:
		enum Column {
			Channel = 0,
			Source,
			Message,
			End
		};

	private:
		using TraceEventIdx = int;
		QVector<TraceEventIdx> m_visibleTraceEvents;
		ProcessData *m_procData = nullptr;
		class ChannelView *m_channelView = nullptr;

	public:
		explicit TraceEventModel(class ChannelView *cv);

		int rowCount(const QModelIndex &parent = QModelIndex()) const override ;

		int columnCount(const QModelIndex &parent = QModelIndex()) const override;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

		void setProcessData(ProcessData *data);

		[[nodiscard]]
		const TraceEvent &traceEvent(int row);

	public slots:
		void addEvent(std::size_t idx);

	private:
		void addVisibleEvent(std::size_t idx);

		void clearVisibleEvents();

		void resetChannels();

};
