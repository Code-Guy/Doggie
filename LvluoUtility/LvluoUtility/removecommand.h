#ifndef REMOVECOMMAND_H
#define REMOVECOMMAND_H

#include "icommand.h"
#include "sobj.h"

class RemoveCommand : public ICommand
{
public:
	RemoveCommand(std::vector<SObj *> *sObjs, SObj *removedSObj);
	RemoveCommand(std::vector<SObj *> *sObjs, const std::vector<SObj *> &removedSObjs);
	~RemoveCommand();

	virtual void execute();
	virtual void unexecute();

private:
	std::vector<SObj *> removedSObjs;
	std::vector<SObj *> deletedSObjs;

	std::vector<SObj *> *sObjs; // Stimulus Objects
};

#endif // REMOVECOMMAND_H
