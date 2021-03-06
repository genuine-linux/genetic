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
source $GENETIC_LIB/libgenetic-env.so;
source $GENETIC_LIB/libgenetic-i18n.so;

# Load genetic Logger Functions #
source $GENETIC_LIB/libgenetic-log.so;

# Load genetic Core Functions #
source $GENETIC_LIB/libgenetic.so;

# Load genetic environment debug #
source $GENETIC_LIB/libgenetic-debug.so;

# Load genetic Package Manager Functions #
source $GENETIC_LIB/libgenetic-db.so;
source $GENETIC_LIB/libgenetic-package.so;
source $GENETIC_LIB/libgenetic-source.so;
source $GENETIC_LIB/libgenetic-build.so;
source $GENETIC_LIB/libgenetic-packager.so;
source $GENETIC_LIB/libgenetic-uninstall.so;
source $GENETIC_LIB/libgenetic-install.so;
source $GENETIC_LIB/libgenetic-unpack.so;

# Load genetic Repo Functions #
source $GENETIC_LIB/libgenetic-repos.so;

# Load genetic Bootstrap Genuine system #
source $GENETIC_LIB/libgenetic-bootstrap.so;

# Load genetic GTK GUI (zenity) #
source $GENETIC_LIB/libgenetic-gtk.so;

# Load genetic Installer #
source $GENETIC_LIB/libgenetic-installer.so;

# Start '@PACKAGE@' #

main "$@";

# End '@PACKAGE@' #

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
