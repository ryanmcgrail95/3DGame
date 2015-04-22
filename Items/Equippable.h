// Equippable.h


#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "../Graphics/GraphicsOGL.h"
#include "Item.h"


class Equippable : public Item {
	public:
		~Equippable();
		
		virtual void update(GraphicsOGL*, float);
		virtual void draw(GraphicsOGL*, float);

		void equip();
	
	protected:
		Equippable();

		float atkBoost;
		float defBoost;
}

#endif