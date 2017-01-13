#include "removecommand.h"

RemoveCommand::RemoveCommand(std::vector<SObj *> *sObjs, SObj *removedSObj) : sObjs(sObjs)
{
	removedSObjs.push_back(removedSObj);
}

RemoveCommand::RemoveCommand(std::vector<SObj *> *sObjs, const std::vector<SObj *> &removedSObjs) : 
	sObjs(sObjs), removedSObjs(removedSObjs)
{

}

RemoveCommand::~RemoveCommand()
{
	for (SObj *deletedSObj : deletedSObjs)
	{
		delete deletedSObj;
	}
}

void RemoveCommand::execute()
{
	for (SObj *removedSObj : removedSObjs)
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