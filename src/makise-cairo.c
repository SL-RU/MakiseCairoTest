#include "makise_gui.h"
#include <cairo.h>
#include <gtk/gtk.h>

MakiseGUI* mGui;
MHost *host;
MakiseGUI    Gu;
MakiseBuffer Bu;
MakiseDriver Dr;
MHost hs;
MContainer co;

void start_m();

static cairo_t * cr_current = 0;

static void point(cairo_t *cr, int x, int y, MColor c);

static void do_drawing(GtkWidget* widget, cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
                              gpointer user_data)
{      
    do_drawing(widget, cr);

    return FALSE;
}
static void do_drawing(GtkWidget* widget, cairo_t *cr)
{
    cr_current = cr;

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    
    GtkWidget* window = gtk_widget_get_toplevel(widget);

    makise_g_host_call(host, mGui, M_G_CALL_PREDRAW);
    makise_g_host_call(host, mGui, M_G_CALL_DRAW);

    makise_gui_input_perform(host);
    cr_current = 0;

    gtk_widget_queue_draw(window);
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event,
                        gpointer user_data)
{

    MInputData d = {0};
    d.event = M_INPUT_CLICK;
    d.key = M_KEY_CURSOR;
    d.time = 100;
    d.cursor.x = event->x;
    d.cursor.y = event->y;
    makise_gui_input_send(host, d);

    if (event->button == 3) {
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
}

GtkWidget *window;
GtkWidget *darea;
void makise_cairo_init(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    g_signal_connect(G_OBJECT(darea), "draw", 
                     G_CALLBACK(on_draw_event), NULL); 
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    
    g_signal_connect(window, "button-press-event", 
                     G_CALLBACK(clicked), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 240); 
    gtk_window_set_title(GTK_WINDOW(window), "MakiseGUI Cairo backend test");


    MakiseDriver *d = &Dr;
    
    d->lcd_height    = 320;
    d->lcd_width     = 240;
    d->buffer_height = 320;
    d->buffer_width  = 240;
    d->pixeldepth    = 0;
    d->buffer        = 0;
    d->size          = 0;
    d->posx          = 0;
    d->posy          = 0;

    
    start_m();
}

void makise_cairo_start()
{
    gtk_widget_show_all(window);

    gtk_main();
}

static void set_borders(const MakiseBuffer *b) {
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

static void d_char              ( const MakiseBuffer *b,
                                  uint16_t ch,
                                  int16_t x, int16_t y,
                                  const MakiseFont* font,
                                  MColor c )
{
    set_borders(b);
    /* cairo_move_to(cr, 10.0, 50.0); */
    /* cairo_set_source_rgb(cr, 255, 0, 0); */
    /* cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, */
    /*                        CAIRO_FONT_WEIGHT_NORMAL); */
    /* cairo_set_font_size(cr, 40.0); */

    //cairo_show_text(cr, "Disziplin ist Macht.");
    cairo_move_to(cr_current, x, y);
    cairo_set_source_rgb(cr_current, c.r, c.g, c.b);
    cairo_select_font_face(cr_current, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr_current, font->height);

    cairo_show_text(cr_current, (char*)&ch);

}
static void string              ( const MakiseBuffer *b,
                                  const char *s,
                                  uint32_t len,
                                  int16_t x, int16_t y,
                                  MDTextPlacement place,
                                  const MakiseFont* font,
                                  MColor c)
{
    set_borders(b);

    cairo_text_extents_t extents;
    
    cairo_set_source_rgb(cr_current, c.r, c.g, c.b);
    cairo_select_font_face(cr_current, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr_current, font->height);

    cairo_text_extents (cr_current, s, &extents);
    if(place == MDTextPlacement_Center) {
        x = x-(extents.width/2 + extents.x_bearing);
        y = y-(extents.height/2 + extents.y_bearing);
    } else if(place == MDTextPlacement_LeftUp) {
        x = x-(extents.width + extents.x_bearing);
        y = y-(extents.height + extents.y_bearing);
    } else if(place == MDTextPlacement_CenterDown) {
        x = x-(extents.width/2 + extents.x_bearing);        
    } else if(place == MDTextPlacement_CenterUp) {
        x = x-(extents.width/2 + extents.x_bearing);
        y = y-(extents.height + extents.y_bearing);
    }
    cairo_move_to(cr_current, x, y);

    cairo_show_text(cr_current, s);
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
    set_borders(b);
    cairo_rectangle(cr_current, x, y, w, h);
    cairo_clip(cr_current);
    cairo_move_to(cr_current, x, y);
    cairo_set_source_rgb(cr_current, c.r, c.g, c.b);
    cairo_select_font_face(cr_current, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr_current, font->height);

    cairo_show_text(cr_current, s);
}
static uint32_t get_width       ( const MakiseBuffer *b,
                                  const char* s,
                                  uint32_t len,
                                  const MakiseFont* font )
{
    return 0;
}
static uint32_t get_line_count  ( const MakiseBuffer *b,
                                  const char *s,
                                  uint32_t len,
                                  uint16_t w,
                                  const MakiseFont *font )
{
    return 0;
}
static char*    get_line        ( const MakiseBuffer *b,
                                  char *s,
                                  uint32_t len,
                                  uint32_t n,
                                  uint16_t w,
                                  const MakiseFont *font)
{
    return s;
}
static uint32_t get_height      ( const MakiseBuffer *b,
                                  const char*             s,
                                  uint32_t          len,
                                  uint16_t          width_window,
                                  const MakiseFont* font,
                                  uint32_t          font_line_spacing)
{
    return 0;
}

MakiseTextDrawer mtd = {
    .d_char = &d_char,
    .string = &string,
    .string_frame = &string_frame,
    .get_width = &get_width,
    .get_line_count = &get_line_count,
    .get_line = &get_line,
    .get_height = &get_height
};

MResult cairo_drawer(const MakiseBuffer *b, const MDPrimitive *p) {
    if(b == 0 || p == 0)
        return M_ZERO_POINTER;

    set_borders(b);
    
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


//input result handler. That will be called by gui thread after recieving result from input
MInputData inp_handler(MInputData d, MInputResultEnum res);
static void* _get_gui_buffer(uint32_t size)
{
    return cr_current;
}
void start_m()
{
    MakiseGUI    * gu = &Gu;
    MakiseDriver * dr = &Dr;
    host = &hs;
    
    makise_gui_autoinit(host,
			gu, dr,
			&_get_gui_buffer,
			&inp_handler,
			0, 0, 0, &cairo_drawer, &mtd);
    
    mGui = gu;
}
