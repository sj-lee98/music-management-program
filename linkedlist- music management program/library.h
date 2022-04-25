//
//  library.h
//  linkedlist- music management program
//
//  Created by 이성주 on 2020/10/28.
//

#ifndef library_h
#define library_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct song Song;
typedef struct snode Snode;
typedef struct artist Artist;

struct song  {
    Artist *artist;
    char *title;
    char *path;
    int  index;
};

struct snode {
    struct snode *next, *prev;
    Song *song;
};

struct artist {
    char * name;
    struct artist *next;
    Snode *head, *tail;
};

void add_song(char *artist, char *title, char *path);
void initialize(void);
void status(void);
void load(FILE *fp);
void search_song1(char *artist);
void search_song2(char *artist, char *title);
void play(int index);
void save(FILE *fp);
void remove1(int index);

#endif /* library_h */
