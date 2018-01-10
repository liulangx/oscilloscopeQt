#version 420 core

in vec4 outcolor;

void main(void)
{
    gl_FragColor = outcolor;
};

//#version 420 core

//in layout(location=1) vec4 Color;

////in vec4 outcolor;
////in vec4 finalcolor;

////layout(location = 1) out vec4 Color;
////out vec4 out_color;
//uniform int chose;
//uniform vec4 lineColor;
////out vec4 out_color;

//void main(void)
//{
//    if(chose == 1)
//        gl_FragColor = lineColor;
//    else if(chose == 0)
//        gl_FragColor = Color;
////    Color = finalcolor;

//};
