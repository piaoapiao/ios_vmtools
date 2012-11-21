//
//  vmutil.h
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

#ifndef bighouse_vmutil_h
#define bighouse_vmutil_h

#include <mach/mach.h>
#include <stdbool.h>

#define VMDUMP_CHUNKSIZE 2048
#define VMADDR_MAX 0x40000000

bool vmHexDump(mach_port_t task, vm_address_t address, vm_size_t count);
bool vmDump(int fd, mach_port_t task, vm_address_t address, vm_size_t count);
bool vmRegionBasicInfo(vm_map_t task,  vm_address_t *address, vm_size_t *vmSize, vm_region_basic_info_data_t *info);
void vmForEachRegion(vm_map_t task, bool(^callback)(vm_region_basic_info_data_t *info, vm_address_t address, vm_size_t vmSize));

#endif // bighouse_vmutil_h

