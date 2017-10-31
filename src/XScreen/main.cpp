#include "xscreen.h"
#include <QtWidgets/QApplication>
#include "XCaptureThread.h"
#include "XAudioThread.h"
#include "XScreenRecord.h"
#include <QDateTime>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
#if 0
	/////////////////////////////////////////////////
	////²âÊÔXScreenRecordÀà
	QDateTime t = QDateTime::currentDateTime();
	QString filename = t.toString("yyyyMMdd_hhmmss");
	filename = "xcreen_" + filename;
	filename += ".mp4";

	XScreenRecord::Get()->Start(filename.toLocal8Bit());
	getchar();

	XScreenRecord::Get()->Stop();
#endif

	///////////////////////////////////////////////////
#if 0
	/////////////////////////////////////////////////
	////²âÊÔÒôÆµÂ¼ÖÆÀà
	XAudioThread::Get()->Start();
	for (;;)
	{
		char* pcm = XAudioThread::Get()->GetPCM();
		if (pcm)
		{
			cout << "*";
		}
	}
	//////////////////////////////////////////////////
#endif

#if 0
	////²âÊÔÆÁÄ»Â¼ÖÆÀà
	XCaptureThread::Get()->Start();
	for (;;)
	{
		char* data = XCaptureThread::Get()->GetRGB();
		if (data)
		{
			cout << "+";
		}
	}

	XCaptureThread::Get()->Stop();
	////////////////////////////////////////
#endif
	//getchar();
    QApplication a(argc, argv);
    XScreen w;
    w.show();
    return a.exec();
}
