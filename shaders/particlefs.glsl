#version 330 core

in vec2 tCoord;
in vec4 pCol;
out vec4 fcol;

uniform sampler2D image;

void main(){
    fcol = texture(image, tCoord) * pCol;
}