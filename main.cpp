// main.cpp

#include <iostream>
#include "Graphics/GraphicsOGL.h"
#include "Global.h"
using namespace std;


int argc;
char** argv;

int main(int arc, char* arv[]) {

	argc = arc;
	argv = arv;

	GraphicsOGL(arc,arv);
}
