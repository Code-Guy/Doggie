#ifndef SOBJICON_H
#define SOBJICON_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <vector>

class Scene;
class SObjSelectionWidget;
class SObjIcon : public QWidget 
{
	Q_OBJECT

public:
	SObjIcon(Scene *scene, QStringList ids, SObjSelectionWidget *childWidget = nullptr, int fps = 5, QWidget *parent = Q_NULLPTR);
	~SObjIcon();

	void shutdown();

protected:
	virtual void enterEvent(QEvent *evt);
	virtual void leaveEvent(QEvent *evt);
	virtual void mousePressEvent(QMouseEvent *evt);
	virtual void mouseReleaseEvent(QMouseEvent *evt);

signals:
	void hover(QString id, SObjIcon *emitter);

private slots:
	void updateImage();

private:
	QPixmap getSquareImage(const QPixmap &image);
	void postMouseEvent(QMouseEvent *evt);
	QPoint adjustChildWidgetPos(QPoint pos);

	QLabel imageLabel;
	QLabel nameLabel;
	std::vector<QPixmap> images;
	int currentImageIndex; 
	QTimer updateTimer; // 更新计时器

	QStringList ids;
	SObjSelectionWidget *childWidget; // 子窗口
	Scene *scene;
};

#endif // SOBJICON_H