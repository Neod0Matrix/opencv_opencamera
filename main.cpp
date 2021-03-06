/*
 * OpenCV 3.1.0 安装环境测试
 * 测试IDE：Clion 2017.3 (Linux) CMake 3.9 gcc 6.3
 *			Visual Studio 2013 (Windows) 
 * 功能实现：打开摄像头(优先外部)尝试捕获1920x1080图像
 * 视频保存：按1保存写入，按2停止写入，按esc退出
 * code by </MATRIX>@Neod Anderjon
 */
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//设置视频保存地址，以系统区分
#if defined(_WIN32)
string video_save_path{ "E:\\OperationCache\\opencv_camera_test.avi" };
#elif defined(__linux__)
string video_save_path{ "/home/neod/Downloads/opencv_camera_test.avi" };
#endif

int main (int argc, char *argv[]) {
    printf("</MATRIX> opencv c++ test project\n");

    VideoCapture cap;										//创建摄像捕获
    VideoWriter wrt;										//视频流文件写入
    Mat frame;												//创建视频流

    //打开摄像头，先测试外部摄像头1，失败转为内部摄像头0
    cap.open(1);
    if (!cap.isOpened()) {
        printf("Extra camera#1 open error, try inner camera#0\n");
        cap.open(0);
        if (!cap.isOpened()) {
            printf("Inner camera#0 open error, program exit\n");
            return -1;
        }
    }
    //设置摄像头输出画面
    //cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('D', 'I', 'V', 'X'));
    cap.set(CV_CAP_PROP_FPS, 60);
	//尝试以1920x1080分频率打开，达不到的摄像头会自动调整到合适最大尺寸
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);					
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	//static_cast<int>C++数据类型强制转换
    int video_weight = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int video_height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    Size frame_size(video_weight, video_height);            //获得帧的宽高
    double frame_fps = cap.get(CV_CAP_PROP_FPS);            //获取fps
    //创建视频文件，通用MPEG-4格式
	wrt.open(video_save_path, CV_FOURCC('D', 'I', 'V', 'X'), frame_fps, frame_size, true);

    while (true) {
		cap.read(frame);									//将摄像头读取到frame流中						
        //判断成功与否
        if (frame.empty()) {
            printf("Read frame stream error\n");
            break;
        }
        imshow("cvDemoMonitor#1", frame);					//显示图像

        //按'1'开始写入视频流，按'2'结束，\a声音提示
        static bool startVideoflag = false;
        if (waitKey(20) == '1' && !startVideoflag) {
            startVideoflag = true;							//开始录像标志
            printf("\aStart write video to file\n");
			//打印实际的视频信息
			printf("Video weight: %d height: %d fps: %.2lf\n", 
				video_weight, video_height, frame_fps);
			//string特性
			printf("Video file save to %s\n", video_save_path.c_str());
        }
        else if (waitKey(20) == '2' && startVideoflag) {
            startVideoflag = false;
            wrt.release();									//回收进程
            printf("\aFinish write video to file\n");
        }

        if (startVideoflag)
            wrt.write(frame);								//持续写入

        //esc退出
        if (waitKey(20) == 27) {
            cap.release();					
            wrt.release();
            cvDestroyAllWindows();							//回收进程关闭窗口

            break;
        }
    }
    printf("code by </MATRIX>@Neod Anderjon\n");

    return 0;
}
