/*
 * Copyright 2018 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QAction>
#include <QApplication>
#include <QDateTime>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSplitter>

#include "traceview.hpp"

#include "mainwindow.hpp"

MainWindow::MainWindow() {
	setWindowTitle(tr("Bullock"));

	auto screenSize = QApplication::desktop()->screenGeometry();
	constexpr auto sizePct = 0.85;
	resize(screenSize.width() * sizePct, screenSize.height() * sizePct);
	move(-x(), -y());
	move(screenSize.width() * (1 - sizePct) / 2, screenSize.height() * (1 - sizePct) / 2);

	auto central = new QWidget(this);
	auto splitter = new QSplitter(central);

	auto leftPane = new QWidget(this);
	auto leftPaneSplitter = new QSplitter(Qt::Vertical, leftPane);
	m_procSelector = new ProcessSelector(leftPaneSplitter);
	leftPaneSplitter->addWidget(m_procSelector);

	splitter->addWidget(leftPaneSplitter);
	splitter->addWidget(new TraceView(splitter));
	splitter->setStretchFactor(1, 3);

	setCentralWidget(splitter);
	setupMenu();
}

MainWindow::~MainWindow() {
}

void MainWindow::addDataFeed(DataFeed *feed) {
	m_procData.push_back(feed->procData());
	auto time = QDateTime::currentDateTime();
	m_procSelector->addProcess(time.toString());
}

void MainWindow::setupMenu() {
	auto menu = menuBar();
	auto fileMenu = menu->addMenu(tr("&File"));
	auto helpMenu = menu->addMenu(tr("&Help"));

	auto quit = fileMenu->addAction(tr("&Quit"));
	quit->setShortcuts(QKeySequence::Quit);
	quit->setStatusTip(tr("Quit application"));
	connect(quit, &QAction::triggered, this, &MainWindow::close);

	helpMenu->addAction(tr("&About"));
}
