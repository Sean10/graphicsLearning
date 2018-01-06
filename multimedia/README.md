# 数字媒体 课程设计
大四 BUPT 专业选修

##要求：

显示一个bmp文件的C程序，并实现图像亮度、对比度调整、图像平移、放大、旋转和镜像。


## 环境依赖：
* openCV
```
opengl 编译 opentest.cc
clang++ open_test.cc -o gl_test -framework OpenGL -framework GLUT

#opencv 编译 test_cv.cc
clang++ $(pkg-config --cflags --libs opencv) test_cv.cc -o test_cv
```


.
├── README.md
├── failure
│   ├── gl_test.cc
│   ├── main.c
│   ├── new.cc
│   ├── new_main.cc
│   ├── test.bmp
│   └── test_gray.bmp
├── open_test.cc
└── test_cv.cc

test_cv.cc是最终使用的处理代码文件
原计划用failure/main.c按照bmp格式进行算法编写操作，不过因为仅仅只是显示就需要对linux的framebuffer设备进行操作，之后的处理过程又需要写其他的算法，过于耗时，就暂时放下了。
