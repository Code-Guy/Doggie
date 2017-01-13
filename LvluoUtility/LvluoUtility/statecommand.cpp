#include "statecommand.h"

StateCommand::StateCommand(const std::map<ISObj *, ISObj *> &lastSnapshots, 
	const std::map<ISObj *, ISObj *> &curSnapshots) : 
	lastSnapshots(lastSnapshots), curSnapshots(curSnapshots)
{

}

StateCommand::StateCommand(const std::pair<ISObj *, ISObj *> &lastSnapshot,
	const std::pair<ISObj *, ISObj *> &curSnapshot)
{
	lastSnapshots.insert(lastSnapshot);
	curSnapshots.insert(curSnapshot);
}

StateCommand::~StateCommand()
{
	for (auto iter : curSnapshots)
	{
		delete iter.second;
	}
	for (auto iter : lastSnapshots)
	{
		delete iter.second;
	}
}

void StateCommand::execute()
{
	for (auto iter : curSnapshots)
	{
		iter.first->setSnapshot(iter.second);
	}
}

void StateCommand::unexecute()
{
	for (auto iter : lastSnapshots)
	{
		iter.first->setSnapshot(iter.second);
	}
}