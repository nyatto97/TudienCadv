gboolean check_add(GtkWidget *widget,gpointer data){
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	int rsize;
	char mess[MAXLEN_MEAN];

	strcpy(word,(char*)gtk_entry_get_text(GTK_ENTRY(add_tu)));
	strcpy(mean,(char*)gtk_entry_get_text(GTK_ENTRY(add_nghia)));
	xoakhoangtrangcuoichuoi(word);
	if((strcmp(word,"")== 0) || (strcmp(mean,"")== 0)){
		strcpy(mess,"Bạn chưa nhập đầy đủ dữ liệu.");
	}
	else{
		if(btsel(dic,word,mean,MAXLEN_MEAN*sizeof(char),&rsize)==0){
			strcpy(mess,"Từ này đã tồn tại.");
		}
		else{
			if(btins(dic,word,mean,MAXLEN_MEAN*sizeof(char))){
				strcpy(mess,"Thêm từ không thành công - Tràn bộ nhớ");
			}
			else
				strcpy(mess,"Thêm từ thành công.");
		}
		
	}
	show_dialog(widget,data,mess);
	return TRUE;
}


void window_add(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *window_add;
	GtkWidget *btn_add2;
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/giaodien.glade", NULL);
	window_add = GTK_WIDGET(gtk_builder_get_object(builder, "window_add"));
	gtk_window_set_title (GTK_WINDOW (window_add), "Add");
	add_tu = GTK_WIDGET(gtk_builder_get_object(builder, "add_tu"));
	add_nghia = GTK_WIDGET(gtk_builder_get_object(builder, "add_nghia"));
	btn_add2 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add2"));
	g_signal_connect(G_OBJECT(btn_add2), "clicked", G_CALLBACK(check_add),NULL);
	gtk_widget_show_all(window_add);

}
