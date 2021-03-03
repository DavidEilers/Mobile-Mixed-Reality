#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

layout(binding=0) uniform samplerExternalOES image;

in vec2 texCoord;

out vec4 color;

void main() {
    //color=vec4(0.0f,1.0f,1.0f,1.0f);
    color=texture(image,texCoord);
}
