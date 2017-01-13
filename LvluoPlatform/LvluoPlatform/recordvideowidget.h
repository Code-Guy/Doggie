#ifndef RECORDVIDEOWIDGET_H
#define RECORDVIDEOWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QProcess>
#include <deque>
#include <QMutex>

namespace Ui {
class RecordVideoWidget;
}

class RecordVideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecordVideoWidget(QWidget *parent = 0);
    ~RecordVideoWidget();

protected:
	virtual void closeEvent(QCloseEvent *evt);

private:
	void init();

	void saveSnapshots();
	QString getCounterStr(int c);

    Ui::RecordVideoWidget *ui;
	int frameCounter;
	int recordMSTime;
	int quality;
	bool recordFinished;

	QTimer timer;
	QTime time;
	QProcess ffmpegProcess;
	std::deque<QImage> snapshots;

	QMutex mutex;

signals:
	void getSnapshot();

private slots:
	void recordPushButtonClicked();
	void resetPushButtonClicked();
	void qualitySliderValueChanged(int val);
	void updateSnapshot();
	void ffmpegProcessFinished();
	void setSnapshot(const QImage &snapshot);
};

#endif // RECORDVIDEOWIDGET_H
