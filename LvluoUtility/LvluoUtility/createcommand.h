#ifndef CREATECOMMAND_H
#define CREATECOMMAND_H

#include "icommand.h"
#include "sobj.h"

class CreateCommand : public ICommand
{
public:
	CreateCommand(std::vector<SObj *> *sObjs, SObj *createdSObj);
	CreateCommand(std::vector<SObj *> *sObjs, const std::vector<SObj *> &createdSObjs);
	~CreateCommand();

	virtual void execute();
	virtual void unexecute();

private:
	std::vector<SObj *> createdSObjs;
	std::vector<SObj *> deletedSObjs;

	std::vector<SObj *> *sObjs; // Stimulus Objects
};

#endif // CREATECOMMAND_H
