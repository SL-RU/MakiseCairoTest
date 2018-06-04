#include "makise_gui.h"
#include <cairo.h>
#include <gtk/gtk.h>

void start_m();

static void point(cairo_t *cr, int x, int y, MColor c);

void set_borders(const MakiseBuffer *b) {
    cairo_t * cr_current = b->buffer;
    
    cairo_reset_clip(cr_current);
    cairo_set_source_rgb(cr_current, 255, 255, 255);
    cairo_set_line_width(cr_current, 1);
    cairo_rectangle(cr_current, b->border.x, b->border.y, b->border.w, b->border.h);
    cairo_clip(cr_current);
}

static void clear(cairo_t *cr, MColor c)
{    
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_paint(cr);
}
static void point(cairo_t *cr, int x, int y, MColor c)
{
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_set_line_width(cr, 2);
    cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); /* Round dot*/
    cairo_move_to (cr, x, y); cairo_line_to (cr, x, y);/* a very short line is a dot */
    cairo_stroke (cr);
}

static void rect(cairo_t *cr, int x, int y,
                 unsigned int w, unsigned int h,
                 unsigned int r, unsigned int t,
                 MColor c, MColor c_fill)
{
    cairo_set_source_rgb(cr, c_fill.r, c_fill.g, c_fill.b);
    cairo_rectangle(cr, x, y, w, h);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_set_line_width(cr, t);
    cairo_rectangle(cr, x, y, w, h);
    cairo_stroke(cr);
}

static void line(cairo_t *cr,
                 int x0, int y0,
                 int x1, int y1,
                 unsigned int thickness, MColor c)
{
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_set_line_width(cr, thickness);
    //cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); /* Round dot*/
    cairo_move_to (cr, x0, y0);
    cairo_line_to (cr, x1, y1);
    cairo_stroke (cr);
}

static void circle(cairo_t *cr, int x, int y,
                   unsigned int r, unsigned int t,
                   MColor c, MColor c_fill)
{
    
}

static void triangle(cairo_t *cr,
                     int x0, int y0,
                     int x1, int y1,
                     int x2, int y2,
                     unsigned int t,
                     MColor c, MColor c_fill)
{
    
}


MResult cairo_drawer(const MakiseBuffer *b, const MDPrimitive *p) {
    if(b == 0 || p == 0)
        return M_ZERO_POINTER;

    set_borders(b);
    cairo_t * cr_current = b->buffer;
    
    switch(p->type) {
    case MP_Clear: clear(cr_current, p->color); break;
    case MP_Point: point(cr_current,
                         p->points[0].x, p->points[0].y,
                         p->color);
        break;
    case MP_Rectangle: rect(cr_current,
                            p->points[0].x, p->points[0].y,
                            p->w, p->h, p->r, p->thickness,
                            p->color, p->color_fill);
        break;
    case MP_Line: line
            ( cr_current,
              p->points[0].x, p->points[0].y,
              p->points[1].x, p->points[1].y,
              p->thickness,
              p->color);
        break;
    case MP_Circle: circle
            ( cr_current,
              p->points[0].x, p->points[0].y,
              p->r, p->thickness,
              p->color, p->color_fill);
        break;
    case MP_Triangle: triangle
            ( cr_current,
              p->points[0].x, p->points[0].y,
              p->points[1].x, p->points[1].y,
              p->points[2].x, p->points[2].y,
              p->thickness,
              p->color, p->color_fill );
        break;                        
    default: break;
    }
    cairo_reset_clip(cr_current);

    return M_OK;
}
