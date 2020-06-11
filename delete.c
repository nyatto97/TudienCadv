gboolean delete_word(GtkWidget *widget,gpointer data){
	char word[MAXLEN_WORD];
	strcpy(word,(char *)gtk_label_get_text(GTK_LABEL(widget)));
	if( btdel(dic,word) && btdel(sou,word)){
		show_dialog(widget,data,"Không tồn tại từ này\n");
		return FALSE;
	}
	else{
		gtk_widget_destroy(window_delete_mess);
		show_dialog(widget,data,"Đã xoá thành công\n");
		return TRUE;
	}	
}

void delete_message(char *w_delete){ 
	GtkBuilder *builder;
	GtkWidget *label_tuxoa;
	GtkWidget *label_thongbao;
	GtkWidget *yes_delete;
	GtkWidget *no_delete;
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	int rsize;

	int result;
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/giaodien.glade", NULL);
	window_delete_mess = GTK_WIDGET(gtk_builder_get_object(builder, "window_delete_mess"));
	gtk_window_set_title (GTK_WINDOW (window_delete_mess), "Delete");

	label_tuxoa = GTK_WIDGET(gtk_builder_get_object(builder, "label_tuxoa"));
	label_thongbao = GTK_WIDGET(gtk_builder_get_object(builder, "label_thongbao"));
	yes_delete = GTK_WIDGET(gtk_builder_get_object(builder, "yes_delete"));
	no_delete = GTK_WIDGET(gtk_builder_get_object(builder, "no_delete"));

	gtk_label_set_text(GTK_LABEL(label_tuxoa),w_delete);
	gtk_label_set_text(GTK_LABEL(label_thongbao),"Bạn thực sự muốn xoá từ này!");	


	// g_signal_connect_swapped(G_OBJECT(yes_delete), "clicked", G_CALLBACK(check_delete),search);
	g_signal_connect_swapped(G_OBJECT(no_delete), "clicked", G_CALLBACK(gtk_widget_destroy),window_delete_mess);
	result = g_signal_connect_swapped(G_OBJECT(yes_delete), "clicked", G_CALLBACK(delete_word),label_tuxoa);

	gtk_widget_show_all(window_delete_mess);
}

gboolean check_delete(GtkWidget *widget,gpointer data){
	int rsize,rsize2;
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	char thongBao[MAXLEN_MEAN];
	char goiy[MAXLEN_WORD];
	char temp[MAXLEN_WORD];
	strcpy(word,(char *)gtk_entry_get_text(GTK_ENTRY(widget)));
	xoakhoangtrangcuoichuoi(word);
	if(strlen(word) == 0){
		strcpy(thongBao,"Bạn chưa nhập từ vào ô xoá!\n");
	}
	else{
		xoakhoangtrangcuoichuoi(word);
		if(btsel(dic,word,mean,MAXLEN_MEAN*sizeof(char),&rsize)!=0){
			strcpy(thongBao,"\nXin lỗi! Không tìm thấy từ cần xoá!\n");
			soundEx(goiy,word,MAXLEN_WORD,1);
			if(btsel(sou,goiy,temp, MAXLEN_WORD*sizeof(char), &rsize2)==0) {
				strcat(thongBao, "Có phải bạn muốn tìm:");
				strcat(thongBao,temp );
			}
		} 
		else{
			delete_message(word);
		} 
	}

	gtk_text_buffer_set_text(buffer,"", -1);
	gtk_text_buffer_get_iter_at_offset(buffer, &iter2,-1);
	gtk_text_buffer_insert_with_tags_by_name (buffer, &iter2, thongBao, -1, "italic","lmarg",  NULL);
	return TRUE;
}

void window_delete(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *window_delete;
	GtkWidget *search,*delete;
	GtkWidget *scrolledwindow1;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/giaodien.glade", NULL);
	window_delete = GTK_WIDGET(gtk_builder_get_object(builder, "window_delete"));
	gtk_window_set_title (GTK_WINDOW (window_delete), "Delete");
	
	search = GTK_WIDGET(gtk_builder_get_object(builder, "search_delete"));
	delete = GTK_WIDGET(gtk_builder_get_object(builder, "btn_delete2"));

	show_delete = GTK_WIDGET(gtk_builder_get_object(builder, "show_delete"));
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(show_delete),GTK_WRAP_WORD);

	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(show_delete));
	scrolledwindow1 = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow"));

	gtk_text_buffer_create_tag(buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
	gtk_text_buffer_create_tag(buffer, "lmarg", "left_margin", 5, NULL);



	completion=gtk_entry_completion_new();
	gtk_entry_completion_set_text_column(completion,0);
	GtkListStore *liststore=gtk_list_store_new(5,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));

	gtk_entry_set_completion(GTK_ENTRY(search),completion);


	g_signal_connect(G_OBJECT(search),"key-press-event",G_CALLBACK(searchword),liststore);
	g_signal_connect_after( G_OBJECT(search), "insert-text", G_CALLBACK(name_insert_after), liststore );
	g_signal_connect_after( G_OBJECT(search), "delete-text", G_CALLBACK(name_delete_after), liststore );


	g_signal_connect_swapped(G_OBJECT(delete), "clicked", G_CALLBACK(check_delete),search);
	gtk_widget_show_all(window_delete);
}
