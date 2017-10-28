#include "QtScreen.h"
#include <QScreen>
#include <QDesktopWidget>
#include <QImage>
#include <QPainter>

QtScreen::QtScreen(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	startTimer(100);//启动定时器
}

void QtScreen::paintEvent(QPaintEvent *e)
{
	//获取QScreen
	static QScreen* scr = NULL;
	if (!scr)
	{
		scr = QGuiApplication::primaryScreen();//获取主屏幕
	}
	
	//截取全屏
	QPixmap pix = scr->grabWindow(QApplication::desktop()->winId());//HWND

	//pix.toImage().bits();

	//绘制截屏
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0, 0), pix.toImage());
	p.end();
}

void QtScreen::timerEvent(QTimerEvent *e)
{
	update();//引发重绘操作
}