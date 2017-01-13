#ifndef RECENTPROJECTACTION_H
#define RECENTPROJECTACTION_H

#include <QAction>

class RecentProjectAction : public QAction
{
	Q_OBJECT
public:
	RecentProjectAction(QString fileName, QObject *parent = Q_NULLPTR);
	~RecentProjectAction();

signals:
	void triggered(RecentProjectAction *emitter, QString fileName);

private:
	QString fileName;

private slots:
	void actionTriggered();
};

#endif // RECENTPROJECTACTION_H
