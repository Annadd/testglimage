#version 330

in vec2 texCoord;

out mediump vec4 color;

uniform sampler2D img;

void main()
{
    color = texture2D(img, texCoord);
}
