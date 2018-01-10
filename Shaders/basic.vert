#version 420 core
in layout(location=0) vec3 Position;
in layout(location=1) vec4 Color;

uniform int chose;
uniform vec4 lineColor;
uniform int drawtype;
uniform float scale;  //zoom
uniform mat4 rot;     //view matrix
uniform vec2 rangeX;  //X range
uniform vec2 rangeY;  //Y range
uniform vec2 rangeZ;  //Z range
uniform mat4 move;    //移动
uniform vec3 basecord;  //基准坐标
uniform mat4 projection;
uniform float viewScale;

out vec4 outcolor;
void main()
{
    vec4 _tmpposition;
    mat4 _rightHandChange = mat4(
               1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, -1.0f, 0.0f,
               0.0f,0.0f,0.0f,1.0f
           );      //转为右手坐标系
    //mat4 _tmpRot;

    if(1==chose)      //point
    {
      outcolor = lineColor;
      float X = rangeX.x-rangeX.y;
      float Y = rangeY.x-rangeY.y;
      float Z = rangeZ.x-rangeZ.y;
      switch(drawtype)
      {
        case 0:   //xyz
        {
          _tmpposition = vec4(viewScale * (Position.x-rangeX.y)/X*scale,viewScale * (Position.y-rangeY.y)/Y*scale, viewScale * (Position.z-rangeZ.y)/Z*scale,1);
          _tmpposition -=vec4(basecord,0);
          break;
        }
        case 1:   //xy
        {
          _tmpposition = vec4(viewScale * (Position.x-rangeX.y)/X*scale*1.8,viewScale * (Position.y-rangeY.y)/Y*scale*1.8, 0,1);
          _tmpposition -=vec4(basecord,0);
          break;
        }
        case 2:   //xz
        {
          _tmpposition = vec4(viewScale * (Position.x-rangeX.y)/X*scale*1.8,0, viewScale * (Position.z-rangeZ.y)/Z*scale*1.8,1);
          _tmpposition -=vec4(basecord,0);

          break;
        }
        case 3:   //yz
        {
          _tmpposition = vec4(0,viewScale * (Position.y-rangeY.y)/Y*scale*1.8, viewScale * (Position.z-rangeZ.y)/Z*scale*1.8,1);
          _tmpposition -=vec4(basecord,0);

          break;
        }
      }
    }
    if(0==chose)
    {
        _tmpposition = vec4(Position.x, Position.y, Position.z, 1);
        outcolor = Color;
    }
    vec4 _tmpsiztion1;
    if(drawtype == 0)
    {
        _tmpsiztion1 = move * rot * _tmpposition;
        _tmpsiztion1.z -= 15;
        gl_Position = /*_rightHandChange **/ projection * _tmpsiztion1;
    }
    else
    {
        _tmpsiztion1 = move * rot * _tmpposition;
        gl_Position = /*_rightHandChange **/ projection * _tmpsiztion1;
    }


    //gl_Position = move * rot * _tmpposition;
};
