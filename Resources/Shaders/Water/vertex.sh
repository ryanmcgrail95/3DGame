uniform mat4 uMVPMatrix;
attribute vec4 vPosition;
varying vec4 vColor;
varying vec3 vVertex;

void main() {
	gl_PointSize = 8.0;
	gl_Position = ftransform();

	vColor = vPosition;
	vVertex = vec3(0.,200.,0.); //vPosition.xyz;
}
