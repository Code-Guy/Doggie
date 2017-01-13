#ifndef UNGROUPCOMMAND_H
#define UNGROUPCOMMAND_H

#include "icommand.h"
#include "grouper.h"
#include "sobj.h"

class UngroupCommand : public ICommand
{
public:
	UngroupCommand(Grouper *grouper, const std::vector<ISObj *> &groupedSObjs);
	~UngroupCommand();

	virtual void execute();
	virtual void unexecute();

private:
	Grouper *grouper;
	std::vector<ISObj *> groupedSObjs;
};

#endif // UNGROUPCOMMAND_H