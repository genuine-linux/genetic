#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-installer) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# libgenetic-installer: Package Manager Installer functions and definitions #

# gen_installer(): Install Genuine Distribution #
gen_installer() {
	# Parameters: none #

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_installer()'!";
	echolog_debug "$DEBUG =====================================================";

	if gen_check_zenity; then
		echolog_debug "$ERROR $ZENITY was found in your system!";
		echolog "$WARNING Genuine Installer Main Menu";

		# ZENITY WINDOW SIZE
		_ZW=500; # Window width
		_ZH=400; # Window height	
		_ZENITY_WINDOW_SIZE="--width=$_ZW --height=$_ZH";

		gen_installer_options=$($ZENITY --list \
			$_ZENITY_WINDOW_SIZE \
			--title="Genuine GNU/Linux Installer Menu" \
			--column="Genuine Installer Options" \
			--text="Welcome to Genuine GNU/Linux Installer\n\nThis installer will guide you with all the steps of this proccess.\nPlease, BE CAREFUL because you can delete all your hard drives' data!\n" \
			"Install Genuine GNU/Linux" \
			"Administrator Password" \
			"System Users" \
			"Select Language" \
			"Date & Timezone" \
			"Hostname" \
			"Reboot" \
			--ok-label="Exit Installer" \
			--cancel-label="Cancel Installer" \
			--modal --hide-header);
		
		if [[ $? -ne 0 ]]; then
			echolog "$INFO Genuine Installer cancelled by user!";
			exit_instance $false;
		fi;

		if [ "$gen_installer_options" == "Install Genuine GNU/Linux" ]; then
    			gen_zenity_install_system;
		fi
		
		if [ "$gen_installer_options" == "Administrator Password" ]; then
			gen_zenity_root_passwd;
		fi
	
		if [ "$gen_installer_options" == "System Users" ]; then
    			gen_zenity_create_user;
		fi

		if [ "$gen_installer_options" == "Select Language" ]; then
    			gen_zenity_select_locale;
		fi
		
		if [ "$gen_installer_options" == "Date & Timezone" ]; then
    			gen_zenity_set_datetime;
		fi
		
		if [ "$gen_installer_options" == "Hostname" ]; then
    			gen_zenity_set_hostname;
		fi

		if [ "$gen_installer_options" == "Reboot" ]; then
    			gen_zenity_reboot;
		fi
	else
		echolog_debug "$ERROR $ZENITY was NOT found in your system!";
		echolog "$ERROR Genuine Installer cannot be started!";
		exit_instance $false;
	fi
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
