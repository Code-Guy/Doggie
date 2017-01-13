#ifndef GROUPER_H
#define GROUPER_H

#include "sobj.h"

class Grouper
{
public:
	Grouper();
	~Grouper();

	void group(const std::vector<ISObj *> &sObjs);
	void ungroup(const std::vector<ISObj *> &sObjs);

	std::vector<ISObj *> getGroup(ISObj *sObj);
	std::vector<std::vector<ISObj *>> &getGroups();

	bool containSObj(ISObj *sObj);
	void addSObj(ISObj *sObj);

	void clear();

private:
	std::vector<ISObj *> regroup(const std::vector<ISObj *> &sObjs);

	std::vector<std::vector<ISObj *>> groups;
};

#endif // GROUPER_H
