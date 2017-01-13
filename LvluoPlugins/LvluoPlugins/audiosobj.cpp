#include "audiosobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

// 默认初始化数据
const float ButtonSize = 0.3; // 按钮大小

AudioSObj::AudioSObj()
{

}

AudioSObj::AudioSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, TwoD)
{
	init();
}

AudioSObj::AudioSObj(AudioSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->fileName);
	connectParamWidget();
	isRecord = true;
}

AudioSObj::~AudioSObj()
{
	if (mediaPlayer)
	{
		libvlc_media_player_stop(mediaPlayer);
		libvlc_media_player_release(mediaPlayer);
	}
	delete fileWidget;
}

void AudioSObj::destroy()
{
	delete this;
}

void AudioSObj::logic(float dt)
{
	SObj::logic(dt);

	if (mediaPlayer && 
		libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Ended)
	{
		libvlc_media_player_stop(mediaPlayer);
		currentBtnTex = playBtnTex;
	}
}

void AudioSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getTextureShader()->enable();
		ResourceManager::getInstance()->getTextureShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getTextureShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getTextureShader()->setColorID(colorID);

		// background
		bgTex->bind();
		ResourceManager::getInstance()->getTextureShader()->setMVP(
			camera->getViewProjectionMatrix()  * m * glm::scale(aabb));
		PrimitiveManager::getInstance()->renderQuad();

		if (mediaPlayer)
		{
			// play button
			currentBtnTex->bind();
			ResourceManager::getInstance()->getTextureShader()->setMVP(
				camera->getViewProjectionMatrix() *
				glm::translate(glm::vec3(0.0f, 0.0f, LayerDepthOffset)) * m * glm::scale(playBtnAABB * ButtonSize));

			PrimitiveManager::getInstance()->renderQuad();
		}
	}
}

void AudioSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("File Name", fileName);
}

void AudioSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (!jsonObject.isEmpty()) // 用载入数据初始化
	{
		inflateParamWidget(jsonObject.value("File Name").toString());
	}
	connectParamWidget();
	isRecord = true;
}

void AudioSObj::click(QPoint mousePos)
{
	SObj::click(mousePos);

	if (mediaPlayer && getButtonRect().contains(mousePos))
	{
		if (libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Playing)
		{
			libvlc_media_player_pause(mediaPlayer);
			currentBtnTex = playBtnTex;
		}
		else if (libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Paused ||
			libvlc_media_player_get_state(mediaPlayer) == libvlc_state_t::libvlc_Stopped)
		{
			libvlc_media_player_play(mediaPlayer);
			currentBtnTex = pauseBtnTex;
		}
	}
}

void AudioSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	dynamic_cast<AudioSObj *>(snapshot)->fileName = fileName;
}

void AudioSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	inflateParamWidget(dynamic_cast<AudioSObj *>(snapshot)->fileName);
	connectParamWidget();
	isRecord = true;
}

void AudioSObj::init()
{
	mediaPlayer = nullptr;

	bgTex = ResourceManager::getInstance()->getTexture(
		"Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/background.jpg");
	playBtnTex = ResourceManager::getInstance()->getTexture(
		"Resources/Icons/play_media.png");
	pauseBtnTex = ResourceManager::getInstance()->getTexture(
		"Resources/Icons/pause_media.png");

	currentBtnTex = pauseBtnTex;

	aabb = Tool::calcTexAABB(bgTex->getWidth(), bgTex->getHeight());
	playBtnAABB = Tool::calcTexAABB(playBtnTex->getWidth(), playBtnTex->getHeight());
	pauseBtnAABB = Tool::calcTexAABB(pauseBtnTex->getWidth(), pauseBtnTex->getHeight());
}

void AudioSObj::setParamWidget()
{
	paramWidget->addFile(ids.last(),
		"Audios (*.mp3)",
		fileWidget);
}

void AudioSObj::inflateParamWidget(QString _fileName)
{
	if (_fileName != fileName)
	{
		fileWidget->setFileName(_fileName);
		fileNameChanged(_fileName);
	}
}

void AudioSObj::connectParamWidget()
{
	connect(fileWidget, SIGNAL(fileNameChanged(QString)), this, SLOT(fileNameChanged(QString)));
}

void AudioSObj::disconnectParamWidget()
{
	disconnect(fileWidget, SIGNAL(fileNameChanged(QString)), this, SLOT(fileNameChanged(QString)));
}

void AudioSObj::setAudio(QString fileName)
{
	currentBtnTex = pauseBtnTex;
	libvlc_media_player_t *newMediaPlayer = mediaPlayer;
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
		libvlc_media_player_play(newMediaPlayer);
	}

	mediaPlayer = newMediaPlayer;
}

QRect AudioSObj::getButtonRect()
{
	QRect rect = getRect();
	QPoint center = rect.center();
	int size = rect.width() * ButtonSize;

	return QRect(center - QPoint(size / 2, size / 2), QSize(size, size));
}

void AudioSObj::fileNameChanged(QString fileName)
{
	recordLastSnapshot();
	this->fileName = fileName;
	setAudio(fileName);
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new AudioSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new AudioSObj(dynamic_cast<AudioSObj *>(other));
}

ISObj *nuller()
{
	return new AudioSObj();
}