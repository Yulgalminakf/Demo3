varying vec3 fragColor;
varying in unsigned int instanceID;

void main(void)  
{
	gl_FragColor = vec4(fragColor,1);
}