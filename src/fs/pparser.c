#include "fs/pparser.h"
#include <stddef.h>
#include <stdbool.h>
#include "status.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "kernel.h"

int path_get_drive_number(const char* path){
    if(     is_digit(path[0]) &&
            path[1] == ':' &&
            path[2] == '/')
        return char_to_digit(path[0]);
    return -1;
}

bool path_valid_char(char c){
    return true;
}

int path_parse(const char* path, struct path_root* proot){
    proot = kzalloc(sizeof(struct path_root));
    if(proot == NULL)
        return -EMEMORY;
    int drive_no = path_get_drive_number(path);
    if(drive_no < 0)
        return -EBADPATH;
    proot->drive_no = drive_no;
    //proot->first = NULL;
    struct path_part* prev_part = NULL;
    struct path_part* cur_part = NULL; 
    for(int i = 3, start = 3; ; ++i){
        if(path[i] == '/' || path[i] == '\0'){
            //Part can't be blank
            if(start == i){
                if(path[i] == '\0')
                    return PEACHOS_ALL_OK;
                return -EBADPATH;
            }
            //Allocate memory for part
            cur_part = kzalloc(sizeof(struct path_part));
            if(cur_part == NULL)
                return -EMEMORY;

            //Allocate memory for char string
            int n = i - start;
            if((cur_part->part = kzalloc(n)) == NULL)
                return -EMEMORY;
            strncpy(cur_part->part, path+start, n - 1);
            cur_part->part[i] = '\0';
            if(prev_part != NULL)
                prev_part->next = cur_part;
            else   
                proot->first = cur_part;
            prev_part = cur_part;
            //start at the index after the '/'
            start = i + 1;
            if(path[i] == '\0')
                break;
        }else if(!path_valid_char(path[i])){
            return -EBADPATH;
        }
    }
    return PEACHOS_ALL_OK;
}
