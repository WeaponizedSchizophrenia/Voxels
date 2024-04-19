#version 460 core

in vec2 textureCoords;
in vec3 localPos;
in mat4 invModel;

out vec4 outColor;

uniform vec3 cameraWorldPos;
uniform usampler2D uTexture;

#define MAX_STEPS 256
#define CHUNK_SIZE 32
#define MINIMUM_DISTANCE 1.0f / (CHUNK_SIZE * 2.0f)
#define SUN_POSITION vec3(0, 3, 5)
#define EPSILON 0.1f

float getDistance(vec3 position);
vec4 rayMarch(vec3 position, vec3 direction);

void main() {

    vec3 cameraPos = (invModel * vec4(cameraWorldPos, 1.0f)).xyz;
    vec3 viewDir = normalize(localPos - cameraPos);

    vec4 color = vec4(1.0f);

    vec4 rayResult = rayMarch(cameraPos, viewDir);
    if(!bool(rayResult.w))
        discard;

    outColor = color;
}

float getDistance(vec3 position) {
    vec3 coord = (position + 1.0f) / 2;
    uint sampled = texture(uTexture, coord.xy).r;
    uint current = sampled & (1u << uint(coord.z * CHUNK_SIZE));
    if(current == 0) {
        return 1.0f / CHUNK_SIZE;
    } else {
        return 0;
    }
}

/// This returns a vec4 of xyz - position and w - if it has hit something.
vec4 rayMarch(vec3 position, vec3 direction) {
    for(uint i = 0; i < MAX_STEPS; ++i) {
        float distance = getDistance(position);
        if(distance <= MINIMUM_DISTANCE) {
            return vec4(position, 1);
        }
        position += direction * distance;
    }
    return vec4(0);
}