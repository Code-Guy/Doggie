#ifndef GROUPCOMMAND_H
#define GROUPCOMMAND_H

#include "icommand.h"
#include "grouper.h"
#include "sobj.h"

class GroupCommand : public ICommand
{
public:
	GroupCommand(Grouper *grouper, const std::vector<SObj *> &groupedSObjs);
	~GroupCommand();

	virtual void execute();
	virtual void unexecute();

private:
	Grouper *grouper;
	std::vector<SObj *> groupedSObjs;
};

#endif // GROUPCOMMAND_H