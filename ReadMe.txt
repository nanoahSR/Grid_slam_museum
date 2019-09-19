数﻿据集
德意志博物馆Deutsches Museum的2D激光SLAM数据，链接：
https://google-cartographer-ros.readthedocs.io/en/latest/data.html
更换数据源要修改以下参数：
	map size：range
	step size：gap 

依赖项
　　1.c++ 11
  2.opencv 3.4.7
  3.Eigen3

运行
　修改　main.cpp 中 path　的数据集路径
  
　　cd Grid_slam_museum
　　mkdir build
  cd build
  cmake ..
  make
  ./Museum

  运行结束后，按任意键退出．

修改记录
2019/09/19
  修改 main.cpp 增加变量 path，用于存储数据集路径；　
  修改 occupid.h/cpp 修改语句；
　所有变量增加初始化赋值．
