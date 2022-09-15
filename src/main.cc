/**
 * Author: Root (GitHub: 0x526f6f74; Discord: stdio.h#5797)
 **/

#define RB_REAL_TIME 1
#define RB_OFFSCREEN 0

#include <iostream>

#include "buffer.h"
#include "camera.h"
#include "constants.h"
#include "cubemap.h"
#include "offscreen.h"
#include "shader.h"
#include "vertex.h"

static std::array<rb::Vertex, NUM_VERTICES> vertices;
static std::array<rb::index_t, NUM_INDICES> indices;

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
    std::cout << "\u001B[36m" << STARTUP_MESSAGE << "\u001B[0m";

    for (int i = 0; i < NUM_INDICES; ++i)
        indices[i] = BLOCK_INDICES[i % 36] + ((i / 36) * 8);
    set_block_vertices(0, 0, {0.0f, 0.0f, 0.0f});
    set_block_vertices(1, 1, {1.0f, 0.0f, 0.0f});

    const rb::CameraConfig camera_config{WIDTH, HEIGHT};
    const rb::WindowConfig window_config{{3, 3}, 8};
#if RB_REAL_TIME
    rb::IsometricCameraController camera{camera_config, 2.0f};
    rb::RealtimeWindow window{
        {window_config,
         "Render Bat",
         {WIDTH, HEIGHT},
         true,
         [&camera](rb::RealtimeWindow& window, double xoffset, double yoffset) { camera.on_mouse_scroll(yoffset); },
         [&camera](rb::RealtimeWindow& window, double xpos, double ypos)
         {
             auto& state = window.get_state();
             const glm::dvec2 new_cursor_pos = {xpos, ypos};
             if (state.cursor_is_disabled) camera.on_mouse_move(new_cursor_pos - state.cursor_pos);
             state.cursor_pos = new_cursor_pos;
         }}};
#else
    rb::IsometricCamera camera{camera_config, 2.0f};
    const rb::OffscreenWindow window{window_config};
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

        const rb::VertexBuffer vao{sizeof(vertices), reinterpret_cast<const void*>(vertices.data())};
        const rb::IndexBuffer ibo{sizeof(indices), reinterpret_cast<const void*>(indices.data())};
        const rb::Shader shader{"../render-bat/shaders/cubemap.glsl"};

        const rb::Cubemap grass_cubemap{{
            "../assets/blocks/grass_side_carried.png",
            "../assets/blocks/grass_side_carried.png",
            "../assets/blocks/grass_carried.png",
            "../assets/blocks/dirt.png",
            "../assets/blocks/grass_side_carried.png",
            "../assets/blocks/grass_side_carried.png",
        }};
        const rb::Cubemap bedrock_cubemap{{"../assets/blocks/bedrock.png"}};

#if RB_REAL_TIME
        while (window.is_open())
        {
            window.update_time();
            camera.update(window.get_state());
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
        rb::write_color_buffer_to_png_file("../output.png", WIDTH, HEIGHT);
#endif
    }

    return 0;
}
