#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

out vec4 vertexColor;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(inPos, 1.0f);
    vertexColor = vec4(inColor, 1.0f);
}