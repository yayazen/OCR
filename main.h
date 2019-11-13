#ifndef GUI_H
#define	GUI_H

typedef struct
      {
        GtkBuilder *builder;
        gpointer user_data;
      } SGlobalData;


GtkWidget 		*window;

GtkWidget		*FileSelect;

GtkWidget		*view;
GtkTextBuffer	*buffer;
GtkTextMark		*mark;
GtkTextIter		iter;

GtkWidget 		*button0;

SGlobalData data;        
GError *error;        
gchar *filename;




#endif
