#pragma once
#include <QThread>
#include <QMutex>

class XAudioThread : protected QThread
{
public:
	int sampleRate = 44100;
	int channels = 2;
	int sampleByte = 2;

	int cacheSize = 10;
	int nbSample = 1024;

	//返回空间由用户清理
	char* GetPCM();
	void Start();
	void Stop();
	void run();
	static XAudioThread *Get()
	{
		static XAudioThread ct;
		return &ct;
	}

	virtual ~XAudioThread();
protected:
	bool isExit = false;
	std::list <char*> pcms;
	QMutex mutex;

	XAudioThread();
};

