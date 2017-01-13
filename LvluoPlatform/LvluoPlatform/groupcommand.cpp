#include "groupcommand.h"

GroupCommand::GroupCommand(Grouper *grouper, const std::vector<ISObj *> &groupedSObjs) : 
	grouper(grouper), groupedSObjs(groupedSObjs)
{

}

GroupCommand::~GroupCommand()
{

}

void GroupCommand::execute()
{
	grouper->group(groupedSObjs);
}

void GroupCommand::unexecute()
{
	grouper->ungroup(groupedSObjs);
}