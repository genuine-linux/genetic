#!/bin/bash - 
#===============================================================================
#
#          FILE: find_deps.sh
# 
#         USAGE: ./find_deps.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 30/07/20 01:30
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

check_file="$1"
all_deps=

real_file=$(which $check_file)

if file $real_file | grep -q dynamically; then
  printf " * Checking $check_file depends: "
  deps=$(ldd $real_file | awk '{print $1}' | sort | uniq)
  for dep in $deps; do
    genetic_packages=$(genetic -L $dep | grep -v Genuine | awk '{print $2}' | sort | uniq)
    for genetic_package in $genetic_packages; do
      if [ -f "/var/cache/genetic/$genetic_package/Info" ]; then
        genetic_package_version=$(grep ^version /var/cache/genetic/$genetic_package/Info | \
          awk -F'~' '{print $1}' | awk -F'=' '{print $2}')
        echo "$all_deps" | grep -q "${genetic_package}_${genetic_package_version}" || \
          all_deps="$all_deps ${genetic_package}_${genetic_package_version}"
      fi
    done
  done
  if [ ! -z "$all_deps" ]; then
    echo $all_deps | sort | uniq | awk -v ORS=" " '{print $0}'
    echo ""
  else
    echo "* No genetic depends found for $check_file!"
  fi
else
  echo " * ERROR! $check_file is not dynamically linked."
fi
