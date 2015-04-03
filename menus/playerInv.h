#ifndef PLAYERINV_H
#define PLAYERINV_H

#include<iostream>
#include <iomanip>
#include <vector>
#include "../Graphics/GraphicsOGL.h"
#include "../Primitives/Drawable.h"
#include "../Graphics/TexturePack.h"
#include "../Graphics/Texture.h"

#include "item.h"
#include "invslot.h"
#include "inventory.h"
using namespace std;
class playerInv {
	friend ostream& operator<<(ostream&, const playerInv);
	public:
		playerInv(); // constructor
		void draw(GraphicsOGL*, float);
		void update(InputController* Input, int x, int y, int invwidth, int invheight, double Rot);
		void updateDrawCoords(int x, int y, int fullwidth, int fullheight);
		int posInRange(int x, int y, int x1, int y1, int x2, int y2);
		Inventory *InvAt(int x, int y);
	private:
		InvSlot *CursorSlot;
		Inventory *inv;
		vector<Item*> items;
		TexturePack *Textures;
		
		Texture *Cursor;
		
		int mouseX;
		int mouseY;
		
		int leftx,topy;
		int inswidth,insheight;
		int slotwidth,slotheight;
};

#endif