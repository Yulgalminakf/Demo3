attribute vec3 color;
attribute mat4 matrix;
varying out vec3 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = gl_ModelViewProjectionMatrix * matrix * gl_Vertex;
}