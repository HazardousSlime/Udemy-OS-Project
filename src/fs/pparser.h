#ifndef PPARSER_H
#define PPARSER_H

struct path_root{
    int drive_no;
    struct path_part* first;
};

struct path_part{
    char* part;
    struct path_part* next;
};

int path_parse(const char* path, const char* current_directory_path, struct path_root* proot);

#endif //PPARSER_H