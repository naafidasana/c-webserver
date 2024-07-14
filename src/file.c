#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../headers/file.h"


/**
 * Routine for loading a file into memory
 *  - returns a pointer to the data
 */
file_data* load_file(char* fname)
{
    char* buffer, *write_pointer;
    struct stat buf;
    int bytes_read, bytes_remaining, total_bytes = 0;

    // Get file size
    if (stat(fname, &buf) == -1)
        return NULL;
    
    // Check that file is a regular file
    if (!(buf.st_mode & S_IFREG))
        return NULL;    // File is not a regular file
    
    // Open file
    FILE* fpointer = fopen(fname, "rb");

    if (fpointer == NULL)
        return NULL;    // Failed to open file
    
    // Allocate enough memory to store file content
    bytes_remaining = buf.st_size;
    write_pointer = buffer = malloc(bytes_remaining);

    if (buffer == NULL)
    {
        fclose(fpointer);
        return NULL;    // Memory allocation failed.
    }
    
    // Read entire file
    while ((bytes_read = fread(write_pointer, 1, bytes_remaining, fpointer)) != 0 && bytes_remaining > 0)
    {
        if (ferror(fpointer))
        {
            free(buffer);
            fclose(fpointer);
            return NULL;
        }

        bytes_remaining -= bytes_read;
        write_pointer += bytes_read;    // Advance pointer to read next block of data
        total_bytes += bytes_read;
    }

    fclose(fpointer);   // Close file, we no longer need it.

    // Allocate file_data struct
    file_data* fdata = malloc(sizeof(*fdata));

    if (fdata == NULL)
    {
        // Memory allocation failed
        free(buffer);
        return NULL;
    }

    fdata->size = total_bytes;
    fdata->data = buffer;

    return fdata;
}

/**
 * Routine for freeing memory allocated by laod_file
 */
void free_file(file_data* fdata)
{
    free(fdata->data);
    free(fdata);
}