#include "videosobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

VideoSObj::VideoSObj()
{

}

VideoSObj::VideoSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::TwoD)
{
	init();
}

VideoSObj::VideoSObj(VideoSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->fileName, other->frameRate);
	connectParamWidget();
	isRecord = true;
}

VideoSObj::~VideoSObj()
{
	if (frameTex)
	{
		delete frameTex;
	}
	if (mediaPlayer)
	{
		libvlc_media_player_stop(mediaPlayer);
		libvlc_media_player_release(mediaPlayer);
	}
	if (cp)
	{
		delete[] cp->data;
	}

	delete fileWidget;
	delete frameRateDoubleSpinBox;
}

void VideoSObj::destroy()
{
	delete this;
}

void VideoSObj::logic(float dt)
{
	SObj::logic(dt);

	if (mediaPlayer)
	{
		if (initialize)
		{
			if (frameTex)
			{
				delete frameTex;
			}

			frameTex = new Texture(w, h);
			aabb = Tool::calcTexAABB(w, h);
			initialize = false;
		}
		else if (update)
		{
			frameTex->setBits(cp->data, GL_RGBA);
			update = false;
		}

		if (libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Ended)
		{
			libvlc_media_player_stop(mediaPlayer);
		}
	}
}

void VideoSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getTextureShader()->enable();
		ResourceManager::getInstance()->getTextureShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getTextureShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getTextureShader()->setColorID(colorID);
		ResourceManager::getInstance()->getTextureShader()->setInverse(true);
		if (!mediaPlayer)
		{
			defaultTex->bind(); // 未加载，画默认静态图片
		}
		else
		{
			aabb = Tool::calcTexAABB(defaultTex->getWidth(), defaultTex->getHeight());
			frameTex->bind(); // 加载，画视频
		}
		ResourceManager::getInstance()->getTextureShader()->setMVP(
			camera->getViewProjectionMatrix() * m * glm::scale(aabb));
		PrimitiveManager::getInstance()->renderQuad();
		ResourceManager::getInstance()->getTextureShader()->setInverse(false);
	}
}

void VideoSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("File Name", fileName);
	jsonObject.insert("Frame Rate", frameRate);
}

void VideoSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (!jsonObject.isEmpty()) // 用载入数据初始化
	{
		inflateParamWidget(jsonObject.value("File Name").toString(), jsonObject.value("Frame Rate").toDouble());
	}
	connectParamWidget();
	isRecord = true;
}

void VideoSObj::click(QPoint mousePos)
{
	SObj::click(mousePos);

	if (mediaPlayer)
	{
		if (libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Playing)
		{
			libvlc_media_player_pause(mediaPlayer);
		}
		else if (libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Paused ||
			libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Stopped)
		{
			libvlc_media_player_play(mediaPlayer);
		}
	}
}

void VideoSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	VideoSObj *castedSnapshot = dynamic_cast<VideoSObj *>(snapshot);
	castedSnapshot->fileName = fileName;
	castedSnapshot->frameRate = frameRate;
}

void VideoSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	VideoSObj *castedSnapshot = dynamic_cast<VideoSObj *>(snapshot);
	inflateParamWidget(castedSnapshot->fileName, castedSnapshot->frameRate);
	connectParamWidget();
	isRecord = true;
}

void VideoSObj::init()
{
	mediaPlayer = nullptr;
	newMediaPlayer = nullptr;
	cp = nullptr;
	initialize = true;
	update = false;

	frameTex = nullptr;
	defaultTex = ResourceManager::getInstance()->getTexture(
		"Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/default.jpg");
	aabb = Tool::calcTexAABB(defaultTex->getWidth(), defaultTex->getHeight());
}

void VideoSObj::setParamWidget()
{
	paramWidget->addFile(ids.last(),
		"Videos (*.mp4)",
		fileWidget);
	paramWidget->addFloat("Frame Rate", 1.0f, 1000.0f, 1.0f, frameRateDoubleSpinBox);
}

void VideoSObj::inflateParamWidget(QString _fileName, float _frameRate)
{
	if (_fileName != fileName)
	{
		fileWidget->setFileName(_fileName);
		fileNameChanged(_fileName);
	}
	frameRateDoubleSpinBox->setValue(_frameRate);
	frameRateChanged(_frameRate);
}

void VideoSObj::connectParamWidget()
{
	connect(fileWidget, SIGNAL(fileNameChanged(QString)), this, SLOT(fileNameChanged(QString)));
	connect(frameRateDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(frameRateChanged(double)));
}

void VideoSObj::disconnectParamWidget()
{
	disconnect(fileWidget, SIGNAL(fileNameChanged(QString)), this, SLOT(fileNameChanged(QString)));
	disconnect(frameRateDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(frameRateChanged(double)));
}

void VideoSObj::setVideo(QString fileName)
{
	newMediaPlayer = mediaPlayer;
	mediaPlayer = nullptr;
	if (newMediaPlayer)
	{
		libvlc_media_player_stop(newMediaPlayer);
		libvlc_media_player_release(newMediaPlayer);
		newMediaPlayer = nullptr;
	}

	if (!fileName.isEmpty())
	{
		libvlc_media_t *media = libvlc_media_new_location(
			ResourceManager::getInstance()->getLibvlcEngine(),
			("file:///" + fileName).toStdString().c_str());

		newMediaPlayer = libvlc_media_player_new_from_media(media);
		libvlc_media_release(media);

		libvlc_event_manager_t *eventManager = libvlc_media_player_event_manager(newMediaPlayer);
		libvlc_event_attach(eventManager, libvlc_event_e::libvlc_MediaPlayerPlaying, getVideoInfo, (void *)this);

		libvlc_media_player_play(newMediaPlayer);
	}
}

void *VideoSObj::lock(void *opaque, void **planes)
{
	CallbackParam *cp = (CallbackParam *)opaque;
	cp->mutex.lock();
	cp->self->update = true;
	*planes = cp->data;

	return nullptr;
}

void VideoSObj::unlock(void *opaque, void *picture, void *const *planes)
{
	CallbackParam *cp = (CallbackParam *)opaque;
	cp->mutex.unlock();
}

void VideoSObj::getVideoInfo(const struct libvlc_event_t *event, void *p)
{
	VideoSObj *self = (VideoSObj *)p;
	if (self->mediaPlayer == nullptr)
	{
		unsigned int w, h;
		float fps;
		libvlc_video_get_size(self->newMediaPlayer, 0, &w, &h);
		self->fps = libvlc_media_player_get_fps(self->newMediaPlayer);

		if (self->cp)
		{
			delete[] self->cp->data;
		}
		self->cp = new CallbackParam;
		self->cp->self = self;
		self->cp->data = new unsigned char[w * h * 4];
		self->w = w;
		self->h = h;

		libvlc_video_set_callbacks(self->newMediaPlayer,
			lock,
			unlock,
			nullptr, self->cp);
		libvlc_video_set_format(self->newMediaPlayer, "RGBA", w, h, w * 4);
		self->mediaPlayer = self->newMediaPlayer;
		self->disconnectParamWidget();
		self->frameRateDoubleSpinBox->setValue(self->fps);
		self->connectParamWidget();
	}
}

void VideoSObj::fileNameChanged(QString fileName)
{
	recordLastSnapshot();
	this->fileName = fileName;
	setVideo(fileName);
	recordCurSnapshot();
}

void VideoSObj::frameRateChanged(double frameRate)
{
	recordLastSnapshot();
	this->frameRate = frameRate;
	if (mediaPlayer)
	{
		libvlc_media_player_set_rate(mediaPlayer, frameRate / fps);
	}
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new VideoSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new VideoSObj(dynamic_cast<VideoSObj *>(other));
}

ISObj *nuller()
{
	return new VideoSObj();
}