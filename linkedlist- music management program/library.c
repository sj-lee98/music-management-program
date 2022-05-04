//
//  library.c
//  linkedlist- music management program
//
//  Created by 이성주 on 2020/10/28.
//

#include "library.h"
#include "string_tools.h"
//#include <windows.h>


#define NUM_CHARS 256    //2^8 = 256
#define BUFFER_LENTH 200
#define SIZE_INDEX_TABLE 100

// Separate artists by their initials and organize each group into a one-way linked list
Artist *artist_directory[NUM_CHARS];    //name can be english or korean
Snode *index_directory[SIZE_INDEX_TABLE];
int num_index = 0;

Artist *find_artist(char *name);
Artist *create_artist_instance(char *name);
Song *create_song_instance(Artist *ptr_artist, char *title, char *path);
void insert_node(Artist *ptr_artist, Snode *ptr_snode);
void status(void);
void print_artist(Artist *p);
void print_song(Song *ptr_song);
Snode *find_snode(Artist *ptr_artist, char *title);
void insert_to_index_directory(Song *ptr_song);
Snode *find_song(int index);
void save_artist(Artist *p, FILE *fp);
void save_song(Song *ptr_song, FILE *fp);
void remove_snode(Artist *ptr_artist, Snode *ptr_snode);
void destroy_song(Song *ptr_song);


void initialize(void) {
    for (int i = 0; i < NUM_CHARS; i++) {
        artist_directory[i] = NULL ;
    }
    for (int i = 0; i < SIZE_INDEX_TABLE; i++) {
        index_directory[i] = NULL ;
    }
}

void load(FILE *fp) {
    char buffer[BUFFER_LENTH];
    char *name, *title, *path;
    
    while (1) { //infinite loop
        
        if (read_line(fp, buffer, BUFFER_LENTH) <= 0) {
            break;
        }
        name = strtok(buffer, "#");
        if (strcmp(name, " ") == 0) {
            name = NULL;
        }
        else
            name = strdup(name);
        
        title = strtok(NULL, "#");
        if (strcmp(title, " ") == 0) {
            title = NULL;
        }
        else
            title = strdup(title);
        
        path = strtok(NULL, "#");
        if (strcmp(path, " ") == 0) {
            path = NULL;
        }
        else
            path = strdup(path);
        
        // printf("%s %s %s\n", name, title, path);  //middle check->complete
        add_song(name, title, path);
    }
    
}




void search_song2(char *artist, char *title) {
    
    Artist *ptr_artist = find_artist(artist);
    if (ptr_artist == NULL) { // if there's no matching singer
        printf("No such artist exists.\n");
        return;
    }
    Snode *ptr_snode = find_snode(ptr_artist, title);
    
    if (ptr_snode != NULL) {
        printf("    Found:\n");
        print_song(ptr_snode->song);
    }
    else {
        printf("No such song exists.\n");
        return;
    }
    
}

void search_song1(char *artist) {
    
    Artist *ptr_artist = find_artist(artist);
    
    if (ptr_artist == NULL) { // if there's no matching singer
        printf("No such artist exists.\n");
        return;
    }
    else {
        printf("    Found:\n");
        print_artist(ptr_artist);
    }
    
}

Snode *find_snode(Artist *ptr_artist, char *title) {
    Snode *ptr_snode = ptr_artist->head;
    while (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) < 0) {
        ptr_snode = ptr_snode->next;
    }
    
    if (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) == 0)
        return ptr_snode;
    else
        return NULL;
}

Artist *create_artist_instance(char *name) {
    Artist *ptr_artist = (Artist *)malloc(sizeof(Artist));
    ptr_artist->name = name;
    ptr_artist->head = NULL;
    ptr_artist->next = NULL;
    ptr_artist->tail = NULL;
    
    return ptr_artist;
}

Artist *add_artist(char *name) {
    
    Artist *ptr_artist = create_artist_instance(name); //add node
    Artist *p = artist_directory[(unsigned char)name[0]]; //first node 'head'
    Artist *q = NULL;
    
    while (p != NULL && strcmp(p->name, name) < 0) {
        q = p;
        p = p->next;
    }
    
    if (p == NULL && q == NULL) { //unique node
        artist_directory[(unsigned char)name[0]] = ptr_artist;
    }
    else if (q == NULL) { //add at the front
        ptr_artist->next = p;
        artist_directory[(unsigned char)name[0]] = ptr_artist;
    }
    else { //add after q
        ptr_artist->next = p;
        q->next = ptr_artist;
    }
    return ptr_artist;
}

Song *create_song_instance(Artist *ptr_artist, char *title, char *path) {
    Song *ptr_song = (Song *)malloc(sizeof(Song));
    ptr_song->artist = ptr_artist;
    ptr_song->title = title;
    ptr_song->path = path;
    ptr_song->index = num_index;
    num_index++;
    
    return ptr_song;
}

void add_song(char *artist, char *title, char *path) {
    //find if the artist already exists
    //return NULL if not
    Artist *ptr_artist =  find_artist(artist);
    
    if (ptr_artist == NULL) {
        //if there's no matching singer
        ptr_artist =  add_artist(artist);
    }
    //add the song to the artist pointed by ptr_artist
    Song *ptr_song = create_song_instance(ptr_artist, title, path);
    Snode *ptr_snode = (Snode *)malloc(sizeof(Snode));
    ptr_snode->song = ptr_song;
    ptr_snode->next = NULL;
    ptr_snode->prev = NULL;
    
    
    
    //insert node
    insert_node(ptr_artist, ptr_snode);
    insert_to_index_directory(ptr_song);
}

void insert_to_index_directory(Song *ptr_song) {
    
    Snode *ptr_snode = (Snode *)malloc(sizeof(Snode));
    ptr_snode->song = ptr_song;
    ptr_snode->next = NULL;
    ptr_snode->prev = NULL; //unused
    
    int index = ptr_song->index % SIZE_INDEX_TABLE;
    
    //insert the snode into the single linked list at index_table[index]
    Snode *p = index_directory[index];
    Snode *q = NULL;
    while (p != NULL && strcmp(p->song->title, ptr_song->title) < 0) {
        q = p;
        p = p->next;
    }
    if (q == NULL) { //add first
        ptr_snode->next = p;
        index_directory[index] = ptr_snode;
    }
    else { //add after q
        ptr_snode->next = p;
        q->next = ptr_snode;
    }
}


void insert_node(Artist *ptr_artist, Snode *ptr_snode) {
    Snode *p = ptr_artist->head;
    while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0) {
        p = p->next;
    }
    
    //add ptr_snode before p
    //1.empty 2.first 3.last 4.at the middle
    if (ptr_artist->head == NULL) {  //empty
        ptr_artist->head = ptr_snode;
        ptr_artist->tail = ptr_snode;
    }
    else if (p == ptr_artist->head) { //first
        ptr_snode->next = ptr_artist->head;
        ptr_artist->head->prev = ptr_snode;
        ptr_artist->head = ptr_snode;
    }
    else if (p == NULL) { //last
        ptr_snode->prev = ptr_artist->tail;
        ptr_artist->tail->next = ptr_snode;
        ptr_artist->tail = ptr_snode;
        
    }
    else { // at the middle -> add before p
        ptr_snode->next = p;
        ptr_snode->prev = p->prev;
        p->prev->next = ptr_snode;
        p->prev = ptr_snode;
    }
    
}

Artist *find_artist(char *name) {
    
    Artist *p = artist_directory[(unsigned char)name[0]];     //first artist with initial name[0]
    while (p != NULL && strcmp(p->name, name) < 0) {
        p = p->next;
    }
    if (p != NULL && strcmp(p->name, name) == 0) {
        return p;
    }
    else
        return NULL;
}

//test middle
void status() {
    for (int i = 0; i < NUM_CHARS; i++) {
        Artist *p = artist_directory[i];
        while (p != NULL) {
            print_artist(p);
            p = p->next;
        }
    }
}

void print_artist(Artist *p) {
    
    printf("%s\n", p->name);
    
    Snode *ptr_snode = p->head;
    while (ptr_snode != NULL) {
        print_song(ptr_snode->song);
        ptr_snode = ptr_snode->next;
    }
}

void print_song(Song *ptr_song) {
    printf("    %d : %s %s\n", ptr_song->index, ptr_song->title, ptr_song->path);
}

Snode *find_song(int index) {
    Snode *ptr_snode = index_directory[index % SIZE_INDEX_TABLE];
    while (ptr_snode != NULL && ptr_snode->song->index != index) {
        ptr_snode = ptr_snode->next;
    }
    
    return ptr_snode;
}

void play(int index) {
    
    Snode *ptr_snode = find_song(index);
    if (ptr_snode == NULL) {
        printf("No such song exists.\n");
    }
    printf("Found the song : %s\n", ptr_snode->song->title);
    system("open valentin.mp3"); //for mac version
    //ShellExecute(GetDesktopWindow(), "open", ptr_snode->song->path, NULL, NULL, SW_SHOW);
}

void save(FILE *fp) {
    for (int i = 0; i < NUM_CHARS; i++) {
        Artist *p = artist_directory[i];
        while (p != NULL) {
            save_artist(p, fp);
            p = p->next;
        }
    }
}

void save_artist(Artist *p, FILE *fp) {
    
    Snode *ptr_snode = p->head;
    while (ptr_snode != NULL) {
        save_song(ptr_snode->song, fp);
        ptr_snode = ptr_snode->next;
    }
}

void save_song(Song *ptr_song, FILE *fp) {
    
    if (ptr_song->artist != NULL)
        fprintf(fp, "%s#", ptr_song->artist->name);
    else
        fprintf(fp, " #");
    
    if (ptr_song->title != NULL)
        fprintf(fp, "%s#", ptr_song->title);
    else
        fprintf(fp, " #");
    
    if (ptr_song->path != NULL)
        fprintf(fp, "%s#\n", ptr_song->path);
    else
        fprintf(fp, " #\n");
    
}

void remove1(int index) {
    //find the song with index
    Snode *q = NULL;                                    //previous to p
    Snode *p = index_directory[index % SIZE_INDEX_TABLE];   //head node
    while (p != NULL && p->song->index != index) {
        q = p;
        p = p->next;
    }
    
    if (p == NULL) { //either empty list or not exists.
        printf("No such song exists.\n");
        return;
    }
    Song *ptr_song = p->song;
    
    if (q == NULL) { //delete first node
        index_directory[index % SIZE_INDEX_TABLE] = p->next;
    }
    else {//remove after q
        q->next = p->next; //q->next = q->next->next
    }
    free(p); //Snode delete complete
    
    Artist *ptr_artist = ptr_song->artist;
    
    //find the snode in the double linked list of ptr_artist
    Snode *ptr_snode = find_snode(ptr_artist, ptr_song->title);
    if (ptr_song == NULL) {
        printf("Not found Snode - something wrong.\n");
        return;
    }
    
    remove_snode(ptr_artist, ptr_snode);
    destroy_song(ptr_song);
}

void destroy_song(Song *ptr_song) {
    if (ptr_song->title != NULL) {
        free(ptr_song->title);
    }
    if (ptr_song->path != NULL) {
        free(ptr_song->path);
    }
    free(ptr_song);
}

void remove_snode(Artist *ptr_artist, Snode *ptr_snode) {
    
    Snode *first = ptr_artist->head;
    Snode *last = ptr_artist->tail  ;
    
    if (first == ptr_snode && last == ptr_snode) { //unique node
        first = NULL;
        last = NULL;
    }
    else if (first == ptr_snode) { //remove first
        first = ptr_snode->next;
        first->prev = NULL;
    }
    else if (last == ptr_snode) { //remove last
        ptr_snode->prev = last;
        ptr_snode->next->prev = ptr_snode->prev;
    }
    else { //delete node in the middle
        ptr_snode->prev->next = ptr_snode->next;
        ptr_snode->next->prev = ptr_snode->prev;
    }
    free(ptr_snode);
}
