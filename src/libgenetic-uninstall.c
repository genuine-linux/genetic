#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-uninstall) #

# Copyright (c) 2014-2019 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Genetic Package Manager Unistaller functions Library #

# libgenetic-uninstall.la: Package Manager Uninstaller functions and definitions #

# gen_uninstall_packages($PACKAGE_NAMES): Uninstall installed package from system. #
gen_uninstall_packages() {
  # Params: $PACKAGE_NAMES #

  UNINSTALL_PACKAGES_PWD=$(pwd)
  for package_name in "$@"; do

    echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
    echolog_debug "$DEBUG Starting 'gen_uninstall_packages($package_name)'!";
    echolog_debug "$DEBUG =====================================================";

    echolog_debug_verbose "$DEBUG Select '$package_name' to be uninstalled!";

    # Defile '$package_name' InfoFile #
    InfoFile="$CACHEDIR/$package_name/Info";

    # Check for package "$InfoFile" file #
    if test -f "$InfoFile"; then
      # $package_name is installed #
      echolog_debug_verbose "$DEBUG Package '$package_name' is installed.";
      echolog_debug_verbose "$DEBUG Loading package '$package_name' installed 'Info' file.";

      # Load '$Package ($Version)' from 'Info' file #
      Package=$($GREP "^name=" $InfoFile | $AWK -F"=" '{print $2}');
      Version=$($GREP "^version=" $InfoFile | $AWK -F"=" '{print $2}');

      echolog "$WARNING Selecting package '${color_wht}$package_name $Version${color_reset}' to be uninstalled!";

      # Warning!!! Never uninstall '$Package-$Version.dirs' directories otherwise required system dirs like '/bin' or '/sbin' will be removed! #

      # Load list of installed package files and links to be removed from system #
      echolog_debug_verbose "$DEBUG Loading package '$package_name $Version' installed files and links."

      start_spinner "Uninstalling previously installed package '${color_wht}$package_name $Version${color_reset}'";

      # PreRemv script for '$Package' #
      echolog_debug "$DEBUG Executing 'PreRemv' script for '$Package  $Version' ($Arch) ...";
      check_file "$CACHEDIR/$package_name/PreRemv" "$CHMOD +x $CACHEDIR/$package_name/PreRemv";
      check_file "$CACHEDIR/$package_name/PreRemv" "$BASH $CACHEDIR/$package_name/PreRemv &>PreRemv.log";

      if test -f "$CACHEDIR/$package_name/$Package.instdir"; then
        INSTDIR=$($CAT $CACHEDIR/$package_name/$Package.instdir);
      fi;

      # If $INSTDIR equals to /, then unset it to prevent extra slashes on paths #
      if test "$INSTDIR" == "/"; then unset INSTDIR; fi;

      ### REMOVE INSTALLED FILES & LINKS ###
      if test -f "$CACHEDIR/$package_name/$Package.files"; then
        echolog_debug "$DEBUG Removing installed package '$package_name $Version' files.";
        for installed_file in $($CAT $CACHEDIR/$package_name/$Package.files); do
          check_file "${INSTDIR}${installed_file}" "$RM -f ${INSTDIR}$installed_file";
        done;
      fi;

      if test -f "$CACHEDIR/$package_name/$Package.links"; then
        echolog_debug "$DEBUG Removing installed package '$package_name $Version' links.";
        for installed_link in $($CAT $CACHEDIR/$package_name/$Package.links); do
          check_link "${INSTDIR}${installed_link}" "$RM -f ${INSTDIR}${installed_link}";
        done;
      fi;
      
      if test -f "$CACHEDIR/$package_name/$Package.lib.files"; then
        echolog_debug "$DEBUG Removing installed package '$package_name $Version' library files.";
        for installed_lib_file in $($CAT $CACHEDIR/$package_name/$Package.lib.files); do
          check_file "${INSTDIR}${installed_lib_file}" "$RM -f ${INSTDIR}$installed_lib_file";
        done
      fi

      if test -f "$CACHEDIR/$package_name/$Package.lib.links"; then
        echolog_debug "$DEBUG Removing installed package '$package_name $Version' library links.";
        for installed_lib_link in $($CAT $CACHEDIR/$package_name/$Package.lib.links); do
          check_link "${INSTDIR}${installed_lib_link}" "$RM -f ${INSTDIR}${installed_lib_link}";
        done
      fi

      if test -f "$CACHEDIR/$package_name/$Package.dbg.files"; then
        echolog_debug "$DEBUG Removing installed package '$package_name $Version' debug files.";
        for installed_dbg_file in $($CAT $CACHEDIR/$package_name/$Package.dbg.files); do
          check_file "${INSTDIR}${installed_dbg_file}" "$RM -f ${INSTDIR}$installed_dbg_file";
        done
      fi

      if test -f "$CACHEDIR/$package_name/$Package.dbg.links"; then
        echolog_debug "$DEBUG Removing installed package '$package_name $Version' debug links.";
        for installed_dbg_link in $($CAT $CACHEDIR/$package_name/$Package.dbg.links); do
          check_link "${INSTDIR}${installed_dbg_link}" "$RM -f ${INSTDIR}${installed_dbg_link}";
        done
      fi

      ### PURGE SYSCONFIG FILES & LINKS ###

      if test -f "$CACHEDIR/$package_name/$Package.etc.files"; then
        echolog_debug "$DEBUG Removing system config installed package '$package_name $Version' files.";
        for installed_config_file in $($CAT $CACHEDIR/$package_name/$Package.etc.files); do
          check_file "${INSTDIR}${installed_config_file}" "$RM -f ${INSTDIR}$installed_config_file";
          check_file "${INSTDIR}${installed_config_file}.new" "$RM -f ${INSTDIR}$installed_config_file.new";
        done;
      else
        echolog_debug "$DEBUG Devel package '$package_name $Version' has not system config files to remove.";
      fi;

      if test -f "$CACHEDIR/$package_name/$Package.etc.links"; then
        echolog_debug "$DEBUG Removing system config installed package '$package_name $Version' links.";
        for installed_config_link in $($CAT $CACHEDIR/$package_name/$Package.etc.links); do
          check_link "${INSTDIR}${installed_config_link}" "$RM -f ${INSTDIR}${installed_config_link}";
        done;
      else
        echolog_debug "$DEBUG Devel package '$package_name $Version' has not system config files to remove.";
      fi;

      # PostRemv script for '$Package' #
      echolog_debug "$DEBUG Executing 'PostRemv' script for '$Package  $Version' ($Arch) debug ...";
      check_file "$CACHEDIR/$package_name/PostRemv" "$CHMOD +x $CACHEDIR/$package_name/PostRemv";
      check_file "$CACHEDIR/$package_name/PostRemv" "$BASH $CACHEDIR/$package_name/PostRemv &>PostRemv.log";

      # Remove installed '$CACHEDIR/$Package' data files #
      echolog_debug "$DEBUG Removing installed '$CACHEDIR/$package_name' data files from system!"
      check_dir "$CACHEDIR/$package_name" "$RM -rf $CACHEDIR/$package_name";

      # Reset $INSTDIR #
      if test -z "$INSTDIR"; then INSTDIR="/"; fi;

      # Stop work_spinner #
      stop_spinner $?;

      ### Starting 'Final Configuration' for installed package ###  
      start_spinner "Cleaning '${color_wht}$package_name${color_reset}' from '${color_wht}genetic${color_reset}' packages database";

      # Reconfiguring 'ld' dinamic linker #
      $LDCONFIG &>/dev/null;

      # Update 'Shared Info Database' #
      update_info_dir;

      # Update genetic 'Installed Packages Database' #
      update_genetic_packages_db;
  
      # Stop spinner #
      stop_spinner $?;

      echolog_debug_verbose "$DEBUG Package '$package_name $Version' has been uninstalled.";

    else
      # $package_name is not installed #
      echolog "$ERROR Selected package '${color_wht}$package_name${color_reset}' is NOT installed!";

      # Close '@PACKAGE' instance #
      exit_instance $false;
    fi;
    $CD $UNINSTALL_PACKAGES_PWD
  done;

  return $true;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet expandtab
# vi: filetype=bash
