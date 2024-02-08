#include "Geometry.h"
#include <cstring>


ShapeData ShapeGenerator::makeTriangle()
{
  ShapeData vTriangleShapeData;

  Vertex vVertices[] =
  {
    //front of triangle
    {-1.0F, -1.0F, 0.0F,      1.0F, 0.0F, 0.0F, 1.0F,     0.0F, 0.0F},
    { 0.0F,  1.0F, 0.0F,      0.0F, 1.0F, 0.0F, 1.0F,     0.5F, 1.0F},
    { 1.0F, -1.0F, 0.0F,      0.0F, 0.0F, 1.0F, 1.0F,     1.0F, 0.0F},

    //rear of triangle
    {-1.0F, -1.0F, 0.0F,      1.0F, 0.0F, 0.0F, 1.0F,     0.0F, 0.0F},
    { 0.0F,  1.0F, 0.0F,      0.0F, 1.0F, 0.0F, 1.0F,     0.5F, 1.0F},
    { 1.0F, -1.0F, 0.0F,      0.0F, 0.0F, 1.0F, 1.0F,     1.0F, 0.0F}
  };

  int vIndices[] = { 0, 1, 2, 3, 4, 5 };

  vTriangleShapeData.sizeVertices = sizeof(vVertices);
  vTriangleShapeData.sizeIndices  = sizeof(vIndices);

  vTriangleShapeData.numVertices = sizeof(vVertices) / sizeof(Vertex);
  vTriangleShapeData.numIndices  = sizeof(vVertices) / sizeof(Vertex);

  vTriangleShapeData.vertices    = new Vertex[vTriangleShapeData.numVertices];
  vTriangleShapeData.indices     = new int[vTriangleShapeData.numIndices];

  memcpy(vTriangleShapeData.vertices, vVertices, vTriangleShapeData.sizeVertices);
  memcpy(vTriangleShapeData.indices, vIndices, sizeof(int) * vTriangleShapeData.numIndices);

  return vTriangleShapeData;
}


ShapeData ShapeGenerator::makeCube()
{
  ShapeData vCubeShapeData;
  
  Vertex* vVertices = new Vertex[36]
  {
    //front face
    {1.0F, 1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,     0.0F, 0.0F, 1.0F}, //0
    {-1.0F, 1.0F, 1.0F,     0.0F, 1.0F, 0.0F, 1.0F,       0.0F, 1.0F,     0.0F, 0.0F, 1.0F}, //1
    {-1.0F, -1.0F, 1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       0.0F, 0.0F,     0.0F, 0.0F, 1.0F}, //2
    {1.0F, 1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,     0.0F, 0.0F, 1.0F}, //0
    {-1.0F, -1.0F, 1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       0.0F, 0.0F,     0.0F, 0.0F, 1.0F}, //2
    {1.0F, -1.0F, 1.0F,     1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 0.0F,     0.0F, 0.0F, 1.0F}, //3

    //back face
    {-1.0F, 1.0F, -1.0F,    1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,     0.0F, 0.0F, -1.0F}, //4
    {1.0F, 1.0F, -1.0F,     0.0F, 1.0F, 0.0F, 1.0F,       0.0F, 1.0F,     0.0F, 0.0F, -1.0F}, //5
    {1.0F, -1.0F, -1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       0.0F, 0.0F,     0.0F, 0.0F, -1.0F}, //6
    {-1.0F, 1.0F, -1.0F,    1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,     0.0F, 0.0F, -1.0F},
    {1.0F, -1.0F, -1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       0.0F, 0.0F,     0.0F, 0.0F, -1.0F},
    {-1.0f, -1.0F, -1.0F,   1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 0.0F,     0.0F, 0.0F, -1.0F}, //7

    //right face
    {1.0F, 1.0F, -1.0F,     1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,     1.0F, 0.0F, 0.0F},
    {1.0F, 1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 1.0F,     1.0F, 0.0F, 0.0F},
    {1.0F, -1.0F, 1.0F,     1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 0.0F,     1.0F, 0.0F, 0.0F},
    {1.0F, 1.0F, -1.0F,     1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,     1.0F, 0.0F, 0.0F},
    {1.0F, -1.0F, 1.0F,     1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 0.0F,     1.0F, 0.0F, 0.0F},
    {1.0F, -1.0F, -1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       1.0F, 0.0F,     1.0F, 0.0F, 0.0F},

    //left face
    {-1.0F, 1.0F, 1.0F,     0.0F, 1.0F, 0.0F, 1.0F,       1.0F, 1.0F,     -1.0F, 0.0F, 0.0F},
    {-1.0F, 1.0F, -1.0F,    1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 1.0F,     -1.0F, 0.0F, 0.0F},
    {-1.0f, -1.0F, -1.0F,   1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 0.0F,     -1.0F, 0.0F, 0.0F},
    {-1.0F, 1.0F, 1.0F,     0.0F, 1.0F, 0.0F, 1.0F,       1.0F, 1.0F,     -1.0F, 0.0F, 0.0F},
    {-1.0f, -1.0F, -1.0F,   1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 0.0F,     -1.0F, 0.0F, 0.0F},
    {-1.0F, -1.0F, 1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       1.0F, 0.0F,     -1.0F, 0.0F, 0.0F},

    //top face
    {-1.0F, 1.0F, -1.0F,    1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,      0.0F, 1.0F, 0.0F},
    {-1.0F, 1.0F, 1.0F,     0.0F, 1.0F, 0.0F, 1.0F,       0.0F, 1.0F,      0.0F, 1.0F, 0.0F},
    {1.0F, 1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 0.0F,      0.0F, 1.0F, 0.0F},
    {-1.0F, 1.0F, -1.0F,    1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 1.0F,      0.0F, 1.0F, 0.0F},
    {1.0F, 1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 0.0F,      0.0F, 1.0F, 0.0F},
    {1.0F, 1.0F, -1.0F,     0.0F, 1.0F, 0.0F, 1.0F,       1.0F, 0.0F,      0.0F, 1.0F, 0.0F},

    //bottom face
    {1.0F, -1.0F, -1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       1.0F, 1.0F,      0.0F, -1.0F, 0.0F},
    {1.0F, -1.0F, 1.0F,     1.0F, 0.0F, 0.0F, 1.0F,       0.0F, 1.0F,      0.0F, -1.0F, 0.0F},
    {-1.0F, -1.0F, 1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       0.0F, 0.0F,      0.0F, -1.0F, 0.0F},
    {1.0F, -1.0F, -1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       1.0F, 1.0F,      0.0F, -1.0F, 0.0F},
    {-1.0F, -1.0F, 1.0F,    0.0F, 0.0F, 1.0F, 1.0F,       0.0F, 0.0F,      0.0F, -1.0F, 0.0F},
    {-1.0f, -1.0F, -1.0F,   1.0F, 0.0F, 0.0F, 1.0F,       1.0F, 0.0F,      0.0F, -1.0F, 0.0F}
  };

  int* vIndices = new int[36]
  {
    //front face
    0,1,2, 0,2,3,

    //right face
    5,0,3,  5,3,6,

    //left face
    1,4,7,  1,7,2,

    //top face
    4,1,0,  4,0,5,

    //bottom face
    6,3,2,  6,2,7,

    //rear face
    4,5,6,  4,6,7
  };

  vCubeShapeData.sizeVertices = sizeof(Vertex) * 36;
  vCubeShapeData.sizeIndices  = sizeof(int) * 36;

  vCubeShapeData.numVertices = 24;
  vCubeShapeData.numIndices  = 36;

  /*
  vCubeShapeData.vertices = new Vertex[vCubeShapeData.numVertices];
  vCubeShapeData.indices  = new int[vCubeShapeData.numIndices];

  memcpy(vCubeShapeData.vertices, vVertices, vCubeShapeData.sizeVertices);
  memcpy(vCubeShapeData.indices,  vIndices,  vCubeShapeData.sizeIndices);
  */

  vCubeShapeData.vertices = vVertices;
  vCubeShapeData.indices  = vIndices;

  return vCubeShapeData;
}


ShapeData ShapeGenerator::makeArrow()
{
  ShapeData vArrowShapeData;

  Vertex* vVertices = new Vertex[10]
  {
    //front face
  {-1.0F, -0.5F, 0.5F,        1.0F, 0.0F, 0.0F, 1.0F,      0.0F, 0.0F},   //0
  {-1.0F,  0.5F, 0.5F,        0.0F, 1.0F, 0.0F, 1.0F,      0.0F, 1.0F},   //1
  {1.0F,  0.5F, 0.5F,        0.0F, 0.0F, 1.0F, 1.0F,      1.0F, 1.0F},    //2
  {1.0F, -0.5F, 0.5F,        0.5F, 0.1F, 0.0F, 1.0F,      1.0F, 0.0F},   //3

     //rear face
  {1.0F, -0.5F, -0.5F,      1.0F, 0.0F, 0.0F, 1.0F,      0.0F, 0.0F},  //4
  {1.0F,  0.5F, -0.5F,      0.0F, 1.0F, 0.0F, 1.0F,      0.0F, 1.0F},  //5
  {-1.0F,  0.5F, -0.5F,      0.0F, 0.0F, 1.0F, 1.0F,      1.0F, 1.0F},  //6
  {-1.0F, -0.5F, -0.5F,      0.0F, 0.8F, 1.0F, 1.0F,      0.0F, 1.0F},   //7
  
  //triangle
  {2.0F, 0.5F,  0.0F,        1.0F, 0.0F, 0.0F, 1.0F,      0.0F, 0.0F}, //8
  {2.0F, -0.5F, 0.0F,        1.0F, 0.0F, 0.0F, 1.0F,      0.0F, 0.0F}, //9
  };

  int* vIndices = new int[54]
  {
    //front face
    0,1,2, 0,2,3,

    //right face
    3,2,5, 3,5,4,

    //left face
    7,6,1, 7,1,0,

    //top face
    2,1,6, 2,6,5,

    //bottom face
    0,3,4, 0,4,7,

    //rear face
    4,5,6, 4,6,7,

    //triangle top
    2,5,8,
    //triangle bottom
    3,4,9,
    //triangle face 1
    3,2,8,    3,8,9,
    //triangle face 2
    8,5,4,    8,4,9
  };

  vArrowShapeData.sizeVertices = sizeof(Vertex) * 10;
  vArrowShapeData.sizeIndices = sizeof(int) * 54;

  vArrowShapeData.numVertices = 10;
  vArrowShapeData.numIndices = 54;

  vArrowShapeData.vertices = vVertices;
  vArrowShapeData.indices = vIndices;

  return vArrowShapeData;
}