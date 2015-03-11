// Instantiable.cpp


#include "../Graphics/GraphicsOGL.h"
#include "Drawable.h"
#include "Instantiable.h"


Instantiable :: Instantiable() : Drawable2() {	
}

void Instantiable :: update(GraphicsOGL* gl, float deltaTime) {

	// Run Parent Class's Update Function
	Drawable2 :: update(gl, deltaTime);
}

void Instantiable :: draw(GraphicsOGL* gl, float deltaTime) {

	// Run Parent Class's Draw Function
	Drawable2 :: draw(gl, deltaTime);
}