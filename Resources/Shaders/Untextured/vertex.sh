uniform mat4 uMVPMatrix;
varying vec4 vColor;

void main() {
	gl_Position = ftransform();
	vColor = gl_Color;
}