#version 420 core
layout(lines) in ;
layout(line_strip, max_vertices = 2) out;

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

uniform bool xyPlaneMark;
uniform bool xzPlaneMark;
uniform bool yzPlaneMark;

uniform float xLower;
uniform float yLower;
uniform float zLower;

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

      float X = rangeX.x-rangeX.y;
      float Y = rangeY.x-rangeY.y;
      float Z = rangeZ.x-rangeZ.y;
      vec3 Position;
      if(xyPlaneMark)
      {
        Position = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, zLower);
      }
      else if(xzPlaneMark)
      {
        Position = vec3(gl_in[0].gl_Position.x, yLower, gl_in[0].gl_Position.z);
      }
      else if(yzPlaneMark)
      {
        Position = vec3(xLower, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z);
      }
      else {
        Position = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z);
      }
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

    vec4 _tmpPosition1;
    if(drawtype == 0)
    {
        _tmpPosition1 = move * rot * _tmpposition;
        _tmpPosition1.z -= 15;
        gl_Position = /*_rightHandChange **/ projection * _tmpPosition1;
    }
    else
    {
        _tmpPosition1 = move * rot * _tmpposition;
        gl_Position = /*_rightHandChange */ projection * _tmpPosition1;
    }

    EmitVertex();

    _rightHandChange = mat4(
               1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, -1.0f, 0.0f,
               0.0f,0.0f,0.0f,1.0f
           );      //转为右手坐标系

        if(xyPlaneMark)
        {
          Position = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, zLower);
        }
        else if(xzPlaneMark)
        {
          Position = vec3(gl_in[1].gl_Position.x, yLower, gl_in[1].gl_Position.z);
        }
        else if(yzPlaneMark)
        {
          Position = vec3(xLower, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z);
        }
        else {
          Position = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z);
        }

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
      if(drawtype == 0)
      {
          _tmpPosition1 = move * rot * _tmpposition;
          _tmpPosition1.z -= 15;
          gl_Position = /*_rightHandChange **/ projection * _tmpPosition1;
      }
      else
      {
          _tmpPosition1 = move * rot * _tmpposition;
          gl_Position = /*_rightHandChange */ projection * _tmpPosition1;
      }


    EmitVertex();

    EndPrimitive();
}
