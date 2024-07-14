
#ifndef BASIC_MODEL
#define BASIC_MODEL
struct basic_model {
  GLuint shader = 0;
  cgra::gl_mesh mesh;
  glm::vec3 color{0.7};
  glm::mat4 modelTransform{1.0};
  GLuint texture;

  void draw(const glm::mat4 &view, const glm::mat4 proj);
};
#endif