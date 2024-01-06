#ifndef MODEL
#define MODEL

#include "mesh.hpp"
#include "shader.hpp"
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

class Model {
public:
  Model(char *path) { loadModel(path); }
  void Draw(Shader &shader);

private:
  // model data
  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Texture> textures_loaded;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialsTextures(aiMaterial *mat,
                                             aiTextureType type,
                                             std::string typeName);
};

#endif // !MODEL
