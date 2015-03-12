uniform vec2 iResolution;
uniform float iGlobalTime;

void main() {
	vec2 coords = gl_TexCoord[0].xy;
	vec2 uv = coords;


 	vec3 col = vec3(uv,0.5+0.5*sin(iGlobalTime));

	gl_FragColor = vec4(col,1);
}
