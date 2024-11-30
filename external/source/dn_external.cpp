// If __GL_H__ is not defined, then the Windows OpenGL header will define GL_VERSION_1_1. For some
// reason, the Windows header does not define the macro version of the function pointers, so the
// ImGui OpenGL loader will fail. Define this here to prevent that.
#define __GL_H__

#include "glad/glad.c"

#include "imgui/cimgui.cpp"
#include "imgui/imgui.cpp"
#include "imgui/imgui_demo.cpp"
#include "imgui/imgui_draw.cpp"
#include "imgui/imgui_tables.cpp"
#include "imgui/imgui_widgets.cpp"
#include "imgui/imgui_impl_glfw.cpp"
#include "imgui/imgui_impl_opengl3.cpp"
