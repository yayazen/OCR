#include <gtk/gtk.h>
#include <stdio.h>


#include "main.h"
#include "train.h"
#include "proc.h"
#include "seg.h"

#define H   400
#define W   600

void on_MainWindow_destroy(void) {
    printf("Thanks, closing gui now ...\n");
    gtk_main_quit();
}



bool IMG_loaded = false;
char *file;
size_t len = 0;

void set_goal() {
    NN.goal = gtk_spin_button_get_value (GTK_SPIN_BUTTON (Spin));
    if (NN.goal == 0)
        NN.goal = 0.1;
}



void print_gui(GtkTextBuffer *buffer, const char *fmt, ...) {
    char buf[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
        
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);

    if (gtk_text_buffer_get_char_count (buffer))
        gtk_text_buffer_insert (buffer, &iter, "\n", 1);
    
    gtk_text_buffer_insert (buffer, &iter, buf, -1);
}

gchar *time_handler(void) {
    return g_date_time_format(g_date_time_new_now_local(), "%H:%M:%S");
}







/* NN */

void on_Save_clicked (void) {
    
    if (NN.loaded) {
        const gchar *file = gtk_entry_get_text (GTK_ENTRY (NNsave));
        save_training(file);
        print_gui(NN.buffer, "[Success] Network saved under \"%s\" ... ", file);
        
        NN.loaded = FALSE;
    }

    else
        print_gui(NN.buffer,"[Failed] - There is yet no network loaded ...");
}


void on_Clear_clicked (void) {
    gtk_text_buffer_set_text(NN.buffer, "", -1);
    gtk_text_view_set_buffer ( GTK_TEXT_VIEW (NN.view), NN.buffer);
}



void on_Load_clicked (void) {
    if (NN.event != 0)
        print_gui(NN.buffer, "[Failed] Wait for unfinished jobs to terminate !");
    else { 
        if (load_gui(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (NN.Select)), false) == 0)
            NN.loaded = TRUE;
    }
}






void eval(int train) {
    set_goal();
    NN.train = train;
    if (NN.event != 0)
        print_gui(NN.buffer, "[Failed] Wait for unfinished jobs to terminate !");

    else if (NN.loaded) {
        NN.event = gdk_threads_add_idle(train_gui, NULL);

        gtk_button_set_label (GTK_BUTTON(TrainButton), "INTERRUPT");
        if (train == 1)
            print_gui(NN.buffer, "[Started at %s] Training Network (GOAL : %f)", time_handler(), NN.goal);
    }

    else
        print_gui(NN.buffer,"[Failed] - There is yet no network loaded ...");
}


void on_Train_clicked (void) {
    if (NN.event != 0) {
        NN.train = 2;
    }
    else
        eval(1);
}

void on_Test_clicked (void) {
    eval(0);
}








/* IMG */

void on_IMGSelect_selection_changed (void) {
    file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (IMGSelect));  
    
    Pixbuf = gdk_pixbuf_new_from_file (file, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE (IMGShow), 
            gdk_pixbuf_scale_simple(Pixbuf, W, H, GDK_INTERP_NEAREST));
    
    if (Pixbuf == NULL) {
        print_gui(img_buffer, " %s \n [Failed] Cannot load from incompatible file type\n", file);
        return ;
    }

    IMG_loaded = true;
            
    gtk_widget_show (ToolProc);
    print_gui(img_buffer, " [Success] Load from location %s\n + Tools unlocked ...\n", file);
}

    

/* Proc Toggle */

void on_TogGray_toggled(void) {
    if (IMG_loaded) {
        grayscale(Pixbuf);
        gtk_image_set_from_pixbuf(GTK_IMAGE (IMGShow), 
            gdk_pixbuf_scale_simple(Pixbuf, W, H, GDK_INTERP_NEAREST));
    }
}

void on_TogBin_toggled(void) {
    if (IMG_loaded) {
        binarization(Pixbuf, 100);
        gtk_image_set_from_pixbuf(GTK_IMAGE (IMGShow), 
            gdk_pixbuf_scale_simple(Pixbuf, W, H, GDK_INTERP_NEAREST));
    }
}



void on_TogSeg_toggled(void) {
    GdkPixbuf *buf = Pixbuf;

    set = segmentation(buf, 28, 28, 100, &len);
    if (!set) {
        print_gui(img_buffer, " [Failed] Segmentation failure !\n");
    }

    GtkTreeIter it;
    print_gui(img_buffer, " [Success] Document has %ld different lines!\n", len);
    
    for (size_t i = 0; i < len; i++) {
        charSetObj *c = set[i]->head;

        size_t j = 0;
        while (c) {
            if (c->img) {
                char *s = calloc(100, sizeof(char));
                sprintf(s, "tmp/%03lu%03lu.bmp", i, j);
                
                gdk_pixbuf_save(c->img, s, "bmp", NULL, NULL);
                j++;

                gtk_list_store_append(store, &it);
                gtk_list_store_set (store, &it, 0, c->img,-1);
            }
            c = c->next;
        }
    }
}





/* Main iteration */

int main(int argc, char *argv [])
{
    
    /* Library load */
    gtk_init(&argc, &argv);
    
    data.builder = gtk_builder_new();

    /* glade/xml file */
    filename =  g_build_filename ("./gui/gui.glade", NULL);

    gtk_builder_add_from_file (data.builder, filename, &error);
    g_free (filename);

 
    if (error)
        {
          gint code = error->code;
          g_printerr("%s\n", error->message);
          g_error_free (error);
          return code;
        }

    
    /* Link signals to interface */
    gtk_builder_connect_signals (data.builder, &data);

    /* Load of widgets pointer - see header 
    Callback signals for button are managed by the xml file*/
    window = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));

    /*NN Widgets*/
    NN.Select = GTK_WIDGET(gtk_builder_get_object (data.builder, "NETSelect"));
    NN.fsave = GTK_WIDGET(gtk_builder_get_object (data.builder, "NNEntry"));
        
    NN.view = GTK_WIDGET(gtk_builder_get_object (data.builder, "textview1"));
    NN.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (NN.view));
        
    NN.loaded = FALSE;
    
    NNsave = GTK_WIDGET(gtk_builder_get_object (data.builder, "NNEntry")),
    
    Spin = GTK_WIDGET(gtk_builder_get_object (data.builder, "Epsilon"));    
    TrainButton = GTK_WIDGET(gtk_builder_get_object (data.builder, "Train"));

    /*IMG Widgets*/
    IMGSelect = GTK_WIDGET(gtk_builder_get_object (data.builder, "IMGSelect")); 
    IMGShow = GTK_WIDGET(gtk_builder_get_object (data.builder, "IMGview"));
    img_view = GTK_WIDGET(gtk_builder_get_object (data.builder, "textview0"));   
    img_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (img_view));

    ToolProc = GTK_WIDGET(gtk_builder_get_object (data.builder, "Tools"));

    IconView = GTK_ICON_VIEW(gtk_builder_get_object (data.builder, "IconView"));
    store = GTK_LIST_STORE(gtk_builder_get_object (data.builder, "Store1"));
    gtk_icon_view_set_pixbuf_column(IconView, 0);
    /* Selector */
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (NN.Select), "~");
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (IMGSelect), "~");

        
    g_signal_connect (window, "destroy", on_MainWindow_destroy, NULL);

    /* Main window activation */
    gtk_widget_show (window);


    gtk_main();
    
    return 0;
}
