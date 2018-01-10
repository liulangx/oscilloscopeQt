#version 430 core
//in layout(location=0) vec3 Position;
in layout(location=0) vec3 Position;
//in layout(location=1) vec4 Color;
//in vec4 Color;
in layout(location=1) vec2 textureCoords;

uniform mat4 _rot;
uniform mat4 _projection;
uniform mat4 _move;
uniform int _drawtype;
uniform vec2 _rangeX;  //X range
uniform vec2 _rangeY;  //Y range
uniform vec2 _rangeZ;  //Z range

//out vec4 outcolor;
out vec2 pass_texture;
void main(void)
{
    vec4 _tmpposition;
    mat4 _rightHandChange = mat4(
               1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, -1.0f, 0.0f,
               0.0f,0.0f,0.0f,1.0f
           );      //转为右手坐标系
    float X = _rangeX.x-_rangeX.y;
    float Y = _rangeY.x-_rangeY.y;
    float Z = _rangeZ.x-_rangeZ.y;
//    switch(_drawtype)
//    {
//      case 0:   //xyz
//      {
//        _tmpposition = vec4(Position.x, Position.y, Position.z, 1);
//        //_tmpposition -=vec4(basecord,0);
//        gl_Position = /*_rightHandChange **/ _projection * _move * _rot * _tmpposition;
//        break;
//      }
//      case 1:   //xy
//      {
//        _tmpposition = vec4(Position.x, Position.y, Position.z, 1);
//        //_tmpposition -=vec4(basecord,0);
//        gl_Position = /*_rightHandChange **/ _projection * _move * _tmpposition;
//        break;
//      }
//      case 2:   //xz
//      {
//        _tmpposition = vec4(Position.x, Position.y, Position.z - 0.1*Y, 1);
//        //_tmpposition -=vec4(basecord,0);
//        gl_Position = /*_rightHandChange **/ _projection * _move * _tmpposition;
//        break;
//      }
//      case 3:   //yz
//      {
//        _tmpposition = vec4(Position.x, Position.y - 0.1*Z, Position.z, 1);
//        //_tmpposition -=vec4(basecord,0);
//        gl_Position = /*_rightHandChange **/ _projection * _move * _tmpposition;
//        break;
//      }
//    }
    vec4 _tmpsiztion1;
    if(0 == _drawtype)
    {
        _tmpsiztion1 = _move * _rot * vec4(Position, 1);
        _tmpsiztion1.z -= 15;
        gl_Position = /*_rightHandChange **/ _projection * _tmpsiztion1;
    }
    else
    {
        _tmpsiztion1 = _move * _rot * vec4(Position, 1);
        gl_Position = /*_rightHandChange **/ _projection * _tmpsiztion1;
    }
    pass_texture = textureCoords;

};
