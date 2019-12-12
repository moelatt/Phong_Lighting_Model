#ifndef MAIN_H
#define MAIN_H
#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include  <GL/glut.h>
#endif
#include "glm/glm.hpp"
#include <cstdio>
#include <fstream>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;
float *PixelBuffer,	*halfToneColor, Ia, Il, K, n; 
int screen_width = 770, screen_height = 770; 
glm::vec3 light, fromPoint, ka, kd, ks, normal;  
int choose = -1, turnOnOff = 0, switchs = 0;

void readFile();
void KeyBoardFunc(unsigned char key, int x, int y);  
void MouseFunc(int button, int state, int x, int y);  
void MotionFunc(int x, int y); 
void display();
void reshape (int w, int h);
float* halfToning(); 
float *phongOff();
void drawStringOnWin();
void drawWinLabel();
void drawWinLabelWithHT(); 
void drawWinLabelWithPhongOff();
void sliderFunction();  
void drawSliders();
void polyObjects();  
void DepthSort(char x);
void DrawPix(int x, int y, float r, float g, float b);
void DrawHalfTonePix(int x, int y, float r, float g, float b);
void DrawPhongOffPix(int x, int y, float r, float g, float b);
void renderString(float x, float y, const char* format, size_t length, char);
void rotationMethod(double x1, double x2, double y1, double y2, double z1, double z2, double degree);
void animation(int i);
struct PhongModel {
	float x, y, z, r, g, b;
	vector <glm::vec3> normals;
	glm::vec3 normal, PhongVec;    
	PhongModel(float, float, float); 
	void facesNormalize(glm::vec3 normal); 
	glm::vec3 toXYZvector(); 
	void normalization(); 
	void PhongLightingModel(glm::vec3 fromPoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
					  float, float, float, float); 
	void normalizeRGB(float rgbmax); 
	void convertPixelWorld(char);
};
struct drawLine{
	PhongModel *start, *end;
	drawLine(PhongModel* start, PhongModel* end); 
	float DDALineIntersect(int, float, float, float, float);
	glm::vec3 interceptRGB(int, int, float, float, float, float);
	int renderIntercept(int, char);
	void renderGouraudLine(char);
	glm::vec3 rgbCrossPoints(int, int, char);
};
struct gouraudShading{
	PhongModel *point1, *point2, *point3;
	glm::vec3 n;  
	vector<drawLine*> edges; 
	gouraudShading(PhongModel* P1, PhongModel* P2, PhongModel* P3);
	float renderMinMax(char);
	void renderGouraudShading(char);
	void renderGouraudEdge(char);
	void facesNormalization(); 
};
struct object3D {
	vector<PhongModel*> vertices;
	vector<PhongModel*> colorVertex;
	vector<gouraudShading*> faces;
	void normalize(); 
	void PhongLightingModel(glm::vec3 viewpoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
					  float n, float Ia, float IL, float K); 
	void normalizeRGB(); 
	void changePixelAWorld(char);
	friend istream& operator>>(istream& is, object3D& object);
};
struct UI{
	float x, y, max, min, length, position;
	string str;
	UI(float x, float y, float max, float min, float length, string name);
	void drawSlider();
	void MoveSlider(int);
	float changedValue();
	int renderMinMaxCoor(char);
};
vector<object3D*> polygon3D;
vector<gouraudShading*> objectFaces; 
vector<int> specularity;
vector<UI> UIslide;
#endif