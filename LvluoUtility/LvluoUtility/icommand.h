#ifndef ICOMMAND_H
#define ICOMMAND_H

#include "lvluoapi.h"

class LVLUO_API ICommand // command�ӿ�
{
public:
	virtual void execute() = 0;
	virtual void unexecute() = 0;
};

#endif // ICOMMAND_H