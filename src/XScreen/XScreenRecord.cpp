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
	//��ʼ����Ļ¼��
	XCaptureThread::Get()->fps = fps;
	XCaptureThread::Get()->Start();
	//��ʼ����Ƶ¼��
	XAudioThread::Get()->Start();
	//��ʼ��������
	XVideoWriter::Get()->inWidth = XCaptureThread::Get()->width;//��ȡ��Ļ�Ŀ�
	XVideoWriter::Get()->inHeight = XCaptureThread::Get()->height;//��ȡ��Ļ�ĸ�
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
	wait();//�ȴ���ǰ�߳��˳�

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
		//д����Ƶ
		char *rgb = XCaptureThread::Get()->GetRGB();
		if (rgb)
		{
			AVPacket *p = XVideoWriter::Get()->EncodeVideo((unsigned char*)rgb);
			delete rgb;

			XVideoWriter::Get()->WriteFrame(p);
			cout << "@";
		}
		
		//д����Ƶ
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
