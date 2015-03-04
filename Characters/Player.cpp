// Player.cpp


#include <deque>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <string>
#include <iostream>
#include "../Graphics/Texture.h"
#include "../Graphics/GraphicsOGL.h"
#include "../Primitives/Physical.h"
#include "Player.h"
#include <cmath>
#include "../Functions/Math2D.h"
#include "../Environment/Heightmap.h"
using namespace std;

int spin = 0;

float nCDir = 0, s = 6, h = 8;


Texture* Player :: shTex;

Player :: Player(float x, float y, float z) : Physical(x,y,z) {

	hopZ = 
	hopZVel = 0;
	hopSc = 1;

 	shTex = new Texture("Resources/Images/shadow.png",false);
}

void Player :: update(GraphicsOGL* gl, float deltaTime) {

	// Update Physics/Collisions
	Physical :: update(gl, deltaTime);

	// Update User Control of Player
	updateControl(gl,deltaTime);



	/*glEnable(GL_LIGHT0);
	GLfloat lightpos[] = {x, y, z, 1};
	//GLfloat lightpos[] = {0,0,-1, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);*/



	float hopZVelP = hopZVel;

	hopZVel -= .2;
	hopZ += hopZVel;
	if(hopZ < 0) {
		hopZ = hopZVel = 0;

		if(hopZVelP != 0)
			hopSc *= 1.3;
	}

	hopSc += (1 - hopSc)/3;


	float len = 50, dir = nCDir;


	// Place Camera on Heightmap
		Heightmap* h = gl->getHeightmap();
		float cX, cY;
		cX = x-calcLenX(len,nCDir);
		cY = y-calcLenY(len,nCDir);	

		gl->setProjectionPrep(cX,cY,h->getHeightXY(cX,cY)+8+10,x,y,z+8);
}

void Player :: draw(GraphicsOGL* gl, float deltaTime) {

	//Physical :: draw(gl, deltaTime);

	Heightmap* hm = gl->getHeightmap();

	float sideNum = 6;
	float ang = 45, dir, xN, yN;

	float normal[3];
	float xyDis, nX, nY, nZ, xRot, yRot, setupRot, xyRot;

		hm->getNormal(x,y,normal);
		
			nX = normal[0];
			nY = normal[1];
			nZ = normal[2];


		xyDis = sqrt(nX*nX + nY*nY);
		xRot = 90 - (180-(90+calcPtDir(0,0,nX,nZ)));
		yRot = calcPtDir(0,0,nY,nZ);

		setupRot = 90+calcPtDir(0,0,nX,nY);
		xyRot = 90-calcPtDir(0,0,xyDis,nZ);



	// Draw Several Shadows
	/*for(int i = 0; i < 4; i++) {
		float shNorm[3];
		float l = s, d = i*90., shadowX = x + calcLenX(l,d), shadowY = y + calcLenY(l,d), shadowZ = hm->getHeightXY(shadowX,shadowY);
		float shadowSetupRot, shadowXYRot;

			hm->getNormal(shadowX, shadowY, shNorm);

		shadowSetupRot = 90+calcPtDir(0,0,shNorm[0],shNorm[1]);
		shadowXYRot = 90-calcPtDir(0,0,calcPtDis(0,0,shNorm[0],shNorm[1]),shNorm[2]);

		//DRAW SHADOW
			glTranslatef(x,y,z+.3);
			glRotated(shadowSetupRot, 0, 0, 1);	
			glRotated(shadowXYRot, 1, 0, 0);
	
			gl->draw3DFloor(-8,-8,8,8,0,shTex);

			glLoadIdentity();
	}*/


	// Draw Sky
	gl->enableSkyShader();
		glTranslatef(x,y,z);

		float sc = 400;

		gl->draw3DFloor(-sc,-sc,sc,sc,sc,NULL);
		gl->draw3DWall(-sc,-sc,sc,sc,-sc,-sc,NULL);
		gl->draw3DWall(-sc,sc,sc,sc,sc,-sc,NULL);
		gl->draw3DWall(-sc,-sc,sc,-sc,sc,-sc,NULL);
		gl->draw3DWall(sc,-sc,sc,sc,sc,-sc,NULL);
		glLoadIdentity();
	gl->disableShaders();

	// Draw Water
	gl->enableWaterShader();
		gl->draw3DFloor(0,0,2048,2048,200,NULL);	
	gl->disableShaders();




	glTranslatef(x,y,z+hopZ);

	//glRotated(xRot, 1, 0, 0);

	if(hopZ <= 0) {
		glRotated(setupRot, 0, 0, 1);	
		glRotated(xyRot, 1, 0, 0);
		glRotated(direction-setupRot, 0, 0, 1);	
	}
	else
		glRotated(direction, 0, 0, 1);	
	//glRotated(direction, 0, 0, 1);
	
	glScalef(hopSc,hopSc,1/hopSc);

	// Draw Top of Player Model
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(.5, .5);
				glVertex3d(0,0,h);

			for(int i = 0; i <= sideNum; i++) {
				dir = ang + i/sideNum*360;

				xN = calcLenX(1,dir);
				yN = calcLenY(1,dir);

				glTexCoord2d(.5 + .5*xN,.5 + .5*yN);
					glVertex3d(xN*s, yN*s, h);
			}
		glEnd();

	// Draw Side Faces of Player Model
	for(int i = 0; i < sideNum; i++) {
		glBegin(GL_QUADS);

			dir = ang + i/sideNum*360;
				xN = calcLenX(1,dir);
				yN = calcLenY(1,dir);

			glTexCoord2d(0,0);
				glVertex3d(xN*s, yN*s, h);
			glTexCoord2d(0,1);
				glVertex3d(xN*s, yN*s, 0);


			dir = ang + (i+1)/sideNum*360;
				xN = calcLenX(1,dir);
				yN = calcLenY(1,dir);

			glTexCoord2d(1,1);
				glVertex3d(xN*s, yN*s, 0);
			glTexCoord2d(1,0);
				glVertex3d(xN*s, yN*s, h);
		glEnd();
	}

	gl->disableShaders();
	
	glLoadIdentity();
}

void Player :: updateControl(GraphicsOGL* gl, float deltaTime) {
	float dir = gl->getWASDDir(), cDir = gl->getCamDir(), aDir;
	aDir = dir-90;

	if(dir != -1) {
		vel = 1 + gl->getShift()*2;

		if(dir == 90 || dir == -90) {
			nCDir = cDir;
			direction = cDir - 90 + dir;	
		}
		else if(dir == 0 || dir == 180) {
			direction = cDir - 90 + dir;	
			nCDir = cDir + .2*calcTurnToDir(cDir,cDir-90+dir);
		}
		else if(dir == 45 || dir == 135) {
			direction = cDir - 90 + dir;	
			nCDir = cDir + .2*calcTurnToDir(cDir,cDir-90+dir);
		}
		else {
			direction = cDir - 90 + dir;	
			nCDir = cDir - .2*calcTurnToDir(cDir,180+cDir-90+dir);
		}

		
		if(hopZ == 0)
			hopZVel = 1.3;
	}
	else
		vel = 0;
}
