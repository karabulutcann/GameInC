#include "engine/engine.h"
#include "gui/gui.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl4.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define INCLUDE_OVERVIEW
#include <nuklear/overview.c>

struct GUI
{
    struct nk_context *ctx;
};

static struct GUI staticGui;

void guiCreate(struct Engine *engine)
{

    staticGui = (struct GUI){0};

#ifdef INCLUDE_CONFIGURATOR
    static struct nk_color color_table[NK_COLOR_COUNT];
    memcpy(color_table, nk_default_color_style, sizeof(color_table));
#endif

    staticGui.ctx = nk_glfw3_init(engine->window.windowHandle, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        // struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/DroidSans.ttf", 14, 0);
        // struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/Roboto-Regular.ttf", 16, 0);
        // struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/kenvector_future_thin.ttf", 13, 0);
        // struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/ProggyClean.ttf", 16, 0);
        // struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/ProggyTiny.ttf", 10, 0);
        // struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/Cousine-Regular.ttf", 13, 0);
        struct nk_font *font = nk_font_atlas_add_from_file(atlas, "fonts/DepartureMonoNerdFont-Regular.otf", 16, 0);
        
        nk_glfw3_font_stash_end();
        nk_style_load_all_cursors(staticGui.ctx, atlas->cursors);
        nk_style_set_font(staticGui.ctx, &font->handle);
    }

    /* Create bindless texture.
     * The index returned is not the opengl resource id.
     * IF you need the GL resource id use: nk_glfw3_get_tex_ogl_id() */
}

static const char *blocks[] = {
    "AIR",
    "DIRT",
    "STONE",
    "GRASS",
    "OAK_PLANK",
    "OAK_LOG",
    "CRAFTING_TABLE",
    "STONE_BRICK",
    "MOSSY_STONE_BRICK",
    "CRACKED_STONE_BRICK",
};

#define MAX_BLOCK_NAME_LENGTH 24

void guiUpdate(i4 cameraPos[3], Bool *isClicked, int *selectedBlock, float deltaTime)
{
    nk_glfw3_new_frame();

    /* GUI */
    if (nk_begin(staticGui.ctx, "Engine", nk_rect(50, 50, 300, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_MINIMIZED))
    {
        static const float ratio[] = {120, 150};
        static float property_float = 2;
        static int property_int = 10;
        static int property_neg = 10;

        nk_layout_row(staticGui.ctx, NK_STATIC, 25, 1, ratio);
        nk_label(staticGui.ctx, "Position:", NK_TEXT_LEFT);
        nk_property_int(staticGui.ctx, "x:", 0, &cameraPos[0], 15, 1, 1);
        nk_property_int(staticGui.ctx, "y:", 0, &cameraPos[1], 127, 1, 1);
        nk_property_int(staticGui.ctx, "z:", 0, &cameraPos[2], 15, 1, 1);
        *selectedBlock = nk_combo(staticGui.ctx, blocks, NK_LEN(blocks), *selectedBlock, 25, nk_vec2(200, 200));
        if (nk_button_label(staticGui.ctx, "SetBlock"))
        {
            *isClicked = TRUE;
        }

        // nk_layout_row_dynamic(staticGui.ctx, 25, 1);
        // nk_property_int(staticGui.ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(staticGui.ctx);

    if (nk_begin(staticGui.ctx, "Debug", nk_rect(50, 50, 300, 200),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_MINIMIZED))
    {

        nk_layout_row_dynamic(staticGui.ctx, 20, 1);
        mUseFormatted(str, "FPS: %f", 1 / deltaTime)
        {
            nk_label(staticGui.ctx, str.string, NK_TEXT_LEFT);
        }
        mUseFormatted(str2, "Detlta Time: %f", deltaTime)
        {
            nk_label(staticGui.ctx, str2.string, NK_TEXT_LEFT);
        }
        // nk_layout_row_dynamic(staticGui.ctx, 25, 1);
        // nk_property_int(staticGui.ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(staticGui.ctx);

    /* ----------------------------------------- */

    // overview(staticGui.ctx);

    nk_glfw3_render(NK_ANTI_ALIASING_ON);
}

void guiDestroy()
{
    nk_glfw3_shutdown();
}