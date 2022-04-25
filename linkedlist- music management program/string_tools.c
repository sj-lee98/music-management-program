//
//  string_tools.c
//  linkedlist- music management program
//
//  Created by 이성주 on 2020/10/28.
//

#include "string_tools.h"
 
int read_line(FILE *fp, char str[], int n) {
    int i = 0, ch = 0;
    
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (i < n -1) {
            str[i++] = ch;
        }
    }
    str[i] = '\0';
    
    return i;   //읽은 글자의 갯수 반환
}
