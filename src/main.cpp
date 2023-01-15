/*
 * Copyright 2018 - 2021 gary@drinkingtea.net
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QApplication>

#include "server.hpp"
#include "mainwindow.hpp"

int main(int argc, char **args) {
	QApplication app(argc, args);
	QApplication::setApplicationName("Bullock");
	QApplication::setOrganizationName("DrinkingTea");
	QApplication::setOrganizationDomain("drinkingtea.net");

	LogServer server;

	MainWindow w;
	w.show();

	QObject::connect(&server, &LogServer::newDataFeed, &w, &MainWindow::addDataFeed);

	return QApplication::exec();
}
