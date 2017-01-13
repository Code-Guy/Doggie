#ifndef AUDIOSOBJ_H
#define AUDIOSOBJ_H

#include "sobj.h"
#include "texture.h"

#include "lvluopluginapi.h"

#include <vlc/vlc.h>

class AudioSObj : public SObj
{
	Q_OBJECT

public:
	AudioSObj();
	AudioSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	AudioSObj(AudioSObj *other);

	virtual ~AudioSObj();

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void click(QPoint mousePos); // ������¼�����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(QString _fileName); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	void setAudio(QString fileName);
	QRect getButtonRect();

	Texture *bgTex;
	Texture *playBtnTex;
	Texture *pauseBtnTex;
	Texture *currentBtnTex;

	glm::vec3 playBtnAABB;
	glm::vec3 pauseBtnAABB;

	QString fileName; // ��Ƶ�ļ���

	libvlc_media_player_t *mediaPlayer; // ý�岥����

	// �Ҽ����Բ˵����
	FileWidget *fileWidget;

private slots:
	void fileNameChanged(QString fileName);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize, 
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // AUDIOSOBJ_H
