ios_vmtools
===========

Virtual Memory Dumping Tools For iOS


Building
========

    make

The makefile is set up to use the iOS 6.0sdk in the default location on OS X.
If this doesn't match your setup, you'll need to tweak it a bit (or a
lot if you're on windows or linux).

Usage
=====

      iPod:~ root# ./vmregions -h
      [-] Error: Invalid pid argument: -h
      Usage: ./vmregions <PID>

      iPod:~ root# ./vmdump -h
      ./vmdump: illegal option -- h
      usage: vmdump [rs:e:c:?] [pid]
         -r   raw dump on stdout (instead of hexdump)
         -s   start-address (required)
         -e   end-address (must be greater than -s)
         -c   byte-count

Usage Example
=============

Step 1 Pick a process:

      iPod:~ root# cat
      ^Z
      [1]+  Stopped(SIGTSTP)        cat
      iPod:~ root# ps |grep cat
        501 ttys002    0:00.01 cat

Step 2 list its regions in memory with vmregions:

      iPod:~ root# ./vmregions 501
      task_for_pid RC 0 - Task: 4355
      [  idx] address-range          size  perm/max     behavior  sharing
      [    0] 00001000-00014000       76K  r-x/r-x         COPY   private     DEFAULT
      [    1] 00018000-00019000        4K  r--/rwx         COPY   private     DEFAULT
      [    2] 00019000-0001a000        4K  rw-/rwx         COPY   private     DEFAULT
      [    3] 0001a000-0001b000        4K  ---/rwx         COPY   private     DEFAULT
      [    4] 0001b000-00025000       40K  rw-/rwx         COPY   private     DEFAULT
      [    5] 00025000-00026000        4K  ---/rwx         COPY   private     DEFAULT
      [    6] 00026000-00027000        4K  ---/rwx         COPY   private     DEFAULT
      [    7] 00027000-00031000       40K  rw-/rwx         COPY   private     DEFAULT
      [    8] 00031000-00032000        4K  ---/rwx         COPY   private     DEFAULT
      [    9] 00032000-00033000        4K  r--/rwx         COPY   private     DEFAULT
      [   10] 00033000-00034000        4K  rw-/rwx         COPY   private     DEFAULT
      [   11] 00034000-00045000       68K  rw-/rwx         COPY   private     DEFAULT
      [   12] 00045000-00046000        4K  rw-/rwx         COPY   private     DEFAULT
      [   13] 00100000-00200000     1024K  rw-/rwx         COPY   private     DEFAULT
      [   14] 00800000-01000000     8192K  rw-/rwx         COPY   private     DEFAULT
      ...
      [   31] 3b200000-40000000    79872K  r--/rwx        SHARE   private     DEFAULT
      Found 32 regions

Step 3 dump some memory!. (in this case the __TEXT,__text segment):

      iPod:~ root# ./vmdump -s 0x1000 -e 0x14000 501|head -20
      [+] task_for_pid for pid: 501 returned Task: 4355
      [+] dumping memory from 0x1000 to 0x14000
      00001000: CE FA ED FE 0C 00 00 00   00 00 00 00 02 00 00 00  |................|
      00001010: 0E 00 00 00 E0 05 00 00   85 00 00 00 01 00 00 00  |................|
      00001020: 38 00 00 00 5F 5F 50 41   47 45 5A 45 52 4F 00 00  |8...__PAGEZERO..|
      00001030: 00 00 00 00 00 00 00 00   00 10 00 00 00 00 00 00  |................|
      00001040: 00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00  |................|
      00001050: 00 00 00 00 01 00 00 00   48 01 00 00 5F 5F 54 45  |........H...__TE|
      00001060: 58 54 00 00 00 00 00 00   00 00 00 00 00 10 00 00  |XT..............|
      00001070: 00 30 01 00 00 00 00 00   00 30 01 00 05 00 00 00  |.0.......0......|
      00001080: 05 00 00 00 04 00 00 00   00 00 00 00 5F 5F 74 65  |............__te|
      00001090: 78 74 00 00 00 00 00 00   00 00 00 00 5F 5F 54 45  |xt..........__TE|
      000010a0: 58 54 00 00 00 00 00 00   00 00 00 00 00 20 00 00  |XT........... ..|
      000010b0: 04 FE 00 00 00 10 00 00   04 00 00 00 00 00 00 00  |................|
      000010c0: 00 00 00 00 00 04 00 80   00 00 00 00 00 00 00 00  |................|
      000010d0: 5F 5F 73 79 6D 62 6F 6C   5F 73 74 75 62 34 00 00  |__symbol_stub4..|
      000010e0: 5F 5F 54 45 58 54 00 00   00 00 00 00 00 00 00 00  |__TEXT..........|
      000010f0: 04 1E 01 00 B4 03 00 00   04 0E 01 00 02 00 00 00  |................|
      00001100: 00 00 00 00 00 00 00 00   08 04 00 80 00 00 00 00  |................|
      00001110: 0C 00 00 00 5F 5F 63 73   74 72 69 6E 67 00 00 00  |....__cstring...|
      00001120: 00 00 00 00 5F 5F 54 45   58 54 00 00 00 00 00 00  |....__TEXT......|
      00001130: 00 00 00 00 B8 21 01 00   22 0D 00 00 B8 11 01 00  |.....!..".......|
      ...

License
=======

Copyright (c) 2012 Eric Monti

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

