#version 300 es

layout(binding=0) uniform sampler2D image;

in vec2 texCoord;

out vec4 color;

void main() {
    color=vec4(0.0f,1.0f,1.0f,1.0f);//texture2D(image,texCoord);
}
