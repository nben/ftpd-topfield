-include config.mak

VERSION=0.7.4

LDLIBS += -Llibtopfield -ltopfield

ifdef USE_LIBUSB
LIBUSB ?= -lusb
LDLIBS += $(LIBUSB)
endif

#CFLAGS += -DDEBUG #-DIGNORE_NO_TOPFIELD
CFLAGS += -I . -Ilibtopfield
LFLAGS += -g

PKG=ftpd-topfield-$(VERSION)

CFLAGS += -std=gnu99 -Wall -D_FILE_OFFSET_BITS=64 -O3 -g -fexpensive-optimizations -fomit-frame-pointer -frename-registers
CFLAGS += -D_GNU_SOURCE
CFLAGS += -DPATH_FTPDPID=\"/var/run/ftpd-topfield.pid\"
CFLAGS += -DHAVE_CONFIG_H=1
CFLAGS += -DPATH_FTPWELCOME=\"ftpwelcome\"
CFLAGS += -DPATH_FTPLOGINMESG=\"motd\"
CFLAGS += -DPACKAGE=\"ftpd-topfield\" -DPACKAGE_NAME=\"Topfield\ ftpd\" -DPACKAGE_VERSION=\"$(VERSION)\"

OBJS = ftpd.o auth.o conf.o ftpcmd.o localhost.o server_mode.o xmalloc.o

all: ftpd

ftpd: libs $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean: libs-clean
	$(RM) *.o ftpd core core.*
	$(RM) tags ftpd.pid ftpd.log topfield*.tar.gz

libs:
	if [ ! -d libtopfield ]; then ln -s ../libtopfield .; fi
	$(MAKE) -C libtopfield

libs-clean:
	[ ! -d libtopfield ] || $(MAKE) -C libtopfield clean
	-$(RM) libtopfield

test:
	./ftpd -d -D -P 2021

install:
	install -d $(DESTDIR)/usr/sbin $(DESTDIR)/usr/share/man/man8
	install ftpd $(DESTDIR)/usr/sbin/ftpd-topfield
	install -m644 ftpd.8 $(DESTDIR)/usr/share/man/man8/ftpd-topfield.8

dist: libs
	mkdir $(PKG)
	cp *.c *.h *.y *.mak ftpd.8 Makefile ChangeLog* README* $(PKG)
	mkdir $(PKG)/libtopfield
	cp libtopfield/*.[ch] *.mak libtopfield/LICENCE libtopfield/Makefile $(PKG)/libtopfield
	tar -czf $(PKG).tar.gz $(PKG)
	rm -rf $(PKG)
