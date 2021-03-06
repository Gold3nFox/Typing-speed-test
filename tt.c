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
bool started=0,startchecker =0;
int wordnum = 0,level_is_passed_or_not[10],level = 1,maxlevel= 1,gameispaused = 0,max_saved_level = 0,current_word_number_on_list = 0,wrong_characters,all_characters,score_mode = 0;
float score = 0,levelscore = 0,player_score = 0;
GtkHButtonBox * buttonbox;
GtkDialog *dialog,*dialog_playername;
GtkBuilder *builder, *builder_dialog,*builder_dialog_scoreboard,*start_builder;
GtkWidget *progressbar,*buttonpause,*entry2,*entry,*button11,*button12,*button13,*label1,*label2,*label4,*label5,*label6,*label7,*label8,*label9,*label10,*label11,*label12,*label13,*label14,*label15,*label16,*messagedialog_scoreboard,*statusbar1,*dialog_newlevel,*button1,*button2,*button3,*button4,*button5,*button6,*button7,*button8,*button9,*button10;
clock_t start_t, end_t, total_t , pause_t =0,resume_t =0;



// chon nemitonam ro xml bishtar az ye object bedam be eventa baranke dasresi dashte basham global bayad bokonam
// albate bishtar az ye objectam shayad beshe man chizi peyda nakardam


struct linkedlist
{
    char * value;
    struct linkedlist * next;
};
// baraye zakhire kalamate file ha


struct scoreboard
{
    char * name;
    float * score;
};
// baraye zakhire esm va emtiaz top 10


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
// baraye sort kardane structe top 10


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
// baraye ezafe kardan be linkedlist


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
// baraye inke agar save bod bishtarin leveli ke rafte bodo bekhonam


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
//baraye inke bishtarin leveli ke rafte save beshe


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
            word[i] = c;
            i++;
        }else if (c == ' ')
        {
            wordnum ++;
            word[i] = '\0';

            addend(list_of_words,word,i);
            i = 0;
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
// baz kardane level va rikhtane kalamat dar linkedlist


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
    }
}
//random kardane kalamate linked list


void mode_to_wpm (void)
{
	score_mode = 1;
}
// baraye gozashtane emtiaz dar halate wpm


void mode_to_cps(void)
{
	score_mode = 0;
}
// baraye gozashtane emtiaz dar halate cps


float scorecalculator(float time,int wrongchars,int allchars)
{
	if(score_mode == 0)
	{
		if(time < 1 && gameispaused == 0)
    	{
    		wrongchars = 0;
    		allchars = 0;
    		//age kasi kolle levelo zire 1sanie bere hackere :|
    	}
    	if(gameispaused == 1) time = -1; // age to pause kalame benevisi harchi emtiaz begiri az emtiaz kollet kam mishe :)
    	score = (allchars - wrongchars ) / time; //character per second
    	return (score) ;
	}
	else if(score_mode == 1)
	{
		if(time < 1 && gameispaused == 0)
    	{
    		wrongchars = 0;
    		allchars = 0;
    		//age kasi kolle levelo zire 1sanie bere hackere :|
    	}
    	if(gameispaused == 1) time = -1; // age to pause kalame benevisi harchi emtiaz begiri az emtiaz kollet kam mishe :)
		score = ( (allchars - wrongchars) / 5) / (time/60);
		return (score) ;
	}
    return 0;
}
// baraye ehtesabe emtiazat


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
    //agar az akharine scoreboard emtiaze fard bishtar bod bejaye akharine scoreboard gharar midaham va scoreboard ra dobare sort mikonam
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
    }
    fclose(fp);
    char * score_of_top10s;
    score_of_top10s = (char *)malloc(10*sizeof(char));
    for (int i = 0; i < 10; ++i)
    {
        scoreboard_toshow[i] = malloc(32*sizeof(char));
        strcpy(scoreboard_toshow[i],top10[i].name);
        sprintf(score_of_top10s,"%.1f",*(top10[i].score));
        strcat(scoreboard_toshow[i]," : ");
        strcat(scoreboard_toshow[i],score_of_top10s);
    }
    return 0;
}
//baraye didane inke farde jadid dar scoreboard top10 jaye migirad ya na va khandane scoreboard az file


void game_pause(void)
{
	system("killall -STOP afplay");
	gameispaused = 1;
	pause_t = time(NULL);
	if(max_saved_level > 9)
	{
		gtk_widget_hide(button10);
	}
	if(max_saved_level > 8)
	{
		gtk_widget_hide(button8);	
	} 
	if(max_saved_level > 7)
	{
		gtk_widget_hide(button7);	
	} 
	if(max_saved_level > 6)
	{
		gtk_widget_hide(button9);	
	} 
	if(max_saved_level > 5) 
	{
		gtk_widget_hide(button6);
	}
	if(max_saved_level > 4)
	{
		gtk_widget_hide(button4);	
	} 
	if(max_saved_level > 3)
	{
		gtk_widget_hide(button5);
	} 
	if(max_saved_level > 2)
	{
		gtk_widget_hide(button3);	
	} 
	if(max_saved_level > 1)
	{
		gtk_widget_hide(button2);	
	} 
	gtk_widget_hide(button1);
}
//gheyre faal kardane dokmeha dar halati ke bazi pause shode va mohasebe zamane pause


void game_play(void)
{
	system("killall -CONT afplay");
	gameispaused = 0;
	resume_t = time(NULL);
	if(max_saved_level > 9)
	{
		gtk_widget_show(button10);
	}
	if(max_saved_level > 8)
	{
		gtk_widget_show(button8);	
	} 
	if(max_saved_level > 7)
	{
		gtk_widget_show(button7);	
	} 
	if(max_saved_level > 6)
	{
		gtk_widget_show(button9);	
	} 
	if(max_saved_level > 5) 
	{
		gtk_widget_show(button6);
	}
	if(max_saved_level > 4)
	{
		gtk_widget_show(button4);	
	} 
	if(max_saved_level > 3)
	{
		gtk_widget_show(button5);
	} 
	if(max_saved_level > 2)
	{
		gtk_widget_show(button3);	
	} 
	if(max_saved_level > 1)
	{
		gtk_widget_show(button2);	
	} 
	gtk_widget_show(button1);
}
//faal kardane dokmeha dar halate resume va mohasebe zamane pause


void game_quit(GtkWidget * button12)
{
	system("killall afplay");
	exit(1);
}
//khoroj az bazi ba gozine quit


void newlevel_showdialog (void)
{
    gtk_widget_show (GTK_WIDGET(dialog));
}
//baraye neshan dadane menuye raftan be levele jadid - baadi


void toupperchanged (char * string_to_be_uppercased,char * word_to_be_typed)
{
    int len_main = strlen(string_to_be_uppercased);
    int len_current = strlen(word_to_be_typed);
    int len_min = len_main > len_current ? len_current : len_main;
    for (int i = 0; i < len_min; ++i)
    {
        if (string_to_be_uppercased[i] == word_to_be_typed[i])
        {
            string_to_be_uppercased[i] = toupper(string_to_be_uppercased[i]);
        }
    }
}
//uppercase kardane kalamate moshabehe kalameye aval ba dovome vorodi


void tolowerchanged (char * word_to_be_lowercased)
{
    int len_current = strlen(word_to_be_lowercased);
    for (int i = 0; i < len_current; ++i)
    {
        word_to_be_lowercased[i] = tolower(word_to_be_lowercased[i]);
    }
}
//lowercase kardane kolle harf haye yek kalame


int wrong_chars (char * word_to_calculate_its_wrong_chars,char * word_to_be_typed)
{
    int wrongchars_of_input_word = 0;
    tolowerchanged(word_to_be_typed);
    tolowerchanged(word_to_calculate_its_wrong_chars);
    int len_main = strlen(word_to_calculate_its_wrong_chars);
    int len_current = strlen(word_to_be_typed);
    int len_min = len_main > len_current ? len_current : len_main;
    for (int i = 0; i < len_min; ++i)
    {
        if (word_to_calculate_its_wrong_chars[i] != word_to_be_typed[i])
        {
            wrongchars_of_input_word++;
        }
    }
    wrongchars_of_input_word = wrongchars_of_input_word + strlen(word_to_calculate_its_wrong_chars) - strlen(word_to_be_typed) ;
    wrongchars_of_input_word++;
    return wrongchars_of_input_word;
}
//baraye mohasebe tedad harfaye ghalat type shode



void save_clicked(GtkWidget * button12,GtkWidget * label3)
{
    isinscoreboard(player_name,player_score);
    if(maxlevel != 10) save(player_name,maxlevel+1);
    if(maxlevel == 10) save(player_name,maxlevel);
    gtk_widget_hide(button12);
}
//save kardane bazi hengami ke gozine save ra zad


void save_clicked_file(GtkWidget * button12,GtkWidget * label3)
{
    isinscoreboard(player_name,player_score);
    save(player_name,maxlevel);
}
//save kardane bazi hengami ke az menu ha save ra entekhab kard


void show_dialog_git(void)
{
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



        gtk_label_set_text(GTK_LABEL(label7),"This game is performed by BehnamAminazad");
        gtk_widget_hide(GTK_WIDGET(label8));
        gtk_widget_hide(GTK_WIDGET(label9));
        gtk_widget_hide(GTK_WIDGET(label10));
        gtk_widget_hide(GTK_WIDGET(label11));
        gtk_widget_hide(GTK_WIDGET(label12));
        gtk_widget_hide(GTK_WIDGET(label13));
        gtk_widget_hide(GTK_WIDGET(label14));
        gtk_label_set_text(GTK_LABEL(label15),"Source code is available at:  https://github.com/Gold3nFox/Typing-speed-test.git");
       	gtk_label_set_text(GTK_LABEL(label16)," ");



        gtk_dialog_run (GTK_DIALOG (messagedialog_scoreboard));
        g_object_unref(G_OBJECT(builder_dialog_scoreboard));
}
//baraye namayeshe moshakhasat vaghti az menu ha about entekhab shod


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
//baraye namayeshe scoreboard


void start_clicked(GtkWidget * button11,GtkWidget * label3)
{
	gtk_progress_bar_set_fraction (progressbar,0);
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
        max_saved_level++;
        current_level = (char *)malloc(8*sizeof(char));
        level_number = (char *)malloc(2*sizeof(char));
        strcpy(current_level,"Level ");
        sprintf(level_number,"%d",level);
        strcat(current_level,level_number);
        gtk_label_set_text (GTK_LABEL(label1),current_level);
    	if (level > maxlevel) maxlevel = level;
    }else
    {
    	levelopener(level,&list_of_words);
        randomshuffle(&list_of_words,level);
        show_dialog_scoreboard();
    }
}
//shoroe levele badi vaghti start ra zad


void scoreboard_close (void)
{
	gtk_widget_destroy(GTK_WIDGET(messagedialog_scoreboard));
}
//baste shodane scoreboard ba click bar roye dokme close


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
//neshan dadane peyghame gereftane esm dar ebtedae bazi


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
//hide kardane level hayi ke hanoz baz nashode baraye bazikon


void name_clicked (GtkWidget * button13 , GtkWidget * entry2)
{
	gtk_progress_bar_set_fraction (progressbar,0);
	player_name = malloc(15*sizeof(char));
	player_name = (gchar *)(gtk_entry_get_text(GTK_ENTRY(entry2)));
	max_saved_level = savedornot(player_name);
	level = max_saved_level;
	maxlevel = max_saved_level;
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
//baraye zakhire esme bazikon va agar ghablan save karde bod levele save shode baraye on load shavad


void text_changed(GtkWidget *entry1, GtkWidget *label3){
    char *score_to_string;
    float progress_amount = 0;
    gchar * input;
    gchar * labelvalue;
    input = (gchar *)(gtk_entry_get_text (GTK_ENTRY(entry1)));
    labelvalue = (gchar *)(gtk_label_get_text (GTK_LABEL(label3)));
    int len_main = strlen(list_of_words -> value);
    char * lastword = malloc(strlen(labelvalue)*sizeof(char));
    char * matched_entry_to_upper = malloc(strlen(labelvalue)*sizeof(char));
    if(strcmp(input,"start ") == 0)
    {
    	current_word_number_on_list = 0;
    	resume_t = 0;
		pause_t = 0;
    	wrong_characters = 0;
    	all_characters = 0;
        start_t = time(NULL);
        started = 1;
    }else if(input[strlen(input)-1] == ' ')
    {
    	current_word_number_on_list++;
    	progress_amount = (double)current_word_number_on_list / (double)wordnum;
    	gtk_progress_bar_set_fraction (progressbar,progress_amount);
    	all_characters += strlen(list_of_words -> value);
    	wrong_characters += wrong_chars(list_of_words -> value,input);
    }
    if(started == 1)
    {
        if(input[strlen(input)-1] == ' ')
        {
            if (list_of_words -> next == NULL )
            {
            	end_t = time(NULL);
            	total_t = (int)(end_t - start_t) - (int)(resume_t - pause_t);
            	resume_t = 0;
            	pause_t = 0;

            	levelscore = scorecalculator(total_t,wrong_characters,all_characters);



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
               

                score_to_string = malloc(13*sizeof(char));
                sprintf(score_to_string,"%.1f",levelscore);
                strcat(score_to_string,":score");
                gtk_label_set_text (GTK_LABEL(label5),score_to_string);

                if(score_mode == 1)
                {
                	levelscore = levelscore / 15;
                }
                player_score += levelscore;
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
//harbar kalamei dakhele textbox avaz shavad in tabe ejra mishavad va baraye khandan va ejrae amaliate bazi roye kalamate vorodi nesbat be kalamate file ha ast


void level1_clicked (GtkWidget *button1,GtkWidget *entry1)
{
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
	gtk_progress_bar_set_fraction (progressbar,0);
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
//baste shodane menuye raftan be levele badi bad az start


int main (int argc, char *argv[])
{
	system("afplay song.mp3 &");
    srand(time(NULL));
    list_of_words = NULL;
    isinscoreboard("behnam", -300000);
    GtkWidget *window,*menubar,*menuitem1,*menuitem2,*menuitem3,*menuitem4,*label3;
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
