#version 330

//导入一个vertPos变量
in vec3 vertPos;

//导入变量
in vec3 texPos;

//输出一个texCoord变量
out vec2 texCoord;

void main()
{
    gl_Position = vec4(vertPos, 1.0);
    texCoord = texPos.xy;
}
