#!/bin/bash

# @PACKAGE@-@VERSION@ (Genetic Package Manager) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software; 
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# genetic 19.x.x (Genetic Package Manager) #

# Load genetic Configuration Files and Setups #

GENETIC_CONF="@sysconfdir@/@PACKAGE@";
GENETIC_LIB="@libdir@";

# Import genetic Main Config File #
source $GENETIC_CONF/@PACKAGE@.conf;

# Import genetic Dinamic Config Files (conf.d) #
if test -d "$GENETIC_CONF/conf.d"; then
  CONFD=$(find $GENETIC_CONF/conf.d -iname "*.conf");
  for confd in $CONFD; do
    source $confd;
  done
fi;

# Load genetic Libraries #

# Load genetic Environment Definitions #
source $GENETIC_LIB/libgenetic-env.la;
source $GENETIC_LIB/libgenetic-i18n.la;

# Load genetic Logger Functions #
source $GENETIC_LIB/libgenetic-log.la;

# Load genetic Core Functions #
source $GENETIC_LIB/libgenetic.la;

# Load genetic environment debug #
source $GENETIC_LIB/libgenetic-debug.la;

# Load genetic Package Manager Functions #
source $GENETIC_LIB/libgenetic-db.la;
source $GENETIC_LIB/libgenetic-package.la;
source $GENETIC_LIB/libgenetic-source.la;
source $GENETIC_LIB/libgenetic-build.la;
source $GENETIC_LIB/libgenetic-packager.la;
source $GENETIC_LIB/libgenetic-uninstall.la;
source $GENETIC_LIB/libgenetic-install.la;
source $GENETIC_LIB/libgenetic-unpack.la;

# Load genetic Repo Functions #
source $GENETIC_LIB/libgenetic-repos.la;

# Load genetic Bootstrap Genuine system #
source $GENETIC_LIB/libgenetic-bootstrap.la;

# Load genetic GTK GUI (zenity) #
source $GENETIC_LIB/libgenetic-gtk.la;

# Load genetic Installer #
source $GENETIC_LIB/libgenetic-installer.la;

# Start '@PACKAGE@' #

main "$@";

# End '@PACKAGE@' #

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
