#include <gtk/gtk.h>
#include <stdio.h>
#include "string.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
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
void addend(struct linkedlist **list,char * value, int length)
{
    struct linkedlist *current;
    struct linkedlist *nn;
    current = *list;
    if (current == NULL)
    {
        *list = (struct linkedlist *)malloc(sizeof(struct linkedlist));
        ((struct linkedlist*)*list) -> value = (char *)malloc(sizeof(length * sizeof(char)));
        memcpy(((struct linkedlist*)*list) -> value , value, length);
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
    memcpy(nn -> value , value, length);
    // printf("-----%s\n", ((struct linkedlist *)nn) -> value);
}

int savedornot(char *esm)
{
    FILE *fp;
    int level;
    char * esmtmp;
    esmtmp = (char *)malloc(20 * sizeof(char));
    if (!(fp = fopen("save.txt", "r+"))) {
        printf("Failed to open FILE\n");
        return -1;
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s %d", esmtmp, &level);
        if(strcmp(esm, esmtmp) == 0)
        {
            return level;
        }
    }
    return 0;
    fclose(fp);
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
        return savedornot(esm);
    }
}
int wordnum(int a)
{
    FILE *fp;
    int i =0;
    int c;
    char * str;
    str = (char *)malloc(12*sizeof(char));
    char *first;
    char *end;
    first = (char *)malloc(7*sizeof(char));
    end = (char *)malloc(5*sizeof(char));
    first = "level-";
    end = ".txt";
    char *mid;
    mid =(char *)malloc(sizeof(char));
    sprintf(mid, "%d", a);
    memcpy(str,first,6);
    strcat(str,mid);
    strcat(str,end);
    if (!(fp = fopen(str, "rt"))) {
        printf("Failed to open FILE\n");
        exit(1);
    }
    while ((c = fgetc(fp)) != EOF) {
        if (c ==' ')
        {
            i++;
        }
    }
    return i;
}

int levelopener(int a,struct linkedlist ** nn)
{
    int i = 0;
    FILE *fp;
    int c;
    char * str;
    str = (char *)malloc(12*sizeof(char));
    char *first;
    char *end;
    first = (char *)malloc(7*sizeof(char));
    end = (char *)malloc(5*sizeof(char));
    first = "level-";
    end = ".txt";
    char *mid;
    mid =(char *)malloc(sizeof(char));
    sprintf(mid, "%d", a);
    memcpy(str,first,6);
    strcat(str,mid);
    strcat(str,end);
    char word[20];
    if (!(fp = fopen(str, "rt"))) {
        printf("Failed to open FILE\n");
        exit(1);
    }
    while ((c = fgetc(fp)) != EOF) {
    if (c!=' ')
    {
        word[i] = c;
        i++;
    }else if (c == ' ')
    {
        addend(nn,word,i);
        i = 0;
        struct linkedlist *node;
        node = *nn;
        while(node -> next != NULL)
        {
            // printf("%s\n", ((struct linkedlist *)node) -> value);
            node = node -> next;
        }
        // printf("%s\n", ((struct linkedlist *)node) -> value);
    }
    }
    fclose(fp);
    return 0;
}

void randomshuffle(struct linkedlist **nn,int a)
{
    char * shuffle[wordnum(a)];
    for (int i = 0; i < wordnum(a); i++)
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
    for (int i = wordnum(a) - 1; i > 0; --i) {
      int w = rand()%i;
      char * t = shuffle[i];
      shuffle[i] = shuffle[w];
      shuffle[w] = t;
    }
    current = *nn;
    for (int i = 0; i < wordnum(a); ++i)
    {
        current -> value = shuffle[i];
        current = current -> next;
        printf("**********%s\n", shuffle[i]);
    }
}

float scorecalculator(int time,int wrongword,int wordnum)
{
    float score = (3 * wordnum - wrongword ) / time;
    return (score / wordnum) ;
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
void print_text(GtkWidget *widget, GtkWidget *entry){
    const gchar * buff;
    buff = gtk_entry_get_text (GTK_ENTRY(widget));
    printf("%s\n",buff);
}


int main (int argc, char *argv[])
{
    srand(time(0));
    struct linkedlist * nn;
    nn = NULL;
    levelopener(1,&nn);
    randomshuffle(&nn,1);







    GtkBuilder *builder;
    GError *err = NULL;
    GtkWidget *window,*menubar,*menuitem1,*menuitem2,*menuitem3,*menuitem4,*label1,*label2,*label3,*progressbar,*entry,*button1,*button2,*button3,*button4,*button5,*button6,*button7,*button8,*button9,*button10,*label,*dialog;
    gtk_init (&argc, &argv);
    builder = gtk_builder_new ();
    printf("%s\n", err);

    gtk_builder_add_from_file (builder, "GUI.glade", &err);
    if (err){
        g_print(err->message);
        printf("error dad maskhare\n");
        printf("%s\n", err);
        return 1;
    }
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

    // gtk_init(&argc,&argv);
    // window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // grid = gtk_grid_new ();
    // entry = gtk_entry_new();
    // label = gtk_label_new("Player name:");
    // gtk_container_add(GTK_CONTAINER(window),grid);
    // gtk_grid_attach (grid,entry,1,0,1,1);
    // gtk_grid_attach (grid,label,0,0,1,1);
    // g_signal_connect(entry, "changed", G_CALLBACK(print_text), NULL);
    // gtk_window_set_resizable(window,FALSE);
    g_object_unref(G_OBJECT(builder));
    gtk_widget_show(GTK_WIDGET(window));
    gtk_main ();
    return 0;
}
