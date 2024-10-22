/**
 * Author: Root (Discord: Root#0923; GitHub: 0x526f6f74)
 **/

#ifdef RB_REAL_TIME
#    define RB_REAL_TIME 1
#    define RB_OFFSCREEN 0
#else
#    define RB_REAL_TIME 0
#    define RB_OFFSCREEN 1
#endif

#include "buffer.h"
#include "camera.h"
#include "constants.h"
#include "cubemap.h"
#include "offscreen.h"
#include "shader.h"
#include "vertex.h"
#include "window.h"
#include "world.h"

static std::array<rb::Vertex, NUM_VERTICES> vertices;
static std::array<rb::Index, NUM_INDICES> indices;

static void set_block_vertices(int block_index, int texture_index, const glm::vec3& position)
{
    const int index = block_index * 8;
    const float glsl_texture_index = static_cast<float>(texture_index) + 0.5f;

    vertices[index] = rb::Vertex {position + glm::vec3 {0.0f, 0.0f, 0.0f}, {-0.5f, -0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 1] = rb::Vertex {position + glm::vec3 {1.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 2] = rb::Vertex {position + glm::vec3 {1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 3] = rb::Vertex {position + glm::vec3 {0.0f, 1.0f, 0.0f}, {-0.5f, 0.5f, -0.5f}, glsl_texture_index};
    vertices[index + 4] = rb::Vertex {position + glm::vec3 {0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f, 0.5f}, glsl_texture_index};
    vertices[index + 5] = rb::Vertex {position + glm::vec3 {1.0f, 0.0f, 1.0f}, {0.5f, -0.5f, 0.5f}, glsl_texture_index};
    vertices[index + 6] = rb::Vertex {position + glm::vec3 {1.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, glsl_texture_index};
    vertices[index + 7] = rb::Vertex {position + glm::vec3 {0.0f, 1.0f, 1.0f}, {-0.5f, 0.5f, 0.5f}, glsl_texture_index};
}

int main(int argc, char* argv[])
{
    std::cout << "\u001B[36m" << STARTUP_MESSAGE << "\u001B[0m";

    if (argc < 2) return 0;
    std::filesystem::current_path(argv[1]);

    rb::World world {"assets/structures/test_2.mcstructure"};

    for (int i = 0; i < NUM_INDICES; ++i)
        indices[i] = BLOCK_INDICES[i % 36] + ((i / 36) * 8);
    set_block_vertices(0, 0, {0.0f, 0.0f, 0.0f});
    set_block_vertices(1, 1, {1.0f, 0.0f, 0.0f});

    rb::IsometricCamera camera {{WIDTH, HEIGHT, 2.0f}};
#if RB_REAL_TIME
    const rb::CameraController controller {{3.0f, 0.3f, 0.1f}, &camera};
    rb::RealtimeWindow window {
        {"Render Bat",
         WIDTH,
         HEIGHT,
         true,
         3,
         3,
         8,
         [&controller](rb::RealtimeWindow& window, double xoffset, double yoffset) { controller.on_mouse_scrolled(yoffset); },
         [&controller](rb::RealtimeWindow& window, double xpos, double ypos)
         {
             auto& state = window.get_state();
             const glm::vec2 new_cursor_pos = {xpos, ypos};
             if (state.cursor_is_disabled) controller.on_mouse_moved(new_cursor_pos - state.cursor_pos);
             state.cursor_pos = new_cursor_pos;
         }}};
#else
    const rb::OffscreenWindow window {{3, 3, 8}};
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
        const rb::Framebuffer framebuffer {WIDTH, HEIGHT};
#endif

        const rb::VertexBuffer vao {sizeof(vertices), static_cast<const void*>(vertices.data())};
        const rb::IndexBuffer ibo {sizeof(indices), static_cast<const void*>(indices.data())};
        const rb::Shader shader {"render-bat/shaders/cubemap.glsl"};

        const rb::Cubemap grass_cubemap {{
            "assets/blocks/grass_side_carried.png",
            "assets/blocks/grass_side_carried.png",
            "assets/blocks/grass_carried.png",
            "assets/blocks/dirt.png",
            "assets/blocks/grass_side_carried.png",
            "assets/blocks/grass_side_carried.png",
        }};
        const rb::Cubemap bedrock_cubemap {{"assets/blocks/bedrock.png"}};

#if RB_REAL_TIME
        while (window.is_open())
        {
            window.update();
            const auto& state = window.get_state();
            controller.update(state.dt, state.keyboard);
#endif

            glViewport(0, 0, WIDTH, HEIGHT);
            glClearColor(0.471f, 0.655f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.bind();
            shader.set_uniform_mat4("MVP", camera.get_view_projection_matrix());
            int cubemaps[MAX_TEXTURE_SLOTS];
            for (int i = 0; i < MAX_TEXTURE_SLOTS; ++i)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_CUBE_MAP, i + 1);
                cubemaps[i] = i;
            }
            shader.set_uniform_int_array("cubemaps", MAX_TEXTURE_SLOTS, cubemaps);
            vao.bind();
            glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_SHORT, nullptr);

#if RB_REAL_TIME
            window.swap_buffers();
        }
#else
        rb::write_color_buffer_to_png_file("../output.png", WIDTH, HEIGHT);
#endif
    }

    return 0;
}
