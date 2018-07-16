/*
 * OpenCV 3.1.0 安装环境测试
 * 测试IDE：Clion 2017.3(Linux) CMake 3.9 gcc 6.3
 * 功能实现：打开外置摄像头/dev/video1捕获1920x1080图像
 * 视频保存：按1保存写入，按2停止写入，按esc退出
 * code by </MATRIX>@Neod Anderjon
 */
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string video_save_path = "/home/neod/Downloads/opencv_camera_test.avi";

int main (int argc, char *argv[])
{
    //打印标志
    printf("</MATRIX> opencv c++ test project\n");

    VideoCapture cap;										//创建摄像捕获
    VideoWriter wrt;										//视频流文件写入
    Mat frame;												//创建视频流

    cap.open(1);								            //打开摄像头
    //cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('D', 'I', 'V', 'X'));
    cap.set(CV_CAP_PROP_FPS, 60);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    int video_weight = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int video_height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    Size frame_size(video_weight, video_height);    //获得帧的宽高
    double frame_fps = cap.get(CV_CAP_PROP_FPS);    //获取fps
    //创建视频文件
    wrt.open(video_save_path,                       //视频保存路径
             CV_FOURCC('D', 'I', 'V', 'X'),         //保存格式
             frame_fps,                             //FPS
             frame_size,                            //保存大小
             true);                                 //仅保存彩色画面

    //判断是否正常打开
    if (!cap.isOpened())
    {
        printf("Camera open error\n");

        return -1;
    }

    while (true)
    {
        cap >> frame;										//将摄像头读取到frame流中
        if (frame.empty())									//判断成功与否
        {
            printf("Read frame stream error\n");
            break;
        }
        imshow("cvDemoMonitor#1", frame);					//显示图像

        //按'1'开始写入视频流，按'2'结束
        static bool startVideoflag = false;
        if (waitKey(10) == '1' && !startVideoflag)
        {
            startVideoflag = true;							//开始录像标志
            printf("Start Write Video to File\n");
        }
        else if (waitKey(10) == '2' && startVideoflag)
        {
            startVideoflag = false;
            wrt.release();									//回收进程
            printf("Finish Write Video to File\n");
        }

        if (startVideoflag)
            wrt.write(frame);								//持续写入

        //esc退出
        if (waitKey(20) == 27)
        {
            //回收进程关闭窗口
            cap.release();
            wrt.release();
            cvDestroyAllWindows();

            break;
        }
    }
    printf("code by </MATRIX>@Neod Anderjon\n");

    return 0;
}
