uniform vec3 color;
varying vec3 fragColor;
attribute mat4 matrix;
varying out unsigned int instanceID;

void main(void)
{
	fragColor = color;
	instanceID = gl_InstanceID;
	gl_Position = gl_ModelViewProjectionMatrix * matrix * gl_Vertex;
}