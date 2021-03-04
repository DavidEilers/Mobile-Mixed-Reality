#version 300 es

uniform mat4 mvp;
uniform mat4 normalMatrix;

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TextureCoords;


out vec2 texCoords;
out vec3 normals;
void main() {
    const vec4 vertices[3]
    = vec4[3](  vec4(0.25,-0.25,0.5,1.0f),
                vec4(-0.25,-0.25,0.5,1.0f),
                vec4(0.25,0.25,0.5,1.0f));

    texCoords=TextureCoords;
    normals = VertexNormal;//(normalMatrix*vec4(VertexNormal,1.0f)).xyz;
    mat4 test = mat4(0.1);
    test[3][3]=1.0f;

    vec4 pos = vec4(0.25*VertexPosition.xy,0.5f,1.0f);
//    if(gl_VertexID<3){
//        gl_Position = vertices[gl_VertexID];
//    }else{
//        gl_Position=vec4(0.0f);
//    }
    gl_Position = pos;
}
