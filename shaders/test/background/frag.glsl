#version 330 core

in vec2 fPos;

out vec4 color;

uniform float uColor;

void main() {
    color = vec4(fPos, uColor, 1);
}
