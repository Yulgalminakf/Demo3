varying vec2 gUV;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gUV = gl_MultiTexCoord0;
}
