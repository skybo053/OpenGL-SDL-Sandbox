#include <fstream>
#include <sstream>
#include <exception>
#include <vector>


std::string getShader(const char* pFileName)
{
  std::ifstream     vInput(pFileName);
  std::stringstream vStringStream;
  
  vStringStream << vInput.rdbuf();

  return vStringStream.str();
}


unsigned char* loadImage(const char* pImageFileName)
{
  unsigned char* vBuffer = nullptr;
  std::ifstream  vImageFile(pImageFileName, std::ifstream::binary);

  if(!vImageFile) return NULL;

  vImageFile.seekg(0, std::ifstream::end);
  int len = vImageFile.tellg();
  vImageFile.seekg(0);

  vBuffer = new unsigned char[len];

  vImageFile.read((char*)vBuffer, len);

  return vBuffer;
}