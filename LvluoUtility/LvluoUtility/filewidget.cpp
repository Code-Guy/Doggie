#include "filewidget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QDir>
#include <QDebug>

FileWidget::FileWidget(QString filter, QWidget *parent) : filter(filter), QWidget(parent)
{
	button.setText(tr("..."));
	button.setMaximumWidth(60);

	label.setText(tr("null"));

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(&label);
	mainLayout->addSpacing(8);
	mainLayout->addWidget(&button);

	setLayout(mainLayout);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	connect(&button, SIGNAL(clicked()), this, SLOT(onClicked()));
}

FileWidget::~FileWidget()
{

}

void FileWidget::setFileName(QString fileName)
{
	label.setText(QFileInfo(fileName).fileName());
	emit fileNameChanged(fileName);
}

void FileWidget::onClicked()
{
	fileName = QFileDialog::getOpenFileName(nullptr,
		tr("Open File"), ".", filter);
	if (!fileName.isNull())
	{
		setFileName(fileName);
	}
}