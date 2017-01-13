#ifndef STATECOMMAND_H
#define STATECOMMAND_H

#include "icommand.h"
#include "sobj.h"
#include <map>

#include "lvluoapi.h"

class LVLUO_API StateCommand : public ICommand
{
public:
	StateCommand(const std::map<ISObj *, ISObj *> &lastSnapshots, 
		const std::map<ISObj *, ISObj *> &curSnapshots);
	StateCommand(const std::pair<ISObj *, ISObj *> &lastSnapshot,
		const std::pair<ISObj *, ISObj *> &curSnapshot);
	~StateCommand();

	virtual void execute();
	virtual void unexecute();

private:
	std::map<ISObj *, ISObj *> lastSnapshots; // 上次快照
	std::map<ISObj *, ISObj *> curSnapshots; // 当前快照
};

#endif // STATECOMMAND_H
