/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QJsonObject>
#include <QObject>
#include <QStringList>
#include <QVector>

struct Field {
	QString name;
	QString type;
	QString value;
	QVector<Field> fields;

	Field(QJsonObject field = {});

};

struct Frame {
	QString function;
	QString arch;
	QString file;
	int line = 0;
	QVector<Field> fields;

	Frame(QJsonObject frame = {});

};

struct TraceEvent {
	QString channel;
	QString logMsg;
	QVector<Frame> frames;
	QString _file;
	int _line;

	TraceEvent(QJsonObject tp = {});

	QString file() const;

	int line() const;

};

struct ProcessData: public QObject {
	Q_OBJECT

	public:
		QString procKey;
		QVector<TraceEvent> traceEvents;

	signals:
		/**
		 * Emitted whenever a new TraceEvent is added.
		 */
		void traceEvent(const TraceEvent&);
};
