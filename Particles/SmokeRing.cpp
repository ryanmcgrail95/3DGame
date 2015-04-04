// SmokeRing.cpp


#include <iostream>
#include "../Graphics/GraphicsOGL.h"
#include "SmokeRing.h"
#include "Particle.h"
using namespace std;

SmokeRing :: SmokeRing(float nX,float nY,float nZ,float startS,float endS,int nTotSteps, float myAlpha) : Particle(nX,nY,nZ) {
	startSize = curSize = startS;
	endSize = endS;

	totSteps = nTotSteps;

	addAmt = nTotSteps;
	percSmoke = 1;

	alpha = myAlpha;
}

void SmokeRing :: update(GraphicsOGL* gl, float deltaTime) {

	gl->logMessage("SmokeRing.cpp, update()");

	curSize += (endSize - curSize)/addAmt;

	percSmoke += (0 - percSmoke)/addAmt;

	if(abs(curSize - endSize) < .01)
		destroy();
}
		
void SmokeRing :: draw(GraphicsOGL* gl, float deltaTime) {

	gl->logMessage("SmokeRing.cpp, draw()");

	float xyRot, setupRot;
	gl->getHeightmap()->getFloorRotations(x,y,setupRot,xyRot);

	gl->transformClear();
		gl->transformTranslation(x,y,z);
	
		gl->transformRotationZ(setupRot);	
		gl->transformRotationX(xyRot);
		gl->transformRotationZ(-setupRot);

		gl->enableShader("SmokeRing");	
		gl->setShaderVariable("iSmoke", percSmoke*alpha);
		gl->draw3DCircle(0,0,.5,curSize,10);
		gl->disableShaders();

	gl->transformClear();
}
