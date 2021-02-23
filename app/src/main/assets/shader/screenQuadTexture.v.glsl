#version 300 es

layout(location=0) in vec2 pos;
layout(location=1) in vec2 texCoord_in;

out vec2 texCoord;

void main(){
    gl_Position=vec4(pos.x,pos.y,0.0f,1.0f);
    texCoord=texCoord_in;
}