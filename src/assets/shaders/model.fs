#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 colorMultiplier;
uniform vec3 maskedColor;
uniform vec3 maskColor;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (abs(texColor.r - maskedColor.r) + abs(texColor.g - maskedColor.g) + abs(texColor.b - maskedColor.b) < 0.001)
        {
            FragColor = vec4(maskColor, texColor.a);
        }
        else
    {
        FragColor = texColor;
    }
}