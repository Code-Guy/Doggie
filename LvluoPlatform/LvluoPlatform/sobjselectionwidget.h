#ifndef SOBJSELECTIONWIDGET_H
#define SOBJSELECTIONWIDGET_H

#include "sobjicon.h"

class SObjSelectionWidget : public QFrame 
{
	Q_OBJECT

public:
	SObjSelectionWidget(const QString &id, std::vector<SObjIcon *> &sObjIcons, 
		QFrame *parent = Q_NULLPTR);
	~SObjSelectionWidget();

protected:
	virtual void enterEvent(QEvent *evt);
	virtual void closeEvent(QCloseEvent *evt);

private slots:
	void hover(QString id, SObjIcon *emitter);

private:
	QString id;
	QLabel titleLabel;
	QLabel selectionNameLabel;
	std::vector<SObjIcon *> sObjIcons;

	SObjIcon *currentSObjIcon;
};

#endif // SOBJSELECTIONWIDGET_H