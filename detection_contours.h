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

//Calcul l'angle d'oeil
// p1_1 et p1_2 sont les deux points sur la droite1, p2_1 et p2_2 sont deux points sur la droite2
Point intersection_deux_droite(Point p1_1, Point P1_2, Point p2_1, Point p2_2);
// p1 est un point sur la droite1, p2 est un point sur la droite2, intersection est le point d'intersection de la droite1 et la droite2
float angle_deux_droite(Point &p1, Point &p2, Point &intersection);


