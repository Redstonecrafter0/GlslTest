#version 330 core

in vec2 fPos;

out vec4 color;

uniform float uColor;
uniform sampler2D uMask;
uniform sampler2D uTexture;

void main() {
    if (texture2D(uMask, fPos).a >= 0.5) {
        color = texture2D(uTexture, fPos);
    } else {
        color = vec4(fPos, uColor, 1);
    }
}
