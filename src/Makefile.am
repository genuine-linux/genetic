CLEANFILES = genbuild.so genetic.so geninstall.so genuninstall.so libgenetic-bootimg.so libgenetic-bootstrap.so \
						 libgenetic-build.so libgenetic.so libgenetic-db.so libgenetic-debug.so libgenetic-env.so \
						 libgenetic-gtk.so libgenetic-i18n.so libgenetic-install.so libgenetic-installer.so libgenetic-log.so \
						 libgenetic-package.so libgenetic-packager.so libgenetic-repoclient.so libgenetic-repolocal.so \
						 libgenetic-repos.so libgenetic-reposerver.so libgenetic-source.so libgenetic-uninstall.so libgenetic-unpack.so

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

libgeneticdir = $(libdir)

libgenetic_DATA = libgenetic-bootimg.so libgenetic-bootstrap.so libgenetic-build.so libgenetic.so libgenetic-db.so libgenetic-debug.so libgenetic-env.so libgenetic-gtk.so libgenetic-i18n.so libgenetic-install.so libgenetic-installer.so libgenetic-log.so libgenetic-package.so libgenetic-packager.so libgenetic-repoclient.so libgenetic-repolocal.so libgenetic-repos.so libgenetic-reposerver.so libgenetic-source.so libgenetic-uninstall.so libgenetic-unpack.so

libgenetic.so: libgenetic.c Makefile
	$(do_subst) < $(srcdir)/libgenetic.c > libgenetic.so; \
	chmod +x libgenetic.so

libgenetic-i18n.so: libgenetic-i18n.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-i18n.c > libgenetic-i18n.so; \
	chmod +x libgenetic-i18n.so

libgenetic-install.so: libgenetic-install.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-install.c > libgenetic-install.so; \
	chmod +x libgenetic-install.so

libgenetic-uninstall.so: libgenetic-uninstall.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-uninstall.c > libgenetic-uninstall.so; \
	chmod +x libgenetic-uninstall.so

libgenetic-unpack.so: libgenetic-unpack.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-unpack.c > libgenetic-unpack.so; \
	chmod +x libgenetic-unpack.so

libgenetic-source.so: libgenetic-source.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-source.c > libgenetic-source.so; \
	chmod +x libgenetic-source.so

libgenetic-build.so: libgenetic-build.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-build.c > libgenetic-build.so; \
	chmod +x libgenetic-build.so

libgenetic-db.so: libgenetic-db.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-db.c > libgenetic-db.so; \
	chmod +x libgenetic-db.so

libgenetic-debug.so: libgenetic-debug.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-debug.c > libgenetic-debug.so; \
	chmod +x libgenetic-debug.so

libgenetic-env.so: libgenetic-env.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-env.c > libgenetic-env.so; \
	chmod +x libgenetic-env.so

libgenetic-log.so: libgenetic-log.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-log.c > libgenetic-log.so; \
	chmod +x libgenetic-log.so

libgenetic-package.so: libgenetic-package.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-package.c > libgenetic-package.so; \
	chmod +x libgenetic-package.so

libgenetic-packager.so: libgenetic-packager.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-packager.c > libgenetic-packager.so; \
	chmod +x libgenetic-packager.so

libgenetic-bootimg.so: libgenetic-bootimg.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-bootimg.c > libgenetic-bootimg.so; \
	chmod +x libgenetic-bootimg.so

libgenetic-bootstrap.so: libgenetic-bootstrap.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-bootstrap.c > libgenetic-bootstrap.so; \
	chmod +x libgenetic-bootstrap.so

libgenetic-gtk.so: libgenetic-gtk.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-gtk.c > libgenetic-gtk.so; \
	chmod +x libgenetic-gtk.so

libgenetic-installer.so: libgenetic-installer.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-installer.c > libgenetic-installer.so; \
	chmod +x libgenetic-installer.so

libgenetic-repos.so: libgenetic-repos.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-repos.c > libgenetic-repos.so; \
	chmod +x libgenetic-repos.so

libgenetic-reposerver.so: libgenetic-reposerver.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-reposerver.c > libgenetic-reposerver.so; \
	chmod +x libgenetic-reposerver.so

libgenetic-repolocal.so: libgenetic-repolocal.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-repolocal.c > libgenetic-repolocal.so; \
	chmod +x libgenetic-repolocal.so

libgenetic-repoclient.so: libgenetic-repoclient.c Makefile
	$(do_subst) < $(srcdir)/libgenetic-repoclient.c > libgenetic-repoclient.so; \
	chmod +x libgenetic-repoclient.so

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

all-local: libgenetic-bootimg.so libgenetic-bootstrap.so libgenetic-build.so libgenetic.so libgenetic-db.so libgenetic-debug.so libgenetic-env.so libgenetic-gtk.so libgenetic-i18n.so libgenetic-install.so libgenetic-installer.so libgenetic-log.so libgenetic-package.so libgenetic-packager.so libgenetic-repoclient.so libgenetic-repolocal.so libgenetic-repos.so libgenetic-reposerver.so libgenetic-source.so libgenetic-uninstall.so libgenetic-unpack.so \
	genbuild$(EXEEXT) genetic$(EXEEXT) geninstall$(EXEEXT) genuninstall$(EXEEXT)
