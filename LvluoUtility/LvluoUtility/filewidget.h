#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "lvluoapi.h"

class LVLUO_API FileWidget : public QWidget
{
	Q_OBJECT

public:
	FileWidget(QString filter, QWidget *parent = Q_NULLPTR);
	~FileWidget();

	void setFileName(QString fileName);

signals:
	void fileNameChanged(QString fileName);

private slots:
	void onClicked();

private:
	QString fileName;
	QString filter;

	QLabel label;
	QPushButton button;
};

#endif // FILEWIDGET_H
