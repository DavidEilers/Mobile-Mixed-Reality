#version 300 es

in mediump vec3 texCoords;

out lowp vec4 color;

void main() {
    //set the color of the grid
    vec3 gridColor = vec3(0.8f,0.0f,0.8f); // magenta highly visible

    //Factor from 0.0 to sqrt(2) 0.0f being the middle sqrt(2) being the corners of the quad
    float distanceFromMid = length(texCoords-vec3(0.5f,0.5f,0.5f))*2.0f;

    float midDistanceFactor = 1.8f-distanceFromMid;


    //add horizontal lines
    float gridIntensity = step(0.9,cos(100.0f*texCoords.x))+0.2f;
    //add vertical lines
    gridIntensity += step(0.9,cos(100.0f*texCoords.y))+0.2f;
    gridIntensity = clamp(gridIntensity,0.0f,1.0f);
    //make grid more transparent at the edges
    gridIntensity *= midDistanceFactor;

    //set output to Color
    color = vec4(gridIntensity*gridColor,0.5f);
    //color = vec4(0.0f,1.0f,0.0f,0.3f);
}
