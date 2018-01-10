#version 430 core

//in vec4 outcolor;
in vec2 pass_texture;

out vec4 out_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main(void)
{
        //gl_FragColor = outcolor;
    vec4 textureColor = texture(texture0, pass_texture);
    if(textureColor.a < 0.5)
    {
        discard;
    }
    out_color = textureColor;
    //out_color = mix(texture(texture1, pass_texture),texture(texture0,pass_texture),0);
    //out_color = outcolor;
};
