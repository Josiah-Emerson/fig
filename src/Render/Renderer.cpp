#include "Renderer.h"
#include <GL/gl.h>

void Renderer::render(const float R, const float G, const float B){
   glClearColor(R, G, B, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   platform_.RenderImGuiDrawData();
   platform_.SwapBuffers();
}
