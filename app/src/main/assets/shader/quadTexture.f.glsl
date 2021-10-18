#version 300 es

uniform sampler2D image;
in mediump vec2 texCoord;

out lowp vec4 color;

void main() {
    //color=vec4(0.0f,1.0f,1.0f,1.0f);
    vec2 helpCoord = vec2(texCoord.x,1.0f-texCoord.y);


    vec4 texColor = texture(image,helpCoord);
    if(texColor.a<0.1){
        discard;
    }
    color=texColor;
}
