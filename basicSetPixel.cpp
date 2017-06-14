#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define MAX 256

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
void drawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
void drawCircle(HDC, int xc, int yc, int x, int y, COLORREF color);
void drawCircleBresenham(HDC, int xc, int yc, int r, COLORREF color);
void EllipseArc(HDC hdc, int x, int y, int a, int b, COLORREF color);
void ColorFill(HDC hdc, int x, int y, COLORREF old_color, COLORREF new_color);
void swap(int &x1, int &x2);
void FloodFill8(HDC hdc, int x, int y, COLORREF boundaryColor, COLORREF newcolor);

typedef struct aet {
	int x;
	int deltaX;
	int yMax;
	struct iet *next;
} AET;

void FloodFill8(HDC hdc, int x, int y, COLORREF boundaryColor, COLORREF newcolor) {
	COLORREF color = GetPixel(hdc, x, y);
	if (color != newcolor && color != boundaryColor) {
		SetPixel(hdc, x, y, newcolor);
		FloodFill8(hdc, x, y + 1, boundaryColor, newcolor);
		FloodFill8(hdc, x, y - 1, boundaryColor, newcolor);
		//FloodFill8(hdc, x - 1, y + 1, boundaryColor, newcolor);
		//FloodFill8(hdc, x - 1, y - 1, boundaryColor, newcolor);
		//FloodFill8(hdc, x + 1, y + 1, boundaryColor, newcolor);
		FloodFill8(hdc, x + 1, y - 1, boundaryColor, newcolor);
		FloodFill8(hdc, x - 1, y, boundaryColor, newcolor);
		FloodFill8(hdc, x + 1, y, boundaryColor, newcolor);
	}

}

void ColorFill(HDC hdc, int yStart, int yEnd, COLORREF old_color, COLORREF new_color)
{
	//for (int i = 0; i < xc; i++)
	//	for (int j = 0; j < yc; j++)
	//		if (GetPixel(hdc, i, j) == old_color) {
	//			SetPixel(hdc, i, j, new_color);
		//ColorFill(hdc, x + 1, y, old_color, new_color);
		//ColorFill(hdc, x, y + 1, old_color, new_color);
		//ColorFill(hdc, x, y - 1, old_color, new_color);
		//ColorFill(hdc, x - 1, y, old_color, new_color);
		//	}
	AET NET[MAX];
	for (int i = yStart; i < yEnd; i++)
	{
		NET[i] = { 0,0,0,NULL };


	}
}

void EllipseArc(HDC hdc, int x, int y, int a, int b,COLORREF color)
{
	double t = 0, dt = 0.001;
	int x1 = x + a, y1 = y, x2, y2;
	while (t < 2 * 3.14159) {
		t += dt;
		x2 = x + (int)(a * cos(t));
		y2 = y + (int)(b * sin(t));
		drawLineBresenham(hdc, x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}
}


void drawCircleBresenham(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	int x = 0;
	int	y = r;
	int d = 3 - 2*r;
	while (x < y)
	{
		drawCircle(hdc, xc, yc, x, y, color);
		if (d < 0)
		{
			d = d + 4 * x + 6;
		}
		else
		{
			d = d + 4 * (x - y) + 10;
			y = y - 1;
		}
		x = x + 1;
	}
	if (x == y)
		drawCircle(hdc, xc, yc, x, y, color);


}

void drawCircle(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	SetPixel(hdc, xc + x, yc - y, color);
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc - y, yc - x, color);
	SetPixel(hdc, xc - y, yc + x, color);
	SetPixel(hdc, xc + y, yc - x, color);
	SetPixel(hdc, xc + y, yc + x, color);
}

void drawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	bool step = abs(y2 - y1) > abs(x2 - x1);
	if (step)
	{
		swap(x1, y1);
		swap(x2, y2);
	}
	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}
	int deltaX = x2 - x1;
	int deltaY = abs(y2 - y1);
	int x = x1;
	int y = y1;
	int error = -deltaX;
	//float deltaError = (float)deltaY / (float)deltaX;
	int ystep;
	if (y2 < y1)
		ystep = -1;
	else
		ystep = 1;
	for (int i = 0; i < deltaX; i++)
	{
		if (step > 0)
		{
			SetPixel(hdc, y, x, color);
		}
		else
		{
			SetPixel(hdc, x, y, color);
		}
		error += deltaY*2;
		if (error >= 0.5)
		{
			y += ystep;
			error -= 2*deltaX;
		}
		x += 1;
	}
}

void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
	{
		fprintf(stderr, "Error location\n");
		return;
	}

	int k = abs(x2 - x1);
	if (k < abs(y2 - y1))
		k = abs(y2 - y1);
	int dx = abs(x2 - x1) / k;
	int dy = abs(y2 - y1) / k;
	int x = x1;
	int y = y1;
	for (int i = 0; i < k; i++)
	{
		SetPixel(hdc, x, y, color);
		x += dx;
		y += dy;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND hwnd = 0; //为什么应该初始化为0？
	PAINTSTRUCT paintstruct;
	MSG              msg;
	UINT              nWight = 320;    //窗口宽度
	UINT              nHeight = 400;    //窗口高度

	static TCHAR szAppName[] = TEXT("Graph");    //窗口类标题
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;                //窗口大小发生变化时自动刷新
	wndclass.lpfnWndProc = WndProc;                                //设置窗口过程(fnp)
	wndclass.cbClsExtra = 0;                                    //预留空间，字节为单位
	wndclass.cbWndExtra = 0;                                    //预留空间，字节为单位
	wndclass.hInstance = hInstance;                            //指想程序的实列句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);        //设置窗口图标样式
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);            //设置鼠标样式
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //设置窗口背景颜色
	wndclass.lpszMenuName = NULL;                                    //指定窗口菜单项NULL为空
	wndclass.lpszClassName = szAppName;                            //窗口类名称，这里指想字符串数组


	if (!RegisterClass(&wndclass))        //注册窗口类
	{
		MessageBox(NULL, TEXT("程序运行失败!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,                  // RegisterClass注册的窗口类名
		TEXT("Graph"), // 窗口标题
		WS_OVERLAPPEDWINDOW,        // 窗口样式   *
		CW_USEDEFAULT,                 // int x 窗口打开的X坐标
		CW_USEDEFAULT,                 // int y 窗口打开的Y坐标
		nWight,                     // int nWidth----窗口宽
		nHeight,                     // int nHeight---窗口高
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);                     // creation parameters

	ShowWindow(hwnd, iCmdShow);        //在显示器上显示窗口

	while (GetMessage(&msg, NULL, 0, 0))
	{
		//消息循环
		TranslateMessage(&msg);    //将击键消息转换为字符消息
		DispatchMessage(&msg);    //将消息回传给Windows，由Windows转发给适当的窗口过程
								  //这里的窗口过程为WndProc()函数
	}
	return msg.wParam;
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	//窗口过程函数
	HDC          hdc;
	PAINTSTRUCT ps;
	RECT         rect;

	switch (message)
	{
	case WM_PAINT:
		//InvalidateRect(hwnd,NULL,TRUE);

		hdc = BeginPaint(hwnd, &ps);    //创建设备描述表句柄

		GetClientRect(hwnd, &rect);
		//画‘王’
		drawLineDDA(hdc, 20, 100, 100, 100, RGB(0, 0, 0));
		drawLineBresenham(hdc, 30, 200, 90, 200, RGB(0, 0, 0));
		drawLineBresenham(hdc, 10, 300, 110, 300, RGB(0, 0, 0));
		drawLineBresenham(hdc, 60, 100, 60, 300, RGB(0, 0, 0 ));
		//drawCircleBresenham(hdc, 200, 200, 150, RGB(0, 0, 0));

		drawLineBresenham(hdc, 140, 120, 180, 120, RGB(0, 0, 0));
		drawLineBresenham(hdc, 130, 160, 190, 160, RGB(0, 0, 0));
		drawLineBresenham(hdc, 160, 250, 180, 250, RGB(0, 0, 0));
		drawLineBresenham(hdc, 160, 100, 160, 300, RGB(0, 0, 0));
		drawLineBresenham(hdc, 150, 200, 120, 300, RGB(0, 0, 0));
		drawLineBresenham(hdc, 135, 250, 160, 300, RGB(0, 0, 0));
		drawLineBresenham(hdc, 160, 300, 300, 300, RGB(0, 0, 0));

		drawLineBresenham(hdc, 200, 120, 300, 120, RGB(0, 0, 0));
		drawLineBresenham(hdc, 300, 120, 300, 200, RGB(0, 0, 0));
		drawLineBresenham(hdc, 300, 200, 280, 180, RGB(0, 0, 0));
		drawLineBresenham(hdc, 250, 120, 200, 200, RGB(0, 0, 0));
		drawLineBresenham(hdc, 200, 220, 300, 220, RGB(0, 0, 0));
		drawLineBresenham(hdc, 200, 290, 300, 290, RGB(0, 0, 0));
		drawLineBresenham(hdc, 200, 220, 200, 290, RGB(0, 0, 0));
		drawLineBresenham(hdc, 300, 220, 300, 290, RGB(0, 0, 0));
		//ColorFill(hdc,150,150,RGB(255,255,255),RGB(0,0,255));
		EllipseArc(hdc,200,200,50,150,RGB(0,0,0));
		//画一个多边形
		drawLineBresenham(hdc, 50, 300, 100, 10, RGB(0, 0, 0));
		drawLineBresenham(hdc, 100, 10, 150, 100, RGB(0, 0, 0));
		drawLineBresenham(hdc, 150, 100, 200, 10, RGB(0, 0, 0));
		drawLineBresenham(hdc, 200, 10, 250, 300, RGB(0, 0, 0));
		drawLineBresenham(hdc, 50, 300, 250, 300, RGB(0, 0, 0));
		FloodFill8(hdc, 250, 260, RGB(0, 0, 0), RGB(50, 50, 50));


		EndPaint(hwnd, &ps);    //释放设备描述表句柄
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam); //处理所有窗口过程不予处理的消息
}

void swap(int &x1, int &x2)
{
	int temp = x1;
	x1 = x2;
	x2 = temp;
}
