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
	//char outfile[] = "test.wmv";//wmv���װ��ͬ���������ݲ�̫��
	char outfile[] = "test.mov";

	//ע�����еķ�װ���ͽ���� muxer,demuxer
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

	//����Ƶ���ı�������Ϣ����
	avcodec_parameters_copy(videoStream->codecpar, ic->streams[0]->codecpar);
	avcodec_parameters_copy(audioStream->codecpar, ic->streams[1]->codecpar);

	//�����־��Ϊ0����������
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
		��Ϊ��avformat_write_header�󣬻Ὣ�����timebase�����޸ģ���˲�����ʹ�ö�ȡ���е�Դtimebase���м���
		ʱ����������ʹ������������е�timebase�� 
		timebase����һ��ʱ���׼����ʾһ�����ж��ٸ��㡣����timebase.num = 1, timebase.den = 128000,���ʾ
		һ������12800���㣬ʱ�����Ҫ�������׼ȥ����
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

		pkt.pos = -1;//����packet��λ�ã�����д������ļ��н������¸�ֵ���������

		av_write_frame(oc, &pkt);

		av_packet_unref(&pkt);

		cout << ".";
	}

	//д��β����Ϣ
	av_write_trailer(oc);
	avio_close(oc->pb);

	cout << "=======================end=================" << endl;
	getchar();
	return 0;
}