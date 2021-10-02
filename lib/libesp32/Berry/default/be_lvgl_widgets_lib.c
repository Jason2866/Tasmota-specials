
/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 
 /********************************************************************
 * Tasmota LVGL classes for widgets
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"
#include "be_lvgl.h"

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);
extern int lco_tostring(bvm *vm);

extern int lvx_member(bvm *vm);
extern int lvx_tostring(bvm *vm);       // generic function

extern int lvs_init(bvm *vm);
extern int lvs_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;

/* `lv_style` external functions definitions */
extern int lvbe_style_set_width(bvm *vm);
extern int lvbe_style_set_min_width(bvm *vm);
extern int lvbe_style_set_max_width(bvm *vm);
extern int lvbe_style_set_height(bvm *vm);
extern int lvbe_style_set_min_height(bvm *vm);
extern int lvbe_style_set_max_height(bvm *vm);
extern int lvbe_style_set_x(bvm *vm);
extern int lvbe_style_set_y(bvm *vm);
extern int lvbe_style_set_align(bvm *vm);
extern int lvbe_style_set_transform_width(bvm *vm);
extern int lvbe_style_set_transform_height(bvm *vm);
extern int lvbe_style_set_translate_x(bvm *vm);
extern int lvbe_style_set_translate_y(bvm *vm);
extern int lvbe_style_set_transform_zoom(bvm *vm);
extern int lvbe_style_set_transform_angle(bvm *vm);
extern int lvbe_style_set_pad_top(bvm *vm);
extern int lvbe_style_set_pad_bottom(bvm *vm);
extern int lvbe_style_set_pad_left(bvm *vm);
extern int lvbe_style_set_pad_right(bvm *vm);
extern int lvbe_style_set_pad_row(bvm *vm);
extern int lvbe_style_set_pad_column(bvm *vm);
extern int lvbe_style_set_radius(bvm *vm);
extern int lvbe_style_set_clip_corner(bvm *vm);
extern int lvbe_style_set_opa(bvm *vm);
extern int lvbe_style_set_color_filter_dsc(bvm *vm);
extern int lvbe_style_set_color_filter_opa(bvm *vm);
extern int lvbe_style_set_anim_time(bvm *vm);
extern int lvbe_style_set_anim_speed(bvm *vm);
extern int lvbe_style_set_transition(bvm *vm);
extern int lvbe_style_set_blend_mode(bvm *vm);
extern int lvbe_style_set_layout(bvm *vm);
extern int lvbe_style_set_base_dir(bvm *vm);
extern int lvbe_style_set_bg_color(bvm *vm);
extern int lvbe_style_set_bg_color_filtered(bvm *vm);
extern int lvbe_style_set_bg_opa(bvm *vm);
extern int lvbe_style_set_bg_grad_color(bvm *vm);
extern int lvbe_style_set_bg_grad_color_filtered(bvm *vm);
extern int lvbe_style_set_bg_grad_dir(bvm *vm);
extern int lvbe_style_set_bg_main_stop(bvm *vm);
extern int lvbe_style_set_bg_grad_stop(bvm *vm);
extern int lvbe_style_set_bg_img_src(bvm *vm);
extern int lvbe_style_set_bg_img_opa(bvm *vm);
extern int lvbe_style_set_bg_img_recolor(bvm *vm);
extern int lvbe_style_set_bg_img_recolor_filtered(bvm *vm);
extern int lvbe_style_set_bg_img_recolor_opa(bvm *vm);
extern int lvbe_style_set_bg_img_tiled(bvm *vm);
extern int lvbe_style_set_border_color(bvm *vm);
extern int lvbe_style_set_border_color_filtered(bvm *vm);
extern int lvbe_style_set_border_opa(bvm *vm);
extern int lvbe_style_set_border_width(bvm *vm);
extern int lvbe_style_set_border_side(bvm *vm);
extern int lvbe_style_set_border_post(bvm *vm);
extern int lvbe_style_set_text_color(bvm *vm);
extern int lvbe_style_set_text_color_filtered(bvm *vm);
extern int lvbe_style_set_text_opa(bvm *vm);
extern int lvbe_style_set_text_font(bvm *vm);
extern int lvbe_style_set_text_letter_space(bvm *vm);
extern int lvbe_style_set_text_line_space(bvm *vm);
extern int lvbe_style_set_text_decor(bvm *vm);
extern int lvbe_style_set_text_align(bvm *vm);
extern int lvbe_style_set_img_opa(bvm *vm);
extern int lvbe_style_set_img_recolor(bvm *vm);
extern int lvbe_style_set_img_recolor_filtered(bvm *vm);
extern int lvbe_style_set_img_recolor_opa(bvm *vm);
extern int lvbe_style_set_outline_width(bvm *vm);
extern int lvbe_style_set_outline_color(bvm *vm);
extern int lvbe_style_set_outline_color_filtered(bvm *vm);
extern int lvbe_style_set_outline_opa(bvm *vm);
extern int lvbe_style_set_outline_pad(bvm *vm);
extern int lvbe_style_set_shadow_width(bvm *vm);
extern int lvbe_style_set_shadow_ofs_x(bvm *vm);
extern int lvbe_style_set_shadow_ofs_y(bvm *vm);
extern int lvbe_style_set_shadow_spread(bvm *vm);
extern int lvbe_style_set_shadow_color(bvm *vm);
extern int lvbe_style_set_shadow_color_filtered(bvm *vm);
extern int lvbe_style_set_shadow_opa(bvm *vm);
extern int lvbe_style_set_line_width(bvm *vm);
extern int lvbe_style_set_line_dash_width(bvm *vm);
extern int lvbe_style_set_line_dash_gap(bvm *vm);
extern int lvbe_style_set_line_rounded(bvm *vm);
extern int lvbe_style_set_line_color(bvm *vm);
extern int lvbe_style_set_line_color_filtered(bvm *vm);
extern int lvbe_style_set_line_opa(bvm *vm);
extern int lvbe_style_set_arc_width(bvm *vm);
extern int lvbe_style_set_arc_rounded(bvm *vm);
extern int lvbe_style_set_arc_color(bvm *vm);
extern int lvbe_style_set_arc_color_filtered(bvm *vm);
extern int lvbe_style_set_arc_opa(bvm *vm);
extern int lvbe_style_set_arc_img_src(bvm *vm);

/* `lv_img` external functions definitions */
extern int lvbe_img_set_tasmota_logo(bvm *vm);
extern int lvbe_img_create(bvm *vm);
extern int lvbe_img_set_src(bvm *vm);
extern int lvbe_img_set_offset_x(bvm *vm);
extern int lvbe_img_set_offset_y(bvm *vm);
extern int lvbe_img_set_angle(bvm *vm);
extern int lvbe_img_set_pivot(bvm *vm);
extern int lvbe_img_set_zoom(bvm *vm);
extern int lvbe_img_set_antialias(bvm *vm);
extern int lvbe_img_get_src(bvm *vm);
extern int lvbe_img_get_offset_x(bvm *vm);
extern int lvbe_img_get_offset_y(bvm *vm);
extern int lvbe_img_get_angle(bvm *vm);
extern int lvbe_img_get_pivot(bvm *vm);
extern int lvbe_img_get_zoom(bvm *vm);
extern int lvbe_img_get_antialias(bvm *vm);

/* `lv_obj` external functions definitions */
extern int lvbe_obj_add_event_cb(bvm *vm);
extern int lvbe_obj_remove_event_cb(bvm *vm);
extern int lvbe_obj_remove_event_dsc(bvm *vm);
extern int lvbe_obj_create(bvm *vm);
extern int lvbe_obj_add_flag(bvm *vm);
extern int lvbe_obj_clear_flag(bvm *vm);
extern int lvbe_obj_add_state(bvm *vm);
extern int lvbe_obj_clear_state(bvm *vm);
extern int lvbe_obj_set_user_data(bvm *vm);
extern int lvbe_obj_has_flag(bvm *vm);
extern int lvbe_obj_has_flag_any(bvm *vm);
extern int lvbe_obj_get_state(bvm *vm);
extern int lvbe_obj_has_state(bvm *vm);
extern int lvbe_obj_get_group(bvm *vm);
extern int lvbe_obj_get_user_data(bvm *vm);
extern int lvbe_obj_allocate_spec_attr(bvm *vm);
extern int lvbe_obj_check_type(bvm *vm);
extern int lvbe_obj_has_class(bvm *vm);
extern int lvbe_obj_get_class(bvm *vm);
extern int lvbe_obj_is_valid(bvm *vm);
extern int lvbe_obj_dpx(bvm *vm);
extern int lvbe_obj_class_init_obj(bvm *vm);
extern int lvbe_obj_is_editable(bvm *vm);
extern int lvbe_obj_is_group_def(bvm *vm);
extern int lvbe_obj_init_draw_rect_dsc(bvm *vm);
extern int lvbe_obj_init_draw_label_dsc(bvm *vm);
extern int lvbe_obj_init_draw_img_dsc(bvm *vm);
extern int lvbe_obj_init_draw_line_dsc(bvm *vm);
extern int lvbe_obj_init_draw_arc_dsc(bvm *vm);
extern int lvbe_obj_calculate_ext_draw_size(bvm *vm);
extern int lvbe_obj_refresh_ext_draw_size(bvm *vm);
extern int lvbe_obj_set_pos(bvm *vm);
extern int lvbe_obj_set_x(bvm *vm);
extern int lvbe_obj_set_y(bvm *vm);
extern int lvbe_obj_set_size(bvm *vm);
extern int lvbe_obj_refr_size(bvm *vm);
extern int lvbe_obj_set_width(bvm *vm);
extern int lvbe_obj_set_height(bvm *vm);
extern int lvbe_obj_set_content_width(bvm *vm);
extern int lvbe_obj_set_content_height(bvm *vm);
extern int lvbe_obj_set_layout(bvm *vm);
extern int lvbe_obj_is_layout_positioned(bvm *vm);
extern int lvbe_obj_mark_layout_as_dirty(bvm *vm);
extern int lvbe_obj_update_layout(bvm *vm);
extern int lvbe_obj_set_align(bvm *vm);
extern int lvbe_obj_align(bvm *vm);
extern int lvbe_obj_align_to(bvm *vm);
extern int lvbe_obj_center(bvm *vm);
extern int lvbe_obj_get_coords(bvm *vm);
extern int lvbe_obj_get_x(bvm *vm);
extern int lvbe_obj_get_x2(bvm *vm);
extern int lvbe_obj_get_y(bvm *vm);
extern int lvbe_obj_get_y2(bvm *vm);
extern int lvbe_obj_get_width(bvm *vm);
extern int lvbe_obj_get_height(bvm *vm);
extern int lvbe_obj_get_content_width(bvm *vm);
extern int lvbe_obj_get_content_height(bvm *vm);
extern int lvbe_obj_get_content_coords(bvm *vm);
extern int lvbe_obj_get_self_width(bvm *vm);
extern int lvbe_obj_get_self_height(bvm *vm);
extern int lvbe_obj_refresh_self_size(bvm *vm);
extern int lvbe_obj_refr_pos(bvm *vm);
extern int lvbe_obj_move_to(bvm *vm);
extern int lvbe_obj_move_children_by(bvm *vm);
extern int lvbe_obj_invalidate_area(bvm *vm);
extern int lvbe_obj_invalidate(bvm *vm);
extern int lvbe_obj_area_is_visible(bvm *vm);
extern int lvbe_obj_is_visible(bvm *vm);
extern int lvbe_obj_set_ext_click_area(bvm *vm);
extern int lvbe_obj_get_click_area(bvm *vm);
extern int lvbe_obj_hit_test(bvm *vm);
extern int lvbe_obj_set_scrollbar_mode(bvm *vm);
extern int lvbe_obj_set_scroll_dir(bvm *vm);
extern int lvbe_obj_set_scroll_snap_x(bvm *vm);
extern int lvbe_obj_set_scroll_snap_y(bvm *vm);
extern int lvbe_obj_get_scrollbar_mode(bvm *vm);
extern int lvbe_obj_get_scroll_dir(bvm *vm);
extern int lvbe_obj_get_scroll_snap_x(bvm *vm);
extern int lvbe_obj_get_scroll_snap_y(bvm *vm);
extern int lvbe_obj_get_scroll_x(bvm *vm);
extern int lvbe_obj_get_scroll_y(bvm *vm);
extern int lvbe_obj_get_scroll_top(bvm *vm);
extern int lvbe_obj_get_scroll_bottom(bvm *vm);
extern int lvbe_obj_get_scroll_left(bvm *vm);
extern int lvbe_obj_get_scroll_right(bvm *vm);
extern int lvbe_obj_get_scroll_end(bvm *vm);
extern int lvbe_obj_scroll_by(bvm *vm);
extern int lvbe_obj_scroll_to(bvm *vm);
extern int lvbe_obj_scroll_to_x(bvm *vm);
extern int lvbe_obj_scroll_to_y(bvm *vm);
extern int lvbe_obj_scroll_to_view(bvm *vm);
extern int lvbe_obj_scroll_to_view_recursive(bvm *vm);
extern int lvbe_obj_is_scrolling(bvm *vm);
extern int lvbe_obj_update_snap(bvm *vm);
extern int lvbe_obj_get_scrollbar_area(bvm *vm);
extern int lvbe_obj_scrollbar_invalidate(bvm *vm);
extern int lvbe_obj_readjust_scroll(bvm *vm);
extern int lvbe_obj_add_style(bvm *vm);
extern int lvbe_obj_remove_style(bvm *vm);
extern int lvbe_obj_remove_style_all(bvm *vm);
extern int lvbe_obj_refresh_style(bvm *vm);
extern int lvbe_obj_get_style_prop(bvm *vm);
extern int lvbe_obj_set_local_style_prop(bvm *vm);
extern int lvbe_obj_get_local_style_prop(bvm *vm);
extern int lvbe_obj_remove_local_style_prop(bvm *vm);
extern int lvbe_obj_fade_in(bvm *vm);
extern int lvbe_obj_fade_out(bvm *vm);
extern int lvbe_obj_set_style_pad_all(bvm *vm);
extern int lvbe_obj_set_style_pad_hor(bvm *vm);
extern int lvbe_obj_set_style_pad_ver(bvm *vm);
extern int lvbe_obj_set_style_pad_gap(bvm *vm);
extern int lvbe_obj_set_style_size(bvm *vm);
extern int lvbe_obj_get_style_width(bvm *vm);
extern int lvbe_obj_get_style_min_width(bvm *vm);
extern int lvbe_obj_get_style_max_width(bvm *vm);
extern int lvbe_obj_get_style_height(bvm *vm);
extern int lvbe_obj_get_style_min_height(bvm *vm);
extern int lvbe_obj_get_style_max_height(bvm *vm);
extern int lvbe_obj_get_style_x(bvm *vm);
extern int lvbe_obj_get_style_y(bvm *vm);
extern int lvbe_obj_get_style_align(bvm *vm);
extern int lvbe_obj_get_style_transform_width(bvm *vm);
extern int lvbe_obj_get_style_transform_height(bvm *vm);
extern int lvbe_obj_get_style_translate_x(bvm *vm);
extern int lvbe_obj_get_style_translate_y(bvm *vm);
extern int lvbe_obj_get_style_transform_zoom(bvm *vm);
extern int lvbe_obj_get_style_transform_angle(bvm *vm);
extern int lvbe_obj_get_style_pad_top(bvm *vm);
extern int lvbe_obj_get_style_pad_bottom(bvm *vm);
extern int lvbe_obj_get_style_pad_left(bvm *vm);
extern int lvbe_obj_get_style_pad_right(bvm *vm);
extern int lvbe_obj_get_style_pad_row(bvm *vm);
extern int lvbe_obj_get_style_pad_column(bvm *vm);
extern int lvbe_obj_get_style_radius(bvm *vm);
extern int lvbe_obj_get_style_clip_corner(bvm *vm);
extern int lvbe_obj_get_style_opa(bvm *vm);
extern int lvbe_obj_get_style_color_filter_opa(bvm *vm);
extern int lvbe_obj_get_style_anim_time(bvm *vm);
extern int lvbe_obj_get_style_anim_speed(bvm *vm);
extern int lvbe_obj_get_style_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_layout(bvm *vm);
extern int lvbe_obj_get_style_base_dir(bvm *vm);
extern int lvbe_obj_get_style_bg_color(bvm *vm);
extern int lvbe_obj_get_style_bg_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_bg_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_color(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_dir(bvm *vm);
extern int lvbe_obj_get_style_bg_main_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_img_src(bvm *vm);
extern int lvbe_obj_get_style_bg_img_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_img_recolor(bvm *vm);
extern int lvbe_obj_get_style_bg_img_recolor_filtered(bvm *vm);
extern int lvbe_obj_get_style_bg_img_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_img_tiled(bvm *vm);
extern int lvbe_obj_get_style_border_color(bvm *vm);
extern int lvbe_obj_get_style_border_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_border_opa(bvm *vm);
extern int lvbe_obj_get_style_border_width(bvm *vm);
extern int lvbe_obj_get_style_border_side(bvm *vm);
extern int lvbe_obj_get_style_border_post(bvm *vm);
extern int lvbe_obj_get_style_text_color(bvm *vm);
extern int lvbe_obj_get_style_text_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_text_opa(bvm *vm);
extern int lvbe_obj_get_style_text_font(bvm *vm);
extern int lvbe_obj_get_style_text_letter_space(bvm *vm);
extern int lvbe_obj_get_style_text_line_space(bvm *vm);
extern int lvbe_obj_get_style_text_decor(bvm *vm);
extern int lvbe_obj_get_style_text_align(bvm *vm);
extern int lvbe_obj_get_style_img_opa(bvm *vm);
extern int lvbe_obj_get_style_img_recolor(bvm *vm);
extern int lvbe_obj_get_style_img_recolor_filtered(bvm *vm);
extern int lvbe_obj_get_style_img_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_outline_width(bvm *vm);
extern int lvbe_obj_get_style_outline_color(bvm *vm);
extern int lvbe_obj_get_style_outline_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_outline_opa(bvm *vm);
extern int lvbe_obj_get_style_outline_pad(bvm *vm);
extern int lvbe_obj_get_style_shadow_width(bvm *vm);
extern int lvbe_obj_get_style_shadow_ofs_x(bvm *vm);
extern int lvbe_obj_get_style_shadow_ofs_y(bvm *vm);
extern int lvbe_obj_get_style_shadow_spread(bvm *vm);
extern int lvbe_obj_get_style_shadow_color(bvm *vm);
extern int lvbe_obj_get_style_shadow_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_shadow_opa(bvm *vm);
extern int lvbe_obj_get_style_line_width(bvm *vm);
extern int lvbe_obj_get_style_line_dash_width(bvm *vm);
extern int lvbe_obj_get_style_line_dash_gap(bvm *vm);
extern int lvbe_obj_get_style_line_rounded(bvm *vm);
extern int lvbe_obj_get_style_line_color(bvm *vm);
extern int lvbe_obj_get_style_line_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_line_opa(bvm *vm);
extern int lvbe_obj_get_style_arc_width(bvm *vm);
extern int lvbe_obj_get_style_arc_rounded(bvm *vm);
extern int lvbe_obj_get_style_arc_color(bvm *vm);
extern int lvbe_obj_get_style_arc_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_arc_opa(bvm *vm);
extern int lvbe_obj_get_style_arc_img_src(bvm *vm);
extern int lvbe_obj_set_style_width(bvm *vm);
extern int lvbe_obj_set_style_min_width(bvm *vm);
extern int lvbe_obj_set_style_max_width(bvm *vm);
extern int lvbe_obj_set_style_height(bvm *vm);
extern int lvbe_obj_set_style_min_height(bvm *vm);
extern int lvbe_obj_set_style_max_height(bvm *vm);
extern int lvbe_obj_set_style_x(bvm *vm);
extern int lvbe_obj_set_style_y(bvm *vm);
extern int lvbe_obj_set_style_align(bvm *vm);
extern int lvbe_obj_set_style_transform_width(bvm *vm);
extern int lvbe_obj_set_style_transform_height(bvm *vm);
extern int lvbe_obj_set_style_translate_x(bvm *vm);
extern int lvbe_obj_set_style_translate_y(bvm *vm);
extern int lvbe_obj_set_style_transform_zoom(bvm *vm);
extern int lvbe_obj_set_style_transform_angle(bvm *vm);
extern int lvbe_obj_set_style_pad_top(bvm *vm);
extern int lvbe_obj_set_style_pad_bottom(bvm *vm);
extern int lvbe_obj_set_style_pad_left(bvm *vm);
extern int lvbe_obj_set_style_pad_right(bvm *vm);
extern int lvbe_obj_set_style_pad_row(bvm *vm);
extern int lvbe_obj_set_style_pad_column(bvm *vm);
extern int lvbe_obj_set_style_radius(bvm *vm);
extern int lvbe_obj_set_style_clip_corner(bvm *vm);
extern int lvbe_obj_set_style_opa(bvm *vm);
extern int lvbe_obj_set_style_color_filter_dsc(bvm *vm);
extern int lvbe_obj_set_style_color_filter_opa(bvm *vm);
extern int lvbe_obj_set_style_anim_time(bvm *vm);
extern int lvbe_obj_set_style_anim_speed(bvm *vm);
extern int lvbe_obj_set_style_transition(bvm *vm);
extern int lvbe_obj_set_style_blend_mode(bvm *vm);
extern int lvbe_obj_set_style_layout(bvm *vm);
extern int lvbe_obj_set_style_base_dir(bvm *vm);
extern int lvbe_obj_set_style_bg_color(bvm *vm);
extern int lvbe_obj_set_style_bg_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_bg_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_color(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_dir(bvm *vm);
extern int lvbe_obj_set_style_bg_main_stop(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_stop(bvm *vm);
extern int lvbe_obj_set_style_bg_img_src(bvm *vm);
extern int lvbe_obj_set_style_bg_img_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_img_recolor(bvm *vm);
extern int lvbe_obj_set_style_bg_img_recolor_filtered(bvm *vm);
extern int lvbe_obj_set_style_bg_img_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_img_tiled(bvm *vm);
extern int lvbe_obj_set_style_border_color(bvm *vm);
extern int lvbe_obj_set_style_border_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_border_opa(bvm *vm);
extern int lvbe_obj_set_style_border_width(bvm *vm);
extern int lvbe_obj_set_style_border_side(bvm *vm);
extern int lvbe_obj_set_style_border_post(bvm *vm);
extern int lvbe_obj_set_style_text_color(bvm *vm);
extern int lvbe_obj_set_style_text_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_text_opa(bvm *vm);
extern int lvbe_obj_set_style_text_font(bvm *vm);
extern int lvbe_obj_set_style_text_letter_space(bvm *vm);
extern int lvbe_obj_set_style_text_line_space(bvm *vm);
extern int lvbe_obj_set_style_text_decor(bvm *vm);
extern int lvbe_obj_set_style_text_align(bvm *vm);
extern int lvbe_obj_set_style_img_opa(bvm *vm);
extern int lvbe_obj_set_style_img_recolor(bvm *vm);
extern int lvbe_obj_set_style_img_recolor_filtered(bvm *vm);
extern int lvbe_obj_set_style_img_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_outline_width(bvm *vm);
extern int lvbe_obj_set_style_outline_color(bvm *vm);
extern int lvbe_obj_set_style_outline_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_outline_opa(bvm *vm);
extern int lvbe_obj_set_style_outline_pad(bvm *vm);
extern int lvbe_obj_set_style_shadow_width(bvm *vm);
extern int lvbe_obj_set_style_shadow_ofs_x(bvm *vm);
extern int lvbe_obj_set_style_shadow_ofs_y(bvm *vm);
extern int lvbe_obj_set_style_shadow_spread(bvm *vm);
extern int lvbe_obj_set_style_shadow_color(bvm *vm);
extern int lvbe_obj_set_style_shadow_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_shadow_opa(bvm *vm);
extern int lvbe_obj_set_style_line_width(bvm *vm);
extern int lvbe_obj_set_style_line_dash_width(bvm *vm);
extern int lvbe_obj_set_style_line_dash_gap(bvm *vm);
extern int lvbe_obj_set_style_line_rounded(bvm *vm);
extern int lvbe_obj_set_style_line_color(bvm *vm);
extern int lvbe_obj_set_style_line_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_line_opa(bvm *vm);
extern int lvbe_obj_set_style_arc_width(bvm *vm);
extern int lvbe_obj_set_style_arc_rounded(bvm *vm);
extern int lvbe_obj_set_style_arc_color(bvm *vm);
extern int lvbe_obj_set_style_arc_color_filtered(bvm *vm);
extern int lvbe_obj_set_style_arc_opa(bvm *vm);
extern int lvbe_obj_set_style_arc_img_src(bvm *vm);
extern int lvbe_obj_del(bvm *vm);
extern int lvbe_obj_clean(bvm *vm);
extern int lvbe_obj_del_async(bvm *vm);
extern int lvbe_obj_set_parent(bvm *vm);
extern int lvbe_obj_move_foreground(bvm *vm);
extern int lvbe_obj_move_background(bvm *vm);
extern int lvbe_obj_get_screen(bvm *vm);
extern int lvbe_obj_get_parent(bvm *vm);
extern int lvbe_obj_get_child(bvm *vm);
extern int lvbe_obj_get_child_cnt(bvm *vm);
extern int lvbe_obj_get_child_id(bvm *vm);
extern int lvbe_obj_tree_walk(bvm *vm);

/* `lv_group` external functions definitions */
extern int lvbe_group_create(bvm *vm);
extern int lvbe_group_del(bvm *vm);
extern int lvbe_group_set_default(bvm *vm);
extern int lvbe_group_add_obj(bvm *vm);
extern int lvbe_group_remove_obj(bvm *vm);
extern int lvbe_group_remove_all_objs(bvm *vm);
extern int lvbe_group_focus_obj(bvm *vm);
extern int lvbe_group_focus_next(bvm *vm);
extern int lvbe_group_focus_prev(bvm *vm);
extern int lvbe_group_focus_freeze(bvm *vm);
extern int lvbe_group_send_data(bvm *vm);
extern int lvbe_group_set_focus_cb(bvm *vm);
extern int lvbe_group_set_refocus_policy(bvm *vm);
extern int lvbe_group_set_editing(bvm *vm);
extern int lvbe_group_set_wrap(bvm *vm);
extern int lvbe_group_get_focused(bvm *vm);
extern int lvbe_group_get_focus_cb(bvm *vm);
extern int lvbe_group_get_editing(bvm *vm);
extern int lvbe_group_get_wrap(bvm *vm);
extern int lvbe_group_get_obj_count(bvm *vm);

/* `lv_indev` external functions definitions */
extern int lvbe_indev_enable(bvm *vm);
extern int lvbe_indev_get_type(bvm *vm);
extern int lvbe_indev_reset(bvm *vm);
extern int lvbe_indev_reset_long_press(bvm *vm);
extern int lvbe_indev_set_cursor(bvm *vm);
extern int lvbe_indev_set_group(bvm *vm);
extern int lvbe_indev_set_button_points(bvm *vm);
extern int lvbe_indev_get_point(bvm *vm);
extern int lvbe_indev_get_gesture_dir(bvm *vm);
extern int lvbe_indev_get_key(bvm *vm);
extern int lvbe_indev_get_scroll_dir(bvm *vm);
extern int lvbe_indev_get_scroll_obj(bvm *vm);
extern int lvbe_indev_get_vect(bvm *vm);
extern int lvbe_indev_wait_release(bvm *vm);
extern int lvbe_indev_search_obj(bvm *vm);

/* `lv_chart` external functions definitions */
extern int lvbe_chart_create(bvm *vm);
extern int lvbe_chart_set_type(bvm *vm);
extern int lvbe_chart_set_point_count(bvm *vm);
extern int lvbe_chart_set_range(bvm *vm);
extern int lvbe_chart_set_update_mode(bvm *vm);
extern int lvbe_chart_set_div_line_count(bvm *vm);
extern int lvbe_chart_set_zoom_x(bvm *vm);
extern int lvbe_chart_set_zoom_y(bvm *vm);
extern int lvbe_chart_get_zoom_x(bvm *vm);
extern int lvbe_chart_get_zoom_y(bvm *vm);
extern int lvbe_chart_set_axis_tick(bvm *vm);
extern int lvbe_chart_get_type(bvm *vm);
extern int lvbe_chart_get_point_count(bvm *vm);
extern int lvbe_chart_get_x_start_point(bvm *vm);
extern int lvbe_chart_get_point_pos_by_id(bvm *vm);
extern int lvbe_chart_refresh(bvm *vm);
extern int lvbe_chart_remove_series(bvm *vm);
extern int lvbe_chart_hide_series(bvm *vm);
extern int lvbe_chart_set_series_color(bvm *vm);
extern int lvbe_chart_set_x_start_point(bvm *vm);
extern int lvbe_chart_set_cursor_pos(bvm *vm);
extern int lvbe_chart_set_cursor_point(bvm *vm);
extern int lvbe_chart_get_cursor_point(bvm *vm);
extern int lvbe_chart_set_all_value(bvm *vm);
extern int lvbe_chart_set_next_value(bvm *vm);
extern int lvbe_chart_set_next_value2(bvm *vm);
extern int lvbe_chart_set_value_by_id(bvm *vm);
extern int lvbe_chart_set_value_by_id2(bvm *vm);
extern int lvbe_chart_set_ext_y_array(bvm *vm);
extern int lvbe_chart_set_ext_x_array(bvm *vm);
extern int lvbe_chart_get_pressed_point(bvm *vm);

/* `lv_colorwheel` external functions definitions */
extern int lvbe_colorwheel_create(bvm *vm);
extern int lvbe_colorwheel_set_hsv(bvm *vm);
extern int lvbe_colorwheel_set_rgb(bvm *vm);
extern int lvbe_colorwheel_set_mode(bvm *vm);
extern int lvbe_colorwheel_set_mode_fixed(bvm *vm);
extern int lvbe_colorwheel_get_hsv(bvm *vm);
extern int lvbe_colorwheel_get_rgb(bvm *vm);
extern int lvbe_colorwheel_get_color_mode(bvm *vm);
extern int lvbe_colorwheel_get_color_mode_fixed(bvm *vm);

/* `lv_imgbtn` external functions definitions */
extern int lvbe_imgbtn_create(bvm *vm);
extern int lvbe_imgbtn_set_src(bvm *vm);

/* `lv_led` external functions definitions */
extern int lvbe_led_create(bvm *vm);
extern int lvbe_led_set_color(bvm *vm);
extern int lvbe_led_set_brightness(bvm *vm);
extern int lvbe_led_on(bvm *vm);
extern int lvbe_led_off(bvm *vm);
extern int lvbe_led_toggle(bvm *vm);
extern int lvbe_led_get_brightness(bvm *vm);

/* `lv_meter` external functions definitions */
extern int lvbe_meter_create(bvm *vm);
extern int lvbe_meter_add_scale(bvm *vm);
extern int lvbe_meter_set_scale_ticks(bvm *vm);
extern int lvbe_meter_set_scale_major_ticks(bvm *vm);
extern int lvbe_meter_set_scale_range(bvm *vm);
extern int lvbe_meter_add_needle_line(bvm *vm);
extern int lvbe_meter_add_needle_img(bvm *vm);
extern int lvbe_meter_add_arc(bvm *vm);
extern int lvbe_meter_add_scale_lines(bvm *vm);
extern int lvbe_meter_set_indicator_value(bvm *vm);
extern int lvbe_meter_set_indicator_start_value(bvm *vm);
extern int lvbe_meter_set_indicator_end_value(bvm *vm);

/* `lv_msgbox` external functions definitions */
extern int lvbe_msgbox_create(bvm *vm);
extern int lvbe_msgbox_get_title(bvm *vm);
extern int lvbe_msgbox_get_close_btn(bvm *vm);
extern int lvbe_msgbox_get_text(bvm *vm);
extern int lvbe_msgbox_get_btns(bvm *vm);
extern int lvbe_msgbox_get_active_btn_text(bvm *vm);
extern int lvbe_msgbox_close(bvm *vm);

/* `lv_spinbox` external functions definitions */
extern int lvbe_spinbox_create(bvm *vm);
extern int lvbe_spinbox_set_value(bvm *vm);
extern int lvbe_spinbox_set_rollover(bvm *vm);
extern int lvbe_spinbox_set_digit_format(bvm *vm);
extern int lvbe_spinbox_set_step(bvm *vm);
extern int lvbe_spinbox_set_range(bvm *vm);
extern int lvbe_spinbox_get_rollover(bvm *vm);
extern int lvbe_spinbox_get_value(bvm *vm);
extern int lvbe_spinbox_get_step(bvm *vm);
extern int lvbe_spinbox_step_next(bvm *vm);
extern int lvbe_spinbox_step_prev(bvm *vm);
extern int lvbe_spinbox_increment(bvm *vm);
extern int lvbe_spinbox_decrement(bvm *vm);

/* `lv_arc` external functions definitions */
extern int lvbe_arc_create(bvm *vm);
extern int lvbe_arc_set_start_angle(bvm *vm);
extern int lvbe_arc_set_end_angle(bvm *vm);
extern int lvbe_arc_set_angles(bvm *vm);
extern int lvbe_arc_set_bg_start_angle(bvm *vm);
extern int lvbe_arc_set_bg_end_angle(bvm *vm);
extern int lvbe_arc_set_bg_angles(bvm *vm);
extern int lvbe_arc_set_rotation(bvm *vm);
extern int lvbe_arc_set_mode(bvm *vm);
extern int lvbe_arc_set_value(bvm *vm);
extern int lvbe_arc_set_range(bvm *vm);
extern int lvbe_arc_set_change_rate(bvm *vm);
extern int lvbe_arc_get_angle_start(bvm *vm);
extern int lvbe_arc_get_angle_end(bvm *vm);
extern int lvbe_arc_get_bg_angle_start(bvm *vm);
extern int lvbe_arc_get_bg_angle_end(bvm *vm);
extern int lvbe_arc_get_value(bvm *vm);
extern int lvbe_arc_get_min_value(bvm *vm);
extern int lvbe_arc_get_max_value(bvm *vm);
extern int lvbe_arc_get_mode(bvm *vm);

/* `lv_bar` external functions definitions */
extern int lvbe_bar_create(bvm *vm);
extern int lvbe_bar_set_value(bvm *vm);
extern int lvbe_bar_set_start_value(bvm *vm);
extern int lvbe_bar_set_range(bvm *vm);
extern int lvbe_bar_set_mode(bvm *vm);
extern int lvbe_bar_get_value(bvm *vm);
extern int lvbe_bar_get_start_value(bvm *vm);
extern int lvbe_bar_get_min_value(bvm *vm);
extern int lvbe_bar_get_max_value(bvm *vm);
extern int lvbe_bar_get_mode(bvm *vm);

/* `lv_btn` external functions definitions */
extern int lvbe_btn_create(bvm *vm);

/* `lv_btnmatrix` external functions definitions */
extern int lvbe_btnmatrix_create(bvm *vm);
extern int lvbe_btnmatrix_set_map(bvm *vm);
extern int lvbe_btnmatrix_set_ctrl_map(bvm *vm);
extern int lvbe_btnmatrix_set_selected_btn(bvm *vm);
extern int lvbe_btnmatrix_set_btn_ctrl(bvm *vm);
extern int lvbe_btnmatrix_clear_btn_ctrl(bvm *vm);
extern int lvbe_btnmatrix_set_btn_ctrl_all(bvm *vm);
extern int lvbe_btnmatrix_clear_btn_ctrl_all(bvm *vm);
extern int lvbe_btnmatrix_set_btn_width(bvm *vm);
extern int lvbe_btnmatrix_set_one_checked(bvm *vm);
extern int lvbe_btnmatrix_get_selected_btn(bvm *vm);
extern int lvbe_btnmatrix_get_btn_text(bvm *vm);
extern int lvbe_btnmatrix_has_btn_ctrl(bvm *vm);
extern int lvbe_btnmatrix_get_one_checked(bvm *vm);

/* `lv_canvas` external functions definitions */
extern int lvbe_canvas_create(bvm *vm);
extern int lvbe_canvas_set_buffer(bvm *vm);
extern int lvbe_canvas_set_px(bvm *vm);
extern int lvbe_canvas_set_palette(bvm *vm);
extern int lvbe_canvas_get_px(bvm *vm);
extern int lvbe_canvas_copy_buf(bvm *vm);
extern int lvbe_canvas_transform(bvm *vm);
extern int lvbe_canvas_blur_hor(bvm *vm);
extern int lvbe_canvas_blur_ver(bvm *vm);
extern int lvbe_canvas_fill_bg(bvm *vm);
extern int lvbe_canvas_draw_rect(bvm *vm);
extern int lvbe_canvas_draw_text(bvm *vm);
extern int lvbe_canvas_draw_img(bvm *vm);
extern int lvbe_canvas_draw_line(bvm *vm);
extern int lvbe_canvas_draw_polygon(bvm *vm);
extern int lvbe_canvas_draw_arc(bvm *vm);

/* `lv_checkbox` external functions definitions */
extern int lvbe_checkbox_create(bvm *vm);
extern int lvbe_checkbox_set_text(bvm *vm);
extern int lvbe_checkbox_set_text_static(bvm *vm);
extern int lvbe_checkbox_get_text(bvm *vm);

/* `lv_dropdown` external functions definitions */
extern int lvbe_dropdown_create(bvm *vm);
extern int lvbe_dropdown_set_text(bvm *vm);
extern int lvbe_dropdown_set_options(bvm *vm);
extern int lvbe_dropdown_set_options_static(bvm *vm);
extern int lvbe_dropdown_add_option(bvm *vm);
extern int lvbe_dropdown_clear_options(bvm *vm);
extern int lvbe_dropdown_set_selected(bvm *vm);
extern int lvbe_dropdown_set_dir(bvm *vm);
extern int lvbe_dropdown_set_symbol(bvm *vm);
extern int lvbe_dropdown_set_selected_highlight(bvm *vm);
extern int lvbe_dropdown_get_list(bvm *vm);
extern int lvbe_dropdown_get_text(bvm *vm);
extern int lvbe_dropdown_get_options(bvm *vm);
extern int lvbe_dropdown_get_selected(bvm *vm);
extern int lvbe_dropdown_get_option_cnt(bvm *vm);
extern int lvbe_dropdown_get_selected_str(bvm *vm);
extern int lvbe_dropdown_get_symbol(bvm *vm);
extern int lvbe_dropdown_get_selected_highlight(bvm *vm);
extern int lvbe_dropdown_get_dir(bvm *vm);
extern int lvbe_dropdown_open(bvm *vm);
extern int lvbe_dropdown_close(bvm *vm);

/* `lv_label` external functions definitions */
extern int lvbe_label_create(bvm *vm);
extern int lvbe_label_set_text(bvm *vm);
extern int lvbe_label_set_text_fmt(bvm *vm);
extern int lvbe_label_set_text_static(bvm *vm);
extern int lvbe_label_set_long_mode(bvm *vm);
extern int lvbe_label_set_recolor(bvm *vm);
extern int lvbe_label_set_text_sel_start(bvm *vm);
extern int lvbe_label_set_text_sel_end(bvm *vm);
extern int lvbe_label_get_text(bvm *vm);
extern int lvbe_label_get_long_mode(bvm *vm);
extern int lvbe_label_get_recolor(bvm *vm);
extern int lvbe_label_get_letter_pos(bvm *vm);
extern int lvbe_label_get_letter_on(bvm *vm);
extern int lvbe_label_is_char_under_pos(bvm *vm);
extern int lvbe_label_get_text_selection_start(bvm *vm);
extern int lvbe_label_get_text_selection_end(bvm *vm);
extern int lvbe_label_ins_text(bvm *vm);
extern int lvbe_label_cut_text(bvm *vm);

/* `lv_line` external functions definitions */
extern int lvbe_line_create(bvm *vm);
extern int lvbe_line_set_points(bvm *vm);
extern int lvbe_line_set_y_invert(bvm *vm);
extern int lvbe_line_get_y_invert(bvm *vm);

/* `lv_roller` external functions definitions */
extern int lvbe_roller_create(bvm *vm);
extern int lvbe_roller_set_options(bvm *vm);
extern int lvbe_roller_set_selected(bvm *vm);
extern int lvbe_roller_set_visible_row_count(bvm *vm);
extern int lvbe_roller_get_selected(bvm *vm);
extern int lvbe_roller_get_selected_str(bvm *vm);
extern int lvbe_roller_get_options(bvm *vm);
extern int lvbe_roller_get_option_cnt(bvm *vm);

/* `lv_slider` external functions definitions */
extern int lvbe_slider_create(bvm *vm);
extern int lvbe_slider_set_value(bvm *vm);
extern int lvbe_slider_set_left_value(bvm *vm);
extern int lvbe_slider_set_range(bvm *vm);
extern int lvbe_slider_set_mode(bvm *vm);
extern int lvbe_slider_get_value(bvm *vm);
extern int lvbe_slider_get_left_value(bvm *vm);
extern int lvbe_slider_get_min_value(bvm *vm);
extern int lvbe_slider_get_max_value(bvm *vm);
extern int lvbe_slider_is_dragged(bvm *vm);
extern int lvbe_slider_get_mode(bvm *vm);

/* `lv_switch` external functions definitions */
extern int lvbe_switch_create(bvm *vm);

/* `lv_table` external functions definitions */
extern int lvbe_table_create(bvm *vm);
extern int lvbe_table_set_cell_value(bvm *vm);
extern int lvbe_table_set_cell_value_fmt(bvm *vm);
extern int lvbe_table_set_row_cnt(bvm *vm);
extern int lvbe_table_set_col_cnt(bvm *vm);
extern int lvbe_table_set_col_width(bvm *vm);
extern int lvbe_table_add_cell_ctrl(bvm *vm);
extern int lvbe_table_clear_cell_ctrl(bvm *vm);
extern int lvbe_table_get_cell_value(bvm *vm);
extern int lvbe_table_get_row_cnt(bvm *vm);
extern int lvbe_table_get_col_cnt(bvm *vm);
extern int lvbe_table_get_col_width(bvm *vm);
extern int lvbe_table_has_cell_ctrl(bvm *vm);
extern int lvbe_table_get_selected_cell(bvm *vm);

/* `lv_textarea` external functions definitions */
extern int lvbe_textarea_create(bvm *vm);
extern int lvbe_textarea_add_char(bvm *vm);
extern int lvbe_textarea_add_text(bvm *vm);
extern int lvbe_textarea_del_char(bvm *vm);
extern int lvbe_textarea_del_char_forward(bvm *vm);
extern int lvbe_textarea_set_text(bvm *vm);
extern int lvbe_textarea_set_placeholder_text(bvm *vm);
extern int lvbe_textarea_set_cursor_pos(bvm *vm);
extern int lvbe_textarea_set_cursor_click_pos(bvm *vm);
extern int lvbe_textarea_set_password_mode(bvm *vm);
extern int lvbe_textarea_set_one_line(bvm *vm);
extern int lvbe_textarea_set_accepted_chars(bvm *vm);
extern int lvbe_textarea_set_max_length(bvm *vm);
extern int lvbe_textarea_set_insert_replace(bvm *vm);
extern int lvbe_textarea_set_text_selection(bvm *vm);
extern int lvbe_textarea_set_password_show_time(bvm *vm);
extern int lvbe_textarea_set_align(bvm *vm);
extern int lvbe_textarea_get_text(bvm *vm);
extern int lvbe_textarea_get_placeholder_text(bvm *vm);
extern int lvbe_textarea_get_label(bvm *vm);
extern int lvbe_textarea_get_cursor_pos(bvm *vm);
extern int lvbe_textarea_get_cursor_click_pos(bvm *vm);
extern int lvbe_textarea_get_password_mode(bvm *vm);
extern int lvbe_textarea_get_one_line(bvm *vm);
extern int lvbe_textarea_get_accepted_chars(bvm *vm);
extern int lvbe_textarea_get_max_length(bvm *vm);
extern int lvbe_textarea_text_is_selected(bvm *vm);
extern int lvbe_textarea_get_text_selection(bvm *vm);
extern int lvbe_textarea_get_password_show_time(bvm *vm);
extern int lvbe_textarea_clear_selection(bvm *vm);
extern int lvbe_textarea_cursor_right(bvm *vm);
extern int lvbe_textarea_cursor_left(bvm *vm);
extern int lvbe_textarea_cursor_down(bvm *vm);
extern int lvbe_textarea_cursor_up(bvm *vm);

#include "../generate/be_fixed_be_class_lv_style.h"
#include "../generate/be_fixed_be_class_lv_img.h"
#include "../generate/be_fixed_be_class_lv_obj.h"
#include "../generate/be_fixed_be_class_lv_group.h"
#include "../generate/be_fixed_be_class_lv_indev.h"
#include "../generate/be_fixed_be_class_lv_chart.h"
#include "../generate/be_fixed_be_class_lv_colorwheel.h"
#include "../generate/be_fixed_be_class_lv_imgbtn.h"
#include "../generate/be_fixed_be_class_lv_led.h"
#include "../generate/be_fixed_be_class_lv_meter.h"
#include "../generate/be_fixed_be_class_lv_msgbox.h"
#include "../generate/be_fixed_be_class_lv_spinbox.h"
#include "../generate/be_fixed_be_class_lv_arc.h"
#include "../generate/be_fixed_be_class_lv_bar.h"
#include "../generate/be_fixed_be_class_lv_btn.h"
#include "../generate/be_fixed_be_class_lv_btnmatrix.h"
#include "../generate/be_fixed_be_class_lv_canvas.h"
#include "../generate/be_fixed_be_class_lv_checkbox.h"
#include "../generate/be_fixed_be_class_lv_dropdown.h"
#include "../generate/be_fixed_be_class_lv_label.h"
#include "../generate/be_fixed_be_class_lv_line.h"
#include "../generate/be_fixed_be_class_lv_roller.h"
#include "../generate/be_fixed_be_class_lv_slider.h"
#include "../generate/be_fixed_be_class_lv_switch.h"
#include "../generate/be_fixed_be_class_lv_table.h"
#include "../generate/be_fixed_be_class_lv_textarea.h"

void be_load_lv_style_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_style);
    be_setglobal(vm, "lv_style");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_style (scope: global, name: lv_style) {
    _p, var
    init, func(lvs_init)
    tostring, func(lvs_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_img_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_img);
    be_setglobal(vm, "lv_img");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_img (scope: global, name: lv_img, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_img_class)
    init, func(lvbe_img_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_obj_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_obj);
    be_setglobal(vm, "lv_obj");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_obj (scope: global, name: lv_obj) {
    _p, var
    _class, int(&lv_obj_class)
    init, func(lvbe_obj_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_group_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_group);
    be_setglobal(vm, "lv_group");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_group (scope: global, name: lv_group) {
    _p, var
    init, func(lvbe_group_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_indev_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_indev);
    be_setglobal(vm, "lv_indev");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_indev (scope: global, name: lv_indev) {
    _p, var
    init, func(lv0_init)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_chart_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_chart);
    be_setglobal(vm, "lv_chart");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_chart (scope: global, name: lv_chart, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_chart_class)
    init, func(lvbe_chart_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_colorwheel_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_colorwheel);
    be_setglobal(vm, "lv_colorwheel");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_colorwheel (scope: global, name: lv_colorwheel, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_colorwheel_class)
    init, func(lvbe_colorwheel_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_imgbtn_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_imgbtn);
    be_setglobal(vm, "lv_imgbtn");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_imgbtn (scope: global, name: lv_imgbtn, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_imgbtn_class)
    init, func(lvbe_imgbtn_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_led_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_led);
    be_setglobal(vm, "lv_led");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_led (scope: global, name: lv_led, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_led_class)
    init, func(lvbe_led_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_meter_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_meter);
    be_setglobal(vm, "lv_meter");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_meter (scope: global, name: lv_meter, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_meter_class)
    init, func(lvbe_meter_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_msgbox_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_msgbox);
    be_setglobal(vm, "lv_msgbox");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_msgbox (scope: global, name: lv_msgbox, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_msgbox_class)
    init, func(lvbe_msgbox_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_spinbox_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_spinbox);
    be_setglobal(vm, "lv_spinbox");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_spinbox (scope: global, name: lv_spinbox, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_spinbox_class)
    init, func(lvbe_spinbox_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_arc_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_arc);
    be_setglobal(vm, "lv_arc");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_arc (scope: global, name: lv_arc, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_arc_class)
    init, func(lvbe_arc_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_bar_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_bar);
    be_setglobal(vm, "lv_bar");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_bar (scope: global, name: lv_bar, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_bar_class)
    init, func(lvbe_bar_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_btn_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_btn);
    be_setglobal(vm, "lv_btn");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_btn (scope: global, name: lv_btn, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_btn_class)
    init, func(lvbe_btn_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_btnmatrix_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_btnmatrix);
    be_setglobal(vm, "lv_btnmatrix");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_btnmatrix (scope: global, name: lv_btnmatrix, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_btnmatrix_class)
    init, func(lvbe_btnmatrix_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_canvas_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_canvas);
    be_setglobal(vm, "lv_canvas");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_canvas (scope: global, name: lv_canvas, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_canvas_class)
    init, func(lvbe_canvas_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_checkbox_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_checkbox);
    be_setglobal(vm, "lv_checkbox");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_checkbox (scope: global, name: lv_checkbox, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_checkbox_class)
    init, func(lvbe_checkbox_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_dropdown_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_dropdown);
    be_setglobal(vm, "lv_dropdown");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_dropdown (scope: global, name: lv_dropdown, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_dropdown_class)
    init, func(lvbe_dropdown_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_label_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_label);
    be_setglobal(vm, "lv_label");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_label (scope: global, name: lv_label, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_label_class)
    init, func(lvbe_label_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_line_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_line);
    be_setglobal(vm, "lv_line");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_line (scope: global, name: lv_line, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_line_class)
    init, func(lvbe_line_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_roller_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_roller);
    be_setglobal(vm, "lv_roller");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_roller (scope: global, name: lv_roller, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_roller_class)
    init, func(lvbe_roller_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_slider_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_slider);
    be_setglobal(vm, "lv_slider");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_slider (scope: global, name: lv_slider, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_slider_class)
    init, func(lvbe_slider_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_switch_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_switch);
    be_setglobal(vm, "lv_switch");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_switch (scope: global, name: lv_switch, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_switch_class)
    init, func(lvbe_switch_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_table_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_table);
    be_setglobal(vm, "lv_table");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_table (scope: global, name: lv_table, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_table_class)
    init, func(lvbe_table_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_textarea_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_textarea);
    be_setglobal(vm, "lv_textarea");
    be_pop(vm, 1);
};

/* @const_object_info_begin
class be_class_lv_textarea (scope: global, name: lv_textarea, super: be_class_lv_obj) {
    _p, var
    _class, int(&lv_textarea_class)
    init, func(lvbe_textarea_create)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */


#endif // USE_LVGL

