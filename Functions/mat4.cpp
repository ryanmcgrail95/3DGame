// mat4.cpp


#include <iostream>
#include "mat4.h"
using namespace std;

mat4 :: mat4(float nA1, float nA2, float nA3, float nA4, float nB1, float nB2, float nB3, float nB4, float nC1, float nC2, float nC3, float nC4, float nD1, float nD2, float nD3, float nD4) {
	array[0][0] = nA1;
	array[0][1] = nA2;
	array[0][2] = nA3;
	array[0][3] = nA4;

	array[1][0] = nB1;
	array[1][1] = nB2;
	array[1][2] = nB3;
	array[1][3] = nB4;

	array[2][0] = nC1;
	array[2][1] = nC2;
	array[2][2] = nC3;
	array[2][3] = nC4;

	array[3][0] = nD1;
	array[3][1] = nD2;
	array[3][2] = nD3;
	array[3][3] = nD4;
}
	
const mat4& mat4 :: operator=(const mat4& other) {
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			array[r][c] = other.array[r][c];

	return *this;
}
	
mat4& mat4 :: operator*=(const mat4& other) {
	float oldValues[4][4];
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			oldValues[r][c] = array[r][c];

	float v;
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++) {
			v = 0;
			for(int i = 0; i < 4; i++)
				v += oldValues[r][i]*other.array[i][c];
			array[r][c] = v;
		}

	return *this;
}

void mat4 :: setIdentity() {
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			array[r][c] = (r == c);
}
	
void mat4 :: getArray(float out[16]) {
	int i = 0;
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			out[i++] = array[r][c];
}
