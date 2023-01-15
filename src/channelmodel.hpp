/*
 * Copyright 2018 - 2023 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QAbstractItemModel>

#include "processdata.hpp"


enum ChannelColumn {
	ColEnabled = 0,
	ColName,
	ColCount,
	ColChildCount,
	ChannelColumnCnt,
};

class ChannelModel: public QAbstractItemModel {
	Q_OBJECT
	private:
		Channel *m_rootCh = nullptr;
		ProcessData *m_procData = nullptr;

	public:
		ChannelModel() = default;

		[[nodiscard]]
		bool setData(const QModelIndex &index, const QVariant &val, int role) override;

		[[nodiscard]]
		QVariant data(const QModelIndex &index, int role) const override;

		[[nodiscard]]
		Qt::ItemFlags flags(const QModelIndex &index) const override;

		[[nodiscard]]
		QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

		[[nodiscard]]
		QModelIndex index(int row, int column,
		                  const QModelIndex &parent) const override;

		[[nodiscard]]
		QModelIndex parent(const QModelIndex &index) const override;

		[[nodiscard]]
		int rowCount(const QModelIndex &parent) const override;

		[[nodiscard]]
		int columnCount(const QModelIndex &parent) const override;

		void addChannel(ChId chId);

		void incChannel(ChId chId);

		void setProcessData(ProcessData *data);

	private:
		Channel *createChannel(Channel *ch, const QStringList &name, int it = 0);

		Channel *addChannelInternal(ChId chId);

};
