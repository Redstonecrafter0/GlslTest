#version 330 core

in vec2 fPos;

out vec4 color;

uniform vec2 uTexOffset;
uniform sampler2D uTexture;

void main() {
//    if (fPos.y >= 0.5) {
        color = texture2D(uTexture, fPos);
//    } else {
//        color = texture2D(uTexture, fPos + uTexOffset) / 4
//        + texture2D(uTexture, fPos - uTexOffset) / 4
//        + texture2D(uTexture, fPos + vec2(uTexOffset.x, -uTexOffset.y)) / 4
//        + texture2D(uTexture, fPos + vec2(-uTexOffset.x, uTexOffset.y)) / 4;
//    }
}
