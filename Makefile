SDKVER?=6.0

CSNAME="iPhone Developer"

DEVELOPER=/Applications/Xcode.app/Contents/Developer
PLATFORM=$(DEVELOPER)/Platforms/iPhoneOS.platform/Developer
SDK=$(PLATFORM)/SDKs/iPhoneOS$(SDKVER).sdk

CC=$(PLATFORM)/usr/bin/arm-apple-darwin10-llvm-gcc-4.2 
CFLAGS=-ggdb -Wall -isysroot $(SDK) -I../include

MACOSX=$(DEVELOPER)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks

PROGS=vmregions vmdump

all: $(PROGS)

vmdump: vmdump.o vmutil.o util.o
	$(CC) $(CFLAGS) -o $@ $^
	codesign -f -s $(CSNAME) --entitlements ents/dbg.xcent $@

vmregions: vmregions.o vmutil.o util.o
	$(CC) $(CFLAGS) -o $@ $^
	codesign -f -s $(CSNAME) --entitlements ents/dbg.xcent $@

clean:
	rm -rf *.o *.dSYM $(PROGS)

