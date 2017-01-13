#ifndef REMOVECOMMAND_H
#define REMOVECOMMAND_H

#include "icommand.h"
#include "sobj.h"

class RemoveCommand : public ICommand
{
public:
	RemoveCommand(std::vector<ISObj *> *sObjs, ISObj *removedSObj);
	RemoveCommand(std::vector<ISObj *> *sObjs, const std::vector<ISObj *> &removedSObjs);
	~RemoveCommand();

	virtual void execute();
	virtual void unexecute();

private:
	std::vector<ISObj *> removedSObjs;
	std::vector<ISObj *> deletedSObjs;

	std::vector<ISObj *> *sObjs; // Stimulus Objects
};

#endif // REMOVECOMMAND_H
