/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "processdata.hpp"

#include <QAbstractItemModel>


class TraceEventModel: public QAbstractItemModel {
	Q_OBJECT

	private:
		enum Column {
			Channel,
			Message,
			End
		};

		ProcessData *m_procData = nullptr;

	public:
		int rowCount(const QModelIndex &parent = QModelIndex()) const override ;

		int columnCount(const QModelIndex &parent = QModelIndex()) const override;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};
