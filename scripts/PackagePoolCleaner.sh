#!/bin/bash - 
#===============================================================================
#
#          FILE: PackagePoolCleaner.sh
# 
#         USAGE: ./PackagePoolCleaner.sh 
# 
#   DESCRIPTION: Genuine PackagePool Cleaner Assistant
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

PackagePool=/var/cache/genetic/PackagePool
OldPackagePool=/var/cache/genetic/OldPackagePool

for gen in $(ls $PackagePool/*.gen | awk -F"/" '{print $NF}' | sed 's/\.gen//g' | sort); do # Sort packages by name
  # Check package type
#  case gen in
#    *.dbg$) gen_type=".dbg.gen"
#      gen_full_name=$(echo "$gen" | sed 's/\.dbg\.gen//g')
#    ;;
#    *.doc$) gen_type=".doc.gen"
#      gen_full_name=$(echo "$gen" | sed 's/\.doc\.gen//g')
#    ;;
#    *.dev$) gen_type=".dev.gen"
#      gen_full_name=$(echo "$gen" | sed 's/\.dev\.gen//g')
#    ;;
#    *) gen_type=".gen"
#      gen_full_name=$gen
#    ;;
#  esac

  for pkg in "doc,dbg,dev"; do
    if echo $gen | grep -q ".${pkg}$"; then
     gen_full_name=$(echo $gen | sed 's/\.'$pkg'//g')
     gen_type=".${pkg}.gen"
    else
      gen_full_name=$gen
      gen_type=".gen"
    fi
  done

  # Get package info
  gen_arch=$(echo "$gen_full_name" | awk -F"." '{print $NF}')
  gen_build=$(echo "$gen_full_name" | sed 's/\.'$gen_arch'//g' | awk -F"~" '{print $NF}')
  version=$(echo "$gen_full_name" | awk -F"-" '{print $NF}' | sed 's/\.'$gen_arch'//g' | sed 's/~genuine~'$gen_build'//g')
  name=$(echo "$gen_full_name" | sed 's/\.'$gen_arch'//g' | sed 's/~genuine~'$gen_build'//g' | sed 's/-'$version'//g')

  # Check for package old packages
  package_builds=$(ls $PackagePool/${name}-[0-9]*~genuine~[0-9]*.${gen_arch}${gen_type} --sort=version -l | awk -F"/" '{print $NF}')
  last_version=$(ls $PackagePool/${name}-[0-9]*~genuine~[0-9]*.${gen_arch}${gen_type} --sort=version -l | awk -F"/" '{print $NF}' | tail -1)

  if [ "$package_builds" != "$last_version" ]; then
    if [ "$last_version" != "$gen" ]; then
      echo "Checking builds for package: $name $version $gen_build $gen_arch $gen_type"
       
      echo "Package $name last version: $last_version"
        
      old_versions=$(ls $PackagePool/${name}-[0-9]*~genuine~[0-9]*.${gen_arch}${gen_type} --sort=version -l | awk -F"/" '{print $NF}' | grep -v $last_version)
      echo "Package $name old versions: $old_versions"
      echo ""

      for old_version in $old_versions; do
        if [ -f "$PackagePool/$old_version" ]; then
          echo -n "Moving old package version $old_version to $OldPackagePool ..."
          mv $PackagePool/$old_version $OldPackagePool/
          echo " OK!"
          echo ""
        fi
      done
    fi
  fi
done

exit $?
