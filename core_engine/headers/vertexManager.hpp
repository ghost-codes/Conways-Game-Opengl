//
//  vertexManager.hpp
//  ping-pong
//
//  Created by Nkatie3 on 09/12/2023.
//

#ifndef vertexManager_hpp
#define vertexManager_hpp

#include "pos.hpp"
#include <iostream>
class VertexManager {
public:
  // float vertices[288] = {};
  void init(int xDiv, int yDiv, int xPos, int yPos) {

    float topLeftx = ((2.0 / (xDiv)) * xPos) - 1;
    float topLefty = -((2.0 / (yDiv)) * yPos) + 1;

    float topRightx = ((2.0 / (xDiv)) * (xPos + 1)) - 1;
    float topRighty = topLefty;

    float bottomLeftx = topLeftx;
    float bottomLefty = -((2.0 / (yDiv)) * (yPos + 1)) + 1;

    float bottomRightx = topRightx;
    float bottomRighty = bottomLefty;
    vertices[0] = topRightx;
    vertices[1] = topRighty;
    vertices[2] = 0.0f;
    vertices[3] = bottomRightx;
    vertices[4] = bottomRighty;
    vertices[5] = 0.0f;
    vertices[6] = bottomLeftx;
    vertices[7] = bottomLefty;
    vertices[8] = 0.0f;
    vertices[9] = topLeftx;
    vertices[10] = topLefty;
    vertices[11] = 0.0f;

    position = Pos(xPos, yPos);

    // std::cout << "------------------------>" << std::endl;
    // std::cout << "topRight" << vertices[0] << vertices[1] << std::endl;
    // std::cout << "bottomRight" << vertices[3] << vertices[4] << std::endl;
    // std::cout << "bottomLeft" << vertices[6] << vertices[7] << std::endl;
    // std::cout << "topLeft" << vertices[9] << vertices[10] << std::endl;

    // std::cout << vertices[1];
    // vertices = {topRightx,    topRighty,    0.0f, // top right
    //             bottomRightx, bottomRighty, 0.0f, // bottom right
    //             bottomLeftx,  bottomLefty,  0.0f, // bottom left
    //             topLeftx,     topLefty,     0.0f};
  };
  float vertices[12] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  unsigned int texture;
  Pos position = Pos(0, 0);
  unsigned int indices[6] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  void logCurrentVertexData() {
    std::cout << "------------------------>" << std::endl;
    std::cout << "topRight" << vertices[0] << vertices[1] << std::endl;
    std::cout << "bottomRight" << vertices[3] << vertices[4] << std::endl;
    std::cout << "bottomLeft" << vertices[6] << vertices[7] << std::endl;
    std::cout << "topLeft" << vertices[9] << vertices[10] << std::endl;
  }

  unsigned int VBO, VAO, EBO;
  void createAndBindBuffers();
  void drawTriangle();
  void deleteShaders();
  int initTextures(char *path);
  unsigned int fragmentShader, vertexShader;
  unsigned int shaderProgram;
};

#endif /* vertexManager_hpp */
