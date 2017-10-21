/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               �������Ĳܿ����˴��������Ϊѧϰ�ο�                       **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : FFmpeg
** Description   : FFMPEG��Ŀ����ʾ��
** Contact       : xiacaojun@qq.com
**        ����   : http://blog.csdn.net/jiedichina
**		��Ƶ�γ� : http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
				 : http://edu.51cto.com/lecturer/index/user_id-12016059.html
				 : http://study.163.com/u/xiacaojun
				 : https://jiedi.ke.qq.com/

**   ΢�Ź��ں�  : jiedi2007
**		ͷ����	 : �Ĳܿ�
** Creation date : 2017-05-17
**
*******************************************************************************/
//cpp������Ҫ��
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/mem.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

//Ԥ����ָ��
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avutil.lib")

//ffmpeg �汾��3.3.1
int main(int argc, char *argv[])
{
	//ע�����пɽ�������

	//muxers, demuxers ���еĽ����
	av_register_all();
	
	//ע�����еĽ������͹����� codecs
	avcodec_register_all();
	
	//֧�������
	avformat_network_init();
	

	getchar();
	return 0;
}