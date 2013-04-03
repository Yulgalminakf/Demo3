uniform vec3 color;
uniform float alpha;

void main(void)  
{  
	gl_FragColor = vec4(color, alpha);
}    
