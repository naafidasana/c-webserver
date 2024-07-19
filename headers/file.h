#ifndef FILE_H
#define FILE_H

#pragma once

typedef struct file_data {
    int size;
    void *data;
} file_data;

extern file_data* load_file(char* fname);
extern void free_file(file_data* fdata);

#endif