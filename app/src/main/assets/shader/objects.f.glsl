#version 300 es
uniform highp mat4 mvp;
uniform highp mat4 mv;
uniform highp mat4 view;
uniform highp vec4 baseColor;
//in vec4 pos;
in mediump vec2 texCoords;
in lowp vec3 normals;
in highp vec3 viewPos;

const lowp vec3 light_dir= vec3(-1.0f,-1.0f,0.0f);

out lowp vec4 color;
void main() {
    color=baseColor;//vec4(1.0f,0.0f,0.0f,1.0f);
    lowp vec3 viewLightDir= (view*vec4(light_dir,1.0f)).xyz;
    viewLightDir = normalize(-viewLightDir);
    color= dot(viewLightDir,normalize(normals))*color;
}