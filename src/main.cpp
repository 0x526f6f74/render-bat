/**
 * Author: Root (GitHub: 0x526f6f74; Discord: stdio.h#5797)
 **/

#define RB_REAL_TIME 1
#define RB_OFFSCREEN 0

#include <array>
#include <iostream>

#include "buffer.h"
#include "camera.h"
#include "constants.h"
#include "cubemap.h"
#include "offscreen.h"
#include "shader.h"
#include "vertex.h"

static constexpr int MAX_TEXTURE_SLOTS = 32;

static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;
static constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;

static constexpr int NUM_BLOCKS = 2;
static constexpr int NUM_VERTICES = NUM_BLOCKS * 8;
static constexpr int NUM_INDICES = NUM_BLOCKS * 36;

static constexpr std::array<rb::index_t, 36> BLOCK_INDICES = {0, 3, 1, 3, 2, 1, 1, 2, 5, 2, 6, 5, 5, 6, 4, 6, 7, 4,
                                                              4, 7, 0, 7, 3, 0, 3, 7, 2, 7, 6, 2, 4, 0, 5, 0, 1, 5};

static std::array<rb::Vertex, NUM_VERTICES> vertices;
static std::array<rb::index_t, NUM_INDICES> indices;
static float zoom_level = 2.0f;

#if RB_REAL_TIME
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool& cursor_is_disabled = static_cast<rb::RealtimeWindowState*>(glfwGetWindowUserPointer(window))->cursor_is_disabled;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && cursor_is_disabled)
    {
        cursor_is_disabled = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    bool& cursor_is_disabled = static_cast<rb::RealtimeWindowState*>(glfwGetWindowUserPointer(window))->cursor_is_disabled;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !cursor_is_disabled)
    {
        cursor_is_disabled = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}
#endif

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    zoom_level -= yoffset * 0.2f;
}

static void set_block_vertices(int block_index, int texture_index, const glm::vec3& position)
{
    const int index = block_index * 8;
    const float glsl_texture_index = static_cast<float>(texture_index) + 0.5f;

    vertices[index] = rb::Vertex{position + glm::vec3{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 1] = rb::Vertex{position + glm::vec3{0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 2] = rb::Vertex{position + glm::vec3{0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 3] = rb::Vertex{position + glm::vec3{-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 4] = rb::Vertex{position + glm::vec3{-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, glsl_texture_index};
    vertices[index + 5] = rb::Vertex{position + glm::vec3{0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, glsl_texture_index};
    vertices[index + 6] = rb::Vertex{position + glm::vec3{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, glsl_texture_index};
    vertices[index + 7] = rb::Vertex{position + glm::vec3{-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, glsl_texture_index};
}

int main()
{
    std::cout << "Render Bat \033[92mv1.0\033[0m\n";

    for (int i = 0; i < NUM_INDICES; ++i)
        indices[i] = BLOCK_INDICES[i % 36] + ((i / 36) * 8);
    set_block_vertices(0, 0, {0.0f, 0.0f, 0.0f});
    set_block_vertices(1, 1, {1.0f, 0.0f, 0.0f});

    const rb::WindowConfig window_config{{3, 3}, 8};
#if RB_REAL_TIME
    rb::RealtimeWindow window{{window_config, "Render Bat", {WIDTH, HEIGHT}, true, key_callback, mouse_button_callback, scroll_callback}};
#else
    const rb::OffscreenWindow window{rb::OffscreenWindowConfig(window_config)};
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    {
#if RB_OFFSCREEN
        const rb::Framebuffer framebuffer{WIDTH, HEIGHT};
#endif

        const rb::VertexBuffer vao{sizeof(vertices), static_cast<const void*>(vertices.data())};
        const rb::IndexBuffer ibo{sizeof(indices), static_cast<const void*>(indices.data())};
        const rb::Shader shader{"../shaders/cubemap.glsl"};
        const rb::CameraConfig camera_config{WIDTH, HEIGHT};
        rb::IsometricCameraController camera{camera_config, zoom_level};

        const rb::Cubemap grass_cubemap{{
            "../../assets/blocks/grass_side_carried.png",
            "../../assets/blocks/grass_side_carried.png",
            "../../assets/blocks/grass_carried.png",
            "../../assets/blocks/dirt.png",
            "../../assets/blocks/grass_side_carried.png",
            "../../assets/blocks/grass_side_carried.png",
        }};
        const rb::Cubemap bedrock_cubemap{{"../../assets/blocks/bedrock.png"}};

#if RB_REAL_TIME
        while (window.is_open())
        {
            window.update();
            camera.update(window.get_state());
            camera.set_zoom_level(zoom_level);
#endif

            glViewport(0, 0, WIDTH, HEIGHT);
            glClearColor(0.471f, 0.655f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.bind();
            shader.set_uniform_matrix4fv("MVP", camera.get_view_projection_matrix());
            int cubemaps[MAX_TEXTURE_SLOTS];
            for (int i = 0; i < MAX_TEXTURE_SLOTS; ++i)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_CUBE_MAP, i + 1);
                cubemaps[i] = i;
            }
            shader.set_uniform_1iv("cubemaps", MAX_TEXTURE_SLOTS, cubemaps);
            vao.bind();
            glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_SHORT, nullptr);

#if RB_REAL_TIME
            window.swap_buffers();
        }
#else
        rb::write_color_buffer_to_png_file("../../output.png", WIDTH, HEIGHT);
#endif
    }

    return 0;
}
