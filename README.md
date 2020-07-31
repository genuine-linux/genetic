Genetic
=======

Genetic is a Software Package Manager developed for Genuine GNU/Linux

Help
----

```
[(·)] Genuine 'GNU/Linux' Genuine Package Manager

[(·)] Usage: genetic --option1 --option2 'args' ... --command|-c 'file'

[(·)] Commands:

	[-h] --help
	[-V] --version
	[-p] --print-architecture

[(·)] Options:

	     --admindir   'directory'
	     --color
	     --verbose
	     --silent
	     --force
	[-R] --rebuildb

[(·)] Repositories:

	[-r] --repo
	     --update

[(·)] Bootstrap Genuine System:

	[-B] --bootstrap

[(·)] Install Genuine System:

	[-I] --installer

[(·)] Source Development Commands:

	     --noarch     '(Create <noarch> package)'
	     --autotools
	     --prefix     'directory'
	     --configure  '--with --enable'
	     --configure-self-dir
	     --packages   'package1 package2'
	[-s] --source     'source.tar.<gz|bz2|xz>'

	     --noarch     '(Build <noarch> package)'
	     --autotools
	     --disable-gen-orig
	     --disable-gen-source
	     --disable-gen-debug
	[-d] --disable-gen-all
	[-b] --build

	[-C] --disable-clean
	[-D] --disable-distclean
	[-c] --clean

	[-P] --packagepool-clean

[(·)] Package Management Commands:

	     --noscripts  (Ignore 'PostInst' & 'PreInst' scripts)
	     --instdir    'directory'
	[-i] --install    'package-version.<arch|dev|doc|dbg|src>.gen'

	     --noscripts  (Ignore 'PostRemv' & 'PreRemv' scripts)
	     --instdir    'directory'
	     --purge      (Delete 'package' </etc> files)
	[-u] --uninstall  'package.<dbg|dev>'

	[-l] --list       'pattern'
	[-L] --listfiles  'pattern'

	     --unpackdir  'directory'
	[-U] --unpack     'package-version.<arch|dev|doc|dbg|src>.gen'

[(·)] Try 'man genetic.(1)' for more information.
```

==============

Genuine GNU/Linux (c) 2009-2016
