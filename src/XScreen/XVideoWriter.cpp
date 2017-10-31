#include "XVideoWriter.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

#include <iostream>
using namespace std;

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

class CXVideoWriter : public XVideoWriter
{
public:
	AVFormatContext *ic = NULL;//封装mp4输出上下文
	AVCodecContext *vc = NULL;//视频编码器上下文
	AVCodecContext *ac = NULL;//音频编码器上下文
	AVStream* vs = NULL;//视频流
	AVStream* as = NULL;//音频流
	SwsContext* vsc = NULL;//像素转换的上下文
	SwrContext* asc = NULL;//音频重采样上下文
	AVFrame* yuv = NULL;//输出yuv
	AVFrame* pcm = NULL;//重采样后输出的pcm
	int vpts = 0;//视频的pts;
	int apts = 0;//音频的pts;

	void Close()
	{
		if (ic) avformat_close_input(&ic);//内部会将ic置为NULL
		if (vc)
		{
			avcodec_close(vc);
			avcodec_free_context(&vc);
		}
		if (ac)
		{
			avcodec_close(ac);
			avcodec_free_context(&ac);
		}
		if (vsc)
		{
			sws_freeContext(vsc);
			vsc = NULL;
		}
		if (asc)
		{
			swr_free(&asc);
		}

		if (yuv)
		{
			av_frame_free(&yuv);
		}
		if (pcm)
		{
			av_frame_free(&pcm);
		}
	}

	bool Init(const char* file)
	{
		Close();

		//封装文件输出上下文
		avformat_alloc_output_context2(&ic, NULL, NULL, file);
		if (!ic)
		{
			cerr << "avformat_alloc_output_context2 failed!" << endl;
			return false;
		}

		filename = file;

		return true;
	}

	bool AddVideoStream()
	{
		if (!ic) return false;
		//1 视频编码器创建
		AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec)
		{
			cerr << "avcodec_find_encoder AV_CODEC_ID_H264 failed!" << endl;
			return false;
		}

		vc = avcodec_alloc_context3(codec);
		if (!vc)
		{
			cerr << "avcodec_alloc_context3 failed!" << endl;
			return false;
		}

		//比特率，压缩后每秒大小
		vc->bit_rate = vBitrate;
		vc->width = outWidth;
		vc->height = outHeight;
		
		//时间基数
		vc->time_base = { 1, outFPS };
		vc->framerate = {outFPS, 1};

		//画面组大小，多少帧一个关键帧
		vc->gop_size = 50;

		//B帧数设为0
		vc->max_b_frames = 0;
		vc->pix_fmt = AV_PIX_FMT_YUV420P;
		vc->codec_id = AV_CODEC_ID_H264;
		av_opt_set(vc->priv_data, "preset", "superfast", 0);
		vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

		//打开编码器
		int ret = avcodec_open2(vc, codec, NULL);
		if (ret != 0)
		{
			avcodec_free_context(&vc);
			cerr << "avcodec_open2 failed!" << endl;
			return false;
		}
		cout << "avcodec_open2 success!" << endl;

		//添加视频流到输出上下文
		vs = avformat_new_stream(ic, NULL);
		vs->codecpar->codec_tag = 0;//流内部不包含编码器信息
		avcodec_parameters_from_context(vs->codecpar, vc);
		
		av_dump_format(ic, 0, filename.c_str(), 1);

		//像素(尺寸)转换上下文
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight, (AVPixelFormat)inPixFmt,
			outWidth, outHeight, AV_PIX_FMT_YUV420P,
			SWS_BICUBIC, NULL, NULL, NULL);

		if (!vsc)
		{
			cerr << "sws_getCachedContext failed!" << endl;
			return false;
		}

		if (!yuv)
		{
			yuv = av_frame_alloc();
			yuv->format = AV_PIX_FMT_YUV420P;
			yuv->width = outWidth;
			yuv->height = outHeight;
			yuv->pts = 0;
			int ret = av_frame_get_buffer(yuv, 32);
			if (ret != 0)
			{
				cerr << "av_frame_get_buffer failed!" << endl;
				return false;
			}
		}

		return true;
	}

	AVPacket* EncodeVideo(const unsigned char* rgb)
	{
		if (!ic || !vsc || !yuv) return NULL;

		AVPacket* p = NULL;

		uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = (uint8_t *)rgb;

		int insize[AV_NUM_DATA_POINTERS] = { 0 };
		insize[0] = inWidth * 4;

		//rgb to yuv
		int h = sws_scale(vsc, indata, insize, 0, inHeight,
			yuv->data, yuv->linesize
			);
		if (h < 0)
		{
			return p;
		}

		//cout << h << "|";

		/*前面编码器设置的时间戳基准为1/fps,因此这里时间戳变化只需自增即可
		视频编码必须设置时间戳，不然编码器内部无法知道时间戳，会报错
		*/
		yuv->pts = vpts;
		vpts++;

		//encode
		int ret = avcodec_send_frame(vc, yuv);
		if (ret != 0)
		{
			return NULL;
		}

		p = av_packet_alloc();
		av_init_packet(p);
		ret = avcodec_receive_packet(vc, p);
		if (ret != 0 || p->size <= 0)
		{
			av_packet_free(&p);
			return NULL;
		}

		/*
		给编码器编码时设置的timebase是1/fps,每编码一次需要自增1，进行时间戳的累计，
		因为timebase就是1秒内有多少个时间单位，现在是帧率就是时间单位数,那时间戳每次累加
		即可。
		但是在输出封装器中，写入头信息后，timebase会更改，那么在写入文件或推流时，不能再以
		之前的timebase进行计算，必须是以改后的timebase进行转换计算

		写文件或推流前必须得转换，不然生成的文件无法播放
		*/
		av_packet_rescale_ts(p, vc->time_base, vs->time_base);

		p->stream_index = vs->index;//视频流的索引

		return p;
	}

	bool WriteHead()
	{
		if (!ic) return false;
		//打开io
		int ret = avio_open(&ic->pb, filename.c_str(), AVIO_FLAG_WRITE);
		if (ret != 0)
		{
			cout << "avio_open failed!" << endl;
			return false;
		}
		//写入封装头
		ret = avformat_write_header(ic, NULL);
		if (ret != 0)
		{
			cout << "avformat_write_header failed!" << endl;
			return false;
		}
		cout << "write " << filename << " head success!" << endl;

		return true;
	}

	bool WriteFrame(AVPacket* pkt)
	{
		if (!ic || !pkt || pkt->size <= 0) return false;
		//av_write_frame
		if (av_interleaved_write_frame(ic, pkt) != 0) return false;

		return true;
	}

	bool WriteEnd()
	{
		if (!ic || !ic->pb) return false;
		//写入尾部信息索引
		if (av_write_trailer(ic) != 0)
		{
			cerr << "av_write_trailer failed!" << endl;
			return false;
		}
			
		//关闭IO
		if (avio_closep(&ic->pb) != 0)
		{
			cerr << "avio_close failed!" << endl;
			return false;
		}

		cout << "WriteEnd success!" << endl;

		return true;
	}

	bool AddAudioStream()
	{
		if (!ic) return false;

		//1 找到音频编码
		AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
		if (!codec)
		{
			cerr << "avcodec_find_encoder AV_CODEC_ID_AAC failed!" << endl;
			return false;
		}

		//2 创建并打开音频编码器
		ac = avcodec_alloc_context3(codec);
		if (!ac)
		{
			cerr << "avcodec_alloc_context3 failed!" << endl;
			return false;
		}

		ac->bit_rate = aBitrate;
		ac->sample_rate = outSampleRate;
		ac->sample_fmt = (AVSampleFormat)outSampleFmt;
		ac->channels = outChannels;
		ac->channel_layout = av_get_default_channel_layout(outChannels);
		ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

		int ret = avcodec_open2(ac, codec, NULL);
		if (ret != 0)
		{
			avcodec_free_context(&ac);
			cerr << "avcodec_open2 failed!" << endl;
			return false;
		}
		cout << "avcodec_open2 AV_CODEC_ID_AAC success" << endl;

		//3 新增音频流
		as = avformat_new_stream(ic, NULL);
		if (!as)
		{
			cerr << "avformat_new_stream failed" << endl;
			return false;
		}

		as->codecpar->codec_tag = 0;
		avcodec_parameters_from_context(as->codecpar, ac);

		av_dump_format(ic, 0, filename.c_str(), 1);

		//4 音频重采样上下文创建
		asc = swr_alloc_set_opts(asc, 
			ac->channel_layout, ac->sample_fmt, ac->sample_rate,//输出格式
			av_get_default_channel_layout(inChannles), (AVSampleFormat)inSampleFmt, inSampleRate,//输入参数
			0, 0);

		ret = swr_init(asc);
		if (ret != 0)
		{
			cerr << "swr_init failed!" << endl;
			return false;
		}

		//5 音频重采样后输出AVFrame
		if (!pcm)
		{
			pcm = av_frame_alloc();
			pcm->format = ac->sample_fmt;
			pcm->channels = ac->channels;
			pcm->channel_layout = ac->channel_layout;
			pcm->nb_samples = nb_sample;//一帧音频的样本数量
			ret = av_frame_get_buffer(pcm, 0);
			if (ret < 0)
			{
				cerr << "av_frame_get_buffer failed!" << endl;
				return false;
			}

			cout << "audio AVFrame create success!" << endl;
		}

		return true;
	}
	
	AVPacket* EncodeAudio(const unsigned char* d)
	{
		if (!ic || !asc || !pcm) return NULL;

		const uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
		data[0] = (uint8_t *)d;

		//1 音频重采样
		int len = swr_convert(asc, pcm->data, pcm->nb_samples, 
			data, pcm->nb_samples);

		//cout << len << "*";

		//2 音频的编码
		int ret = avcodec_send_frame(ac, pcm);
		if (ret != 0)
		{
			return NULL;
		}

		AVPacket *pkt = av_packet_alloc();
		av_init_packet(pkt);
		ret = avcodec_receive_packet(ac, pkt);
		if (ret != 0)
		{
			av_packet_free(&pkt);
			return NULL;
		}

		cout << pkt->size << "|";
		pkt->stream_index = as->index;//音频流的索引

		pkt->pts = apts;
		pkt->dts = pkt->pts;
		apts += av_rescale_q(pcm->nb_samples, { 1, ac->sample_rate }, ac->time_base);

		return pkt;
	}

	bool IsVideoBefore()
	{
		//音视频时间戳进行比较
		if (!ic || !as || !vs) return false;
		int ret = av_compare_ts(vpts, vc->time_base, apts, ac->time_base);
		if (ret <= 0)
		{
			return true;
		}

		return false;
	}
};

XVideoWriter* XVideoWriter::Get(unsigned short index)
{
	static bool isfirst = true;
	if (isfirst)
	{
		av_register_all();
		avcodec_register_all();
		isfirst = false;
	}

	static CXVideoWriter wrs[65535];
	return &wrs[index];
}

XVideoWriter::XVideoWriter()
{
}

XVideoWriter::~XVideoWriter()
{
}
