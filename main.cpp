#include "main.h"
int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(50, 50);
	int MainWindow = glutCreateWindow("Project-3");
	readFile();
	PixelBuffer = new float[screen_width * screen_height * 3];
	polyObjects();
	sliderFunction();
	glutKeyboardFunc(KeyBoardFunc); 
	glutMouseFunc(MouseFunc); 
	glutMotionFunc(MotionFunc); 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
void readFile(){
	n = 5.0, Ia = 0.5f, Il = 0.5f, K = 1.0f;
	fromPoint = glm::vec3(0, 0, 0);
	light = glm::vec3(0, 0, 0);
	ka.r = 0.5f; ka.g = 0.5f; ka.b = 0.5f;
	kd.r = 0.5f; kd.g = 0.5f; kd.b = 0.5f;
	ks.r = 0.5f; ks.g = 0.5f; ks.b = 0.5f;
	int numOfObjects, i = 0;
	ifstream myFile;
	myFile.open("test.txt");
	myFile >> numOfObjects;
	while(i < numOfObjects){
		object3D *obj = new object3D();
		myFile >> *obj;
		obj->normalize(); int j = 0;
		while(j < obj->faces.size()){
			objectFaces.push_back(obj->faces.at(j));
			j = j + 1;
		}
		polygon3D.push_back(obj);
		i = i + 1;
	}
	myFile.close();
}
// Perspective Projections
void reshape (int w, int h){
		w = screen_width, h = screen_height;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, w, h);
		gluPerspective(37.0f, 1.0f * w / h, 1.0f, 50.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
}
istream& operator>>(istream& myFile, object3D& object) {
	int totalPoints, totalFaces, colorRead;
	float x, y, z, r,g,b, specular;
	int edgeA, edgeB, edgeC;
	myFile >> totalPoints;
	for (int i = 0; i < totalPoints; i++) {
		myFile >> x >> y >> z;
		PhongModel* XYZpoints = new PhongModel(x, y, z);
		object.vertices.push_back(XYZpoints);
	}
	for(int j = 0; j < totalPoints; j++){
		myFile >> r >> g >> b;
		r /= 255; g /= 255; b /= 255;
		PhongModel* color = new PhongModel(r,g,b);
		object.colorVertex.push_back(color);
	}
	myFile >> totalFaces;
	for (int i = 0; i < totalFaces; i++) {
		myFile >> edgeA >> edgeB >> edgeC;
		gouraudShading* objFaces = new gouraudShading(object.vertices[edgeA - 1], object.vertices[edgeB - 1], object.vertices[edgeC - 1]);
		objFaces->facesNormalization();
		object.faces.push_back(objFaces);
	}
	for(int k = 0; k < totalFaces; k++){
		myFile >> specular;
		specularity.push_back(specular);
	}
	return myFile;
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glViewport(0, 0, screen_width, screen_height);
	// Camera 
	gluLookAt(
		0.0f, 0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f 
	);
	glRasterPos3f(-1.5f, -1.5f, -1.5f);
	switch (turnOnOff){
		case 0:
			glDrawPixels(screen_width, screen_height, GL_RGB, GL_FLOAT, PixelBuffer); break;
		case 1:
			glDrawPixels(screen_width, screen_height, GL_RGB, GL_FLOAT, halfToneColor); break;
		case 2: 
			glDrawPixels(screen_width, screen_height, GL_RGB, GL_FLOAT, PixelBuffer); break;
		default: break;
	}
	drawStringOnWin();
	drawSliders();
	glFlush();
}
// ==============================================================================================================//
void drawWinLabel(){
	glColor3f(1, 1, 1);
	glViewport(0, screen_height / 2, screen_width / 2, screen_height / 2);
	string str = "Window-XY with Phong-On";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
	glViewport(screen_width / 2, screen_height / 2, screen_width / 2, screen_height / 2);
	str = "Window-XZ with Phong-On";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
	glViewport(0, 0, screen_width / 2, screen_height / 2);
	str = "Window-YZ with Phong-On";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
}
void drawWinLabelWithHT(){
	glColor3f(1, 1, 1);
	glViewport(0, screen_height / 2, screen_width / 2, screen_height / 2);
	string str = "Window-XY with Half-Tone";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
	glViewport(screen_width / 2, screen_height / 2, screen_width / 2, screen_height / 2);
	str = "Window-XZ with Half-Tone";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
	glViewport(0, 0, screen_width / 2, screen_height / 2);
	str = "Window-YZ with Half-Tone";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
}
void drawWinLabelWithPhongOff(){
	glColor3f(1, 1, 1);
	glViewport(0, screen_height / 2, screen_width / 2, screen_height / 2);
	string str = "Window-XY with Phong-Off";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
	glViewport(screen_width / 2, screen_height / 2, screen_width / 2, screen_height / 2);
	str = "Window-XZ with Phong-Off";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
	glViewport(0, 0, screen_width / 2, screen_height / 2);
	str = "Window-YZ with Phong-Off";
	renderString(-0.6f, 0.9f, str.c_str(), str.length(), 'l');
}
void drawStringOnWin() {
	switch (turnOnOff){
		case 0:
			drawWinLabel();break;
		case 1: 
			drawWinLabelWithHT(); break;
		case 2:
			drawWinLabelWithPhongOff(); break;
		default: break;
	}
	glViewport(0, 0, screen_width, screen_height);
	glLineWidth(1.5);
	glBegin(GL_LINES); // border
	glColor3f(0.3, 0.5, 0.6);
	glVertex2f(0, -0.99);
	glVertex2f(0, 0.99);
	glVertex2f(-0.99, 0);
	glVertex2f(0.99, 0);
	glEnd();
	glColor3f(1, 1, 1);
	glViewport(screen_width / 2, 0, screen_width / 2, screen_height / 2);
	string str = "Phong Lighting Model:";
	renderString(-0.95f, 0.90f, str.c_str(), str.length() , 's');
	str = "Ambient-coeff:";
	renderString(0.06f, 0.90f, str.c_str(), str.length() , 's');
	str = "Diffuse-coeff:";
	renderString(0.06f, 0.48f, str.c_str(), str.length() , 's');
	str = "Specular-coeff:";
	renderString(0.06f, 0.04f, str.c_str(), str.length() , 's');
	str = "Half-Toning";
	renderString(-0.9f, -0.52f, str.c_str(), str.length(), 'l');
	glColor3f(1,0,0.2);
	str = "Press 0: OFF";
	renderString(-0.9f, -0.65f, str.c_str(), str.length(), 'l');
	glColor3f(0,1,0);
	str = "Press 1: ON";
	renderString(-0.9f, -0.75f, str.c_str(), str.length(), 'l');
	glColor3f(1,1,1);
	str = "Phong-Lighting-Model:";
	renderString(-0.2f, -0.52f, str.c_str(), str.length(), 'l');
	glColor3f(1,0.8,0);
	str = "Press 2: OFF";
	renderString(0.2f, -0.65f, str.c_str(), str.length(), 'l');
	glColor3f(0,0.8,1);
	str = "Press 0: ON";
	renderString(0.2f, -0.75f, str.c_str(), str.length(), 'l');
	
	glColor3f(1,1,1);
	str = "Press q to Exit-Program";
	renderString(-0.2f, -0.89f, str.c_str(), str.length(), 'l');
}
void drawSliders() {
	int i = 0;
	while(i < UIslide.size()){
		UIslide.at(i).drawSlider();
		i = i + 1;
	}
}
// ==============================================================================================================//
void DrawPix(int x, int y, float r, float g, float b){
	if (x >= 0 && x < screen_width && y >= 0 && y < screen_height && turnOnOff == 0 || turnOnOff == 1 && switchs == 0) {
		PixelBuffer[screen_width * 3 * y + x * 3 + 0] = r;
		PixelBuffer[screen_width * 3 * y + x * 3 + 1] = g;
		PixelBuffer[screen_width * 3 * y + x * 3 + 2] = b;
	}
	else if (x >= 0 && x < screen_width && y >= 0 && y < screen_height && turnOnOff == 2 && switchs == 2) {
		float red = 0.7, green = 0.7, blue = 0.7;
		PixelBuffer[screen_width * 3 * y + x * 3 + 0] = red;
		PixelBuffer[screen_width * 3 * y + x * 3 + 1] = green;
		PixelBuffer[screen_width * 3 * y + x * 3 + 2] = blue;
	}
}
void DrawHalfTonePix(int x, int y, float r, float g, float b){
	if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
		halfToneColor[screen_width * 3 * y + x * 3 + 0] = r;
		halfToneColor[screen_width * 3 * y + x * 3 + 1] = g;
		halfToneColor[screen_width * 3 * y + x * 3 + 2] = b;
	}
}
void renderString(float x, float y, const char* format, size_t length, char c){
	glRasterPos3f(x, y, 0);
	int i = 0, j = 0;
	switch (c) {
		case 'l':
			while(i < length){
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, format[i]);
			i = i + 1;
			} break;
		case 's':
			while(j < length){
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, format[j]);
				j = j + 1;
			} break;
		default: break;
	}
}
// ==============================================================================================================//
void KeyBoardFunc(unsigned char key, int x, int y){
	if(key == '0'){
		turnOnOff = 0; switchs = 0;
		polyObjects();
		glutPostRedisplay();
	}
	else if(key == '1'){
		turnOnOff = 1;
		glutPostRedisplay();
	}
	else if (key == '2'){
		turnOnOff = 2; switchs = 2;
		polyObjects();
		glutPostRedisplay();	
	}
	else if(key == 'q' || key == 'Q'){ 
		exit(0);
	}
}
void MouseFunc(int button, int state, int x, int y) {
	int numY = screen_height - y, numX = x, i = 0;
	if(button == GLUT_LEFT_BUTTON){
		switch (state) {
			case GLUT_DOWN:
				while(i < UIslide.size()){
					if (numY >= UIslide.at(i).renderMinMaxCoor('2') && numY <= UIslide.at(i).renderMinMaxCoor('3')
						&& numX <= UIslide.at(i).renderMinMaxCoor('1') && numX >= UIslide.at(i).renderMinMaxCoor('0')){
							choose = i;
						}
					i = i + 1;
				} break;
			case GLUT_UP:
				if (choose != -1){
						choose = -1;
						polyObjects();
						glutPostRedisplay();
				} break;
			default: break;
		}
	}
}
int UI::renderMinMaxCoor(char c){
	int minX, maxX, minY, maxY;
	switch (c) {
	case '0':
		minX = round((position - 0.03f + 1.0f) * screen_width / 2 / 2.0f) + screen_width / 2;
		return minX; break;
	case '1':
		maxX = round((position + 0.03f + 1.0f) * screen_width / 2 / 2.0f) + screen_width / 2;
		return maxX; break;
	case '2':
		minY = round((y - 0.03f + 1.0f) * screen_width / 2 / 2.0f);
		return minY; break;
	case '3':
		maxY = round((y + 0.03f + 1.0f) * screen_width / 2 / 2.0f);
		return maxY; break;
	default: break;
	}
}
void MotionFunc(int x, int y) {
	int numX = x;
	if (choose != -1) {
		UIslide[choose].MoveSlider(numX);
		ka = glm::vec3(UIslide.at(0).changedValue(), UIslide.at(1).changedValue(), UIslide.at(2).changedValue());
		kd = glm::vec3(UIslide.at(3).changedValue(), UIslide.at(4).changedValue(), UIslide.at(5).changedValue());
		ks = glm::vec3(UIslide.at(6).changedValue(), UIslide.at(7).changedValue(), UIslide.at(8).changedValue());
		Ia = UIslide.at(9).changedValue();
		Il = UIslide.at(10).changedValue();
		n = UIslide.at(11).changedValue();
		light = glm::vec3(UIslide.at(12).changedValue(), UIslide.at(13).changedValue(), UIslide.at(14).changedValue());
		fromPoint = glm::vec3(UIslide.at(15).changedValue(), UIslide.at(16).changedValue(), UIslide.at(17).changedValue());
		K = UIslide.at(18).changedValue();
		glutPostRedisplay();
	}
}
// ==============================================================================================================//
float* halfToning(){
	halfToneColor = new float[screen_width * screen_height * 3];
	float pix[9][3];
	int row, col, x, y, activePix, pickPix;
	for (int i = 0; i < screen_width * screen_height / 9; i++) {
		int k = 0, h = 0, j = 0, index;
		row = i / (screen_height / 3) + (i / (screen_height / 3)) * 2;
		col = (i % (screen_width / 3)) * 3;
		while(j < 9) {
			x = col + j % 3;
			y = row + j / 3;
			pix[j][0] = PixelBuffer[screen_width * 3 * y + x * 3 + 0];
			pix[j][1] = PixelBuffer[screen_width * 3 * y + x * 3 + 1];
			pix[j][2] = PixelBuffer[screen_width * 3 * y + x * 3 + 2];
			j = j + 1;
		}
		float rgb, color = max(pix[0][0],max( pix[0][1], pix[0][2]));
		int minPix = 0, p = 1;
		while(p < 9){
			rgb = max(pix[p][0], max(pix[p][1], pix[p][2]));
			if (rgb > color){
				color = rgb;
				minPix = p;
			}
			p = p + 1;
		}
		index = minPix;
		activePix = (unsigned int)(max(max(pix[index][0], pix[index][1]), pix[index][2]) * 9);
		if(activePix == 0)continue;
		vector<int> choosePix;
		while(k < activePix){
			bool displayPix = true;
			while(displayPix){
				pickPix = rand() % 9;
				displayPix = false;
				for (unsigned int j = 0; j < choosePix.size(); j++) {
					if (choosePix[j] == pickPix) {
						displayPix = true; break;
					}
				}
			}
			choosePix.push_back(pickPix);
			k = k + 1;
		}
		float r = 0, g = 0, b = 0;
		srand(time(NULL) + i);
		while(h < choosePix.size()){
			x = col + choosePix.at(h) % 3;
			y = row + choosePix.at(h) / 3;
			DrawHalfTonePix(x, y, 1.0f, 1.0f, 1.0f);
			h = h + 1;
		}
	}
	return halfToneColor;
}
// ==============================================================================================================//
void sliderFunction(){
	// Ambient-coeff
	UI kaR(0.25f, 0.83f, 1.0f, 0.0f, 10.0f, "kaR");
	UI kaG(0.25f, 0.73f, 1.0f, 0.0f, 10.0f, "kaG");
	UI kaB(0.25f, 0.63f, 1.0f, 0.0f, 10.0f, "kaB");
	UIslide.push_back(kaR);
	UIslide.push_back(kaG);
	UIslide.push_back(kaB);
	// Diffuse-coeff
	UI kdR(0.25f, 0.38f, 1.0f, 0.0f, 10.0f, "kdR");
	UI kdG(0.25f, 0.28f, 1.0f, 0.0f, 10.0f, "kdG");
	UI kdB(0.25f, 0.18f, 1.0f, 0.0f, 10.0f, "kdB");
	UIslide.push_back(kdR);
	UIslide.push_back(kdG);
	UIslide.push_back(kdB);
	// Specular-coeff
	UI ksR(0.25f, -0.04f, 1.0f, 0.0f, 10.0f, "ksR");
	UI ksG(0.25f, -0.14f, 1.0f, 0.0f, 10.0f, "ksG");
	UI ksB(0.25f, -0.24f, 1.0f, 0.0f, 10.0f, "ksB");
	UIslide.push_back(ksR);
	UIslide.push_back(ksG);
	UIslide.push_back(ksB);
	// Intensity
	UI Ia(-0.75f, 0.82f, 1.0f, 0.0f, 10.0f, "Ia");
	UI Il(-0.75f, 0.72f, 1.0f, 0.0f, 10.0f, "Il");
	UI n(-0.75f, 0.62f, 10.0f, 0.0f, 10.0f, "n");
	UI K(-0.75f, 0.52f, 2.0f, 0.0f, 10.0f, "K");
	UIslide.push_back(Ia);
	UIslide.push_back(Il);
	UIslide.push_back(n);

	// X-Y-Z light position
	UI lightX(-0.75f, 0.38f, 6.0f, -6.0f, 10.0f, "lX");
	UI lightY(-0.75f, 0.28f, 6.0f, -6.0f, 10.0f, "lY");
	UI lightZ(-0.75f, 0.18f, 6.0f, -6.0f, 10.0f, "lZ");
	UIslide.push_back(lightX);
	UIslide.push_back(lightY);
	UIslide.push_back(lightZ);

	// X-Y-Z view position
	UI fromX(-0.75f, 0.00f, 6.0f, -6.0f, 10.0f, "fX");
	UI fromY(-0.75f, -0.1f, 6.0f, -6.0f, 10.0f, "fY");
	UI fromZ(-0.75f, -0.2f, 6.0f, -6.0f, 10.0f, "fZ");
	UIslide.push_back(fromX);
	UIslide.push_back(fromY);
	UIslide.push_back(fromZ);

	UIslide.push_back(K);
}
//=======================================================================//
UI::UI(float x, float y, float max, float min, float length, string name): x(x), y(y), max(max), min(min), length(length) {
	this->position = x + (length / 40.0f); this->str = name;
}
void UI::drawSlider() {
	glViewport(screen_width / 2, 0, screen_width / 2, screen_width / 2);
	glColor3f(1, 1, 0);
	renderString(x - 0.15f, y - 0.02f, str.c_str(), str.length(), 's');
	glColor3f(1, 1, 1);
	glLineWidth(2.0);
	glBegin(GL_LINES); 
	glVertex2f(x, y);
	glVertex2f(x + length / 20.0f, y);
	glEnd();
	glViewport(screen_width / 2, 0, screen_width / 2, screen_width / 2);
	glColor3f(0.4, 1, 0.3);
	glBegin(GL_QUADS); // cube slider button
	glVertex2f(position - 0.032f, y - 0.032f);
	glVertex2f(position + 0.032f, y - 0.032f);
	glColor3f(0.3, 0.5, 1);
	glVertex2f(position + 0.032f, y + 0.032f);
	glVertex2f(position - 0.032f, y + 0.032f);
	glEnd();
	glColor3f(1, 1, 1);
	string value = to_string(changedValue()).substr(0, 4);
	renderString(x + length / 20.0f + 0.05f, y - 0.02f, value.c_str(), value.length(), 's');
}
void UI::MoveSlider(int numX) {
	int xposition;
	if (numX < round((x + 1.0f) * screen_width / 2 / 2.0f) + screen_width / 2)
		xposition = round((x + 1.0f) * screen_width / 2 / 2.0f) + screen_width / 2;
	else if (numX > round((x + length / 20.0f + 1.0f) * screen_width / 2 / 2.0f) + screen_width / 2)
		xposition = round((x + length / 20.0f + 1.0f) * screen_width / 2 / 2.0f) + screen_width / 2;
	else
		xposition = numX;
	position = ((xposition - screen_width / 2)  * 1.0f) / (screen_width / 2 * 1.0f) * 2 - 1.0f;
}
float UI::changedValue() {
	float value = (round(((max - min) / (length / 20.0f) * (position - x) + min) * 100) / 100.0f);
	if (abs(value) < 0.03)
		value = abs(value);
	return value;
}
// ==============================================================================================================//
void polyObjects() {
	for (int i = 0; i < polygon3D.size(); i++) {
		polygon3D.at(i)->PhongLightingModel(fromPoint, light, ka, kd, ks, n, Ia, Il, K);
		polygon3D.at(i)->changePixelAWorld('p');
	}
	DepthSort('0');
	for (int i = 0; i < objectFaces.size(); i++) {
		if (dot(objectFaces.at(i)->n, glm::vec3(0, 0, 1)) > 0) {
			objectFaces.at(i)->renderGouraudEdge('x');
			objectFaces.at(i)->renderGouraudShading('x');
		}
	}
	DepthSort('1');
	for (int i = 0; i < objectFaces.size(); i++) {
		if (dot(objectFaces.at(i)->n, glm::vec3(0, -1, 0)) > 0) {
			objectFaces.at(i)->renderGouraudEdge('y');
			objectFaces.at(i)->renderGouraudShading('y');
		}
	}
	DepthSort('2');
	for (int i = 0; i < objectFaces.size(); i++) {
		if (dot(objectFaces.at(i)->n, glm::vec3(1, 0, 0)) > 0) {
			objectFaces.at(i)->renderGouraudEdge('z');
			objectFaces.at(i)->renderGouraudShading('z');
		}
	}
	int j = 0;
	while(j < polygon3D.size()){
		polygon3D.at(j)->changePixelAWorld('w');
		j = j + 1;
	}
	halfToneColor = halfToning();
	glutPostRedisplay();
}
// ==============================================================================================================//
void DepthSort(char c){
	float xmin, ymax, zmin, size = objectFaces.size();
	int index, i = 0;
	gouraudShading* objFaces;
	switch (c) {
		case '0':
			while(i < size - 1){
				zmin = objectFaces.at(i)->renderMinMax('2');
				index = i;
				for (int j = i + 1; j < size; j++) {
					if (objectFaces.at(j)->renderMinMax('2') <= zmin) {
						zmin = objectFaces.at(j)->renderMinMax('2');
						index = j;
					}
				}
				objFaces = objectFaces.at(index);
				objectFaces.erase(objectFaces.begin() + index);
				objectFaces.insert(objectFaces.begin() + i, objFaces);
				i = i + 1;
			} break;
		case '1':
			while(i < size - 1){
				ymax = objectFaces.at(i)->renderMinMax('1');
				index = i;
				for (int j = i + 1; j < size; j++) {
					if (objectFaces.at(j)->renderMinMax('1') >= ymax) {
						ymax = objectFaces.at(j)->renderMinMax('1');
						index = j;
					}
				}
				objFaces = objectFaces.at(index);
				objectFaces.erase(objectFaces.begin() + index);
				objectFaces.insert(objectFaces.begin() + i, objFaces);
				i = i + 1;
			} break;
		case '2':
			while(i < size - 1){
				xmin = objectFaces.at(i)->renderMinMax('0');
				index = i;
				for (int j = i + 1; j < size; j++) {
					if (objectFaces.at(j)->renderMinMax('0') <= xmin) {
						xmin = objectFaces.at(j)->renderMinMax('0');
						index = j;
					}
				}
				objFaces = objectFaces.at(index);
				objectFaces.erase(objectFaces.begin() + index);
				objectFaces.insert(objectFaces.begin() + i, objFaces);
				i = i + 1;
			} break;
		default: break;
	}
}
//=====================================================================//
drawLine::drawLine(PhongModel* start, PhongModel* end){
	this->start = start, this->end = end;
}
float drawLine::DDALineIntersect(int axisValue, float xStart, float xEnd, float yStart, float yEnd){
	if(axisValue > round(max(yStart, yEnd)) || axisValue < round(min(yStart, yEnd)))
		return -1;
	float dx = xEnd - xStart, dy = yEnd - yStart, steps;
	float xIncrement, yIncrement, x = xStart, y = yStart;

	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xIncrement = dx / steps;
	yIncrement = dy / steps;
	while (round(y) != axisValue){
		x += xIncrement;
		y += yIncrement;
	}
	return round(x);
}
int drawLine::renderIntercept(int value, char c){
	switch (c) {
		case 'x':
			return DDALineIntersect(value, start->x, end->x, start->y, end->y); break;
		case 'y':
			return DDALineIntersect(value, start->x, end->x, start->z, end->z); break;
		case 'z':
			return DDALineIntersect(value, start->y, end->y, start->z, end->z); break;
		default: break;
	}
}
void drawLine::renderGouraudLine(char c){
	glm::vec3 intercept_rgb = glm::vec3(0);
	float dx, dy, dz, steps, xIncre, zIncre, yIncre, x, y, z;
	switch (c){
		case 'x':
			dx = end->x - start->x, dy = end->y - start->y, steps;
			x = start->x, y = start->y;
			if (fabs(dx) > fabs(dy))
				steps = fabs(dx);
			else
				steps = fabs(dy);
			xIncre = dx / steps;
			yIncre = dy / steps;
			DrawPix(round(x), round(y) + screen_width / 2, start->PhongVec[0], start->PhongVec[1], start->PhongVec[2]);
			for (int i = 0; i < round(steps) - 1; i++) {
				x = x + xIncre;
				y = y + yIncre;
				if (fabs(dx) > fabs(dy))
					intercept_rgb = (end->x - x) / dx * start->PhongVec + (x - start->x) / dx * end->PhongVec;
				else
					intercept_rgb = (end->y - y) / dy * start->PhongVec + (y - start->y) / dy * end->PhongVec;
				DrawPix(round(x), round(y) + screen_width / 2, intercept_rgb[0], intercept_rgb[1], intercept_rgb[2]);
			}	break;
		case 'y':
			dx = end->x - start->x, dz = end->z - start->z, steps;
			x = start->x, z = start->z;
			if (fabs(dx) > fabs(dz))
				steps = fabs(dx);
			else
				steps = fabs(dz);
			xIncre = dx / steps;
			zIncre = dz / steps;
			DrawPix(round(x) + screen_width / 2, round(z) + screen_width / 2, start->PhongVec[0], start->PhongVec[1], start->PhongVec[2]);
			for (int i = 0; i < round(steps) - 1; i++) {
				x = x + xIncre;
				z = z + zIncre;
				if (fabs(dx) > fabs(dz))
					intercept_rgb = (end->x - x) / dx * start->PhongVec + (x - start->x) / dx * end->PhongVec;
				else
					intercept_rgb = (end->z - z) / dz * start->PhongVec + (z - start->z) / dz * end->PhongVec;
				DrawPix(round(x) + screen_width / 2, round(z) + screen_width / 2, intercept_rgb[0], intercept_rgb[1], intercept_rgb[2]);
			} break;
		case 'z':
			dy = end->y - start->y, dz = end->z - start->z, steps;
			y = start->y, z = start->z;
			if (fabs(dy) > fabs(dz))
				steps = fabs(dy);
			else
				steps = fabs(dz);
			yIncre = dy / steps;
			zIncre = dz / steps;
			DrawPix(round(y), round(z), start->PhongVec[0], start->PhongVec[1], start->PhongVec[2]);
			for (int i = 0; i < round(steps) - 1; i++){
				y = y + yIncre;
				z = z + zIncre;
				if (fabs(dy) > fabs(dz))
					intercept_rgb = (end->y - y) / dy * start->PhongVec + (y- start->y) / dy * end->PhongVec;
				else
					intercept_rgb = (end->z - z) / dz * start->PhongVec + (z - start->z) / dz * end->PhongVec;
				DrawPix(round(y), round(z), intercept_rgb[0], intercept_rgb[1], intercept_rgb[2]);
			} break;
	default: break;
	}
}
glm::vec3 drawLine::interceptRGB(int x, int y, float xStart, float xEnd, float yStart, float yEnd){
	glm::vec3 intercept_rgb = glm::vec3(0);
	float dx = xEnd - xStart, dy = yEnd - yStart;
	if (fabs(dx) > fabs(dy))
		intercept_rgb = (xEnd - x * 1.0f) / dx * start->PhongVec + (x * 1.0f - xStart) / dx * end->PhongVec;
	else
		intercept_rgb = (yEnd - y * 1.0f) / dy * start->PhongVec + (y * 1.0f - yStart) / dy * end->PhongVec;
	return intercept_rgb;
}
glm::vec3 drawLine::rgbCrossPoints(int x, int y, char c){
	switch (c) {
		case 'x':
			return interceptRGB(x,y, start->x, end->x, start->y, end->y);	break;
		case 'y':
			return interceptRGB(x,y, start->x, end->x, start->z, end->z); break;
		case 'z':
			return interceptRGB(x,y, start->y, end->y, start->z, end->z); break;
		default: break;
	}
}
// =================================================================================================//
void object3D::normalize() {
	for (unsigned int i = 0; i < vertices.size(); i++){
		vertices.at(i)->normalization();
	}
}
void object3D::PhongLightingModel(glm::vec3 fromPoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
																	float n, float Ia, float Il, float K){
	for (unsigned int i = 0; i < vertices.size(); i++){
		vertices.at(i)->PhongLightingModel(fromPoint,light, ka, kd, ks, n, Ia, Il, K);
	}
	normalizeRGB();
}
void object3D::normalizeRGB(){
	//find the rgb max value in all vertices
	float rgbmax = max(vertices.at(0)->PhongVec[0], vertices.at(0)->PhongVec[1]);
	rgbmax = max(rgbmax, vertices.at(0)->PhongVec[2]);
	float rgb;
	for (unsigned int i = 1; i < vertices.size(); i++)
	{
		rgb = max(vertices.at(i)->PhongVec[0], vertices.at(i)->PhongVec[1]);
		rgb = max(rgb, vertices.at(i)->PhongVec[2]);
		if (rgb > rgbmax)
			rgbmax = rgb;
	}
	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices.at(i)->normalizeRGB(rgbmax);
}
void object3D::changePixelAWorld(char c){
	switch (c){
		case 'p':
			for (unsigned int i = 0; i < vertices.size(); i++)
			vertices.at(i)->convertPixelWorld('p'); break;
		case 'w':
			for (unsigned int i = 0; i < vertices.size(); i++)
			vertices.at(i)->convertPixelWorld('w'); break;
	default: break;
	}
}
//==============================================================================================================//
PhongModel::PhongModel(float x, float y, float z): x(x), y(y), z(z) {}
void PhongModel::facesNormalize(glm::vec3 normal) {
	normals.push_back(normal);
}
glm::vec3 PhongModel::toXYZvector(){
	return glm::vec3(x, y, z);
}
void PhongModel::normalization() {
	glm::vec3 n = glm::vec3(0);
	int i = 0;
	while(i < normals.size()){
		n = n + normals.at(i);
		i = i + 1;
	}
	n /= (1.0f * normals.size());
	normal = normalize(n);
}

void PhongModel::PhongLightingModel(glm::vec3 fromPoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
	float n, float Ia, float Il, float K) {
	if(Il == 0){Il = 0.01;}
	glm::vec3 l, r, v;
	l = normalize(light - toXYZvector());	
	r = (l * -1.0f) + (2.0f * dot(normal, l) * normal); 
	v = normalize(fromPoint - toXYZvector()); 
	float normalLight = dot(normal, l), reflectionView =  dot(r, v);
	normalLight = max(0.0f, normalLight);
	reflectionView = max(0.0f, reflectionView);
	PhongVec = ka * Ia + ((kd * normalLight + ks * pow(reflectionView, n)) * (Il / (length(fromPoint - toXYZvector()) + K)));
}
void PhongModel::normalizeRGB(float color){
	PhongVec = glm::vec3(PhongVec[0] / color, PhongVec[1] / color, PhongVec[2] / color);
}
void PhongModel::convertPixelWorld(char c){
	switch (c){
		case 'p':
			x = x * 350.0f, y = y * 350.0f, z = z * 350.0f; break;
		case 'w':
			x = x / 350.0f, y = y / 350.0f, z = z / 350.0f; break;
		default: break;
	}
}
// ========================================================================//
gouraudShading::gouraudShading(PhongModel* P1, PhongModel* P2, PhongModel* P3) {
	this->point1 = P1; this->point2 = P2; this->point3 = P3;
	drawLine *edgeA= new drawLine(point1, point2), *edgeB = new drawLine(point1, point3), *edgeC = new drawLine(point2, point3); 
	edges.push_back(edgeA); edges.push_back(edgeB); edges.push_back(edgeC);
	n = cross(point2->toXYZvector() - point1->toXYZvector(), point3->toXYZvector() - point1->toXYZvector());
}
float gouraudShading::renderMinMax(char c){
	float minX, maxY, minZ;
	switch (c) {
		case '0':
			minX = min(point1->x, point2->x);
			minX = min(minX, point3->x);
			return minX; break;
		case '1':
			maxY = max(point1->y, point2->y);
			maxY = max(maxY, point3->y);
			return maxY; break;
		case '2':
			minZ = min(point1->z, point2->z);
			minZ = min(minZ, point3->z);
			return minZ; break;
	default: break;
	}
}
void gouraudShading::facesNormalization(){
	normal = cross(point2->toXYZvector() - point1->toXYZvector(), point3->toXYZvector() - point1->toXYZvector());
	point1->facesNormalize(normal);
	normal = cross(point3->toXYZvector() - point2->toXYZvector(), point1->toXYZvector() - point2->toXYZvector());
	point2->facesNormalize(normal);
	normal = cross(point1->toXYZvector() - point3->toXYZvector(), point2->toXYZvector() - point3->toXYZvector());
	point3->facesNormalize(normal);
}
void gouraudShading::renderGouraudShading(char c){
		int ymax, ymin, zmax, zmin, x;
		vector<int> interceptions;
		vector<glm::vec3> rgb;
		switch (c) {
			case 'x':
				ymax = round(max(point1->y, max(point2->y, point3->y)));
				ymin = round(min(point1->y, min(point2->y, point3->y)));
				for (int i = ymin; i <= ymax; i++) {
					for (unsigned int j = 0; j < edges.size(); j++) {
						x = edges.at(j)->renderIntercept(i, 'x');
						if (x != -1) {
							bool found = false;
							for (unsigned int i = 0; i < interceptions.size(); i++) {
								if (interceptions.at(i) == x) {
									found = true;
									break;
								}
							}
							if (!found) {
								unsigned int k;
								for (k = 0; k < interceptions.size(); k++) {
									if (x < interceptions.at(k))
										break;
								}
								interceptions.insert(interceptions.begin() + k, x);
								rgb.insert(rgb.begin() + k, edges.at(j)->rgbCrossPoints(x, i, 'x'));
							}
						}
					}
					if (interceptions.size() == 1){
						DrawPix(interceptions.at(0), i + screen_height / 2, rgb[0][0], rgb[0][1], rgb[0][2]);
					}
					else if (interceptions.size() == 2) {
						glm::vec3 rgb_between = glm::vec3(0);
						int dx;
						dx = interceptions.at(1) - interceptions.at(0);
						for (int k = interceptions.at(0); k <= interceptions.at(1); k++) {
							rgb_between = (interceptions.at(1) - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions.at(0)) * 1.0f / (dx * 1.0f) * rgb[1];
							DrawPix(k , i + screen_height / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
						}
					}
					else {
						glm::vec3 rgb_between = glm::vec3(0);
						int dx;
						dx = interceptions.at(2) - interceptions.at(0);
						for (int k = interceptions.at(0); k <= interceptions.at(2); k++) {
							rgb_between = (interceptions.at(2) - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions.at(0)) * 1.0f / (dx * 1.0f) * rgb[2];
							DrawPix(k, i + screen_height / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
						}
					}
					interceptions.clear();
					rgb.clear();	
				} break;
			case 'y':
				zmax = round(max(max(point1->z, point2->z), point3->z));
				zmin = round(min(min(point1->z, point2->z), point3->z));
				for (int i = zmin; i <= zmax; i++) {
					int y;
					for (unsigned int j = 0; j < edges.size(); j++) {
						y = edges.at(j)->renderIntercept(i, 'y');
						if (y != -1) {
							bool found = false;
							for (unsigned int i = 0; i < interceptions.size(); i++) {
								if (interceptions.at(i) == y) {
									found = true;
									break;
								}
							}
							if (!found) {
								unsigned int k;
								for (k = 0; k < interceptions.size(); k++) {
									if (y < interceptions.at(k))
										break;
								}
								interceptions.insert(interceptions.begin() + k, y);
								rgb.insert(rgb.begin() + k, edges.at(j)->rgbCrossPoints(y, i, 'y'));
							}
						}
					}
					if (interceptions.size() == 1)
						DrawPix(interceptions.at(0) + screen_width / 2, i + screen_height / 2, rgb[0][0], rgb[0][1], rgb[0][2]);
					else if (interceptions.size() == 2) {
						glm::vec3 rgb_between = glm::vec3(0);
						int dx;
						dx = interceptions.at(1) - interceptions.at(0);
						for (int k = interceptions.at(0); k <= interceptions.at(1); k++) {
							rgb_between = (interceptions.at(1) - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions.at(0)) * 1.0f / (dx * 1.0f) * rgb[1];
							DrawPix(k + screen_width / 2, i + screen_height / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
						}
					}
					else {
						glm::vec3 rgb_between = glm::vec3(0);
						int dx;
						dx = interceptions.at(2) - interceptions.at(0);
						for (int k = interceptions.at(0); k <= interceptions.at(2); k++) {
							rgb_between = (interceptions.at(2) - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions.at(0)) * 1.0f / (dx * 1.0f) * rgb[2];
							DrawPix(k + screen_width / 2, i + screen_height / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
						}
					}
					interceptions.clear();
					rgb.clear();
				} break;
			case 'z':
				zmax = round(max(max(point1->z, point2->z), point3->z));
				zmin = round(min(min(point1->z, point2->z), point3->z));
				for (int i = zmin; i <= zmax; i++) {
					int y;
					for (unsigned int j = 0; j < edges.size(); j++){
						y = edges.at(j)->renderIntercept(i, 'z');
						if (y != -1) {
							bool found = false;
							for (unsigned int i = 0; i < interceptions.size(); i++) {
								if (interceptions.at(i) == y) {
									found = true;
									break;
								}
							}
							if (!found) {
								unsigned int k;
								for (k = 0; k < interceptions.size(); k++) {
									if (y < interceptions.at(k))
										break;
								}
								interceptions.insert(interceptions.begin() + k, y);
								rgb.insert(rgb.begin() + k, edges.at(j)->rgbCrossPoints(y, i, 'z'));
							}

						}
					}
					if (interceptions.size() == 1){
						DrawPix(interceptions.at(0), i, rgb[0][0], rgb[0][1], rgb[0][2]);
					}	
					else if (interceptions.size() == 2) {
						glm::vec3 rgb_between = glm::vec3(0);
						int dy;
						dy = interceptions.at(1) - interceptions.at(0);
						for (int k = interceptions.at(0); k <= interceptions.at(1); k++) {
							rgb_between = (interceptions.at(1) - k) * 1.0f / (dy * 1.0f) * rgb[0] + (k - interceptions.at(0)) * 1.0f / (dy * 1.0f) * rgb[1];
							DrawPix(k, i, rgb_between[0], rgb_between[1], rgb_between[2]);
						}
					}
					else {
						glm::vec3 rgb_between = glm::vec3(0);
						int dy;
						dy = interceptions.at(2) - interceptions.at(0);
						for (int k = interceptions.at(0); k <= interceptions.at(2); k++) {
							rgb_between = (interceptions.at(2) - k) * 1.0f / (dy * 1.0f) * rgb[0] + (k - interceptions.at(0)) * 1.0f / (dy * 1.0f) * rgb[2];
							DrawPix(k, i, rgb_between[0], rgb_between[1], rgb_between[2]);
						}
					}
					interceptions.clear();
					rgb.clear();
				} break;
			default: break;
		}
}
void gouraudShading::renderGouraudEdge(char c){
	switch (c) {
		case 'x':
			for (unsigned int i = 0; i < edges.size(); i++)
				edges.at(i)->renderGouraudLine('x'); break;
		case 'y':
			for (unsigned int i = 0; i < edges.size(); i++)
				edges.at(i)->renderGouraudLine('y'); break;
		case 'z':
			for (unsigned int i = 0; i < edges.size(); i++)
				edges.at(i)->renderGouraudLine('z'); break;
		default: break;
	}
}
//==========================================================================================//