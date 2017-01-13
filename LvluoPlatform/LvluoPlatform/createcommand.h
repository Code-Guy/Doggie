#ifndef CREATECOMMAND_H
#define CREATECOMMAND_H

#include "icommand.h"
#include "sobj.h"

class CreateCommand : public ICommand
{
public:
	CreateCommand(std::vector<ISObj *> *sObjs, ISObj *createdSObj);
	CreateCommand(std::vector<ISObj *> *sObjs, const std::vector<ISObj *> &createdSObjs);
	~CreateCommand();

	virtual void execute();
	virtual void unexecute();

private:
	std::vector<ISObj *> createdSObjs;
	std::vector<ISObj *> deletedSObjs;

	std::vector<ISObj *> *sObjs; // Stimulus Objects
};

#endif // CREATECOMMAND_H
