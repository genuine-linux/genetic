#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-install) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# libgenetic-install.la: Package Manager Installer functions and definitions #

# gen_install_packages($package_name-version.*.gen): Install 'genetic' '.gen' packages #
gen_install_packages() {
  # Parameters: $INSTALL_PACKAGES #

  INSTALL_PACKAGES_PWD=$(pwd);
  for INSTALL_PACKAGE in "$@"; do

    echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
    echolog_debug "$DEBUG Starting 'gen_install_packages($INSTALL_PACKAGE)'!";
    echolog_debug "$DEBUG =====================================================";

    ### CHECK $REPO_INDEX_CACHE ###
    if ! check_file $INSTALL_PACKAGE; then
      echolog "$INFO Searching for '$INSTALL_PACKAGE' on $REPO_INDEX_CACHE"

      check_file "$REPO_INDEX_CACHE"
      errorcheck $? "Missing $REPO_INDEX_CACHE (try genetic --update)"

      case $INSTALL_PACKAGE in
        *.dev)
          MATCH_LIST=$(<"$REPO_INDEX_CACHE" cut -d' ' -f2,3,8,9 \
            | grep -w devel \
            | grep -E "${INSTALL_PACKAGE}"
          )
        ;;
        *.doc)
          MATCH_LIST=$(<"$REPO_INDEX_CACHE" cut -d' ' -f2,3,8,9 \
            | grep -w documentation \
            | grep -E "$INSTALL_PACKAGE"
          )
        ;;
        *)
          MATCH_LIST=$(<"$REPO_INDEX_CACHE" cut -d' ' -f2,3,8,9 \
            | grep -E -w "$INSTALL_PACKAGE"
          )
        ;;
      esac

      test -n "$MATCH_LIST"
      errorcheck $? "'$INSTALL_PACKAGE' not found"

      echolog "$WARNING concidencias:"
      while read line; do printf "$INFO * %s\\n" "$line"; done <<<"$MATCH_LIST"
      continue_prompt

      while read line
      do
        echolog "$INFO Comprobando '$line'"
        read package_file package_alias package_type server <<<"$line"
        if test -f $PACKAGEPOOL/$package_file; then
          echolog "$INFO '$package_file' presente en $PACKAGEPOOL"
          continue
        fi

        package_url="http://$server/PackagePool/$package_file"
        echolog "$INFO Descargando $package_url"
        wget "$package_url" -O $PACKAGEPOOL/$package_file
        errorcheck $? "fallou a descarga"
      done <<<"$MATCH_LIST"

      $FUNCNAME $(printf "$PACKAGEPOOL/%s\n" $(cut -d' ' -f1 <<<"$MATCH_LIST"))
      errorcheck $? "Instalation failed"
      continue
    fi

    ### Install packages from $PACKAGEPOOL or $SOURCEPOOL ###

    # Define '$PACKAGE' to be installed #
    PACKAGE=$($ECHO $INSTALL_PACKAGE | $AWK -F"/" '{print $NF}');

    # Check package file '$INSTALL_PACKAGE' #
    package_check "$INSTALL_PACKAGE";

    # Check package type #
    get_package_type "$PACKAGE";

    ### Select binary or source package to be installed ###

    # Checking '$PACKAGE' name '.gen' extension prefix 'src', 'dbg', 'dev', 'arch' #
    TYPE_PREFIX=$($ECHO "$PACKAGE" | $AWK  -F"." '{print $(NF-1)}');

    echolog_debug "$DEBUG Package '$PACKAGE' name extension prefix is '$TYPE_PREFIX'.";

    # Check 'genetic' package '$TYPE_PREFIX' #
    case "$TYPE_PREFIX" in
      src)
        # Select source package to install #
        echolog_debug "$DEBUG Select source package '$PACKAGE' to be installed _(gen_install_source_package)_";
        gen_install_source_package "$PACKAGE";
        errorcheck $? "gen_install_package -> gen_install_source_package";
      ;;
      dev)
        # Select devel package to install #
        echolog_debug "$DEBUG Select devel package '$PACKAGE' to be installed _(gen_install_devel_package)_";
        gen_install_devel_package "$PACKAGE";
        errorcheck $? "gen_install_package -> gen_install_devel_package";
      ;;
      doc)
        # Select documentation package to install #
        echolog_debug "$DEBUG Select documentation package '$PACKAGE' to be installed _(gen_install_doc_package)_";
        gen_install_doc_package "$PACKAGE";
        errorcheck $? "gen_install_package -> gen_install_doc_package";
      ;;
      dbg)  # DEBUG REPLACE '$arch.gen' binary package #
        # Select binary package with debug symbols to install #
        echolog_debug "$DEBUG Select debug package '$PACKAGE' to be installed _(gen_install_debug_package)_";
        gen_install_debug_package "$PACKAGE";
        errorcheck $? "gen_install_package -> gen_install_debug_package";
      ;;
      i?86|x86_64) # BINARY REPLACE '$arch.dbg.gen' debug package #
        # Select binary package to install #
        echolog_debug "$DEBUG Select binary package '$PACKAGE' to be installed _(gen_install_binary_package)_";
        gen_install_binary_package "$PACKAGE";
        errorcheck $? "gen_install_package -> gen_install_binary_package";
      ;;
      *) echolog "$ERROR Error! Selected package '${color_wht}$PACKAGE${color_reset}' has an unknown extension prefix '${color_wht}$TYPE_PREFIX${color_reset}'!";
         exit_instance $false;
      ;;
    esac;
    $CD $INSTALL_PACKAGES_PWD;
  done;

  return $true;
}

# gen_install_source_package($PACKAGE): Install package from 'genetic' source. #
gen_install_source_package() {
  # Params: $PACKAGE #
  PACKAGE="$1";

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'gen_install_source_package($PACKAGE)'!";
  echolog_debug "$DEBUG =====================================================";

  ### Select binary package to be installed ###
  echolog "$INFO Selecting source package '${color_wht}$PACKAGE${color_reset}' to be installed.";

  # Install $PACKAGE #
  echolog_debug_verbose "$DEBUG Installing source '$PACKAGE' ...";

  #SOURCE_NAME=$($ECHO "$PACKAGE" | $SED 's/.src.gen//g');
  SOURCE_NAME="${PACKAGE%.src.gen}";

  # Initialize '$GENETIC_TMP/$SOURCE_NAME' data directory #
  check_dir "$GENETIC_TMP/$SOURCE_NAME" "$RM -rf $GENETIC_TMP/$SOURCE_NAME";

  ### Extract source package '$INSTALL_PACKAGE' to '$GENETIC_TMP' directory ###
  genextract $EXTRACT $INSTALL_PACKAGE $GENETIC_TMP;

  ### Starting 'Final Configuration' for source installed package ###  
  start_spinner "Setting up '${color_wht}$SOURCE_NAME${color_reset}'";

  # Check '$GENETIC_TMP/$SOURCE_NAME' #
  check_dir "$GENETIC_TMP/$SOURCE_NAME";

  ### Load source package '$SOURCE_NAME' information ###

  # Define '$SOURCE_NAME' 'SrcInfo' file #
  SrcInfoFile="$GENETIC_TMP/$SOURCE_NAME/SrcInfo";

  # If 'SrcInfo' do not exists exit #
  check_file "$SrcInfoFile" || errorcheck $? "gen_install_source_package";

  # Load source package information from 'SrcInfo' file #
  source_package=$($CAT $SrcInfoFile | $GREP "^source=" | $AWK -F"=" '{print $2}');
  source_version=$($CAT $SrcInfoFile | $GREP "^version=" | $AWK -F"=" '{print $2}');

  # Check dependencies from 'SrcInfo' file.
  gen_check_package_depends "$SrcInfoFile";

  stop_spinner $?;

  ### Source package '$SOURCE_NAME' installed #
  echolog "$INFO Source package '${color_wht}$source_package $source_version${color_reset}' installed in '${color_wht}$GENETIC_TMP/$SOURCE_NAME${color_reset}'.";

  return $true;
}

# gen_install_devel_package($PACKAGE): Install package from 'genetic' devel. #
gen_install_devel_package() {
  # Params: $PACKAGE #
  PACKAGE="$1";

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'gen_install_devel_package($PACKAGE)'!";
  echolog_debug "$DEBUG =====================================================";

  # Select binary package to be installed #
  echolog "$INFO Selecting devel package '${color_wht}$PACKAGE${color_reset}' to be installed.";

  # Initialize '$CACHEDIR/$PACKAGE' data directory #
  echolog_debug_verbose "$DEBUG Create '$CACHEDIR/$PACKAGE' directory to store '$PACKAGE' data files."
  check_dir "$CACHEDIR/$PACKAGE" "$RM -rf $CACHEDIR/$PACKAGE";
  $MKDIR -p $CACHEDIR/$PACKAGE;

  ### Extract binary '$PACKAGE' to temporary directory ###
  genextract $EXTRACT $INSTALL_PACKAGE $CACHEDIR/$PACKAGE;
  
  # Change to extracted '$PACKAGE' data #
  check_dir "$CACHEDIR/$PACKAGE" "$CD $CACHEDIR/$PACKAGE";

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  ### Finished loading '$Package' info ###

  # Check if '$Package' data directory "$CACHEDIR/$Package" exists #
  if test -d "$CACHEDIR/$Package"; then
    # If true package is installed #
    echolog "$WARNING Warning! Package '${color_wht}$Package${color_reset}' devel is already installed!";
    echolog "$WARNING Warning! Reinstall '${color_wht}$Package${color_reset}' devel using replacement '${color_wht}$Package $Version${color_reset}'!";

    # Delete old stored package data #
    gen_uninstall_packages "$Package";
  fi;

  # Rename data directory to '$CACHEDIR/$Package' #
  $MV $CACHEDIR/$PACKAGE $CACHEDIR/$Package;
  $CD $CACHEDIR/$Package;

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  # Get '$Package' Target Arquitecture from 'PkgArch' file #
  Arch=$($CAT PkgArch);

  # Checking binary '$Package' arquitecture '$Arch' #
  check_package_arch "$Package" "$Version" "$Arch";

  # Start work_spinner #
  start_spinner "Installing '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset}) devel";

  # Check dependencies from 'Info' file.
  gen_check_package_depends "$CACHEDIR/$Package/Info";

  ### Install root.tmp files (package content) ###
  $ECHO "$INSTDIR" &>$CACHEDIR/$Package/$Package.instdir 

  # Extract $ROOTFS files to $INSTDIR #
  echolog_debug "$DEBUG Extract '$Package $Version' ($Arch) '$ROOTFS' file '$ROOTFS.tmp' into '$INSTDIR'.";
  $TAR xf $ROOTFS.tmp -C $INSTDIR &>/dev/null;

  # If $INSTDIR equals to /, then unset it to prevent extra slashes on paths #
  if test "$INSTDIR" == "/"; then unset INSTDIR; fi;

  ### Check installed files, dirs, links ###
  echolog_debug "$DEBUG Verifying that all '$Package $Version' ($Arch) devel files are installed";

  # Check '$Package' index file list #
  if test -f "$Package.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) devel file index '$Package.files'.";

    # Check files in '$Package.files' index #
    #for devfile in $($CAT $Package.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.files | sed 's/[ \(\)]/\\&/g' | while read devfile; do
      check_file "${INSTDIR}$devfile" || errorcheck $? "gen_install_devel_package [${INSTDIR}$devfile] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) devel file index '$Package.files'.";
  fi;

  # Check '$Package' devel index directory list #
  if test -f "$Package.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) devel directory index '$Package.dirs'.";

    # Check directories in '$Package.dirs' index #
    #for devdir in $($CAT $Package.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dirs | sed 's/[ \(\)]/\\&/g' | while read devdir; do
      check_dir "${INSTDIR}$devdir" || errorcheck $? "gen_install_devel_package [${INSTDIR}$devdir] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) devel directory index '$Package.dirs'.";
  fi;

  # Check '$Package' devel index link list #
  if test -f "$Package.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) devel link index '$Package.links'.";

    # Check links in '$Package.links' index #
    #for devlink in $($CAT $Package.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.links | sed 's/[ \(\)]/\\&/g' | while read devlink; do
      check_link "${INSTDIR}$devlink" || errorcheck $? "gen_install_binary_package [${INSTDIR}$devlink] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) devel link index '$Package.links'.";
  fi;

  echolog_debug "$DEBUG OK! '$Package $Version' ($Arch) devel indexed files are installed to system.";

  # Reset $INSTDIR #
  if test -z "$INSTDIR"; then INSTDIR="/"; fi;

  # End work_spinner #
  stop_spinner $?;

  ### Starting 'Final Configuration' for installed package ###  
  start_spinner "Setting up '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset}) devel";

  echolog_debug "$DEBUG Starting '$Package $Version' ($Arch) devel 'Final Configuration'.";    

  # Delete $ROOTFS.tmp
  echolog_debug "$DEBUG Deleting 'root.tmp' file.";
  check_file "$ROOTFS.tmp" "$RM $ROOTFS.tmp";

  # Update genetic 'Installed Packages Database' #
  update_genetic_packages_db;

  echolog_debug "$DEBUG Finished '$Package $Version' ($Arch) devel 'Final Configuration'.";

  ### End of 'Final Configuration' for installed package ###      

  echolog_debug "$DEBUG Package '$Package $Version' ($Arch) devel has been installed.";

  # End work_spinner #
  stop_spinner $?;

  return $true;
}

# gen_install_doc_package($PACKAGE): Install package from 'genetic' doc. #
gen_install_doc_package() {
  # Params: $PACKAGE #
  PACKAGE="$1";

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'gen_install_doc_package($PACKAGE)'!";
  echolog_debug "$DEBUG =====================================================";

  # Select doc package to be installed #
  echolog "$INFO Selecting documentation package '${color_wht}$PACKAGE${color_reset}' to be installed.";

  # Initialize '$CACHEDIR/$PACKAGE' data directory #
  echolog_debug_verbose "$DEBUG Create '$CACHEDIR/$PACKAGE' directory to store '$PACKAGE' data files."
  check_dir "$CACHEDIR/$PACKAGE" "$RM -rf $CACHEDIR/$PACKAGE";
  $MKDIR -p $CACHEDIR/$PACKAGE;

  ### Extract binary '$PACKAGE' to temporary directory ###
  genextract $EXTRACT $INSTALL_PACKAGE $CACHEDIR/$PACKAGE;
  
  # Change to extracted '$PACKAGE' data #
  check_dir "$CACHEDIR/$PACKAGE" "$CD $CACHEDIR/$PACKAGE";

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  ### Finished loading '$Package' info ###

  # Check if '$Package' data directory "$CACHEDIR/$Package" exists #
  if test -d "$CACHEDIR/$Package"; then
    # If true package is installed #
    echolog "$WARNING Warning! Package '${color_wht}$Package${color_reset}' documentation is already installed!";
    echolog "$WARNING Warning! Reinstall '${color_wht}$Package${color_reset}' documentation using replacement '${color_wht}$Package $Version${color_reset}'!";

    # Delete old stored package data #
    gen_uninstall_packages "$Package";
  fi;

  # Rename data directory to '$CACHEDIR/$Package' #
  $MV $CACHEDIR/$PACKAGE $CACHEDIR/$Package;
  $CD $CACHEDIR/$Package;

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  # Get '$Package' Target Arquitecture from 'PkgArch' file #
  Arch=$($CAT PkgArch);

  # Checking binary '$Package' arquitecture '$Arch' #
  check_package_arch "$Package" "$Version" "$Arch";

  # Start work_spinner #
  start_spinner "Installing '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset}) documentation";

  # Check dependencies from 'Info' file.
  gen_check_package_depends "$CACHEDIR/$Package/Info";

  ### Install root.tmp files (package content) ###
  $ECHO "$INSTDIR" &>$CACHEDIR/$Package/$Package.instdir 

  # Extract $ROOTFS files to $INSTDIR #
  echolog_debug "$DEBUG Extract '$Package $Version' ($Arch) '$ROOTFS' file '$ROOTFS.tmp' into '$INSTDIR'.";
  $TAR xf $ROOTFS.tmp -C $INSTDIR &>/dev/null;

  # If $INSTDIR equals to /, then unset it to prevent extra slashes on paths #
  if test "$INSTDIR" == "/"; then unset INSTDIR; fi;

  ### Check installed files, dirs, links ###
  echolog_debug "$DEBUG Verifying that all '$Package $Version' ($Arch) documentation files are installed";

  # Check '$Package' index file list #
  if test -f "$Package.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) documentation file index '$Package.files'.";

    # Check files in '$Package.files' index #
    #for docfile in $($CAT $Package.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.files | sed 's/[ \(\)]/\\&/g' | while read docfile; do
      check_file "${INSTDIR}$docfile" || errorcheck $? "gen_install_doc_package [${INSTDIR}$docfile] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) documentation file index '$Package.files'.";
  fi;

  # Check '$Package' documentation index directory list #
  if test -f "$Package.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) documentation directory index '$Package.dirs'.";

    # Check directories in '$Package.dirs' index #
    #for docdir in $($CAT $Package.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dirs | sed 's/[ \(\)]/\\&/g' | while read docdir; do
      check_dir "${INSTDIR}$docdir" || errorcheck $? "gen_install_doc_package [${INSTDIR}$docdir] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) documentation directory index '$Package.dirs'.";
  fi;

  # Check '$Package' documentation index link list #
  if test -f "$Package.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) documentation link index '$Package.links'.";

    # Check links in '$Package.links' documentation index #
    #for doclink in $($CAT $Package.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.links | sed 's/[ \(\)]/\\&/g' | while read doclink; do
      check_link "${INSTDIR}$doclink" || errorcheck $? "gen_install_doc_package [${INSTDIR}$doclink] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) documentation link index '$Package.links'.";
  fi;

  echolog_debug "$DEBUG OK! '$Package $Version' ($Arch) documentation indexed files are installed to system.";

  # Reset $INSTDIR #
  if test -z "$INSTDIR"; then INSTDIR="/"; fi;

  # End work_spinner #
  stop_spinner $?;

  ### Starting 'Final Configuration' for installed package ###  
  start_spinner "Setting up '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset}) documentation";

  echolog_debug "$DEBUG Starting '$Package $Version' ($Arch) documentation 'Final Configuration'.";    

  # Delete $ROOTFS.tmp
  echolog_debug "$DEBUG Deleting 'root.tmp' file.";
  check_file "$ROOTFS.tmp" "$RM $ROOTFS.tmp";

  # Update genetic 'Installed Packages Database' #
  update_genetic_packages_db;

  echolog_debug "$DEBUG Finished '$Package $Version' ($Arch) documentation 'Final Configuration'.";

  ### End of 'Final Configuration' for installed package ###      

  echolog_debug "$DEBUG Package '$Package $Version' ($Arch) documentation has been installed.";

  # End work_spinner #
  stop_spinner $?;

  return $true;
}

# gen_install_debug_package($PACKAGE): Install debug package from 'genetic' binary. #
gen_install_debug_package() {
  # Params: $PACKAGE #
  PACKAGE="$1";

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'gen_install_debug_package($PACKAGE)'!";
  echolog_debug "$DEBUG =====================================================";

  # Select binary package to be installed #
  echolog "$INFO Selecting binary debug package '${color_wht}$PACKAGE${color_reset}' to be installed.";

  # Initialize '$CACHEDIR/$PACKAGE' data directory #
  echolog_debug_verbose "$DEBUG Create '$CACHEDIR/$PACKAGE' directory to store '$PACKAGE' data files."
  check_dir "$CACHEDIR/$PACKAGE" "$RM -rf $CACHEDIR/$PACKAGE";
  $MKDIR -p $CACHEDIR/$PACKAGE;

  ### Extract binary '$PACKAGE' to temporary directory ###
  genextract $EXTRACT $INSTALL_PACKAGE $CACHEDIR/$PACKAGE;
  
  # Change to extracted '$PACKAGE' data #
  check_dir "$CACHEDIR/$PACKAGE" "$CD $CACHEDIR/$PACKAGE";

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  ### Finished loading '$Package' info ###

  # Check if '$Package' data directory "$CACHEDIR/$Package" or "$CACHEDIR/$Package" exists #
  if test -d "$CACHEDIR/$Package" || test -d "$CACHEDIR/$Package"; then
    # If true package is installed #
    echolog "$WARNING Warning! Package '${color_wht}$Package${color_reset}' is already installed!";
    echolog "$WARNING Warning! Reinstall '${color_wht}$Package${color_reset}' using replacement '${color_wht}$Package $Version${color_reset}'!";

    # Delete old stored package data #
    gen_uninstall_packages "$Package";
  fi;

  # Rename data directory to '$CACHEDIR/$Package' #
  $MV $CACHEDIR/$PACKAGE $CACHEDIR/$Package;
  $CD $CACHEDIR/$Package;

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  # Get '$Package' Target Arquitecture from 'PkgArch' file #
  Arch=$($CAT PkgArch);

  # Checking binary '$Package' arquitecture '$Arch' #
  check_package_arch "$Package" "$Version" "$Arch";

  # Start work_spinner #
  start_spinner "Installing '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset}) debug";

  # Check dependencies from 'Info' file.
  gen_check_package_depends "$CACHEDIR/$Package/Info";

  # Preset '$Package' before install #
  echolog_debug "$DEBUG Executing 'PreInst' script for '$Package $Version' ($Arch) ...";
  $CHMOD +x PreInst; $BASH PreInst &>PreInst.log;

  ### Install root.tmp files (package content) ###
  $ECHO "$INSTDIR" &>$CACHEDIR/$Package/$Package.instdir 

  # Extract $ROOTFS files to $INSTDIR #
  echolog_debug "$DEBUG Extract '$Package $Version' ($Arch) '$ROOTFS' file '$ROOTFS.tmp' into '$INSTDIR'.";
  $TAR xf $ROOTFS.tmp -C $INSTDIR &>/dev/null;

  # If $INSTDIR equals to /, then unset it to prevent extra slashes on paths #
  if test "$INSTDIR" == "/"; then unset INSTDIR; fi;

  ### Install sysconfig files ###
  
  # Check '$Package' sysconfig index directory list #
  if test -f "$Package.etc.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) sysconfig directory index '$Package.etc.dirs'.";

    # Check directories in '$Package.etc.dirs' index #
    #for dir in $($CAT $Package.etc.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dirs | sed 's/[ \(\)]/\\&/g' | while read dir; do
      if test ! -d "${INSTDIR}$dir"; then
        echolog_debug "$DEBUG Creating '$Package $Version' ($Arch) sysconfig directory '${INSTDIR}$dir'.";
        $MKDIR -pv ${INSTDIR}$dir &>/dev/null;
      else
        echolog_debug "$DEBUG '$Package $Version' ($Arch) sysconfig directory '${INSTDIR}$dir' exists.";
      fi;
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) sysconfig directory index '$Package.etc.dirs'.";
  fi;

  # Check '$Package' sysconfig index file list #
  if test -f "$Package.etc.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) sysconfig file index '$Package.etc.files'.";

    # Check files in '$Package.etc.files' index #
    #for file in $($CAT $Package.etc.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.files | sed 's/[ \(\)]/\\&/g' | while read file; do
      if test ! -f "${INSTDIR}$file"; then
        echolog_debug "$DEBUG Creating '$Package $Version' ($Arch) sysconfig file '${INSTDIR}$file'.";
        $CP -a .$file ${INSTDIR}$file &>/dev/null;
      else
        echolog_debug "$DEBUG '$Package $Version' ($Arch) sysconfig file '${INSTDIR}$file' exists, creating new version as '${INSTDIR}$file.new'.";
        $CP -a .$file ${INSTDIR}$file.new &>/dev/null;
      fi;
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) sysconfig file index '$Package.etc.files'.";
  fi;

  # Check '$Package' sysconfig index link list #
  if test -f "$Package.etc.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) sysconfig link index '$Package.etc.links'.";

    # Check links in '$Package.etc.links' index #
    #for link in $($CAT $Package.etc.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.links | sed 's/[ \(\)]/\\&/g' | while read link; do
      if test ! -L "${INSTDIR}$link"; then
        echolog_debug "$DEBUG Creating '$Package $Version' ($Arch) sysconfig link '${INSTDIR}$link'.";
        $MV .$link ${INSTDIR}$link &>/dev/null;
      else
        echolog_debug "$DEBUG '$Package $Version' ($Arch) sysconfig link '${INSTDIR}$link' exists.";
      fi;
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) sysconfig link index '$Package.etc.links'.";
  fi;

  ### Check installed files, dirs, links ###
  echolog_debug "$DEBUG Verifying that all '$Package $Version' ($Arch) debug files are installed";

  # Check '$Package' index file list #
  if test -f "$Package.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) debug file index '$Package.files'.";

    # Check files in '$Package.files' index #
    #for file in $($CAT $Package.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.files | sed 's/[ \(\)]/\\&/g' | while read file; do
      check_file "${INSTDIR}$file" || errorcheck $? "gen_install_debug_package [${INSTDIR}$file] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) debug file index '$Package.files'.";
  fi;

  # Check '$Package' index directory list #
  if test -f "$Package.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) debug directory index '$Package.dirs'.";

    # Check directories in '$Package.dirs' index #
    #for dir in $($CAT $Package.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dirs | sed 's/[ \(\)]/\\&/g' | while read dir; do
      check_dir "${INSTDIR}$dir" || errorcheck $? "gen_install_debug_package [${INSTDIR}$dir] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) debug directory index '$Package.dirs'.";
  fi;

  # Check '$Package' index link list #
  if test -f "$Package.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) debug link index '$Package.links'.";

    # Check links in '$Package.links' index #
    #for link in $($CAT $Package.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.links | sed 's/[ \(\)]/\\&/g' | while read link; do
      check_link "${INSTDIR}$link" || errorcheck $? "gen_install_debug_package [${INSTDIR}$link] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) debug link index '$Package.links'.";
  fi;

  echolog_debug "$DEBUG OK! '$Package $Version' ($Arch) debug indexed files are installed to system.";

  # Reset $INSTDIR #
  if test -z "$INSTDIR"; then INSTDIR="/"; fi;

  # End work_spinner #
  stop_spinner $?;

  ### Starting 'Final Configuration' for installed package ###  
  start_spinner "Setting up '${color_wht}$Package  $Version${color_reset}' (${color_wht}$Arch${color_reset}) debug";

  echolog_debug "$DEBUG Starting '$Package $Version' ($Arch) debug 'Final Configuration'.";    

  # Post install setup for '$Package' #
  echolog_debug "$DEBUG Executing 'PostInst' script for '$Package  $Version' ($Arch) debug ...";
  $CHMOD +x PostInst; $BASH PostInst &>PostInst.log;

  # Delete $ROOTFS.tmp
  echolog_debug "$DEBUG Deleting 'root.tmp' file.";
  check_file "$ROOTFS.tmp" "$RM $ROOTFS.tmp";

  # Delete sysconfig files
  echolog_debug "$DEBUG Deleting 'etc' sysconfig directory.";
  check_dir "etc" "$RM -rf etc";

  # Reconfiguring 'ld' dinamic linker #
  $LDCONFIG &>/dev/null;

  # Libtool finish libraries #
  package_libtool_finish;

  # Update 'Shared Info Database' #
  update_info_dir;

  # Update 'perllocal.pod' #
  update_perllocal_pod;

  # Update genetic 'Installed Packages Database' #
  update_genetic_packages_db;

  echolog_debug "$DEBUG Finished '$Package $Version' ($Arch) debug 'Final Configuration'.";

  ### End of 'Final Configuration' for installed package ###      

  echolog_debug "$DEBUG Package '$Package $Version' ($Arch) debug has been installed.";

  # End work_spinner #
  stop_spinner $?;

  return $true;
}

# gen_install_binary_package($PACKAGE): Install package from 'genetic' binary. #
gen_install_binary_package() {
  # Params: $PACKAGE #
  PACKAGE="$1";

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'gen_install_binary_package($PACKAGE)'!";
  echolog_debug "$DEBUG =====================================================";

  # Select binary package to be installed #
  echolog "$INFO Selecting binary package '${color_wht}$PACKAGE${color_reset}' to be installed.";

  export CACHEDIR_ORIG="$CACHEDIR";
  if test "$INSTDIR" != "/"; then # If --instdir is used, change CACHEDIR value
    echolog_debug "$WARNING Warning! Changing genetic CACHEDIR to $INSTDIR$CACHEDIR!";
    echolog "$WARNING Warning! Changing genetic CACHEDIR to $INSTDIR$CACHEDIR!";
    export CACHEDIR="$INSTDIR$CACHEDIR";
  fi;

  # Initialize '$CACHEDIR/$PACKAGE' data directory #
  echolog_debug_verbose "$DEBUG Create '$CACHEDIR/$PACKAGE' directory to store '$PACKAGE' data files."
  check_dir "$CACHEDIR/$PACKAGE" "$RM -rf $CACHEDIR/$PACKAGE";
  $MKDIR -p $CACHEDIR/$PACKAGE;

  ### Extract binary '$PACKAGE' to temporary directory ###
  genextract $EXTRACT $INSTALL_PACKAGE $CACHEDIR/$PACKAGE;
  
  # Change to extracted '$PACKAGE' data #
  check_dir "$CACHEDIR/$PACKAGE" "$CD $CACHEDIR/$PACKAGE";

  ### Load new '$Package' data files ###

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  ### Finished loading '$Package' info ###

  ### UNINSTALL previous versions ###
  # Check if '$Package' data directory "$CACHEDIR/$Package" or "$CACHEDIR/$Package.dbg" exists #
  if test "$INSTDIR" == "/"; then # If --instdir is used, do not uninstall packages from database
    if test -d "$CACHEDIR/$Package" || test -d "$CACHEDIR/$Package.dbg"; then
      # If true package is installed #
      echolog "$WARNING Warning! Package '${color_wht}$Package${color_reset}' is already installed!";
      echolog "$WARNING Warning! Reinstall '${color_wht}$Package${color_reset}' using replacement '${color_wht}$Package $Version${color_reset}'!";

                   # If updated package is linux then keep old files #
                    case $Package in
                         linux)  # CRITICAL KERNEL PACKAGE #
                                echolog "$WARNING Warning! Package $Package kernel package files ${color_bwht}SHOULD NOT${color_reset} be overwritten!";
                                echolog "$ERROR Error! ${color_ured}!!!Package $Package kernel package must manually uninstalled using: genetic -u linux!!!${color_reset}";
                                # Close genetic instance #
                                exit_instance $true;
                        ;;
                        glibc|libc|ncurses|gcc|bash) # CRITICAL BASE PACKAGES #
                                echolog "$WARNING Warning! Package $Package files will be directly overwritten but not uninstalled!";
                                echolog "$WARNING Warning! ${color_ured}!!!You must reboot your system after installing $Package!!!${color_reset}";
                        ;;
                        libgenetic|genetic|zlib|libz|file|diffutils|findutils|util-linux|coreutils|attr|acl|procps-ng|readline|gmp|mpfr|mpc|bzip2|flex|libtool|gettext|autogen|automakesed|gawk|psmisc) # CRITICAL BASE PACKAGES #
                                echolog "$WARNING Warning! Package $Package files will be directly overwritten but not uninstalled!";
                        ;;
                        *) # NOT CRITICAL PACKAGES #
                                # Delete old stored package data #
                                gen_uninstall_packages "$Package";
        unset _TARGET;
        INSTDIR="/";
                        ;;
                  esac;
    fi;
  else
    echolog_debug "$WARNING Warning! Option [--instdir] was selected! Package $Package will not be removed!";
    echolog "$WARNING Warning! Option [--instdir] was selected! Package $Package will not be removed!";
  fi;

  # Rename data directory to '$CACHEDIR/$Package' #
  if [ -d "$CACHEDIR/$Package" ]; then
    if [ -d "$CACHEDIR/$Package.old" ]; then
      $RM -rf $CACHEDIR/$Package.old
    fi;
                echolog "$WARNING Warning! Package $Package genetic cache files are being directly overwritten but not uninstalled!";
    $MV $CACHEDIR/$Package $CACHEDIR/$Package.old
  fi
  $MV $CACHEDIR/$PACKAGE $CACHEDIR/$Package;
  $CD $CACHEDIR/$Package;

  # Load '$Package ($Version)' from 'Info' file #
  Package=$($GREP "^name=" Info | $AWK -F"=" '{print $2}');
  Version=$($GREP "^version=" Info | $AWK -F"=" '{print $2}');

  # Get '$Package' Target Arquitecture from 'PkgArch' file #
  Arch=$($CAT PkgArch);

  # Checking binary '$Package' arquitecture '$Arch' #
  check_package_arch "$Package" "$Version" "$Arch";

  # Start work_spinner #
  if test "$INSTDIR" == "/"; then
    start_spinner "Installing '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset})";
  else
    start_spinner "Installing '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset}) to: $INSTDIR";
  fi

  # Check dependencies from 'Info' file.
  gen_check_package_depends "$CACHEDIR/$Package/Info";

  # Preset '$Package' before install #
  if [ "$ENABLE_NOSCRIPTS" != "yes" ]; then
    echolog_debug "$DEBUG Executing 'PreInst' script for '$Package $Version' ($Arch) ...";
    $CHMOD +x PreInst; $BASH PreInst &>PreInst.log;
  fi;

  ### Install root.tmp files (package content) ###
  $ECHO "$INSTDIR" &>$CACHEDIR/$Package/$Package.instdir 

  # Extract $ROOTFS files to $INSTDIR #
  echolog_debug "$DEBUG Extract '$Package $Version' ($Arch) '$ROOTFS' file '$ROOTFS.tmp' into '$INSTDIR'.";
  $TAR xf $ROOTFS.tmp -C $INSTDIR &>/dev/null;

  # If $INSTDIR equals to /, then unset it to prevent extra slashes on paths #
  if test "$INSTDIR" == "/"; then unset INSTDIR; fi;

  ### Install sysconfig files ###
  
  # Check '$Package' sysconfig index directory list #
  if test -f "$Package.etc.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) sysconfig directory index '$Package.etc.dirs'.";

    # Check directories in '$Package.etc.dirs' index #
    #for dir in $($CAT $Package.etc.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.etc.dirs | sed 's/[ \(\)]/\\&/g' | while read dir; do
      if test ! -d "${INSTDIR}$dir"; then
        echolog_debug "$DEBUG Creating '$Package $Version' ($Arch) sysconfig directory '${INSTDIR}$dir'.";
        $MKDIR -pv ${INSTDIR}$dir &>/dev/null;
      else
        echolog_debug "$DEBUG '$Package $Version' ($Arch) sysconfig directory '${INSTDIR}$dir' exists.";
      fi;
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) sysconfig directory index '$Package.etc.dirs'.";
  fi;

  # Check '$Package' sysconfig index file list #
  if test -f "$Package.etc.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) sysconfig file index '$Package.etc.files'.";

    # Check files in '$Package.etc.files' index #
    #for file in $($CAT $Package.etc.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.etc.files | $SED 's/[ \(\)]/\\&/g' | $SED 's/ /\\ /g' | while read file; do
      if test ! -f "${INSTDIR}$file"; then
        echolog_debug "$DEBUG Creating '$Package $Version' ($Arch) sysconfig file '${INSTDIR}$file'.";
        $CP -a .$file ${INSTDIR}$file &>/dev/null;
      else
        echolog_debug "$DEBUG '$Package $Version' ($Arch) sysconfig file '${INSTDIR}$file' exists, creating new version as '${INSTDIR}$file.new'.";
        $CP -a .$file ${INSTDIR}$file.new &>/dev/null;
      fi;
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) sysconfig file index '$Package.etc.files'.";
  fi;

  # Check '$Package' sysconfig index link list #
  if test -f "$Package.etc.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) sysconfig link index '$Package.etc.links'.";

    # Check links in '$Package.etc.links' index #
    #for link in $($CAT $Package.etc.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.etc.links | sed 's/[ \(\)]/\\&/g' | while read link; do
      if test ! -L "${INSTDIR}$link"; then
        echolog_debug "$DEBUG Creating '$Package $Version' ($Arch) sysconfig link '${INSTDIR}$link'.";
        $MV .$link ${INSTDIR}$link &>/dev/null;
      else
        echolog_debug "$DEBUG '$Package $Version' ($Arch) sysconfig link '${INSTDIR}$link' exists.";
      fi;
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) sysconfig link index '$Package.etc.links'.";
  fi;

  ### Check installed files, dirs, links ###
  echolog_debug "$DEBUG Verifying that all '$Package $Version' ($Arch) files are installed";

# Check '$Package' index debug file list #
  if test -f "$Package.dbg.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) debug file index '$Package.dbg.files'.";

    # Check files in '$Package.dbg.files' index #
    #for file in $($CAT $Package.dbg.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dbg.files | sed 's/[ \(\)]/\\&/g' | while read file; do
      check_file "${INSTDIR}${file}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${file}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) debug file index '$Package.dbg.files'.";
  fi;

  # Check '$Package' index debug directory list #
  if test -f "$Package.dbg.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) debug directory index '$Package.dbg.dirs'.";

    # Check directories in '$Package.dbg.dirs' index #
    #for dir in $($CAT $Package.dbg.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dbg.dirs | sed 's/[ \(\)]/\\&/g' | while read dir; do
      check_dir "${INSTDIR}${dir}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${dir}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) debug directory index '$Package.dbg.dirs'.";
  fi;

  # Check '$Package' index debug link list #
  if test -f "$Package.dbg.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) debug link index '$Package.dbg.links'.";

    # Check links in '$Package.dbg.links' index #
    #for link in $($CAT $Package.dbg.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dbg.links | sed 's/[ \(\)]/\\&/g' | while read link; do
      check_link "${INSTDIR}${link}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${link}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) debug link index '$Package.dbg.links'.";
  fi;

  # Check '$Package' index library file list #
  if test -f "$Package.lib.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) library file index '$Package.lib.files'.";

    # Check files in '$Package.lib.files' index #
    #for file in $($CAT $Package.lib.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.lib.files | sed 's/[ \(\)]/\\&/g' | while read file; do
      check_file "${INSTDIR}${file}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${file}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) library file index '$Package.lib.files'.";
  fi;

  # Check '$Package' index library directory list #
  if test -f "$Package.lib.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) library directory index '$Package.lib.dirs'.";

    # Check directories in '$Package.lib.dirs' index #
    #for dir in $($CAT $Package.lib.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.lib.dirs | sed 's/[ \(\)]/\\&/g' | while read dir; do
      check_dir "${INSTDIR}${dir}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${dir}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) library directory index '$Package.lib.dirs'.";
  fi;

  # Check '$Package' index library link list #
  if test -f "$Package.lib.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) library link index '$Package.lib.links'.";

    # Check links in '$Package.lib.links' index #
    #for link in $($CAT $Package.lib.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.lib.links | sed 's/[ \(\)]/\\&/g' | while read link; do
      check_link "${INSTDIR}${link}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${link}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) library link index '$Package.lib.links'.";
  fi;

  # Check '$Package' index file list #
  if test -f "$Package.files"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) file index '$Package.files'.";

    # Check files in '$Package.files' index #
    #for file in $($CAT $Package.files | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.files | sed 's/[ \(\)]/\\&/g' | while read file; do
      check_file "${INSTDIR}${file}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${file}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) file index '$Package.files'.";
  fi;

  # Check '$Package' index directory list #
  if test -f "$Package.dirs"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) directory index '$Package.dirs'.";

    # Check directories in '$Package.dirs' index #
    #for dir in $($CAT $Package.dirs | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.dirs | sed 's/[ \(\)]/\\&/g' | while read dir; do
      check_dir "${INSTDIR}${dir}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${dir}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) directory index '$Package.dirs'.";
  fi;

  # Check '$Package' index link list #
  if test -f "$Package.links"; then
    echolog_debug "$DEBUG Found '$Package $Version' ($Arch) link index '$Package.links'.";

    # Check links in '$Package.links' index #
    #for link in $($CAT $Package.links | sed 's/[ \(\)]/\\&/g'); do
    $CAT $Package.links | sed 's/[ \(\)]/\\&/g' | while read link; do
      check_link "${INSTDIR}${link}" || errorcheck $? "gen_install_binary_package [${INSTDIR}${link}] failed!";
    done;
  else
    echolog_debug "$DEBUG Not found '$Package $Version' ($Arch) link index '$Package.links'.";
  fi;


  echolog_debug "$DEBUG All '$Package $Version' ($Arch) indexed files are installed to system '$INSTDIR'.";

  # End work_spinner #
  stop_spinner $?;

  ### Starting 'Final Configuration' for installed package ###  
  start_spinner "Setting up '${color_wht}$Package $Version${color_reset}' (${color_wht}$Arch${color_reset})";

  echolog_debug "$DEBUG Starting '$Package $Version' ($Arch) 'Final Configuration'.";    

  # Post install setup for '$Package' #
  if [ "$ENABLE_NOSCRIPTS" != "yes" ]; then
    echolog_debug "$DEBUG Executing 'PostInst' script for '$Package $Version' ($Arch) ...";
    $CHMOD +x PostInst; $BASH PostInst &>PostInst.log;
  fi;

  # Delete $ROOTFS.tmp
  echolog_debug "$DEBUG Deleting 'root.tmp' file.";
  check_file "$ROOTFS.tmp" "$RM $ROOTFS.tmp";
  
  # Delete sysconfig files
  echolog_debug "$DEBUG Deleting 'etc' sysconfig directory.";
  check_dir "etc" "$RM -rf etc";

  if test "$INSTDIR" == "/"; then
    # Reconfiguring 'ld' dinamic linker #
    $LDCONFIG &>/dev/null;

    # Update 'Shared Info Database' #
    update_info_dir;

    # Update "perllocal.pod" #
    update_perllocal_pod;

    # Update "glib-schemas" #
    update_glib_schemas;

    # Update "desktop-file-utils" applications database #
    update_desktop_database;

    # Update mime applications database #
    update_mime_database;

    # Update "fc-cache" font cache #
    update_fc_cache;
  else
    $EXPORT GENETIC_PACKAGES_DB="$CACHEDIR/packages.db";
  fi;

  # Update genetic 'Installed Packages Database' #
  update_genetic_packages_db;

  # Reset $INSTDIR #
  if test -z "$INSTDIR"; then INSTDIR="/"; fi;

  # Reset $CACHEDIR
  $UNSET CACHEDIR;
  CACHEDIR="$CACHEDIR_ORIG";

  $UNSET GENETIC_PACKAGES_DB;  
  GENETIC_PACKAGES_DB="$CACHEDIR/packages.db";

  echolog_debug "$DEBUG Finished '$Package $Version' ($Arch) 'Final Configuration'.";

  ### End of 'Final Configuration' for installed package ###      

  echolog_debug "$DEBUG Package '$Package $Version' ($Arch) has been installed.";

  # End work_spinner #
  stop_spinner $?;

  return $true;
}


# vi: syntax=bash ts=2 sw=2 sts=2 sr noet expandtab
# vi: filetype=bash
