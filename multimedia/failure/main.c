#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <math.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;//64位编译器会影响到，所以不能用long

//位图文件头定义;
//其中不包含文件类型信息（由于结构体的内存结构决定，
//要是加了的话将不能正确读取文件信息）
typedef struct  tagBITMAPFILEHEADER{
	//WORD bfType;//文件类型，必须是0x424D，即字符“BM”
	DWORD bfSize;//文件大小
	WORD bfReserved1;//保留字
	WORD bfReserved2;//保留字
	DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	DWORD biSize;//信息头大小
	LONG biWidth;//图像宽度
	LONG biHeight;//图像高度
	WORD biPlanes;//位平面数，必须为1
	WORD biBitCount;//每像素位数
	DWORD  biCompression; //压缩类型
	DWORD  biSizeImage; //压缩图像大小字节数
	LONG  biXPelsPerMeter; //水平分辨率
	LONG  biYPelsPerMeter; //垂直分辨率
	DWORD  biClrUsed; //位图实际用到的色彩数
	DWORD  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER; //位图信息头定义

typedef struct tagRGBQUAD{
	BYTE rgbBlue; //该颜色的蓝色分量
	BYTE rgbGreen; //该颜色的绿色分量
	BYTE rgbRed; //该颜色的红色分量
	BYTE rgbReserved; //保留值
}RGBQUAD;//调色板定义

//像素信息
typedef struct tagIMAGEDATA
{
	BYTE blue;
	//BYTE green;
	//BYTE red;
}IMAGEDATA;

#define PI 3.14159//圆周率宏定义
#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度

BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256色调色板
BITMAPINFOHEADER strInfo;

void showBMPFILEHEADER(BITMAPFILEHEADER strHead)
{
	printf("%x\n", strHead.bfSize);
	printf("%x\n", strHead.bfReserved1);
	printf("%x\n", strHead.bfReserved2);
	printf("%x\n", strHead.bfOffBits);

}

void showBMPINFOHEADER(BITMAPINFOHEADER strInfo)
{
	printf("%x\n", strInfo.biSize);
	printf("%d\n", strInfo.biWidth);
	printf("%d\n", strInfo.biHeight);
	printf("%x\n", strInfo.biPlanes);
	printf("%x\n", strInfo.biBitCount);
	printf("%x\n", strInfo.biCompression);
	printf("%x\n", strInfo.biSizeImage);
	printf("%d\n", strInfo.biXPelsPerMeter);
	printf("%d\n", strInfo.biYPelsPerMeter);
	printf("%x\n", strInfo.biClrUsed);
	printf("%x\n", strInfo.biClrImportant);
}

int main()
{
	freopen("in.txt", "r", stdin);

	char fileName[LENGTH_NAME_BMP];
	IMAGEDATA *imagedata = NULL;//动态分配存储原图片的像素信息的二维数组
    IMAGEDATA *imagedataRot = NULL;//动态分配存储旋转后的图片的像素信息的二维数组
    int width,height;//图片的宽度和高度
	printf("请输入要读取的文件名");
	scanf("%s", fileName);
	FILE *fpi, *fpw;
	fpi = fopen(fileName, "rb");
	if( fpi == NULL)
	{
		printf("file open error\n");
		return -1;
	}
	WORD bfType;
	fread(&bfType, 1,sizeof(WORD), fpi);
	//printf("%x", bfType);
	if(0x4d42 != bfType)
	{
		printf("the file is not a bmp file");
		return -1;
	}

	fread(&strHead, 1, sizeof(BITMAPFILEHEADER), fpi);
	//showBmpHead(strHead);//显示文件头
    fread(&strInfo,1,sizeof(BITMAPINFOHEADER),fpi);
        //showBmpInforHead(strInfo);//显示文件信息头
	showBMPFILEHEADER(strHead);
	showBMPINFOHEADER(strInfo);
        //读取调色板
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
    {
            fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);
            fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);
            fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);
            fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);
    }

        width = strInfo.biWidth;
        height = strInfo.biHeight;
        //图像每一行的字节数必须是4的整数倍
        width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
        //imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
        imagedata = (IMAGEDATA*)malloc(width * height);
        imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
        //初始化原始图片的像素数组
        for(int i = 0;i < height;++i)
        {
            for(int j = 0;j < width;++j)
            {
                (*(imagedata + i * width + j)).blue = 0;
                //(*(imagedata + i * width + j)).green = 0;
                //(*(imagedata + i *  width + j)).red = 0;
            }
        }
        //初始化旋转后图片的像素数组
        for(int i = 0;i < 2 * height;++i)
        {
            for(int j = 0;j < 2 * width;++j)
            {
                (*(imagedataRot + i * 2 * width + j)).blue = 0;
                //(*(imagedataRot + i * 2 * width + j)).green = 0;
                //(*(imagedataRot + i * 2 * width + j)).red = 0;
            }
        }
        fseek(fpi,54,SEEK_SET);
        //读出图片的像素数据
        fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);
        fclose(fpi);

    //图片旋转处理
    int RotateAngle;//要旋转的角度数
    double angle;//要旋转的弧度数
    int midX_pre,midY_pre,midX_aft,midY_aft;//旋转所围绕的中心点的坐标
    midX_pre = width / 2;
    midY_pre = height / 2;
    midX_aft = width;
    midY_aft = height;
    int pre_i,pre_j,after_i,after_j;//旋转前后对应的像素点坐标
    printf("输入要旋转的角度（0度到360度，逆时针旋转）：\n");
    scanf("%d", &RotateAngle);
    angle = 1.0 * RotateAngle * PI / 180;
    for(int i = 0;i < 2 * height;++i)
    {
        for(int j = 0;j < 2 * width;++j)
        {
            after_i = i - midX_aft;//坐标变换
            after_j = j - midY_aft;
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;
            if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)//在原图范围内
                *(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);
        }
    }

    //保存bmp图片
    if((fpw=fopen("b.bmp","wb"))==NULL)
    {
        printf("create the bmp file error!");
        return -1;
    }
    WORD bfType_w=0x4d42;
    fwrite(&bfType_w,1,sizeof(WORD),fpw);
    //fpw +=2;
    fwrite(&strHead,1,sizeof(BITMAPFILEHEADER),fpw);
     strInfo.biWidth = 2 * width;
     strInfo.biHeight = 2 * height;
    fwrite(&strInfo,1,sizeof(BITMAPINFOHEADER),fpw);
    //保存调色板数据
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
    {
        fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);
        fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);
        fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);
        fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);
    }
    //保存像素数据
    for(int i =0;i < 2 * height;++i)
    {
        for(int j = 0;j < 2 * width;++j)
        {
            fwrite( &((*(imagedataRot + i * 2 * width + j)).blue),1,sizeof(BYTE),fpw);
            //fwrite( &((*(imagedataRot + i * 2 * width + j)).green),1,sizeof(BYTE),fpw);
            //fwrite( &((*(imagedataRot + i * 2 * width + j)).red),1,sizeof(BYTE),fpw);
        }
    }
    fclose(fpw);

    //释放内存
    free(imagedata);
    free(imagedataRot);

}
