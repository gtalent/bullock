/*
 * Copyright 2018 - 2023 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include "channelmodel.hpp"

QVariant ChannelModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case ColEnabled:
				return tr("On");
			case ColName:
				return tr("Name");
			case ColCount:
				return tr("Messages");
			case ColChildCount:
				return tr("Child Messages");
		}
	}
	return {};
}

Qt::ItemFlags ChannelModel::flags(const QModelIndex &index) const {
	if (!index.isValid()) {
		return {};
	}
	auto out = QAbstractItemModel::flags(index);
	switch (index.column()) {
		case ColEnabled:
			out |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
			break;
	}
	return out;
}

bool ChannelModel::setData(const QModelIndex &index, const QVariant &val, int role) {
	const auto item = static_cast<Channel*>(index.internalPointer());
	switch (role) {
		case Qt::EditRole:
			return false;
		case Qt::CheckStateRole:
			switch (index.column()) {
				case ColEnabled: {
					item->setEnabled(val.toBool());
					emit dataChanged(index, index);
					emit m_procData->channelToggled();
					return item->on;
				}
			}
			break;
	}
	return QAbstractItemModel::setData(index, val, role);
}

QVariant ChannelModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return {};
	}
	const auto item = static_cast<Channel*>(index.internalPointer());
	switch (role) {
		case Qt::CheckStateRole:
			switch (index.column()) {
				case ColEnabled:
					return item->on ? Qt::Checked : Qt::Unchecked;
			}
			break;
		case Qt::DisplayRole:
			switch (index.column()) {
				case ColName:
					return item->name();
				case ColCount:
					return item->msgCnt;
				case ColChildCount:
					return item->msgCnt + item->childrenMsgCnt();
			}
	}
	return {};
}

QModelIndex ChannelModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent)) {
		return {};
	}
	const auto parentItem = parent.isValid() ?
		static_cast<Channel*>(parent.internalPointer()) :
		m_rootCh;
	const auto childItem = parentItem->children[row].get();
	if (childItem) {
		return createIndex(row, column, childItem);
	}
	return {};
}

QModelIndex ChannelModel::parent(const QModelIndex &index) const {
	if (!index.isValid()) {
		return {};
	}
	const auto item = static_cast<Channel*>(index.internalPointer());
	const auto parentItem = item->parent;
	if (parentItem == m_rootCh) {
		return {};
	}
	return createIndex(parentItem->row(), 0, parentItem);
}

int ChannelModel::rowCount(const QModelIndex &parent) const {
	Channel *ch = nullptr;
	if (parent.isValid()) {
		ch = static_cast<Channel*>(parent.internalPointer());
	} else {
		ch = m_rootCh;
	}
	if (!ch) {
		return 0;
	}
	return ch->children.size();
}

int ChannelModel::columnCount(const QModelIndex&) const {
	return ChannelColumnCnt;
}

Channel *ChannelModel::createChannel(Channel *ch, const QStringList &name, int it) {
	if (it == name.size()) {
		return ch;
	}
	const auto lvlName = name.sliced(0, it + 1).join(ChannelSplitter);
	auto childIt = std::find_if(ch->children.begin(), ch->children.end(), [&](auto &val) {
		return val->fullName() == lvlName;
	});
	Channel *child = nullptr;
	if (childIt != ch->children.end()) {
		child = (*childIt).get();
	} else {
		const auto childId = getChannelId(lvlName);
		const auto insertPt = std::find_if(ch->children.begin(), ch->children.end(), [lvlName](const auto &b) {
			return b->name() >= lvlName;
		}).offset();
		const auto chRow = ch->row();
		const auto parIdx = m_rootCh == ch ? QModelIndex{} : createIndex(chRow, 0, ch);
		beginInsertRows(parIdx, insertPt, 0);
		child = (*ch->children.emplace(insertPt, ox::make_unique<Channel>(childId, ch))).get();
		m_procData->channels[childId].channel = child;
		endInsertRows();
		//ch->children.empty
		//std::sort(ch->children.begin(), ch->children.end(), [](const auto &a, const auto &b) {
		//	return a->name() < b->name();
		//});
	}
	return createChannel(child, name, it + 1);
}

void ChannelModel::addChannel(ChId chId) {
	addChannelInternal(chId);
}

void ChannelModel::incChannel(ChId chId) {
	++m_procData->channels[chId].channel->msgCnt;
}

void ChannelModel::setProcessData(ProcessData *data) {
	if (m_procData) {
		disconnect(m_procData, &ProcessData::channelAdded, this, &ChannelModel::addChannel);
		disconnect(m_procData, &ProcessData::channelInc, this, &ChannelModel::incChannel);
	}
	m_procData = data;
	beginResetModel();
	m_rootCh = &m_procData->rootChannel;
	if (m_procData) {
		for (auto i = 0l; i < m_procData->traceEvents.size(); ++i) {
			addChannelInternal(m_procData->traceEvents[i]._channel);
		}
		connect(m_procData, &ProcessData::channelAdded, this, &ChannelModel::addChannel);
		connect(m_procData, &ProcessData::channelInc, this, &ChannelModel::incChannel);
	}
	endResetModel();
}

Channel *ChannelModel::addChannelInternal(ChId chId) {
	const auto &chName = getChannelFullName(chId);
	auto namePath = chName.split(ChannelSplitter);
	if (namePath.size() == 0) {
		namePath = {chName};
	}
	return createChannel(m_rootCh, namePath);
}

