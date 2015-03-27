// Updateable.h


#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include <deque>
#include "../Graphics/GraphicsOGL.h"

class GraphicsOGL;
class Updateable {
	public:
		static void updateAll(GraphicsOGL*, float);
		virtual void update(GraphicsOGL*, float);

	protected:
		Updateable();
		Updateable(int);

		int oType;

		static deque<Updateable*> updateableList;

		static const int TYPE_NONE = -1;
		static const int TYPE_INVENTORY = 0;
};

#endif
