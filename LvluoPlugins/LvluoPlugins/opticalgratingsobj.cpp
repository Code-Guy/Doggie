#include "opticalgratingsobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

// 默认初始化数据
const OpticalGratingSObj::BoundaryShape DefaultBoundaryShape = OpticalGratingSObj::BoundaryShape::Circle;
const OpticalGratingSObj::MotionCurve DefaultMotionCurve = OpticalGratingSObj::MotionCurve::Sin;
const glm::vec3 DefaultColor = glm::vec3(1.0f, 1.0f, 1.0f);
const float DefaultLuminance = 1.0f;
const float DefaultOrientation = 0.0f;
const float DefaultSF = 5.0f;
const float DefaultTF = 1.0f;
const float DefaultDuration = 10.0f;

OpticalGratingSObj::OpticalGratingSObj()
{

}

OpticalGratingSObj::OpticalGratingSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::TwoD)
{
	init();
}

OpticalGratingSObj::OpticalGratingSObj(OpticalGratingSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->boundaryShape, other->motionCurve, other->color,
		other->luminance, other->sf, other->tf, other->orientation, other->duration);
	connectParamWidget();
	isRecord = true;
}

OpticalGratingSObj::~OpticalGratingSObj()
{
	delete gratingImg;
	delete gratingTex;
	delete boundaryShapeComboBox;
	delete motionCurveComboBox;
	delete colorLabel;
	delete luminanceDoubleSpinBox;
	delete orientationDoubleSpinBox;
	delete sfDoubleSpinBox;
	delete tfDoubleSpinBox;
	delete durationDoubleSpinBox;
}

void OpticalGratingSObj::destroy()
{
	delete this;
}

void OpticalGratingSObj::logic(float dt)
{
	SObj::logic(dt);

	if (t > duration)
	{
		t = 0.0f;
	}
	if (isPlay)
	{
		t += dt;
	}

	genGratingTexture();
}

void OpticalGratingSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getTextureShader()->enable();
		ResourceManager::getInstance()->getTextureShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getTextureShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getTextureShader()->setColorID(colorID);
		ResourceManager::getInstance()->getTextureShader()->setMVP(
			camera->getViewProjectionMatrix() * m);

		gratingTex->bind();

		PrimitiveManager::getInstance()->renderQuad();
	}
}


void OpticalGratingSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Boundary Shape", boundaryShape);
	jsonObject.insert("Motion Curve", motionCurve);
	jsonObject.insert("Color", Tool::vec3ToStr(color));
	jsonObject.insert("Luminance", luminance);
	jsonObject.insert("Spatial Frequency", sf);
	jsonObject.insert("Temporal Frequency", tf);
	jsonObject.insert("Orientation", orientation);
	jsonObject.insert("Duration", duration);
}

void OpticalGratingSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用默认数据初始化
	{
		inflateParamWidget(DefaultBoundaryShape, DefaultMotionCurve, DefaultColor,
			DefaultLuminance, DefaultSF, DefaultTF, DefaultOrientation, DefaultDuration);
	}
	else // 用载入数据初始化
	{
		inflateParamWidget(jsonObject.value("Boundary Shape").toInt(), jsonObject.value("Motion Curve").toInt(), Tool::vec3FromStr(jsonObject.value("Color").toString()),
			jsonObject.value("Luminance").toDouble(), jsonObject.value("Spatial Frequency").toDouble(), jsonObject.value("Temporal Frequency").toDouble(), 
			jsonObject.value("Orientation").toDouble(), jsonObject.value("Duration").toDouble());
	}
	connectParamWidget();
	isRecord = true;
}

void OpticalGratingSObj::click(QPoint mousePos)
{
	SObj::click(mousePos);

	isPlay = !isPlay;
	if (isPlay && t > duration)
	{
		t = 0.0f;
	}
}

void OpticalGratingSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	OpticalGratingSObj *castedSnapshot = dynamic_cast<OpticalGratingSObj *>(snapshot);
	castedSnapshot->boundaryShape = boundaryShape;
	castedSnapshot->motionCurve = motionCurve;
	castedSnapshot->color = color;
	castedSnapshot->luminance = luminance;
	castedSnapshot->sf = sf;
	castedSnapshot->tf = tf;
	castedSnapshot->orientation = orientation;
	castedSnapshot->duration = duration;
}

void OpticalGratingSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	OpticalGratingSObj *castedSnapshot = dynamic_cast<OpticalGratingSObj *>(snapshot);
	inflateParamWidget(castedSnapshot->boundaryShape,
	castedSnapshot->motionCurve,
	castedSnapshot->color,
	castedSnapshot->luminance,
	castedSnapshot->sf,
	castedSnapshot->tf,
	castedSnapshot->orientation,
	castedSnapshot->duration);
	connectParamWidget();
	isRecord = true;
}

void OpticalGratingSObj::init()
{
	t = 0.0f;
	isUpdate = true;
	isPlay = true;

	gratingImg = new QImage(GratingImageSize, GratingImageSize, QImage::Format_RGBA8888);
	maxSquareImgSize = GratingImageSize * glm::sqrt(2.0f) / 2.0f;
	gratingTex = new Texture(maxSquareImgSize, maxSquareImgSize);

	mask[0] = QBitmap("Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/circle.bmp");
	mask[1] = QBitmap("Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/ellipse.bmp");
	mask[2] = QBitmap("Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/rectangle.bmp");
	mask[3] = QBitmap("Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/square.bmp");
}

void OpticalGratingSObj::setParamWidget()
{
	paramWidget->addEnum("Boundary Shape", QStringList{ "Circle", "Ellipse", "Rectangle", "Square" }, boundaryShapeComboBox);
	paramWidget->addEnum("Motion Curve", QStringList{ "Sin", "Cos" }, motionCurveComboBox);
	paramWidget->addColor("Color", colorLabel);
	paramWidget->addFloat("Luminance", 0.0f, 1.0f, 0.1f, luminanceDoubleSpinBox);
	paramWidget->addFloat("Orientation", 0.0f, 180.0f, 1.0f, orientationDoubleSpinBox);
	paramWidget->addFloat("Spatial Frequency", 1.0f, 100.0f, 1.0f, sfDoubleSpinBox);
	paramWidget->addFloat("Temporal Frequency", 1.0f, 100.0f, 1.0f, tfDoubleSpinBox);
	paramWidget->addFloat("Duration", 1.0f, 600.0f, 1.0f, durationDoubleSpinBox);
}

void OpticalGratingSObj::inflateParamWidget(int _boundaryShape, int _motionCurve, glm::vec3 _color, 
	float _luminance, float _sf, float _tf, float _orientation, float _duration)
{
	boundaryShapeComboBox->setCurrentIndex(_boundaryShape);
	motionCurveComboBox->setCurrentIndex(_motionCurve);
	colorLabel->setColor(_color);
	luminanceDoubleSpinBox->setValue(_luminance);
	orientationDoubleSpinBox->setValue(_orientation);
	sfDoubleSpinBox->setValue(_sf);
	tfDoubleSpinBox->setValue(_tf);
	durationDoubleSpinBox->setValue(_duration);

	boundaryShapeComboBoxCurrentIndexChanged(_boundaryShape);
	motionCurveComboBoxCurrentIndexChanged(_motionCurve);
	colorChanged(_color);
	luminanceDoubleSpinBoxChanged(_luminance);
	orientationDoubleSpinBoxChanged(_orientation);
	sfDoubleSpinBoxChanged(_sf);
	tfDoubleSpinBoxChanged(_tf);
	durationDoubleSpinBoxChanged(_duration);
}

void OpticalGratingSObj::connectParamWidget()
{
	connect(boundaryShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(boundaryShapeComboBoxCurrentIndexChanged(int)));
	connect(motionCurveComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(motionCurveComboBoxCurrentIndexChanged(int)));
	connect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
	connect(luminanceDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(luminanceDoubleSpinBoxChanged(double)));
	connect(orientationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(orientationDoubleSpinBoxChanged(double)));
	connect(sfDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sfDoubleSpinBoxChanged(double)));
	connect(tfDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(tfDoubleSpinBoxChanged(double)));
	connect(durationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationDoubleSpinBoxChanged(double)));
}

void OpticalGratingSObj::disconnectParamWidget()
{
	disconnect(boundaryShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(boundaryShapeComboBoxCurrentIndexChanged(int)));
	disconnect(motionCurveComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(motionCurveComboBoxCurrentIndexChanged(int)));
	disconnect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
	disconnect(luminanceDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(luminanceDoubleSpinBoxChanged(double)));
	disconnect(orientationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(orientationDoubleSpinBoxChanged(double)));
	disconnect(sfDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sfDoubleSpinBoxChanged(double)));
	disconnect(tfDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(tfDoubleSpinBoxChanged(double)));
	disconnect(durationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationDoubleSpinBoxChanged(double)));
}

void OpticalGratingSObj::genGratingTexture()
{
	if (isUpdate)
	{
		// 操作光栅图片的像素
		unsigned char *pixelLine = new unsigned char[GratingImageSize * 4];
		for (int i = 0; i < GratingImageSize; i++)
		{
			float ratio = (sf * i / GratingImageSize + tf * t) * glm::pi<float>() * 2.0f;
			switch (motionCurve)
			{
			case OpticalGratingSObj::Sin:
				ratio = std::sin(ratio);
				break;
			case OpticalGratingSObj::Cos:
				ratio = std::cos(ratio);
				break;
			default:
				break;
			}

			ratio = ratio / 2.0f + 0.5f;

			pixelLine[i * 4] = color.x * ratio * luminance * 255;
			pixelLine[i * 4 + 1] = color.y * ratio * luminance * 255;
			pixelLine[i * 4 + 2] = color.z * ratio * luminance * 255;
			pixelLine[i * 4 + 3] = 255;
		}

		for (int i = 0; i < GratingImageSize; i++)
		{
			memcpy(gratingImg->scanLine(i), pixelLine, GratingImageSize * 4);
		}

		delete[] pixelLine;

		// 变换裁剪蒙版光栅图片
		// 1、旋转
		QTransform transform;
		QTransform rotateTrans = transform.rotate(orientation);
		QPixmap rotatedGratingImg = QPixmap::fromImage(*gratingImg).transformed(rotateTrans, Qt::SmoothTransformation);
		// 2、裁剪
		int offset = (rotatedGratingImg.width() - maxSquareImgSize) / 2;
		QRect clippedRect(offset, offset, maxSquareImgSize, maxSquareImgSize);
		QPixmap clippedSquareImg = rotatedGratingImg.copy(clippedRect);
		// 3.蒙版
		QBitmap resizedMask = mask[boundaryShape].scaled(clippedSquareImg.size());
		clippedSquareImg.setMask(resizedMask);
		QImage retImg = clippedSquareImg.toImage();

		gratingTex->setBits(retImg.bits(), GL_BGRA);
	}

	isUpdate = !isUpdate;
}

void OpticalGratingSObj::boundaryShapeComboBoxCurrentIndexChanged(int index)
{
	recordLastSnapshot();
	boundaryShape = (BoundaryShape)index;

	switch (boundaryShape)
	{
	case OpticalGratingSObj::Circle:
	case OpticalGratingSObj::Square:
		aabb = Tool::calcTexAABB(1, 1); // 1 : 1
		break;
	case OpticalGratingSObj::Ellipse:
	case OpticalGratingSObj::Rectangle:
		aabb = Tool::calcTexAABB(8, 5); // 8 : 5
		break;
	default:
		break;
	}
	recordCurSnapshot();
}

void OpticalGratingSObj::motionCurveComboBoxCurrentIndexChanged(int index)
{
	recordLastSnapshot();
	motionCurve = (MotionCurve)index;
	recordCurSnapshot();
}

void OpticalGratingSObj::colorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	this->color = color;
	recordCurSnapshot();
}

void OpticalGratingSObj::luminanceDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	luminance = val;
	recordCurSnapshot();
}

void OpticalGratingSObj::orientationDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	orientation = val;
	recordCurSnapshot();
}

void OpticalGratingSObj::sfDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	sf = val;
	recordCurSnapshot();
}

void OpticalGratingSObj::tfDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	tf = val;
	recordCurSnapshot();
}

void OpticalGratingSObj::durationDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	duration = val;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new OpticalGratingSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new OpticalGratingSObj(dynamic_cast<OpticalGratingSObj *>(other));
}

ISObj *nuller()
{
	return new OpticalGratingSObj();
}