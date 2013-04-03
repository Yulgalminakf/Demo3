uniform sampler2D tex1;
uniform sampler2D tex2;
varying vec2 gUV;

void main(void)  
{
	gl_FragColor = texture2D(tex1, gUV);
}    
