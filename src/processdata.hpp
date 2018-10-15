/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QJsonObject>
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

	TraceEvent(QJsonObject tp = {});

};

struct ProcessData {
	QString cmd;
	QVector<TraceEvent> traceEvents;
};
