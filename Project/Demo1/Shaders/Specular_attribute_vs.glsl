//uniform is set from outside shader (in the main code)
//varying is set from inside shader
//attribute is for when fixed functions are disabled

uniform mat4 worldMat;

varying vec3 gNormal;
varying vec3 gWorldPos;
varying vec2 gUV;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;

void main(void)
{
	vec4 rotVert;

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * worldMat * vec4(a_position, 1);
	gNormal = normalize(mat3(worldMat) * a_normal);
	
	gWorldPos = (worldMat * vec4(a_position, 1)).xyz;
	
	gUV = a_uv;
}