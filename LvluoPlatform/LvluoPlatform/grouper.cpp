#include "grouper.h"
#include <algorithm>

Grouper::Grouper()
{
	
}

Grouper::~Grouper()
{

}

void Grouper::group(const std::vector<ISObj *> &sObjs)
{
	std::vector<ISObj *> newGroup = regroup(sObjs);
	groups.push_back(newGroup);
}

void Grouper::ungroup(const std::vector<ISObj *> &sObjs)
{
	std::vector<ISObj *> newGroup = regroup(sObjs);
	for (ISObj *sObj : newGroup)
	{
		groups.push_back(std::vector<ISObj *>{sObj});
	}
}

std::vector<ISObj *> Grouper::getGroup(ISObj *sObj)
{
	for (auto iter = groups.begin(); iter != groups.end(); iter++)
	{
		if (std::find(iter->begin(), iter->end(), sObj) != iter->end())
		{
			return *iter;
		}
	}
	return std::vector<ISObj *>();
}

std::vector<std::vector<ISObj *>> &Grouper::getGroups()
{
	return groups;
}

bool Grouper::containSObj(ISObj *sObj)
{
	for (auto iter = groups.begin(); iter != groups.end(); iter++)
	{
		if (std::find(iter->begin(), iter->end(), sObj) != iter->end())
		{
			return true;
		}
	}
	return false;
}

void Grouper::addSObj(ISObj *sObj)
{
	groups.push_back(std::vector<ISObj *>{sObj});
}

void Grouper::clear()
{
	groups.clear();
}

std::vector<ISObj *> Grouper::regroup(const std::vector<ISObj *> &sObjs)
{
	// 1.删除已经被删除的sobj组
	for (auto iter = groups.begin(); iter != groups.end(); )
	{
		if (iter->front() == nullptr)
		{
			iter = groups.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	// 2.重组sobj
	std::vector<ISObj *> newGroup;
	for (ISObj *sObj : sObjs)
	{
		if (std::find(newGroup.begin(), newGroup.end(), sObj) == newGroup.end())
		{
			for (auto iter = groups.begin(); iter != groups.end(); )
			{
				if (std::find(iter->begin(), iter->end(), sObj) != iter->end())
				{
					newGroup.insert(newGroup.end(), iter->begin(), iter->end());
					iter = groups.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}
	}

	return newGroup;
}