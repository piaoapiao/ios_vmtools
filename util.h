//
//  util.h
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

#ifndef bighouse_util_h
#define bighouse_util_h

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define __error(...) fprintf(stderr, "[-] Error: " __VA_ARGS__)
#define __status(...) fprintf(stderr, "[+] " __VA_ARGS__)
#define __perror(msg) perror("[-] " msg)

#define swap32(n32) OSSwapBigToHostInt32(n32)

void
hexify(unsigned char *data, uint32_t size);

void
hexdump(unsigned char *data, uint32_t size, uint32_t dmpOffset);

bool
with_file_mmap_ro(char *filename, void(^callback)(void *filedata));

#endif

