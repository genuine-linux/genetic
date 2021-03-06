#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-packager) #

# Copyright (c) 2014-2019 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# libgenetic-packager.la: Package Manager Core functions and definitions #

# gen_find_package_depts($source_package,$source_version): Create 'genetic' original package for use in future builds #
gen_find_package_deps() {
	all_deps=

	for real_file in $($CAT *.files); do
	if $FILE $real_file | $GREP -q dynamically; then
		echolog_debug "$DEBUG Checking $real_file depends: "
		deps=$($LDD $real_file | $AWK '{print $1}' | $SORT | $UNIQ)
		for dep in $deps; do
			genetic_packages=$(list_installed_files "$dep" | $GREP -v Genuine | $AWK '{print $2}' | $SORT | $UNIQ)
			for genetic_package in $genetic_packages; do
				if [ -f "$CACHEDIR/$genetic_package/Info" ]; then
					genetic_package_version=$($GREP ^version $CACHEDIR/$genetic_package/Info | \
						$AWK -F'~' '{print $1}' | $AWK -F'=' '{print $2}')
					$ECHO "$all_deps" | $GREP -q "${genetic_package}_${genetic_package_version}" || \
						all_deps="$all_deps ${genetic_package}_${genetic_package_version}"
				fi
			done
		done
		if [ ! -z "$all_deps" ]; then
			echologn "$INFO Package dependencies found: "
			$ECHO $all_deps | $SORT | $UNIQ | $AWK -v ORS=" " '{print $0}'
			$ECHO ""
		else
			echolog_debug "$DEBUG No genetic depends found for $real_file!"
		fi
	else
		echolog_debug "$ERROR $real_file is not dynamically linked."
	fi
	done
}

# gen_create_orig_package($source_package,$source_version): Create 'genetic' original package for use in future builds #
gen_create_orig_package() {
	# Parameters: $source_package, $source_version #
	source_package="$1";
	source_version="$2";

	echolog_debug "$DEBUG ================= %%% genetic %%% =================";
	echolog_debug "$DEBUG Starting 'gen_create_orig_package($source_package,$source_version)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create original source package '$source_package-$source_version.orig.tar.bz2' ###

	echolog_debug_verbose "$DEBUG Create original source package '$source_package-$source_version.orig.tar.bz2'.";

	### Start creation of original source package file '$source_package-$source_version.orig.tar.bz2' ###

	# Create original source Package '$source_package-$source_version.src.gen' sources '$source_package-$source_version' #
	echolog_debug_verbose "$DEBUG Creating original source package at '$GENETIC_TMP/$source_package-$source_version.orig.tar.bz2'!"

	# Use "work_spinner" to create original package #
	start_spinner "Creating original source package '${color_wht}$source_package-$source_version.orig.tar.bz2${color_reset}'";
	$TAR cfj $GENETIC_TMP/$source_package-$source_version.orig.tar.bz2 $source_package-$source_version &>/dev/null;
	stop_spinner $?;

	return $true;
}

# gen_create_source_package($source_package,$source_version): Create 'genetic' source package for use in future builds #
gen_create_source_package() {
	# Parameters: $source_package, $source_version #
	source_package="$1";
	source_version="$2";

	echolog_debug "$DEBUG ================= %%% genetic %%% =================";
	echolog_debug "$DEBUG Starting 'gen_create_source_package($source_package,$source_version)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create source package '$source_package-$source_version.src.gen' ###

	echolog_debug_verbose "$DEBUG Create source package '${color_wht}$source_package-$source_version.src.gen${color_reset}'.";

	# Create scripts: PostInst PreInst PostRemv PreRemv #
	# genetic will fail if these files are not found #
	echolog_debug "$DEBUG Creating scripts: PostInst, PreInst, PostRemv, PreRemv.";
	for script_file in PostInst PreInst PostRemv PreRemv; do
		if test ! -f "$source_package/$script_file"; then
			$ECHO '#!/bin/bash' > $source_package/$script_file;
		fi;
	done;

	### Check if package source needs to be cleaned ###

	# Change to source code directory #
	$CD $source_package-$source_version;

	# Search for 'Makefile' #
	if test -f "Makefile"; then
		# Makefile found! Go clean! #
		echolog_debug_verbose "$DEBUG Source 'Makefile' found! Clean '$source_package $source_version' before create a source package!";

		# If source package belongs to Linux Kernel then "make mrproper" #
		if test -f "Kconfig" && test -f "Kbuild"; then
			# Linux Kernel Source Found! #
			echologn "$WARNING Warning! Linux Kernel source found! Cleaning source with '${color_wht}make mrproper${color_reset}' ...";
			$MAKE mrproper &>/dev/null;
			echolog "$FINISHED";
		else
			# Standard Package Source Found! #
			echologn "$WARNING Warning! Cleaning source package '${color_wht}$source_package $source_version${color_reset}' files ...";

			# Search for 'distclean' in 'Makefile' #
			test_makefile_distclean=$($GREP -w "^distclean:" Makefile);

			if test ! -z "$test_makefile_distclean"; then
				# make distclean #
				$MAKE distclean &>/dev/null;
			else
				# Search for 'clean' in 'Makefile' #
				test_makefile_clean=$($GREP -w "^clean:" Makefile);

				if test ! -z "$test_makefile_clean"; then
					# make clean #
					$MAKE clean &>/dev/null;
				fi;
			fi;

			echolog "$FINISHED";
		fi;
	fi;

	### Start creation of source package file '$source_package-$source_version.src.gen' ###

	# Return back two dir levels #
	$CD ../../;

	# Create Source Package '$source_package-$source_version.src.gen' sources '$source_package-$source_version' #
	echolog_debug_verbose "$DEBUG Creating source package at '$GENETIC_SOURCES/$source_package-$source_version.src.gen'!"

	# Use "work_spinner" to create source package #
	start_spinner "Creating source package '${color_wht}$source_package-$source_version.src.gen${color_reset}'";
	$TAR cfj $GENETIC_SOURCES/$source_package-$source_version.src.gen $source_package-$source_version &>/dev/null;
	stop_spinner $?;

	# Change to sources dir (start dir) #
	$CD $source_package-$source_version;

	return $true;
}

# gen_create_lib_package($name,$version,$pkgarch): Create library installable packages from last prefixed install #
gen_create_lib_package() {
	# Parameters
	name="$1";
	libname="$2";
	version="$3";
	bversion="$4";
	pkgarch="$5";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_create_lib_package($libname,$version,$pkgarch)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create library package ###
	start_spinner "Creating library package '${color_wht}$libname-$version.$pkgarch.gen${color_reset}'"

	# Change to '$GENETIC_TMP/$source_package-$source_version' directory #
	$CD $GENETIC_TMP/$source_package-$source_version;

	### Use installed files to create package '$libname-$version.$pkgarch.gen' ###
	echolog_debug "$DEBUG Use installed files to create devel package '$libname-$version.$pkgarch.gen'.";

	# Create a temp directory #
	echolog_debug "$DEBUG Creating temporary directory '$libname.tmp'. It's needed to install package files.";
	check_dir "$libname.tmp" "$RM -rf $libname.tmp";

	$MKDIR -p $libname.tmp;

	### Write new 'lib$package_name' 'Info' file ###
	echolog_debug "$DEBUG Writing a new '$libname $version' ($pkgarch) package 'Info' file ...";
	$ECHO "# Package <$libname> info.

original_name=$source_package
name=$libname
version=$version
package=$libname-$version
depends=\"$depends\"
installbefore=
installafter=
replaces=$libname
" > $libname.tmp/Info;
	### End 'lib$package_name' 'Info' file ###

	# Copy necesary package files
	echolog_debug "$DEBUG Copying files: '$name.*', 'PkgArch' to '$libname.tmp' directory."

	$CP -a $name/${GEN_BUILD_VERSION_PREFIX}${bversion} $name/PkgArch $libname.tmp/
	$CP -a $name/$name.lib.files $libname.tmp/$libname.lib.files
	$CP -a $name/$name.lib.dirs $libname.tmp/$libname.lib.dirs
	$CP -a $name/$name.lib.links $libname.tmp/$libname.lib.links
	errorcheck $? "gen_create_lib_package";

	# Create scripts: PostInst PreInst PostRemv PreRemv #
	# genetic will fail if these files are not found #
	echolog_debug "$DEBUG Creating scripts: PostInst, PreInst, PostRemv, PreRemv.";
	for script_file in PostInst PreInst PostRemv PreRemv; do
		if test ! -f "$libname.tmp/$script_file"; then
			$ECHO '#!/bin/bash' > $libname.tmp/$script_file;
		fi;
	done

	# Moving built files to new directory 
	echolog_debug "$DEBUG Moving '$name $version' library installed files to '$libname.tmp' ...";

	# Create header indexed directories #
	for libdir in $($CAT $libname.tmp/$libname.lib.dirs); do
		if test ! -z "$libdir"; then
			$MKDIR -pv $libname.tmp/$ROOTFS/$libdir &>/dev/null
		fi;
	done;

	for libfile in $($CAT $libname.tmp/$libname.lib.files $libname.tmp/$libname.lib.links); do
		if test ! -z "$libfile"; then
			$MV $name/$ROOTFS/$libfile $libname.tmp/$ROOTFS/$libfile &>/dev/null;
		fi;
	done;

	$CD $libname.tmp/$ROOTFS;

	# Strip debug symbols from binaries and libraries #
	echologn_debug "$DEBUG Stripping debug symbols from '$libname $version' programs and libraries ...";

	for dir in bin sbin usr/bin usr/sbin usr/local/bin usr/local/sbin opt/bin usr/libexec; do
		if test -d "$dir"; then
			$FIND $dir -type f -exec $STRIP --strip-all {} \; &>../stripbin.log
		fi;
	done;

	for dir in lib usr/lib usr/local/lib opt/lib; do
    if test -d "$dir"; then
			$FIND $dir -type f -exec $STRIP --strip-debug {} \; &>../striplib.log
		fi;
	done;

	echolog_debug_nodate "$FINISHED";

	# Create tar package with root files inside gen file
	echolog_debug "$DEBUG Creating '../$ROOTFS.tmp' file from '$ROOTFS.lib' directory ...";
	$TAR cvf ../$ROOTFS.tmp * &>../../$name/$ROOTFS.lib

	# Delete unnecesary files and $ROOTFS directory
	echolog_debug "$DEBUG Deleting unnecesary files and directories '$ROOTFS', 'Rules', 'files' and 'patches' ...";
	$CD ..;	$RM -rf $ROOTFS Rules files patches &>/dev/null

	echolog_debug "$DEBUG Create library package '$libname-$version.$pkgarch.gen'.";
	$TAR cfvj $GENETIC_PACKAGES/$libname-$version.$pkgarch.gen * &> ../$libname-$version.$pkgarch.content

	# End work_spinner #
	stop_spinner $?;

	# Delete temporary directory
	echolog_debug_verbose "$DEBUG Deleting temporary working directory '$libname.tmp'."; 
	$CD ..; $RM -rf $libname.tmp &> /dev/null

	# Change to $name directory #
	$CD $name;

	# Let gen_create_binary_package function know about creation of library packages #
	$TOUCH has_library_$libname

	return $true;
}

# gen_create_devel_package($name,$version,$pkgarch): Create devel installable packages from last prefixed install #
gen_create_devel_package() {
	# Parameters
	name="$1";
	version="$2";
	bversion="$3";
	pkgarch="$4";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_create_devel_package($name,$version,$pkgarch)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create devel package ###
	start_spinner "Creating devel package '${color_wht}$name-$version.$pkgarch.dev.gen${color_reset}'"

	# Change to '$GENETIC_TMP/$source_package-$source_version' directory #
	$CD $GENETIC_TMP/$source_package-$source_version;

	### Use installed files to create package '$name-$version.$pkgarch.dev.gen' ###
	echolog_debug "$DEBUG Use installed files to create devel package '$name-$version.$pkgarch.dev.gen'.";

	# Create a temp directory #
	echolog_debug "$DEBUG Creating temporary directory '$name.tmp'. It's needed to install package files.";
	check_dir "$name.tmp" "$RM -rf $name.tmp";

	$MKDIR -p $name.tmp;

	### Write new '$package_name.dev' 'Info' file ###
	echolog_debug "$DEBUG Writing a new '$name $version' ($pkgarch) package 'Info' file ...";
	$ECHO "# Package <$name.dev> info.

original_name=$source_package
name=$name.dev
version=$version
package=$name-$version
depends=\"$depends\"
installbefore=
installafter=
replaces=$name.dev
" > $name.tmp/Info;
	### End '$package_name' 'Info' file ###

	# Copy necesary package files
	echolog_debug "$DEBUG Copying files: '$name.*', 'PkgArch' to '$name.tmp' directory."

	$CP -a  $name/${GEN_BUILD_VERSION_PREFIX}${bversion} \
	$name/$name.dev.files \
	$name/$name.dev.dirs \
	$name/$name.dev.links \
	$name/PkgArch $name.tmp/
	errorcheck $? "gen_create_devel_package";

	# Moving built files to new directory 
	echolog_debug "$DEBUG Moving '$name $version' devel installed files to '$name.tmp' ...";

	# Create header indexed directories #
	for devdir in $($CAT $name.tmp/$name.dev.dirs); do
		if test ! -z "$devdir"; then
			$MKDIR -pv $name.tmp/$ROOTFS/$devdir &>/dev/null
		fi;
	done;

	for devfile in $($CAT $name.tmp/$name.dev.files $name.tmp/$name.dev.links); do
		if test ! -z "$devfile"; then
			$MV $name/$ROOTFS/$devfile $name.tmp/$ROOTFS/$devfile &>/dev/null;
		fi;
	done;

	$CD $name.tmp/$ROOTFS;
	
	# Create tar package with root files inside gen file
	echolog_debug "$DEBUG Creating '../$ROOTFS.tmp' file from '$ROOTFS' directory ...";
	$TAR cvf ../$ROOTFS.tmp * &>../../$name/$ROOTFS.dev

	# Delete unnecesary files and $ROOTFS directory
	echolog_debug "$DEBUG Deleting unnecesary files and directories '$ROOTFS', 'Rules', 'files' and 'patches' ...";
	$CD ..;	$RM -rf $ROOTFS Rules files patches &>/dev/null

	echolog_debug "$DEBUG Create devel package '$name-$version.$pkgarch.dev.gen'.";
	$TAR cfvj $GENETIC_PACKAGES/$name-$version.$pkgarch.dev.gen * &> ../$name-$version.$pkgarch.dev.content

	# End work_spinner #
	stop_spinner $?;

	# Delete temporary directory
	echolog_debug_verbose "$DEBUG Deleting temporary working directory '$name.tmp'."; 
	$CD ..; $RM -rf $name.tmp &> /dev/null

	# Change to $name directory #
	$CD $name;

	return $true;
}

# gen_create_doc_package($name,$version,$pkgarch): Create doc installable packages from last prefixed install #
gen_create_doc_package() {
	# Parameters
	name="$1";
	version="$2";
	bversion="$3";
	pkgarch="$4";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_create_doc_package($name,$version,$pkgarch)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create doc package ###
	start_spinner "Creating documentation package '${color_wht}$name-$version.$pkgarch.doc.gen${color_reset}'"

	# Change to '$GENETIC_TMP/$source_package-$source_version' directory #
	$CD $GENETIC_TMP/$source_package-$source_version;

	### Use installed files to create package '$name-$version.$pkgarch.doc.gen' ###
	echolog_debug "$DEBUG Use installed files to create documentation package '$name-$version.$pkgarch.doc.gen'.";

	# Create a temp directory #
	echolog_debug "$DEBUG Creating temporary directory '$name.tmp'. It's needed to install package files.";
	check_dir "$name.tmp" "$RM -rf $name.tmp";

	$MKDIR -p $name.tmp;

	### Write new '$package_name.doc' 'Info' file ###
	echolog_debug "$DEBUG Writing a new '$name $version' ($pkgarch) package 'Info' file ...";
	$ECHO "# Package <$name.doc> info.

original_name=$source_package
name=$name.doc
version=$version
package=$name-$version
depends=\"$depends\"
installbefore=
installafter=
replaces=$name.doc
" > $name.tmp/Info;
	### End '$package_name' 'Info' file ###

	# Copy necesary package files
	echolog_debug "$DEBUG Copying files: '$name.*', 'PkgArch' to '$name.tmp' directory."

	$CP -a  $name/${GEN_BUILD_VERSION_PREFIX}${bversion} \
	$name/$name.doc.files \
	$name/$name.doc.dirs \
	$name/$name.doc.links \
	$name/PkgArch $name.tmp/
	errorcheck $? "gen_create_doc_package";

	# Moving built files to new directory 
	echolog_debug "$DEBUG Moving '$name $version' documentation installed files to '$name.tmp' ...";

	# Create doc indexed directories #
	for docdir in $($CAT $name.tmp/$name.doc.dirs); do
		if test ! -z "$docdir"; then
			$MKDIR -pv $name.tmp/$ROOTFS/$docdir &>/dev/null
		fi;
	done;

	for docfile in $($CAT $name.tmp/$name.doc.files $name.tmp/$name.doc.links); do
		if test ! -z "$docfile"; then
			$MV $name/$ROOTFS/$docfile $name.tmp/$ROOTFS/$docfile &>/dev/null;
		fi;
	done;

	$CD $name.tmp/$ROOTFS;

	# Create tar package with root files inside gen file
	echolog_debug "$DEBUG Creating '../$ROOTFS.tmp' file from '$ROOTFS' directory ...";
	$TAR cvf ../$ROOTFS.tmp * &>../../$name/$ROOTFS.doc

	# Delete unnecesary files and $ROOTFS directory
	echolog_debug "$DEBUG Deleting unnecesary files and directories '$ROOTFS', 'Rules', 'files' and 'patches' ...";
	$CD ..;	$RM -rf $ROOTFS Rules files patches &>/dev/null

	echolog_debug "$DEBUG Create documentation package '$name-$version.$pkgarch.doc.gen'.";
	$TAR cfvj $GENETIC_PACKAGES/$name-$version.$pkgarch.doc.gen * &> ../$name-$version.$pkgarch.doc.content

	# End work_spinner #
	stop_spinner $?;

	# Delete temporary directory
	echolog_debug_verbose "$DEBUG Deleting temporary working directory '$name.tmp'."; 
	$CD ..; $RM -rf $name.tmp &> /dev/null

	# Change to $name directory #
	$CD $name;

	return $true;
}

# gen_create_debug_package($name,$version,$pkgarch): Create binary with debug symbols installable packages from last prefixed install #
gen_create_debug_package() {
	# Parameters
	name="$1";
	version="$2";
	bversion="$3";
	pkgarch="$4";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_create_debug_package($name,$version,$pkgarch)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create binary debug package ###
	start_spinner "Creating binary debug package '${color_wht}$name-$version.$pkgarch.dbg.gen${color_reset}'"

	# Change to '$GENETIC_TMP/$source_package-$source_version' directory #
	$CD $GENETIC_TMP/$source_package-$source_version;

	### Use installed files to create package '$name-$version.$pkgarch.gen' ###
	echolog_debug "$DEBUG Use installed files to create debug package '$name-$version.$pkgarch.dbg.gen'.";

	# Create a temp directory #
	echolog_debug "$DEBUG Creating temporary directory '$name.tmp'. It's needed to install package files.";
	check_dir "$name.tmp" "$RM -rf $name.tmp";

	$MKDIR -p $name.tmp;

	### Write new '$package_name.dbg' 'Info' file ###
	echolog_debug "$DEBUG Writing a new '$name $version' ($pkgarch) package 'Info' file ...";
	$ECHO "# Package <$name.dbg> info.

original_name=$source_package
name=$name.dbg
version=$version
package=$name-$version
depends=\"$depends\"
installbefore=
installafter=
replaces=$name.dbg
" > $name.tmp/Info;
	### End '$package_name' 'Info' file ###

	# Create scripts: PostInst PreInst PostRemv PreRemv #
	# genetic will fail if these files are not found #
	echolog_debug "$DEBUG Creating scripts: PostInst, PreInst, PostRemv, PreRemv.";
	for script_file in PostInst PreInst PostRemv PreRemv; do
		if test ! -f "$name.tmp/$script_file"; then
			$ECHO '#!/bin/bash' > $name.tmp/$script_file;
		fi;
	done

	# Copy necesary package files
	echolog_debug "$DEBUG Copying files: '$name.*', 'Post*', 'Pre*', 'PkgArch' to '$name.tmp' directory."

	$CP -a $name/${GEN_BUILD_VERSION_PREFIX}${bversion} \
		$name/$name.dbg.files \
		$name/$name.dbg.dirs \
		$name/$name.dbg.links \
		$name/PkgArch \
		$name.tmp/

	errorcheck $? "gen_create_debug_package";

	# Moving built files to new directory 
	echolog_debug "$DEBUG Moving '$name $version' debug installed files to '$name.tmp' ...";

	# Create dbg indexed directories #
	for dbgdir in $($CAT $name.tmp/$name.dbg.dirs); do
		if test ! -z "$dbgdir"; then
			$MKDIR -pv $name.tmp/$ROOTFS/$dbgdir &>/dev/null
		fi;
	done;

	for dbgfile in $($CAT $name.tmp/$name.dbg.files $name.tmp/$name.dbg.links); do
		if test ! -z "$dbgfile"; then
			$MV $name/$ROOTFS/$dbgfile $name.tmp/$ROOTFS/$dbgfile &>/dev/null;
		fi;
	done;

	$CD $name.tmp/$ROOTFS;

	# Create tar package with root files inside gen file
	echolog_debug "$DEBUG Creating '../$ROOTFS.tmp' file from '$ROOTFS' directory ...";
	$TAR cvf ../$ROOTFS.tmp * &>../../$name/$ROOTFS.dbg

	# Delete unnecesary files and $ROOTFS directory
	echolog_debug "$DEBUG Deleting unnecesary files and directories '$ROOTFS', 'Rules', 'files' and 'patches' ...";
	$CD ..;	$RM -rf $ROOTFS Rules files patches &>/dev/null

	echolog_debug "$DEBUG Create binary package '$name-$version.$pkgarch.dbg.gen'.";
	$TAR cfvj $GENETIC_PACKAGES/$name-$version.$pkgarch.dbg.gen * &>../$name-$version.$pkgarch.dbg.content

	# End work_spinner #
	stop_spinner $?;

	# Delete temporary directory
	echolog_debug_verbose "$DEBUG Deleting temporary working directory '$name.tmp'."; 
	$CD ..; $RM -rf $name.tmp &> /dev/null

	# Change to $name directory #
	$CD $name;

	return $true;
}

# gen_create_binary_package($name,$version,$pkgarch): Create binary installable packages from last prefixed install #
gen_create_binary_package() {
	# Parameters
	name="$1";
	version="$2";
	bversion="$3";
	pkgarch="$4";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_create_binary_package($name,$version,$pkgarch)'!";
	echolog_debug "$DEBUG =====================================================";

	### Create binary package ###
	start_spinner "Creating binary package '${color_wht}$name-$version.$pkgarch.gen${color_reset}'"

	# Change to '$GENETIC_TMP/$source_package-$source_version' directory #
	$CD $GENETIC_TMP/$source_package-$source_version;

	### Use installed files to create package '$name-$version.$pkgarch.gen' ###
	echolog_debug "$DEBUG Use installed files to create package '$name-$version.$pkgarch.gen'.";

	# Create a temp directory #
	echolog_debug "$DEBUG Creating temporary directory '$name.tmp'. It's needed to install package files.";
	check_dir "$name.tmp" "$RM -rf $name.tmp";

	$MKDIR -p $name.tmp;

	### Write new '$package_name' 'Info' file ###
	echolog_debug "$DEBUG Writing a new '$name $version' ($pkgarch) package 'Info' file ...";
	$ECHO "# Package <$name> info.

original_name=$source_package
name=$name
version=$version
package=$name-$version
depends=\"$depends\"
installbefore=
installafter=
replaces=$name
" >> $name.tmp/Info;
	### End '$package_name' 'Info' file ###

	# Create scripts: PostInst PreInst PostRemv PreRemv #
	# genetic will fail if these files are not found #
	echolog_debug "$DEBUG Creating scripts: PostInst, PreInst, PostRemv, PreRemv.";
	for script_file in PostInst PreInst PostRemv PreRemv; do
		if test ! -f "$name/$script_file"; then
			$ECHO '#!/bin/bash' > $name/$script_file;
		fi;
	done

	# Copy necesary package files
	echolog_debug "$DEBUG Copying files: 'Info', '$name.*', 'Post*', 'Pre*', 'PkgArch' to '$name.tmp' directory."
	$CP -a $name/${GEN_BUILD_VERSION_PREFIX}${bversion} \
		$name/$name.etc.files \
		$name/$name.etc.dirs \
		$name/$name.etc.links \
		$name/$name.files \
		$name/$name.dirs \
		$name/$name.links \
		$name/PostInst \
		$name/PreInst \
		$name/PostRemv \
		$name/PreRemv \
		$name/PkgArch \
		$name.tmp/
	
	errorcheck $? "$FUNCNAME";

	if [ "$DISABLE_GEN_DEBUG" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		$CP -a $name/$name.dbg.files \
			$name/$name.dbg.links \
			$name/$name.dbg.dirs \
			$name.tmp/
	fi

	errorcheck $? "$FUNCNAME -> Error Copying debug package file lists!";

	if [ "$DISABLE_GEN_LIBRARY" == "yes" ] || [ "$DISABLE_GEN_ALL" == "yes" ]; then
		$CP -a $name/$name.lib.files \
			$name/$name.lib.links \
			$name/$name.lib.dirs \
			$name.tmp/
	fi

	errorcheck $? "$FUNCNAME -> Error Copying library package file lists!";

	# Moving built files to new directory 
	echolog_debug "$DEBUG Moving '$name $version' installed files to '$name.tmp' ...";
	$MV $name/$ROOTFS $name.tmp/;
	$CD $name.tmp/$ROOTFS;
	
	# Preserve sysconfig files #
	#echolog_debug "$DEBUG Moving '$name $version' sysconfig files to '$name.tmp' ...";
	#check_dir "etc" "$MV etc ../";

	# Strip debug symbols from binaries and libraries #
	echologn_debug "$DEBUG Stripping debug symbols from '$name $version' programs and libraries ...";

	for dir in bin sbin usr/bin usr/sbin usr/local/bin usr/local/sbin opt/bin usr/libexec; do
		if test -d "$dir"; then
			$FIND $dir -type f -exec $STRIP --strip-all {} \; &>../stripbin.log
		fi;
	done;

	for dir in lib usr/lib usr/local/lib opt/lib; do
    if test -d "$dir"; then
			$FIND $dir -type f -exec $STRIP --strip-debug {} \; &>../striplib.log
		fi;
	done;

	echolog_debug_nodate "$FINISHED";

	# Delete devel files and directories
	echolog_debug "$DEBUG Delete devel files from '$name $version' programs and libraries.";
	check_dir "include" "$RM -rf include";
	check_dir "usr/include" "$RM -rf usr/include";
	check_dir "usr/local/include" "$RM -rf usr/local/include";

	# Create tar package with root files inside gen file
	echolog_debug "$DEBUG Creating '../$ROOTFS.tmp' file from '$ROOTFS' directory ...";
	$TAR cvf ../$ROOTFS.tmp * &>../../$name/$ROOTFS.bin

	# Delete unnecesary files and $ROOTFS directory
	echolog_debug "$DEBUG Deleting unnecesary files and directories '$ROOTFS', 'Rules', 'files' and 'patches' ...";
	$CD ..;	$RM -rf $ROOTFS Rules files patches &>/dev/null

	echolog_debug "$DEBUG Create binary package '$name-$version.$pkgarch.gen'.";
	$TAR cfvj $GENETIC_PACKAGES/$name-$version.$pkgarch.gen * &>../$name-$version.$pkgarch.content

	# End work_spinner #
	stop_spinner $?;

	# Delete temporary directory
	echolog_debug_verbose "$DEBUG Deleting temporary working directory '$name.tmp'."; 
	$CD ..; $RM -rf $name.tmp &> /dev/null

	# Change to $name directory #
	$CD $name;

	return $true;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
