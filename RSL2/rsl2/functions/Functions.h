#pragma once
#include "RFGR_Types/rfg/Game.h"
#include "RFGR_Types/rfg/Vector.h"
#include "RFGR_Types/rfg/Plane.h"
#include "RFGR_Types/rfg/Matrix.h"
#include "RFGR_Types/rfg/DrawData.h"
#include "RFGR_Types/rfg/Memory.h"
#include "RFGR_Types/rfg/Camera.h"
#include "RFGR_Types/rfg/Xml.h"

//Forward declarations
struct human;

//Todo: Organize this properly
namespace rfg
{
    //game_state __cdecl gameseq_set_state(game_state state, bool uninterruptible)
    using F_GameseqGetState = game_state(__cdecl*)();

    //void __cdecl gameseq_set_state(game_state state, bool uninterruptible)
    using F_GameseqSetState = void(__cdecl*)(game_state state, bool uninterruptible);

    //int __cdecl memmgr_debug_render(split_memmgr *mgr, int sy) //0x003D25D0
    using F_memmgr_debug_render = bool(__cdecl*)(split_memmgr* mgr, int sy);

    //int __cdecl memmgr_debug_render_tiny(split_memmgr *mgr, int sy) //0x003E56A0
    using F_memmgr_debug_render_tiny = int(__cdecl*)(split_memmgr* mgr, int sy);

    //void __thiscall gr_state::gr_state(gr_state *this, gr_alpha_mode am, gr_clamp_mode cm, gr_zbuf_mode zm, gr_stencil_mode stm, unsigned int const_alpha, 
    //gr_cull_mode tclm, gr_tnl_clip_mode tcm, gr_tnl_light_mode tlm)
    using F_gr_state_constructor = void(__fastcall*)(gr_state* thisPtr, void* edx, gr_alpha_mode am, gr_clamp_mode cm, gr_zbuf_mode zm, gr_stencil_mode stm, unsigned int const_alpha,
                                                     gr_cull_mode tclm, gr_tnl_clip_mode tcm, gr_tnl_light_mode tlm);

    //bool __cdecl gr_bbox_aligned(vector* mn, vector* mx, gr_state* state) //0015E3A0
    using F_gr_bbox_aligned = bool(__cdecl*)(vector* min, vector* max, gr_state* state);

    //void __cdecl grd_3d_string(vector *pos, matrix *orient, float scale, const char *s, int fontnum, gr_state *state)
    using F_gr_3d_string = void(__cdecl*)(vector* pos, matrix* orient, float scale, const char* s, int fontnum, gr_state* state);

    //void __cdecl gr_3d_line(vector *v0, vector *v1, gr_state *state)
    using F_gr_3d_line = void(__cdecl*)(vector* v0, vector* v1, gr_state* state);

    //void __cdecl gr_string(int sx, int sy, const char *s, int fontnum, gr_state *state) //0x0015EC80
    using F_gr_string = void(__cdecl*)(int sx, int sy, const char* s, int fontnum, gr_state* state);

    //void __cdecl gr_get_string_size(int *w, int *h, const char *text, int len, int fontnum) //0x000B9DE0
    using F_get_string_size = void(__cdecl*)(int* w, int* h, const char* text, int len, int fontnum);

    //int __cdecl gr_get_font_height(int fontnum) //0x000B9E40
    using F_get_font_height = int(__cdecl*)(int fontnum);

    //void __cdecl gr_set_color(int r, int g, int b, int a) //0x001090C0
    using F_gr_set_color = void(__cdecl*)(int r, int g, int b, int a);
 
    //void __cdecl gr_line(int x1, int y1, int x2, int y2, gr_state *state, bool __formal) //0x00154DB0
    using F_gr_line = void(__cdecl*)(int x1, int y1, int x2, int y2, gr_state* state, bool __formal);

    //bool __cdecl gr_sphere(vector *center, float radius, gr_state *state) //0x0015E160
    using F_gr_sphere = bool(__cdecl*)(vector* center, float radius, gr_state* state);

    //bool __cdecl gr_bbox_oriented(vector *mn, vector *mx, vector *pos, matrix *orient, gr_state *state) //0x0015E520
    using F_gr_bbox_oriented = bool(__cdecl*)(vector* mn, vector* mx, vector* pos, matrix* orient, gr_state* state);

    //void __cdecl gr_rect(int x1, int y1, int w, int h, gr_state *state) //0x00154BC0
    using F_gr_rect = void(__cdecl*)(int x1, int y1, int w, int h, gr_state* state);

    //Todo: Setup all of these function pointers. Some gr_ functions not included as I don't have immediate use for them
    //void __cdecl gr_set_color(float r, float g, float b, float a) //0x00109160
    using F_gr_set_color2 = void(__cdecl*)(float r, float g, float b, float a);
    
    //void __cdecl gr_set_alpha(int a) //0x001092B0
    using F_gr_set_alpha = void(__cdecl*)(int a);

    //void __cdecl gr_start_instance(vector *pos, matrix *orient, matrix *scale) //0x00109320
    using F_gr_start_instance = void(__cdecl*)(vector* pos, matrix* orient, matrix* scale);

    //void __cdecl gr_stop_instance() //0x001093E0
    using F_gr_stop_instance = void(__cdecl*)();

    //void __cdecl hud_bm_res_independent(int bitmap_id, int cx, int cy, float scale, gr_state state) //0x00441CA0
    using F_hud_bm_res_independent = void(__cdecl*)(int bitmap_id, int cx, int cy, float scale, gr_state state);

    //void __cdecl hud_bm_pos_res_independent(int bitmap_id, int cx, int cy, float scale, gr_state state) //0x00441EF0
    using F_hud_bm_pos_res_independent = void(__cdecl*)(int bitmap_id, int cx, int cy, float scale, gr_state state);

    //void __cdecl gr_rect_outline(int x, int y, int w, int h, int t, gr_state *state) //0x00154B00
    using F_gr_rect_outline = void(__cdecl*)(int x, int y, int w, int h, int t, gr_state* state);

    //void __cdecl gr_rect(float x, float y, float w, float h, gr_state *state) //0x00154C70
    using F_gr_rect2 = void(__cdecl*)(float x, float y, float w, float h, gr_state* state);

    //void __cdecl gr_line2(float x1, float y1, float x2, float y2, gr_state *state, bool __formal) //0x00154D20
    using F_gr_line2 = void(__cdecl*)(float x1, float y1, float x2, float y2, gr_state* state, bool __formal);

    //void __cdecl gr_bitmap(int bitmap_id, int x, int y, gr_state *state) //0x00154E50
    using F_gr_bitmap = void(__cdecl*)(int bitmap_id, int x, int y, gr_state* state);

    //void __cdecl gr_bitmap_scaled(int bitmap_id, int x, int y, int w, int h, gr_state *state) //0x00154ED0
    using F_gr_bitmap_scaled = void(__cdecl*)(int bitmap_id, int x, int y, int w, int h, gr_state* state);

    //void __cdecl gr_bitmap_scaled(int bitmap_id, float x, float y, float w, float h, gr_state *state) //0x00154FB0
    using F_gr_bitmap_scaled2 = void(__cdecl*)(int bitmap_id, float x, float y, float w, float h, gr_state* state);

    //void __cdecl gr_bitmap_centered(int bitmap_id, int cx, int cy, gr_state *state) //0x00155080
    using F_gr_bitmap_centered = void(__cdecl*)(int bitmap_id, int cx, int cy, gr_state* state);

    //void __cdecl gr_bitmap_ex(int bitmap_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool flip_x, bool flip_y, gr_state *state) //0x00155100
    using F_gr_bitmap_ex = void(__cdecl*)(int bitmap_id, int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool flip_x, bool flip_y, gr_state* state);

    //void __cdecl gr_bitmap_rotated_scale(int bitmap_id, float x, float y, float angle, float scalew, float scaleh, gr_state *state) //0x001551F0
    using F_gr_bitmap_rotated_scale = void(__cdecl*)(int bitmap_id, float x, float y, float angle, float scalew, float scaleh, gr_state* state);

    //void __cdecl gr_bitmap_rotated(int bitmap_id, float x, float y, float angle, float scale, gr_state *state) //0x001552D0
    using F_gr_bitmap_rotated = void(__cdecl*)(int bitmap_id, float x, float y, float angle, float scale, gr_state* state);

    //void __cdecl gr_circle(int x, int y, int radius, gr_state *state) //0x00155370
    using F_gr_circle = void(__cdecl*)(int x, int y, int radius, gr_state* state);

    //void __cdecl gr_string_scale(int sx, int sy, const char *s, float scale, int fontnum, gr_state *state) //0x00155400
    using F_gr_string_scale = void(__cdecl*)(int sx, int sy, const char* s, float scale, int fontnum, gr_state* state);

    //void __cdecl gr_3d_bitmap(int bitmap_id, vector *pnt, matrix *orient, float radius, gr_state *state) //0x0015DF80
    using F_gr_3d_bitmap = void(__cdecl*)(int bitmap_id, vector* pnt, matrix* orient, float radius, gr_state* state);

    //void __cdecl gr_bbox_solid(vector *bmin, vector *bmax, gr_state *state) //0x0015E710
    using F_gr_bbox_solid = void(__cdecl*)(vector* bmin, vector* bmax, gr_state* state);

    //void __cdecl gr_cylinder(vector *center, matrix *orient, float radius, float height, gr_state *state) //0x0015E790
    using F_gr_cylinder = void(__cdecl*)(vector* center, matrix* orient, float radius, float height, gr_state* state);

    //void __cdecl gr_plane(vector *pt, matrix *orient, float w, float h, gr_state *state) //0x0015E830
    using F_gr_plane = void(__cdecl*)(vector* pt, matrix* orient, float w, float h, gr_state* state);

    //void __cdecl gr_plane_solid(plane *pl, gr_state *state) //0x0015E8D0
    using F_gr_plane_solid = void(__cdecl*)(plane* pl, gr_state* state);

    //void __cdecl gr_axis(vector *pos, matrix *orient, float scale, gr_state *state) //0x0015E9C0
    using F_gr_axis = void(__cdecl*)(vector* pos, matrix* orient, float scale, gr_state* state);

    //void __cdecl gr_3d_arrow(vector *v1, vector *v2, float arrowhead_pct, gr_state *state) //0x0015E0C0
    using F_gr_3d_arrow = void(__cdecl*)(vector* v1, vector* v2, float arrowhead_pct, gr_state* state);

    //void __cdecl gr_circle_xz(vector *center, float radius, gr_state *state) //0x0015EB20
    using F_gr_circle_xz = void(__cdecl*)(vector* center, float radius, gr_state* state);

    //void __cdecl gr_arc_xz(vector *center, vector *start, float angle, gr_state *state) //0x0015EBF0
    using F_gr_arc_xz = void(__cdecl*)(vector* center, vector* start, float angle, gr_state* state);

    //void __cdecl gr_string(int sx, int sy, const wchar_t *s, int fontnum, gr_state *state) //0x0015ED30
    using F_gr_string2 = void(__cdecl*)(int sx, int sy, const wchar_t* s, int fontnum, gr_state* state);

    //void __cdecl gr_frustum_solid(vector *eye_pos, plane *planes, int nplanes, gr_state *state) //0x00161B50
    using F_gr_frustum_solid = void(__cdecl*)(vector* eye_pos, plane* planes, int nplanes, gr_state* state);

    //void __cdecl gr_frustum(vector *eye_pos, plane *planes, int nplanes, gr_state *state) //0x00161BD0
    using F_gr_frustum = void(__cdecl*)(vector* eye_pos, plane* planes, int nplanes, gr_state* state);

    //bool __cdecl gr_screen_coords_from_world_coords(vector *world_pos, float *screen_x, float *screen_y, float *screen_z, int allow_off_edges) //0x00109570
    using F_gr_screen_coords_from_world_coords = bool(__cdecl*)(vector* world_pos, float* screen_x, float* screen_y, float* screen_z, int allow_off_edges);

    //bool __cdecl gr_sphere_solid(vector *center, float radius, int __formal, gr_state *state) //0x0015E1F0
    using F_gr_sphere_solid = bool(__cdecl*)(vector* center, float radius, int __formal, gr_state* state);

    //bool __cdecl gr_cone(vector *cone_point_pos, matrix *orient, float radius, float height, gr_state *state) //0x0015E280
    using F_gr_cone = bool(__cdecl*)(vector* cone_point_pos, matrix* orient, float radius, float height, gr_state* state);

    //bool __cdecl gr_box(vector *center, matrix *orient, vector *size, gr_state *state) //0x0015E320
    using F_gr_box = bool(__cdecl*)(vector* center, matrix* orient, vector* size, gr_state* state);

    //bool __cdecl gr_3d_circle(vector *center, float radius, vector axis, int __formal, gr_state *state) //0x0015EA50
    using F_gr_3d_circle = bool(__cdecl*)(vector* center, float radius, vector axis, int __formal, gr_state* state);

    //void __cdecl camera_push_mode(camera_mode m) //0x002C9240
    using F_camera_push_mode = void(__cdecl*)(camera_mode mode);

    //void __cdecl camera_pop_mode(camera_mode m) //0x002C92D0
    using F_camera_pop_mode = void(__cdecl*)(camera_mode mode);

    //void __cdecl human_teleport_unsafe(human *hp, vector pos, matrix orient) //0x0067C290
    using F_human_teleport_unsafe = void(__cdecl*)(human* hp, vector pos, matrix orient);

    //void __cdecl hud_hide(bool hide) //0x00441AE0
    using F_hud_hide = void(__cdecl*)(bool hide);

    //void __cdecl game_render_set_fog_enabled(bool enabled) //0x003C2C50
    using F_game_render_set_fog_enabled = void(__cdecl*)(bool hide);

    //xml_element *__cdecl xml_parse(const char *filename, mempool_base *dest) //0x001CD2F0
    using F_xml_parse = xml_element*(__cdecl*)(const char* filename, void* dest);


    class RfgFunctions
    {
    public:
        F_GameseqGetState GameseqGetState = nullptr;
        F_GameseqSetState GameseqSetState = nullptr;

        F_memmgr_debug_render memmgr_debug_render = nullptr;
        F_memmgr_debug_render_tiny memmgr_debug_render_tiny = nullptr;

        //Primitive rendering helpers
        F_gr_state_constructor gr_state_constructor = nullptr;
        F_gr_bbox_aligned gr_bbox_aligned = nullptr;
        F_gr_3d_string gr_3d_string = nullptr;
        F_gr_3d_line gr_3d_line = nullptr;
        F_gr_string gr_string = nullptr;
        F_get_string_size gr_get_string_size = nullptr;
        F_get_font_height gr_get_font_height = nullptr;
        F_gr_set_color gr_set_color = nullptr;
        F_gr_line gr_line = nullptr;
        F_gr_sphere gr_sphere = nullptr;
        F_gr_bbox_oriented gr_bbox_oriented = nullptr;
        F_gr_rect gr_rect = nullptr;
        F_gr_set_color2 gr_set_color2 = nullptr;
        F_gr_set_alpha gr_set_alpha = nullptr;
        F_gr_start_instance gr_start_instance = nullptr;
        F_gr_stop_instance gr_stop_instance = nullptr;
        F_hud_bm_res_independent hud_bm_res_independent = nullptr;
        F_hud_bm_pos_res_independent hud_bm_pos_res_independent = nullptr;
        F_gr_rect_outline gr_rect_outline = nullptr;
        F_gr_rect2 gr_rect2 = nullptr;
        F_gr_line2 gr_line2 = nullptr;
        F_gr_circle gr_circle = nullptr;
        F_gr_string_scale gr_string_scale = nullptr;
        F_gr_bbox_solid gr_bbox_solid = nullptr;
        F_gr_cylinder gr_cylinder = nullptr;
        F_gr_plane gr_plane = nullptr;
        F_gr_plane_solid gr_plane_solid = nullptr;
        F_gr_axis gr_axis = nullptr;
        F_gr_circle_xz gr_circle_xz = nullptr;
        F_gr_arc_xz gr_arc_xz = nullptr;
        F_gr_string2 gr_string2 = nullptr;
        F_gr_frustum_solid gr_frustum_solid = nullptr;
        F_gr_frustum gr_frustum = nullptr;
        F_gr_screen_coords_from_world_coords gr_screen_coords_from_world_coords = nullptr;
        F_gr_sphere_solid gr_sphere_solid = nullptr;
        F_gr_cone gr_cone = nullptr;
        F_gr_box gr_box = nullptr;
        F_gr_3d_circle gr_3d_circle = nullptr;
        //Todo: Fix primitive rendering helpers that don't work / cause crashes below
        F_gr_3d_bitmap gr_3d_bitmap = nullptr;
        //Ones which cause crashes
        //F_gr_3d_arrow gr_3d_arrow = nullptr;
        //F_gr_bitmap gr_bitmap = nullptr;
        //F_gr_bitmap_scaled gr_bitmap_scaled = nullptr;
        //F_gr_bitmap_scaled2 gr_bitmap_scaled2 = nullptr;
        //F_gr_bitmap_centered gr_bitmap_centered = nullptr;
        //F_gr_bitmap_ex gr_bitmap_ex = nullptr;
        //F_gr_bitmap_rotated_scale gr_bitmap_rotated_scale = nullptr;
        //F_gr_bitmap_rotated gr_bitmap_rotated = nullptr;

        F_camera_push_mode camera_push_mode = nullptr;
        F_camera_pop_mode camera_pop_mode = nullptr;
        F_human_teleport_unsafe human_teleport_unsafe = nullptr;
        F_hud_hide hud_hide = nullptr;
        F_game_render_set_fog_enabled game_render_set_fog_enabled = nullptr;
        
        F_xml_parse xml_parse = nullptr;
    };
}