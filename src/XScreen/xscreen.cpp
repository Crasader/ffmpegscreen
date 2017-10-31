#include "xscreen.h"
#include<iostream>
#include <QTime>
#include "XScreenRecord.h"

using namespace std;
static bool isRecord = false;

#define RECORDQSS "\
QPushButton:!hover\
{background-image:url(:/XScreen/record_normal.png);}\
QPushButton:hover\
{background-image:url(:/XScreen/record_hot.png);}\
QPushButton:pressed\
{background-image:url(:/XScreen/record_pressed.png);\
background-color:rgba(255, 255, 255, 0);}"

static QTime rtime;

XScreen::XScreen(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
	setAttribute(Qt::WA_TranslucentBackground);//透明背景

	startTimer(100);//开启定时器
	cout << "XScreen" << endl;
}

void XScreen::Record()
{
	cout << " Record";
	isRecord = !isRecord;

	if (isRecord)
	{
		rtime.restart();//开始计时
		ui.recordButton->setStyleSheet("background-image: url(:/XScreen/stop.png);background-color:rgba(255, 255, 255, 0);");

		QDateTime t = QDateTime::currentDateTime();
		QString filename = t.toString("yyyyMMdd_hhmmss");
		filename = "xcreen_" + filename;
		filename += ".mp4";
		filename = ui.urlEdit->text() + "\\" + filename;
		XScreenRecord::Get()->outWidth = ui.widthEdit->text().toInt();
		XScreenRecord::Get()->outHeight = ui.heightEdit->text().toInt();
		XScreenRecord::Get()->fps = ui.fpsEdit->text().toInt();

		if (XScreenRecord::Get()->Start(filename.toLocal8Bit()))
		{
			return;
		}

		isRecord = false;
	}
	

	{
		ui.recordButton->setStyleSheet(RECORDQSS);
		XScreenRecord::Get()->Stop();
	}
}

void XScreen::timerEvent(QTimerEvent *e)
{
	if (isRecord)
	{
		int es = rtime.elapsed()/1000;//得到时间秒
		char buf[1024] = { 0 };
		sprintf(buf, "%03d:%02d", es / 60, es % 60);
		ui.timelabel->setText(buf);
	}
}