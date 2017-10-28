#include "QtScreen.h"
#include <QScreen>
#include <QDesktopWidget>
#include <QImage>
#include <QPainter>

QtScreen::QtScreen(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	startTimer(100);//������ʱ��
}

void QtScreen::paintEvent(QPaintEvent *e)
{
	//��ȡQScreen
	static QScreen* scr = NULL;
	if (!scr)
	{
		scr = QGuiApplication::primaryScreen();//��ȡ����Ļ
	}
	
	//��ȡȫ��
	QPixmap pix = scr->grabWindow(QApplication::desktop()->winId());//HWND

	//pix.toImage().bits();

	//���ƽ���
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0, 0), pix.toImage());
	p.end();
}

void QtScreen::timerEvent(QTimerEvent *e)
{
	update();//�����ػ����
}