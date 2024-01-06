#include <algorithm>
#include <cmath>
#include <cstdio>
#include <glad/glad.h>
// Glad
#include "entity.hpp"
#include "neighbors.hpp"
#include "perlin.hpp"
#include "pos.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "state.hpp"
#include "vertexManager.hpp"
#include "windowManager.hpp"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <sstream>
#include <string>

using std::__fs::filesystem::absolute;

void processInput(GLFWwindow *window);
void populationSystem(Scene *scene);
///// -------------->       entity helpers

int timeDiff = 0;

int main() {
  GLFWManager windowManger = GLFWManager();
  Scene scene = Scene();
  windowManger.initializeGLFW();
  GLFWwindow *window = windowManger.createWindow(500, 500);

  if (window == NULL) {
    std::cout << "ERROR::Could not initialize window" << std::endl;
  }

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  Perlin perlin = Perlin(4, 4, 10, 5099);

  float xDiv = mode->width / 5.0;
  float yDiv = mode->height / 5.0;

  std::map<std::string, EntityID> positionMap = {};

  for (int i = 0; i < yDiv; i++) {
    for (int j = 0; j < xDiv; j++) {
      EntityID id = scene.NewEntity();
      scene.Assign<VertexManager>(id);
      scene.Get<VertexManager>(id)->init(xDiv, yDiv, j, i);
      scene.Get<VertexManager>(id)->createAndBindBuffers();

      std::stringstream ss;
      ss << j << "," << i << std::endl;
      positionMap.insert({ss.str(), id});
      scene.Assign<State>(id);
      scene.Assign<PrevState>(id);
      scene.Get<State>(id)->isAlive =
          std::round(perlin.Get(j / xDiv, i / yDiv));
      //
      // scene.Get<State>(id)->isAlive = false;

      // if (i == 1) {
      //   if (j == 2)
      //     scene.Get<State>(id)->isAlive = true;
      // }
      // if (i == 2) {
      //   if (j == 3)
      //     scene.Get<State>(id)->isAlive = true;
      // }
      // if (i == 3) {
      //   if (j == 1 || j == 2 || j == 3)
      //     scene.Get<State>(id)->isAlive = true;
      // }
    };
  }

  for (auto ent : scene.entities) {
    Pos pos = scene.Get<VertexManager>(ent.id)->position;
    float mags[8][2] = {{0, 1}, {0, -1}, {1, -1}, {-1, -1},
                        {1, 1}, {-1, 1}, {1, 0},  {-1, 0}};

    scene.Assign<Neighbors>(ent.id);

    for (float *vec : mags) {
      std::stringstream stream;
      stream << vec[0] + pos.x << "," << vec[1] + pos.y << std::endl;

      if (positionMap.count(stream.str())) {
        scene.Get<Neighbors>(ent.id)->push_back(positionMap.at(stream.str()));
      }
    }
  }

  //
  VertexManager square1 = VertexManager();
  VertexManager square2 = VertexManager();
  square1.init(2, 2, 0, 0);
  square1.createAndBindBuffers();
  square2.init(2, 2, 1, 0);
  square2.createAndBindBuffers();
  Shader rec_shader("/Users/macbookpro/Documents/personal/graphics/"
                    "ecs_case_study/shaders/rect.vert.glsl",
                    "/Users/macbookpro/Documents/personal/graphics/"
                    "ecs_case_study/shaders/rect.frag.glsl");

  // rect.createAndBindBuffers();
  // Game Loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    int time = std::floor(glfwGetTime() + 1);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    std::vector<Scene::EntityDesc>::iterator it;
    for (it = scene.entities.begin(); it != scene.entities.end(); ++it) {
      int number = 0;
      for (auto neighbor : *scene.Get<Neighbors>(it->id)) {
        if (scene.Get<State>(neighbor)->isAlive)
          number++;
      }
      rec_shader.setBool("isAlive", true);
      rec_shader.use();
      if (scene.Get<State>(it->id)->isAlive)
        scene.Get<VertexManager>(it->id)->drawTriangle();

      scene.Get<PrevState>(it->id)->isAlive =
          scene.Get<State>(it->id)->isAlive == true;
    }

    if ((glfwGetTime() >= 0.025)) {
      glfwSetTime(0);
      populationSystem(&scene);
    }
    windowManger.swapAndPool(window);
  }
  // rect.deleteShaders()
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void populationSystem(Scene *scene) {
  // Any live cell with fewer than two live neighbours dies, as if by
  // underpopulation.
  // Any live cell with two or three live neighbours lives on
  // to the next generation.
  // Any live cell with more than three live neighbours
  // dies, as if by overpopulation.
  // Any dead cell with exactly three live
  // neighbours becomes a live cell, as if by reproduction.

  for (auto ent : scene->entities) {

    auto neighbors = scene->Get<Neighbors>(ent.id);
    int numberOfNeighborsAlive = 0;
    for (auto neighbor : *neighbors) {
      if (scene->Get<PrevState>(neighbor)->isAlive) {
        numberOfNeighborsAlive++;
      }
    }

    // Condition 1
    if (scene->Get<PrevState>(ent.id)->isAlive && numberOfNeighborsAlive < 2) {
      scene->Get<State>(ent.id)->isAlive = false;
    } else if (scene->Get<PrevState>(ent.id)->isAlive &&
               numberOfNeighborsAlive > 3) {
      scene->Get<State>(ent.id)->isAlive = false;
    } else if (!scene->Get<PrevState>(ent.id)->isAlive &&
               numberOfNeighborsAlive == 3) {
      scene->Get<State>(ent.id)->isAlive = true;
    } else if (scene->Get<PrevState>(ent.id)->isAlive &&
               (numberOfNeighborsAlive == 2 || numberOfNeighborsAlive == 3)) {
      scene->Get<State>(ent.id)->isAlive = true;
    }
  }
}
