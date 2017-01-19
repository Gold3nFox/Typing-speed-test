#include <gtk/gtk.h>
#include <stdio.h>
#include "string.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
struct linkedlist * list_of_words;
char *player_name;
char * scoreboard_toshow[10];
bool started=0;
bool startchecker =0;
int wordnum = 0;
int level_is_passed_or_not[10];
int all_chars = 0;
int word_wrong_chars = 0;
int level = 1;
int maxlevel= 1;
int max_saved_level = 0;
float score = 0,levelscore = 0,player_score = 0;
GtkHButtonBox * buttonbox;
GtkDialog *dialog,*dialog_playername;
GtkBuilder *builder, *builder_dialog,*builder_dialog_scoreboard,*start_builder;
GtkWidget *entry2,*button11,*button12,*button13,*label1,*label2,*label4,*label5,*label6,*label7,*label8,*label9,*label10,*label11,*label12,*label13,*label14,*label15,*label16,*messagedialog_scoreboard,*statusbar1,*dialog_newlevel,*button1,*button2,*button3,*button4,*button5,*button6,*button7,*button8,*button9,*button10;
clock_t start_t, end_t, total_t;
// chon nemitonam ro xml bishtar az ye object bedam be eventa baranke dasresi dashte basham global bayad bokonam
// albate bishtar az ye objectam shayad beshe man chizi peyda nakardam
struct linkedlist
{
    char * value;
    struct linkedlist * next;
};

struct scoreboard
{
    char * name;
    float * score;
};

void structswapper(struct scoreboard * one,struct scoreboard * two)
{
    struct scoreboard tmp;
    tmp.name = one -> name;
    tmp.score = one -> score;
    one -> name = two -> name;
    one -> score = two -> score;
    two -> name = tmp.name;
    two -> score = tmp.score;
}
void addend(struct linkedlist **list,char * word, int length)
{
    struct linkedlist *current;
    struct linkedlist *list_of_words;
    current = *list;
    length++;
    if (current == NULL)
    {
        *list = (struct linkedlist *)malloc(sizeof(struct linkedlist));
        ((struct linkedlist*)*list) -> value = (char *)malloc(sizeof(length * sizeof(char)));
        strncpy(((struct linkedlist*)*list) -> value , word, length);
        ((struct linkedlist*)*list) -> next = NULL;
        return;
    }
    while(current->next !=NULL)
    {
        current = current -> next;
    }
    list_of_words = (struct linkedlist *)malloc(sizeof(struct linkedlist));
    list_of_words -> value = (char *)malloc(sizeof(length * sizeof(char)));
    current -> next = list_of_words;
    list_of_words -> next = NULL;
    strncpy(list_of_words -> value , word, length);
}

int savedornot(char *esm)
{
    int i = 0;
    FILE *fp;
    int player_level = 0;
    int player_highest_lvl = 1;
    char * names_in_file;
    names_in_file = (char *)malloc(20 * sizeof(char));
    if (!(fp = fopen("save.txt", "rt"))) {
        printf("Couldn't open that level please choose another level\n");
        fclose(fp);
        return -1;            
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s %d", names_in_file, &player_level);
        if(strcmp(esm, names_in_file) == 0)
        {
        	printf("%s  %d\n", names_in_file, player_level);
            if(player_level > player_highest_lvl) player_highest_lvl = player_level;
        }
    }
    fclose(fp);
    return player_highest_lvl;
}


int save(char *esm,int level)
{
	int player_levels;
    FILE *fp;
    char * names_in_file;
    names_in_file = (char *)malloc(20 * sizeof(char));
    if (!(fp = fopen("save.txt", "rt"))) {
        printf("Failed to open FILE\n");
        return -1;
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s %d", names_in_file, &player_levels);
        if(strcmp(esm, names_in_file) == 0)
        {
            if(player_levels >= maxlevel)  return savedornot(esm);
        }
    }
    fclose(fp);
    if (!(fp = fopen("save.txt", "a+"))) {
        printf("Failed to open FILE\n");
        return -1;
    }
    fprintf(fp, "%s %d",esm,level);
    fprintf(fp,"\n");
    fclose(fp);
    return savedornot(esm);
}

int levelopener(int a,struct linkedlist ** list_of_words)
{
    wordnum = 0;
    int i = 0;
    FILE *fp;
    char c;
    char * full_level_file_name;
    full_level_file_name = (char *)malloc(13*sizeof(char));
    char *levelname = "level-";
    char *extention = ".txt";
    char *levelnumber =(char *)malloc(2*sizeof(char));
    sprintf(levelnumber, "%d", a);
    printf("%s====%s======%s\n",levelname, levelnumber,extention);
    strncpy(full_level_file_name,levelname,7);
    strcat(full_level_file_name,levelnumber);
    strcat(full_level_file_name,extention);
    char word[20];
    if (!(fp = fopen(full_level_file_name, "rt"))) {
        while(!(fp = fopen(full_level_file_name, "rt")))
        {
            sleep(1);
        }
    }
   while ((c = fgetc(fp)) != EOF) {
        if (c!=' ')
        {
            printf("Char: %c\n", c);
            word[i] = c;
            i++;
        }else if (c == ' ')
        {
            wordnum ++;
            word[i] = '\0';

            addend(list_of_words,word,i);
            i = 0;
            printf("%s\n",word);
            struct linkedlist *node;
            node = *list_of_words;
            while(node -> next != NULL)
            {
                node = node -> next;
            }
            
        }
    }
        fclose(fp);
        free(full_level_file_name);
        return 0;
}

void randomshuffle(struct linkedlist **list_of_words,int filenumber)
{
    char * shuffle[wordnum];
    for (int i = 0; i < wordnum; i++)
        shuffle[i] = malloc(sizeof(char *));
    struct linkedlist * current;
    current = *list_of_words;
    int i = 0;
    while (current != NULL)
    {
        shuffle[i] = current -> value;
        current = current -> next;
        i++;
    }
    for (int i = wordnum - 1; i > 0; --i) {
      int w = rand()%i;
      char * t = shuffle[i];
      shuffle[i] = shuffle[w];
      shuffle[w] = t;
    }
    current = *list_of_words;
    for (int i = 0; i < wordnum; ++i)
    {
        current -> value = shuffle[i];
        current = current -> next;
        printf("**********%s\n", shuffle[i]);
    }
}

float scorecalculator(float time,int wrongchars,int allchars)
{
    if(time < 1) time = 1;
    score += (3 * allchars - wrongchars ) / time;
    printf("%f :time %d :wrongword %d :allwords\n",time,wrongchars,allchars );
    return (score) ;
}

void scoreboard(float score,char * esm)
{
    FILE *fp;
    if (!(fp = fopen("scoreboard.txt", "a+"))) {
        printf("Failed to open FILE\n");
        return;
    }
    float savedscore;
    char * names_in_file[10];
    for(int i = 0; i< 10 ; i++)
        names_in_file[i] = (char *)malloc(sizeof(char *));
    
}

int isinscoreboard(char * esm,float newscore)
{
    struct scoreboard top10[10];
    FILE *fp;
    if (!(fp = fopen("scoreboard.txt", "r+"))) {
        printf("Failed to open FILE\n");
        return -1;
    }
    float savedscore[10];
    char * names_in_file[10];
    char * name;
    for(int i = 0; i< 10 ; i++)
    {
        names_in_file[i] = (char *)malloc(sizeof(char *));
        top10[i].name = (char *)malloc(20 *sizeof(char));
        top10[i].score = (float *)malloc(sizeof(float));
    }
    int i = 0 ;
    while (!feof(fp))
    {
        fscanf(fp, "%s %f", top10[i].name, top10[i].score);
        i++;
    }
    for (int j = 0; j < 10; ++j)
    {
        for (int i = 0; i < 9; ++i)
        {
            if(*(top10[i].score) < *(top10[i+1].score))
            {
                structswapper(&top10[i],&top10[i+1]);
            }
        }
    }
    if(newscore > *(top10[9].score))
    {
        top10[9].score = &newscore;
        top10[9].name = esm;
    }
    for (int j = 0; j < 10; ++j)
    {
        for (int i = 0; i < 9; ++i)
        {
            if(*(top10[i].score) < *(top10[i+1].score))
            {
                structswapper(&top10[i],&top10[i+1]);
            }
        }
    }

    fclose(fp);
    if (!(fp = fopen("scoreboard.txt", "w+"))) {
        printf("Failed to open FILE\n");
        return -1;
    }
    for (int i = 0; i < 10; ++i)
    {
        fprintf(fp, "%s %.1f \n",top10[i].name,*(top10[i].score));
        printf("%s %f\n",top10[i].name,*(top10[i].score) );
    }
    fclose(fp);
    for (int i = 0; i < 10; ++i)
    {
        scoreboard_toshow[i] = malloc(30*sizeof(char));
        sprintf(scoreboard_toshow[i],"%.1f",*(top10[i].score));
        strcat(scoreboard_toshow[i],":");
        strcat(scoreboard_toshow[i],top10[i].name);
    }
    return 0;
}

void game_quit(GtkWidget * button12)
{
	exit(1);
}
void game_about(GtkWidget * button12)
{
	printf("Heres game address on github%s\n","https://github.com/Gold3nFox/Typing-speed-test.git" );
}
void newlevel_showdialog (void)
{
    gtk_widget_show (GTK_WIDGET(dialog));
}

void toupperchanged (char * mainstr,char * str2)
{
    int len_main = strlen(mainstr);
    // char * matched_entry_to_upper = malloc(len_main*sizeof(char));
    // strncpy(matched_entry_to_upper,mainstr,len_main);
    int len_current = strlen(str2);
    int len_min = len_main > len_current ? len_current : len_main;
    for (int i = 0; i < len_min; ++i)
    {
        if (mainstr[i] == str2[i])
        {
            mainstr[i] = toupper(mainstr[i]);
        }
    }
}

int wrong_chars (char * mainstr,char * str2)
{
    int wrongchars = 0;
    int len_main = strlen(mainstr);
    // char * matched_entry_to_upper = malloc(len_main*sizeof(char));
    // strncpy(matched_entry_to_upper,mainstr,len_main);
    int len_current = strlen(str2);
    int len_min = len_main > len_current ? len_current : len_main;
    for (int i = 0; i < len_min; ++i)
    {
        if (mainstr[i] != str2[i] && str2[i] != ' ' && strcmp(mainstr," ") != 0)
        {
            wrongchars++;
            printf("%s=========%s---->%d\n",mainstr,str2,wrongchars );
        }
    }
    wrongchars = wrongchars + strlen(mainstr) - strlen(str2) ;
    printf("%d.......\n",wrongchars );
    wrongchars++;
    printf("%d-------------\n",wrongchars );
    return wrongchars;
}

void tolowerchanged (char * str2)
{
    int len_current = strlen(str2);
    for (int i = 0; i < len_current; ++i)
    {
        str2[i] = tolower(str2[i]);
    }
}

void save_clicked(GtkWidget * button12,GtkWidget * label3)
{
    isinscoreboard(player_name,player_score);
    printf("%d------------>>>\n", maxlevel);
    if(maxlevel != 10) save(player_name,maxlevel+1);
    if(maxlevel == 10) save(player_name,maxlevel);
    gtk_widget_hide(button12);
}
void save_clicked_file(GtkWidget * button12,GtkWidget * label3)
{
    isinscoreboard(player_name,player_score);
    printf("%d------------>>>\n", maxlevel);
    save(player_name,maxlevel);
}
void show_dialog_scoreboard(void)
{
    gtk_widget_destroy(GTK_WIDGET(dialog));
        builder_dialog_scoreboard = gtk_builder_new();
        gtk_builder_add_from_file (builder_dialog_scoreboard, "GUI.glade" , NULL);
        label7 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label7"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label7");
        label8 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label8"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label8");
        label9 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label9"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label9");
        label10 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label10"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label10");
        label11 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label11"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label11");
        label12 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label12"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label12");
        label13 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label13"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label13");
        label14 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label14"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label14");
        label15 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label15"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label15");
        label16 = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"label16"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"label16");
        messagedialog_scoreboard = GTK_WIDGET(gtk_builder_get_object(builder_dialog_scoreboard,"messagedialog_scoreboard"));
        gtk_builder_connect_signals(builder_dialog_scoreboard,"messagedialog_scoreboard");
        
        gtk_label_set_text(GTK_LABEL(label7),scoreboard_toshow[0]);
        gtk_label_set_text(GTK_LABEL(label8),scoreboard_toshow[1]);
        gtk_label_set_text(GTK_LABEL(label9),scoreboard_toshow[2]);
        gtk_label_set_text(GTK_LABEL(label10),scoreboard_toshow[3]);
        gtk_label_set_text(GTK_LABEL(label11),scoreboard_toshow[4]);
        gtk_label_set_text(GTK_LABEL(label12),scoreboard_toshow[5]);
        gtk_label_set_text(GTK_LABEL(label13),scoreboard_toshow[6]);
        gtk_label_set_text(GTK_LABEL(label14),scoreboard_toshow[7]);
        gtk_label_set_text(GTK_LABEL(label15),scoreboard_toshow[8]);
        gtk_label_set_text(GTK_LABEL(label16),scoreboard_toshow[9]);



        gtk_dialog_run (GTK_DIALOG (messagedialog_scoreboard));
        g_object_unref(G_OBJECT(builder_dialog_scoreboard));
}
void start_clicked(GtkWidget * button11,GtkWidget * label3)
{
    if (level < 10)
    {
        levelopener(level+1,&list_of_words);
        randomshuffle(&list_of_words,level+1);
        gtk_widget_destroy(GTK_WIDGET(dialog));
        char * current_level;
        char * level_number;
        if(level ==1  )
        {
        	level_is_passed_or_not[0] = 0;
        	gtk_widget_show_all (button2);	
        } 
        if(level ==2  )
        {
        	level_is_passed_or_not[1] = 0;
        	gtk_widget_show_all (button3);	
        } 
        if(level ==3  )
        {
        	level_is_passed_or_not[2] = 0;
        	gtk_widget_show_all (button5);	
        } 
        if(level ==4 )
        {
        	level_is_passed_or_not[3] = 0;
        	gtk_widget_show_all (button4);	
        } 
        if(level ==5  )
        {
        	level_is_passed_or_not[4] = 0;
        	gtk_widget_show_all (button6);	
        } 
        if(level ==6 )
        {
        	level_is_passed_or_not[5] = 0;
        	gtk_widget_show_all (button9);	
        } 
        if(level ==7  )
        {
        	level_is_passed_or_not[6] = 0;
        	gtk_widget_show_all (button7);	
        } 
        if(level ==8 )
        {
        	level_is_passed_or_not[7] = 0;
        	gtk_widget_show_all (button8);	
        } 
        if(level ==9 )
        {
        	level_is_passed_or_not[8] = 0;
        	gtk_widget_show_all (button10);	
        } 
        level++;
        current_level = (char *)malloc(8*sizeof(char));
        level_number = (char *)malloc(2*sizeof(char));
        strcpy(current_level,"Level ");
        sprintf(level_number,"%d",level);
        strcat(current_level,level_number);
        gtk_label_set_text (GTK_LABEL(label1),current_level);
    	if (level > maxlevel) maxlevel = level;
    }else
    {
        show_dialog_scoreboard();
    }
}

void game_start(void)
{
	start_builder = gtk_builder_new();
	gtk_builder_add_from_file(start_builder, "GUI.glade" , NULL);
	button13 = GTK_WIDGET(gtk_builder_get_object(start_builder,"button13"));
    gtk_builder_connect_signals(start_builder,"button13");
    dialog_playername = GTK_DIALOG(gtk_builder_get_object(start_builder,"dialog_playername"));
    gtk_builder_connect_signals(start_builder,"dialog_playername");
    label6 = GTK_LABEL(gtk_builder_get_object(start_builder,"label6"));
    gtk_builder_connect_signals(start_builder,"label6");
    entry2 = GTK_WIDGET(gtk_builder_get_object(start_builder,"entry2"));
    gtk_builder_connect_signals(start_builder,"entry2");
    gtk_entry_set_max_length (entry2,14);


    gtk_widget_show (GTK_WIDGET(dialog_playername));


}
void locked_level_disabler(void)
{
	char * button_name;
	char * button_number;
	button_number = (char *)malloc(2*sizeof(char));
	button_name = (char *)malloc(7*sizeof(char));
	if(max_saved_level <= 9)
	{
		level_is_passed_or_not[8] = 1;
		gtk_widget_hide(button10);
	}
	if(max_saved_level <= 8)
	{
		level_is_passed_or_not[7] = 1;
		gtk_widget_hide(button8);	
	} 
	if(max_saved_level <= 7)
	{
		level_is_passed_or_not[6] = 1;
		gtk_widget_hide(button7);	
	} 
	if(max_saved_level <= 6)
	{
		level_is_passed_or_not[5] = 1;
		gtk_widget_hide(button9);	
	} 
	if(max_saved_level <= 5) 
	{
		level_is_passed_or_not[4] = 1;
		gtk_widget_hide(button6);
	}
	if(max_saved_level <= 4)
	{
		level_is_passed_or_not[3] = 1;
		gtk_widget_hide(button4);	
	} 
	if(max_saved_level <= 3)
	{
		level_is_passed_or_not[2] = 1;
		gtk_widget_hide(button5);
	} 
	if(max_saved_level <= 2)
	{
		level_is_passed_or_not[1] = 1;
		gtk_widget_hide(button3);	
	} 
	if(max_saved_level <= 1)
	{
		level_is_passed_or_not[0] = 1;
		gtk_widget_hide(button2);	
	} 
}
void name_clicked (GtkWidget * button13 , GtkWidget * entry2)
{
	player_name = malloc(15*sizeof(char));
	player_name = (gchar *)(gtk_entry_get_text(GTK_ENTRY(entry2)));
	max_saved_level = savedornot(player_name);
	level = max_saved_level;
	maxlevel = max_saved_level;
	printf("%d---------\n",max_saved_level );
	locked_level_disabler();
    gtk_label_set_text(label2,player_name);
	gtk_widget_hide (dialog_playername);
	levelopener(max_saved_level,&list_of_words);
    randomshuffle(&list_of_words,max_saved_level);
    char * loaded_level_name;
    char * loaded_level_number;
    loaded_level_number = (char *)malloc(2*sizeof(char));
    loaded_level_name = (char *)malloc(9*sizeof(char));
    strcpy(loaded_level_name,"Level ");
    sprintf(loaded_level_number,"%d",max_saved_level);
    strcat(loaded_level_name,loaded_level_number);
    gtk_label_set_text (GTK_LABEL(label1),loaded_level_name);

}

void text_changed(GtkWidget *entry1, GtkWidget *label3){
    char *score_to_string;
    gchar * input;
    gchar * labelvalue;
    char *emptystring;
    input = (gchar *)(gtk_entry_get_text (GTK_ENTRY(entry1)));
    labelvalue = (gchar *)(gtk_label_get_text (GTK_LABEL(label3)));
    int len_main = strlen(list_of_words -> value);
    char * lastword = malloc(strlen(labelvalue)*sizeof(char));
    char * matched_entry_to_upper = malloc(strlen(labelvalue)*sizeof(char));
    if(strcmp(input,"start ") == 0)
    {
        ftime(&start_t);
        printf("%lf:starting time\n",(double)(start_t ));
        started = 1;
    }else if(input[strlen(input)-1] == ' ')
    {
                tolowerchanged(labelvalue);
                printf("------%s====\n",input );
                word_wrong_chars = wrong_chars(labelvalue,input);
                printf("%dkkkkkakksdkaskdaksd\n",word_wrong_chars );
                all_chars = strlen(labelvalue);
                printf("%dOooooooooOooo\n", all_chars);
                ftime(&end_t);
                printf("%lf:ending time\n",(double)(end_t ));
                total_t = (double)(end_t - start_t);
                levelscore += scorecalculator(total_t,word_wrong_chars,all_chars);
                ftime(&start_t);
    }
    if(started == 1)
    {
        if(input[strlen(input)-1] == ' ')
        {
            if (list_of_words -> next == NULL )
            {
                word_wrong_chars = 0;
                all_chars = 0;
                started = 0;
                builder_dialog = gtk_builder_new();
                gtk_builder_add_from_file (builder_dialog, "GUI.glade" , NULL);
                //gtk_builder_add_from_file (builder_dialog, "GUI.glade", NULL);
                button11 = GTK_WIDGET(gtk_builder_get_object(builder_dialog,"button11"));
                gtk_builder_connect_signals(builder_dialog,"button11");
                button12 = GTK_WIDGET(gtk_builder_get_object(builder_dialog,"button12"));
                gtk_builder_connect_signals(builder_dialog,"button12");
                label4 = GTK_WIDGET(gtk_builder_get_object(builder_dialog,"label4"));
                gtk_builder_connect_signals(builder_dialog,"label4");
                label5 = GTK_WIDGET(gtk_builder_get_object(builder_dialog,"label5"));
                gtk_builder_connect_signals(builder_dialog,"label5");
                statusbar1 = GTK_WIDGET(gtk_builder_get_object(builder_dialog,"statusbar1"));
                gtk_builder_connect_signals(builder_dialog,"statusbar1");
                dialog = GTK_DIALOG(gtk_builder_get_object(builder_dialog,"dialog_newlevel"));
                gtk_builder_connect_signals(builder_dialog,"dialog_newlevel");   
                printf("%f>>>>>>>>>\n", levelscore);
                levelscore = levelscore / wordnum;
                printf("%d------------------\n",wordnum );
                score_to_string = malloc(13*sizeof(char));
                sprintf(score_to_string,"%.3f",levelscore);
                strcat(score_to_string,":score");
                gtk_label_set_text (GTK_LABEL(label5),score_to_string);


                player_score += levelscore;
                printf("%f==========%f====scores\n",levelscore,player_score);
                levelscore = 0;

                gtk_entry_set_text (GTK_ENTRY(entry1),"");
                gtk_label_set_text (GTK_LABEL(label3),"start");
                gtk_label_set_text (GTK_LABEL(label4),player_name);
                newlevel_showdialog();
                g_object_unref(G_OBJECT(builder_dialog));
                return;
            }
            if(startchecker == 1)
            {
                list_of_words = list_of_words -> next;  
            }
            startchecker = 1;
            gtk_entry_set_text (GTK_ENTRY(entry1),"");
            strcpy(matched_entry_to_upper,list_of_words -> value);
            gtk_label_set_text (GTK_LABEL(label3),matched_entry_to_upper);
            labelvalue = (gchar *)(gtk_label_get_text (GTK_LABEL(label3)));
        }else
        {
            strcpy(matched_entry_to_upper,list_of_words -> value);
        }
    }
    strcpy(matched_entry_to_upper,labelvalue);
    tolowerchanged(matched_entry_to_upper);
    toupperchanged (matched_entry_to_upper , input);
    gtk_label_set_text (GTK_LABEL(label3),matched_entry_to_upper);

}
void level1_clicked (GtkWidget *button1,GtkWidget *entry1)
{
    level = 1;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(1,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,1);
}
void level1_pressed (GtkWidget *button1,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level1_released (GtkWidget *button1,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 1");
}
void level2_clicked (GtkWidget *button2,GtkWidget *entry1)
{
    level = 2;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(2,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,2);
}
void level2_pressed (GtkWidget *button2,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level2_released (GtkWidget *button2,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 2");
}
void level3_clicked (GtkWidget *button3,GtkWidget *entry1)
{
    level = 3;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(3,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,3);
}
void level3_pressed (GtkWidget *button3,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level3_released (GtkWidget *button3,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 3");
}
void level4_clicked (GtkWidget *button4,GtkWidget *entry1)
{
    level = 4;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(4,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,4);
}
void level4_pressed (GtkWidget *button4,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level4_released (GtkWidget *button4,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 4");
}
void level5_clicked (GtkWidget *button5,GtkWidget *entry1)
{
    level = 5;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(5,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,5);
}
void level5_pressed (GtkWidget *button5,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level5_released (GtkWidget *button5,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 5");
}
void level6_clicked (GtkWidget *button6,GtkWidget *entry1)
{
    level = 6;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(6,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,6);
}
void level6_pressed (GtkWidget *button6,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level6_released (GtkWidget *button6,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 6");
}
void level7_clicked (GtkWidget *button7,GtkWidget *entry1)
{
    level = 7;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(7,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,7);
}
void level7_pressed (GtkWidget *button7,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level7_released (GtkWidget *button7,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 7");
}
void level8_clicked (GtkWidget *button8,GtkWidget *entry1)
{
    level = 8;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(8,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,8);
}
void level8_pressed (GtkWidget *button8,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level8_released (GtkWidget *button8,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 8");
}
void level9_clicked (GtkWidget *button9,GtkWidget *entry1)
{
    level = 9;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");
    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(9,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,9);
}
void level9_pressed (GtkWidget *button9,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level9_released (GtkWidget *button9,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 9");
}
void level10_clicked (GtkWidget *button10,GtkWidget *entry1)
{
    level = 10;
    if (level > maxlevel) maxlevel = level;
    gtk_entry_set_text (GTK_ENTRY(entry1),"");

    int openner_feedback;
    struct linkedlist * head;
    while(list_of_words -> next != NULL && list_of_words != NULL)
    {
        head = list_of_words;
        list_of_words = list_of_words -> next;
        free(head);
    }
    list_of_words = NULL;
    openner_feedback = levelopener(10,&list_of_words);
    if (openner_feedback != -1)randomshuffle(&list_of_words,10);
}
void level10_pressed (GtkWidget *button10,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level10_released (GtkWidget *button10,GtkWidget *label1)
{   
    gtk_label_set_text (GTK_LABEL(label1),"Level 10");
}
void dialog_closer (void)
{
    gtk_widget_destroy (GTK_WIDGET(dialog));
}
int main (int argc, char *argv[])
{
    srand(time(NULL));
    list_of_words = NULL;
    isinscoreboard("behnam", -300000);
    GtkWidget *window,*menubar,*menuitem1,*menuitem2,*menuitem3,*menuitem4,*label3,*progressbar,*entry;
    gtk_init (&argc, &argv);
    builder = gtk_builder_new ();
    builder_dialog = gtk_builder_new ();
    // GError *err = NULL;  baraye printe errore builder NULL e payin ba &err jabeja she

    gtk_builder_add_from_file (builder, "GUI.glade", NULL);
    // if (err){
    //     g_print(err->message);
    //     printf("%s\n", err);      Baraye moshahede daghighe error uncomment she
    //     return 1;
    // }
    window = GTK_WIDGET(gtk_builder_get_object(builder,"window_main"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder,"button1"));
    button2 = GTK_WIDGET(gtk_builder_get_object(builder,"button2"));
    button3 = GTK_WIDGET(gtk_builder_get_object(builder,"button3"));
    button4 = GTK_WIDGET(gtk_builder_get_object(builder,"button4"));
    button5 = GTK_WIDGET(gtk_builder_get_object(builder,"button5"));
    button6 = GTK_WIDGET(gtk_builder_get_object(builder,"button6"));
    button7 = GTK_WIDGET(gtk_builder_get_object(builder,"button7"));
    button8 = GTK_WIDGET(gtk_builder_get_object(builder,"button8"));
    button9 = GTK_WIDGET(gtk_builder_get_object(builder,"button9"));
    button10 = GTK_WIDGET(gtk_builder_get_object(builder,"button10"));
    progressbar = GTK_WIDGET(gtk_builder_get_object(builder,"progressbar1"));
    entry = GTK_WIDGET(gtk_builder_get_object(builder,"entry1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
    label2 = GTK_WIDGET(gtk_builder_get_object(builder,"label2"));
    label3 = GTK_WIDGET(gtk_builder_get_object(builder,"label3"));
    menubar = GTK_WIDGET(gtk_builder_get_object(builder,"menubar1"));
    menuitem1 = GTK_WIDGET(gtk_builder_get_object(builder,"menuitem1"));
    menuitem2 = GTK_WIDGET(gtk_builder_get_object(builder,"menuitem2"));
    menuitem3 = GTK_WIDGET(gtk_builder_get_object(builder,"menuitem3"));
    menuitem4 = GTK_WIDGET(gtk_builder_get_object(builder,"menuitem4"));
    gtk_builder_connect_signals(builder,"window.main");
    gtk_builder_connect_signals(builder,"button1");
    gtk_builder_connect_signals(builder,"button2");
    gtk_builder_connect_signals(builder,"button3");
    gtk_builder_connect_signals(builder,"button4");
    gtk_builder_connect_signals(builder,"button5");
    gtk_builder_connect_signals(builder,"button6");
    gtk_builder_connect_signals(builder,"button7");
    gtk_builder_connect_signals(builder,"button8");
    gtk_builder_connect_signals(builder,"button9");
    gtk_builder_connect_signals(builder,"button10");
    gtk_builder_connect_signals(builder,"progressbar1");
    gtk_builder_connect_signals(builder,"entry1");
    gtk_builder_connect_signals(builder,"label1");
    gtk_builder_connect_signals(builder,"label2");
    gtk_builder_connect_signals(builder,"label3");
    gtk_builder_connect_signals(builder,"menubar1");
    gtk_builder_connect_signals(builder,"menuitem1");
    gtk_builder_connect_signals(builder,"menuitem2");
    gtk_builder_connect_signals(builder,"menuitem3");
    gtk_builder_connect_signals(builder,"menuitem4");

    
    g_object_unref(G_OBJECT(builder));
    gtk_widget_show(GTK_WIDGET(window));
    game_start();
    gtk_main ();
    return 0;
}
