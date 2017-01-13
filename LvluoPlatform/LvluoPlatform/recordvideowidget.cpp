#include "recordvideowidget.h"
#include "ui_recordvideowidget.h"

#include <chrono>
#include <thread>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include "tool.h"

const int fps = 30;

RecordVideoWidget::RecordVideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordVideoWidget)
{
    ui->setupUi(this);
	
	init();

	connect(ui->recordPushButton, SIGNAL(clicked()), this, SLOT(recordPushButtonClicked()));
	connect(ui->resetPushButton, SIGNAL(clicked()), this, SLOT(resetPushButtonClicked()));
	connect(ui->qualityHorizontalSlider, SIGNAL(valueChanged(int)), SLOT(qualitySliderValueChanged(int)));

	connect(&ffmpegProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ffmpegProcessFinished()));

	connect(&timer, SIGNAL(timeout()), this, SLOT(updateSnapshot()));
}

RecordVideoWidget::~RecordVideoWidget()
{
    delete ui;
}

void RecordVideoWidget::closeEvent(QCloseEvent *evt)
{
	init();
}

void RecordVideoWidget::init()
{
	frameCounter = 0;
	recordMSTime = 0;
	quality = ui->qualityHorizontalSlider->value();
	recordFinished = false;

	ui->recordPushButton->setEnabled(true);
	ui->recordPushButton->setText("Start Record");
	ui->resetPushButton->setEnabled(false);
	ui->hintLabel->setText("Press \"Start Record\" button to start record.");

	if (timer.isActive())
	{
		timer.stop();
	}

	Tool::removeFolder("tmp");
}

void RecordVideoWidget::saveSnapshots()
{
	QDir tmp;
	if (!tmp.exists("tmp"))
	{
		tmp.mkdir("tmp");
	}

	while (true)
	{
		mutex.lock();
		if (!snapshots.empty())
		{
			QImage snapshot = snapshots.back();
			snapshots.pop_back();
			mutex.unlock();

			snapshot.save("tmp/" + getCounterStr(frameCounter++) + ".jpg");
		}
		else
		{
			mutex.unlock();
			if (!timer.isActive())
			{
				recordFinished = true;
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

QString RecordVideoWidget::getCounterStr(int c)
{
	QString str;
	for (int i=4; i>=0; i--)
	{
		int p = (int)std::pow(10, i);
		int n = c / p;
		c -= n * p;
		str += QString::number(n);
	}

	return str;
}

void RecordVideoWidget::recordPushButtonClicked()
{
	if (ui->recordPushButton->text() == "Start Record")
	{
		ui->recordPushButton->setText("Stop Record");
		ui->resetPushButton->setEnabled(true);
		timer.start(1000 / fps);
		time.start();
		std::thread t(&RecordVideoWidget::saveSnapshots, this);
		t.detach();
	}
	else if (ui->recordPushButton->text() == "Stop Record")
	{
		timer.stop();
		recordMSTime = time.elapsed();
		ui->recordPushButton->setText("Record Finished");
		ui->recordPushButton->setEnabled(false);

		while (!recordFinished)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}

		QString videoFileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Video"),
			"./untitled.mp4", tr("Video (*.mp4)"));
		if (!videoFileName.isNull())
		{
			QStringList arguments;
			arguments << "-r" << QString::number((int)(1000.0 * frameCounter / recordMSTime))
				<< "-i" << "tmp/%5d.jpg"
				<< "-c:v" << "libx264"
				<< "-vf" << "scale=trunc(iw/2)*2:trunc(ih/2)*2,fps=" + QString::number(fps) + ",format=yuv420p"
				<< "-y"
				<< "-crf" << QString::number((int)(55.444f - 5.444f * quality))
				<< videoFileName;

			ffmpegProcess.start("ffmpeg.exe", arguments);
			ui->resetPushButton->setEnabled(false);
			ui->hintLabel->setText("Compressing...");
		}
	}
}

void RecordVideoWidget::resetPushButtonClicked()
{
	init();
}

void RecordVideoWidget::qualitySliderValueChanged(int val)
{
	quality = val;
}

void RecordVideoWidget::updateSnapshot()
{
	ui->hintLabel->setText(QString("Elapsed time: %1s\n%2").arg(time.elapsed() / 1000).arg(
		"Press \"Stop Record\" button to stop record."));
	emit getSnapshot();
}

void RecordVideoWidget::ffmpegProcessFinished()
{
	init();
	ui->hintLabel->setText("Compress finished!\nPress \"Start Record\" button to start record.");
	QByteArray procOutput;
	procOutput = ffmpegProcess.readAllStandardError();
	QFile outFile("Log.txt");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream out(&outFile);
	out << procOutput.data() << endl;
}

void RecordVideoWidget::setSnapshot(const QImage &snapshot)
{
	mutex.lock();
	snapshots.push_front(snapshot);
	mutex.unlock();
}