#version 300 es
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 view;
//in vec4 pos;
in vec2 texCoords;
in vec3 normals;
in vec3 viewPos;

const vec3 light_dir= vec3(-1.0f,-1.0f,0f);

out vec4 color;
void main() {
    color=vec4(1.0f,0.0f,0.0f,1.0f);
    vec3 viewLightDir= (view*vec4(light_dir,1.0f)).xyz;
    viewLightDir = normalize(-viewLightDir);
    color= dot(viewLightDir,normalize(normals))*color;
}