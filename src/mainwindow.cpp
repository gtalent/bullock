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
#include <QSettings>
#include <QSplitter>

#include "mainwindow.hpp"

MainWindow::MainWindow() {
	setWindowTitle(tr("Bullock"));

	auto screenSize = QApplication::desktop()->screenGeometry();
	constexpr auto sizePct = 0.85;
	resize(screenSize.width() * sizePct, screenSize.height() * sizePct);
	move(-x(), -y());
	move(screenSize.width() * (1 - sizePct) / 2, screenSize.height() * (1 - sizePct) / 2);

	auto central = new QWidget(this);
	m_splitter = new QSplitter(central);

	auto leftPane = new QWidget(this);
	auto leftPaneSplitter = new QSplitter(Qt::Vertical, leftPane);
	m_procSelector = new ProcessSelector(leftPaneSplitter);
	connect(m_procSelector, &ProcessSelector::selectionChanged, this, &MainWindow::setProcess);
	leftPaneSplitter->addWidget(m_procSelector);

	m_splitter->addWidget(leftPaneSplitter);
	m_traceView = new TraceView(m_splitter);
	m_splitter->addWidget(m_traceView);
	m_splitter->setStretchFactor(1, 3);

	setCentralWidget(m_splitter);
	setupMenu();

	readState();
}

MainWindow::~MainWindow() {
	writeState();
}

void MainWindow::addDataFeed(DataFeed *feed) {
	auto &procKey = feed->procData()->procKey;
	if (procKey == "") {
		procKey = QDateTime::currentDateTime().toString();
	}
	m_procData[procKey] = feed->procData();
	m_procSelector->addProcess(procKey);
}

void MainWindow::readState() {
	QSettings settings;
	settings.beginGroup("MainWindow");
	m_splitter->restoreState(settings.value("splitterState").toByteArray());
	settings.endGroup();
}

void MainWindow::writeState() {
	QSettings settings;
	settings.beginGroup("MainWindow");
	settings.setValue("splitterState", m_splitter->saveState());
	settings.endGroup();
}

void MainWindow::setProcess(QString procKey) {
	if (m_procData.contains(procKey)) {
		m_currentProc = m_procData[procKey].data();
	} else {
		m_currentProc = nullptr;
	}
	m_traceView->setProcessData(m_currentProc);
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
