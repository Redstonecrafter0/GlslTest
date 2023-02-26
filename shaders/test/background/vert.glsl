#version 330 core

in vec2 aPos;

out vec2 fPos;

void main() {
    gl_Position = vec4(aPos, 0, 1);
    fPos = (aPos + 1) / 2;
}
