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
#include <mach/mach.h>

#include "vmutil.h"
#include "util.h"

#define MAX_INHERITANCE 4
const char *
inheritanceStrings[] = {
    "SHARE", "COPY", "NONE", "DONATE_COPY",
};

#define MAX_BEHAVIOR 6
const char *
behaviorStrings[] = {
    "DEFAULT", "RANDOM", "SEQUENTIAL", "RESQNTL", "WILLNEED", "DONTNEED",
};

int main(int argc, char **argv)
{

    kern_return_t rc;
    mach_port_t task;

    if (argc > 2) {
        printf ("Usage: %s <PID>\n", argv[0]); exit (1);
    } else if (argc == 2) {
        char *endp=NULL;
        int pid = strtoul(argv[1], &endp, 0);
        if (endp != argv[1]+strlen(argv[1])) {
            __error("Invalid pid argument: %s\n", argv[1]);
            printf ("Usage: %s <PID>\n", argv[0]); exit (1);
        }

        rc = task_for_pid(mach_task_self(),pid, &task);
        if (rc) {
            __error("task_for_pid() failed: %s\n", mach_error_string(rc));
            exit(1);
        }
        printf ("task_for_pid RC %d - Task: %d\n", rc, task);

    } else {
        task=mach_task_self();
        printf ("Dumping info for mach_task_self - Task: %d\n", task);
    }

    __block uint32_t n=0;

    printf("[  idx] address-range          size  perm/max     behavior  sharing\n");
    vmForEachRegion(task, ^bool(vm_region_basic_info_data_t *info, vm_address_t address, vm_size_t vmsize) {
        if (info->max_protection & VM_PROT_EXECUTE) {
            printf("[ %4u] ", n);
            printf("%08x-%08x %8uK  %c%c%c/%c%c%c  %11s   %6s  %10s\n",
                (unsigned int)address, (unsigned int)(address + vmsize), (unsigned int)(vmsize >> 10),
                (info->protection & VM_PROT_READ)        ? 'r' : '-',
                (info->protection & VM_PROT_WRITE)       ? 'w' : '-',
                (info->protection & VM_PROT_EXECUTE)     ? 'x' : '-',
                (info->max_protection & VM_PROT_READ)    ? 'r' : '-',
                (info->max_protection & VM_PROT_WRITE)   ? 'w' : '-',
                (info->max_protection & VM_PROT_EXECUTE) ? 'x' : '-',
                (info->inheritance < MAX_INHERITANCE)? inheritanceStrings[info->inheritance] : "unk",
                (info->shared)? "shared" : "private",
                (((unsigned int) info->behavior) < MAX_BEHAVIOR)? behaviorStrings[info->behavior] : "unk");

            n++;
        }
        return true;
    });    
    printf("Found %d regions\n", n);
}
