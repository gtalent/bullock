/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QVector>
#include <QWidget>

#include "callstackmodel.hpp"
#include "channelmodel.hpp"
#include "traceeventmodel.hpp"

#pragma once

class ChannelView: public QWidget {
	Q_OBJECT

	private:
		int m_selectedChannel = 0;
		class QTreeView *m_tree = nullptr;
		// maps channel ids to the channels
		QVector<Channel*> m_channels;
		QVector<Channel*> m_channelModelRoot;
		QHash<QString, Channel*> m_channelMap;
		ChannelModel *m_model = nullptr;

	public:
		explicit ChannelView(QWidget *parent = nullptr);

		~ChannelView() override;

		void addChannel(ChId chId);

		void logChEntry(ChId chId);

		[[nodiscard]]
		bool channelOn(ChId chId) const;

		[[nodiscard]]
		Channel *findChannel(const QStringList &path, const ox::Vector<ox::UPtr<Channel>> &channels, int it = 0);

		void setProcessData(ProcessData *data);

	private:
		void readState();

		void writeState();

};
