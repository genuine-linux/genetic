#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-log) #

# Copyright (c) 2019 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Genetic Logger Library #

# libgenetic-log.la: Log files functions #

### --- Define 'genetic' log files funtions --- ###

# echolog($string): Write <$string> to genetic $LOG file and to stdout #
echolog() {
	# Params: $string #
	string="$1";

	# Always write $string to <stdout> #
	$ECHO "$string";

	# If enabled, write $string to $LOG file #
	if test "$ENABLELOG" == "yes"; then
		# Check if $LOGDIR is installed #
		if test ! -d "$LOGDIR"; then
			$MKDIR -p $LOGDIR &>/dev/null;
		fi;

		# Write entry to $LOG file #
		$ECHO "[$LOGDATE] $string" >> $LOG;
		return $true;
	fi;
}

# echolog_debug($string): Write <$string> to genetic $DEBUGLOG file #
echolog_debug() {
	# Params: $string #
	string="$1";

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Check if $LOGDIR is installed #
		if test ! -d "$LOGDIR"; then
			$MKDIR -p $LOGDIR &>/dev/null;
		fi;

		# Write entry to $DEBUGLOG file #
		$ECHO "[$LOGDATE] $string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echolog_debug_verbose($string): Write <$string> to genetic $DEBUGLOG file and to stdout if ENABLE_VERBOSE=$true #
echolog_debug_verbose() {
	# Params: $string #
	string="$1";

	# Write $string to STDOUT if ENABLE_VERBOSE #
	if test "$ENABLE_VERBOSE" == "yes"; then $ECHO "$string"; fi;

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Check if $LOGDIR is installed #
		if test ! -d "$LOGDIR"; then
			$MKDIR -p $LOGDIR &>/dev/null;
		fi;

		# Write entry to $DEBUGLOG file #
		$ECHO "[$LOGDATE] $string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echologn($string): Write <$string> to genetic $LOG file and to stdout (without return) #
echologn() {
	# Params: $string #
	string="$1";

	# Always write $string to <stdout> #
	$ECHO -n "$string";

	# If enabled, write $string to $LOG file #
	if test "$ENABLELOG" == "yes"; then
		# Write entry to $LOG file #
		$ECHO -n "[$LOGDATE] $string" >> $LOG;
		return $true;
	fi;
}

# echologn_debug($string): Write <$string> to genetic $DEBUGLOG file (without return) #
echologn_debug() {
	# Params: $string #
	string="$1";

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Write entry to $DEBUGLOG file #
		$ECHO -n "[$LOGDATE] $string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echologn_debug_verbose($string): Write <$string> to genetic $DEBUGLOG file (without return) and to stdout if ENABLE_VERBOSE=$true #
echologn_debug_verbose() {
	# Params: $string #
	string="$1";

	# Write $string to STDOUT if ENABLE_VERBOSE=$true #
	if test "$ENABLE_VERBOSE" == "yes"; then $ECHO -n "$string"; fi;

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Write entry to $DEBUGLOG file #
		$ECHO -n "[$LOGDATE] $string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echologn_nodate($string): Write <$string> to genetic $LOG file and to stdout (without logdate, for use with "echologn" function) #
echologn_nodate() {
	# Params: $string #
	string="$1";

	# Always write $string to <stdout> #
	$ECHO -n "$string";

	# If enabled, write $string to $LOG file #
	if test "$ENABLELOG" == "yes"; then
		# Write entry to $LOG file #
		$ECHO -n "$string" >> $LOG;
		return $true;
	fi;
}

# echologn_debug_nodate($string): Write <$string> to genetic $LOG file (without logdate, for use with "echologn" function) #
echologn_debug_nodate() {
	# Params: $string #
	string="$1";

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Write entry to $DEBUGLOG file #
		$ECHO -n "$string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echologn_debug_nodate_verbose($string): Write <$string> to genetic $LOG file and to stdout if ENABLE_VERBOSE=$true (without logdate, for use with "echologn" function) #
echologn_debug_nodate_verbose() {
	# Params: $string #
	string="$1";

	# Write $string to STDOUT if ENABLE_VERBOSE=$true #
	if test "$ENABLE_VERBOSE" == "yes"; then $ECHO -n "$string"; fi;

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Write entry to $DEBUGLOG file #
		$ECHO -n "$string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echolog_nodate($string): Write <$string> to genetic $LOG file and to stdout (without logdate, for use with "echologn" function) #
echolog_nodate() {
	# Params: $string #
	string="$1";

	# Always write $string to <stdout> #
	$ECHO "$string";

	# If enabled, write $string to $LOG file #
	if test "$ENABLELOG" == "yes"; then
		# Write entry to $LOG file #
		$ECHO "$string" >> $LOG;
		return $true;
	fi;
}

# echolog_debug_nodate($string): Write <$string> to genetic $LOG file (without logdate, for use with "echologn" function) #
echolog_debug_nodate() {
	# Params: $string #
	string="$1";

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Write entry to $DEBUGLOG file #
		$ECHO "$string" >> $DEBUGLOG;
		return $true;
	fi;
}

# echolog_debug_nodate_verbose($string): Write <$string> to genetic $LOG file and to stdout if ENABLE_VERBOSE=$true (without logdate, for use with "echologn" function) #
echolog_debug_nodate_verbose() {
	# Params: $string #
	string="$1";

	# Write $string to STDOUT if ENABLE_VERBOSE=$true #
	if test "$ENABLE_VERBOSE" == "yes"; then $ECHO "$string"; fi;

	# If enabled, write $string to $DEBUGLOG file #
	if test "$ENABLEDEBUG" == "yes"; then
		# Write entry to $DEBUGLOG file #
		$ECHO "$string" >> $DEBUGLOG;
		return $true;
	fi;
}


# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
