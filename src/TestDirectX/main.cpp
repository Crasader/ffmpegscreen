#include <d3d9.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "d3d9.lib")

//截取全屏
void CaptureScreen(void *data)
{
	//1 创建directx3d的对象
	static IDirect3D9 *d3d = NULL;
	if (!d3d)
	{
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
	}
	if (!d3d) return;

	//2 创建显卡设备对象
	static IDirect3DDevice9 *device = NULL;
	if (!device)
	{
		D3DPRESENT_PARAMETERS pa;
		ZeroMemory(&pa, sizeof(pa));
		pa.Windowed = true;//设置为窗口程序
		pa.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		pa.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pa.hDeviceWindow = GetDesktopWindow();
		d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &pa, &device);
	}

	if (!device) return;

	//3 创建离屏表面
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	static IDirect3DSurface9 *sur = NULL;
	if (!sur)
	{	
		device->CreateOffscreenPlainSurface(w, h,
			D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &sur, 0);
	}

	if (!sur) return;

	//4 抓屏
	device->GetFrontBufferData(0, sur);

	//5 取出数据
	D3DLOCKED_RECT rect;
	ZeroMemory(&rect, sizeof(rect));
	if (sur->LockRect(&rect, 0, 0) != S_OK)
	{
		return;
	}
	
	memcpy(data, rect.pBits, w*h*4);

	sur->UnlockRect();
	cout << ".";
}

int main()
{
	FILE *fp = fopen("out.rgb", "wb");
	int size = 1920 * 1080 * 4;
	char* buf = new char[size];
	
	for (int i = 0; i < 100; i++)
	{
		CaptureScreen(buf);
		fwrite(buf, 1, size, fp);
		Sleep(100);
	}

	return 0;
}