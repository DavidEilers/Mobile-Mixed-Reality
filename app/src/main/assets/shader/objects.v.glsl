#version 300 es
uniform highp mat4 mvp;
uniform highp mat4 mv;

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TextureCoords;


out mediump vec2 texCoords;
out lowp vec3 normals;
out highp vec3 viewPos;
void main() {
    highp mat4 normalMatrix = inverse(transpose(mv));

    texCoords=TextureCoords;
    normals = (normalMatrix*vec4(VertexNormal,1.0f)).xyz;
    viewPos = (mv*vec4(VertexPosition,1.0f)).xyz;


    highp vec4 pos = mvp*vec4(VertexPosition,1.0f);
//    if(gl_VertexID<3){
//        gl_Position = vertices[gl_VertexID];
//    }else{
//        gl_Position=vec4(0.0f);
//    }
    gl_Position = pos;
}
