extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#include <iostream>
using namespace std;

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib, "swresample.lib")

int main()
{
	char infile[] = "16.wav";//暂时没有找到数据源

	//char outfile[] = "test.wmv";//wmv解封装不同播放器兼容不太好
	char outfile[] = "out.aac";

	//注册所有的封装器和解封器 muxer,demuxer
	av_register_all();

	avcodec_register_all();

	//1 打开音频编码器
	AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	if (!codec)
	{
		cout << "avcodec_find_encoder error" << endl;
		getchar();
		return -1;
	}

	AVCodecContext* c = avcodec_alloc_context3(codec);
	if (!c)
	{
		cout << "avcodec_alloc_context3 error" << endl;
		getchar();
		return -1;
	}
	c->bit_rate = 64000;
	c->sample_rate = 44100;
	c->sample_fmt = AV_SAMPLE_FMT_FLTP;//支持aac编码的pcm数据类型,aac需要使用AV_SAMPLE_FMT_FLTP格式进行编码
	c->channel_layout = AV_CH_LAYOUT_STEREO;//通道类型 立体声
	c->channels = 2;
	c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;//全局头信息

	int ret = avcodec_open2(c, codec, NULL);
	if (ret < 0)
	{
		cout << "avcodec_open2 error" << endl;
		getchar();
		return -1;
	}

	cout << "avcodec_open2 success" << endl;

	//2 打开输出封装的上下文
	AVFormatContext* oc = NULL;
	avformat_alloc_output_context2(&oc, NULL, NULL, outfile);
	if (!oc)
	{
		cout << "avformat_alloc_output_context2 error" << endl;
		getchar();
		return -1;
	}

	AVStream* st = avformat_new_stream(oc, NULL);
	st->codecpar->codec_tag = 0;//封装器中不需要编码，设为0
	avcodec_parameters_from_context(st->codecpar, c);//将编码器信息拷贝到封装器中

	av_dump_format(oc, 0, outfile, 1);

	//3 open io, write head
	ret = avio_open(&oc->pb, outfile, AVIO_FLAG_WRITE);
	if (ret < 0)
	{
		cout << "avio_open error" << endl;
		getchar();
		return -1;
	}

	ret = avformat_write_header(oc, NULL);

	//44100 16 2
	//4 创建音频重采样上下文
	SwrContext *actx = NULL;
	actx = swr_alloc_set_opts(actx,
		c->channel_layout, c->sample_fmt, c->sample_rate,  //输出格式
		AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,    //输入格式
		0, 0);
	if (!actx)
	{
		cout << "swr_alloc_set_opts error" << endl;
		getchar();
		return -1;
	}

	ret = swr_init(actx);
	if (ret < 0)
	{
		cout << "swr_init error" << endl;
		getchar();
		return -1;
	}

	//5 打开输入音频文件，进行重采样
	AVFrame *frame = av_frame_alloc();
	frame->format = AV_SAMPLE_FMT_FLTP;//重采样后的目标格式
	frame->channels = 2;
	frame->channel_layout = AV_CH_LAYOUT_STEREO;
	frame->nb_samples = 1024;//一帧音频存放的样本数量
	ret = av_frame_get_buffer(frame, 0);
	if (ret < 0)
	{
		cout << "av_frame_get_buffer error" << endl;
		getchar();
		return -1;
	}

	int readSize = frame->nb_samples * 2 * 2;
	char* pcm = new char[readSize];
	FILE* fp = fopen(infile, "rb");

	for (;;)
	{
		int len = fread(pcm, 1, readSize, fp);
		if (len <= 0) break;
		const uint8_t *data[1];
		data[0] = (uint8_t*)pcm;

		//音频重采样
		len = swr_convert(actx, frame->data, frame->nb_samples, data, frame->nb_samples);
		if (len <= 0)
			break;
		//cout << "[" << len << "]";

		//6 音频编码
		AVPacket pkt;
		av_init_packet(&pkt);
		ret = avcodec_send_frame(c, frame);
		if (ret != 0) continue;
		ret = avcodec_receive_packet(c, &pkt);
		if (ret != 0) continue;

		//7 音频封装入acc文件
		pkt.stream_index = 0;
		pkt.pts = 0;//音频包编码时间戳可不处理，写入文件由内部处理
		pkt.dts = 0;
		ret = av_interleaved_write_frame(oc, &pkt);

		cout << "[" << len << "]";
	}

	delete pcm;
	pcm = NULL;

	//写入视频索引
	av_write_trailer(oc);
	//关闭视频输出IO
	avio_close(oc->pb);
	//清理封装输出上下文
	avformat_free_context(oc);
	//关闭编码器
	avcodec_close(c);
	//清理编码器上下文
	avcodec_free_context(&c);

	cout << "======================end=================" << endl;
	
	getchar();
	return 0;
}