#version 330 core

in vec2 tCoord;

uniform sampler2D image;
uniform vec3 color;

out vec4 fcol;

void main(){
    fcol = vec4(color, 1.0) * texture(image, tCoord);
    // fcol = vec4(1.0, 0.0, 0.0, 1.0);
    // fcol = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}