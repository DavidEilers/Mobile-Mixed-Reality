#version 300 es
uniform highp mat4 mvp;

layout (location=0) in vec3 VertexPosition;


out mediump vec3 texCoords;
void main() {

    int vertexNum = gl_VertexID%3;
    if(vertexNum==0){
        texCoords = vec3(1.0f,0.0f,0.0f);
    } else if (vertexNum==1){
        texCoords = vec3(0.0f,1.0f,0.0f);
    }else{
        texCoords = vec3(0.0f,0.0f,1.0f);
    }

    highp vec4 pos = mvp*vec4(VertexPosition,1.0f);
    gl_Position = pos;
    //gl_PointSize=10.0f;
}
