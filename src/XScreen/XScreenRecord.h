#pragma once
#include <iostream>
#include <QThread>
#include <QMutex>

using namespace std;

class XScreenRecord : protected QThread
{
public:
	int fps = 10;//输出录制文件的帧率
	int outWidth = 1280;//输出录制文件的宽
	int outHeight = 720;//输出录制文件的高

	bool Start(const char* filename);
	void Stop();
	void run();
	static XScreenRecord *Get()
	{
		static XScreenRecord ct;
		return &ct;
	}

	virtual ~XScreenRecord();
protected:
	bool isExit = false;
	QMutex mutex;

	XScreenRecord();
};

