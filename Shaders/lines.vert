#version 420 core
in layout(location=0) vec3 Position;

void main()
{
    gl_Position = vec4(Position.xyz, 1);
};
