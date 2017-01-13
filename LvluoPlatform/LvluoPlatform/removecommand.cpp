#include "removecommand.h"

RemoveCommand::RemoveCommand(std::vector<ISObj *> *sObjs, ISObj *removedSObj) : sObjs(sObjs)
{
	removedSObjs.push_back(removedSObj);
}

RemoveCommand::RemoveCommand(std::vector<ISObj *> *sObjs, const std::vector<ISObj *> &removedSObjs) : 
	sObjs(sObjs), removedSObjs(removedSObjs)
{

}

RemoveCommand::~RemoveCommand()
{
	for (ISObj *deletedSObj : deletedSObjs)
	{
		deletedSObj->destroy();
	}
}

void RemoveCommand::execute()
{
	for (ISObj *removedSObj : removedSObjs)
	{
		for (auto sObjIter = sObjs->begin(); sObjIter != sObjs->end(); )
		{
			if (removedSObj == *sObjIter)
			{
				sObjIter = sObjs->erase(sObjIter);
			}
			else
			{
				sObjIter++;
			}
		}
	}
	deletedSObjs = removedSObjs;
}

void RemoveCommand::unexecute()
{
	sObjs->insert(sObjs->end(), removedSObjs.begin(), removedSObjs.end());
	deletedSObjs.clear();
}