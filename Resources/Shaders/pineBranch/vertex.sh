uniform float iGlobalTime;
varying vec2 uvCoords;

uniform vec3 camNormal;
varying float dp;


float calcDir(float x, float y) {
	return 3.14159 + atan(y,x);
}

vec3 rotateNormal(vec3 norm, float dir) {
	//mat3 mat = mat3(1, 0, 0, 0, cos(dir), sin(dir), 0, -sin(dir), cos(dir));
	float co = cos(dir), si = sin(dir);

	mat3 mat = mat3(co, si, 0, -si, co, 0, 0, 0, 1);



	return mat*norm;
}

float calcDark() {    

	float camNX, camNY;
	camNX = camNormal.x;
	camNY = camNormal.y;

	vec3 vNormal = gl_Normal;
		//vNormal = rotateNormal(vNormal, calcDir(camNX, camNY));
		//vNormal = (gl_ModelViewMatrix * vNormal).xyz;
	
	vec3 wvNormal = normalize((gl_ModelViewMatrix * vec4(vNormal, 0.0)).xyz);
    	
	return abs(dot(vNormal, camNormal));
}

void main() {
	vec4 vert = gl_Vertex;

	float rX = gl_Vertex.x, rY = gl_Vertex.y, rZ = gl_Vertex.z;	
	//float time = (iGlobalTime*20. + rX*2. + rY*3. + rZ*4.)/30.;
	float time = (iGlobalTime + rX + rY + rZ)*2.;

		float amt = .5;

		vert.y += amt*sin(time);
		vert.x += amt*cos(time);

		vert.z += 1.5*amt*cos(.5*time);


	mat4 mMat = gl_ModelViewProjectionMatrix;

	gl_Position = mMat * vert;

	gl_TexCoord[0] = gl_MultiTexCoord0;


	dp = calcDark();

	uvCoords = gl_TexCoord[0].xy;
		uvCoords += -.15-.15*sin(time/5.)*vec2(0.,1.)*uvCoords.y;
}
