
#include <math.h>
#include <stdio.h>
#include <libqu/libqu.h>

//------------------------------------------------------------------------------

enum
{
    TEXTURE_SKY,
    TEXTURE_MOUNTAINS_BG,
    TEXTURE_MOUNTAINS_FG,
    TEXTURE_TREES_BG,
    TEXTURE_TREES_FG,
    TOTAL_TEXTURES,
};

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] textures.c");
    qu_set_window_size(720, 480);

    qu_initialize();

    char const *texture_paths[TOTAL_TEXTURES] = {
        "assets/textures/sky.png",
        "assets/textures/mountains-bg.png",
        "assets/textures/mountains-fg.png",
        "assets/textures/trees-bg.png",
        "assets/textures/trees-fg.png",
    };

    qu_texture textures[TOTAL_TEXTURES];

    for (int i = 0; i < TOTAL_TEXTURES; i++) {
        textures[i] = qu_load_texture_from_file(texture_paths[i]);

        if (!textures[i].id) {
            return 0;
        }
    }

    float x_camera = 0.f;
    float dx_camera = 8.f;

    while (qu_process()) {
        x_camera += dx_camera;

        qu_clear(0x202020FF);

        qu_draw_texture(textures[TEXTURE_SKY], -48.f, 0.f, 816.f, 480.f);

        float x_4 = fmodf(x_camera / 4.f, 816.f) + (dx_camera / 4.f);
        qu_draw_texture(textures[TEXTURE_MOUNTAINS_BG], -x_4, 0.f, 816.f, 480.f);
        qu_draw_texture(textures[TEXTURE_MOUNTAINS_BG], 816.f - x_4, 0.f, 816.f, 480.f);
        
        float x_3 = fmodf(x_camera / 3.f, 1632.f) + (dx_camera / 3.f);
        qu_draw_texture(textures[TEXTURE_MOUNTAINS_FG], -x_3, 0.f, 1632.f, 480.f);
        qu_draw_texture(textures[TEXTURE_MOUNTAINS_FG], 1632.f - x_3, 0.f, 1632.f, 480.f);

        float x_2 = fmodf(x_camera / 2.f, 1632.f) + (dx_camera / 2.f);
        qu_draw_texture(textures[TEXTURE_TREES_BG], -x_2, 0.f, 1632.f, 480.f);
        qu_draw_texture(textures[TEXTURE_TREES_BG], 1632.f - x_2, 0.f, 1632.f, 480.f);

        float x_1 = fmodf(x_camera, 1632.f) + dx_camera;
        qu_draw_texture(textures[TEXTURE_TREES_FG], -x_1, 0.f, 1632.f, 480.f);
        qu_draw_texture(textures[TEXTURE_TREES_FG], 1632.f - x_1, 0.f, 1632.f, 480.f);

        qu_present();
    }

    qu_terminate();

    return 0;
}