#include "history.h"

const int MaxUndoStep = 50; // 最大回退步数

History *History::instance = nullptr;

History::History()
{
	overflowed = false;
	saved = true;
}

History::~History()
{
	
}

void History::execute(ICommand *command, bool commit)
{
	saved = false;
	if (undoDeque.size() >= MaxUndoStep)
	{
		ICommand *removedCommand = undoDeque.front();
		delete removedCommand;
		undoDeque.pop_front();
		overflowed = true;
	}
	if (commit)
	{
		command->execute();
	}
	undoDeque.push_back(command);
	redoDeque.clear();
}

void History::undo()
{
	if (!undoDeque.empty())
	{
		ICommand *command = undoDeque.back();
		command->unexecute();
		undoDeque.pop_back();
		redoDeque.push_back(command);
	}
}

void History::redo()
{
	if (!redoDeque.empty())
	{
		ICommand *command = redoDeque.back();
		command->execute();
		redoDeque.pop_back();
		undoDeque.push_back(command);
	}
}

void History::save()
{
	saved = true;
}

void History::clear()
{
	for (ICommand *undoCommand : undoDeque)
	{
		delete undoCommand;
	}
	for (ICommand *redoCommand : redoDeque)
	{
		delete redoCommand;
	}
	undoDeque.clear();
	redoDeque.clear();
}

bool History::isDirty()
{
	return !saved && (overflowed || !undoDeque.empty());
}

History *History::getInstance()
{
	if (instance == nullptr)
	{
		instance = new History;
	}
	return instance;
}