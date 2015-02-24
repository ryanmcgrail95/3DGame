// InputController.cpp

#include <iostream>
#include "InputController.h"
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "../Functions/Math2D.h"
using namespace std;

InputController* inp;
void mouseCallback(int button, int state, int x, int y) {
	inp->updateMouse(button, state, x, y);
}
void passiveMouseCallback(int x, int y) {
	inp->updatePassiveMouse(x, y);
}
void keyboardCallback(unsigned char key, int x, int y) {
	inp->updateKeyboard(key, x, y);
}
void keyboardUpCallback(unsigned char key, int x, int y) {
	inp->updateKeyboardUp(key, x, y);
}


InputController :: InputController() {
	inp = this;

	wasdArray = new bool[4];
	wasdArray[0] = false;
	wasdArray[1] = false;
	wasdArray[2] = false;
	wasdArray[3] = false;

	glutMouseFunc(mouseCallback);
	glutPassiveMotionFunc(passiveMouseCallback);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
}

int InputController :: getMouseX() {
	return mouseX;
}

int InputController :: getMouseY() {
	return mouseY;
}

		
void InputController :: updateMouse(int button, int state, int x, int y) {
	//bool* leftMouse;
	//bool* middleMouse;
	//bool* rightMouse;
}
void InputController :: updatePassiveMouse(int x, int y) {
	mouseX = x;
	mouseY = y;
}
void InputController :: updateKeyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'w':
		case 'a':
		case 's':
		case 'd':	setWASD(key, true);
			break;
	}
}
void InputController :: updateKeyboardUp(unsigned char key, int x, int y) {
	switch(key) {
		case 'w':
		case 'a':
		case 's':
		case 'd':	setWASD(key, false);
			break;
	}
}

float InputController :: getWASDDir() {
	int hDir, vDir;

	if(getWASD('a') && !getWASD('d'))
		hDir = -1;
	else if(!getWASD('a') && getWASD('d'))
		hDir = 1;
	else
		hDir = 0;

	if(getWASD('w') && !getWASD('s'))
		vDir = 1;
	else if(!getWASD('w') && getWASD('s'))
		vDir = -1;
	else
		vDir = 0;

	if(hDir == 0 && vDir == 0)
		return -1;
	else
		return calcPtDir(0,0,hDir,vDir);
}

bool InputController :: getWASD(char key) {
	switch(key) {
		case 'w':	return wasdArray[0];
			break;
		case 'a':	return wasdArray[1];
			break;
		case 's':	return wasdArray[2];
			break;
		case 'd':	return wasdArray[3];
			break;
	}

	return false;
}

void InputController :: setWASD(char key, bool down) {
	switch(key) {
		case 'w':	wasdArray[0] = down;
			break;
		case 'a':	wasdArray[1] = down;
			break;
		case 's':	wasdArray[2] = down;
			break;
		case 'd':	wasdArray[3] = down;
			break;
	}
}

