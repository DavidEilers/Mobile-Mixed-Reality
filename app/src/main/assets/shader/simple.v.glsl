#version 300 es
uniform highp mat4 mvp;

layout (location=0) in vec3 VertexPosition;

void main() {
    gl_Position = mvp*vec4(VertexPosition,1.0f);
    gl_PointSize=10.0f;
}