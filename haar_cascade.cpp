#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>
#include<vector>
#include"detection_contours.h"
#include<ctime>
#include<algorithm>
#include<math.h>

using namespace std;
using namespace cv;



vector<Rect> cascade_contour(Mat image, string fichier_cascade){
  clock_t tstart_cascade, tend_cascade;
  tstart_cascade=clock();
  vector<Rect> eyes;
  string window_name="Rectangle_haar_cascade";
  CascadeClassifier eye_cascade;
  if(!eye_cascade.load(fichier_cascade))
    cout<<"Cascasde file not found!!!"<<endl;
  else{
    Mat image_gray;
    cvtColor(image, image_gray, CV_BGR2GRAY);
    equalizeHist(image_gray, image_gray);//augmenter le contrast pour l'image en noir et blanc

    //Plusieurs oeil peuvent être trouvés, mais pour notre image, on a seulement un oeil
    
    eye_cascade.detectMultiScale(image_gray, eyes, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(3,3));
    
    
    for (size_t j = 0; j < eyes.size(); j++){
      Rect rect(eyes[j].x, eyes[j].y, eyes[j].width, eyes[j].height);
      rectangle(image, rect, Scalar(0, 255, 0), 2, 8, 0);
    }

    tend_cascade=clock();
    cout<<"Le temps de calcul pour haar cascade est:"<< (tend_cascade - tstart_cascade)/(double)CLOCKS_PER_SEC <<" seconde."<<endl;
        
    namedWindow(window_name, 2);
    imshow(window_name, image);
  }
  return eyes;
}

// Point(x,y), Rect(x,y), représentent (col, row)
// image.at<>(i,j) représente (row, col)
vector<Point> points_contour(Mat contour, vector<Rect> eyes){
  vector<Point> points;
  cout<<contour.rows<<endl;
  cout<<contour.cols<<endl;
  for(int i=0;i<1;i++){
    for(int x=eyes[i].x;x<eyes[i].x+eyes[i].width;x++){
      for(int y=eyes[i].y;y<eyes[i].y+eyes[i].height;y++){
	//cout<<x<<" "<<y<<endl;
	if(contour.at<uchar>(y,x)==255)
	  points.push_back(Point(x,y));
      }
    }
  }
  return points;  
}


vector<vector<Point> > separer_doite(Mat image, vector<Point> points){
  vector<vector<Point> > points_droite(2);
  int row=image.rows;
  int col=image.cols;
  
  for(int y=0;y<col;y++){
    vector<int> tmp;
    vector<Point> tmp_point;
    for(int i=0;i<points.size();i++){
      if(points[i].x==y){
	tmp_point.push_back(points[i]);
	tmp.push_back(points[i].y);
	//points.erase(i);
      }
    }
    if(tmp.size()>=2){
      vector<int>::iterator max=max_element(tmp.begin(), tmp.end());
      vector<int>::iterator min=min_element(tmp.begin(), tmp.end());

      //int position=distance(begin(tmp), max);
      points_droite[0].push_back(Point(y, *max));// Pour la droite en haut
      points_droite[1].push_back(Point(y, *min)); // Pour la droite en bas
    }
    vector<int>(tmp).swap(tmp);// ramasser le vecteur tmp
    vector<Point>(tmp_point).swap(tmp_point);//ramasser le vexteur tmp_point
  }
  return points_droite;
  
}

Point intersection_deux_droite(Point p1_1, Point p1_2, Point p2_1, Point p2_2){
  int x,y;
  int X1=p1_1.x-p1_2.x;
  int Y1=p1_1.y-p1_2.y;
  int X2=p2_1.x-p2_2.x;
  int Y2=p2_1.y-p2_2.y;

  if(X1*Y2==X2*Y1)
    return Point(int((p1_2.x+p2_1.x)/2), int((p1_2.y+p2_1.y)/2));

  int A=X1*p1_1.y-Y1*p1_1.x;
  int B=X2*p2_1.y-Y2*p2_1.x;
  //X1*y=(Y1*x+A)
  //X2*y=(Y2*x+B)

  y=(A*Y2-B*Y1)/(X1*Y2-X2*Y1);
  x=(B*X1-A*X2)/(Y1*X2-Y2*X1);

  return Point(x,y);


}
float angle_deux_droite(Point &p1, Point &p2, Point &intersection){

  float theta=atan2(p1.x -intersection.x, p1.y-intersection.y)-atan2(p2.x-intersection.x, p2.y-intersection.y);
  if(theta>CV_PI)
    theta -= 2*CV_PI;
  if(theta<-CV_PI)
    theta += 2*CV_PI;

  theta=theta*180.0/CV_PI;
  return theta;
}







