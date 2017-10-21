extern "C"
{
#include <libavformat/avformat.h>
}

#include <iostream>
using namespace std;

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib,"avutil.lib")

int main()
{
	char infile[] = "testbig.mp4";
	//char outfile[] = "test.wmv";//wmv解封装不同播放器兼容不太好
	char outfile[] = "test.mov";

	//注册所有的封装器和解封器 muxer,demuxer
	av_register_all();

	AVFormatContext *ic = NULL;

	///1 open input file
	avformat_open_input(&ic, infile, 0, 0);
	if (!ic)
	{
		cerr << "avformat_open_input failed!" << endl;
		getchar();
		return -1;
	}
	cout << "open" << infile << " success!" << endl;

	///2 create output context
	AVFormatContext *oc = NULL;
	avformat_alloc_output_context2(&oc, NULL, NULL, outfile);
	if (!oc)
	{
		cerr << "avformat_alloc_output_context2 failed!" << outfile << endl;
		getchar();
		return -1;
	}

	///3 add the stream
	AVStream* videoStream = avformat_new_stream(oc, NULL);
	AVStream* audioStream = avformat_new_stream(oc, NULL);

	//音视频流的编码器信息拷贝
	avcodec_parameters_copy(videoStream->codecpar, ic->streams[0]->codecpar);
	avcodec_parameters_copy(audioStream->codecpar, ic->streams[1]->codecpar);

	//编码标志设为0，不做编码
	videoStream->codecpar->codec_tag = 0;
	audioStream->codecpar->codec_tag = 0;

	av_dump_format(ic, 0, infile, 0);
	cout << "====================================" << endl;
	av_dump_format(oc, 0, outfile, 1);

	///5 open out file io, write head
	int ret = avio_open(&oc->pb, outfile, AVIO_FLAG_WRITE);
	if (ret < 0)
	{
		cerr << "avio open failed!" << endl;
		getchar();

		return -1;
	}

	ret = avformat_write_header(oc, NULL);
	if (ret < 0)
	{
		cerr << "avformat_write_header failed!" << endl;
		getchar();

		return -1;
	}

	AVPacket pkt;
	for (;;)
	{
		int re = av_read_frame(ic, &pkt);
		if (re < 0)
		{
			break;
		}

		/*
		因为在avformat_write_header后，会将输入的timebase进行修改，因此不能再使用读取包中的源timebase进行计算
		时间戳，必须得使用输出上下文中的timebase。 
		timebase：是一个时间基准，表示一秒内有多少个点。比如timebase.num = 1, timebase.den = 128000,则表示
		一秒内有12800个点，时间戳需要以这个基准去计算
		*/
		pkt.pts = av_rescale_q_rnd(pkt.pts,
			ic->streams[pkt.stream_index]->time_base, 
			oc->streams[pkt.stream_index]->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts,
			ic->streams[pkt.stream_index]->time_base,
			oc->streams[pkt.stream_index]->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q_rnd(pkt.duration,
			ic->streams[pkt.stream_index]->time_base,
			oc->streams[pkt.stream_index]->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

		pkt.pos = -1;//重置packet的位置，用于写入输出文件中进行重新赋值，避免出错

		av_write_frame(oc, &pkt);

		av_packet_unref(&pkt);

		cout << ".";
	}

	//写入尾部信息
	av_write_trailer(oc);
	avio_close(oc->pb);

	cout << "=======================end=================" << endl;
	getchar();
	return 0;
}