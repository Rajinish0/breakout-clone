#version 330 core

layout (location = 0) in vec4 vertex; 

out vec2 tCoord;

uniform mat4 proj;
uniform mat4 model;

void main(){
    gl_Position = proj*model*vec4(vertex.xy, 0.0, 1.0);
    // gl_Position = vec4(vertex.xy, 0.0, 1.0);
    tCoord = vertex.zw;
}