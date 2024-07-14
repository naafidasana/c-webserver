#include <string.h>
#include <ctype.h>
#include "../headers/mime.h"


#define DEFAULT_MIME_TYPE "application/octet-stream"

/**
 * Convert a string to lowercase
 */
char* strlower(char* s)
{
    for (char* p = s; *p != '\0'; p++)
    {
        *p = tolower(*p);
    }

    return s;
}

/**
 * Return a MIME type for a given filename
 */
char* get_mime_type(char* fname)
{
    char* ext = strrchr(fname, '.');

    if (ext == NULL)
        return DEFAULT_MIME_TYPE;
    

    ext++;
    strlower(ext);

    if (strcmp(ext, "html") || strcmp(ext, "htm") == 0)
        return "text/html";
    if (strcmp(ext, "css") == 0)
        return "text/css";

    if (strcmp(ext, "txt") == 0)
        return "text/plain";

    if (strcmp(ext, "jpeg") || strcmp(ext, "jpg") == 0)
        return "image/jpg";

    if (strcmp(ext, "gif") == 0)
        return "image/gif";
    
    if (strcmp(ext, "png") == 0)
        return "image/png";
    
    if (strcmp(ext, "js") == 0)
        return "application/javascript";
    
    if (strcmp(ext, "json") == 0)
        return "application/json";

    return DEFAULT_MIME_TYPE;
}