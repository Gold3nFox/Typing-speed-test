#include <gtk/gtk.h>
#include <stdio.h>
#include "string.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
struct linkedlist * nn;
bool started=0;
bool startchecker =0;
int wordnum = 0;
int all_chars = 0;
int word_wrong_chars = 0;
int level = 0;
float score = 0,levelscore = 0;
GtkHButtonBox * buttonbox;
GtkDialog *dialog;
GtkBuilder *builder, *builder_dialog,*builder_dialog_scoreboard;
GtkWidget *button11,*button12,*label4,*label5,*label7,*label8,*label9,*label10,*label11,*label12,*label13,*label14,*label15,*label16,*messagedialog_scoreboard,*statusbar1,*dialog_newlevel;
clock_t start_t, end_t, total_t;
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
    struct linkedlist *nn;
    current = *list;
    length++; // bara nulle akhareshon
    if (current == NULL)
    {
        *list = (struct linkedlist *)malloc(sizeof(struct linkedlist));
        ((struct linkedlist*)*list) -> value = (char *)malloc(sizeof(length * sizeof(char)));
        strncpy(((struct linkedlist*)*list) -> value , word, length);
        // printf("null-word len: %i", length);
        ((struct linkedlist*)*list) -> next = NULL;
        return;
    }
    while(current->next !=NULL)
    {
        current = current -> next;
    }
    nn = (struct linkedlist *)malloc(sizeof(struct linkedlist));
    nn -> value = (char *)malloc(sizeof(length * sizeof(char)));
    current -> next = nn;
    nn -> next = NULL;
    strncpy(nn -> value , word, length);
    // printf("word len: %i", length);
    // printf("-----%s\n", ((struct linkedlist *)nn) -> value);
}

int savedornot(char *esm)
{
    int i = 0;
    FILE *fp;
    int level;
    char * esmtmp;
    esmtmp = (char *)malloc(20 * sizeof(char));
    if (!(fp = fopen("save.txt", "rt"))) {
        while(!(fp = fopen("save.txt", "rt")))
        {
            printf("Trying to open this level for you\n");
            i++;
            if(i == 5)
            {
                printf("Couldn't open that level please choose another level\n");
                fclose(fp);
                return -1;            
            } 
        }
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s %d", esmtmp, &level);
        if(strcmp(esm, esmtmp) == 0)
        {
            return level;
        }
    }
    fclose(fp);
    return 0;
}


int save(char *esm,int level)
{
    FILE *fp;
    if (!(fp = fopen("save.txt", "a+"))) {
        printf("Failed to open FILE\n");
        return -1;
    }
    if( savedornot(esm) == 0 )
    {
        fprintf(fp, "%s %d",esm,level);
        fprintf(fp,"\n");
        fclose(fp);
        return 0;
    }
    else
    {
        fclose(fp);
        return savedornot(esm);
    }
}

int levelopener(int a,struct linkedlist ** nn)
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

            addend(nn,word,i);
            i = 0;
            printf("%s\n",word);
            struct linkedlist *node;
            node = *nn;
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

void randomshuffle(struct linkedlist **nn,int filenumber)
{
    char * shuffle[wordnum];
    for (int i = 0; i < wordnum; i++)
        shuffle[i] = malloc(sizeof(char *));
    struct linkedlist * current;
    current = *nn;
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
    current = *nn;
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
    char * esmtmp[10];
    for(int i = 0; i< 10 ; i++)
        esmtmp[i] = (char *)malloc(sizeof(char *));
    
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
    char * esmtmp[10];
    char * name;
    for(int i = 0; i< 10 ; i++)
    {
        esmtmp[i] = (char *)malloc(sizeof(char *));
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
         fprintf(fp, "%s %f \n",top10[i].name,*(top10[i].score));
    }
    return 0;
}

void newlevel_showdialog (void)
{
    gtk_widget_show (dialog);
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
        if (mainstr[i] != str2[i] && str2[i] != ' ' && mainstr != ' ')
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

void start_clicked(GtkWidget * button11,GtkWidget * label3)
{
    if (level < 10)
    {
        levelopener(level+1,&nn);
        randomshuffle(&nn,level+1);
        gtk_widget_destroy(dialog);
    }else
    {
        gtk_widget_destroy(dialog);
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
        gtk_dialog_run (GTK_DIALOG (messagedialog_scoreboard));
        g_object_unref(G_OBJECT(builder_dialog_scoreboard));
    }
}

void text_changed(GtkWidget *entry1, GtkWidget *label3){
    char *score_to_string;
    const gchar * input;
    gchar * labelvalue;
    char *emptystring;
    input = gtk_entry_get_text (GTK_ENTRY(entry1));
    labelvalue = gtk_label_get_text (label3);
    int len_main = strlen(nn -> value);
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
            if (nn -> next == NULL )
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
                dialog = GTK_WIDGET(gtk_builder_get_object(builder_dialog,"dialog_newlevel"));
                gtk_builder_connect_signals(builder_dialog,"dialog_newlevel");   
                printf("%f>>>>>>>>>\n", levelscore);
                levelscore = levelscore / wordnum;
                printf("%d------------------\n",wordnum );
                score_to_string = malloc(13*sizeof(char));
                sprintf(score_to_string,"%.3f",levelscore);
                strcat(score_to_string,":score");
                gtk_label_set_text (label5,score_to_string);

                gtk_entry_set_text (entry1,"");
                gtk_label_set_text (GTK_LABEL(label3),"start");
                newlevel_showdialog();
                g_object_unref(G_OBJECT(builder_dialog));
                return;
            }
            if(startchecker == 1)
            {
                nn = nn -> next;  
            }
            startchecker = 1;
            gtk_entry_set_text (entry1,"");
            strcpy(matched_entry_to_upper,nn -> value);
            gtk_label_set_text (GTK_LABEL(label3),matched_entry_to_upper);
            labelvalue = gtk_label_get_text (label3);
        }else
        {
            strcpy(matched_entry_to_upper,nn -> value);
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
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(1,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,1);
}
void level1_pressed (GtkWidget *button1,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level2_clicked (GtkWidget *button2,GtkWidget *entry1)
{
    level = 2;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(2,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,2);
}
void level2_pressed (GtkWidget *button2,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level3_clicked (GtkWidget *button3,GtkWidget *entry1)
{
    level = 3;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(3,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,3);
}
void level3_pressed (GtkWidget *button3,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level4_clicked (GtkWidget *button4,GtkWidget *entry1)
{
    level = 4;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(4,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,4);
}
void level4_pressed (GtkWidget *button4,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level5_clicked (GtkWidget *button5,GtkWidget *entry1)
{
    level = 5;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(5,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,5);
}
void level5_pressed (GtkWidget *button5,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level6_clicked (GtkWidget *button6,GtkWidget *entry1)
{
    level = 6;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(6,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,6);
}
void level6_pressed (GtkWidget *button6,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level7_clicked (GtkWidget *button7,GtkWidget *entry1)
{
    level = 7;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(7,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,7);
}
void level7_pressed (GtkWidget *button7,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level8_clicked (GtkWidget *button8,GtkWidget *entry1)
{
    level = 8;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(8,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,8);
}
void level8_pressed (GtkWidget *button8,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level9_clicked (GtkWidget *button9,GtkWidget *entry1)
{
    level = 9;
    gtk_entry_set_text (entry1,"");
    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(9,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,9);
}
void level9_pressed (GtkWidget *button9,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void level10_clicked (GtkWidget *button10,GtkWidget *entry1)
{
    level = 10;
    gtk_entry_set_text (entry1,"");

    int openner_feedback;
    struct linkedlist * head;
    while(nn -> next != NULL && nn != NULL)
    {
        head = nn;
        nn = nn -> next;
        free(head);
    }
    nn = NULL;
    openner_feedback = levelopener(10,&nn);
    if (openner_feedback != -1)randomshuffle(&nn,10);
}
void level10_pressed (GtkWidget *button10,GtkWidget *label3)
{   
    gtk_label_set_text (GTK_LABEL(label3),"start");
}
void dialog_closer (void)
{
    gtk_widget_destroy (dialog);
}
int main (int argc, char *argv[])
{
    srand(time(0));
    nn = NULL;
    levelopener(1,&nn);
    randomshuffle(&nn,1);
    
    GError *err = NULL;
    GtkWidget *window,*menubar,*menuitem1,*menuitem2,*menuitem3,*menuitem4,*label1,*label2,*label3,*progressbar,*entry,*button1,*button2,*button3,*button4,*button5,*button6,*button7,*button8,*button9,*button10;
    gtk_init (&argc, &argv);
    builder = gtk_builder_new ();
    builder_dialog = gtk_builder_new ();
    printf("%s\n", err);

    gtk_builder_add_from_file (builder, "GUI.glade", &err);
    if (err){
        g_print(err->message);
        printf("%s\n", err);
        return 1;
    }
    window = GTK_WIDGET(gtk_builder_get_object(builder,"window_main"));
    // dialog = GTK_WIDGET(gtk_builder_get_object(builder,"dialog_newlevel"));
    // gtk_builder_connect_signals(builder,"dialog_newlevel");
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
    // g_signal_connect (button, "clicked", G_CALLBACK (print_text), NULL);
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


    // button11 = GTK_WIDGET(gtk_builder_get_object(builder,"button11"));
    // gtk_builder_connect_signals(builder,"button11");
    // button12 = GTK_WIDGET(gtk_builder_get_object(builder,"button12"));
    // gtk_builder_connect_signals(builder,"button12");
    // label4 = GTK_WIDGET(gtk_builder_get_object(builder,"label4"));
    // gtk_builder_connect_signals(builder,"label4");
    // label5 = GTK_WIDGET(gtk_builder_get_object(builder,"label5"));
    // gtk_builder_connect_signals(builder,"label5");
    // statusbar1 = GTK_WIDGET(gtk_builder_get_object(builder,"statusbar1"));
    // gtk_builder_connect_signals(builder,"statusbar1");


    // user_function (GtkDialog *arg0,gpointer   user_data)

    // newlevel_showdialog(button11,button12,label4,label5,statusbar1,dialog_newlevel,builder);


    
    g_object_unref(G_OBJECT(builder));
    gtk_widget_show(GTK_WIDGET(window));
    gtk_main ();
    return 0;
}
