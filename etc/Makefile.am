CLEANFILES = genetic.conf *~

SUBDIRS = conf.d

etcdir = $(sysconfdir)

geneticetcdir = $(etcdir)/@PACKAGE@
geneticetc_DATA = genetic.conf

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

genetic.conf: genetic.conf.in Makefile
	$(do_subst) < $(srcdir)/genetic.conf.in > genetic.conf

repos:
	@mkdir -pv $(DESTDIR)$(sysconfdir)/genetic; \
	cp -av repos $(DESTDIR)$(sysconfdir)/genetic/;

bootstrap:
	@cp -av bootstrap $(DESTDIR)$(sysconfdir)/genetic/;

all-local: genetic.conf repos bootstrap
