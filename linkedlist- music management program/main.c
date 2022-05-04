//
//  main.c
//  linkedlist- music management program
//
//  Created by 이성주 on 2020/10/28.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string_tools.h"
#include "library.h"


#define BUFFER_LENTH 200

void process_command(void);
void handle_add(void);
void handle_load(void);
void handle_search(void);
void handle_play(void);
void handle_save(void);
void handle_remove(void);


int main(void) {
    initialize();
    handle_load();
    process_command();
    return 0;
}

void handle_load() {
    char Buffer[BUFFER_LENTH];
    
        printf("What is your data file name? : ");
        if (read_line(stdin, Buffer, BUFFER_LENTH) <= 0) {
            return; //파일 이름 입력 x
        }
        FILE *fp = fopen(Buffer, "r");
        if (fp == NULL) {
            printf("No such file exists.\n");
            return;
        }
    
    load(fp);
    fclose(fp);
   
}

void process_command(void) {
    char command_line[BUFFER_LENTH];
    char *command;
    
    while (1) { //infinite loop
        printf("$ ");   //prompt
        
        if (read_line(stdin, command_line, BUFFER_LENTH) <= 0) { //read_line : get a user's command line
            continue; // if user typing just enter key, then continue
        }
        
        // find command word
        // use " " as delimeter
        command = strtok(command_line, " ");
        
        
        
        if (strcmp(command, "add") == 0) {
            handle_add();
        }
        else if (strcmp(command, "search") == 0) {
            handle_search();
        }
        else if (strcmp(command, "remove") == 0) {
            handle_remove();
        }
        else if (strcmp(command, "status") == 0) {
            status();
        }
        else if (strcmp(command, "play") == 0) {
            handle_play();
        }
        else if (strcmp(command, "save") == 0) {
            char *tmp = strtok(NULL, " ");
            if (strcmp(tmp, "as") != 0) {
                continue;
            }
            handle_save();

        }
        else if (strcmp(command, "exit")==0) {
            break;
        }
    }
}

void handle_remove() { // delete a song with an index number.
    char *id_str = strtok(NULL, " ");   //song index number
    int index = atoi(id_str);
    remove1(index);
    
}

void handle_play() { //노래 번호를 받아 와야함
    char *id_str = strtok(NULL, " ");   //need to get a song index number
    int index = atoi(id_str);
    play(index); 
}
void handle_search() {
    char name[BUFFER_LENTH], title[BUFFER_LENTH];
    
    printf("    Artist : ");
    if (read_line(stdin, name, BUFFER_LENTH) <= 0) {
        printf("    Artist name required.\n");
        return;
    }
    
    printf("    Title : ");
    int title_lenth = read_line(stdin, title, BUFFER_LENTH);
    if (title_lenth <= 0) { // Search with singer's name.
        search_song1(name);
    }
    else
        search_song2(name, title);
}

void handle_add(void) {
    char buffer[BUFFER_LENTH];
    char *artist = NULL, *title = NULL, *path = NULL;
    
    printf("    Artist : ");
    //read_line: Returns the number of read strings
    if (read_line(stdin, buffer, BUFFER_LENTH) > 0) {
        artist = strdup(buffer);
    }
    
    printf("    Title : ");
    //read_line: Returns the number of read strings
    if (read_line(stdin, buffer, BUFFER_LENTH) > 0) {
        title = strdup(buffer);
    }
    
    printf("    Path : ");
    //read_line: Returns the number of read strings
    if (read_line(stdin, buffer, BUFFER_LENTH) > 0) {
        path = strdup(buffer);
    }
    
    
    //middle check
    //printf("%s %s %s\n", artist, title, path);    //no problem
    
    /* add to the music library */
    add_song(artist, title, path);
    
    }

void handle_save() {
    char *file_name = strtok(NULL, " ");
    FILE *fp = fopen(file_name, "w");
    save(fp);
    fclose(fp);
    
}
