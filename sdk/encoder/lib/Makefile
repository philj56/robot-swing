# Makefile for pmd1208fs
# Copyright 2005, 2009, 2015   Mark Colclough

PACKAGE = pmd1208fs
REALLIB = libpmd1208fs.so.2.2
SONAME = libpmd1208fs.so.2
LINKERNAME = libpmd1208fs.so


# Any commandline DESTDIR gets prepended to these before installation
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
sysconfdir = /etc
includedir = $(prefix)/include
datarootdir = $(prefix)/share
docdir = $(datarootdir)/doc/pmd1208fs
libdirsuffix = 
libdir = $(exec_prefix)/lib$(libdirsuffix)

CFLAGS = -Wall -Wextra -O2
DEFINES =

DOCS = README demo.c demo.py

all: $(REALLIB) demo

debug: DEFINES += -DDEBUG
debug: CFLAGS += -g
debug: all

# The symlinks let the demo link and run (with LD_LIBRARY_PATH=.) before installation
$(REALLIB): libpmd1208fs.o
	gcc -shared -Wl,-soname,$(SONAME) -o $@ $^ -lusb-1.0 -lpthread
	ln -s $(REALLIB) $(LINKERNAME)
	ln -s $(REALLIB) $(SONAME)

libpmd1208fs.o: libpmd1208fs.c pmd1208fs.h
	gcc -fPIC -c $(CFLAGS) $(DEFINES) $<

demo: demo.c pmd1208fs.h
	gcc -o $@ $<  -L. -lpmd1208fs

test: demo
	LD_LIBRARY_PATH=. ./demo

install: all
	mkdir -p $(DESTDIR)$(includedir)
	install -m 644 pmd1208fs.h $(DESTDIR)$(includedir)
	
	mkdir -p $(DESTDIR)$(libdir)
	install -m 755 $(REALLIB) $(DESTDIR)$(libdir)
	cd $(DESTDIR)$(libdir); rm -f $(LINKERNAME); ln -s $(REALLIB) $(LINKERNAME)
	cd $(DESTDIR)$(libdir); rm -f $(SONAME); ln -s $(REALLIB) $(SONAME)
	
	mkdir -p $(DESTDIR)$(sysconfdir)/udev/rules.d/
	install -m 644 80-pmd.rules $(DESTDIR)$(sysconfdir)/udev/rules.d/
	
	mkdir -p $(DESTDIR)$(docdir)
	install -m 644 $(DOCS) $(DESTDIR)$(docdir)
	
	python setup.py install --root /$(DESTDIR)
	
	ldconfig

uninstall:
	rm -f $(DESTDIR)$(includedir)/pmd1208fs.h
	rm -f $(DESTDIR)$(libdir)/$(LINKERNAME)*
	rm -f $(DESTDIR)$(sysconfdir)/udev/rules.d/80-pmd.rules
	cd $(DESTDIR)$(docdir); rm $(DOCS)
	rmdir $(DESTDIR)$(docdir)
# python setuptools does not have an uninstall.  
# Uninstall is better done by the package manager anyway.

clean:
	rm -f libpmd1208fs.o demo $(SONAME) $(LINKERNAME) $(REALLIB)
	rm -rf build
# build is made by setup.py install,  but setup.py clean does not remove it.


# Make a distribution tarball from a Mercurial archive.
#
# Assume we can use hg if we are in a Mercurial archive
ifeq "$(shell test -d .hg && echo 'hg')" "hg"
# We force into the tarball a version.h file, and set the VERSION= line
# in the slackbuild script: both with the version of the tarball being made.
#
# If invoked as  'make dist tag=1.1'  the revision
# labelled by a preexisting tag is packaged with the name
# $(PACKAGE)-$(tag).tar.gz .  If no tag is specified, the tip is used, and the
# tarball version is the revno, with optional prefix specified in DEV.
# Requires PACKAGE and optionally tag to be set elsewhere.
#
# The false clause of DEV can add a prefix to the revno for non-tagged tarballs
DEV := $(if $(tag),,dev)
REV := $(if $(tag),$(tag),$(shell hg tip --template '{rev}\n'))
PKGVER := $(DEV)$(REV)
dist:
	rm -rf $(PACKAGE)-$(PKGVER)
	hg archive -t files -r $(REV) $(PACKAGE)-$(PKGVER)
	echo "#define VERSION \"$(PKGVER)\"" > $(PACKAGE)-$(PKGVER)/version.h
	sed -i 's/^VERSION=.*/VERSION=$${VERSION:-$(PKGVER)}/' $(PACKAGE)-$(PKGVER)/$(PACKAGE).SlackBuild; true	
	tar cfz $(PACKAGE)-$(PKGVER).tar.gz $(PACKAGE)-$(PKGVER)
	rm -r $(PACKAGE)-$(PKGVER)

endif
# hg

.PHONY: all debug test install uninstall clean dist
