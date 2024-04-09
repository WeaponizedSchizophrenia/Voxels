#version 460 core

layout(location = 0) in vec3 inPos;

out vec4 vertexColor;

void main() {
    gl_Position = vec4(inPos, 1.0f);
    vertexColor = vec4(1.0f);
}