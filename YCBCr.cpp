#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<cmath>
#include<string>
#include<ctime>
#include "detection_contours.h"

using namespace cv;
using namespace std;

int main(){

  // Début du programme
  clock_t tstart, tend;
  tstart=clock();
  /*Lecture des images en entrée*/
  
  // Un oeil à côté
  //Mat image=imread("base_image/image.jpg", CV_LOAD_IMAGE_COLOR);
  //Deux yeux en face 
  //Mat image=imread("base_image/image_en_face2.png", CV_LOAD_IMAGE_COLOR);
  // Seulement la partie des yeux en face
  //Mat image=imread("base_image/yeux_en_face.png", CV_LOAD_IMAGE_COLOR);
  
  //Les données à tester de notre bases
  //Mat image=imread("base_image/Amandine1.jpg", CV_LOAD_IMAGE_COLOR);//8
  Mat image=imread("base_image/Amandine2.jpg", CV_LOAD_IMAGE_COLOR);//8
  //Mat image=imread("base_image/Chloe1.jpg", CV_LOAD_IMAGE_COLOR);//8
  //Mat image=imread("base_image/Chloe2.jpg", CV_LOAD_IMAGE_COLOR); //pas bon 
  //Mat image=imread("base_image/Elise1.jpg", CV_LOAD_IMAGE_COLOR);//20
  //Mat image=imread("base_image/Elise2.jpg", CV_LOAD_IMAGE_COLOR);// pas bon
  //Mat image=imread("base_image/Fanny1.jpg", CV_LOAD_IMAGE_COLOR);//30
  //Mat image=imread("base_image/Fanny2.jpg", CV_LOAD_IMAGE_COLOR);// ne peut rien détecter, flou
  //Mat image=imread("base_image/Fanny3.jpg", CV_LOAD_IMAGE_COLOR);// ne peut rien détecter, fermé
  
  //Mat image=imread("base_image/Laurianne1.jpg", CV_LOAD_IMAGE_COLOR); //25
  //Mat image=imread("base_image/Laurianne2.jpg", CV_LOAD_IMAGE_COLOR); // ne peut rien détecter, fermé
  
  int element_ero_dila=15;
  
  if(!image.data){
    cout<<"Image non trouvée, veuillez entrer changer le nom d'image"<<endl;
    return -1;
  }

  resize(image, image, Size(), 0.2, 0.2);

  namedWindow("Image reize", WINDOW_NORMAL);
  resizeWindow("Image resize", image.rows, image.cols);
  imshow("Image resize", image);
  
  

  Mat B,G,R;
  vector<Mat> channelsBGR;
  channelsBGR=decomposition_BGR(image);
  
  
  B=channelsBGR.at(0);
  G=channelsBGR.at(1);
  R=channelsBGR.at(2);

  // création du cananl alpha
  Mat alpha=B+G+R;
  
  /*Afficher les composant B, G, R*/
  afficher_composant_B(B, "B");
  afficher_composant_G(G, "G");
  afficher_composant_R(R, "R");  
  
    
  //BGR to HSV
  Mat H,S,V;
  vector<Mat> channelsHSV=decomposition_HSV(image);
 
  H=channelsHSV.at(0);
  S=channelsHSV.at(1);
  V=channelsHSV.at(2);

  



  //BGR to YCBCr
  Mat YCBCr, Y, Cb, Cr;
  vector<Mat> channelsYCBCr=decomposition_YCBCR(image);
  cvtColor(image, YCBCr, CV_BGR2YCrCb);
  Y=channelsYCBCr.at(0);
  Cr=channelsYCBCr.at(1);
  Cb=channelsYCBCr.at(2);

  Mat Y_3D(Y.rows, Y.cols, CV_32FC3, Scalar(0,0,0));
  Mat Cr_3D(Cr.rows, Cr.cols, CV_32FC3, Scalar(0.0,0.0,0.0));
  Mat Cb_3D(Cb.rows, Cb.cols, CV_32FC3, Scalar(0.0,0.0,0.0));
  /*
  // Erreur de segmentation de cette partie d'affectation
  for(int i=0;i<Y_3D.rows;i++){
    for(int j=0;j<Y_3D.cols;j++){
      Y_3D.at<Vec3f>(i,j)[0]=Y.at<float>(i,j);
      cout<<Y_3D.at<Vec3b>(i,j)<<endl;
    }
  }
  
  for(int i=0;i<image.rows;i++){
    for(int j=0;j<image.cols;j++){
      Cr_3D.at<Vec3b>(i,j)[1]=Cr.at<uchar>(i,j);
    }
  }
   

  for(int i=0;i<image.rows;i++){
    for(int j=0;j<image.cols;j++){
      Cb_3D.at<Vec3b>(i,j)[2]=Cb.at<uchar>(i,j);
    }
  }
  */
  
  /*---------------------------------------------------------*/
  //Afficher l'image de YCBCr de l'image originale
  namedWindow("Image YCBCr", WINDOW_NORMAL);
  resizeWindow("Image YCBCr", YCBCr.rows, YCBCr.cols);
  imshow("Image YCBCr", YCBCr);
  
  /*
  // Affichage des composants Y,Cr,Cb de l'image originale
  // Y
  namedWindow("Y", WINDOW_NORMAL);
  resizeWindow("Y", Y.rows, Y.cols);
  imshow("Y", Y);

  // Cr
  namedWindow("Cr", WINDOW_NORMAL);
  resizeWindow("Cr", Cr.rows, Cr.cols);
  imshow("Cr", Cr);

  // Cb
  namedWindow("Cb", WINDOW_NORMAL);
  resizeWindow("Cb", Cb.rows, Cb.cols);
  imshow("Cb", Cb);
  */ 
 /*---------------------------------------------------*/

  //Décision de l'interval pour les parties de peaux

  // Initialisation de l'image binaire (la sortie)
  Mat image_binaire(image.rows, image.cols, CV_8UC1, Scalar(0));

  for(int i=0; i<image.rows;i++){
    /*
    uchar* Cr_adresse=Cr.ptr< uchar>(i);
    uchar* Cb_adresse=Cb.ptr< uchar>(i);
    uchar* Y_adresse=Y.ptr< uchar>(i);
    */
    //uchar* image_binaire_adresse=image_binaire.ptr< uchar>(i);
    
    for(int j=0; j<image.cols;j++){
      /*Vérification YCBCr*/
      //if((Cr_adresse[j]>135) && (Cr_adresse[j]<180) && (Cb_adresse[j]>85) && (Cb_adresse[j]<135)) 
      //image_binaire_adresse[j]=255;
      //if((Cr.at<uchar>(i,j)>135) && (Cr.at<uchar>(i,j)<180) && (Cb.at<uchar>(i,j)>85) && (Cb.at<uchar>(i,j)<135) && Y.at<uchar>(i,j)>80)
      //if((Cr.at<uchar>(i,j)>133) && (Cr.at<uchar>(i,j)<173) && (Cb.at<uchar>(i,j)>77) && (Cb.at<uchar>(i,j)<127))


      ///*raisonable*/
      if((Cr.at<uchar>(i,j)>133) && (Cr.at<uchar>(i,j)<173) && (Cb.at<uchar>(i,j)>80) && (Cb.at<uchar>(i,j)<120))


      //if((R.at<uchar>(i,j)>95) && (G.at<uchar>(i,j)>40) && (B.at<uchar>(i,j)>20) && (R.at<uchar>(i,j)>B.at<uchar>(i,j)) && (R.at<uchar>(i,j)>G.at<uchar>(i,j)) && (abs(R.at<uchar>(i,j)-G.at<uchar>(i,j))>15) && (Cr.at<uchar>(i,j)>135) && (Cb.at<uchar>(i,j)>85) && (Y.at<uchar>(i,j)>80) && (Cr.at<uchar>(i,j)<=1.5862*Cb.at<uchar>(i,j)+20) && (Cr.at<uchar>(i,j)>=0.3448*Cb.at<uchar>(i,j)+76.2069) && (Cr.at<uchar>(i,j)>=-4.5652*Cb.at<uchar>(i,j)+234.5652) && (Cr.at<uchar>(i,j)<=-1.15*Cb.at<uchar>(i,j)+301.75) && (Cr.at<uchar>(i,j)<=-2.2857*Cb.at<uchar>(i,j)+432.85) && (alpha.at<uchar>(i,j)>15))


      /*Vérification HSV*/
      //if((H.at<uchar>(i,j)>=0) && (H.at<uchar>(i,j)<=50) && (S.at<uchar>(i,j)>=0.23) && (S.at<uchar>(i,j)<=0.68) && (R.at<uchar>(i,j)>95) && (G.at<uchar>(i,j)>40) && (B.at<uchar>(i,j)>20) && (R.at<uchar>(i,j)>B.at<uchar>(i,j)) && (R.at<uchar>(i,j)>G.at<uchar>(i,j)) && (alpha.at<uchar>(i,j)>15))
      //if((H.at<uchar>(i,j)>=6) && (H.at<uchar>(i,j)<=38)){
	image_binaire.at<uchar>(i,j)=255;
    }
  }
  
    
    //Affichage de l'image binaire
    namedWindow("Image binaire", WINDOW_NORMAL);
    resizeWindow("Image binaire", image_binaire.rows, image_binaire.cols);
    imshow("Image binaire", image_binaire); 
    
    
    //Essayer le système HSV aussi pour tester
    
    
    //Effet blur
    Mat image_binaire_blur;
    blur(image_binaire, image_binaire_blur, Size(20,20));

    
    /*-----------------------------------------------------*/
    //Affichage de l'image binaire après blur
    namedWindow("Image binaire après blur", WINDOW_NORMAL);
    resizeWindow("Image binaire après blur", image_binaire_blur.rows, image_binaire_blur.cols);
    imshow("Image binaire après blur", image_binaire_blur); 
    /*-----------------------------------------------------*/

    
    //Extractoin de contour Canny
    Mat edge_non_blur, edge_blur;
    Canny(image_binaire, edge_non_blur, 3,50,3);
    Canny(image_binaire_blur, edge_blur, 3,50,3);

    
    /*-----------------------------------------------------*/
    //Affichage contours de l'image binaire avant et après blur
    namedWindow("Contours après blur", WINDOW_NORMAL);
    resizeWindow("Contours après blur", edge_blur.rows, edge_blur.cols);
    imshow("Contours après blur", edge_blur); 
    /*-----------------------------------------------------*/


    /*-----------------------------------------------------*/
    //Affichage contours de l'image binaire avant et après blur
    namedWindow("Contours avant blur", WINDOW_NORMAL);
    resizeWindow("Contours avant blur", edge_non_blur.rows, edge_non_blur.cols);
    imshow("Contours avant blur", edge_non_blur); 
    /*-----------------------------------------------------*/


    // Dilatation de l'image binaire obtenue
    clock_t ero_dila_start, ero_dila_end;
    ero_dila_start=clock();
    Mat image_binaire_dilate, element;
    element=getStructuringElement(MORPH_ELLIPSE, Size(element_ero_dila, element_ero_dila));
    //MORPH_RECT, MORPH_CROSS
    
    dilate(image_binaire, image_binaire, element);
    erode(image_binaire, image_binaire_dilate, element);
    ero_dila_end=clock();
    cout<<"Le temps de clacul pour éroder+dilater est: "<<(ero_dila_end-ero_dila_start)/(double)CLOCKS_PER_SEC<<" secondes."<<endl;
    
    //Affichage image binaire dilaté
    namedWindow("Image binaire dilatée", WINDOW_NORMAL);
    resizeWindow("Image binaire dilatée", image_binaire_dilate.rows, image_binaire_dilate.cols);
    imshow("Image binaire dilatée", image_binaire_dilate); 
    
    //l'extraction de contours après faire la dilatation
    Mat edge_dilate;
    Canny(image_binaire_dilate, edge_dilate, 3,3,3);

    //Affichage contours après faire la dilatation
    namedWindow("Contours image binaire dilatée", WINDOW_NORMAL);
    resizeWindow("Contours image binaire dilatée", edge_dilate.rows, edge_dilate.cols);
    imshow("Contours image binaire dilatée", edge_dilate); 
    

    //Harris corner detection
    int blockSize=2;
    int apertureSize=3;
    double k=0.04;
    int seuil=150;
    Mat edge_dilate_coin, edge_coin_norm, edge_coin_norm_scaled;
    cornerHarris(edge_dilate, edge_dilate_coin, blockSize, apertureSize, k, BORDER_DEFAULT);
    //Normalizer
    normalize(edge_dilate_coin, edge_coin_norm, 0,255,NORM_MINMAX, CV_32FC1, Mat());
    //convertScaleAbs(edge_coin_norm, edge_coin_norm_scaled);
    for(int  i =0;i<edge_coin_norm.rows;i++){
      for(int j=0;j<edge_coin_norm.cols;j++){
	if((int)edge_coin_norm.at<float>(i,j)>seuil)
	  circle(edge_coin_norm, Point(j,i), 5, Scalar(0), 2,8,0);
      }
    }

    namedWindow("Harris coin détection des contours d'oeil", WINDOW_NORMAL);
    resizeWindow("Harris coin détection des contours d'oeil", edge_coin_norm.rows, edge_coin_norm.cols);
    imshow("Harris coin détection des contours d'oeil", edge_coin_norm);


    //L'ajout de rectangle de haar cascade
    string fichier_cascade="/opt/opencv/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    vector<Rect> eyes=cascade_contour(image, fichier_cascade);
    
    
    //Ajouter des contours sur l'image originale

    //Mat image_contour_ensemble(image.rows, image.cols, CV_8UC3, Scalar(0,0,0));
    //image.copyTo(image_contour_ensemble);
    for(int i=0;i<image.rows;i++){
      for(int j=0;j<image.cols;j++){
	if(edge_dilate.at<uchar>(i,j)!=0){
	  image.at<Vec3b>(i,j)[0]=255;
	  image.at<Vec3b>(i,j)[1]=255;
	  image.at<Vec3b>(i,j)[2]=255;
	}
      }
    }

    //Affichage image originale avec contours
    namedWindow("Image originale avec contours ajoutés", WINDOW_NORMAL);
    resizeWindow("Image originale avec contours ajoutés", image.rows, image.cols);
    imshow("Image originale avec contours ajoutés", image);


    //Les points sur les contours des eyeliners
    
    vector<Point> points=points_contour(edge_dilate, eyes);// Les points sur les contours des eyeliners
    for (int i=0;i<points.size();i++){
      int row=(int)points[i].y;
      int col=(int)points[i].x;
      image.at<Vec3b>(row,col)[0]=0;
      image.at<Vec3b>(row,col)[1]=0;
      image.at<Vec3b>(row,col)[2]=255;
      //cout<<x<<"  "<<y<<endl;
      /*
      image.at<Vec3b>(10, 100)[0]=255;
      image.at<Vec3b>(10, 100)[1]=0;
      image.at<Vec3b>(10, 100)[2]=0;
      */
    }
    

   
    //Affichage image originale avec contours
    namedWindow("Image contours points", WINDOW_NORMAL);
    resizeWindow("Image contours points", image.rows, image.cols);
    imshow("Image contours points", image);

    //Séparation les deux droite pour former un angle d'oeil
    vector<vector<Point> > droite_separe=separer_doite(image, points);
    
    vector<Point> droite_haut=droite_separe[0];
    vector<Point> droite_bas=droite_separe[1];

    int taille_droite1_garde_debut=droite_haut.size()/3;
    int taille_droite2_garde_debut=droite_bas.size()/3;
    int taille_droite1_garde_fin=droite_haut.size()/5;
    int taille_droite2_garde_fin=droite_bas.size()/5;

    
    vector<Point> droite1(droite_haut.begin()+taille_droite1_garde_debut, droite_haut.end()-taille_droite1_garde_fin);
    vector<Point> droite2(droite_bas.begin()+taille_droite2_garde_debut, droite_bas.end()-taille_droite2_garde_fin);

    for(int i=0;i<droite1.size();i++){
      int row=(int)droite1[i].y;
      int col=(int)droite1[i].x;
      //cout<<row<<"  "<<col<<endl;
      
      image.at<Vec3b>(row,col)[0]=255;
      image.at<Vec3b>(row,col)[1]=0;
      image.at<Vec3b>(row,col)[2]=0;
      
    }

    
    for(int i=0;i<droite2.size();i++){
      int row=(int)droite2[i].y;
      int col=(int)droite2[i].x;
      //cout<<row<<"  "<<col<<endl;
      
      image.at<Vec3b>(row,col)[0]=0;
      image.at<Vec3b>(row,col)[1]=255;
      image.at<Vec3b>(row,col)[2]=0;
      
    }
    /*
    image.at<Vec3b>(106,246)[0]=0;
    image.at<Vec3b>(106,246)[1]=0;
    image.at<Vec3b>(106,246)[2]=0;
    */
    
    //Affichage image originale avec contours
    namedWindow("Image droite separée", WINDOW_NORMAL);
    resizeWindow("Image droite separée", image.rows, image.cols);
    imshow("Image droite separée", image);
      

    // On garde pour chaque droite la partie 1/3 qu mileu de chaque ensemble de points, pour enlever les parties qui nous donne des erreurs
   


    Vec4f line_haut, line_bas;
    fitLine(droite1, line_haut, CV_DIST_L2, 0,0.01,0.01);
    fitLine(droite2, line_bas, CV_DIST_L2, 0,0.01,0.01);
    
    Point p1_haut(0,line_haut[3]-(line_haut[1]*line_haut[2]/line_haut[0]));
    Point p2_haut(image.cols, (image.cols-line_haut[2])*line_haut[1]/line_haut[0]+line_haut[3]);
    
    Point p1_bas(0,line_bas[3]-(line_bas[1]*line_bas[2]/line_bas[0]));
    Point p2_bas(image.cols, (image.cols-line_bas[2])*line_bas[1]/line_bas[0]+line_bas[3]);
    
    
    line(image, p1_haut, p2_haut, Scalar(255,255,255), 1);
    line(image, p1_bas, p2_bas, Scalar(255,255,255), 1);

    tend=clock();
    cout<<"Le temps de calcul pour une seule image est: "<<(tend-tstart)/(double)CLOCKS_PER_SEC<<" seconds."<<endl;
    

    //Affichage image originale avec contours
    namedWindow("Image droite separée droite ", WINDOW_NORMAL);
    resizeWindow("Image droite separée droite ", image.rows, image.cols);
    imshow("Image droite separée droite ", image);

    //Calcul l'angle d'oeil
    //L'intersection des deux droites
    Point intersection=intersection_deux_droite(p1_haut, p2_haut, p1_bas, p2_bas);

    //Afficher le point d'intersection
    image.at<Vec3b>(intersection.y, intersection.x)[0]=0;
    image.at<Vec3b>(intersection.y, intersection.x)[1]=0;
    image.at<Vec3b>(intersection.y, intersection.x)[2]=0;

    float angle=angle_deux_droite(p1_haut, p1_bas, intersection);
    cout<<"L'angle d'oeil détecté est: "<<angle<<" degré."<<endl;


    
    //Affichage image originale avec contours
    namedWindow("Image droite separée droite ", WINDOW_NORMAL);
    resizeWindow("Image droite separée droite ", image.rows, image.cols);
    imshow("Image droite separée droite ", image);
    
    
    waitKey(0);
    return 0;
    
}
