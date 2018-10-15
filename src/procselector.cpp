
#include <QLabel>
#include <QVBoxLayout>

#include "procselector.hpp"

ProcessSelector::ProcessSelector(QWidget *parent): QWidget(parent) {
	auto lyt = new QVBoxLayout(this);
	lyt->addWidget(new QLabel(tr("Processes"), this));
	lyt->addWidget(m_list);
	m_list->addItem(tr("Most Recent"));
	m_list->setCurrentRow(RowLatest);
}

void ProcessSelector::addProcess(QString procKey) {
	m_list->insertItem(1, procKey);
	if (m_list->currentRow() == RowLatest) {
		emit selectionChanged(procKey);
	}
}
