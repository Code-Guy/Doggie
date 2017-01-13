#ifndef VIDEOSOBJ_H
#define VIDEOSOBJ_H

#include "sobj.h"
#include "texture.h"

#include <QMutex>
#include <vlc/vlc.h>

#include "lvluopluginapi.h"

class VideoSObj : public SObj 
{
	Q_OBJECT

public:
	VideoSObj();
	VideoSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	VideoSObj(VideoSObj *other);

	virtual ~VideoSObj();

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
	void inflateParamWidget(QString _fileName, float _frameRate); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	struct CallbackParam
	{
		QMutex mutex;
		VideoSObj *self;
		unsigned char *data;
	};

	void setVideo(QString fileName);

	// static callback functions
	static void *lock(void *opaque, void **planes);
	static void unlock(void *opaque, void *picture, void *const *planes);

	static void getVideoInfo(const struct libvlc_event_t *event, void *p);

	Texture *defaultTex; // Ĭ����ͼ
	Texture *frameTex; // ֡��ͼ

	QString fileName; // ��Ƶ�ļ���
	float frameRate; // ֡��

	libvlc_media_player_t *mediaPlayer; // ý�岥����
	libvlc_media_player_t *newMediaPlayer; // ��ʱý�岥����
	CallbackParam *cp; // �ص�����
	bool initialize; // ��ʼ��
	bool update; // ����
	int w, h; // ��Ƶ��С
	float fps; // ��Ƶ֡��

	// �Ҽ����Բ˵����
	FileWidget *fileWidget;
	QDoubleSpinBox *frameRateDoubleSpinBox;

private slots:
	void fileNameChanged(QString fileName);
	void frameRateChanged(double frameRate);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // VIDEOSOBJ_H