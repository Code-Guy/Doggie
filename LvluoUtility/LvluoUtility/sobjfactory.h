#ifndef SOBJFACTORY_H
#define SOBJFACTORY_H

#include <QStringList>
#include <QJsonObject>
#include <map>

#include "sobj.h"
#include "lvluoapi.h"

class LVLUO_API SObjFactory
{
public:
	typedef ISObj*(__cdecl *Constructor)(const Camera *, glm::ivec2 *, QStringList, QJsonObject);
	typedef ISObj*(__cdecl *Copyer)(ISObj *);
	typedef ISObj*(__cdecl *Nuller)();

	typedef std::map<QString, Constructor> ConstructMapType;
	typedef std::map<QString, Copyer> CopyMapType;
	typedef std::map<QString, Nuller> NullMapType;
	typedef std::map<QStringList, QString> ObjectNameMapType;

	static void registerType(QString objectName, std::vector<QStringList> idss,
		Constructor constructor, Copyer copyer, Nuller nuller);

	static ISObj *create(const Camera *camera, glm::ivec2 *sceneSize, 
		QStringList ids, QJsonObject jsonObject = QJsonObject());
	static ISObj *copy(ISObj *other);
	static ISObj *null(ISObj *other);

protected:
	static ConstructMapType *getContructMap();
	static CopyMapType *getCopyMap();
	static NullMapType *getNullMap();
	static ObjectNameMapType *getObjectNameMap();
	
private:
	static ConstructMapType *createMap;
	static CopyMapType *copyMap;
	static NullMapType *nullMap;
	static ObjectNameMapType *objectNameMap;
};

#endif // SOBJFACTORY_H
