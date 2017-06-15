# BUPT图形学大三作业

**作业一：**

绘制直线段、任意圆弧、椭圆弧、多边形区域的阴影填充和颜色填充等功能，并利用该库生成一个二维图形场景，其中包含编写者的名字。

**作业二：**

试设计一个室内三维环境，并利用OPEN GL展示它的三维效果。要求：
* 包含基本的实体元素：球，多面体，椎体，柱体，曲面等；
* 有全局光照和纹理功能
* 程序具有交互功能

**环境配置：**

Mac OS下自带glut，编译时添加选项`-framework OpenGL -framework`即可。

完整编译命令：
```
clang++  -framework OpenGL -framework GLUT 2.cpp -o 2
```
