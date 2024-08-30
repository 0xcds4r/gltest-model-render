#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords[2];  // ћассив текстурных координат (два набора)
layout(location = 2) in vec3 aColor;

out vec2 TexCoords[2];  // ћассив выходных текстурных координат
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords[0] = aTexCoords[0];
    TexCoords[1] = aTexCoords[1];
    ourColor = aColor;
}
