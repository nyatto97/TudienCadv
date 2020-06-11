
void xoakytu(char str[100],int vitri){
	int n=strlen(str);
	for(int i=vitri;i<n;i++){
		str[i]=str[i+1];
	}

}
void xoakhoangtrangcuoichuoi(char str[100]){
	int k=strlen(str)-1;
	while (str[k]==32){
		xoakytu(str,k);
		k=strlen(str)-1;
	}
}
void earse_character(char a[], char temp){
    int n= strlen(a); // cai ham nay cua borland C++  dùng để lấy độ dài của chuỗi.
     for(int i=0;i<n;i++){
         if(a[i]==temp){
            a[i] = ' ,';
          }
      }
}
// ham them tu vao dic
int addWordToDic(BTA *dic,char *word,char *mean){
	int rsize;
	if(btsel(dic,word,mean,(MAXLEN_MEAN)*sizeof(char),&rsize)==0) {
		return 0;
	} else {
		btins(dic,word,mean,(MAXLEN_MEAN)*sizeof(char));
	}
	return 1;
}
// ham them tu vao soud
int addWordToSou(BTA *sou,char *word,char *mean){
	int rsize;
	char sound[MAXLEN_WORD];
	if(btsel(sou,word,sound,MAXLEN_WORD*sizeof(char),&rsize)==0) {
		if(strstr(sound,mean)==NULL && (strlen(sound)+strlen(mean))<MAXLEN_WORD){
			strcat(sound,"\n");
			strcat(sound,mean);
			btupd(sou,word,sound,MAXLEN_WORD*sizeof(char));
		}
		return 0;
	} else {
		btins(sou,word,sound,MAXLEN_WORD*sizeof(char)); 
	}
	return 1;
}

void docfile(){
	// doc file
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	char sound[MAXLEN_WORD];

	FILE *f = fopen(FILE_READ,"r");
	btinit();
	sou = btopn(NAME_SOUND_TREE,0,0);  
	dic = btopn(NAME_DIC_TREE,0,0);

	if(dic == NULL)
		dic = btcrt(NAME_DIC_TREE,0,FALSE);
	if(sou == NULL)
		sou = btcrt(NAME_SOUND_TREE,0,FALSE);
	if((dic == NULL)|| (sou == NULL))
		exit(1);

	if(!f){
		printf("Cann't open file data\n");
		exit(1);
	}
	while(fscanf(f,"%[^\t]\t%[^\n]\n",word,mean)>0){
		xoakhoangtrangcuoichuoi(word);
		addWordToDic(dic,word,mean);
		soundEx(sound,word,150,1);
		addWordToSou(sou,sound,word);
		
	}
	fclose(f);	
}

gboolean ghifile(GtkWidget *widget,gpointer data){
	FILE *f=fopen(FILE_WRITE,"w");;
	char word[MAXLEN_WORD];
	char mean[MAXLEN_MEAN];
	int i,rsize;
	long j=0;
	i=bfndky(dic,"1",&rsize);
	if(i==0){
		j++;
	} 
	j=0;
	while(bnxtky(dic,word,&i)==0){
		btsel(dic,word,mean,MAXLEN_MEAN*sizeof(char),&rsize);
		if(j==0){
			j=1;
		}
		else{
			fprintf(f,"%s\t%s\n",word,mean);
		}
		j++;
	}
	fclose(f);
	show_dialog(widget,data,"Ghi file thành công!");
	return TRUE;

}

int isPrefix(char *s,char *word){
  int len1,len2,i;
  len1=strlen(s);
  len2=strlen(word);
  if (len1 > len2) return 0;
  for (i=0;i<len1;i++){
    if (s[i]!=word[i])
      return 0;
    }
  return 1;
}
gboolean searchword(GtkWidget *entryword,GdkEvent *event,gpointer listword){
  GdkEventKey *key=(GdkEventKey *)event;
  GtkListStore *liststore=(GtkListStore *)listword;
  GtkTreeIter iter;
  int count=0,len;
  BTint value;//bien phuc vu cho ham xu ly tab(su dung thu vien btree)
  char text[MAXLEN_WORD],near_word[MAXLEN_WORD];
  //ham xu ly dau Tab
  if (key->keyval==GDK_KEY_Tab){
    strcpy(text,(char *)gtk_entry_get_text(GTK_ENTRY(entryword)));
    if (bfndky(dic,text,&value)==QNOKEY){
      // ham bnxtky tim key gan nhat voi key muon tim
      bnxtky(dic,near_word,&value);
      if (isPrefix(text,near_word)){
        gtk_entry_set_text(GTK_ENTRY(entryword),near_word);
        gtk_editable_set_position(GTK_EDITABLE(entryword),strlen(near_word));
      }
      else return TRUE;
    }
    gtk_text_buffer_set_text(buffer,entryword, -1);
    return TRUE;
  }
  else{
    strcpy(text,(char *)gtk_entry_get_text(GTK_ENTRY(entryword)));
    bfndky(dic,text,&value);
    bnxtky(dic,near_word,&value);
    gtk_list_store_clear(liststore);
    while ((isPrefix(text,near_word)!=0) && (count < 15)){
      gtk_list_store_append(liststore,&iter);
      gtk_list_store_set(liststore,&iter,0,near_word,-1);
      bnxtky(dic,near_word,&value);
      count++;
    }
    return FALSE;
  }
}
void name_insert_after (GtkEditable* entryword,
                   gchar* new_text,
                   gint new_length,
                   gpointer position,
                   gpointer listword){  
  GtkListStore *liststore=(GtkListStore *)listword;
  GtkTreeIter iter;
  int count=0,len;
  BTint value;
  char text[MAXLEN_WORD],near_word[MAXLEN_WORD];  
  strcpy(text,(char *)gtk_entry_get_text(GTK_ENTRY(entryword)));
  bfndky(dic,text,&value);
  bnxtky(dic,near_word,&value);
  gtk_list_store_clear(liststore);
  while ((isPrefix(text,near_word)!=0) && (count < 15)){
    gtk_list_store_append(liststore,&iter);
    gtk_list_store_set(liststore,&iter,0,near_word,-1);
    bnxtky(dic,near_word,&value);
    count++;
  }  
}
void name_delete_after (GtkEditable* entryword,
                   gint start_pos,
                   gint end_pos,
                   gpointer listword){
  GtkListStore *liststore=(GtkListStore *)listword;
  GtkTreeIter iter;
  int count=0,len;
  BTint value;
  char text[MAXLEN_WORD],near_word[MAXLEN_WORD];  
  strcpy(text,(char *)gtk_entry_get_text(GTK_ENTRY(entryword)));
  bfndky(dic,text,&value);
  bnxtky(dic,near_word,&value);
  gtk_list_store_clear(liststore);
  while ((isPrefix(text,near_word)!=0) && (count < 15)){
    gtk_list_store_append(liststore,&iter);
    gtk_list_store_set(liststore,&iter,0,near_word,-1);
    bnxtky(dic,near_word,&value);
    count++;
  }
}
