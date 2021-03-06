#!/bin/bash

# @PACKAGE@-@VERSION@ (genbuild) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# genbuild: Genetic Build wrapper #

# Call 'genetic --build' #

genetic --disable-gen-orig --disable-gen-debug $@ --build;

exit;

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
