#pragma once
#include <QThread>
#include <QMutex>

class XCaptureThread : protected QThread
{
public:
	//out
	int width = 1280;
	int height = 720;
	//in
	int fps = 10;
	int cacheSize = 3;

	//线程安全,返回的空间由用户释放
	char* GetRGB();
	void Start();
	void Stop();
	void run();
	static XCaptureThread *Get()
	{
		static XCaptureThread ct;
		return &ct;
	}

	virtual ~XCaptureThread();
protected:
	bool isExit = false;
	std::list <char*> rgbs;
	QMutex mutex;
	XCaptureThread();
};

