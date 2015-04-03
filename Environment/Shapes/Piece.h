// Piece.h

#ifndef PIECE_H
#define PIECE_H

#define PTYPE_NULL     -1
#define PTYPE_FLOOR 	0
#define PTYPE_WALL 	1
#define PTYPE_BLOCK	2
#define PTYPE_ELLIPSOID	3
#define PTYPE_GROUP	4
#define PTYPE_PRISM	5

class Piece : public Environmental {
	
	public:
		Piece();
		virtual int collide();
		virtual float calcFloorZ();

	protected:
		const int TYPE;
		float x;
		float y;
		float z;
		float xScale;
		float yScale;
		float zScale;
		float xRot;
		float yRot;
		float zRot;
}

#endif