#include "sobjfactory.h"

SObjFactory::ConstructMapType *SObjFactory::createMap = nullptr;
SObjFactory::CopyMapType *SObjFactory::copyMap = nullptr;
SObjFactory::NullMapType *SObjFactory::nullMap = nullptr;
SObjFactory::ObjectNameMapType *SObjFactory::objectNameMap = nullptr;

void SObjFactory::registerType(QString objectName, std::vector<QStringList> idss,
	Constructor constructor, Copyer copyer, Nuller nuller)
{
	for (QStringList ids : idss)
	{
		getObjectNameMap()->insert(std::make_pair(ids, objectName));
	}
	getContructMap()->insert(std::make_pair(objectName, constructor));
	getCopyMap()->insert(std::make_pair(objectName, copyer));
	getNullMap()->insert(std::make_pair(objectName, nuller));
}

ISObj *SObjFactory::create(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	auto iter = createMap->find(objectNameMap->at(ids));
	if (iter == createMap->end())
	{
		return nullptr;
	}
	else
	{
		return iter->second(camera, sceneSize, ids, jsonObject);
	}
}

ISObj *SObjFactory::copy(ISObj *other)
{
	auto iter = copyMap->find(objectNameMap->at(other->getIDs()));
	if (iter == copyMap->end())
	{
		return nullptr;
	}
	else
	{
		return iter->second(other);
	}
}

ISObj *SObjFactory::null(ISObj *other)
{
	auto iter = nullMap->find(objectNameMap->at(other->getIDs()));
	if (iter == nullMap->end())
	{
		return nullptr;
	}
	else
	{
		return iter->second();
	}
}

SObjFactory::ConstructMapType *SObjFactory::getContructMap()
{
	if (!createMap)
	{
		createMap = new SObjFactory::ConstructMapType;
	}
	return createMap;
}

SObjFactory::CopyMapType *SObjFactory::getCopyMap()
{
	if (!copyMap)
	{
		copyMap = new CopyMapType;
	}
	return copyMap;
}

SObjFactory::NullMapType *SObjFactory::getNullMap()
{
	if (!nullMap)
	{
		nullMap = new NullMapType;
	}
	return nullMap;
}

SObjFactory::ObjectNameMapType *SObjFactory::getObjectNameMap()
{
	if (!objectNameMap)
	{
		objectNameMap = new ObjectNameMapType;
	}
	return objectNameMap;
}