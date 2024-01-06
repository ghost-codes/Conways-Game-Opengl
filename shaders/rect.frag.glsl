#version 410 core
out vec4 FragColor;

uniform bool isAlive;

void main() {
  if (isAlive) {
    FragColor = vec4(1.0f);
  } else {
    FragColor = vec4(0.0f);
  }
}
