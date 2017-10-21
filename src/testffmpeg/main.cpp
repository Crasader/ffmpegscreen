/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：夏曹俊，此代码可用作为学习参考                       **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : FFmpeg
** Description   : FFMPEG项目创建示例
** Contact       : xiacaojun@qq.com
**        博客   : http://blog.csdn.net/jiedichina
**		视频课程 : http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
				 : http://edu.51cto.com/lecturer/index/user_id-12016059.html
				 : http://study.163.com/u/xiacaojun
				 : https://jiedi.ke.qq.com/

**   微信公众号  : jiedi2007
**		头条号	 : 夏曹俊
** Creation date : 2017-05-17
**
*******************************************************************************/
//cpp代码中要加
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/mem.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

//预编译指令
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avutil.lib")

//ffmpeg 版本号3.3.1
int main(int argc, char *argv[])
{
	//注册所有可解码类型

	//muxers, demuxers 所有的解封器
	av_register_all();
	
	//注册所有的解码器和过滤器 codecs
	avcodec_register_all();
	
	//支持网络包
	avformat_network_init();
	

	getchar();
	return 0;
}