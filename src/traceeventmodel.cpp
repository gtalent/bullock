/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "traceeventmodel.hpp"

int TraceEventModel::rowCount(const QModelIndex &parent) const {
	if (m_procData) {
		return m_procData->traceEvents.size();
	} else {
		return 0;
	}
}

int TraceEventModel::columnCount(const QModelIndex &parent) const {
	return Column::End;
}

QVariant TraceEventModel::data(const QModelIndex &index, int role) const {
	return {};
}
