#version 460 core

in vec4 vertexColor;

out vec4 outColor;

uniform float brightness;

void main() {
    outColor = vertexColor * brightness;
}