#include "XScreenRecord.h"
#include "XCaptureThread.h"
#include "XAudioThread.h"
#include "XVideoWriter.h"
#include <iostream>
using namespace std;

bool XScreenRecord::Start(const char* filename)
{
	if (!filename) return false;
	Stop();
	mutex.lock();
	isExit = false;
	//初始化屏幕录制
	XCaptureThread::Get()->fps = fps;
	XCaptureThread::Get()->Start();
	//初始化音频录制
	XAudioThread::Get()->Start();
	//初始化编码器
	XVideoWriter::Get()->inWidth = XCaptureThread::Get()->width;//获取屏幕的宽
	XVideoWriter::Get()->inHeight = XCaptureThread::Get()->height;//获取屏幕的高
	XVideoWriter::Get()->outWidth = outWidth;
	XVideoWriter::Get()->outHeight = outHeight;
	XVideoWriter::Get()->outFPS = fps;
	XVideoWriter::Get()->Init(filename);
	XVideoWriter::Get()->AddVideoStream();
	XVideoWriter::Get()->AddAudioStream();
	if (!XVideoWriter::Get()->WriteHead())
	{
		mutex.unlock();

		Stop();
		return false;
	}

	mutex.unlock();
	start();

	return true;
}

void XScreenRecord::Stop()
{
	mutex.lock();
	isExit = true;
	mutex.unlock();
	wait();//等待当前线程退出

	mutex.lock();
	XVideoWriter::Get()->WriteEnd();
	XVideoWriter::Get()->Close();

	XCaptureThread::Get()->Stop();
	XAudioThread::Get()->Stop();
	mutex.unlock();
}

void XScreenRecord::run()
{
	while (!isExit)
	{
		mutex.lock();
		//写入视频
		char *rgb = XCaptureThread::Get()->GetRGB();
		if (rgb)
		{
			AVPacket *p = XVideoWriter::Get()->EncodeVideo((unsigned char*)rgb);
			delete rgb;

			XVideoWriter::Get()->WriteFrame(p);
			cout << "@";
		}
		
		//写入音频
		char* pcm = XAudioThread::Get()->GetPCM();
		if (pcm)
		{
			AVPacket *p = XVideoWriter::Get()->EncodeAudio((unsigned char*)pcm);
			delete pcm;

			XVideoWriter::Get()->WriteFrame(p);
			cout << "#";
		}
		msleep(10);
		mutex.unlock();
	}
}

XScreenRecord::XScreenRecord()
{
}


XScreenRecord::~XScreenRecord()
{
}
