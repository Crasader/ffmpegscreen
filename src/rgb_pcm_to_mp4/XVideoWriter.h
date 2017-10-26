#pragma once
#include <string>

class AVPacket;//������
enum XSAMPLEFMT
{
	X_S16 = 1,
	X_FLATP = 8
};

//���������������ռ䣬�����ڴ�����ʹ��
class XVideoWriter
{
public:
	//��Ƶ�������
	int inWidth = 848;
	int inHeight = 480;
	int  inPixFmt = 30;//AV_PIX_FMT_BGRA
	//��Ƶ�������
	int inSampleRate = 44100;
	int inChannles = 2;
	XSAMPLEFMT inSampleFmt = X_S16;

	//��Ƶ�������
	int vBitrate = 4000000;
	int outWidth = 848;
	int outHeight = 480;
	int outFPS = 25;

	//��Ƶ�������
	int aBitrate = 64000;
	int outChannels = 2;
	int outSampleRate = 44100;
	XSAMPLEFMT outSampleFmt = X_FLATP;
	int nb_sample = 1024;//���������ÿ֡����ÿͨ����������

	virtual bool Init(const char* file) = 0;
	virtual void Close() = 0;
	virtual bool AddVideoStream() = 0;
	virtual bool AddAudioStream() = 0;
	virtual AVPacket* EncodeVideo(const unsigned char* rgb) = 0;
	virtual AVPacket* EncodeAudio(const unsigned char* d) = 0;
	virtual bool WriteHead() = 0;
	//���ͷ�pkt�Ŀռ�
	virtual bool WriteFrame(AVPacket* pkt) = 0;
	virtual bool WriteEnd() = 0;
	virtual bool IsVideoBefore() = 0;

	static XVideoWriter *Get(unsigned short index = 0);

	virtual ~XVideoWriter();

	std::string filename; 
protected:
	XVideoWriter();
};

