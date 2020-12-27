CLEANFILES = \
	arch.conf  build.conf  db.conf  editor.conf  installer.conf  installer-live.conf log.conf  release.conf  repos.conf *~

confddir = $(sysconfdir)/@PACKAGE@/conf.d
confd_DATA = \
	arch.conf  build.conf  db.conf  editor.conf  installer.conf installer-live.conf log.conf  release.conf  repos.conf

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

log.conf: log.conf.in Makefile
	$(do_subst) < $(srcdir)/log.conf.in > log.conf;

editor.conf: editor.conf.in Makefile
	$(do_subst) < $(srcdir)/editor.conf.in > editor.conf;

arch.conf: arch.conf.in Makefile
	$(do_subst) < $(srcdir)/arch.conf.in > arch.conf;

release.conf: release.conf.in Makefile
	$(do_subst) < $(srcdir)/release.conf.in > release.conf;

db.conf: db.conf.in Makefile
	$(do_subst) < $(srcdir)/db.conf.in > db.conf;

build.conf: build.conf.in Makefile
	$(do_subst) < $(srcdir)/build.conf.in > build.conf;

installer.conf: installer.conf.in Makefile
	$(do_subst) < $(srcdir)/installer.conf.in > installer.conf;

installer-live.conf: installer-live.conf.in Makefile
	$(do_subst) < $(srcdir)/installer-live.conf.in > installer-live.conf;

repos.conf: repos.conf.in Makefile
	$(do_subst) < $(srcdir)/repos.conf.in > repos.conf;

all-local: \
	arch.conf  build.conf  db.conf  editor.conf  installer.conf installer-live.conf log.conf  release.conf  repos.conf
