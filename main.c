#include <gtk/gtk.h>
#include <stdio.h>


#include "main.h"
#include "train.h"
#include "proc.h"

bool IMG_loaded = false;

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


void on_MainWindow_destroy(void) {
    printf("Thanks, closing gui now ...\n");
    gtk_main_quit();
}

/* NN */

void on_Save_clicked (void) {
    
    if (NN.loaded) {
        save_training();
        print_gui(NN.buffer, "Network saved under \"save.nd\" ... ");
        
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
        load_gui(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (NN.Select)), false);
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
    const char *file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (IMGSelect));  
    
    Pixbuf = gdk_pixbuf_new_from_file_at_size (file, 800, 900, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE (IMGview), Pixbuf);
    
    if (Pixbuf == NULL) {
        print_gui(img_buffer, "%s \n[Failed] Cannot load from incompatible file type\n", file);
        return ;
    }

    img = (IMGData) {
        .pixels = gdk_pixbuf_get_pixels (Pixbuf),
        
        .width = gdk_pixbuf_get_width (Pixbuf),
        .height = gdk_pixbuf_get_height (Pixbuf),

        .rowstride = gdk_pixbuf_get_rowstride (Pixbuf),
        .n_chan = gdk_pixbuf_get_n_channels (Pixbuf)
    };

    IMG_loaded = true;
            
    gtk_widget_show (ToolProc);
    print_gui(img_buffer, "%s \nImg loaded, tools unlocked ...\n", file);
}


void on_TogGray_toggled(void) {
    if (IMG_loaded) {
        grayscale(&img);
        gtk_image_set_from_pixbuf(GTK_IMAGE (IMGview), Pixbuf); 
    }
}

void on_TogBin_toggled(void) {
    if (IMG_loaded) {
        binarize(&img);
        gtk_image_set_from_pixbuf(GTK_IMAGE (IMGview), Pixbuf); 
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
    NN.view = GTK_WIDGET(gtk_builder_get_object (data.builder, "textview1"));   
    NN.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (NN.view));
    NN.loaded = FALSE;

    Spin = GTK_WIDGET(gtk_builder_get_object (data.builder, "Epsilon"));    
    TrainButton = GTK_WIDGET(gtk_builder_get_object (data.builder, "Train"));

    /*IMG Widgets*/
    IMGSelect = GTK_WIDGET(gtk_builder_get_object (data.builder, "IMGSelect")); 
    IMGview = GTK_WIDGET(gtk_builder_get_object (data.builder, "IMGview"));
    img_view = GTK_WIDGET(gtk_builder_get_object (data.builder, "textview0"));   
    img_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (img_view));

    ToolProc = GTK_WIDGET(gtk_builder_get_object (data.builder, "Tools"));
    
    /* Selector */
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (NN.Select), "~");
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (IMGSelect), "~");

        
    g_signal_connect (window, "destroy", on_MainWindow_destroy, NULL);
    
    /* Main window activation */
    gtk_widget_show (window);


    gtk_main();
    
    return 0;
}
