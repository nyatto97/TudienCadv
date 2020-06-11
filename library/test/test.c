#include <gtk/gtk.h>
#include <stdlib.h>

gboolean func(GtkEntryCompletion *completion,
              const gchar *key,
              GtkTreeIter *iter,
              gpointer user_data) {
    GtkTreeModel *model = gtk_entry_completion_get_model(completion);
    gchar *item;
    gtk_tree_model_get(model, iter, 0, &item, -1);
    gboolean ans = (atoi(key) % 2 == atoi(item) % 2);
    g_free(item);
    return ans;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *entry = gtk_entry_new();
    GtkEntryCompletion *completion = gtk_entry_completion_new();
    GtkListStore *ls = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;
    int i;
    char buf[20];
    for(i=0; i<100; i++) {
        gtk_list_store_append(ls, &iter);
        sprintf(buf, "%d", i);
        gtk_list_store_set(ls, &iter, 0, buf, -1);
    }
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(ls));
    gtk_entry_completion_set_match_func(completion, (GtkEntryCompletionMatchFunc)func, NULL, NULL);
    gtk_entry_set_completion(GTK_ENTRY(entry), completion);
    gtk_entry_completion_set_text_column(completion, 0);
    gtk_container_add(GTK_CONTAINER(window), entry);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
// gcc `pkg-config --cflags gtk+-3.0` -o test test.c `pkg-config --libs gtk+-3.0` libbt.a -w