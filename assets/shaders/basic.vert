#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoords;

out vec2 textureCoords;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(inPos, 1.0f);
    textureCoords = inTexCoords;
}