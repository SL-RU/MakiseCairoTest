#include "makise_gui.h"
#include <cairo.h>
#include <gtk/gtk.h>

MakiseGUI* mGui;
MHost *host;
MakiseGUI    Gu;
MakiseDriver Dr;
MHost hs;
MContainer co;

void start_m();

static void do_drawing(GtkWidget* widget, cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
                              gpointer user_data)
{      
    do_drawing(widget, cr);

    return FALSE;
}
static void do_drawing(GtkWidget* widget, cairo_t *cr)
{
    mGui->buffer->buffer = cr;

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    
    GtkWidget* window = gtk_widget_get_toplevel(widget);

    makise_g_host_call(host, mGui, M_G_CALL_PREDRAW);
    makise_g_host_call(host, mGui, M_G_CALL_DRAW);

    makise_gui_input_perform(host);

    mGui->buffer->buffer = 0;

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
    
    d->lcd_height    = 240;
    d->lcd_width     = 320;
    d->buffer_height = 240;
    d->buffer_width  = 320;
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

extern MakiseTextDrawer cairo_makise_text_drawer; // makise-cairo-text.c
MResult cairo_drawer(const MakiseBuffer *b, const MDPrimitive *p); // makise-cairo-drawer.c
//input result handler. That will be called by gui thread after recieving result from input
MInputData inp_handler(MInputData d, MInputResultEnum res);
static void* _get_gui_buffer(uint32_t size)
{
    return 0;
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
			0, 0, 0, &cairo_drawer, &cairo_makise_text_drawer);
    
    mGui = gu;
}
