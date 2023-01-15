/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <ox/std/trace.hpp>

#include <QJsonObject>
#include <QObject>
#include <QStringList>
#include <QVector>

using ChId = int;

constexpr auto ChannelSplitter = "::";

[[nodiscard]]
const QString &getChannelFullName(ChId id);

[[nodiscard]]
const QString &getChannelName(ChId id);

[[nodiscard]]
ChId getChannelId(const QString &ch);

[[nodiscard]]
ChId getChannelParentId(ChId id);

struct Field {
	QString name;
	QString type;
	QString value;
	QVector<Field> fields;

	Field(const QJsonObject &field = {});

};

struct Frame {
	QString function;
	QString arch;
	QString file;
	int line = 0;
	QVector<Field> fields;

	Frame(const QJsonObject &field = {});

};

struct TraceEvent {
	uint64_t time = 0;
	int _channel = 0;
	QString logMsg;
	QVector<Frame> frames;
	QString _file;
	int _line = 0;

	TraceEvent() = default;

	TraceEvent(const QJsonObject &tp);

	TraceEvent(const ox::trace::TraceMsgRcv &tm);

	[[nodiscard]]
	const QString &channel() const noexcept;

	[[nodiscard]]
	const QString &file() const noexcept;

	[[nodiscard]]
	int line() const noexcept;

};

struct Channel {
	Channel *parent = nullptr;
	ChId id = -1;
	int msgCnt = 0;
	bool on = true;
	ox::Vector<ox::UPtr<Channel>> children;

	Channel(ChId pId, Channel *pParent);

	[[nodiscard]]
	int childrenMsgCnt() const noexcept;

	[[nodiscard]]
	bool showMsgs() const noexcept;

	[[nodiscard]]
	const QString &name() const;

	[[nodiscard]]
	const QString &fullName() const;

	[[nodiscard]]
	int row() const noexcept;

	void setEnabled(bool val) noexcept;

	[[nodiscard]]
	bool enabled() const noexcept;

};

struct ProcessData: public QObject {
	Q_OBJECT
	public:
		struct ChannelEntry {
			// the initialization of channel should not be used to track whether
			// or not a process has yet received a given channel
			bool present = false;
			Channel *channel = nullptr;
		};

		QString procKey;
		QVector<TraceEvent> traceEvents;
		QVector<ChannelEntry> channels;
		Channel rootChannel = Channel(0, nullptr);

	signals:
		/**
		 * Emitted whenever a new TraceEvent is added.
		 * Emits the index of the TraceEvent
		 */
		void traceEvent(std::size_t);

		void channelAdded(int);
		void channelInc(int);
		void channelToggled();
};
