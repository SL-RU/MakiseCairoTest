#include "makise_gui.h"
#include <cairo.h>
#include <gtk/gtk.h>

void set_borders(const MakiseBuffer *b);

static void d_char              ( const MakiseBuffer *b,
                                  uint16_t ch,
                                  int16_t x, int16_t y,
                                  const MakiseFont* font,
                                  MColor c )
{
    cairo_t * cr = b->buffer;
    
    set_borders(b);
    /* cairo_move_to(cr, 10.0, 50.0); */
    /* cairo_set_source_rgb(cr, 255, 0, 0); */
    /* cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, */
    /*                        CAIRO_FONT_WEIGHT_NORMAL); */
    /* cairo_set_font_size(cr, 40.0); */

    //cairo_show_text(cr, "Disziplin ist Macht.");
    cairo_move_to(cr, x, y);
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_select_font_face(cr, font->name, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font->size);

    cairo_show_text(cr, (char*)&ch);

}
static void string              ( const MakiseBuffer *b,
                                  const char *s,
                                  uint32_t len,
                                  int16_t x, int16_t y,
                                  MDTextPlacement place,
                                  const MakiseFont* font,
                                  MColor c)
{
    cairo_t * cr = b->buffer;
    
    set_borders(b);

    cairo_text_extents_t extents;
    
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_select_font_face(cr, font->name, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font->size);

    cairo_text_extents (cr, s, &extents);
    
    if(place & MDTextPlacement_HCenter) {
        x -= (extents.width/2 + extents.x_bearing);
    } else if(place == MDTextPlacement_Right) {
        x += (extents.width + extents.x_bearing);
    }
    if(place & MDTextPlacement_VCenter) {
        y -= (extents.height/2 + extents.y_bearing);
    } else if(place & MDTextPlacement_Up) {
        y += extents.height;
    }
    cairo_move_to(cr, x, y);

    cairo_show_text(cr, s);
}
static void string_frame        ( const MakiseBuffer *b,
                                  const char *s,
                                  uint32_t len,
                                  int16_t x, int16_t y,
                                  uint16_t w, uint16_t h,
                                  const MakiseFont *font,
                                  uint16_t line_spacing,
                                  MColor c )
{
    cairo_t * cr = b->buffer;
    
    set_borders(b);
    cairo_rectangle(cr, x, y, w, h);
    cairo_clip(cr);
    cairo_move_to(cr, x, y);
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_select_font_face(cr, font->name, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font->size);

    cairo_show_text(cr, s);
}
static uint32_t get_width       ( const MakiseBuffer *b,
                                  const char* s,
                                  uint32_t len,
                                  const MakiseFont* font )
{
    cairo_t * cr = b->buffer;

    cairo_select_font_face(cr, font->name, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font->size);
    cairo_text_extents_t extents;
    cairo_text_extents(cr, s, &extents);
    
    return extents.x_bearing + extents.width;
}
static uint32_t get_line_count  ( const MakiseBuffer *b,
                                  const char *s,
                                  uint32_t len,
                                  uint16_t w,
                                  const MakiseFont *font )
{
    cairo_t * cr = b->buffer;
    
    return 0;
}
static char*    get_line        ( const MakiseBuffer *b,
                                  char *s,
                                  uint32_t len,
                                  uint32_t n,
                                  uint16_t w,
                                  const MakiseFont *font)
{
    cairo_t * cr = b->buffer;
    
    return s;
}
static uint32_t get_height      ( const MakiseBuffer *b,
                                  const char*             s,
                                  uint32_t          len,
                                  uint16_t          width_window,
                                  const MakiseFont* font,
                                  uint32_t          font_line_spacing)
{
    cairo_t * cr = b->buffer;

    cairo_select_font_face(cr, font->name, CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font->size);
    cairo_text_extents_t extents;
    cairo_text_extents(cr, s, &extents);
    
    return extents.height;
}

MakiseTextDrawer cairo_makise_text_drawer = {
    .d_char = &d_char,
    .string = &string,
    .string_frame = &string_frame,
    .get_width = &get_width,
    .get_line_count = &get_line_count,
    .get_line = &get_line,
    .get_height = &get_height
};
