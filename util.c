//
//  util.c
//
//  Created by Eric Monti on 5/1/12.

/* Copyright (c) 2012 Eric Monti

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "util.h"

void hexify(unsigned char *data, uint32_t size)
{
    while(size--) 
        printf("%02x", *data++);
}

void hexdump(unsigned char *data, uint32_t size, uint32_t dmpOffset)
{
    unsigned char *p = data;
    unsigned char c;
    uint32_t n;
    char bytestr[4] = {0};
    char addrstr[10] = {0};
    char hexstr[ 16*3 + 5] = {0};
    char charstr[16*1 + 5] = {0};

    for(n=1;n<=size;n++) {
        if (n%16 == 1) {
            /* store address for this line */
            snprintf(addrstr, sizeof(addrstr), "%.8x",
                    dmpOffset+(uint32_t)(p-data) );
        }

        c = *p;
        if (c < 0x20 || c > 0x7e) {
            c = '.';
        }

        /* store hex str (for left side) */
        snprintf(bytestr, sizeof(bytestr), "%02X ", *p);
        strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);

        /* store char str (for right side) */
        snprintf(bytestr, sizeof(bytestr), "%c", c);
        strncat(charstr, bytestr, sizeof(charstr)-strlen(charstr)-1);

        if(n%16 == 0) { 
            /* line completed */
            printf("%8.8s: %-50.50s |%s|\n", addrstr, hexstr, charstr);
            hexstr[0] = 0;
            charstr[0] = 0;
        } else if(n%8 == 0) {
            /* half line: add whitespaces */
            strncat(hexstr, "  ", sizeof(hexstr)-strlen(hexstr)-1);
            //strncat(charstr, " ", sizeof(charstr)-strlen(charstr)-1);
        }
        p++; /* next byte */
    }

    if (strlen(hexstr) > 0) {
        /* print rest of buffer if not empty */
        printf("%8.8s: %-50.50s |%s|\n", addrstr, hexstr, charstr);
    }
}


bool with_file_mmap_ro(char *filename, void(^callback)(void *filedata))
{
    bool ret=false;
    void *filedata;
    int fd;
    struct stat st = {0};

    fd=open(filename, O_RDONLY);
    if (fd >= 0) {
        if (fstat(fd, &st) >= 0) {
            filedata = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (filedata) {
                callback(filedata);
                munmap(filedata, st.st_size);
                ret=true;
            } else {
                __perror("Cannot mmap()");
            }
        } else {
            __perror("Cannot fstat() file-descriptor");
        }
        close(fd);
    } else {
        __perror("Cannot open file");
    }

    return ret;
}

