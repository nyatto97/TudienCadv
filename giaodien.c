 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "library/inc/bt.h"
#include <time.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "library/data/soundex.h"

BTA *dic;
BTA *sou;

GtkWidget *tu;
GtkWidget *add_tu;
GtkWidget *nghia;
GtkWidget *add_nghia;
GtkListStore *store;
GtkTreeIter iter;
GtkWidget * completion;
GtkWidget *mainwindow;
/// thu muc chung cho cac ham
GtkWidget *show_search;
GtkWidget *show_delete;
GtkTextBuffer *buffer;
GtkWidget *window_search;
GtkTextIter iter2;
GtkWidget *window_delete_mess;
GtkWidget *window_change;

// thuc muc cho edit
GtkWidget *update_tu;
GtkWidget *update_nghia;

#define M 50
#define MAXLEN_WORD 150
#define MAXLEN_MEAN 200
#define BACKSP_LINE "\b \b"

#define FILE_READ "library/data/data3.txt"
#define FILE_WRITE "library/data/data3.txt"
#define NAME_SOUND_TREE "library/data/soutree"
#define NAME_DIC_TREE "library/data/mytree"


static struct termios orig_termios;
static int ttyfd = STDIN_FILENO;


#include "func.h"
#include "search.h"
#include "delete.h"
#include "add.h"
#include "dialog.h"
#include "edit.h"

typedef struct{
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
}data;
data listDic[3500];


int main(int argc, char** argv){

	// doc file 
	docfile();
	// giao dien chinh(MENU)
	GtkBuilder *builder;
	GtkWidget *window,*btn_search,*btn_add,*btn_delete,*btn_exit,*btn_ghifile,*btn_edit;
	GtkWidget *entry;
	GtkWidget *btn_help;
	GtkWidget *btn_info;
	gtk_init(&argc,&argv);
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/giaodien.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_window_set_icon_from_file (GTK_WINDOW(window),"library/data/icon/dictionary.png",NULL);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	btn_search = GTK_WIDGET(gtk_builder_get_object(builder, "btn_search"));
	btn_add = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add"));
	btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));
	btn_delete = GTK_WIDGET(gtk_builder_get_object(builder, "btn_delete"));
	entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry"));
	btn_ghifile = GTK_WIDGET(gtk_builder_get_object(builder, "btn_ghifile"));
	btn_help = GTK_WIDGET(gtk_builder_get_object(builder, "btn_help"));
	btn_info = GTK_WIDGET(gtk_builder_get_object(builder, "btn_info"));
	btn_edit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_edit"));

	mainwindow  = window;
	// Bat su kien  button
	g_signal_connect_swapped(G_OBJECT(btn_exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_search), "clicked", G_CALLBACK(window_Search), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_add), "clicked", G_CALLBACK(window_add), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_delete), "clicked", G_CALLBACK(window_delete), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_edit), "clicked", G_CALLBACK(window_edit), NULL);
	g_signal_connect(G_OBJECT(btn_ghifile), "clicked", G_CALLBACK(ghifile), NULL);
	g_signal_connect(G_OBJECT(btn_help), "clicked", G_CALLBACK(show_guide), NULL);
	g_signal_connect(G_OBJECT(btn_info), "clicked", G_CALLBACK(show_info), NULL);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

// gcc `pkg-config --cflags gtk+-3.0` -o giaodien giaodien.c `pkg-config --libs gtk+-3.0` libbt.a -w
