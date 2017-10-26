
#include "XVideoWriter.h"
#include <iostream>
using namespace std;

int main()
{
	char outfile[] = "rgbpcm.mp4";
	char rgbfile[] = "test.rgb";
	char pcmfile[] = "test.pcm";

	XVideoWriter* xw = XVideoWriter::Get(0);

	cout<<xw->Init(outfile);
	cout << xw->AddVideoStream();
	cout << xw->AddAudioStream();

	FILE* fp = fopen(rgbfile, "rb");
	if (!fp)
	{
		cout << "open " << rgbfile << " failed!" << endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "open " << rgbfile << " success!" << endl;
	}
	FILE* fa = fopen(pcmfile, "rb");
	if (!fa)
	{
		cout << "open " << pcmfile << " failed!" << endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "open " << pcmfile << " success!" << endl;
	}

	xw->WriteHead();

	int size = xw->inWidth*xw->inHeight * 4;
	unsigned char* rgb = new unsigned char[size];

	int asize = xw->nb_sample * xw->inChannles * 2;
	unsigned char* pcm = new unsigned char[asize];
	AVPacket *pkt = NULL;
	int len = 0;
	for (;;)
	{
		if (xw->IsVideoBefore())
		{
			len = fread(rgb, 1, size, fp);
			if (len <= 0)
			{
				break;
			}

			pkt = xw->EncodeVideo(rgb);
			if (pkt)
			{
				cout << ".";
			}
			else
			{
				cout << "-";
				continue;
			}

			if (xw->WriteFrame(pkt))
			{
				cout << "+";
			}
		}
		else
		{
			len = fread(pcm, 1, asize, fa);
			if (len <= 0)
			{
				break;
			}
			pkt = xw->EncodeAudio(pcm);

			xw->WriteFrame(pkt);
		}
	}

	xw->WriteEnd();

	delete rgb;
	rgb = NULL;
	cout << "====================end===================" << endl;

	


	getchar();
	return 0;
}