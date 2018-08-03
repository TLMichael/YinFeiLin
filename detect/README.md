# YinFeiLin
Linux+YOLO目标检测+模板跟踪+串口传输+http视频传输

## 注意事项
- CMakeLists.txt 中使用的 libdarknet.so，需要自行利用AlexeyAB的[darknet代码](https://github.com/AlexeyAB/darknet)编译得到，并放入/usr/local/lib目录中：
    ```shell
    mv darknet.so libdarknet.so
    sudo cp libdarknet.so /usr/local/lib/
    ```
- 模板跟踪算法被封装到template.h的类Template中，平台无关。
- 串口传输模块被封装到serial.h的类Serial中，仅限于Linux系统，Windows请参考[该仓库](https://github.com/TLMichael/Detect-Track/blob/master/beifen/serial.h)中的CSerial类。
- http视频传输模块被封装到http_stream.h中的类MJPGWriter中，仅适用于Linux系统，Windows系统请参考[darknet代码仓库](https://github.com/AlexeyAB/darknet)中的http_stream模块。该功能主要用于当嵌入式计算设备（tx2）被装在运动的机器上时，无法方便地通过HDMI实时显示tx2上摄像头的画面和检测结果，用ssh连接tx2也只能操控命令行，无法显示图形界面。要想实时显示tx2上摄像头的画面，则一种可行的方法是启用该http模块，将tx2的视频帧送入网络，另一台处于同一局域网中的电脑或手机可以通过访问http://192.168.xxx.xxx:xxxx（192.168.xxx.xxx代表tx2在局域网中的ip地址，可通过在tx2终端运行ifconfig命令得到，“:”后的xxxx代表设置的端口号，在main.cpp中设置）实时查看tx2上的画面。