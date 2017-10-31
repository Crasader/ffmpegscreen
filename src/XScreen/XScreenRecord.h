#pragma once
#include <iostream>
#include <QThread>
#include <QMutex>

using namespace std;

class XScreenRecord : protected QThread
{
public:
	int fps = 10;//���¼���ļ���֡��
	int outWidth = 1280;//���¼���ļ��Ŀ�
	int outHeight = 720;//���¼���ļ��ĸ�

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

