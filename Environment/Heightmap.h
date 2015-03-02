// Heightmap.h

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <string>
#include "../Graphics/GraphicsOGL.h"
#include "../Primitives/Drawable.h"
#include "../Graphics/Image.h"
#include "../Graphics/Texture.h"
using namespace std;

class GraphicsOGL;
class Heightmap : public Drawable2 {

	public:
		Heightmap(float, float, float);
		Heightmap(float, float, string);
		Heightmap(float, float, int**);
		Heightmap(float, float, float**);

		void draw(GraphicsOGL*, float);

		float getHeightIJ(int, int);
		float getHeightXY(float, float);
		void getNormal(float, float, float [3]);
		bool isFloor(float, float);
		bool isWall(float, float);
	private:
		void setHeight(int, int, float);

		Texture* tex;
		Image* hmImg;
		void load(string);
		float** heightGrid;
		float xSize;
		float ySize;
		int xNum;
		int yNum;
		float scale;
};

#endif
