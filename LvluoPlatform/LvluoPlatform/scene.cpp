#include "scene.h"
#include "appconfig.h"
#include "sceneproperty.h"
#include "history.h"
#include "removecommand.h"
#include "sobjfactory.h"
#include "tool.h"

#include <map>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QJsonDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <GL/glew.h>

Scene::Scene(int fps, QWidget *parent)
{
	initWidgetProps();
	initWindowContext();
	initGLEW();
	initGLStates();
	initPlugins();

	grid = new Grid;
	camera = new Camera(glm::vec3(0, 0, 6), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0),
		glm::radians(45.0f), 0.1f, 100.0f);
	op = new Operator(camera, &sObjs);
	multiSampleFBO = new MultiSampleFBO;

	connect(op, SIGNAL(updateCursorShape(Operator::CustomCursorShape)), this, SLOT(updateCursorShape(Operator::CustomCursorShape)));
	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	updateTimer.start(1000 / fps);

	lastTime = QTime::currentTime();
}

Scene::~Scene()
{
	for (ISObj *sObj : sObjs)
	{
		sObj->destroy();
	}
	for (HINSTANCE pluginHandle : pluginHandles)
	{
		FreeLibrary(pluginHandle);
	}

	delete sObjSelectionWidget;
	delete grid;
	delete camera;
	delete op;
	delete multiSampleFBO;
}

glm::ivec4 Scene::getViewport()
{
	return glm::ivec4(0, 0, width(), height());
}

glm::ivec2 Scene::getPosition()
{
	QPoint globalPos = mapToGlobal(pos());
	return glm::ivec2(globalPos.x(), globalPos.y());
}

void Scene::dragSObj(QStringList ids, QPoint mousePos)
{
	sObjSelectionWidget->close();

	ISObj *dragSObj = SObjFactory::create(camera, &sceneSize, ids);
	op->addDragSObj(dragSObj, mousePos);
}

void Scene::logic(float dt) // 逻辑
{
	QPoint mousePos = mapFromGlobal(QCursor::pos());
	emit updateMousePos(mousePos);

	camera->logic(SceneProperty::getInstance()->getFd(), SceneProperty::getInstance()->getSd());
	multiSampleFBO->logic(getViewport());
	grid->logic(getViewport());

	op->logic(mousePos);

	for (ISObj *sObj : sObjs)
	{
		sObj->logic(dt);
	}

	std::sort(sObjs.begin(), sObjs.end(), NearFirstComparer());
}

void Scene::render() // 绘制
{
	glm::vec3 bgColor = AppConfig::getInstance()->getPanelColor();
	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
	
	if (!SceneProperty::getInstance()->getStereoOn())
	{
		camera->setDisplayMode(Camera::Normal);
		multiSampleFBO->bind();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderImpl();
	}
	else
	{
		// render back left buffer
		glDrawBuffer(GL_BACK_LEFT);
		camera->setDisplayMode(Camera::LeftEye);
		multiSampleFBO->bind();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
		renderImpl();

		// render back right buffer
		glDrawBuffer(GL_BACK_RIGHT);
		camera->setDisplayMode(Camera::RightEye);
		multiSampleFBO->bind();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
		renderImpl();
	}
}

void Scene::paintEvent(QPaintEvent* evt)
{
	QTime currentTime = QTime::currentTime();
	float dt = lastTime.msecsTo(currentTime) / 1000.0f;
	lastTime = QTime::currentTime();

	logic(dt);
	render();
	
	HWND hwnd = (HWND)winId();
	HDC hdc = GetDC(hwnd);
	SwapBuffers(hdc);
	ReleaseDC(hwnd, hdc);
}

void Scene::resizeEvent(QResizeEvent *evt)
{
	sceneSize = glm::ivec2(width(), height());

	snapshot = QImage(size(), QImage::Format_RGB888);
	camera->setAspectRatio((float)width() / height());
	op->resize(width(), height());
	for (ISObj *sObj : sObjs)
	{
		sObj->updateWinObjRatio();
	}

	glViewport(0, 0, width(), height());
}

void Scene::mousePressEvent(QMouseEvent *evt)
{
	op->mousePressEvent(evt);
}

void Scene::mouseReleaseEvent(QMouseEvent *evt)
{
	op->mouseReleaseEvent(evt);
	if (evt->button() == Qt::RightButton)
	{
		bool showSObjParamWidget = false;
		for (ISObj *sObj : sObjs)
		{
			if (sObj->isShowParamWidget())
			{
				showSObjParamWidget = true;
				break;
			}
		}
		if (!showSObjParamWidget)
		{
			sObjSelectionWidget->move(adjustSObjSelectionWidgetPos(evt->globalPos()));
			sObjSelectionWidget->show();
			sObjSelectionWidget->activateWindow();
		}
	}
}

void Scene::mouseMoveEvent(QMouseEvent *evt)
{
	op->mouseMoveEvent(evt);
}

void Scene::mouseDoubleClickEvent(QMouseEvent *evt)
{
	op->mouseDoubleClickEvent(evt);
}

void Scene::keyPressEvent(QKeyEvent *evt)
{
	if (!evt->isAutoRepeat())
	{
		op->keyPressEvent(evt);
	}
}

void Scene::keyReleaseEvent(QKeyEvent *evt)
{
	if (!evt->isAutoRepeat())
	{
		op->keyReleaseEvent(evt);
	}
}

void Scene::wheelEvent(QWheelEvent *evt)
{
	op->wheelEvent(evt);
}

void Scene::focusInEvent(QFocusEvent *evt)
{
	if (!sObjSelectionWidget->isHidden())
	{
		sObjSelectionWidget->close();
	}
	for (ISObj *sObj : sObjs)
	{
		sObj->closeParamWidget();
	}
}

void Scene::updateCursorShape(Operator::CustomCursorShape cursorShape)
{
	switch (cursorShape)
	{
	case Operator::HoverShape:
		setCursor(Qt::OpenHandCursor);
		break;
	case Operator::DragShape:
		setCursor(Qt::ClosedHandCursor);
		break;
	case Operator::RotateTopLeftShape:
		setCursor(QCursor(QPixmap("Resources/Cursors/top_left.png")));
		break;
	case Operator::RotateTopRightShape:
		setCursor(QCursor(QPixmap("Resources/Cursors/top_right.png")));
		break;
	case Operator::RotateBottomLeftShape:
		setCursor(QCursor(QPixmap("Resources/Cursors/bottom_left.png")));
		break;
	case Operator::RotateBottomRightShape:
		setCursor(QCursor(QPixmap("Resources/Cursors/bottom_right.png")));
		break;
	case Operator::RotateFreeShape:
		setCursor(Qt::SizeAllCursor);
		break;
	case Operator::ScaleBDiagShape:
		setCursor(Qt::SizeBDiagCursor);
		break;
	case Operator::ScaleFDiagShape:
		setCursor(Qt::SizeFDiagCursor);
		break;
	case Operator::MoveCameraShape:
		setCursor(Qt::CrossCursor);
		break;
	case Operator::NoneShape:
		setCursor(Qt::ArrowCursor);
		break;
	default:
		break;
	}
}

void Scene::querySnapshot()
{
	getSnapshot();
	emit setSnapshot(snapshot);
}

bool Scene::create()
{
	if (saveModified())
	{
		for (ISObj *sObj : sObjs)
		{
			sObj->destroy();
		}
		sObjs.clear();
		History::getInstance()->clear();
		op->getGrouper()->clear();
		curProjectFileName = QString();

		return true;
	}
	return false;
}

void Scene::open(QString projectFileName)
{
	if (create())
	{
		if (!projectFileName.isNull())
		{
			curProjectFileName = projectFileName;
		}
		else
		{
			curProjectFileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open Project"),
				".", tr("Project (*.lv)"));
		}
		if (!curProjectFileName.isNull())
		{
			QFile file(curProjectFileName);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				printf("open project file failed!\n");
				return;
			}
			QByteArray byteArray = file.readAll();
			file.close();

			QJsonParseError jsonError;
			QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);

			if (jsonError.error != QJsonParseError::NoError)
			{
				printf("parse project file failed!\n");
				return;
			}

			QJsonObject projectJsonObject = document.object();

			std::map<QString, ISObj *> sObjIDsMap;
			QJsonObject sObjsJsonObject = projectJsonObject.value("SObjs").toObject();
			for (QString key : sObjsJsonObject.keys())
			{
				QStringList ids = key.split("-");
				ids.pop_back();
				ISObj *sObj = SObjFactory::create(camera, &sceneSize, 
					ids, sObjsJsonObject.value(key).toObject());
				sObjs.push_back(sObj);

				sObjIDsMap[key] = sObj;
			}

			auto &groups = op->getGrouper()->getGroups();
			QJsonObject groupsJsonObject = projectJsonObject.value("Groups").toObject();
			for (QString groupKey : groupsJsonObject.keys())
			{
				std::vector<ISObj *> group;
				QJsonObject groupJsonObject = groupsJsonObject.value(groupKey).toObject();
				for (QString itemKey : groupJsonObject.keys())
				{
					group.push_back(sObjIDsMap[groupJsonObject.value(itemKey).toString()]);
				}
				groups.push_back(group);
			}
			
			emit recentProject(curProjectFileName);
		}
	}
}

bool Scene::save()
{
	if (curProjectFileName.isNull())
	{
		curProjectFileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Project"),
			"./untitled.lv", tr("Project (*.lv)"));
	}
	if (!curProjectFileName.isNull())
	{
		save(curProjectFileName);
		return true;
	}
	return false;
}

bool Scene::saveAs()
{
	curProjectFileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save As Project"),
		"./untitled.lv", tr("Project (*.lv)"));
	if (!curProjectFileName.isNull())
	{
		save(curProjectFileName);
		return true;
	}
	return false;
}

void Scene::copy()
{
	op->copy();
}

void Scene::cut()
{
	op->cut();
}

void Scene::paste()
{
	op->paste();
}

void Scene::remove()
{
	op->remove();
}

void Scene::selectAll()
{
	op->selectAll();
}

void Scene::undo()
{
	History::getInstance()->undo();
}

void Scene::redo()
{
	History::getInstance()->redo();
}

void Scene::group()
{
	op->group();
}

void Scene::ungroup()
{
	op->ungroup();
}

QImage &Scene::getSnapshot()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glReadPixels(0, 0, width(), height(), GL_RGB, GL_UNSIGNED_BYTE, snapshot.bits());
	snapshot = snapshot.mirrored();

	return snapshot;
}

void Scene::initWidgetProps() // 初始化widget属性
{
	setWindowOpacity(1);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_PaintOnScreen);
	setFocusPolicy(Qt::StrongFocus);
}

void Scene::initWindowContext() // 初始化window界面context
{
	HWND hwnd = (HWND)winId();
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                        // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER
		| PFD_SUPPORT_GDI/*|PFD_STEREO*/,    // dwFlags
		PFD_TYPE_RGBA,            // PixelType
		24,                       // ColorBits
		0, 0, 0, 0, 0, 0,         // RGB bits and shifts
		8,                        // cAlphaBits
		0,                        // cAlphaShift
		0, 0, 0, 0, 0,            // Accum
		24,                       // Depth
		8,                        // Stencil
		0,                        // cAuxBuffers
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelIndex = ChoosePixelFormat(hdc, &pfd);
	if (pixelIndex == 0)
	{
		pixelIndex = 1;
		if (DescribePixelFormat(hdc, pixelIndex, sizeof(pfd), &pfd) == 0)
			return;
	}

	if (SetPixelFormat(hdc, pixelIndex, &pfd) == FALSE)
		return;

	HGLRC glctx;
	// check order matters
	if (!(glctx = wglCreateContext(hdc)) || !wglMakeCurrent(hdc, glctx))
		return;
}

void Scene::initGLEW() // 初始化Glew
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("failed to initialize GLEW");
	}
}

void Scene::initGLStates() // 初始化opengl状态
{
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Scene::initPlugins()
{
	std::vector<QStringList> sObjIDs;
	std::map<QString, std::vector<QStringList>> idssMap;

	parseIDs(sObjIDs, idssMap); // 解析ID List

	// 初始化SObj选择窗口
	sObjSelectionWidget = new SObjSelectionWidget("Stimulus Objects", genSObjIcons(sObjIDs));

	// 动态加载SObj插件dll
	for (int i=0; i<pluginPaths.size(); i++)
	{
		QString pluginPath = pluginPaths[i];
		QString pluginName = pluginNames[i];

		HINSTANCE pluginHandle = LoadLibrary(pluginPath.toStdWString().c_str());
		if (!pluginHandle)
		{
			printf("open plugin:\"%s\" failed.\n", pluginPath.toStdString().c_str());
			continue;
		}

		pluginHandles.push_back(pluginHandle);

		SObjFactory::Constructor constructor = reinterpret_cast<SObjFactory::Constructor>(
			GetProcAddress(pluginHandle, "construtor"));
		SObjFactory::Copyer copyer = reinterpret_cast<SObjFactory::Copyer>(
			GetProcAddress(pluginHandle, "copyer"));
		SObjFactory::Nuller nuller = reinterpret_cast<SObjFactory::Nuller>(
			GetProcAddress(pluginHandle, "nuller"));

		// 注册插件类
		SObjFactory::registerType(pluginName, idssMap[pluginName], constructor, copyer, nuller);
	}
}

void Scene::parseIDs(std::vector<QStringList> &sObjIDs, std::map<QString, std::vector<QStringList>> &idssMap)
{
	// 遍历Resources的SObjs文件夹
	std::vector<QString> dirs = Tool::traverseDir("Resources/SObjs/", QStringList{ "Icons", "Res" });
	for (QString dir : dirs)
	{
		QStringList dirSplitList = dir.split('/');
		dirSplitList = dirSplitList.mid(2, dirSplitList.length() - 2);

		sObjIDs.push_back(dirSplitList);
	}

	// 遍历插件文件夹
	pluginPaths = Tool::traverseFile("Plugins");
	for (QString pluginPath : pluginPaths)
	{
		QFileInfo fileInfo(pluginPath);
		pluginNames.push_back(fileInfo.baseName());
	}

	// 获取ID List
	std::vector<QStringList> tmpSObjIDs = sObjIDs;
	for (auto tmpSObjIDIter = tmpSObjIDs.begin(); tmpSObjIDIter != tmpSObjIDs.end(); )
	{
		bool found = false;
		int size = tmpSObjIDIter->size();
		for (int i = size - 1; i >= 0; i--)
		{
			auto pluginNameIter = std::find(pluginNames.begin(), pluginNames.end(), tmpSObjIDIter->at(i));
			if (pluginNameIter != pluginNames.end())
			{
				idssMap[*pluginNameIter].push_back(*tmpSObjIDIter);
				found = true;
				break;
			}
		}

		if (found)
		{
			tmpSObjIDIter = tmpSObjIDs.erase(tmpSObjIDIter);
		}
		else
		{
			sObjIDs.erase(std::find(sObjIDs.begin(), sObjIDs.end(), *tmpSObjIDIter));
			tmpSObjIDIter++;
		}
	}
}

std::vector<SObjIcon *> Scene::genSObjIcons(const std::vector<QStringList> &layerSObjIDs, int layer)
{
	std::vector<SObjIcon *> layerSObjIcons;

	std::map<QString, std::vector<QStringList>> layerSObjIDsMap;
	for (QStringList sObjID : layerSObjIDs)
	{
		layerSObjIDsMap[sObjID.at(layer)].push_back(sObjID);
	}

	for (auto layerSObjIDsMapIter : layerSObjIDsMap)
	{
		if (layerSObjIDsMapIter.second.front().size() == layer + 1) // 如果到最后一层，直接返回对应的icon（其下级窗口为空）
		{
			for (QStringList layerSObjID : layerSObjIDsMapIter.second)
			{
				layerSObjIcons.push_back(new SObjIcon(this, layerSObjID));
			}
		}
		else
		{
			QStringList layerSObjsMapIterIDs = layerSObjIDsMapIter.second.front().mid(0, layer + 1);
			SObjSelectionWidget *layerSObjsMapIterWidget = new SObjSelectionWidget(layerSObjsMapIterIDs.last(), genSObjIcons(layerSObjIDsMapIter.second, layer + 1));
			layerSObjIcons.push_back(new SObjIcon(this, layerSObjsMapIterIDs, layerSObjsMapIterWidget));
		}
	}

	return layerSObjIcons;
}

QPoint Scene::adjustSObjSelectionWidgetPos(QPoint originPos)
{
	QPoint adjustPos = originPos;
	QPoint globalPos = mapToGlobal(pos());
	sObjSelectionWidget->show();
	sObjSelectionWidget->layout()->invalidate();
	sObjSelectionWidget->hide();
	QSize size = sObjSelectionWidget->size();
	if (originPos.x() + size.width() > globalPos.x() + width())
	{
		adjustPos.setX(originPos.x() - size.width());
	}
	if (originPos.y() + size.height() > globalPos.y() + height())
	{
		adjustPos.setY(originPos.y() - size.height());
	}
	return adjustPos;
}

void Scene::save(QString projectFileName)
{
	QJsonObject projectJsonObject;

	QJsonObject sObjsJsonObject;
	for (ISObj *sObj : sObjs)
	{
		QJsonObject sObjJsonObject;
		sObj->save(sObjJsonObject);

		sObjsJsonObject.insert(sObj->getJoinID(), sObjJsonObject);
	}
	projectJsonObject.insert("SObjs", sObjsJsonObject);

	QJsonObject groupsJsonObject;
	auto &groups = op->getGrouper()->getGroups();
	int i = 0;
	for (auto iter = groups.begin(); iter != groups.end(); iter++)
	{
		QJsonObject groupJsonObject;
		int j = 0;
		for (ISObj *sObj : *iter)
		{
			if (std::find(sObjs.begin(), sObjs.end(), sObj) != sObjs.end())
			{
				groupJsonObject.insert("Item " + QString::number(j++), sObj->getJoinID());
			}
		}
		groupsJsonObject.insert("Group " + QString::number(i++), groupJsonObject);
	}
	projectJsonObject.insert("Groups", groupsJsonObject);

	QJsonDocument document;
	document.setObject(projectJsonObject);
	QByteArray byteArray = document.toJson(QJsonDocument::Indented);

	QFile file(projectFileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		printf("open project file failed!\n");
		return;
	}

	file.write(byteArray);
	file.close();

	History::getInstance()->save();
}

void Scene::renderImpl()
{
	grid->render();
	op->render();

	for (ISObj *sObj : sObjs)
	{
		sObj->render(SceneProperty::getInstance()->getBlendOn());
	}

	multiSampleFBO->blit();
}

void Scene::timeOut()
{
	update();
}

bool Scene::saveModified()
{
	if (History::getInstance()->isDirty())
	{
		int ret = QMessageBox::warning(Q_NULLPTR, tr("Save Warning"),
			tr("The project has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel,
			QMessageBox::Save);
		if (ret == QMessageBox::Discard)
		{
			return true;
		}
		else if (ret == QMessageBox::Cancel)
		{
			return false;
		}
		else if (ret == QMessageBox::Save)
		{
			return save();
		}
	}
	return true;
}

void Scene::clear()
{
	History::getInstance()->execute(new RemoveCommand(&sObjs, sObjs), true);
}