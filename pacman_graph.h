#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <math.h>
#include <unistd.h>
//#include <pthread.h>

//the global pixmap that will serve as our buffer
static GdkPixmap *pixmap = NULL;
static int TAB_N = 0, TAB_M = 0;
static int* tab;
static int (*draw_next)(void);
char KExx = 0;
int SQ_size_x = 20, SQ_size_y = 20;

#define min(x, y) (x < y ? x : y)

gboolean on_window_configure_event(GtkWidget * da, GdkEventConfigure * event, gpointer user_data){
    static int oldw = 0;
    static int oldh = 0;
    //make our selves a properly sized pixmap if our window has been resized
    if (oldw != event->width || oldh != event->height){
        //create our new pixmap with the correct size.
        GdkPixmap *tmppixmap = gdk_pixmap_new(da->window, event->width,  event->height, -1);
        //copy the contents of the old pixmap to the new pixmap.  This keeps ugly uninitialized
        //pixmaps from being painted upon resize
        int minw = oldw, minh = oldh;
        if( event->width < minw ){ minw =  event->width; }
        if( event->height < minh ){ minh =  event->height; }
        gdk_draw_drawable(tmppixmap, da->style->fg_gc[GTK_WIDGET_STATE(da)], pixmap, 0, 0, 0, 0, minw, minh);
        //we're done with our old pixmap, so we can get rid of it and replace it with our properly-sized one.u
        g_object_unref(pixmap);
        pixmap = tmppixmap;
    }
    oldw = event->width;
    oldh = event->height;
    return TRUE;
}

gboolean on_window_expose_event(GtkWidget * da, GdkEventExpose * event, gpointer user_data){
    gdk_draw_drawable(da->window,
        da->style->fg_gc[GTK_WIDGET_STATE(da)], pixmap,
        // Only copy the area that was exposed.
        event->area.x, event->area.y,
        event->area.x, event->area.y,
        event->area.width, event->area.height);
    return TRUE;
}


static int currently_drawing = 0;
//do_draw will be executed in a separate thread whenever we would like to update
//our animation
void *do_draw(void *ptr){

    currently_drawing = 1;

    int width, height;
///    gdk_threads_enter();
    gdk_drawable_get_size(pixmap, &width, &height);
    draw_next();
//    gdk_threads_leave(); 
    SQ_size_y = height / TAB_N;
    SQ_size_x = width / TAB_M;

    //create a gtk-independant surface to draw on
    cairo_surface_t *cst = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(cst);
    //do some time-consuming drawing
    for(int i = 0; i < TAB_N; i++) 
      for(int j = 0; j < TAB_M; j++)
        switch(tab[i * TAB_M + j]) {
          case 0: // pacman
            cairo_set_source_rgb(cr, 0,0,0);
            cairo_set_line_width(cr, 1);

            cairo_rectangle(cr, SQ_size_y * j, SQ_size_x * i, SQ_size_y, SQ_size_x);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
            cairo_set_source_rgb(cr, 255,255,0);
            cairo_set_line_width(cr, 1);

            cairo_arc(cr, SQ_size_y / 2 + j * SQ_size_y, SQ_size_x / 2 + i * SQ_size_x, min(SQ_size_x, SQ_size_y) / 2, 0, 2*M_PI);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
              
            break;
          case 1: // free
            cairo_set_source_rgb(cr, 0,0,0);
            cairo_set_line_width(cr, 1);

            cairo_rectangle(cr, SQ_size_y * j, SQ_size_x * i, SQ_size_y, SQ_size_x);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
            break;
          case 2: //wall
            cairo_set_source_rgb(cr, 0,0,255);
            cairo_set_line_width(cr, 1);

            cairo_rectangle(cr, SQ_size_y * j, SQ_size_x * i, SQ_size_y, SQ_size_x);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
            break;
          case 3: //pear
            cairo_set_source_rgb(cr, 0,0,0);
            cairo_set_line_width(cr, 1);

            cairo_rectangle(cr, SQ_size_y * j, SQ_size_x * i, SQ_size_y, SQ_size_x);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
            cairo_set_source_rgb(cr, 255,0,0);
            cairo_set_line_width(cr, 1);

            cairo_arc(cr, SQ_size_y / 2 + j * SQ_size_y, SQ_size_x / 2 + i * SQ_size_x, min(SQ_size_x, SQ_size_y) / 3, 0, 2*M_PI);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
            break;
        }
    cairo_destroy(cr);


    //When dealing with gdkPixmap's, we need to make sure not to
    //access them from outside gtk_main().
//    gdk_threads_enter();

    cairo_t *cr_pixmap = gdk_cairo_create(pixmap);
    cairo_set_source_surface (cr_pixmap, cst, 0, 0);
    cairo_paint(cr_pixmap);
    cairo_destroy(cr_pixmap);

//    gdk_threads_leave();

    cairo_surface_destroy(cst);

    currently_drawing = 0;

    return NULL;
}

gboolean keypress_function (GtkWidget *window, GdkEventKey *event, gpointer data) {
      if (event->keyval == GDK_w || event->keyval == GDK_a || GDK_s == event->keyval || GDK_d == event->keyval) {
        switch(event->keyval) {
          case GDK_w:
            KExx = 'w';
            break;
          case GDK_a:
            KExx = 'a';
            break;
          case GDK_s:
            KExx = 's';
            break;
          case GDK_d:
            KExx = 'd';
            break;
        }
        do_draw(NULL);
        int width, height;
        gdk_drawable_get_size(pixmap, &width, &height);
        gtk_widget_queue_draw_area(window, 0, 0, width, height);
        return TRUE;

      }
      return FALSE;
}

gboolean timer_exe(GtkWidget * window){

    static gboolean first_execution = TRUE;

    //use a safe function to get the value of currently_drawing so
    //we don't run into the usual multithreading issues
    int drawing_status = g_atomic_int_get(&currently_drawing);

    //if we are not currently drawing anything, launch a thread to
    //update our pixmap
    if(drawing_status == 0){
//        static pthread_t thread_info;
        int  iret;
        if(first_execution != TRUE){
//            pthread_join(thread_info, NULL);
        }
        do_draw(NULL);
//        iret = pthread_create( &thread_info, NULL, do_draw, NULL);
    }

    //tell our window it is time to draw our animation.
    int width, height;
    gdk_drawable_get_size(pixmap, &width, &height);
    gtk_widget_queue_draw_area(window, 0, 0, width, height);

    first_execution = FALSE;

    return TRUE;

}

/*int main (int argc, char *argv[]){
  
    scanf("%d %d", &TAB_N, &TAB_M);
    tab = (int*) malloc(sizeof(int) * TAB_N * TAB_M + 6.66);
    for(int i = 0; i < TAB_N; i++)
      for(int j = 0; j < TAB_M; j++) scanf("%d", tab + i * TAB_M + j);
    draw_next = draw;
*/
void graphics_init_ui(int argc, char*argv[], void (*UPD)(), int N, int M, int* tabb, int KEY) {
    tab = tabb;
    TAB_M = M;
    TAB_N = N;
    draw_next = UPD;
    //we need to initialize all these functions so that gtk knows
    //to be thread-aware
//    if (!g_thread_supported ()){ g_thread_init(NULL); }
//    gdk_threads_init();
//    gdk_threads_enter();

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), TAB_M * 50, TAB_N * 50);
//    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(on_window_expose_event), NULL);
    g_signal_connect(G_OBJECT(window), "configure_event", G_CALLBACK(on_window_configure_event), NULL);

    //this must be done before we define our pixmap so that it can reference
    //the colour depth and such
    gtk_widget_show_all(window);

    //set up our pixmap so it is ready for drawing
    pixmap = gdk_pixmap_new(window->window,50 * TAB_M, 50 * TAB_N,-1);
    //because we will be painting our pixmap manually during expose events
    //we can turn off gtk's automatic painting and double buffering routines.
    gtk_widget_set_app_paintable(window, TRUE);
    gtk_widget_set_double_buffered(window, FALSE);

    if(KEY) {
      gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
      g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (keypress_function), NULL);
      do_draw(NULL);
    int width, height;
    gdk_drawable_get_size(pixmap, &width, &height);
    gtk_widget_queue_draw_area(window, 0, 0, width, height);

    } else {
      (void)g_timeout_add(100, (GSourceFunc)timer_exe, window);
    }


    gtk_main();
//    gdk_threads_leave();

    return 0;
}
