// GraphicsOGL.cpp


#define GL_GLEXT_PROTOTYPES

#include <unistd.h>
#include <chrono>
#include <cctype>
#include <iostream>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "Texture.h"
#include "Image.h"
#include "Camera.h"
#include "GraphicsOGL.h"
#include "Shader.h"
#include "Font.h"
#include "../IO/InputController.h"
#include "ShaderController.h"
#include "TextureController.h"
#include "../Primitives/Drawable.h"
#include "../Primitives/Updateable.h"
#include "FontController.h"
#include <ctime>
#include "../Functions/Math2D.h"
#include "../Functions/linmath.h"
#include "../Environment/Heightmap.h"
#include "../Characters/Player.h"
#include "../Characters/NPC.h"
#include "../Environment/Tree.h"
#include "../Environment/Terrain.h"
#include "../inventory/inventory.h"

using namespace std;
using namespace std::chrono;

	
NPC* npc;

GraphicsOGL* ogl;
void idleCallback() {

	ogl->idle();
}
void displayCallback() {

	ogl->display();
}


GraphicsOGL :: GraphicsOGL(int argc, char* argv[]) {
	ogl = this;

	// Establish Screen Resolution Variables
	resolution = new float[2];
	SCREEN_WIDTH = resolution[0] = 640;
	SCREEN_HEIGHT = resolution[1] = 480;

	avgFPS = 60;
	fps = 60;
	curProgram = 0;
	globalTime = 0;


	// Intialize 3D Variables
	initialize3D(argc, argv);
}

GraphicsOGL :: ~GraphicsOGL() {
	delete [] resolution;
}

Player* GraphicsOGL :: getPlayer() {
	return myPlayer;
}

Inventory* GraphicsOGL :: getInventory() {
	return myInventory;
}

void GraphicsOGL :: initialize3D(int argc, char* argv[]) {

	//Initialize OpenGL
	glutInit(&argc, argv);

	//Initialize OpenGL Window
	glutInitDisplayMode(GLUT_DEPTH|GLUT_SINGLE|GLUT_RGBA); 
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Game"); 	

	//Set Up View Port for Window
	glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0, -3,1000);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);

		//Load Resources, Create GraphicsOGL's Objects
		glCamera = new Camera();


		inputController = new InputController();
		fontController = new FontController();
		textureController = new TextureController();
		shaderController = new ShaderController();
		myInventory = new Inventory();

		terrain = new Terrain(textureController,2048,2048, "Resources/Images/test.png",150);
		myPlayer = new Player(1028,1028,0);
		npc = new NPC(1018,1018,0);

	//Set Up OpenGL Callbacks (Updating Functions...)
	glutIdleFunc(idleCallback);
	glutDisplayFunc(displayCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Start up Main Loop
	glutMainLoop();
}

//Get Screen Width
int GraphicsOGL :: getScreenWidth() {
	return SCREEN_WIDTH;
}	

//Get Screen Height
int GraphicsOGL :: getScreenHeight() {
	return SCREEN_HEIGHT;
}

InputController* GraphicsOGL :: getInputController() {
	return inputController;
}
Heightmap* GraphicsOGL :: getHeightmap() {
	return terrain->getHeightmap();
}
TextureController* GraphicsOGL :: getTextureController() {
	return textureController;
}

int GraphicsOGL :: isPCSlow() {
	return (avgFPS < 50);
}


void GraphicsOGL :: idle() {

	// Get Start Time
	fpsStart = getTime();

		// Update All Updateable Objects
		Updateable :: updateAll(this, 1);

		Drawable2 :: removeDestroyed();
		Instantiable :: removeDestroyed();

		// Draw Current Frame
		glutPostRedisplay();


	// Get End Time
	fpsEnd = getTime();



	// Determine Time the Game Took to Process Current Frame
	double runTime = ((fpsEnd-fpsStart) + (drawEnd-drawStart))/1000.;

	// Calculate Time Required to Sleep to Stay at 60 FPS
	long sleepTime = 1000.*1000/60 - runTime;


	// Calculate FPS
	fps = 1000.*1000/(runTime);
	if(fps > 60)
		fps = 60;

	avgFPS = (fps + avgFPS)/2.;

	// If Sleep Time is Positive (Not Running Slow), Sleep for # Nanoseconds
	if(sleepTime > 0)
		usleep(sleepTime);
}

void GraphicsOGL :: display() {


	drawStart = getTime();

	


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	glAlphaFunc(GL_GREATER, 0);


	globalTime += 1;
	
	glColor3f(1,1,1);


	// DRAW WORLD

	setPerspective();

		// Draw All Drawable Objects
		terrain->drawFirst(this, 1);
		Drawable2 :: drawAll(this, 1);
		terrain->drawLast(this, 1);

	// DRAW HUD

	setOrtho();
		string fpsStr = "FPS: ", dirStr = "Dir: ", posStr = "Pos: ";
			fpsStr = fpsStr + to_string(fps);
			dirStr = dirStr + to_string(getCamDir());
			posStr = posStr + to_string(myPlayer->getX()) + ", " + to_string(myPlayer->getY()) + ", " + to_string(myPlayer->getZ());

		setFont("8bit");

		float s = 1;
		drawStringScaled(0,0,s,s,fpsStr);
		drawStringScaled(0,20,s,s,dirStr);
		drawStringScaled(0,40,s,s,posStr);


	// Flush Graphics to Screen
	glFlush(); 
    	glutSwapBuffers();


	drawEnd = getTime();
}

//CAMERA FUNCTIONS
	float GraphicsOGL :: getCamDir() {
		return glCamera->getCamDir();
	}

	void GraphicsOGL :: setProjectionPrep(float cX, float cY, float cZ, float tX, float tY, float tZ) {
		glCamera->setProjectionPrep(cX,cY,cZ,tX,tY,tZ);
	}

//DRAWING FUNCTIONS
	void GraphicsOGL :: setColor(int R, int G, int B) {
		glColor3i(R, G, B);
	}

	void GraphicsOGL :: drawPoint(float x, float y) {
		float depth = 0;
	
		// Create Point Primitive at Point (x,y)
		glBegin(GL_POINTS);
			glVertex3f(x, y, depth);
		glEnd();
	}

	void GraphicsOGL :: drawLine(float x1, float y1, float x2, float y2) {
		float depth = 0;
	
		// Create Line Primitive from (x1,y1) to (x2,y2)
		glBegin(GL_LINES);
			glVertex3f(x1, y1, depth);
			glVertex3f(x2, y2, depth);
		glEnd();
	}

	void GraphicsOGL :: drawRect(float x1, float y1, float x2, float y2) {
		float depth = 0;
	
		glBegin(GL_LINE_LOOP);
			glTexCoord2f(0,0);
				glVertex3f(x1, y1, depth);
			glTexCoord2f(1,0);
				glVertex3f(x2, y1, depth);
			glTexCoord2f(1,1);
				glVertex3f(x2, y2, depth);
			glTexCoord2f(0,1);
				glVertex3f(x1, y2, depth);
		glEnd();
	}

	void GraphicsOGL :: fillRect(float x1, float y1, float x2, float y2) {
		float depth = 0;
	
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
				glVertex3f(x1, y1, depth);
			glTexCoord2f(1,0);
				glVertex3f(x2, y1, depth);
			glTexCoord2f(1,1);
				glVertex3f(x2, y2, depth);
			glTexCoord2f(0,1);
				glVertex3f(x1, y2, depth);
		glEnd();
	}

	void GraphicsOGL :: drawCircle(float x, float y, float r, int vertNum) {
		float depth = 0, dir, xN, yN;
	
		glBegin(GL_LINE_LOOP);
			for(float i = 0; i < vertNum; i++) {
				dir = i/vertNum*360;
				xN = calcLenX(1,dir);
				yN = -calcLenY(1,dir);
	
				glTexCoord2f(xN,yN);
					glVertex3f(x + r*xN, y + r*yN, depth);
			}
		glEnd();
	}

	void GraphicsOGL :: fillCircle(float x, float y, float r, int vertNum) {
		float depth = 0, dir, xN, yN;
	
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(.5,.5);
				glVertex3f(x, y, depth);
			for(float i = 0; i < vertNum+1; i++) {
				dir = i/vertNum*360;
				xN = calcLenX(1,dir);
				yN = -calcLenY(1,dir);
	
				glTexCoord2f(xN,yN);
					glVertex3f(x + r*xN, y + r*yN, depth);
			}
		glEnd();
	}

	void GraphicsOGL :: drawPolygon(float x, float y, float r, int vertNum, float angle) {
		float depth = 0, dir, xN, yN;
	
		glBegin(GL_LINE_LOOP);
			for(float i = 0; i < vertNum; i++) {
				dir = angle + i/vertNum*360;
				xN = calcLenX(1,dir);
				yN = -calcLenY(1,dir);
	
				glTexCoord2f(xN,yN);
					glVertex3f(x + r*xN, y + r*yN, depth);
			}
		glEnd();
	}

	void GraphicsOGL :: fillPolygon(float x, float y, float r, int vertNum, float angle) {
		float depth = 0, dir, xN, yN;
	
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(.5,.5);
				glVertex3f(x, y, depth);
			for(float i = 0; i < vertNum+1; i++) {
				dir = angle + i/vertNum*360;
				xN = calcLenX(1,dir);
				yN = -calcLenY(1,dir);
	
				glTexCoord2f(xN,yN);
					glVertex3f(x + r*xN, y + r*yN, depth);
			}
		glEnd();
	}

	void GraphicsOGL :: drawTexture(float x, float y, Texture* tex) {
		drawTextureScaled(x, y, 1, 1, tex);
	}

	void GraphicsOGL :: drawTextureScaled(float x, float y, float xS, float yS, Texture* tex) {
		if(tex == NULL) {
			cout << "Attempted to draw NULL texture." << endl;
			return;
		}

		glEnable(GL_TEXTURE_2D);
		tex->bind();


		float depth = 0, w, h;
		w = xS*tex->getWidth();
		h = yS*tex->getHeight();
	
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
				glVertex3f(x, y, depth);
			glTexCoord2f(1,0);
				glVertex3f(x+w, y, depth);
			glTexCoord2f(1,1);
				glVertex3f(x+w, y+h, depth);
			glTexCoord2f(0,1);
				glVertex3f(x, y+h, depth);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


//3D DRAWING

	// TRANSFORMATIONS
		void GraphicsOGL :: transformTranslation(float x, float y, float z) {
			glTranslatef(x, y, z);
		}
		void GraphicsOGL :: transformScale(float s) {
			transformScale(s, s, s);
		}
		void GraphicsOGL :: transformScale(float xS, float yS, float zS) {
			glScalef(xS, yS, zS);
		}
		void GraphicsOGL :: transformRotationX(float angle) {
			glRotatef(angle, 1, 0, 0);
		}
		void GraphicsOGL :: transformRotationY(float angle) {
			glRotatef(angle, 0, 1, 0);
		}
		void GraphicsOGL :: transformRotationZ(float angle) {
			glRotatef(angle, 0, 0, 1);
		}
		void GraphicsOGL :: transformClear() {
			glLoadIdentity();
		}


	// PRIMITIVES
		void GraphicsOGL :: draw3DSphere(float x, float y, float z, float r, int numPts) {
			draw3DSphere(x,y,z,r,numPts, NULL);
		}
		void GraphicsOGL :: draw3DSphere(float x, float y, float z, float r, int numPts, Texture* tex) {
			float z1, z2, xN1,yN1, xN2,yN2, zDir1, zDir2, xyN1,xyN2, zN1, zN2, dir;

			if(tex != NULL) {
				glEnable(GL_TEXTURE_2D);
				tex->bind();
			}

			for(float i = 0; i < numPts; i++) {

				glBegin(GL_TRIANGLE_STRIP);

				for(float j = 0; j < numPts; j++) {

					zDir1 = 90*2*r*(.5-(i/numPts));
					zDir2 = 90*2*r*(.5-((i+1)/numPts));

					xyN1 = calcLenX(1,zDir1);
					xyN2 = calcLenX(1,zDir2);
					zN1 = calcLenY(1,zDir1);
					zN2 = calcLenY(1,zDir2);

				
					dir = j/numPts*360;
						xN1 = calcLenX(1,dir)*xyN1;
						yN1 = calcLenY(1,dir)*xyN1;
						xN2 = calcLenX(1,dir)*xyN2;
						yN2 = calcLenY(1,dir)*xyN2;

					glTexCoord2f(j/numPts,i/numPts);
					glNormal3f(xN1,yN1,zN1);
						glVertex3f(x+xN1*r, y+yN1*r, z+zN1*r);
					glTexCoord2f(j/numPts,(i+1)/numPts);
					glNormal3f(xN2,yN2,zN2);
						glVertex3f(x+xN2*r, y+yN2*r, z+zN2*r);


					dir = (j+1)/numPts*360;
						xN1 = calcLenX(1,dir)*xyN1;
						yN1 = calcLenY(1,dir)*xyN1;
						xN2 = calcLenX(1,dir)*xyN2;
						yN2 = calcLenY(1,dir)*xyN2;

					glTexCoord2f((j+1)/numPts,(i+1)/numPts);
					glNormal3f(xN2,yN2,zN2);
						glVertex3f(x+xN2*r, y+yN2*r, z+zN2*r);
					glTexCoord2f((j+1)/numPts,i/numPts);
					glNormal3f(xN1,yN1,zN1);
						glVertex3f(x+xN1*r, y+yN1*r, z+zN1*r);
				}

				glEnd();
			}

			if(tex != NULL) {
				glDisable(GL_TEXTURE_2D);
				tex->unbind();
			}
		}

		void GraphicsOGL :: draw3DWall(float x1, float y1, float z1, float x2, float y2, float z2) {
			draw3DWall(x1,y1,z1,x2,y2,z2,NULL);
		}
		void GraphicsOGL :: draw3DWall(float x1, float y1, float z1, float x2, float y2, float z2, Texture* tex) {
			draw3DWall(x1,y1,z1,x2,y2,z2,tex,1,1);
		}
		void GraphicsOGL :: draw3DWall(float x1, float y1, float z1, float x2, float y2, float z2, Texture* tex, float xRepeat, float yRepeat) {

			if(tex != NULL) {
				glEnable(GL_TEXTURE_2D);
				tex->bind();
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glBegin(GL_QUADS);
				glTexCoord2d(0.0, 0.0); 	
					glVertex3d(x1, y1, z1);
				glTexCoord2d(xRepeat, 0.0);
					glVertex3d(x2, y2, z1);
				glTexCoord2d(xRepeat, yRepeat);
					glVertex3d(x2, y2, z2);
				glTexCoord2d(0.0, yRepeat);
					glVertex3d(x1, y1, z2);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			if(tex != NULL)
				tex->unbind();
		}

		void GraphicsOGL :: draw3DFloor(float x1, float y1, float x2, float y2, float z) {
			draw3DFloor(x1,y1,x2,y2,z,NULL);
		}
		void GraphicsOGL :: draw3DFloor(float x1, float y1, float x2, float y2, float z, Texture* tex) {
			draw3DFloor(x1,y1,x2,y2,z,NULL,1,1);
		}
		void GraphicsOGL :: draw3DFloor(float x1, float y1, float x2, float y2, float z, Texture* tex, float xRepeat, float yRepeat) {

			if(tex != NULL) {
				glEnable(GL_TEXTURE_2D);
				tex->bind();
			}
			
			glAlphaFunc(GL_GREATER, 0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
			glBegin(GL_QUADS);
				glTexCoord2d(0.0, 0.0);
					glVertex3d(x1, y1, z);
				glTexCoord2d(xRepeat, 0.0);
					glVertex3d(x2, y1, z);
				glTexCoord2d(xRepeat, yRepeat);
					glVertex3d(x2, y2, z);
				glTexCoord2d(0.0, yRepeat);
					glVertex3d(x1, y2, z);
			glEnd();

			if(tex != NULL) {
				glDisable(GL_TEXTURE_2D);
				tex->unbind();
			}
		}

		void GraphicsOGL :: draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2) {
			// Create Line Primitive from (x1,y1,z1) to (x2,y2,z2)
			glBegin(GL_LINES);
				glVertex3f(x1, y1, z1);
				glVertex3f(x2, y2, z2);
			glEnd();
		}


		void GraphicsOGL :: draw3DBlock(float x1, float y1, float z1, float x2, float y2, float z2) {
			draw3DBlock(x1,y1,z1,x2,y2,z2,NULL);
		}
		void GraphicsOGL :: draw3DBlock(float x1, float y1, float z1, float x2, float y2, float z2, Texture* tex) {
			draw3DBlock(x1,y1,z1,x2,y2,z2,tex,tex);
		}
		void GraphicsOGL :: draw3DBlock(float x1, float y1, float z1, float x2, float y2, float z2, Texture* texWall, Texture* texFloor) {
			draw3DWall(x1,y1,z1,x1,y2,z2,texWall);
			draw3DWall(x2,y1,z1,x2,y2,z2,texWall);
			draw3DWall(x1,y1,z1,x2,y1,z2,texWall);
			draw3DWall(x1,y2,z1,x2,y2,z2,texWall);
			draw3DFloor(x1,y1,x1,y2,z1,texFloor);
			draw3DFloor(x1,y1,x1,y2,z2,texFloor);
		}


		void GraphicsOGL :: draw3DCircle(float x, float y, float z, float rad, int sideNum) {
			draw3DPrism(x,y,z,rad,0,sideNum,NULL);
		}
		void GraphicsOGL :: draw3DCircle(float x, float y, float z, float rad, int sideNum, Texture* tex) {
			draw3DPrism(x,y,z,rad,0,sideNum,tex);
		}

		void GraphicsOGL :: draw3DPrism(float x, float y, float z, float rad, float h, int sideNum) {
			draw3DPrism(x,y,z,rad,h,sideNum,NULL);
		}
		void GraphicsOGL :: draw3DPrism(float x, float y, float z, float rad, float height, int sideNum, Texture* tex) {
			float ang = 45, dir, xN, yN;

			if(tex != NULL) {
				glEnable(GL_TEXTURE_2D);
				tex->bind();
			}

			// Draw Top of Player Model
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(.5, .5);
					glVertex3f(x,y,z+height);

				for(int i = 0; i <= sideNum; i++) {
					dir = ang + 1.*i/sideNum*360;

					xN = calcLenX(1,dir);
					yN = calcLenY(1,dir);

					glTexCoord2f(.5 + .5*xN,.5 + .5*yN);
						glVertex3f(x+xN*rad, y+yN*rad, z+height);
				}
			glEnd();

			if(height != 0) {
				// Draw Top of Player Model
				glBegin(GL_TRIANGLE_FAN);
					glTexCoord2f(.5, .5);
						glVertex3f(x,y,z);

					for(int i = 0; i <= sideNum; i++) {
						dir = ang + 1.*i/sideNum*360;

						xN = calcLenX(1,dir);
						yN = calcLenY(1,dir);

						glTexCoord2f(.5 + .5*xN,.5 + .5*yN);
							glVertex3f(x+xN*rad, y+yN*rad, z);
					}
				glEnd();

				// Draw Side Faces of Player Model
				for(int i = 0; i < sideNum; i++) {
					glBegin(GL_QUADS);

						dir = ang + 1.*i/sideNum*360;
							xN = calcLenX(1,dir);
							yN = calcLenY(1,dir);

						glTexCoord2f(0,0);
							glVertex3f(xN*rad, yN*rad, height);
						glTexCoord2f(0,1);
							glVertex3f(xN*rad, yN*rad, 0);


						dir = ang + (i+1.)/sideNum*360;
							xN = calcLenX(1,dir);
							yN = calcLenY(1,dir);

						glTexCoord2f(1,1);
							glVertex3f(x+xN*rad, y+yN*rad, z);
						glTexCoord2f(1,0);
							glVertex3f(x+xN*rad, y+yN*rad, z+height);
					glEnd();
				}
			}

			if(tex != NULL) {
				glDisable(GL_TEXTURE_2D);
				tex->unbind();
			}
		}

//SHADERS


	void GraphicsOGL :: disableShaders() {
		textureController->getTexture("Noise")->unbind(GL_TEXTURE2);
		glUseProgram(0);
	}

	void GraphicsOGL :: enableShader(string name) {
		enableShader(shaderController->getProgram(name));
	}

	void GraphicsOGL :: enableShader(GLuint program) {

		Texture* noiseTex = textureController->getTexture("Noise");

		// Set Current Shader Program
		curProgram = program;

		// Enable Current Shader
		glUseProgram(program);

		// ** Pass Screen Resolution and Time Value (Used in Animations) to Shader
		glUniform2fv(glGetUniformLocation(curProgram, "iResolution"), 1, resolution);			
    		glUniform1f(glGetUniformLocation(curProgram, "iGlobalTime"), globalTime/50.);      
		glUniform1f(glGetUniformLocation(curProgram, "iRadius"), 0);


		// Get Camera Position and Direction
		float camPos[3], sCamPos[3], camDir[3];
		glCamera->getPosition(camPos);
		glCamera->getShaderPosition(sCamPos);
		glCamera->getDirection(camDir);

		// ** Pass Camera Position and Direction to Shader
		glUniform3fv(glGetUniformLocation(curProgram, "rCamPos"), 1, camPos);
		glUniform3fv(glGetUniformLocation(curProgram, "iCamPos"), 1, sCamPos);
		glUniform3fv(glGetUniformLocation(curProgram, "iCamDir"), 1, camDir);  

		glUniform1f(glGetUniformLocation(curProgram, "seaLevel"), terrain->getSeaLevel());  

		// Pass Texture(s) to Shader
		glUniform1i(glGetUniformLocation(curProgram, "Texture0"), 0);
    		glUniform1i(glGetUniformLocation(curProgram, "Texture1"), 1);
    		glUniform1i(glGetUniformLocation(curProgram, "noiseTex"), 2);
			noiseTex->bind(GL_TEXTURE2);

		

		float locations[3] = {
			myPlayer->getX(), myPlayer->getY(), myPlayer->getZ()
		/*,
		  0, 1.0, 0,
		  0, 0, 0*/
		};
		glUniform3fv(glGetUniformLocation(curProgram, "lights"), 1, locations);


		float r = 640, l = 0, t = 640, b = 0, f = 1000, n = 1; 
		//mat4x4 uMMatrix(2*n/(r-l),0,(r+l)/(r-l),0,0,2*n/(t-b),(t+b)/(t-b),0,0,0,-(f+n)/(f-n),(-2*f*n)/(f-n),0,0,-1,0);

		GLfloat uMMatrix[16], uMatrix[16] = {
			2*n/(r-l),0,(r+l)/(r-l),0,0,2*n/(t-b),(t+b)/(t-b),0,0,0,-(f+n)/(f-n),(-2*f*n)/(f-n),0,0,-1,0
		};

		gluInvertMatrix(uMatrix, uMMatrix);

		glUniformMatrix4fv(glGetUniformLocation(curProgram, "uMMatrix"), 1, GL_FALSE, uMMatrix);
	}

	void GraphicsOGL :: setCurrentTextureSize(int w, int h) {
		if(curProgram == 0)
			return;

		float si[2];
		si[0] = w;
		si[1] = h;

		glUniform2fv(glGetUniformLocation(curProgram, "iSize"), 1, si);				
	}

//TEXT DRAWING
	void GraphicsOGL :: setFont(string name) {
		curFont = fontController->getFont(name);
	}

	float GraphicsOGL :: drawChar(float x, float y, char c) {
		return drawCharScaled(x, y, 1, 1, c);
	}

	float GraphicsOGL :: drawCharScaled(float x, float y, float xS, float yS, char c) {
		Texture* t = curFont->getChar(c);
		if(t == NULL) {
			cout << "Attempted to draw a character not present in the font." << endl;
			return 0;
		}

		if(c == 'y' || c == 'g' || c == 'j' || c == 'p' || c == 'q')
			y += .25*t->getHeight()*yS;

		drawTextureScaled(x, y, xS, yS, t);
		return t->getWidth()*xS;
	}

	void GraphicsOGL :: drawString(float x, float y, string str) {
		drawStringScaled(x,y,1,1,str);
	}

	void GraphicsOGL :: drawStringScaled(float x, float y, float xS, float yS, string str) {
		int len = str.length();
		char c;

		float s = 8, e = -1;

		float dX = x, dY = y;

		for(int i = 0; i < len; i++) {
			c = str[i];

			if(c == '\n') { 
				dY += s*yS + e*yS;
				dX = x;
			}
			else if(c == ' ')
				dX += s*xS + e*xS;
			else if(islower(c))
				dX += drawCharScaled(dX,dY + (s*yS*.25), xS,yS*.75, c) + e*xS;
			else
				dX += drawCharScaled(dX,dY, xS, yS, c) + e*xS;
		}
	}

//Graphics Mode
void GraphicsOGL :: setOrtho() {
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1000, 1000);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH);
}

void GraphicsOGL :: setPerspective() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glCamera->setProjection(this);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH);
}


unsigned long GraphicsOGL :: getTime() {

	nanoseconds ms = duration_cast< nanoseconds >(
 		high_resolution_clock::now().time_since_epoch()
	);

	return ms.count();
}


bool GraphicsOGL :: gluInvertMatrix(const float m[16], float invOut[16]) {
    float inv[16], det;
    int i;
 
    inv[0] = m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];
 
    inv[4] = -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];
 
    inv[8] = m[4]  * m[9] * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5] * m[15] +
    m[8]  * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];
 
    inv[12] = -m[4]  * m[9] * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5] * m[14] -
    m[8]  * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];
 
    inv[1] = -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2] * m[15] -
    m[9]  * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];
 
    inv[5] = m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2] * m[15] +
    m[8]  * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];
 
    inv[9] = -m[0]  * m[9] * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1] * m[15] -
    m[8]  * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];
 
    inv[13] = m[0]  * m[9] * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1] * m[14] +
    m[8]  * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];
 
    inv[2] = m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];
 
    inv[6] = -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];
 
    inv[10] = m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];
 
    inv[14] = -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];
 
    inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];
 
    inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];
 
    inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];
 
    inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];
 
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
 
    if (det == 0)
        return false;
 
    det = 1.0 / det;
 
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
 
    return true;
}
