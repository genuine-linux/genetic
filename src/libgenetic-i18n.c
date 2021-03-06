#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-i18n) #

# Copyright (c) 2014-2019 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# libgenetic-i18n.la: Thin library around basic I18N facilitated function basic text display, file logging, error display, and prompting #

### --- Define 'genetic' Intl functions --- ###

# i18n_error(): Display text to stderr: $1 is assumed to be the Message Catalog key #
i18n_error() {
        $ECHO "$($GETTEXT -s "$1")" >&2;
}

# i18n_display(): Display some text to sdtout: $1 is assumed to be the Message Catalog key. Rest of args are used as misc information #
i18n_display() {
        typeset key="$1";

        $ECHO "$($GETTEXT -s "$key") $@";
}

# i18n_fileout(): Append a log message to a file. Use $1 as target file to append to. Use $2 as catalog key. Rest of args are used as misc information #
i18n_fileout() {
        [[ $# -lt 2 ]] && return 1;

        typeset file="$1";
        typeset key="$2";

        $ECHO "$($GETTEXT -s "$key") $@" >> ${file};
}

# i18n_prompt(): Prompt the user with a message and echo back the response. $1 is assumed to be the Message Catalog key #
i18n_prompt() {
        typeset rv;

        [[ $# -lt 1 ]] && return 1;

        read -p "$($GETTEXT "$1"): " rv;

        $ECHO $rv;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
