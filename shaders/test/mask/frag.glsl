#version 330 core

in vec2 fPos;

out vec4 color;

uniform sampler2D uMask;
uniform sampler2D uTexture;
uniform sampler2D uBackground;

void main() {
    vec4 mask = texture2D(uMask, fPos);
    color = (texture2D(uBackground, fPos) * (1 - mask.a)) + (texture2D(uTexture, fPos) * mask);
}
