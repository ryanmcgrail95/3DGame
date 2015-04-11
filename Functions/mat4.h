// mat4.h

#ifndef MAT4_H
#define MAT4_H


class mat4 {
	public:
		mat4(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);
		void getArray(float[16]);
		void setIdentity();
		
		const mat4& operator=(const mat4&);
		mat4& operator*=(const mat4&);
	

	private:
		float array[4][4];
};

#endif
