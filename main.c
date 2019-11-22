#include <gtk/gtk.h>
#include <stdio.h>
#include "main.h"
#include "train.h"

bool load = false;

void on_MainWindow_destroy(void) {
	printf("Thanks, closing gui now ...\n");
	gtk_widget_destroy(window);
	gtk_main_quit();
}

void on_Save_clicked (void) {
	
	if (load) {
		save_training();
		print_gui("Network saved under \"save.nd\" ... ");
		
		load = false;
	}

	else
		print_gui("[Failed] - There is yet no network loaded ...");
}


void on_Clear_clicked (void) {
	gtk_text_buffer_set_text(buffer, "", -1);
	gtk_text_view_set_buffer ( GTK_TEXT_VIEW (view), buffer);
}


void on_Load_clicked (void) {
	load_gui(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (FileSelect)), false);
	load = true;
}


void on_Train_clicked (void) {
	if (load)
		train_gui(false);
}

void on_Test_clicked (void) {
	if (load)
		train_gui(true);
}





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

    drawer = GTK_WIDGET(gtk_builder_get_object (data.builder, "drawer"));	
    
	FileSelect = GTK_WIDGET(gtk_builder_get_object (data.builder, "FileSelect"));	
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (FileSelect), "~");

	view = GTK_WIDGET(gtk_builder_get_object (data.builder, "textview"));	
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	

		
	g_signal_connect (window, "destroy", on_MainWindow_destroy, NULL);

	/* Main window activation */
    gtk_widget_show_all (window);


    gtk_main();
	
    return 0;
}
