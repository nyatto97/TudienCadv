gboolean check_edit(GtkWidget *widget,gpointer data){
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	int rsize;
	char mean_update[MAXLEN_MEAN];
	char mess[MAXLEN_MEAN];

    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)update_nghia);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    strcpy(mean_update,(char *) gtk_text_buffer_get_text(buffer, &start, &end, FALSE));

	strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(update_tu)));

	earse_character(mean_update,'\n');
	if((strcmp(word,"")== 0) || (strcmp(mean_update,"")== 0)){
		strcpy(mess,"Dữ liệu chỉnh sửa thiếu hoặc chưa đúng.");
	}
	else{
		if(btsel(dic,word,mean,MAXLEN_MEAN*sizeof(char),&rsize)==0){
			btupd(dic,word,mean_update,MAXLEN_MEAN*sizeof(char));
			strcpy(mess,"Chỉnh sửa từ thành công.");
		}
		else{
			strcpy(mess,"Từ này không tồn tại.");
		}

	}
	show_dialog(widget,data,mess);
	return TRUE;
}

gboolean cancel(GtkWidget * widget,gpointer data){
	gtk_widget_destroy(window_change);
	return TRUE;
}
void window_edit(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *btn_update;
	GtkWidget *btn_apply;
	GtkWidget *btn_close;
	GtkWidget *scrolledwindow2;


	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/giaodien.glade", NULL);
	window_change = GTK_WIDGET(gtk_builder_get_object(builder, "window_edit"));
	gtk_window_set_title (GTK_WINDOW (window_change), "Edit");

	label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	label2 = GTK_WIDGET(gtk_builder_get_object(builder, "label2"));
	update_tu = GTK_WIDGET(gtk_builder_get_object(builder, "update_tu"));
	update_nghia = GTK_WIDGET(gtk_builder_get_object(builder, "update_nghia"));
	btn_update = GTK_WIDGET(gtk_builder_get_object(builder, "btn_update"));
	btn_apply = GTK_WIDGET(gtk_builder_get_object(builder, "btn_apply"));
	btn_close = GTK_WIDGET(gtk_builder_get_object(builder, "btn_close"));
	
	// text views
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(update_nghia),GTK_WRAP_WORD);

	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(update_nghia));
	scrolledwindow2 = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow"));


	gtk_text_buffer_create_tag(buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
	gtk_text_buffer_create_tag(buffer, "lmarg", "left_margin", 5, NULL);

	completion=gtk_entry_completion_new();
	gtk_entry_completion_set_text_column(completion,0);
	GtkListStore *liststore=gtk_list_store_new(5,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));

	gtk_entry_set_completion(GTK_ENTRY(update_tu),completion);

	g_signal_connect(G_OBJECT(update_tu),"key-press-event",G_CALLBACK(searchword),liststore);
	g_signal_connect_after( G_OBJECT(update_tu), "insert-text", G_CALLBACK(name_insert_after), liststore );
	g_signal_connect_after( G_OBJECT(update_tu), "delete-text", G_CALLBACK(name_delete_after), liststore );


	g_signal_connect_swapped(G_OBJECT(btn_close), "clicked", G_CALLBACK(cancel),NULL);
	g_signal_connect_swapped(G_OBJECT(btn_apply), "clicked", G_CALLBACK(check_edit),NULL);
	g_signal_connect_swapped(G_OBJECT(btn_update), "clicked", G_CALLBACK(check_search),update_tu);
	gtk_widget_show_all(window_change);

}

