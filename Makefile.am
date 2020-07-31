#!@ENV@ make -f

#      _____________________________________________________________________
#     /                                                                     \
#     |                         -*- Makefile -*-                            |
#     |_____________________________________________________________________|
#    //                                                                     \\
#   [|  GENETIC 2.0 © 2014-2016 Antonio Cao (@burzumishi)                    |]
#    \\_____________________________________________________________________//

CC = gcc
CPP = cpp
CXX = g++

USER = root
GROUP = root

ARCH = `getconf LONG_BIT`

install_dir = install -v -d -m 0755
install_file = install -v -m 644
install_bin = install -v -m 755

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = etc src po

EXTRA_DIST = m4/ChangeLog  m4/ChangeLog scripts config.rpath m4/ChangeLog autogen.sh \
	doc etc src man README.md LICENSE m4 libtool autom4te.cache \
	missing compile config.guess config.sub depcomp aclocal.m4 \
	Doxyfile.in po ABOUT-NLS *.m4 TODO genuine-installer.desktop \
	pixmaps icons

do_subst = sed -e 's,[@]sharedstatedir[@],$(sharedstatedir),g' \
		-e 's,[@]localstatedir[@],$(localstatedir),g' \
		-e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		-e 's,[@]includedir[@],$(includedir),g' \
		-e 's,[@]datarootdir[@],$(datarootdir),g' \
		-e 's,[@]localedir[@],$(localedir),g' \
		-e 's,[@]prefix[@],$(prefix),g' \
		-e 's,[@]bindir[@],$(bindir),g' \
		-e 's,[@]sbindir[@],$(sbindir),g' \
		-e 's,[@]libdir[@],$(libdir),g' \
		-e 's,[@]mandir[@],$(mandir),g' \
		-e 's,[@]PERL[@],$(PERL),g' \
		-e 's,[@]ENV[@],$(ENV),g' \
		-e 's,[@]BASH[@],$(BASH),g' \
		-e 's,[@]USER[@],$(USER),g' \
		-e 's,[@]GROUP[@],$(GROUP),g' \
		-e 's,[@]CC[@],$(CC),g' \
		-e 's,[@]CXX[@],$(CXX),g' \
		-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
		-e 's,[@]VERSION[@],$(VERSION),g' \
		-e 's,[@]DESCRIPTION[@],$(DESCRIPTION),g'

docdir = $(datarootdir)/doc/@PACKAGE@-@VERSION@
doc_DATA = README AUTHORS INSTALL NEWS LICENSE ChangeLog \
	doc/*.txt ABOUT-NLS

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

man_MANS = man/genetic.1

CLEANDEVELFILES = compile configure depcomp genetic.h src/*.la ar-lib \
	autom4te.cache config.guess config.sub install-sh missing src/.libs \
	config.status ltmain.sh depcomp README COPYING NEWS po/*.gmo m4 *.m4 \
	src/genetic$(EXEEXT) src/genbuild$(EXEEXT) src/geninstall$(EXEEXT) \
	src/genuninstall$(EXEEXT) Doxyfile po/POTFILES po/stamp-po \
	AUTHORS ChangeLog INSTALL config.lt libtool src/.deps src/*.o src/*.lo \
	`find $(srcdir) -iname "Makefile.in"` \
	`find $(srcdir) -iname "*.autogen-bak"` \
	`find $(srcdir) -iname "*~"`

CLEANBUILDFILES = config.log etc/genetic.conf etc/conf.d/repos.conf \
	etc/conf.d/arch.conf etc/conf.d/build.conf etc/conf.d/db.conf \
	etc/conf.d/editor.conf etc/conf.d/log.conf etc/conf.d/release.conf \
	etc/conf.d/installer.conf \
	`find $(srcdir) -iname "stamp-h1"` \
	`find $(srcdir) -iname "stamp-po"` \
	`find $(srcdir) -iname "Makefile"`

CLEANFILES = $(CLEANBUILDFILES) \
	`find $(srcdir) -iname "*~"`

if HAVE_DOXYGEN
all-doc:
	@$(DOXYGEN) Doxyfile;

clean-doc:
	@rm -rf $(top_builddir)/doc/latex $(top_builddir)/doc/html;

distclean-doc:
	@rm -rf $(top_builddir)/doc/latex $(top_builddir)/doc/html;

install-doc: all-doc
	@$(mkdir_p) $(DESTDIR)$(docdir); \
	cp -rvf doc/html $(DESTDIR)$(docdir);

uninstall-doc:
	@rm -rvf $(DESTDIR)$(docdir)/html; \
	rm -rvf $(DESTDIR)$(docdir);
else
all-doc:
	@echo "*** WARNING! Doxygen is not available ***"; \
	echo "*** Documentation will not be built ***";

clean-doc:
	@echo "*** WARNING! Doxygen is not available ***"; \
	echo "*** Documentation will not be cleaned ***";

distclean-doc:
	@echo "*** WARNING! Doxygen is not available ***"; \
	echo "*** Documentation will not be cleaned ***";

install-doc:
	@echo "*** WARNING! Doxygen is not available ***"; \
	echo "*** Documentation will not be installed ***";

uninstall-doc:
	@echo "*** WARNING! Doxygen is not available ***"; \
	echo "*** Documentation will not be uninstalled ***";
endif

all-local:
	@echo "=== all-local done ==="

clean-local:
	@echo "=== clean-local done ==="

distclean-local: clean-local
	@echo "=== distclean-local done ==="

devdistclean: distclean-local distclean-doc
	@rm -rfv $(CLEANDEVELFILES) $(CLEANBUILDFILES); \
	echo "=== distclean-local done ==="

install-data-hook:
	@echo "Creating <@PACKAGE@> directories to store desktop files ..."; \
	$(install_dir) $(DESTDIR)$(datarootdir)/applications; \
	$(install_dir) $(DESTDIR)$(datarootdir)/@PACKAGE@/scripts; \
	echo "Creating <@PACKAGE@> directory to store log files ..."; \
	$(install_dir) $(DESTDIR)$(localstatedir)/log/@PACKAGE@; \
	echo "Creating <@PACKAGE@> directories to store lock and run files ..."; \
	$(install_dir) $(DESTDIR)/run/lock/@PACKAGE@; \
	$(install_dir) $(DESTDIR)/run/@PACKAGE@; \
	ln -sfv /run/lock $(DESTDIR)$(localstatedir)/lock; \
	ln -sfv /run $(DESTDIR)/$(localstatedir)/run; \
	echo "Installing <@PACKAGE@> files to $(DESTDIR) ..."; \
	cp -rfv icons $(DESTDIR)$(datarootdir)/icons; \
	cp -rfv pixmaps $(DESTDIR)$(datarootdir)/pixmaps; \
	$(_install_file_) genuine-installer.desktop \
										$(DESTDIR)$(datarootdir)/applications/genuine-installer.desktop; \
	echo "Setting up <$(prefix)> permissions to <$(USER):$(GROUP)> ..."; \
	chown -R $(USER):$(GROUP) $(DESTDIR)$(localstatedir)/log/@PACKAGE@; \
	chown -R $(USER):$(GROUP) $(DESTDIR)$(localstatedir)/run/lock/@PACKAGE@; \
	chown -R $(USER):$(GROUP) $(DESTDIR)$(localstatedir)/run/@PACKAGE@; \
	chown -R $(USER):$(GROUP) $(DESTDIR)$(localstatedir)/$(datarootdir); \
	echo ""; \
	echo "<@PACKAGE@> has ben installed to <$(prefix)> directory!"; \
	echo "";
