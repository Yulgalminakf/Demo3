attribute vec3 color;
varying vec3 fragColor;
char3 test;
void main(void)
{
	fragColor = color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}