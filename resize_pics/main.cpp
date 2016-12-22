//  读取文件夹下的图片生成视频文件
//  Author：www.icvpr.com
//  Blog：  http://blog.csdn.net/icvpr  

#include <iostream>
#include <string>
#include <io.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#define PI 3.14
using namespace std;
using namespace cv;

void rotateImage(Mat img, Mat & img_rotate, double degree)
{
	Mat rotMat(2, 3, CV_32FC1);
	double rad = degree*PI / 180.0;
	Point2d center = Point2d(img.cols / 2, img.rows / 2);
	circle(img, center, 5, Scalar(255, 0, 0), -1);
	//imshow("img origin", img);
	//waitKey(0);
	double angle = degree;
	double scale = 1;
	double newx = abs(sin(rad)*img.rows) + abs(cos(rad)*img.cols);
	double newy = abs(sin(rad)*img.cols) + abs(cos(rad)*img.rows);
	cout << newx << " " << newy << " " << img.cols << " " << img.rows << endl;
	// 求得旋转矩阵
	rotMat = getRotationMatrix2D(center, angle, scale);
	//cout << "The original rotate Mat is " << rotMat << endl;
	rotMat.at<double>(0, 2) += (newx - img.cols) / 2;
	rotMat.at<double>(1, 2) += (newy - img.rows) / 2;
	//cout << "Real rotate image is " << rotMat << endl;
	// 旋转已缩放后的图像
	warpAffine(img, img_rotate, rotMat, Size(int(newx),int(newy)));

}
int main(int argc, char** argv)
{
	// 图片集
	string fileFolderPath = "..\\images";
	//string fileFolderPath = "..\\test_photo";
	string fileExtension = "jpg";
	string fileFolder = fileFolderPath + "\\*." + fileExtension;

	// 输出视频
	//string outputVideoName = "output.avi";

	// openCV video writer
	//VideoWriter writer;
	char filenames[50] = { 0 };
	//int codec = 0;
	//int frameRate = 25;
	//Size frameSize;


	// 遍历文件夹
	char fileName[1000];

	struct _finddata_t fileInfo;    // 文件信息结构体

	// 1. 第一次查找
	long findResult = _findfirst(fileFolder.c_str(), &fileInfo);
	if (findResult == -1)
	{
		_findclose(findResult);
		return -1;
	}

	// 2. 循环查找
	do
	{
		sprintf(fileName, "%s\\%s", fileFolderPath.c_str(), fileInfo.name);

		if (fileInfo.attrib == _A_ARCH)  // 是存档类型文件
		{
			Mat frame;
			Mat resized_frame;
			Mat rotate_frame;
			frame = imread(fileName,1);
			/*imshow("windows", frame);
			waitKey(1);*/
			if (!frame.empty())
			{
				//resize(frame, resized_frame, Size(0, 0), 0.2, 0.2, INTER_LINEAR);
				rotateImage(frame, rotate_frame, 90.0);
				/*imshow("windows", resized_frame);
				waitKey(1);*/
				sprintf(filenames, "..\\ResizedImage\\%s", fileInfo.name);
				imwrite(filenames, rotate_frame);
				//imshow("windows", rotate_frame);
				waitKey(0);
				cout << "write a photo" << fileInfo.name << endl;
			}
			//Mat frame;
			//frame = imread(fileName);    // 读入图片
			//if (!writer.isOpened())
			//{
			//	frameSize.width = frame.cols;
			//	frameSize.height = frame.rows;

			//	if (!writer.open(outputVideoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, frameSize, true))
			//	{
			//		cout << "open writer error..." << endl;
			//		return -1;
			//	}
			//}

			//// 将图片数据写入
			//writer.write(frame);

			//// 显示
			//imshow("video", frame);
			//waitKey(frameRate);
		}

	} while (!_findnext(findResult, &fileInfo));


	_findclose(findResult);


	return 0;
}