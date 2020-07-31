CLEANFILES = genbuild.la genetic.la geninstall.la genuninstall.la libgenetic-bootimg.la libgenetic-bootstrap.la libgenetic-build.la libgenetic.la libgenetic-db.la libgenetic-debug.la libgenetic-env.la libgenetic-gtk.la libgenetic-i18n.la libgenetic-install.la libgenetic-installer.la libgenetic-log.la libgenetic-package.la libgenetic-packager.la libgenetic-repoclient.la libgenetic-repolocal.la libgenetic-repos.la libgenetic-reposerver.la libgenetic-source.la libgenetic-uninstall.la libgenetic-unpack.la

do_subst = sed -e 's,[@]sharedstatedir[@],$(sharedstatedir),g' \
		-e 's,[@]localstatedir[@],$(localstatedir),g' \
		-e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		-e 's,[@]prefix[@],$(prefix),g' \
		-e 's,[@]bindir[@],$(bindir),g' \
		-e 's,[@]sbindir[@],$(sbindir),g' \
		-e 's,[@]libdir[@],$(libdir),g' \
		-e 's,[@]mandir[@],$(mandir),g' \
		-e 's,[@]PERL[@],$(PERL),g' \
		-e 's,[@]ENV[@],$(ENV),g' \
		-e 's,[@]BASH[@],$(BASH),g' \
		-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
		-e 's,[@]VERSION[@],$(VERSION),g' \
		-e 's,[@]DESCRIPTION[@],$(DESCRIPTION),g'

genetic_SOURCES = genbuild.c genetic.c geninstall.c genuninstall.c libgenetic-bootimg.c libgenetic-bootstrap.c libgenetic-build.c libgenetic.c libgenetic-db.c libgenetic-debug.c libgenetic-env.c libgenetic-gtk.c libgenetic-i18n.c libgenetic-install.c libgenetic-installer.c libgenetic-log.c libgenetic-package.c libgenetic-packager.c libgenetic-repoclient.c libgenetic-repolocal.c libgenetic-repos.c libgenetic-reposerver.c libgenetic-source.c libgenetic-uninstall.c libgenetic-unpack.c

sbin_PROGRAMS = genetic$(EXEEXT) genbuild$(EXEEXT) geninstall$(EXEEXT) genuninstall$(EXEEXT)

include_HEADERS = genetic.h

libgeneticdir = $(libdir)/genetic

libgenetic_DATA = libgenetic-bootimg.la libgenetic-bootstrap.la libgenetic-build.la libgenetic.la libgenetic-db.la libgenetic-debug.la libgenetic-env.la libgenetic-gtk.la libgenetic-i18n.la libgenetic-install.la libgenetic-installer.la libgenetic-log.la libgenetic-package.la libgenetic-packager.la libgenetic-repoclient.la libgenetic-repolocal.la libgenetic-repos.la libgenetic-reposerver.la libgenetic-source.la libgenetic-uninstall.la libgenetic-unpack.la

libgenetic.la: libgenetic.c Makefile
	$(do_subst) < $(srcdir)/libgenetic.c > libgenetic.la; \
	chmod +x libgenetic.la

libgenetic-i18n.la: libgenetic-i18n.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-i18n.c > libgenetic-i18n.la; \
	chmod +x libgenetic-i18n.la

libgenetic-install.la: libgenetic-install.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-install.c > libgenetic-install.la; \
	chmod +x libgenetic-install.la

libgenetic-uninstall.la: libgenetic-uninstall.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-uninstall.c > libgenetic-uninstall.la; \
	chmod +x libgenetic-uninstall.la

libgenetic-unpack.la: libgenetic-unpack.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-unpack.c > libgenetic-unpack.la; \
	chmod +x libgenetic-unpack.la

libgenetic-source.la: libgenetic-source.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-source.c > libgenetic-source.la; \
	chmod +x libgenetic-source.la

libgenetic-build.la: libgenetic-build.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-build.c > libgenetic-build.la; \
	chmod +x libgenetic-build.la

libgenetic-db.la: libgenetic-db.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-db.c > libgenetic-db.la; \
	chmod +x libgenetic-db.la

libgenetic-debug.la: libgenetic-debug.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-debug.c > libgenetic-debug.la; \
	chmod +x libgenetic-debug.la

libgenetic-env.la: libgenetic-env.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-env.c > libgenetic-env.la; \
	chmod +x libgenetic-env.la

libgenetic-log.la: libgenetic-log.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-log.c > libgenetic-log.la; \
	chmod +x libgenetic-log.la

libgenetic-package.la: libgenetic-package.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-package.c > libgenetic-package.la; \
	chmod +x libgenetic-package.la

libgenetic-packager.la: libgenetic-packager.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-packager.c > libgenetic-packager.la; \
	chmod +x libgenetic-packager.la

libgenetic-bootimg.la: libgenetic-bootimg.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-bootimg.c > libgenetic-bootimg.la; \
	chmod +x libgenetic-bootimg.la

libgenetic-bootstrap.la: libgenetic-bootstrap.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-bootstrap.c > libgenetic-bootstrap.la; \
	chmod +x libgenetic-bootstrap.la

libgenetic-gtk.la: libgenetic-gtk.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-gtk.c > libgenetic-gtk.la; \
	chmod +x libgenetic-gtk.la

libgenetic-installer.la: libgenetic-installer.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-installer.c > libgenetic-installer.la; \
	chmod +x libgenetic-installer.la

libgenetic-repos.la: libgenetic-repos.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-repos.c > libgenetic-repos.la; \
	chmod +x libgenetic-repos.la

libgenetic-reposerver.la: libgenetic-reposerver.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-reposerver.c > libgenetic-reposerver.la; \
	chmod +x libgenetic-reposerver.la

libgenetic-repolocal.la: libgenetic-repolocal.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-repolocal.c > libgenetic-repolocal.la; \
	chmod +x libgenetic-repolocal.la

libgenetic-repoclient.la: libgenetic-repoclient.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-repoclient.c > libgenetic-repoclient.la; \
	chmod +x libgenetic-repoclient.la

genbuild$(EXEEXT): genbuild.c Makefile
	$(do_subst) < $(srcdir)/genbuild.c > genbuild$(EXEEXT); \
	chmod +x genbuild$(EXEEXT);

geninstall$(EXEEXT): geninstall.c Makefile
	$(do_subst) < $(srcdir)/geninstall.c > geninstall$(EXEEXT); \
	chmod +x geninstall$(EXEEXT);

genuninstall$(EXEEXT): genuninstall.c Makefile
	$(do_subst) < $(srcdir)/genuninstall.c > genuninstall$(EXEEXT); \
	chmod +x genuninstall$(EXEEXT);

genetic$(EXEEXT): genetic.c Makefile
	$(do_subst) < $(srcdir)/genetic.c > genetic$(EXEEXT); \
	chmod +x genetic$(EXEEXT);

all-local: libgenetic-bootimg.la libgenetic-bootstrap.la libgenetic-build.la libgenetic.la libgenetic-db.la libgenetic-debug.la libgenetic-env.la libgenetic-gtk.la libgenetic-i18n.la libgenetic-install.la libgenetic-installer.la libgenetic-log.la libgenetic-package.la libgenetic-packager.la libgenetic-repoclient.la libgenetic-repolocal.la libgenetic-repos.la libgenetic-reposerver.la libgenetic-source.la libgenetic-uninstall.la libgenetic-unpack.la \
	genbuild$(EXEEXT) genetic$(EXEEXT) geninstall$(EXEEXT) genuninstall$(EXEEXT)
