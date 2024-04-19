#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoords;

out vec2 textureCoords;
out vec3 localPos;
out mat4 invModel;

uniform mat4 model;
uniform mat4 projectionView;

void main() {
    vec4 worldPos = model * vec4(inPos, 1.0f);
    localPos = inPos;
    gl_Position = projectionView * worldPos;
    textureCoords = inTexCoords;
    invModel = inverse(model);
}