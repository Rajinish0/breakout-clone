#version 330 core

in vec2 tCoord;
out vec4 fcol;

uniform sampler2D scene;
uniform vec2 offsets[9]; // for the textures
uniform int edgeKernel[9];
uniform float blurKernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main(){
    fcol = vec4(0.0f);
    vec3 samples[9];

    if (chaos || shake)
        for (int i =0; i < 9; ++i)
            samples[i] = vec3(texture(scene, tCoord + offsets[i]));
    
    if (chaos){
        for (int i =0; i < 9; ++i)
            fcol += vec4(samples[i] * edgeKernel[i], 0.0f);
        fcol.a = 1.0f;
    }
    else if (shake){
        for (int i =0; i < 9; ++i)
            fcol += vec4(samples[i] * blurKernel[i], 0.0f);
        fcol.a = 1.0f;
    }
    else if (confuse){
        //also invert the colors
        fcol = vec4(1.0 - texture(scene, tCoord).rgb, 1.0);
    }
    else {
        fcol = texture(scene, tCoord);
    }

}