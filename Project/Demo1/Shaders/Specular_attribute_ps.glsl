uniform vec4 materialAmbientReflectivity;
uniform vec4 ambientIntensity;

uniform sampler2D tex1;
uniform sampler2D tex2;
varying vec2 gUV;

uniform vec3 lightIntensity1;
uniform vec3 lightIntensity2;
uniform vec3 lightIntensity3;
uniform vec3 lightIntensity4;
uniform vec4 materialDiffuseReflectivity;

uniform vec4 materialSpecularReflectivity;

uniform vec3 lightPosition1;
uniform vec3 lightPosition2;
uniform vec3 lightPosition3;
uniform vec3 lightPosition4;
uniform vec3 cameraPosition;

varying vec3 gNormal;
varying vec3 gWorldPos;

uniform float start;
uniform float end;
uniform vec3 fogColor;

uniform vec3 skyAmbientColor;
uniform vec3 groundAmbientColor;

uniform vec3 lightConstants;

uniform vec3 spotLightPosition1;
uniform vec3 spotLightIntensity1;
uniform vec3 spotLightDirection1;
uniform float spotLightPhi1;
uniform float spotLightTheta1;

vec3 spotLightCalc(vec3 position, vec3 intensity, vec3 direction, float phi, float theta)
{
	float cosangle = (dot(direction, normalize(gWorldPos - position)));
	float spotFactor = (cosangle - cos(phi / 2)) / (cos(theta / 2) - cos(phi / 2));
	vec3 spotLight = intensity * spotFactor;
	spotLight = max(spotLight, 0.0);
	return spotLight;
}

vec3 lightingCalc(vec3 lightPos, vec3 tempColor, vec3 vNormal, vec3 vView)
{
	vec3 pointLightDir = normalize(lightPos.xyz - gWorldPos);

	vec3 ref = normalize(reflect(-pointLightDir, vNormal));

	vec3 diffuse = (tempColor.rgb * materialDiffuseReflectivity.rgb * dot(vNormal, pointLightDir));
	vec3 specular = (tempColor.rgb * materialSpecularReflectivity.rgb * pow(dot(vView, ref),materialSpecularReflectivity.w));
	vec3 color = max(specular, 0.0);// + max(diffuse, 0.0f);
	float angleBetween = acos(dot(pointLightDir, vNormal));
	
	if(angleBetween >= 3.1415f / 2.0f)
	//if(dot(pointLightDir, vNormal) < 0)
	{
		color = 0;
	}
	
	return color;
}

inline float Attenuation(vec3 lightPos)
{
	float d = length(lightPos - gWorldPos);
	return min(1 / (lightConstants.x + lightConstants.y * d + lightConstants.z * d * d),1);
}

vec3 finalLightColor()
{
	vec3 vNormal = normalize(gNormal);
	vec3 vView = normalize(cameraPosition.xyz - gWorldPos);
	
	float lerpFactor = vNormal.y / 2 + 0.5;
	vec3 ambient = lerpFactor * skyAmbientColor + (1 - lerpFactor) * groundAmbientColor;
	
	vec3 color1 = lightingCalc(lightPosition1.xyz, lightIntensity1.rgb, vNormal, vView);
	vec3 color2 = lightingCalc(lightPosition2.xyz, lightIntensity2.rgb, vNormal, vView);
	vec3 color3 = lightingCalc(lightPosition3.xyz, lightIntensity3.rgb, vNormal, vView);
	vec3 color4 = lightingCalc(lightPosition4.xyz, lightIntensity4.rgb, vNormal, vView);
	
	float attenuation1 = Attenuation(lightPosition1);
	float attenuation2 = Attenuation(lightPosition2);
	float attenuation3 = Attenuation(lightPosition3);
	float attenuation4 = Attenuation(lightPosition4);
	
	vec3 color = color1 * attenuation1 + color2 * attenuation2 + color3 * attenuation3 + color4 * attenuation4 + ambient;
	
	vec3 spotLight1 = spotLightCalc(spotLightPosition1, spotLightIntensity1, spotLightDirection1, spotLightPhi1, spotLightTheta1);
	float spotLightAttenuation1 = Attenuation(spotLightPosition1);
	
	color += spotLight1 * spotLightAttenuation1;
	
	return color;
}

void main (void)  
{
/*
	fog = clamp(fog, 0, 1);
	//gl_FragColor = (1 - fog) * vec4(fogColor,1);
	vec4 finalColor = (color1 * attenuation1 + color2 * attenuation2 + color3 * attenuation3 + color4 * attenuation4 + vec4(max(ambient, 0.0), 1));
*/
	gl_FragColor = vec4(finalLightColor(),1);
	//vec3 spotLight1 = spotLightCalc(spotLightPosition1, spotLightIntensity1, spotLightDirection1, spotLightPhi1, spotLightTheta1);
	//float spotLightAttenuation1 = Attenuation(spotLightPosition1);
	//gl_FragColor = vec4(spotLight1 * spotLightAttenuation1,1);
}