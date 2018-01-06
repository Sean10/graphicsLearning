#include <glut/glut.h>
static GLint ImageWidth;
static GLint ImageHeight;
static GLint PixelLength;
static GLubyte* PixelData;
#include <stdio.h>
#include <stdlib.h>

#define FileName "test.bmp"


void display(void)
{
    // 清除屏幕并不必要
    // 每次绘制时，画面都覆盖整个屏幕
    // 因此无论是否清除屏幕，结果都一样
    // glClear(GL_COLOR_BUFFER_BIT);
    // 绘制像素
    glDrawPixels(ImageWidth, ImageHeight,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
    // 完成绘制
    glutSwapBuffers();
}

void openBMP()
{
    FILE* pFile = fopen(FileName, "rb");
    if (pFile == 0)
        exit(0);
    // 读取图象的大小信息
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
    fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);
    // 计算像素数据长度
    PixelLength = ImageWidth * 3;
    while (PixelLength % 4 != 0)
        ++PixelLength;
    PixelLength *= ImageHeight;
    // 读取像素数据
    PixelData = (GLubyte*)malloc(PixelLength);
    if (PixelData == 0)
        exit(0);
    fseek(pFile, 54, SEEK_SET);
    fread(PixelData, PixelLength, 1, pFile);
    // 关闭文件
    fclose(pFile);
}

void SetupRC()
{
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	// 设置逆时针环绕的多边形为正面
	glFrontFace(GL_CCW);
	// 启用背面剔除
	glEnable(GL_CULL_FACE);
	// 启用光照计算
	glEnable(GL_LIGHTING);
	// 指定环境光强度（RGBA）
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// 启用颜色追踪
	glEnable(GL_COLOR_MATERIAL);
	// 设置多边形正面的环境光和散射光材料属性，追踪glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// 指定清除色
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

int main(int argc, char* argv[])
{
    // 打开文件
    openBMP();
    // 初始化GLUT并运行
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ImageWidth, ImageHeight);
    glutCreateWindow(FileName);

    SetupRC();

    glutDisplayFunc(&display);
    glutMainLoop();
    return 0;
}
