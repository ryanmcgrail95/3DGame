// Math2D.cpp


#include <cmath>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include "Math2D.h"
using namespace std::chrono;



// BASIC
	float sqr(float x) {
		return x*x;
	}

// GEOMETRIC
	float calcPtDis(float x1, float y1, float x2, float y2) {
		return sqrt(sqr(x2-x1) + sqr(y2-y1));
	}

// TRIG
	float calcLenX(float len, float dir) {
		return len * cos(dir/180*PI);
	}

	float calcLenY(float len, float dir) {
		return len * sin(dir/180*PI);
	}

	float calcPtDir(float x1, float y1, float x2, float y2) {
		return atan2(y2-y1, x2-x1)/PI*180;
	}

	float modf(float x, float y) {
		if(x == y)
			return 0;
		else if(x < y)
			return x;
		else {
			int t = 0;

			while(y < x)
				x -= y;

			return x;
		}
	}

	unsigned long getTime() {

		nanoseconds ms = duration_cast< nanoseconds >(
	 		high_resolution_clock::now().time_since_epoch()
		);

		return ms.count();
	}

	float rnd() {
		return ((getTime()*rand())%100)/100.;
	}

// MAGICAL!!!
	float calcTurnToDir(float dir, float toDir) {
		float newDir = 0;
		
		if(fabs(fmod(fmod(toDir - dir, 360.) + 540., 360.) - 180) >= 160)
			newDir += sin((toDir-dir)/180*PI)*16*15;

		newDir += sin((toDir-dir)/180*PI)*8;
		return newDir;
	}

	float calcAngleDiff(float dir1, float dir2) {
		return modf((modf(dir1 - dir2, 360) + 540), 360) - 180;
	}

/*float abs(float n) {
	return (n > 0) ? n : -n;
}*/
