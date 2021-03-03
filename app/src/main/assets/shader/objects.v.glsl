#version 300 es

uniform mat4 mvp;
uniform mat4 normalMatrix;

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TextureCoords;

out vec2 texCoords;
out vec3 normal;
void main() {
    texCoords=TextureCoords;
    normal = (normalMatrix*vec4(VertexNormal,1.0f)).xyz;
    mat4 test = mat4(0.1);
    test[3][3]=1.0f;
    gl_Position = test*vec4(VertexPosition,1.0f);


}
