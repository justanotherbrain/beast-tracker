// OpenCV offline
#include "flycapture/FlyCapture2.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <highgui.h>


using namespace FlyCapture2;
using namespace cv;
using namespace std;


Vec<int,4> coordinates;

int min_dist_slider_max = 100;
int min_dist_slider;
int min_dist;

int canny_threshold_slider_max = 255;
int canny_threshold_slider;
int canny_threshold;

int center_threshold_slider_max = 255;
int center_threshold_slider;
int center_threshold;

int min_radius_slider_max = 99;
int min_radius_slider;
int min_radius;

int max_radius_slider_max = 100;
int max_radius_slider;
int max_radius;

int med_blur_slider_max = 255;
int med_blur_slider;
int med_blur;

int bin_threshold_slider_max = 255;
int bin_threshold_slider;
int bin_threshold;

bool isDrawing = false;
Point start, end;

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M%S", &tstruct);

    return buf;
}


void drawBox(Point start, Point end, Mat& img){
	imshow("set",img);
	Scalar color = (255,0,0);
	rectangle(img, start, end, color, 10, 8, 0);
	return;
}

void mouseEvent(int evt, int x, int y, int flags, void* param){
	if(isDrawing){
		if(evt==CV_EVENT_LBUTTONUP){
	    		printf("up %d %d\n",x,y);
	        	isDrawing = false;
		        end.x = x;
		        end.y = y;
		        cv::Mat* image  = static_cast<cv::Mat *>(param);
			drawBox(start, end, *image);
			coordinates[2] = end.x-start.x;
			coordinates[3] = end.y-start.y;
	        	return;
    		}
	}
	else{
		if(evt==CV_EVENT_LBUTTONDOWN){
	        	printf("down %d %d\n",x,y);
		        isDrawing = true;
			start.x = x;
		        start.y = y;
			coordinates[0] = start.x;
			coordinates[1] = start.y;
        		return;
    		}
	}
}

void min_dist_trackbar(int, void*){
	if (min_dist_slider==0){
		min_dist_slider=1;
	}
	min_dist = (int) min_dist_slider;
}

void canny_threshold_trackbar(int, void*){
	if (canny_threshold_slider==0){
		canny_threshold_slider=1;
	}
	canny_threshold = (int) canny_threshold_slider;
}

void center_threshold_trackbar(int, void*){
	if (center_threshold_slider==0){
		center_threshold_slider=1;
	}
	center_threshold = (int) center_threshold_slider;
}

void min_radius_trackbar(int, void*){
	if (min_radius_slider==0){
		min_radius_slider=1;
	}
	min_radius = (int) min_radius_slider;
}

void max_radius_trackbar(int,void*){
	max_radius = (int) max_radius_slider;
	min_radius_slider_max = max_radius-1;
}

void med_blur_trackbar(int,void*){
	if (med_blur_slider % 2 == 0){
		med_blur_slider=med_blur_slider+1;
	}
	if (med_blur_slider < 1){
		med_blur_slider=1;
	}
	med_blur = (int) med_blur_slider;
}

void bin_threshold_trackbar(int,void*){
	bin_threshold = (int) bin_threshold_slider;
}

int main()
{
	// save file
	cout << "Choose a file name to save to. Defaults to current date and time... \n \n";
	
	string input = "";
	string filename;
	getline(cin, input);
	if (input == ""){
		filename = currentDateTime();
	}
	else{
		filename = input;
	}
	filename.append(".csv");
	const char *fn = filename.c_str();
	
	ofstream save_file (fn);

	Error error;
	Camera camera;
	CameraInfo camInfo;

	// Connect to the camera
	error = camera.Connect(0);
	if(error != PGRERROR_OK){
		std::cout << "failed to connect to camera..." << std::endl;
		return false;
	}

	error = camera.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK){
		std::cout << "failed to get camera info from camera" << std::endl;
		return false;
	}

	std::cout << camInfo.vendorName << " "
			<< camInfo.modelName << " "
			<< camInfo.serialNumber << std::endl;
	
		error = camera.StartCapture();
	if(error==PGRERROR_ISOCH_BANDWIDTH_EXCEEDED){
		std::cout << "bandwidth exceeded" << std::endl;
		return false;
	}
	else if (error != PGRERROR_OK){
		std::cout << "failed to start image capture" << std::endl;
		return false;
	}
	

	cout << "click and drag on image to select reference size\n";
	cout << "press c to continue\n";
	char kb = 0;	
	namedWindow("set",1);
	
	Image tmpImage;
	Image rgbTmp;
	cv::Mat tmp;
	while(kb != 'c'){
		Error error = camera.RetrieveBuffer(&tmpImage);
		if (error != PGRERROR_OK){
			std::cout<< "capture error" << std::endl;
			return false;
		}

		tmpImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgbTmp);

		unsigned int rowBytes = (double)rgbTmp.GetReceivedDataSize()/(double)rgbTmp.GetRows();

		tmp = cv::Mat(rgbTmp.GetRows(),rgbTmp.GetCols(),CV_8UC3,rgbTmp.GetData(),rowBytes);
	
		imshow("set",tmp);
	
	        cvSetMouseCallback("set", mouseEvent, &tmp);
		if (coordinates[3] != '\0'){
			drawBox(start, end, tmp);
			imshow("set",tmp);
		}
		kb = cvWaitKey(30);
	}
	if (coordinates[0] == 0 and coordinates[1] == 0){
		coordinates[0] = 0;
		coordinates[1] = 0;
		coordinates[2] = tmp.cols;
		coordinates[3] = tmp.rows;
	}
	else{
		if (coordinates[0] > coordinates[2]){
			int hold = coordinates[0];
			coordinates[0] = coordinates[2]+coordinates[0];
			coordinates[2] = hold+coordinates[0];
		}
		if (coordinates[1] > coordinates[3]){
			int hold = coordinates[1];
			coordinates[1] = coordinates[3]+coordinates[1];
			coordinates[3] = hold+coordinates[1];
		}
	}
	//double fps = cap.get(CV_CAP_PROP_FPS);
	int dp = 1;
	//int min_dist = sqrt( pow(tmp.rows,2) + pow(tmp.cols,2));
	min_dist = 1;
	min_dist_slider = 1;
	canny_threshold = 10;
	canny_threshold_slider = 10;
	center_threshold = 10;
	center_threshold_slider = 10;
	
	max_radius = 140;
	max_radius_slider = 100;
	max_radius_slider_max = min(coordinates[2]-coordinates[0],coordinates[3]-coordinates[1]);
	min_radius = 75;
	min_radius_slider = 75;
	min_radius_slider_max = max_radius_slider_max-1;
	bin_threshold = 21;
	bin_threshold_slider = 21;
	med_blur = 75;
	med_blur_slider_max = min(coordinates[2]-coordinates[0],coordinates[3]-coordinates[1])-10;
	med_blur_slider = 75;
	Mat window;
	Rect myROI(coordinates[0],coordinates[1],coordinates[2],coordinates[3]);
	Vec<float,2> offset;
	offset[0] = coordinates[0];
	offset[1] = coordinates[1];
	namedWindow("window",1);
	namedWindow("filtered",1);
	// caputer loop
	//char key = 0;
	bool refresh = true;
	
	
	// make sliders
	createTrackbar("Min Distance", "filtered", &min_dist_slider,min_dist_slider_max,min_dist_trackbar);
	createTrackbar("Canny Threshold", "filtered", &canny_threshold_slider, canny_threshold_slider_max, canny_threshold_trackbar);
	createTrackbar("Center Threshold", "filtered", &center_threshold_slider,center_threshold_slider_max, center_threshold_trackbar);
	createTrackbar("Min Radius", "filtered", &min_radius_slider,min_radius_slider_max, min_radius_trackbar);
	createTrackbar("Max Radius", "filtered", &max_radius_slider,max_radius_slider_max, max_radius_trackbar);
	createTrackbar("Median blur", "filtered", &med_blur_slider, med_blur_slider_max, med_blur_trackbar);
	createTrackbar("Bin Threshold", "filtered", &bin_threshold_slider, bin_threshold_slider_max, bin_threshold_trackbar);
	char key = 0;
	while(key != 'q')
	{
		
		
		Image rawImage;
		Error error = camera.RetrieveBuffer( &rawImage );
		if (error != PGRERROR_OK ){
			std::cout << "capture error" << std::endl;
			continue;
		}
		
		Image rgbImage;
		rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage);
		// convert to OpenCV Mat
		unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();
		Mat image = Mat(rgbImage.GetRows(), rgbImage.GetCols(),CV_8UC3, rgbImage.GetData(),rowBytes);

		//Mat image;
		//Mat i1, i2;
		//if(refresh)
		//{
		//	cap.grab();
		//	cap.retrieve(i1);
		//	cap.grab();
		//	cap.retrieve(i2);
		//} 
		//cap.retrieve(image);
		//addWeighted(i1, 0.5, i2, 0.5, 0.0, image);
		
		// convert to gray
		
		Mat image_gray;
		image_gray = image(myROI);

		cvtColor( image_gray, image_gray, CV_BGR2GRAY);
		medianBlur(image_gray,image_gray,med_blur);
		

		threshold(image_gray, image_gray, bin_threshold, 255, THRESH_BINARY);
		GaussianBlur( image_gray, image_gray, Size(9, 9), 2, 2);
		vector<Vec3f> circles;
		
		//Apply the Hough Transform to find the circles
		HoughCircles( image_gray, circles, CV_HOUGH_GRADIENT, dp, min_dist, canny_threshold, center_threshold, min_radius, max_radius);
		
		//Draw circles detected
		float x=0;
		float y=0;
		float r=0;
		float centerX;
		float centerY;
		if (circles.size()>0){
			for( size_t i=0; i< circles.size(); i++)
			{
				x=x+circles[i][0];
				y=y+circles[i][1];
				r=r+circles[i][2];
			}
		
			centerX=cvFloor(x/circles.size()+offset[0]);
			centerY=cvFloor(y/circles.size()+offset[1]);
			//float centerX=circles[0][0]+offset[0];
			//float centerY=circles[0][1]+offset[1];	
		
			Point center(cvRound(centerX), cvRound(centerY));
			
			int radius = cvRound(r/circles.size());
			// circle center
			circle(image,center,3,Scalar(0,255,0),-1,8,0);
			// circle outline
			circle(image,center,radius,Scalar(0,0,255),3,8,0);
		}
		else{
			cout << "eye-blink";
		}	
		
		save_file << centerX << "," << centerY << endl;

		imshow("window",image);
		imshow("filtered",image_gray);
		key = waitKey(30);
	}
	
	save_file.close();	

}
