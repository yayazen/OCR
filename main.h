#ifndef GUI_H
#define GUI_H

#include "proc.h"

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

typedef struct {
    GtkWidget   *Select;

    GtkWidget   *view;
    GtkTextBuffer   *buffer;
    
    gboolean    loaded;
    float       goal;
    guint       event, train;
} Itf;

Itf NN;
GtkWidget       *TrainButton;

GtkWidget       *window;

GtkWidget       *drawer;

GtkWidget       *ToolProc;
GtkWidget       *IMGSelect;
GtkWidget       *IMGview;
GdkPixbuf       *Pixbuf;
IMGData         img;

GtkWidget       *img_view;

GtkTextBuffer   *img_buffer;

GtkTextMark     *mark;
GtkTextIter     iter;

GtkWidget       *button0;
GtkWidget       *Spin;

SGlobalData data;        
GError *error;        
gchar *filename;


void print_gui(GtkTextBuffer *buffer, const char *fmt, ...);
gchar *time_handler(void);
#endif
