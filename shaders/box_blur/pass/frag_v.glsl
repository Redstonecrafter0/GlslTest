#version 330 core

in vec2 fPos;

out vec4 color;

uniform sampler2D uTexture;
uniform int uRadius;
uniform float uTexelSize;

void main() {
    color = vec4(0);
    int size = uRadius * 2 + 1;
    for (int i = -uRadius; i <= uRadius; i++) {
        color += texture2D(uTexture, fPos + vec2(0, uTexelSize * uRadius * i)) / size;
    }
}
