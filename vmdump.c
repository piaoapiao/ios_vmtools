//
//  vmregions.c
//
//  Created by Eric Monti on 11/16/12.
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#include "util.h"
#include "vmutil.h"

void usage(char *progname) {
    fprintf(stderr, "usage: %s [rs:e:c:?] [pid]\n", progname);
    fprintf(stderr, "   -r   raw dump on stdout (instead of hexdump)\n");
    fprintf(stderr, "   -s   start-address (required)\n");
    fprintf(stderr, "   -e   end-address (must be greater than -s)\n");
    fprintf(stderr, "   -c   byte-count\n");
}

int main(int argc, char **argv)
{
    mach_port_t task;
    vm_address_t addr=0, end=0;
    vm_size_t count=0;
    int ch;
    int dumpraw=0; // none

    char *endp, *progname=basename(argv[0]);

    int ret=0;
    while ((ch = getopt(argc,argv,"rs:e:c:")) != -1) {
        switch(ch) {
            case 's':
                addr = strtoul(optarg, &endp, 0);
                if (endp != optarg+strlen(optarg)) {
                    __error("Invalid addr argument: %s\n", optarg);
                    ret=1;
                }
                break;
            case 'c':
                count = strtoul(optarg, &endp, 0);
                if (!count || endp != optarg+strlen(optarg)) {
                    __error("Invalid count argument: %s\n", optarg);
                    ret=1;
                }
                break;
            case 'e':
                end = strtoul(optarg, &endp, 0);
                if (!end || endp != optarg+strlen(optarg)) {
                    __error("Invalid count argument: %s\n", optarg);
                    ret=1;
                }
                break;
            case 'r':
                dumpraw=1; //raw
                break;
            case '?':
            default:
                usage(progname);
                exit(1);
                break;
        }
    }
    
    if (ret)
        exit(ret);

    if (!addr || addr >= VMADDR_MAX) {
        __error("address out of bounds: 0x%x\n", (unsigned int)addr);
        usage(progname);
        exit(1);
    }

    argc -= optind;
    argv += optind;

    if (argc > 1) {
        usage(progname);
        exit(1);
    } else if (argc == 1) {
        int pid = atoi(argv[0]);
        kern_return_t rc = task_for_pid(mach_task_self(), pid, &task);
        if (rc) {
            __error("Unable to attach to pid: %d - %s\n", pid, mach_error_string(rc));
            usage(progname);
            exit(1);
        }
        __status("task_for_pid for pid: %d returned Task: %d\n", pid, task);

    } else {
        task=mach_task_self();
        __status("Dumping info for mach_task_self - Task: %d\n", task);
    }

    if (end && count) {
        __error("Don't specify -e and -c together\n");
        usage(progname);
        exit(1);
    } else if (end && end <= addr) {
        __error("invalid argument for -e: %x\n", (unsigned int)end);
        usage(progname);
        exit(1);
    } else if (end) {
        count = end-addr;
    }

    if (!count)
        count = (VMADDR_MAX-addr);

    __status("dumping memory from 0x%x to 0x%x\n", (unsigned int)addr, (unsigned int)(addr+count));

    if (dumpraw) 
        ret = !vmDump(fileno(stdout), task, addr, count);
    else
        ret = !vmHexDump(task, addr,count);

    if (ret)
        __error("Unable to dump memory from 0x%x to 0x%x\n", (unsigned int)addr, (unsigned int)(addr+count));

    exit(ret);
}
