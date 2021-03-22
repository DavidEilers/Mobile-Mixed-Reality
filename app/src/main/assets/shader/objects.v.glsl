#version 300 es
uniform mat4 mvp;
uniform mat4 mv;

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TextureCoords;


out vec2 texCoords;
out vec3 normals;
out vec3 viewPos;
void main() {
    mat4 normalMatrix = inverse(transpose(mv));

    texCoords=TextureCoords;
    normals = (normalMatrix*vec4(VertexNormal,1.0f)).xyz;
    viewPos = (mv*vec4(VertexPosition,1.0f)).xyz;


    vec4 pos = mvp*vec4(VertexPosition,1.0f);
//    if(gl_VertexID<3){
//        gl_Position = vertices[gl_VertexID];
//    }else{
//        gl_Position=vec4(0.0f);
//    }
    gl_Position = pos;
}
