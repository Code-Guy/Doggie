#include "createcommand.h"

CreateCommand::CreateCommand(std::vector<ISObj *> *sObjs, ISObj *createdSObj) : sObjs(sObjs)
{
	createdSObjs.push_back(createdSObj);
}

CreateCommand::CreateCommand(std::vector<ISObj *> *sObjs, const std::vector<ISObj *> &createdSObjs) : 
	sObjs(sObjs), createdSObjs(createdSObjs)
{

}

CreateCommand::~CreateCommand()
{
	for (ISObj *deletedSObj : deletedSObjs)
	{
		deletedSObj->destroy();
	}
}

void CreateCommand::execute()
{
	sObjs->insert(sObjs->end(), createdSObjs.begin(), createdSObjs.end());
	deletedSObjs.clear();
}

void CreateCommand::unexecute()
{
	for (ISObj *createdSObj : createdSObjs)
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