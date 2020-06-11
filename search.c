gboolean check_search(GtkWidget *widget,gpointer data){
	int rsize,rsize2;
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	char thongBao[MAXLEN_MEAN];
	char goiy[MAXLEN_WORD];
	char temp[MAXLEN_WORD];
	strcpy(word,(char *)gtk_entry_get_text(GTK_ENTRY(widget)));
	xoakhoangtrangcuoichuoi(word);
	if(strlen(word) == 0){
		strcpy(thongBao,"Bạn chưa nhập từ vào ô tìm kiếm!\n");
	}
	else{
		xoakhoangtrangcuoichuoi(word);
		if(btsel(dic,word,mean,MAXLEN_MEAN*sizeof(char),&rsize)!=0){
			strcpy(thongBao,"\nXin lỗi! Không tìm thấy\n");
			soundEx(goiy,word,MAXLEN_WORD,1);
			if(btsel(sou,goiy,temp, MAXLEN_WORD*sizeof(char), &rsize2)==0) {
				strcat(thongBao, "Có phải bạn muốn tìm:");
				strcat(thongBao,temp );
			}
		} 
		else{
			strcpy(thongBao,mean);
			strcat(thongBao,"\n");
		} 
	}
	gtk_text_buffer_set_text(buffer,"", -1);
	gtk_text_buffer_get_iter_at_offset(buffer, &iter2,-1);
	gtk_text_buffer_insert_with_tags_by_name (buffer, &iter2, thongBao, -1, "italic","lmarg",  NULL);
	return TRUE;
}
void window_Search(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *search_btn_search;
	GtkWidget *separtor;
	GtkWidget *label_kq;
	GtkWidget *input_search;

	GtkWidget *scrolledwindow;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/giaodien.glade", NULL);
	window_search = GTK_WIDGET(gtk_builder_get_object(builder, "window_search"));
	gtk_window_set_title (GTK_WINDOW (window_search), "Search");
	search_btn_search = GTK_WIDGET(gtk_builder_get_object(builder, "search_btn_search"));
	input_search = GTK_WIDGET(gtk_builder_get_object(builder, "input_search"));
	separtor = GTK_WIDGET(gtk_builder_get_object(builder, "separtor"));



	// text views
	show_search = GTK_WIDGET(gtk_builder_get_object(builder, "show_search"));
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(show_search),GTK_WRAP_WORD);

	label_kq = GTK_WIDGET(gtk_builder_get_object(builder, "label_kq"));
	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(show_search));

	scrolledwindow = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow"));


	gtk_text_buffer_create_tag(buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
	gtk_text_buffer_create_tag(buffer, "lmarg", "left_margin", 5, NULL);


	completion=gtk_entry_completion_new();
	gtk_entry_completion_set_text_column(completion,0);
	GtkListStore *liststore=gtk_list_store_new(5,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));

	gtk_entry_set_completion(GTK_ENTRY(input_search),completion);

	g_signal_connect(G_OBJECT(input_search),"key-press-event",G_CALLBACK(searchword),liststore);
  	g_signal_connect_after( G_OBJECT(input_search), "insert-text", G_CALLBACK(name_insert_after), liststore );
  	g_signal_connect_after( G_OBJECT(input_search), "delete-text", G_CALLBACK(name_delete_after), liststore );
	g_signal_connect_swapped(G_OBJECT(search_btn_search), "clicked", G_CALLBACK(check_search),input_search);
	gtk_widget_show_all(window_search);
}

