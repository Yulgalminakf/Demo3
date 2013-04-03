attribute vec3 a_position;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(a_position,1);
}
