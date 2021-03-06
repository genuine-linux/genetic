#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic-repo) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# genetic_update(): Update All Repositories #
function genetic_update() {
	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'get_repo_index($repo_index_file)'!";
	echolog_debug "$DEBUG =====================================================";

  echolog "$INFO Fetching package repositories '${color_wht}$REPO_INDEX_FILE${color_reset}' files.";

	# Read genetic repository files to get package index
	read_repo_files;
	
	# If we don't have remote repositories don't go ahead
	if [ "$INDEX_URLS" != "" ]; then
		echolog_debug "$DEBUG Found INDEX URLS: $INDEX_URLS";
	  get_repo_index "$INDEX_URLS";
	else
		echolog_debug "$DEBUG INDEX URLS NOT FOUND! Can't download package repository INDEX file!"
	fi
	
	genetic_update_index_cache "$REPO_INDEX_CACHE";
	
	return $?;
}

# genetic_update_index_cache(): Update Packages INDEX.cache #
function genetic_update_index_cache() {
	repo_index_cache="$1";
	
	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'get_repo_index($repo_index_cache)'!";
	echolog_debug "$DEBUG =====================================================";

	start_spinner "Updating package repositories '${color_wht}${repo_index_cache}${color_reset}'"

	# Temporary $REPO_INDEX_CACHE file
	REPO_INDEX_CACHE_TMP="${repo_index_cache}.tmp";

	$TOUCH $repo_index_cache;
	$TOUCH $REPO_INDEX_CACHE_TMP;
	
	# Read packages from all INDEX files
	echolog_debug "$DEBUG Get list of all INDEX files in $REPO_CACHE_DIR";
	
	ALL_INDEX_FILES=$($FIND $REPO_CACHE_DIR -type f -name "INDEX*" | grep -v "$repo_index_cache");
	
	if [ "$ALL_INDEX_FILES" != "" ]; then
		for index_file in $ALL_INDEX_FILES; do
			echolog_debug "$DEBUG Reading INDEX: $index_file";

			# Check INDEX host
			if [ "$index_file" == "$REPO_CACHE_DIR/$REPO_INDEX_FILE" ]; then
				REPO_HOST="__localfile__";
			else
				REPO_HOST="$($ECHO $index_file | $AWK -F"/" '{print $NF}' | $SED 's/'$REPO_INDEX_FILE'\.//g')";
			fi;

			echolog_debug "$DEBUG Repository host is: $REPO_HOST";

			# Read packages from all INDEX files
			echolog_debug "$DEBUG Starting to read packages from INDEX files";
			while IFS= read -r package; do
				package_name=$($ECHO "$package" | $AWK '{print $3}');
				package_version=$($ECHO "$package" | $AWK '{print $4}');
				package_build=$($ECHO "$package" | $AWK '{print $5}');
				package_type=$($ECHO "$package" | $AWK '{print $NF}');

				echolog_debug "$DEBUG Temporary cached package: $package_name $package_version ~genetic$package_build $package_type from $REPO_HOST";
				$ECHO "$package $REPO_HOST" >> $REPO_INDEX_CACHE_TMP;
			done < "$index_file";
		done

		echolog_debug "$DEBUG Writing $repo_index_cache";
		
		$MV $REPO_INDEX_CACHE_TMP $repo_index_cache;
	else
		echolog_debug "$DEBUG No INDEX files found!";
	fi
	
	stop_spinner $?;

	return $?;
}

# get_repo_index(): Get Repository Index file #
function get_repo_index() {
	# Parameters: $repo_index_file #
	INDEX_URLS="$1";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'get_repo_index($repo_index_file)'!";
	echolog_debug "$DEBUG =====================================================";

  echolog_debug "$DEBUG Get packages $REPO_INDEX_FILE from remote.";

	for index_url in $INDEX_URLS; do
		index_host=$($ECHO "$index_url" | $SED 's/http\:\/\///g' | $SED 's/https\:\/\///g');

		echolog "$INFO Trying to download packages '${color_wht}$REPO_INDEX_FILE${color_reset}' from: '${color_wht}$index_host${color_reset}'";

		echolog_debug "$DEBUG $REPO_INDEX_FILE download host is: $index_host";

    if ping -c 1 -w 5 -q $index_host &>/dev/null; then
      start_spinner "Downloading packages '${color_wht}${REPO_INDEX_FILE}${color_reset}' from: '${color_wht}$index_host${color_reset}'"
		
  		REPO_REMOTE_INDEX_FILE_URL="$index_url/Repositories/${REPO_INDEX_FILE}";
  		REPO_REMOTE_INDEX_FILE="$REPO_CACHE_DIR/${REPO_INDEX_FILE}.${index_host}";

  		echolog_debug "$DEBUG Downloading packages $REPO_INDEX_FILE from $REPO_REMOTE_INDEX_FILE_URL to $REPO_REMOTE_INDEX_FILE";		

  		$WGET $REPO_REMOTE_INDEX_FILE_URL -O $REPO_REMOTE_INDEX_FILE;

      stop_spinner $?;
    else
      echolog "$WARNING Repository host '${color_wht}$index_host${color_reset}' is not available!"
    fi
	done

	return $?;
}

# list_repo_files(): List all repofiles #
function list_repo_files() {
	# Parameters: $repo_files_dir #
	repo_files_dir="$1";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'list_repo_files($repo_files_dir)'!";
	echolog_debug "$DEBUG =====================================================";

}

# read_repo_files(): Read all repofiles from listed repos #
function read_repo_files() {
	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'read_repo_files()'!";
	echolog_debug "$DEBUG =====================================================";

  start_spinner "Reading package '${color_wht}repository${color_reset}' files";
	
  echolog_debug "$DEBUG Reading package repository files to get INDEX URLS";
	
	# Read INDEX remote files URLS
  INDEX_URLS=$($FIND $REPOSDIR -name "*.list" -exec $CAT {} \; | $GREP -v "^#" | $AWK '{print $2}');
 
  stop_spinner $?;

	test -n "$INDEX_URLS"
  errorcheck $? "$FUNCNAME: no *.list found at $REPOSDIR"

  return $?;
}

# gen_scan_repo_packages($repo): Scan packages that will be added to the repository
function gen_scan_repo_packages() {
	repo_local_dir="$1"; 
	repo_local_index_file="$2";
	
	cd $repo_local_dir;

	start_spinner "Scanning repository packages '${color_wht}${repo_local_dir}${color_reset}'";

	for package in $($LS *.gen); do
		for package_arch in x86_64 i686 i386 arm64; do
			case $package in
				*.${package_arch}.gen) full_package_name=$($ECHO $package | $AWK -F\"/\" '{print $NF}' | $SED 's/\.gen//g');
				package_type="binary";
        gen_type=".gen";
				;;
				*.${package_arch}.dev.gen) full_package_name=$($ECHO $package | $AWK -F\"/\" '{print $NF}' | $SED 's/\.dev\.gen//g');
				package_type="devel";
        gen_type=".dev.gen";
				;;
				*.${package_arch}.doc.gen) full_package_name=$($ECHO $package | $AWK -F\"/\" '{print $NF}' | $SED 's/\.doc\.gen//g');
				package_type="documentation";
        gen_type=".doc.gen";
				;;
				*.${package_arch}.dbg.gen) full_package_name=$($ECHO $package | $AWK -F\"/\" '{print $NF}' | $SED 's/\.dbg\.gen//g');
				package_type="debug";
        gen_type=".dbg.gen";
				;;
			esac
		done
    
    # Get package info
    genetic_package_md5sum=$($MD5SUM $package | $AWK '{print $1}');
		genetic_package_arch=$($ECHO $full_package_name | $AWK -F"." '{print $NF}');
		genetic_package_name=$($ECHO $full_package_name | $SED 's/\.'$genetic_package_arch'//g');
		genetic_package_version=$($ECHO $genetic_package_name | $AWK -F"-" '{print $NF}');
		package_build_version=$($ECHO $genetic_package_version | $AWK -F"~" '{print $NF}');
		package_version=$($ECHO $genetic_package_version | $SED 's/~genuine~'$package_build_version'//g');
		package_name=$($ECHO $genetic_package_name | $SED 's/\-'$genetic_package_version'//g');

    # Check if package was previously added to repository index #
    check_repo_for_package=$($GREP $package_name $repo_local_index_file | $GREP $package_type);

    if test -z "$check_repo_for_package"; then
      last_version=$($LS ${package_name}-[0-9]*~genuine~[0-9]*.${genetic_package_arch}${gen_type} --sort=version -l | $AWK '{print $NF}' | $TAIL -1)

      if [ "$last_version" == "$package" ]; then
        echolog_debug "$DEBUG Checking builds for package: $package_name $package_version $package_build_version $genetic_package_arch $gen_type"
        echolog_debug "$DEBUG Package $package_name last version: $last_version"
				echologn_debug "$DEBUG Adding new package to repository index: $package_name $package_version ~genuine~$package_build_version $genetic_package_arch $gen_type $package_type ... ";
		    $ECHO "$genetic_package_md5sum $package $package_name $package_version ~genuine~$package_build_version $genetic_package_arch $gen_type $package_type" >> $repo_local_index_file;
		    echolog_debug "OK!";
      fi
    fi;
	done
	
	stop_spinner $?;

	errorcheck $? "gen_repo -> gen_scan_repo_packages";

	echolog "$INFO Repository '${color_wht}$repo_local_index_file${color_reset}' created!";

}

# gen_repo(): Gen new repository from local directory #
function gen_repo() {
	# Parameters: $REPO_LOCAL_DIR #
	REPO_LOCAL_DIR="$1";

	echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
	echolog_debug "$DEBUG Starting 'gen_repo($REPO_LOCAL_DIR)'!";
	echolog_debug "$DEBUG =====================================================";
	
	check_dir "$REPO_LOCAL_DIR";

	$MKDIR -p $REPO_CACHE_DIR;

	REPO_LOCAL_INDEX_FILE="$REPO_CACHE_DIR/$REPO_INDEX_FILE";

	> "$REPO_LOCAL_INDEX_FILE";
	
	gen_scan_repo_packages "$REPO_LOCAL_DIR" "$REPO_LOCAL_INDEX_FILE";

	# Exit genetic instance
	exit_instance $?;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet 
# vi: filetype=bash
