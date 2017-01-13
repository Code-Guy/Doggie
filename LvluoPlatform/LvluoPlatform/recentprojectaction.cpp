#include "recentprojectaction.h"

RecentProjectAction::RecentProjectAction(QString fileName, QObject *parent) : 
	fileName(fileName), QAction(parent)
{
	setText(fileName);
	connect(this, SIGNAL(triggered()), this, SLOT(actionTriggered()));
}

RecentProjectAction::~RecentProjectAction()
{

}

void RecentProjectAction::actionTriggered()
{
	emit triggered(this, fileName);
}