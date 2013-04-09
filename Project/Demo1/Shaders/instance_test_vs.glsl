attribute vec3 color;
attribute mat4 matrix;
uniform mat4 globalMatrix;
uniform mat4 individualMatrix;
varying out vec3 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = gl_ModelViewProjectionMatrix * globalMatrix * matrix * individualMatrix * gl_Vertex;
}