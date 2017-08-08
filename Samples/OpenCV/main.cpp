#include <iostream>  
#include <OpenNI.h>  
#include <opencv2\core\core.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  
#include <opencv2\highgui\highgui.hpp>  

using namespace std;

int main()
{
	//定义oni文件中视频的总帧数以及得到的图片的保存目录  
	int total = 0;
	char* imagefile = "E:\\Project\\OpenNI\\code\\OpenNI2\\Bin\\x64-Debug\\pictures";

	//初始化OpenNI环境  
	openni::OpenNI::initialize();

	//声明设备并打开oni文件  
	openni::Device fromonifile;
	fromonifile.open("E:\\Project\\OpenNI\\code\\OpenNI2\\Bin\\x64-Debug\\bat201708081415.oni");

	//声明控制对象，这对视频流的控制起到了关键作用  
	openni::PlaybackControl* pController = fromonifile.getPlaybackControl();

	//声明视频流对象以及帧对象  
	openni::VideoStream streamColor;
	openni::VideoFrameRef frameColor;

	//验证是否有彩色传感器（是否有彩色视频）和建立与设备想关联的视频流  
	if (fromonifile.hasSensor(openni::SENSOR_DEPTH))
	{
		if (streamColor.create(fromonifile, openni::SENSOR_DEPTH) == openni::STATUS_OK)
		{
			cout << "create stream ok" << endl;
		}
		else
		{
			cerr << "ERROR: create stream fail" << endl;
			system("pause");
			return -1;
		}
	}
	else
	{
		cerr << "ERROR: no sensor" << endl;
		system("pause");
		return -1;
	}

	//建立显示窗口  


	//获取总的视频帧数并将该设备的速度设为-1以便能留出足够的时间对每一帧进行处理、显示和保存  
	total = pController->getNumberOfFrames(streamColor);
	pController->setSpeed(-1);

	//开启视频流  
	static string str = "Image";
	cv::namedWindow(str);
	streamColor.start();
	for (int i = 1; i <= total; ++i)
	{
		//读取视频流的当前帧  
		streamColor.readFrame(&frameColor);

		cout << "当前正在读的帧数是：" << frameColor.getFrameIndex() << endl;
		cout << "当前的循环次数是：  " << i << endl;
		cout << "Format of frame:" << frameColor.getVideoMode().getPixelFormat();

		//将帧保存到Mat中并且将其转换到BGR模式，因为在OpenCV中图片的模式是BGR  
		cv::Mat rgbImg(frameColor.getHeight(), frameColor.getWidth(), CV_16UC1, (void*)frameColor.getData());
		//cv::Mat bgrImg;
		//cvtColor(rgbImg, bgrImg, CV_RGB2BGR);

		//将每一帧按顺序帧保存到图片目录下  
		char imagefullname[255];
		char imagenum[50];
		sprintf_s(imagenum, "\\%03d.png", i);
		strcpy_s(imagefullname, imagefile);
		strcat_s(imagefullname, imagenum);
		cv::imwrite(imagefullname, rgbImg);

		//显示当前帧  
		cv::Mat gray8Img;
		rgbImg.convertTo(gray8Img, CV_8UC1, 1.0/256, 0);
		cv::equalizeHist(gray8Img, gray8Img);
		cv::imshow(str, gray8Img);
		cv::waitKey(150);
		//if (cv::waitKey(30) == 27)
		//{
		//	break;
		//}
	}

	//销毁显示窗口  
	//cv::destroyWindow(str);

	//关闭视频流  
	streamColor.destroy();

	//关闭设备  
	fromonifile.close();

	//关闭OpenNI  
	openni::OpenNI::shutdown();

	return 0;
}
