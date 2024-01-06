//
//  windowManager.hpp
//  ping-pong
//
//  Created by Nkatie3 on 09/12/2023.
//

#ifndef windowManager_hpp
#define windowManager_hpp

#include <GLFW/glfw3.h>

class GLFWManager {
public:
  void initializeGLFW();
  GLFWwindow *createWindow(int windowHeight, int windowWidth);
  void swapAndPool(GLFWwindow *window);
};

#endif /* windowManager_hpp */
