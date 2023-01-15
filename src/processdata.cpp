/*
 * Copyright 2018 - 2023 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ox/std/assert.hpp>
#include <ox/std/algorithm.hpp>

#include <QHash>
#include <QJsonArray>
#include <QSettings>

#include "processdata.hpp"

static ChId channelIdIt = 0;
static QHash<QString, ChId> channelToId;
struct ChannelData {
	QString fullName;
	QString name;
	ChId parent = -1;
	ChannelData(const QString &pFullName, ChId pParent) {
		fullName = pFullName;
		parent = pParent;
		const auto lastSep = fullName.lastIndexOf(ChannelSplitter);
		if (lastSep < 0) {
			name = fullName;
		} else {
			name = fullName.mid(lastSep + ox_strlen(ChannelSplitter));
		}
	}
};
static QVector<ChannelData> idToChannel = {{"", -1}};

static void addChannel(const QString &ch, ChId parent = -1) {
	const auto chPath = ch.split(ChannelSplitter);
	if (chPath.empty()) {
		return;
	}
	const auto add = [](const QString &c, ChId parent) {
		if (!channelToId.contains(c)) {
			channelToId.emplace(c, ++channelIdIt);
			idToChannel.emplace_back(c, parent);
		}
		return channelToId[c];
	};
	auto current = chPath.first();
	add(current, parent);
	for (auto i = 1; i < chPath.size(); ++i) {
		const auto &segment = chPath[i];
		current += ChannelSplitter + segment;
		parent = add(current, parent);
	}
}

[[nodiscard]]
const QString &getChannelFullName(ChId id) {
	return idToChannel[id].fullName;
}

[[nodiscard]]
const QString &getChannelName(ChId id) {
	return idToChannel[id].name;
}

[[nodiscard]]
ChId getChannelId(const QString &ch) {
	if (!channelToId.contains(ch)) [[unlikely]] {
		addChannel(ch);
		return channelIdIt;
	}
	return channelToId.value(ch);
}

[[nodiscard]]
ChId getChannelParentId(ChId id) {
	return idToChannel[id].parent;
}


Channel::Channel(ChId pId, Channel *pParent) {
	id = pId;
	parent = pParent;
	QSettings settings;
	if (!fullName().isEmpty()) {
		settings.beginGroup("Bullock.Channel.Enabled");
		on = settings.value(fullName(), true).toBool();
		settings.endGroup();
	}
}

int Channel::childrenMsgCnt() const noexcept {
	auto out = 0;
	for (const auto &c : children) {
		out += c->msgCnt + c->childrenMsgCnt();
	}
	return out;
}

bool Channel::showMsgs() const noexcept {
	return this->on && (!parent || parent->showMsgs());
}

const QString &Channel::name() const {
	return getChannelName(id);
}

const QString &Channel::fullName() const {
	return getChannelFullName(id);
}

int Channel::row() const noexcept {
	if (!parent) [[unlikely]] {
		return 0;
	}
	for (auto i = 0; const auto &c : parent->children) {
		if (c.get() == this) {
			return i;
		}
		++i;
	}
	return -1;
}

void Channel::setEnabled(bool val) noexcept {
	on = val;
	QSettings settings;
	settings.beginGroup("Bullock.Channel.Enabled");
	settings.setValue(fullName(), val);
	settings.endGroup();
}

[[nodiscard]]
bool Channel::enabled() const noexcept {
	return (parent == nullptr || parent->enabled()) && on;
}


Field::Field(const QJsonObject &field) {
	this->name = field["name"].toString();
	this->type = field["type"].toString();
	this->value = field["value"].toString();
	const auto fields = field["fields"].toArray();
	for (const auto &field : fields) {
		this->fields.emplace_back(field.toObject());
	}
}

Frame::Frame(const QJsonObject &frame) {
	this->arch = frame["arch"].toString();
	this->function = frame["function"].toString();
	this->file = frame["file"].toString();
	this->line = frame["line"].toDouble();
	const auto fields = frame["fields"].toArray();
	for (const auto &field : fields) {
		this->fields.emplace_back(field.toObject());
	}
}


TraceEvent::TraceEvent(const QJsonObject &tp) {
	this->_channel = getChannelId(tp["channel"].toString());
	this->logMsg = tp["log_msg"].toString();
	this->_file = tp["file"].toString();
	this->_line = tp["line"].toInt();
	const auto frames = tp["frames"].toArray();
	for (const auto &frame : frames) {
		this->frames.emplace_back(frame.toObject());
	}
}

TraceEvent::TraceEvent(const ox::trace::TraceMsgRcv &tm) {
	this->_channel = getChannelId(tm.ch.c_str());
	this->logMsg = tm.msg.c_str();
	this->_file = tm.file.c_str();
	this->_line = tm.line;
	this->time = tm.time;
}

const QString &TraceEvent::channel() const noexcept {
	return idToChannel[_channel].fullName;
}

const QString &TraceEvent::file() const noexcept {
	return this->_file;
}

int TraceEvent::line() const noexcept {
	return this->_line;
}

