// TextureExt.h


#ifndef TEXTUREEXT_H
#define TEXTUREEXT_H

#include <string>
#include <vector>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "Texture.h"
using namespace std;

class TextureExt {
	public:
		TextureExt(Texture*);
		TextureExt(string);
		void loadDirectory(string);
		Texture* getFrame(int);
		Texture* getFrame(float);
		void bind(int);
		void unbind(int);
		int getWidth();
		int getWidth(int);
		int getHeight();
		int getHeight(int);
		int getFrameNumber();
		
	private:
		void addFrame(Texture*);
		void addFrame(string);

		vector<Texture*> frameList;
};

#endif