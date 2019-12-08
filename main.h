#ifndef GUI_H
#define GUI_H

#include "seg.h"
#include "proc.h"

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

typedef struct {
    GtkWidget   *Select;
    GtkWidget   *fsave;

    GtkWidget       *view;
    GtkTextBuffer   *buffer;
    
    gboolean    loaded;
    float       goal;
    guint       event, train;
} Itf;

/* net */
Itf NN;
GtkWidget       *NNsave;
GtkWidget       *Textsave;

GtkWidget       *window;

GtkImage        *Stock;
/* image */
GtkWidget       *ToolProc;
GtkWidget       *IMGSelect;
GtkWidget       *IMGShow;
GdkPixbuf       *Pixbuf;

charSet         **set;
GtkIconView       *IconView;
GtkListStore   *store;

/* print */
GtkWidget       *img_view;
GtkTextBuffer   *img_buffer;

GtkTextMark     *mark;
GtkTextIter     iter;

/* button */
GtkWidget       *button0;
GtkWidget       *Spin;
GtkWidget       *TrainButton;

/* init */
SGlobalData data;        
GError *error;        
gchar *filename;


void print_gui(GtkTextBuffer *buffer, const char *fmt, ...);
gchar *time_handler(void);
#endif
