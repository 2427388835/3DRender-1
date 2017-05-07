// 3DRender.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "3DRender.h"
#include "Device.h"
#include "Camera.h"


//��Ļ��Ⱥ͸߶�   
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BITS = 32;				//ÿ�����ص�λ��  
const int MAX_LOADSTRING = 100;

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HDC screen_hdc;
HWND screen_hwnd;
HDC hCompatibleDC; //����HDC  
HBITMAP hCompatibleBitmap; //����BITMAP  
HBITMAP hOldBitmap; //�ɵ�BITMAP         
BITMAPINFO binfo; //BITMAPINFO�ṹ��  

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void RenderInit();
void RenderMain();
void RenderEnd();
void Display();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY3DRENDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DRENDER));
	RenderInit();
	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		RenderMain();
		
	}
	RenderEnd();

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DRENDER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   RECT  rectProgram, rectClient;
   GetWindowRect(hWnd, &rectProgram);   //��ó��򴰿�λ����Ļ����
   GetClientRect(hWnd, &rectClient);      //��ÿͻ�������
   //�ǿͻ�����,��
   int nWidth = rectProgram.right - rectProgram.left - (rectClient.right - rectClient.left);
   int nHeiht = rectProgram.bottom - rectProgram.top - (rectClient.bottom - rectClient.top);
   nWidth += SCREEN_WIDTH;
   nHeiht += SCREEN_HEIGHT;
   rectProgram.right = nWidth;
   rectProgram.bottom = nHeiht;
   int showToScreenx = GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth / 2;    //���д���
   int showToScreeny = GetSystemMetrics(SM_CYSCREEN) / 2 - nHeiht / 2;
   MoveWindow(hWnd, showToScreenx, showToScreeny, rectProgram.right, rectProgram.bottom, false);

   memset(&binfo, 0, sizeof(BITMAPINFO));
   binfo.bmiHeader.biBitCount = BITS;      //ÿ�����ض���λ��Ҳ��ֱ��д24(RGB)����32(RGBA)  
   binfo.bmiHeader.biCompression = BI_RGB;
   binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
   binfo.bmiHeader.biPlanes = 1;
   binfo.bmiHeader.biSizeImage = 0;
   binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   binfo.bmiHeader.biWidth = SCREEN_WIDTH;

   //��ȡ��ĻHDC  
   screen_hwnd = hWnd;
   screen_hdc = GetDC(screen_hwnd);

   //��ȡ����HDC�ͼ���Bitmap,����Bitmapѡ�����HDC(ÿ��HDC�ڴ�ÿʱ�̽���ѡ��һ��GDI��Դ,GDI��ԴҪѡ��HDC���ܽ��л���)  
   hCompatibleDC = CreateCompatibleDC(screen_hdc);
   hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
   hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*----------------------------------------Render------------------------------------------*/
//��Ƶ����
UINT Buffer[SCREEN_WIDTH*SCREEN_HEIGHT];
//��Ⱦװ��
Device *device;
//���
UVNCamera *camera;
//�����õ�������
Objecet *object;
//8������
Point3D mesh[8] = {
	{ 1, -1, 1, 1 },
	{ -1, -1, 1, 1 },
	{ -1,  1, 1, 1 },
	{ 1,  1, 1, 1 },
	{ 1, -1, -1, 1 },
	{ -1, -1, -1, 1 },
	{ -1,  1, -1, 1 },
	{ 1,  1, -1, 1 },
};

UINT color[8] = { 0x00FF0000 , 0x0000FF00 , 0x000000FF , 0x00FF0000 , 0x0000FF00 , 0x000000FF , 0x00FF0000 , 0x0000FF00 };
//12��������
Triangle t1(mesh[0], mesh[1], mesh[2], color[0], color[1], color[2]);
Triangle t2(mesh[2], mesh[3], mesh[1], color[2], color[3], color[1]);

Triangle t3(mesh[6], mesh[5], mesh[4], color[6], color[5], color[4]);
Triangle t4(mesh[4], mesh[7], mesh[6], color[4], color[7], color[6]);

Triangle t5(mesh[7], mesh[4], mesh[0], color[7], color[4], color[0]);
Triangle t6(mesh[0], mesh[3], mesh[7], color[0], color[3], color[7]);

Triangle t7(mesh[1], mesh[5], mesh[6], color[1], color[5], color[6]);
Triangle t8(mesh[6], mesh[2], mesh[1], color[6], color[2], color[1]);

Triangle t9(mesh[5], mesh[1], mesh[0], color[5], color[1], color[0]);
Triangle t10(mesh[0], mesh[4], mesh[5], color[0], color[4], color[5]);

Triangle t11(mesh[3], mesh[2], mesh[6], color[3], color[2], color[6]);
Triangle t12(mesh[6], mesh[7], mesh[3], color[6], color[7], color[3]);
//�������б�
Triangle list[12] = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 };

void RenderMain()
{
	device->Render();
	Display();
	Sleep(33);
}

void RenderInit()
{
	Point3D objectPosition = { -2.0f, 1.7f, 4, 1 };

	Point3D camerPos = { 0.0, 0.0f, 0, 1 };
	Vector3D v = { 0, 1, -0.5, 0 };
	camera = new UVNCamera(camerPos, objectPosition, v, 2, 4, 90, 1, SCREEN_WIDTH * 1.0f / SCREEN_HEIGHT);

	device = new Device(camera, SCREEN_WIDTH, SCREEN_HEIGHT, Buffer, RENDER_STATE_WIREFRAME, 0xFFFFFFFF, 0xFFFF0000);

	object = new Objecet(objectPosition, 12, list);
	device->AddObjectList(object);
}

void RenderEnd()
{
	device->ClearObjectList();
	delete object;
	delete device;
	delete camera;
	
}

void Display()
{
	//����ɫ���ݴ�ӡ����Ļ�ϣ���������������ÿ֡���õ���  
	SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, Buffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
}