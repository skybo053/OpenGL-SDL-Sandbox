#ifndef GEOMETRY_H
#define GEOMETRY_H


struct Vertex
{
  Vertex() {}

  Vertex(
    float pX, float pY, float pZ,
    float cR, float cG, float cB, float cA,
    float tU, float tV,
    float nI = 0, float nJ = 0, float nK = 0) :
    position{ pX, pY, pZ },
    color{ cR, cG, cB, cA },
    texture{ tU, tV },
    normal{ nI, nJ, nK }
  {
  }

  struct
  {
    float x, y, z;
  } position;

  struct
  {
    float r, g, b, a;
  } color;

  struct
  {
    float u, v;
  } texture;

  struct
  {
    float i, j, k;
  } normal;
};


struct ShapeData
{
  Vertex* vertices;
  int*    indices;

  size_t sizeVertices;
  size_t sizeIndices;

  size_t numVertices;
  size_t numIndices;
};


class ShapeGenerator
{
public:
  ShapeGenerator() = default;
  ShapeGenerator(const ShapeGenerator&) = delete;
  ShapeGenerator operator=(const ShapeGenerator&) = delete;

  static ShapeData makeTriangle();
  static ShapeData makeCube();
  static ShapeData makeArrow();

private:
};


#endif
