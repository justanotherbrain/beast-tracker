// opencv_flycap
#include "flycapture/FlyCapture2.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace FlyCapture2;
using namespace cv;

int main()
{
	Error error;
	Camera camera;
	CameraInfo camInfo;

	// Connect the camera
    error = camera.Connect( 0 );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to connect to camera" << std::endl;     
        return false;
    }
    
    // Get the camera info and print it out
    error = camera.GetCameraInfo( &camInfo );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to get camera info from camera" << std::endl;     
        return false;
    }
    std::cout << camInfo.vendorName << " "
    		  << camInfo.modelName << " " 
    		  << camInfo.serialNumber << std::endl;
	
	error = camera.StartCapture();
    if ( error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
    {
        std::cout << "Bandwidth exceeded" << std::endl;     
        return false;
    }
    else if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to start image capture" << std::endl;     
        return false;
    } 
	
	// capture loop
	char key = 0;
    while(key != 'q')
	{
		// Get the image
		Image rawImage;
		Error error = camera.RetrieveBuffer( &rawImage );
		if ( error != PGRERROR_OK )
		{
			std::cout << "capture error" << std::endl;
			continue;
		}
		
		// convert to rgb
	    	Image rgbImage;
        	rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );
       
		// convert to OpenCV Mat
		unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();       
		cv::Mat image = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(),rowBytes);
		
		// convert to gray
		Mat image_gray;
		cvtColor( image, image_gray, CV_BGR2GRAY);

		// find circle for iris:
		// Reduce noise so we avoid false circle detection
		GaussianBlur( image_gray, image_gray, Size(9, 9), 2, 2);
		
		vector<Vec3f> circles;

		//Apply the Hough Transform to find the circles
		HoughCircles( image_gray, circles, CV_HOUGH_GRADIENT, 1, image_gray.rows/8, 10, 20, 50, 500);
		
		//Draw circles detected
		for( size_t i=0; i< circles.size(); i++)
		{
			
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(image,center,3,Scalar(0,255,0),-1,8,0);
			// circle outline
			circle(image,center,radius,Scalar(0,0,255),3,8,0);
			
		}

		cv::imshow("image", image);
		key = cv::waitKey(30);        
	}
	
	error = camera.StopCapture();
    if ( error != PGRERROR_OK )
    {
        // This may fail when the camera was removed, so don't show 
        // an error message
    }  
	
	camera.Disconnect();
	
	return 0;
}
