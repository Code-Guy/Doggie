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

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void click(QPoint mousePos); // 鼠标点击事件函数

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

private:
	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(QString _fileName, float _frameRate); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

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

	Texture *defaultTex; // 默认贴图
	Texture *frameTex; // 帧贴图

	QString fileName; // 视频文件名
	float frameRate; // 帧率

	libvlc_media_player_t *mediaPlayer; // 媒体播放器
	libvlc_media_player_t *newMediaPlayer; // 临时媒体播放器
	CallbackParam *cp; // 回调参数
	bool initialize; // 初始化
	bool update; // 更新
	int w, h; // 视频大小
	float fps; // 视频帧速

	// 右键属性菜单组件
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