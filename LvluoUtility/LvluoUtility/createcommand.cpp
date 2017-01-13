#include "createcommand.h"

CreateCommand::CreateCommand(std::vector<SObj *> *sObjs, SObj *createdSObj) : sObjs(sObjs)
{
	createdSObjs.push_back(createdSObj);
}

CreateCommand::CreateCommand(std::vector<SObj *> *sObjs, const std::vector<SObj *> &createdSObjs) : 
	sObjs(sObjs), createdSObjs(createdSObjs)
{

}

CreateCommand::~CreateCommand()
{
	for (SObj *deletedSObj : deletedSObjs)
	{
		delete deletedSObj;
	}
}

void CreateCommand::execute()
{
	sObjs->insert(sObjs->end(), createdSObjs.begin(), createdSObjs.end());
	deletedSObjs.clear();
}

void CreateCommand::unexecute()
{
	for (SObj *createdSObj : createdSObjs)
	{
		for (auto sObjIter = sObjs->begin(); sObjIter != sObjs->end(); )
		{
			if (createdSObj == *sObjIter)
			{
				sObjIter = sObjs->erase(sObjIter);
			}
			else
			{
				sObjIter++;
			}
		}
	}
	deletedSObjs = createdSObjs;
}