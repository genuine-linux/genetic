#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-unpack) #

# Copyright (c) 2014-2019 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Genetic Package Unpack Library #

# libgenetic-unpack.la: Package Unpack #

# gen_unpack_package($PACKAGE_PATH): Unpack 'genetic' package  #
gen_unpack_package() {
	# Params: $PACKAGE_PATH #
	PACKAGE_PATH="$1";

	# If --unpackdir=$UNPACK_PATH is not set, then set to $TMPDIR #
	if test -z "$UNPACK_PATH"; then UNPACK_PATH="$TMPDIR"; fi

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_unpack_package($PACKAGE_PATH -> $UNPACK_PATH)'!";
	echolog_debug "$DEBUG =====================================================";

	# Define '$PACKAGE' to be unpacked #
	PACKAGE=$($ECHO $PACKAGE_PATH | $AWK -F"/" '{print $NF}');

	# Check package file '$PACKAGE_PATH' #
	package_check "$PACKAGE_PATH";

	# Check package type #
	get_package_type "$PACKAGE";

	### Extract binary '$PACKAGE' to temporary directory ###
	genextract $EXTRACT $PACKAGE_PATH $UNPACK_PATH/$PACKAGE;
	
	# Check extracted '$PACKAGE' dir #
	check_dir "$UNPACK_PATH/$PACKAGE";

	return $true;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
