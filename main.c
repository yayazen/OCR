#include <gtk/gtk.h>
#include <stdio.h>
#include "main.h"
#include "train.h"

bool load = false;

int main(int argc, char *argv [])
{
        
	/* Initialisation de la bibliothèque Gtk. */    
	gtk_init(&argc, &argv);
	
	/* Ouverture du fichier Glade de la fenêtre principale */
	data.builder = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier test.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
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
    
	/* Affectation des signaux de l'interface aux différents CallBacks. */
    gtk_builder_connect_signals (data.builder, &data);

    /* Récupération des pointeurs*/
    window = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));

    FileSelect = GTK_WIDGET(gtk_builder_get_object (data.builder, "FileSelect"));	
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (FileSelect), "~");

	view = GTK_WIDGET(gtk_builder_get_object (data.builder, "textview"));	
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	
    button0 = GTK_WIDGET(gtk_builder_get_object (data.builder, "button0"));
    
	/* Affichage de la fenêtre principale. */
    gtk_widget_show_all (window);

    gtk_main();
	
    return 0;
	}


void on_Save_clicked (void) {
	if (load) {
		save_training();
		print_gui("\"gui_save.nd\" network saved ... ");
	}
	else {
		print_gui("No network loaded yet ...");
	}

}

void on_Clear_clicked (void) {
	gtk_text_buffer_set_text(buffer, "", -1);
	gtk_text_view_set_buffer ( GTK_TEXT_VIEW (view), buffer);
}
void on_button0_clicked (void) {
	if (load)
		train_gui(0.001, false);
}

void on_button1_clicked (void) {
	if (load)
		train_gui(0.01, true);
}

void on_button2_clicked (void) {
	load_gui(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (FileSelect)));
	load = true;
}
