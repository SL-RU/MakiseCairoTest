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

/* Function that take input as Control Point x_coordinates and 
Control Point y_coordinates and draw bezier curve */
void bezierCurve(int x[] , int y[])
{
    int steps = 10;
    for(int i = 0; i < 4; i++) {
        point(cr_current, (int)x[i], (int)y[i], (MColor){0,255,0,0});
    }
    cairo_set_source_rgb(cr_current, 255, 0, 255);
    cairo_move_to (cr_current, x[0], y[0]);
    cairo_set_line_width(cr_current, 2);
    int64_t xu = 0 , yu = 0 , u = 0 ;
    int i = 0 ;
    for(u = 0 ; u <= steps ; u += 1)
    {
        xu = (1-u)*(1-u)*(1-u)    * x[0] * steps 
            + 3 * u * (1-u)*(1-u) * x[1] * steps
            + 3 * u * u * (1-u)   * x[2] * steps
            + u * u * u           * x[3] * steps;
        
        yu = (1-u)*(1-u)*(1-u)    * y[0] * steps
            + 3 * u * (1-u)*(1-u) * y[1] * steps
            + 3 * u * u * (1-u)   * y[2] * steps
            + u * u * u           * y[3] * steps;
        cairo_line_to (cr_current,
                       xu / (steps * steps * steps * steps),
                       yu / (steps * steps * steps * steps));
        //cairo_line_to (cr_current, xu, yu);
    }
    cairo_line_to (cr_current, x[3], y[3]);
    cairo_stroke(cr_current);
}

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
    //gtk_window_get_size(GTK_WINDOW(window), &width, &height);

    //printf("redraw\n");
    makise_g_host_call(host, mGui, M_G_CALL_PREDRAW);
    makise_g_host_call(host, mGui, M_G_CALL_DRAW);
    
    /* cairo_set_source_rgb(cr, 255, 255, 255); */
    /* cairo_set_line_width(cr, 1); */
    /* cairo_rectangle(cr, 10, 20, 100, 50); */
    /* cairo_stroke(cr); */
    /* cairo_rectangle(cr, 10, 20, 100, 50); */
    /* cairo_clip(cr); */
    /* cairo_move_to(cr, 10.0, 50.0); */
    /* cairo_set_source_rgb(cr, 255, 0, 0); */
    /* cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, */
    /*                        CAIRO_FONT_WEIGHT_NORMAL); */
    /* cairo_set_font_size(cr, 40.0); */

    /* cairo_show_text(cr, "Disziplin ist Macht."); */

    bezierCurve((int[4]){20, 100, 80, 200}, (int[4]){20, 100, 100, 40});
    
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
    gtk_window_set_title(GTK_WINDOW(window), "GTK window");


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
    cairo_set_line_width(cr, 2);
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
			0, 0, 0, &cairo_drawer);
    
    mGui = gu;
}
