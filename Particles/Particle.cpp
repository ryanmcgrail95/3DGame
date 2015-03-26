// Particle.cpp


#include "../Graphics/GraphicsOGL.h"
#include "Particle.h"
#include "../Primitives/Instantiable.h"

Particle :: Particle(float nX, float nY, float nZ) : Instantiable() {
	x = nX;
	y = nY;
	z = nZ;
}

void Particle :: update(GraphicsOGL* gl, float deltaTime) {
	Instantiable :: update(gl, deltaTime);
}

void Particle :: draw(GraphicsOGL* gl, float deltaTime) {
	Instantiable :: draw(gl, deltaTime);
}