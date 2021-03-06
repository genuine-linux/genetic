#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-build) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# libgenetic-build.la: Package Manager Build functions and definitions #

# gen_build_package($SrcInfoFile): Select build packages from package source (SrcInfo) #
gen_build_package() {
	
	# Parameters: 'SrcInfo' file from source package #
	SrcInfoFile="$1";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_build_package($SrcInfoFile)'!";
	echolog_debug "$DEBUG =====================================================";

	# Chek $SrcInfoFile name #
	if test "$SrcInfoFile" != "SrcInfo"; then
		echolog "$ERROR Error! Expected '${color_wht}genetic${color_reset}' source package '${color_wht}SrcInfo${color_reset}' file!"
		
		# Close genetic instance #
		exit_instance $false;
	fi;

	# If SrcInfo do not exists exit #
	check_file "$SrcInfoFile" || errorcheck $? "gen_build_package";

	### First Read '$source_package $source_version' from '$SrcInfoFile'source package ###
	echologn_debug_verbose "$DEBUG Reading source package '$SrcInfoFile' ..."
	source_package=$($CAT $SrcInfoFile | $GREP "^source=" | $AWK -F"=" '{print $2}');
	source_version=$($CAT $SrcInfoFile | $GREP "^version=" | $AWK -F"=" '{print $2}');
	echolog_debug_nodate_verbose "$source_package $source_version'.";

	### Check for '$source_package' build depends #
	gen_check_package_depends "$SrcInfoFile";

	### Create '$source $version' original source package ###
	if [ "$DISABLE_GEN_ORIG" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		echolog "$WARNING Warning! Source '${color_wht}original${color_reset}' package '${color_wht}$source_package $source_version${color_reset}' will not be created!";
	else
		gen_create_orig_package "$source_package" "$source_version";
		errorcheck $? "gen_build_package -> gen_create_orig_package";
	fi;

	### Create '$source $version' source package ###
	if [ "$DISABLE_GEN_SOURCE" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		echolog "$WARNING Warning! Source '${color_wht}genetic${color_reset}' package '${color_wht}$source_package $source_version${color_reset}' will not be created!";
	else
		gen_create_source_package "$source_package" "$source_version";
		errorcheck $? "gen_build_package -> gen_create_source_package";
	fi;

	### Select binary package to be built from this sources ###

	# List packages to be built from 'SrcInfo' file #
	package_to_build=$($CAT $SrcInfoFile | $GREP "package=" | $AWK -F"=" '{print $2}');

	# Package list horiz format #
	package_list=$($ECHO $package_to_build | $AWK -v ORS=" " '{print $0}');

	# Load packages from source file #
	packages_from_source="$($ECHO "$package_to_build" | $WC -l)";

	# Select package to build from this source #
	PS3="$SELECT Select package to build: ";
	if test $packages_from_source -gt 1; then
		echolog "$WARNING Warning! This source can build '${color_red}#$packages_from_source${color_reset}' packages: '${color_wht}$package_list${color_reset}'";
		# Select package from list
		select package in $package_to_build; do
			# If package is empty retry else cotinue
			if test ! -z "$package"; then
				package_to_build="$package";
				echolog_debug_verbose "$DEBUG Warning! Selected package '$package' will be built!";
				break;
			else
				echolog "$WARNING Warning! Wrong package selection, try again!";
				continue;
			fi;
		done
	fi

	### Start binary package build into fakeroot directory ###
	echolog_debug_verbose "$DEBUG Build binary '$package_to_build' from source '$source_package'.";
	gen_build_binary_package "$package_to_build";

	return $true;
}

# gen_build_binary_package($package_to_build): Build packages from package source #
gen_build_binary_package() {
	# Parameters: $package #
	name="$1";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_build_binary_package($name)'!";
	echolog_debug "$DEBUG =====================================================";

	### Build package '$name $version' sources ###

	# Change to Rules file directory.
	$CD $name;

	# If '$name/Rules' file does not exist then exit #
	check_file "Rules" "$CHMOD +x Rules" || errorcheck $? "gen_build_binary_package [Rules]";

	# Load package Info file vars
	check_file "Info" "source ./Info" || errorcheck $? "gen_build_binary_package [Info]";

	### Update package Genuine build version ###
	bversion=0;

	CURRENT_GEN_BUILD_VERSION_FILE=$($FIND . -iname "${GEN_BUILD_VERSION_PREFIX}*" | $TAIL -1);

	if test ! -f "$CURRENT_GEN_BUILD_VERSION_FILE"; then
			echolog_debug_verbose "$DEBUG This package has not defined a build version file!";
	else
		CURRENT_GEN_BUILD_VERSION=$($CAT ./$CURRENT_GEN_BUILD_VERSION_FILE | $GREP "$GEN_BUILD_VERSION_PREFIX" | $AWK -F"~" '{print $NF}');
		if test -z "$CURRENT_GEN_BUILD_VERSION"; then
			echolog_debug_verbose "$DEBUG This package has not defined a build version!";
		else
			bversion=$((CURRENT_GEN_BUILD_VERSION+1));
		fi;
	fi;

	GEN_BUILD_VERSION="${GEN_BUILD_VERSION_PREFIX}${bversion}";

	$ECHO "${GEN_BUILD_VERSION}" > ./${GEN_BUILD_VERSION_PREFIX}${bversion};

	check_file "$CURRENT_GEN_BUILD_VERSION_FILE" "$RM -f ./$CURRENT_GEN_BUILD_VERSION_FILE";

	# Define package build version #
	version="${version}${GEN_BUILD_VERSION}";

	echolog "$WARNING Warning! Package '${color_wht}$name${color_reset}' build version is '${color_wht}$version${color_reset}'!";

	# Create build version files backup #
	echolog_debug_verbose "$DEBUG Backup '$name $version' build version configuration files ...";
	$TAR cfj $GENETIC_TMP/$name-$version.dsc.tar.bz2 ../$name ../SrcInfo &>/dev/null;

	# Package info debug #
	echolog_debug_verbose "$DEBUG Loaded '$name $version' ($GENETIC_ARCH).";

	### Apply package source patches ###

	# Search for patch files #
	echologn_debug_verbose "$DEBUG Searching '$name $version' patch files ...";
	PATCHES=$($FIND patches -type f | sort);
	echolog_debug_nodate_verbose "$FINISHED";

	# If patches list is empty no patches will be applied #
	if test ! -z "$PATCHES"; then
		# Order patches list #
		PATCHES_LIST=$($ECHO "$PATCHES" | $AWK -v ORS=" " '{print $0}');
	
		echolog_debug_verbose "$DEBUG Patch files found: '$PATCHES_LIST'.";

		# Backup source code #
		echolog_debug_verbose "$DEBUG Backing up $source_package $source_version source code ...";
		$CP -a ../$source_package-$source_version ../$source_package-$source_version.orig;

		# Apply individual patch files to source code
		for patch in $PATCHES; do
			# Change to source code #
			$CD ../$source_package-$source_version;

			# work_spinner apply patches #
			echolog_debug "$DEBUG Apply patch '$patch' to source code.";
			start_spinner "Applying patch file '${color_wht}${patch}${color_reset}'" "../$name/patches-$name-$BUILDLOG";
			$PATCH -Np1 -i ../$name/$patch &>>../$name/patches-$name-$BUILDLOG
			stop_spinner $?;

			# TODO read patches log file TODO #

			# Back to $name #
			$CD ../$name;
		done;
	else
		# No patches found #
		echolog_debug_verbose "$DEBUG Package '$source_package $source_version' does not need any patch!";
	fi;

	### GNU Autotools ###
  if test "$ENABLE_AUTOTOOLS" == "yes"; then
    echolog_debug_verbose "$DEBUG Use of 'GNU Autotools' activated!";

		if test -f "../$source_package-$source_version/configure.ac"; then
			# Installed GNU Autotools Autoconf version #
			AUTOCONF_VERSION=$($AUTOCONF --version | $HEAD -1 | $AWK '{print $NF}');

			# Checking configure.ac Autoconf required version #
			AUTOCONF_PREREQ=$($GREP ^AC_PREREQ ../$source_package-$source_version/configure.ac | $AWK -F'(' '{print $NF}' | $AWK -F')' '{print $1}' | $SED 's/\[//g' | $SED 's/\]//g');

			# If configure.ac has not defined AC_PREREQ #
			# Then check for 'configure' generated by Autoconf (Version) #
			if test -z "$AUTOCONF_PREREQ"; then
				echolog_debug_verbose "$DEBUG 'AC_PREREQ' is not defined in 'configure.ac'!";
				AUTOCONF_PREREQ=$($GREP Autoconf ../$source_package-$source_version/configure | $GREP ^generated | $HEAD -1 | $AWK '{print $NF}');
				if test -z "$AUTOCONF_PREREQ"; then
					echolog_debug_verbose "$DEBUG Check 'configure' defined 'GNU Autoconf' version ($AUTOCONF_PREREQ)!";
				fi;
			fi;

			# If have $AUTOCONF_PREREQ then #
			if test ! -z "$AUTOCONF_PREREQ"; then
				echolog_debug_verbose "$DEBUG File 'configure.ac' requires 'GNU Autoconf' version ($AUTOCONF_PREREQ)!";

				# Check if AC_PREREQ matches installed Autoconf version #
				if test "$AUTOCONF_PREREQ" == "$AUTOCONF_VERSION"; then
					# If installed autoconf is the same version as required autoconf then run autoconf #
					# If required version is not defined then run autoconf #

					$CD ../$source_package-$source_version;

					start_spinner "Running '${color_wht}GNU Autotools${color_reset}' attempting to create '${color_wht}configure${color_reset}'";

					# Run distclean #
					if [ "$DISABLE_DISTCLEAN" != "yes" ]; then
						if test -f "Makefile"; then
							echolog_debug "$DEBUG A previous 'Makefile' was found! Cleaning sources: '$MAKE distclean' ..."; 
							$MAKE distclean &> ../$name/name-distclean-$BUILDLOG;
							$RM -v *.log &> ../$name/name-distclean-$BUILDLOG;
						else
							echolog_debug "$DEBUG No previous 'Makefile' was found!"; 
						fi;
					else
						echolog_debug "$DEBUG Make distclean disabled on user demand!";
					fi

					# Run GNU Autotools #
					#$ACLOCAL --verbose &> ../$name/$name-$ACLOCAL-$BUILDLOG;
					#$LIBTOOLIZE -v &> ../$name/$name-$LIBTOOLIZE-$BUILDLOG;
					#$AUTOCONF -v &> ../$name/$name-$AUTOCONF-$BUILDLOG;
					#$AUTOMAKE -a -v &> ../$name/$name-$AUTOMAKE-$BUILDLOG;
					$AUTORECONF -v &> ../$name/$name-$AUTOCONF-$BUILDLOG;
					stop_spinner $?;

					$CD ../$name;
				else
					echolog_debug_verbose "$DEBUG File 'configure.ac' requires 'GNU Autoconf' version ($AUTOCONF_PREREQ) but installed is ($AUTOCONF_VERSION)!";
				fi;
			else
				echolog_debug_verbose "$DEBUG File 'configure.ac' does not require a 'GNU Autoconf' specific version!";

				echolog_debug_verbose "$DEBUG Running 'GNU Autotools' in '$source_package $source_version' sources!";

				$CD ../$source_package-$source_version;

				start_spinner "Running '${color_wht}GNU Autotools${color_reset}' attempting to create '${color_wht}configure${color_reset}'";

				# Run distclean #					
				if [ "$DISABLE_DISTCLEAN" != "yes" ]; then
					if test -f "Makefile"; then
						echolog_debug "$DEBUG A previous 'Makefile' was found! Cleaning sources: '$MAKE distclean' ..."; 
						$MAKE distclean &> ../$name/name-distclean-$BUILDLOG;
					else
						echolog_debug "$DEBUG No previous 'Makefile' was found!"; 
					fi;
				else
					echolog_debug "$DEBUG Make distclean disabled on user demand!";
				fi;
				
				# Run GNU Autotools #
				#$ACLOCAL --verbose &> ../$name/$name-$ACLOCAL-$BUILDLOG;
				#$LIBTOOLIZE -v &> ../$name/$name-$LIBTOOLIZE-$BUILDLOG;
				#$AUTOCONF -v &> ../$name/$name-$AUTOCONF-$BUILDLOG;
				#$AUTOMAKE -a -v &> ../$name/$name-$AUTOMAKE-$BUILDLOG;
				$AUTORECONF -v &> ../$name/$name-$AUTOCONF-$BUILDLOG;
				stop_spinner $?;

				$CD ../$name;
			fi;
		else
			echolog_debug_verbose "$DEBUG No 'configure.ac' file was found!"; 
		fi;
  else
    echolog_debug_verbose "$DEBUG Use of 'GNU Autotools' disabled by user!";
  fi;

	### Start building '$name $version' sources ###
	echolog_debug_verbose "$DEBUG Create '$name $version' packages on 'Genuine $HOST_OS $GENETIC_RELEASE Release'.";

	start_spinner "Building package '${color_wht}$name $version${color_reset}' (${color_wht}$GENETIC_ARCH${color_reset}), please wait";

	### Clean source directory ###
	if [ "$DISABLE_CLEAN" != "yes" ]; then
		if test -f "../$source_package-$source_version/Makefile"; then
			echolog_debug "$DEBUG Package '$source_package $source_version' Makefile found! Sources does need cleaning!";
			$CD ../$source_package-$source_version;
			echologn_debug "$DEBUG Cleaning '$source_package $source_version' sources";
			$MAKE clean &> ../$name/$name-clean-$BUILDLOG;
			echolog_debug_nodate "$FINISHED";
			$CD ../$name;
		else
			echolog_debug "$DEBUG Package '$source_package $source_version' Makefile not found! Sources does not need cleaning!";
		fi
	else
		echolog_debug "$DEBUG Make clean disabled on user demand!";
	fi;

	# Clear install directory #
	echolog_debug "$DEBUG Clear '$ROOTFS' temporary directory if exits ...";
	check_dir "$ROOTFS" "$RM -rf $ROOTFS";

	# Load work_spinner to show build progress #
	echolog_debug "$DEBUG Please be patient! Building '$name $version' ($GENETIC_ARCH) with default 'Rules' file!";

	# Clean log file '$name-$BUILDLOG' #
	echolog_debug "$DEBUG Write a clean log file '$name-$BUILDLOG'.";
	$ECHO " --- Build '$name $version' ($GENETIC_ARCH) ---" > $name-$BUILDLOG;
	$ECHO " --- Binary package --prefix=$GEN_PKGRULES_PREFIX ---" >> $name-$BUILDLOG;

	# Execute Rules and save build log #
	echolog_debug "$DEBUG Load '$name $version' 'Rules' script and save output to '$name-$BUILDLOG'.";
	echolog_debug "$DEBUG Building '$name $version' on ($GENETIC_ARCH), please wait ...";

	$BASH Rules &> $name-$BUILDLOG;
	stop_spinner $?;

	### Verifying '$name $version' build ###
	# Build log file '$name-$BUILDLOG' found! #
	echologn_debug_verbose "$DEBUG Verifying '$name $version' build log '$name-$BUILDLOG' ...";

	# Search for 'configure' or 'make' errors #
	CHECK_MAKE_STOP=$($TAIL -n 20 $name-$BUILDLOG | $GREP "^make" | $GREP "Stop");
	CHECK_MAKE_ERROR=$($TAIL -n 20 $name-$BUILDLOG | $GREP "^make" | $GREP "Error");
	CHECK_CONFIGURE_ERROR=$($TAIL -n 20 $name-$BUILDLOG | $GREP "configure:" | $GREP "error:");

	echolog_debug_nodate_verbose "$FINISHED";

	if test -z "$CHECK_MAKE_STOP" || test -z "$CHECK_MAKE_ERROR" || test -z "$CHECK_CONFIGURE_ERROR"; then
		# No build errors found #
		echolog_debug_verbose "$DEBUG No errors found while building '$name $version'.";
	else
		# Build errors found #
		echolog "$ERROR Error! Errors found while building '${color_wht}$name $version${color_reset}'!!!";
		echolog "$ERROR Error! All errors saved to '${color_wht}$name-$BUILDLOG${color_reset}' file.";

		# Close genetic instance #
		exit_instance $false;
	fi;

	# Setup package target arquitecture in "PkgArch" file #
	echolog_debug_verbose "$DEBUG Saving package arquitecture '$GENETIC_ARCH' to file 'PkgArch'.";
	$ECHO "$GENETIC_ARCH" > PkgArch;

	# Change to installed package files directory '$ROOTFS' #
	check_dir "$ROOTFS" "$CD $ROOTFS";

	# Indexing <$ROOTFS> intalled files & directories #
	start_spinner "Indexing '${color_wht}$name $version${color_reset}' <${color_wht}$ROOTFS${color_reset}> files, links and directories";

	# Index of package man-page files ### (man) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed man-page files in '$name.man.files.raw' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/man/' > ../$name.man.files.raw;

	# Index of package man-page links ### (man) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed man-page links in '$name.man.links.raw' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/man/' > ../$name.man.links.raw;

	# Compress MAN Pages #
	echolog_debug "$DEBUG Compressing '$name $version' <$ROOTFS> installed man-page files ...";
	for manpage in $($CAT ../$name.man.files.raw); do
		check_file ".$manpage" "$GZIP .$manpage";
	done

	# Index of package man-page files ### (man.gz) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> compressed man-page files in '$name.man.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/man/' > ../$name.man.files;

	### TODO ###
	# Update package man-page links ### (man.gz ###
	# echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> compressed man-page links in '$name.man.links' ...";
	# for manpage_link in $($CAT ../$name.man.links.raw); do
	# 	manpage=$(readlink $manpage_link);
	# 	mandir=$(dirname $manpage_link);
	# 	cd $mandir;
	# 	$LN -sf $manpage.gz $manpage_link.gz;
	# 	cd -;
	# done
	# $FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/man/' > ../$name.man.links;
	### TODO END ###
	
	# Backup Shared INFO #
	echolog_debug "$DEBUG Backup '$name $version' <$ROOTFS> installed 'usr/share/info/dir' file ...";
	check_file "usr/share/info/dir" "$MV usr/share/info/dir ../$name.usr.share.info.dir; $RM usr/share/info/dir";

  # Delete perllocal.pod from Perl Modules #
	echolog_debug "$DEBUG Delete 'perllocal.pod' from '$name $version' Perl modules.";
  PERLLOCALPOD=$($FIND . -type f -name "perllocal.pod");
  if test -f "$PERLLOCALPOD"; then
    $ECHO "$PERLLOCALPOD" > ../$name.perllocal.pod;
    $RM $PERLLOCALPOD;
  fi;

  # Indexing and linking TARGET binary files when needed #
  echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed target binary files in '$name.target.bin.files' ...";
  $FIND . -type f | $GREP "$GENUINE_TGT" | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/bin/' >../$name.target.bin.files;
  echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed target binary files in '$name.target.sbin.files' ...";
  $FIND . -type f | $GREP "$GENUINE_TGT" | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/sbin/' >../$name.target.sbin.files;

  for binfile in $(cat ../$name.target.bin.files ../$name.target.sbin.files); do
    BINFILE_DIR=$(dirname $binfile | sed 's/^\///' | sed 's/\//\\\//g');
    BINFILETGT=$(echo $binfile | sed 's/'$BINFILE_DIR'//g' | sed 's/\///g');
    BINFILE=$(echo $BINFILETGT | sed 's/'$GENUINE_TGT-'//g');
    cd $BINFILE_DIR
    if [ ! -f "$BINFILE" ]; then
      if [ ! -s "$BINFILE" ]; then
        echolog_debug "$DEBUG Linking binary file $BINFILE_DIR/$BINFILETGT to $BINFILE_DIR/$BINFILE ...";
        ln -sf $BINFILETGT $BINFILE;
      fi;
    fi;
    cd - >/dev/null
  done

  # for binfile in $(cat ../$name.target.bin.files ../$name.target.sbin.files); do
  #   BINFILE_DIR=$($DIRNAME $binfile);
  #   BINFILETGT=${binfile/$BINFILE_DIR/};
  #   BINFILE=${BINFILETGT/$GENUINE_TGT-/};
	 #  pushd .$BINFILE_DIR
	#   if [ ! -f "$BINFILE" ]; then
	#   	if [ ! -s "$BINFILE" ]; then
  #   			echolog_debug "$DEBUG Linking binary file '$BINFILE_DIR$BINFILETGT' to '$BINFILE_DIR$BINFILE' ...";
	#   		$LN -sf $BINFILETGT $BINFILE;
	#   	fi
  #   fi
  #   popd
  # done

	  # $RM ../$name.target.bin.files ../$name.target.sbin.files;
  	  echolog_debug "$DEBUG Finished linking binary files!";

  # Indexing and linking TARGET man files when needed #
  echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed target man files in '$name.target.man.files' ...";
  $FIND . -type f | $GREP "$GENUINE_TGT" | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/man/' >../$name.target.man.files;

  for manfile in $(cat ../$name.target.man.files); do
MANFILE_DIR=$(dirname $manfile | sed 's/^\///' | sed 's/\//\\\//g');
MANFILETGT=$(echo $manfile | sed 's/'$MANFILE_DIR'//g' | sed 's/\///g');
MANFILE=$(echo $MANFILETGT | sed 's/'$GENUINE_TGT-'//g');
cd $MANFILE_DIR
if [ ! -f "$MANFILE" ]; then
  if [ ! -s "$MANFILE" ]; then
      echolog_debug "$DEBUG Linking man file $MANFILE_DIR/$MANFILETGT to $MANFILE_DIR/$MANFILE ...";
    ln -sf $MANFILETGT $MANFILE;
  fi;
fi;
cd - >/dev/null
done

  # for manfile in $(cat ../$name.target.man.files); do
  #   MANFILE_DIR=$($DIRNAME $manfile);
  #   MANFILETGT=${manfile/$MANFILE_DIR/};
  #   MANFILE=${MANFILETGT/$GENUINE_TGT-/};
	 # #  pushd .$MANFILE_DIR
	#   if [ ! -f "$MANFILE" ]; then
	#   	if [ ! -s "$MANFILE" ]; then
 #  			echolog_debug "$DEBUG Linking man file '$MANFILE_DIR$MANFILETGT' to '$MANFILE_DIR$MANFILE' ...";
# 	  		$LN -sf $MANFILETGT $MANFILE;
# 	  	fi
 #    fi
  #   popd
 #  done

  # $RM ../$name.target.man.files;
  echolog_debug "$DEBUG Finished linking man files!";

	# Index of package debug files ### (debug) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed debug files in '$name.dbg.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP -e '\.la$' -e '\.a$' >../$name.dbg.files;

	# Index of package debug links #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed debug links in '$name.dbg.links' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP -e '\.la$' -e '\.a$' > ../$name.dbg.links;

	# Index of package debug dirs #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed debug directories in '$name.dbg.dirs' ...";
	$FIND . -type f -name '*\.la' -exec dirname {} \; | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $SORT | $UNIQ > ../$name.dbg.dirs;
	$FIND . -type f -name '*\.a' -exec dirname {} \; | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $SORT | $UNIQ >> ../$name.dbg.dirs;
	#$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP -e '\.la$' -e '\.a$' | $AWK -F'/' '{print $0-$NF}' > ../$name.dbg.dirs;

	# Index of package library files ### (lib) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed library files in '$name.lib.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/lib/' | $GREP -v '\.la$' | $GREP -v '\.a$' | $GREP -v '/usr/include/' >../$name.lib.files;

	# Index of package library links #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed library links in '$name.lib.links' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/lib/' | $GREP -v '\.la$' | $GREP -v '\.a$' | $GREP -v '/usr/include/' > ../$name.lib.links;

	# Index of package library dirs #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed library directories in '$name.lib.dirs' ...";
	$FIND . -type d | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/lib' | $GREP -v '/usr/include' > ../$name.lib.dirs;

	# Index of package sysconfig files ### (etc) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed sysconfig files in '$name.etc.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/etc/' >../$name.etc.files;

	# Index of package sysconfig links #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed sysconfig links in '$name.etc.links' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/etc/' > ../$name.etc.links;

	# Index of package sysconfig dirs #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed sysconfig directories in '$name.etc.dirs' ...";
	$FIND . -type d | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/etc' > ../$name.etc.dirs;

	# Index of package header files ### (dev) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed header files in '$name.dev.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/usr/include/' > ../$name.dev.files;

	# Index of package header links #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed header links in '$name.dev.links' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/usr/include/' > ../$name.dev.links;

	# Index of package header dirs #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed header directories in '$name.dev.dirs' ...";
	$FIND . -type d | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/usr/include' > ../$name.dev.dirs;

	# Index of package doc files ### (doc) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed doc files in '$name.doc.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/doc/' > ../$name.doc.files;

	# Index of package doc links #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed doc links in '$name.doc.links' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/doc/' > ../$name.doc.links;

	# Index of package doc dirs #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed doc directories in '$name.doc.dirs' ...";
	$FIND . -type d | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP '/share/doc' > ../$name.doc.dirs;

	# Index of package files (Exclude previous files) ### (main) ###
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed files in '$name.files' ...";
	$FIND . -type f | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP -v '/usr/include/' | $GREP -v '/etc/' | $GREP -v '/share/doc/' | $GREP -v '/lib/' | $GREP -v '/lib64/' | $GREP -v '\.a$' | $GREP -v '\.la$' > ../$name.files;

	# Index of package linked files (Exclude previous files) #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed links in '$name.links' ...";
	$FIND . -type l | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP -v '/usr/include/' | $GREP -v '/etc/' | $GREP -v '/share/doc/' | $GREP -v '/lib/' | $GREP -v '/lib64/' | $GREP -v '\.a$' | $GREP -v '\.la$' > ../$name.links;

	# Index of package directories (Exclude previous directories) #
	echolog_debug "$DEBUG Indexing '$name $version' <$ROOTFS> installed directories in '$name.dirs' ...";
	$FIND . -type d | $SED 's/^\.//g' | $AWK '{if ($0!="") print $0}' | $GREP -v '/usr/include' | $GREP -v '/etc' | $GREP -v '/share/doc' | $GREP -v '/lib' | $GREP -v '/lib64' > ../$name.dirs;

	stop_spinner $?;

	# Back to $GENETIC_TMP/$source_package-$source_version/$name #
	$CD $GENETIC_TMP/$source_package-$source_version/$name

	# Get all required files to build genetic packages
	GEN_PACKAGE_FILES="$name.etc.files $name.etc.dirs $name.etc.links $name.man.files $name.files $name.dirs $name.links";

	# debug package disabled
	if [ "$DISABLE_GEN_DEBUG" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		GEN_PACKAGE_FILES="$name.dbg.files $name.dbg.dirs $name.dbg.links $GEN_PACKAGE_FILES";
	fi;

	# library package disabled
	if [ "$DISABLE_GEN_LIBRARY" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ] ; then
		GEN_PACKAGE_FILES="$name.lib.files $name.lib.dirs $name.lib.links $GEN_PACKAGE_FILES";
	fi;

	# Check indexed $package.files has files #
	GEN_FILES=$($CAT $GEN_PACKAGE_FILES);

	if test -z "$GEN_FILES"; then
		# Index files are empty #
		echolog_debug_verbose "$DEBUG Package '$name $version' <$ROOTFS> index files are empty!";

		echolog "$ERROR Error! No installed files found in <${color_wht}$ROOTFS${color_reset}> for creating '${color_wht}$name $version${color_reset}' binary package!";
		echolog "$WARNING Warning! Check for errors in '${color_wht}$name-$BUILDLOG${color_reset}'!";
		echolog "$WARNING Warning! Review package '${color_wht}Rules${color_reset}', maybe it needs some tweaking!";

		# Close genetic instance #
		exit_instance $false;
	else
		echolog_debug_verbose "$DEBUG Installed files found in <$ROOTFS> for creating '$name $version' binary and debug packages!";
	fi;

	### Finished building '$name-$version ($GENETIC_ARCH)' package ###

	### Call libgenetic-packager.la functions for creating required packages ###

	### Create '$name-$version.$GENETIC_ARCH.dbg.gen' library package ###
	if [ "$DISABLE_GEN_DEBUG" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		echolog "$WARNING Warning! Genetic '${color_wht}$name $version${color_reset}' debug package will not be created!";
	else
		GEN_DBGFILES=$($CAT $name.dbg.files $name.dbg.dirs $name.dbg.links);
		if test ! -z "$GEN_DBGFILES"; then # If debug files found create a debug package
			echolog_debug_verbose "$DEBUG Installed files found in <$ROOTFS> for creating '$name $version' debug package!";
			gen_create_debug_package "$name" "$version" "$bversion" "$GENETIC_ARCH";
			errorcheck $? "gen_build_binary_package -> gen_create_debug_package";
		else
			echolog "$WARNING Warning! No installed files found in <${color_wht}$ROOTFS${color_reset}> for creating '${color_wht}$name $version${color_reset}' debug package!";
		fi;
	fi;

	### Create 'lib$name-$version.$GENETIC_ARCH.lib.gen' library package if package is not a library ###
	libname="lib$name";

	# Fix some packages with "lib" in it's name
	case "$name" in
		libre*|*libre) DISABLE_GEN_LIBRARY="no"
		;;
		lib*|Lib*|LIB*) DISABLE_GEN_LIBRARY="yes"
			libname="$name"
		;;
		glibc) libname="libc"
		;;
		zlib) libname="libz"
		;;
	esac

	if [ "$DISABLE_GEN_LIBRARY" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		if [ "$libname" == "$name" ]; then
			echolog "$WARNING Warning! Genetic '${color_wht}$libname $version${color_reset}' is a library package already!";
		fi
		echolog "$WARNING Warning! Genetic '${color_wht}$libname $version${color_reset}' library package creation disabled!";
	else
		GEN_LIBFILES=$($CAT $name.lib.files $name.lib.dirs $name.lib.links);
		if test ! -z "$GEN_LIBFILES"; then # If library files found create a library package
			echolog_debug_verbose "$DEBUG Installed files found in <$ROOTFS> for creating '$libname $version' library package!";
			gen_create_lib_package "$name" "$libname" "$version" "$bversion" "$GENETIC_ARCH";
			errorcheck $? "gen_build_binary_package -> gen_create_lib_package";
		else
			echolog "$WARNING Warning! No installed files found in <${color_wht}$ROOTFS${color_reset}> for creating '${color_wht}$libname $version${color_reset}' library package!";
		fi;
	fi;

	### Create '$name-$version.$GENETIC_ARCH.dev.gen' devel package ###
	GEN_DEVFILES=$($CAT $name.dev.files $name.dev.dirs $name.dev.links);
	if test ! -z "$GEN_DEVFILES"; then # If header files found create a devel package
		echolog_debug_verbose "$DEBUG Installed files found in <$ROOTFS> for creating '$name $version' devel package!";
		gen_create_devel_package "$name" "$version" "$bversion" "$GENETIC_ARCH";
		errorcheck $? "gen_build_binary_package -> gen_create_devel_package";
	else
		echolog "$WARNING Warning! No installed files found in <${color_wht}$ROOTFS${color_reset}> for creating '${color_wht}$name $version${color_reset}' devel package!";
	fi;

	### Create '$name-$version.$GENETIC_ARCH.doc.gen' documentation package ###
	GEN_DOCFILES=$($CAT $name.doc.files $name.doc.dirs $name.doc.links);
	if test ! -z "$GEN_DOCFILES"; then # If doc files found create a documentation package
		echolog_debug_verbose "$DEBUG Installed files found in <$ROOTFS> for creating '$name $version' documentation package!";
		gen_create_doc_package "$name" "$version" "$bversion" "$GENETIC_ARCH";
		errorcheck $? "gen_build_binary_package -> gen_create_doc_package";
	else
		echolog "$WARNING Warning! No installed files found in <${color_wht}$ROOTFS${color_reset}> for creating '${color_wht}$name $version${color_reset}' documentation package!";
	fi;

	### Create '$name-$version.$GENETIC_ARCH.gen' binary package ###
	gen_create_binary_package "$name" "$version" "$bversion" "$GENETIC_ARCH";		
	errorcheck $? "gen_build_binary_package -> gen_create_binary_package";

 	### Check generated package files ###

	### Finished creation of original source package file '$source_package-$source_version.orig.tar.bz2' ###
	if test -f "$GENETIC_TMP/$source_package-$source_version.orig.tar.bz2"; then
	ORIG_PACKAGE_SIZE=$($DU $GENETIC_TMP/$source_package-$source_version.orig.tar.bz2 | $AWK '{print $1}');
	echolog "$INFO Created original source package '${color_wht}$GENETIC_TMP/$source_package-$source_version.orig.tar.bz2${color_reset}' size '${color_wht}$ORIG_PACKAGE_SIZE Kb${color_reset}'!";
	fi;

	### Finished creation of source package file '$source_package-$source_version.src.gen' ###
	if test -f "$GENETIC_SOURCES/$source_package-$source_version.src.gen"; then
	SOURCE_PACKAGE_SIZE=$($DU $GENETIC_SOURCES/$source_package-$source_version.src.gen | $AWK '{print $1}');
	echolog "$INFO Created source package '${color_wht}$GENETIC_SOURCES/$source_package-$source_version.src.gen${color_reset}' size '${color_wht}$SOURCE_PACKAGE_SIZE Kb${color_reset}'!";
	fi;

	### Finished building '$name-$version.$pkgarch.dev.gen' devel package ###
	if test -f "$GENETIC_PACKAGES/$name-$version.$pkgarch.dev.gen"; then
	DEVEL_PACKAGE_SIZE=$($DU $GENETIC_PACKAGES/$name-$version.$pkgarch.dev.gen | $AWK '{print $1}');
	echolog "$INFO Created devel package '${color_wht}$GENETIC_PACKAGES/$name-$version.$pkgarch.dev.gen${color_reset}' size '${color_wht}$DEVEL_PACKAGE_SIZE Kb${color_reset}'!";
	fi;

	### Finished building '$name-$version.$pkgarch.doc.gen' documentation package ###
	if test -f "$GENETIC_PACKAGES/$name-$version.$pkgarch.doc.gen"; then
	DOC_PACKAGE_SIZE=$($DU $GENETIC_PACKAGES/$name-$version.$pkgarch.doc.gen | $AWK '{print $1}');
	echolog "$INFO Created documentation package '${color_wht}$GENETIC_PACKAGES/$name-$version.$pkgarch.doc.gen${color_reset}' size '${color_wht}$DOC_PACKAGE_SIZE Kb${color_reset}'!";
	fi;

	### Finished building '$name-$version.$pkgarch.dbg.gen' binary debug package ###
	if test -f "$GENETIC_PACKAGES/$name-$version.$pkgarch.dbg.gen"; then
	DEBUG_PACKAGE_SIZE=$($DU $GENETIC_PACKAGES/$name-$version.$pkgarch.dbg.gen | $AWK '{print $1}');
	echolog "$INFO Created debug package '${color_wht}$GENETIC_PACKAGES/$name-$version.$pkgarch.dbg.gen${color_reset}' size '${color_wht}$DEBUG_PACKAGE_SIZE Kb${color_reset}'!";
	fi;

	### Finished building '$libname-$version.$pkgarch.gen' library package ###
	if [ "$libname" != "$name" ]; then
		if test -f "$GENETIC_PACKAGES/$libname-$version.$pkgarch.gen"; then
		LIBRARY_PACKAGE_SIZE=$($DU $GENETIC_PACKAGES/$libname-$version.$pkgarch.gen | $AWK '{print $1}');
		echolog "$INFO Created library package '${color_wht}$GENETIC_PACKAGES/$libname-$version.$pkgarch.gen${color_reset}' size '${color_wht}$LIBRARY_PACKAGE_SIZE Kb${color_reset}'!";
		fi;
	fi;

	### Finished building '$name-$version.$pkgarch.gen' binary package ###
	if test -f "$GENETIC_PACKAGES/$name-$version.$pkgarch.gen"; then
	BINARY_PACKAGE_SIZE=$($DU $GENETIC_PACKAGES/$name-$version.$pkgarch.gen | $AWK '{print $1}');
	echolog "$INFO Created binary package '${color_wht}$GENETIC_PACKAGES/$name-$version.$pkgarch.gen${color_reset}' size '${color_wht}$BINARY_PACKAGE_SIZE Kb${color_reset}'!";
	fi;

	# Close genetic instance #
	exit_instance $?;
}

# gen_clean_package($SrcInfoFile): Select clean built packages from package source (SrcInfo) #
gen_clean_package() {
	
	# Parameters: 'SrcInfo' file from source package #
	SrcInfoFile="$1";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_clean_package($SrcInfoFile)'!";
	echolog_debug "$DEBUG =====================================================";

	# Chek $SrcInfoFile name #
	if test "$SrcInfoFile" != "SrcInfo"; then
		echolog "$ERROR Error! Expected '${color_wht}genetic${color_reset}' source package '${color_wht}SrcInfo${color_reset}' file!"
		
		# Close genetic instance #
		exit_instance $false;
	fi;

	# If SrcInfo do not exists exit #
	check_file "$SrcInfoFile" || errorcheck $? "gen_clean_package";

	### First Read '$source_package $source_version' from '$SrcInfoFile'source package ###
	echologn_debug_verbose "$DEBUG Reading source package '$SrcInfoFile' ..."
	source_package=$($CAT $SrcInfoFile | $GREP "^source=" | $AWK -F"=" '{print $2}');
	source_version=$($CAT $SrcInfoFile | $GREP "^version=" | $AWK -F"=" '{print $2}');
	echolog_debug_nodate_verbose "$source_package $source_version'.";

	### Select binary package to be cleaned from this sources ###

	# List packages to be cleaned from 'SrcInfo' file #
	package_to_clean=$($CAT $SrcInfoFile | $GREP "package=" | $AWK -F"=" '{print $2}');

	# Package list horiz format #
	package_clean_list=$($ECHO $package_to_clean | $AWK -v ORS=" " '{print $0}');

	# Load packages from source file #
	packages_from_source="$($ECHO "$package_to_clean" | $WC -l)";

	# Select package to clean from this source #
	PS3="$SELECT Select package to clean: ";
	if test $packages_from_source -gt 1; then
		echolog "$WARNING Warning! This source contains '${color_wht}#$packages_from_source${color_reset}' packages to be cleaned: '${color_wht}$package_clean_list${color_reset}'";

		# Select package from list
		select package in $package_to_clean; do
			# If package is empty retry else cotinue
			if test ! -z "$package"; then
				package_to_clean="$package";
				echolog_debug_verbose "$DEBUG Warning! Selected package '$package' will be cleaned!";
				break;
			else
				echolog "$WARNING Warning! Wrong package selection, try again!";
				continue;
			fi;
		done
	fi

	### Start binary package cleaning into fakeroot directory ###

	### Start building '$name $version' sources ###
	echolog_debug_verbose "$DEBUG Cleaning binary '$package_to_clean' from source '$source_package'.";

	start_spinner "Cleaning package '${color_wht}$package_to_clean${color_reset}', please wait";

	$CD $package_to_clean;

	# Clean files #
	$RM -rf $ROOTFS* &>/dev/null;
	$RM -rf $package_to_clean* *.log has_library* PkgArch &>/dev/null;

	$CD ..;	$RM -rf *.content *.log &>/dev/null;

	### Clean source directory ###
	if test -f "$source_package-$source_version/Makefile"; then
		echolog_debug "$DEBUG Package '$source_package $source_version' Makefile found! Sources does need cleaning!";
		$CD $source_package-$source_version;

		echolog_debug "$DEBUG Cleaning '$source_package $source_version' sources";
		$MAKE clean &> ../$source_package-clean-$BUILDLOG; $CD ..;
	else
		echolog_debug "$DEBUG Package '$source_package $source_version' Makefile not found! Sources does not need cleaning!";
	fi

	stop_spinner $?;

	exit_instance $?;
}

# gen_clean_packagepool(): Clean $PACKAGEPOOL #
function gen_clean_packagepool() {

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'gen_repo_file($repo_file)'!";
  echolog_debug "$DEBUG =====================================================";

	#echolog "$WARNING Warning! Cleaning '${color_wht}${PACKAGEPOOL}${color_reset}'!";

	start_spinner "Moving old packages from '${color_wht}PackagePool${color_reset}' to '${color_wht}OldPackagePool${color_reset}'";

  for gen in $($LS $PACKAGEPOOL/*.gen | $AWK -F"/" '{print $NF}' | $SED 's/\.gen//g' | $SORT); do # Sort packages by name
    # Check package type
    for pkg in "doc,dbg,dev"; do
      if $ECHO $gen | $GREP -q ".${pkg}$"; then
        gen_full_name=$($ECHO $gen | $SED 's/\.'$pkg'//g');
        gen_type=".${pkg}.gen";
      else
        gen_full_name=$gen;
        gen_type=".gen";
      fi;
    done;

    # Get package info
    gen_arch=$($ECHO "$gen_full_name" | $AWK -F"." '{print $NF}');
    gen_build=$($ECHO "$gen_full_name" | $SED 's/\.'$gen_arch'//g' | $AWK -F"~" '{print $NF}');
    version=$($ECHO "$gen_full_name" | $AWK -F"-" '{print $NF}' | $SED 's/\.'$gen_arch'//g' | $SED 's/~genuine~'$gen_build'//g');
    name=$($ECHO "$gen_full_name" | $SED 's/\.'$gen_arch'//g' | $SED 's/~genuine~'$gen_build'//g' | $SED 's/-'$version'//g');

    # Check for package old packages
    package_builds=$($LS $PACKAGEPOOL/${name}-[0-9]*~genuine~[0-9]*.${gen_arch}${gen_type} --sort=version -l | $AWK -F"/" '{print $NF}');
    last_version=$($LS $PACKAGEPOOL/${name}-[0-9]*~genuine~[0-9]*.${gen_arch}${gen_type} --sort=version -l | $AWK -F"/" '{print $NF}' | $TAIL -1);

    if [ "$package_builds" != "$last_version" ]; then
      if [ "$last_version" != "$gen" ]; then
        echolog_debug "$DEBUG Checking builds for package: $name $version $gen_build $gen_arch $gen_type";
       
        echolog_debug "$DEBUG Package $name last version: $last_version";
        
        old_versions=$($LS $PACKAGEPOOL/${name}-[0-9]*~genuine~[0-9]*.${gen_arch}${gen_type} --sort=version -l | $AWK -F"/" '{print $NF}' | $GREP -v $last_version);
        
        echolog_debug "$DEBUG Package $name old versions: $old_versions";

        for old_version in $old_versions; do
          if [ -f "$PACKAGEPOOL/$old_version" ]; then
            echologn_debug "$DEBUG Moving old package version $old_version to $OLDPACKAGEPOOL ...";
            $MV $PACKAGEPOOL/$old_version $OLDPACKAGEPOOL/;
            echolog_debug " OK!";
          fi;
        done;
      fi;
    fi;
  done;
  
  stop_spinner $?;
  
 	errorcheck $? "gen_build -> gen_clean_packagepool";

	return $?;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
