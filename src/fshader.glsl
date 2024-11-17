#version 430

const int NO_LIGHT = 0, GOURAUD = 1, PHONG = 2;

in vec4 fragPos;
in vec4 color;
in vec4 normal;
in vec2 texCoord;

out vec4  fColor;

uniform int shadeMode;
uniform mat4 mView;
uniform int isTexture;
uniform int objectType;
uniform sampler2D earTexture;
uniform sampler2D faceTexture;
uniform sampler2D bodyTexture;
uniform sampler2D bagTexture;
uniform sampler2D foreArmTexture;
uniform sampler2D armTexture;
uniform sampler2D upperLegTexture;
uniform sampler2D lowerLegTexture;

void main() 
{ 
	vec4 L = normalize(vec4(2, 2, 3, 0));
	float kd = 0.9, ks = 1.0, ka = 0.2, shininess = 10;
	vec4 Id = color;
	vec4 Is = vec4(1, 1, 1, 1);
	vec4 Ia = color;

	if (shadeMode == NO_LIGHT)
	{
		if (isTexture == 1) {
			if (objectType == 0) fColor = texture( earTexture, texCoord ).rgba;
			else if (objectType == 1) fColor = texture( faceTexture, texCoord ).rgba;
			else if (objectType == 2) fColor = texture( bodyTexture, texCoord ).rgba;
			else if (objectType == 3) fColor = texture( bagTexture, texCoord ).rgba;
			else if (objectType == 4) fColor = texture( foreArmTexture, texCoord ).rgba;
			else if (objectType == 5) fColor = texture( armTexture, texCoord ).rgba;
			else if (objectType == 6) fColor = texture( upperLegTexture, texCoord ).rgba;
			else fColor = texture( lowerLegTexture, texCoord ).rgba;
		} 
		else {
			fColor = color;
		}
	}
	else if (shadeMode == GOURAUD)
	{
		if (isTexture == 1) {
			if (objectType == 0) fColor = color * texture( earTexture, texCoord ).rgba;
			else if (objectType == 1) fColor = color * texture( faceTexture, texCoord ).rgba;
			else if (objectType == 2) fColor = color * texture( bodyTexture, texCoord ).rgba;
			else if (objectType == 3) fColor = color * texture( bagTexture, texCoord ).rgba;
			else if (objectType == 4) fColor = color * texture( foreArmTexture, texCoord ).rgba;
			else if (objectType == 5) fColor = color * texture( armTexture, texCoord ).rgba;
			else if (objectType == 6) fColor = color * texture( upperLegTexture, texCoord ).rgba;
			else fColor = color * texture( lowerLegTexture, texCoord ).rgba;
		} 
		else {
			fColor = color;
		}
	}
	else // if (shadeMode == PHONG)
	{
		// ambient
		float ambient = ka;

		// diffuse
		vec4 N = normalize(normal);
		float diff = kd * clamp(dot(N, L), 0, 1);

		// specular
		vec4 viewPos = inverse(mView) * vec4(0, 0, 0, 1);
		vec4 V =  normalize(viewPos - fragPos);
		vec4 R = reflect(-L, N);
		float spec = ks * pow(clamp(dot(V, R), 0, 1), shininess);

		fColor = ambient * Ia + diff * Id + spec * Is;
		if (isTexture == 1) {
			if (objectType == 0) fColor = fColor * texture( earTexture, texCoord ).rgba;
			else if (objectType == 1) fColor = fColor * texture( faceTexture, texCoord ).rgba;
			else if (objectType == 2) fColor = fColor * texture( bodyTexture, texCoord ).rgba;
			else if (objectType == 3) fColor = fColor * texture( bagTexture, texCoord ).rgba;
			else if (objectType == 4) fColor = fColor * texture( foreArmTexture, texCoord ).rgba;
			else if (objectType == 5) fColor = fColor * texture( armTexture, texCoord ).rgba;
			else if (objectType == 6) fColor = fColor * texture( upperLegTexture, texCoord ).rgba;
			else fColor = fColor * texture( lowerLegTexture, texCoord ).rgba;
		}
	}
} 

