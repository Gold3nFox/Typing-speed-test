#include <stdio.h>
#include "string.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#define falo printf("alo\n");

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

struct linkedlist
{
	char * value;
	int length;
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
		((struct linkedlist*)*list) -> length = length;
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
	nn -> length = length;
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

void uppermatches (char *str,char *str2)
{
	int i = 0;

}

int main(void)
{
	srand(time(0));
	struct linkedlist * nn;
	nn = NULL;
	levelopener(3,&nn);
	char * str1 = "salam";
	randomshuffle(&nn,3);
	printf("%d---<>",isinscoreboard(str1,400000));
	return 0;
}