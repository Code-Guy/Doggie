#include "ungroupcommand.h"

UngroupCommand::UngroupCommand(Grouper *grouper, const std::vector<ISObj *> &groupedSObjs) : 
	grouper(grouper), groupedSObjs(groupedSObjs)
{

}

UngroupCommand::~UngroupCommand()
{

}

void UngroupCommand::execute()
{
	grouper->ungroup(groupedSObjs);
}

void UngroupCommand::unexecute()
{
	grouper->group(groupedSObjs);
}