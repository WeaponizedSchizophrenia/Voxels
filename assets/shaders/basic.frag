#version 460 core

in vec2 textureCoords;

out vec4 outColor;

uniform float brightness;

void main() {
    outColor = vec4(textureCoords, 0.0f, 1.0f) * brightness;
}