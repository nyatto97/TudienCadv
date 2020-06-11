#include "func.c"

// xoa dau cach
void xoakytu(char str[100],int vitri);
void xoakhoangtrangcuoichuoi(char str[100]);
int addWordToDic(BTA *dic,char *word,char *mean);
int addWordToSou(BTA *sou,char *word,char *mean);
static GtkEntryCompletion* create_completion_widget(void);
void docfile();
gboolean ghifile(GtkWidget *widget,gpointer data);
int isPrefix(char *s,char *word);
gboolean searchword(GtkWidget *entryword,GdkEvent *event,gpointer listword);
void name_insert_after (GtkEditable* entryword,gchar* new_text,gint new_length,gpointer position,gpointer listword);
void name_delete_after (GtkEditable* entryword,gint start_pos,gint end_pos,gpointer listword);
