#!/bin/bash - 
#===============================================================================
#
#          FILE: SourcePoolCleaner.sh
# 
#         USAGE: ./SourcePoolCleaner.sh 
# 
#   DESCRIPTION: Genuine SourcePool Cleaner Assistant
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: ANTONIO CAO (@acaogon), 
#  ORGANIZATION: Genuine GNU/Linux
#       CREATED: 27/07/20 13:50
#      REVISION: 0
#===============================================================================

set -o nounset                              # Treat unset variables as an error

SourcePool=/var/cache/genetic/SourcePool

last_name="null"
last_version="null"

for srcgen in $(ls $SourcePool/*.src.gen | sort); do # Sort packages by name
  # *.src.gen package data
  srcgen_name=$(echo $srcgen | awk -F"/" '{print $NF}' | sed 's/\.src\.gen//g')
  version=$(echo $srcgen_name | awk -F"-" '{print $NF}')
  name=$(echo $srcgen_name | sed 's/-'$version'//g')

  if [ "$name" == "$last_name" ]; then
    if [ -f "${name}-${version}.src.gen" ] && [ -f "${last_name}-${last_version}.src.gen" ]; then
      echo "${name}-${version}.src.gen is the same package as ${last_name}-${last_version}.src.gen"
    fi
  fi

  # Remeber last package name
  last_name="$name"
  last_version="$version"
done

exit $?
