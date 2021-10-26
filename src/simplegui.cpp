#include "../include/simplegui.hpp"
#include <iostream>
#include <cstdlib>
#include <gtk/gtk.h>

using namespace std;

char *romfile;

static void on_file_chosen(GtkFileChooserButton* btn, gpointer data) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
    cout << "Filename: " << filename << endl;
    romfile = filename;
}

static void on_quitbutton_clicked(GtkButton* quitbtn, gpointer data) {
    gtk_main_quit();
    exit(0);
}

static void on_srbutton_clicked(GtkButton* quitbtn, gpointer data) {
    if (!romfile) return;
    else {
        gtk_main_quit();
        cout << "Starting ROM " << romfile << endl;
    }
}

int start_form(int argc, char **argv) {
    gtk_init(&argc, &argv);
    
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CHIP8 ROM SELECT");
    // gtk_window_set_default_size(GTK_WINDOW(window), 640, 320);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* chooserom_label = gtk_label_new_with_mnemonic("Pick a CHIP8 ROM file: ");

    GtkWidget* btn = gtk_file_chooser_button_new("Open ROM", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(btn), "/home/anantraina/");
    g_signal_connect(GTK_FILE_CHOOSER_BUTTON(btn), "file-set", G_CALLBACK(on_file_chosen), NULL);
    
    GtkWidget* srbtn = gtk_button_new_with_label("Start ROM");
    g_signal_connect(GTK_BUTTON(srbtn), "clicked", G_CALLBACK(on_srbutton_clicked), NULL);

    GtkWidget* quitbtn = gtk_button_new_with_label("Quit");
    g_signal_connect(GTK_BUTTON(quitbtn), "clicked", G_CALLBACK(on_quitbutton_clicked), NULL);

    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(hbox), chooserom_label);
    gtk_container_add(GTK_CONTAINER(hbox), btn);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_container_add(GTK_CONTAINER(vbox), srbtn);
    gtk_container_add(GTK_CONTAINER(vbox), quitbtn);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();

    cout << "Ended main" << endl;

    return 0;
}

/* int main (int argc, char **argv) {
    int err = start_form(argc, argv);
    cout << "Complete with return " << err << endl;
    return 0;
} */