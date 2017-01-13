#ifndef HISTORY_H
#define HISTORY_H

#include <deque>
#include "icommand.h"

#include "lvluoapi.h"

class LVLUO_API History
{
public:
	History();
	~History();

	void execute(ICommand *command, bool commit = false);

	void undo();
	void redo();

	void save();
	void clear();

	bool isDirty();

	static History *getInstance();

private:
	std::deque<ICommand *> undoDeque;
	std::deque<ICommand *> redoDeque;

	bool overflowed; // 是否修改过整个工程
	bool saved; // 是否已经保存过整个工程

	static History *instance;
};

#endif // HISTORY_H