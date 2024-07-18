#version 330 core
layout (location = 0) in vec4 vertex;

/*
this is the shader
that the quad renderer will use
the game is already rendered on another fbo
but some cool post-processing stuff can be
done here
*/

out vec2 tCoord;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main(){
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
    vec2 texture = vertex.zw;
    if (chaos){
        float strength = 0.3f;
        // periodically sample colors from around the actual coord
        /*
        the texture wrapping methods really matter here!!
        clamp to edge would look really weird, GL_REPEAT is actually 
        very useful here

        chaning which coord is sin and cos here only affects
        the oreintation of the rotation (clockwise - counterclock wise)
        */
        vec2 pos = vec2(texture.x + sin(time) * strength, 
                        texture.y + cos(time) * strength);
        tCoord = pos;
    }

    else if (confuse){
        //invert x and y axis
        tCoord = vec2(1.0 - texture.x, 1.0 - texture.y);
    }
    else{
        tCoord = texture;
    }

    if (shake){
        //just move periodically
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}