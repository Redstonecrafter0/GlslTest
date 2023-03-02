#version 330 core

in vec2 fPos;

out vec4 color;

uniform sampler2D uTexture;

void main() {
    color = texture2D(uTexture, fPos);
}
