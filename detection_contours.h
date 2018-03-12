#include<vector>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<vector>

using namespace std;
using namespace cv;

vector<Mat> decomposition_BGR(Mat image);
vector<Mat> decomposition_YCBCR(Mat image);
vector<Mat> decomposition_HSV(Mat image); 


//Affichage de composant B,G,R, Y, CB, CR
void afficher_composant_B(Mat composant, string nom_composant);
void afficher_composant_G(Mat composant, string nom_composant);
void afficher_composant_R(Mat composant, string nom_composant);


//partie haar cascade
vector<Rect> cascade_contour(Mat contour_nb, string fichier_cascade);
vector<Point> points_contour(Mat contour, vector<Rect>);
vector<vector<Point> > separer_doite(Mat image, vector<Point> points);//retourner les deux droites, les points pour chaque droite, après, on va utiliser ces deux ensemble de points pour faire la simulation des droite, puis, calculer l'angle entre ces deux droites

