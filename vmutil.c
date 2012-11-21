//
//  vmutil.c
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

#include <mach/mach.h>
#include <stdbool.h>
#include <unistd.h>

#include "vmutil.h"
#include "util.h"

bool _vmdump(mach_port_t task, vm_address_t address, vm_size_t size, bool(^callback)(vm_address_t address, uint8_t *chunk, vm_size_t chunksz))
{
    bool ret=true;

    uint8_t out[VMDUMP_CHUNKSIZE];
    vm_size_t outsz;
    vm_address_t end_address = address+size;

    while (address < end_address) {
        vm_size_t szleft = end_address-address;
        vm_size_t rsize = (szleft > VMDUMP_CHUNKSIZE)? VMDUMP_CHUNKSIZE : szleft;

        kern_return_t kr= vm_read_overwrite(task, address, rsize, (vm_address_t)&out, &outsz);

        if (kr != KERN_SUCCESS && kr != KERN_INVALID_ADDRESS) {
            __error("vm_read returned %x: %s\n", kr, mach_error_string(kr));
            ret=false;
            break;
        }

        if (outsz == 0) {
            address += rsize;
            continue;
        }

        if (!callback(address, out, outsz)) {
            ret=true;
            break;
        }
        address += rsize;
    }

    return ret;
}

bool vmHexDump(mach_port_t task, vm_address_t address, vm_size_t count)
{
    bool ret=false;

    __block vm_address_t nextaddr = address;

    ret=_vmdump(task, address, count, ^bool(vm_address_t offset, uint8_t *out, vm_size_t outsz) {
        hexdump(out, outsz, offset);

        if (offset != nextaddr)
            printf("---\n");

        nextaddr = offset+outsz;

        return true;
    });

    return ret;
}

bool vmDump(int fd, mach_port_t task, vm_address_t address, vm_size_t count)
{
    bool ret=false;

    __block vm_address_t nextaddr = address;

    ret=_vmdump(task, address, count, ^bool(vm_address_t offset, uint8_t *out, vm_size_t outsz) {
        if (offset != nextaddr)
        return false;

        nextaddr = offset+outsz;
        write(fd, out, outsz);

        return true;
    });

    return ret;
}

bool vmRegionBasicInfo(vm_map_t task,  vm_address_t *address, vm_size_t *vmSize, vm_region_basic_info_data_t *info)
{
    vm_region_flavor_t flavor = VM_REGION_BASIC_INFO;
    mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT;
    memory_object_name_t unused;
    kern_return_t kr;

    kr = vm_region(task, address, vmSize, flavor, (vm_region_info_t)info, &info_count, &unused);

    if (kr == KERN_SUCCESS) {
        return true;
    } else {
        // KERN_INVALID_ADDRESS signifies that there is no region at or beyond the specified address - we just stop
        if (kr != KERN_INVALID_ADDRESS) {
            fprintf(stderr, "getVMRegionBasicInfo failed - vm_region error(%i): %s\n", kr, mach_error_string(kr));
        }
        return false;
    }

}

void vmForEachRegion(vm_map_t task, bool(^callback)(vm_region_basic_info_data_t *info, vm_address_t address, vm_size_t vmSize))
{
    kern_return_t kr = KERN_SUCCESS;
    vm_address_t address = 0;

    while(kr == KERN_SUCCESS) {
        vm_size_t vmsize;
        vm_region_basic_info_data_t info;

        if (! vmRegionBasicInfo(task, &address, &vmsize, &info) || 
            ! callback(&info, address, vmsize))
        {
            break;
        }

        address += vmsize;
    }
}

