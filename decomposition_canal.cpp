#include "detection_contours.h"
#include<vector>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>


using namespace std;
using namespace cv;

vector<Mat> decomposition_BGR(Mat image){
	vector<Mat> canauxBGR;
	split(image, canauxBGR);
	return canauxBGR;
}

vector<Mat> decomposition_YCBCR(Mat image){
	vector<Mat> canauxYCBCR;
	Mat YCBCR;
	cvtColor(image, YCBCR, CV_BGR2YCrCb);
	split(YCBCR, canauxYCBCR);
	return canauxYCBCR;
}

vector<Mat> decomposition_HSV(Mat image){
	vector<Mat> canauxHSV;
	Mat HSV;
	cvtColor(image, HSV, CV_BGR2HSV);
	split(HSV, canauxHSV);
	return canauxHSV;
}

//Affichage de composant B,G,R, Y, CB, CR
void afficher_composant_B(Mat composant, string nom_composant){
	Mat composant_3D(composant.rows, composant.cols, CV_8UC3, Scalar(0,0,0));
	for(int i=0;i<composant.rows;i++){
    	for(int j=0;j<composant.cols;j++){
      		composant_3D.at<Vec3b>(i,j)[0]=composant.at<uchar>(i,j);
    	}
  	}
  namedWindow(nom_composant, WINDOW_NORMAL);
  resizeWindow(nom_composant, composant.rows, composant.cols);
  imshow(nom_composant, composant_3D);
}

void afficher_composant_G(Mat composant, string nom_composant){
	Mat composant_3D(composant.rows, composant.cols, CV_8UC3, Scalar(0,0,0));
	for(int i=0;i<composant.rows;i++){
    	for(int j=0;j<composant.cols;j++){
      		composant_3D.at<Vec3b>(i,j)[1]=composant.at<uchar>(i,j);
    	}
  	}
  namedWindow(nom_composant, WINDOW_NORMAL);
  resizeWindow(nom_composant, composant.rows, composant.cols);
  imshow(nom_composant, composant_3D);
}

void afficher_composant_R(Mat composant, string nom_composant){
	Mat composant_3D(composant.rows, composant.cols, CV_8UC3, Scalar(0,0,0));
	for(int i=0;i<composant.rows;i++){
    	for(int j=0;j<composant.cols;j++){
      		composant_3D.at<Vec3b>(i,j)[2]=composant.at<uchar>(i,j);
    	}
  	}
  namedWindow(nom_composant, WINDOW_NORMAL);
  resizeWindow(nom_composant, composant.rows, composant.cols);
  imshow(nom_composant, composant_3D);
}






















