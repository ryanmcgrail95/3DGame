// PineTree.cpp

#include "../Graphics/GraphicsOGL.h"
#include "PineTree.h"
#include "Tree.h"
#include "../Graphics/Texture.h"

PineTree :: PineTree(float nX, float nY, float size, float colR, float colG, float colB) : Tree(nX,nY,size) {

	colRed = colR;
	colGreen = colG;
	colBlue = colB;
}

void PineTree :: update(GraphicsOGL* gl, float deltaT) {
	Tree :: update(gl, deltaT);
}

void PineTree :: draw(GraphicsOGL* gl, float deltaT) {

	int fidelity = 10;

	gl->transformClear();
	gl->transformTranslation(x,y,z);
	gl->transformScale(size);

	float R = 20, G = 255, B = 50;
	float r = 15, dZ, h = 30, uR, uDZ, uH;

	Texture* branchTex = gl->getTextureController()->getTexture("pineBranch");
	Texture* barkTex = gl->getTextureController()->getTexture("bark");

	
	/*float bR, bG, bB;
		bR = 255*(colGreen);
		bG = 255*(1-colGreen);
		bB = 255*(1-colGreen);

	gl->setColor(bR,bG,bB);*/
	gl->draw3DPrism(0,0,0,5,30, fidelity, barkTex);


	gl->enableShader("pineBranch");
	gl->setColor(255*colRed, 255*colGreen, 255*colBlue);

	for (int i = 0; i < 3; i++) {
		r -= 2*i;
		dZ += 13+i;
		h -= 2*i;

		uR = r;
		uDZ = dZ;
		uH = h;
		for (int j = 0; j < 2; j++) {
			uDZ += 3;

			//float aR, aG, aB;
			//aR = rand();

			//gl->setColor(R+aR,G+aG,B+aB);
			
			gl->setShaderVariable("iDark", pow((3.-i)/3.,2.));

			gl->draw3DCone(0,0,uDZ,uR,uH, fidelity, branchTex);
			gl->draw3DCone(0,0,uDZ,uR*1.1,uH*.8, fidelity, branchTex);
		}
	}

	gl->setColor(255,255,255);
	gl->disableShaders();

	gl->transformClear();
}