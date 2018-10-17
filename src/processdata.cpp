/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDebug>
#include <QJsonArray>

#include "processdata.hpp"

Field::Field(QJsonObject field) {
	this->name = field["name"].toString();
	this->type = field["field_type"].toString();
	this->value = field["value"].toString();
	auto fields = field["fields"].toArray();
	for (auto field : fields) {
		this->fields.push_back(field.toObject());
	}
}

Frame::Frame(QJsonObject frame) {
	this->arch = frame["arch"].toString();
	this->file = frame["file"].toString();
	this->line = frame["line"].toDouble();
	auto fields = frame["fields"].toArray();
	for (auto field : fields) {
		this->fields.push_back(field.toObject());
	}
}

TraceEvent::TraceEvent(QJsonObject tp) {
	this->channel = tp["channel"].toString();
	this->logMsg = tp["log_msg"].toString();
	auto frames = tp["frames"].toArray();
	for (auto frame : frames) {
		this->frames.push_back(frame.toObject());
	}
}

QString TraceEvent::file() const {
	return this->frames[0].file;
}

int TraceEvent::line() const {
	return this->frames[0].line;
}

