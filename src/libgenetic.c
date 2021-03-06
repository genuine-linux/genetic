#!/bin/bash

# @PACKAGE@-@VERSION@ (libgenetic) #

# Copyright (c) 2014-2020 Antonio Cao (@burzumishi) #

# This is free software;
# You have unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# libgenetic.la: Core functions and definitions #

### --- Define 'genetic' Core functions --- ###

# main($@): Start genetic #
main() {

  # Load 'genetic' core variables #
  load_core_variables;

  # Trap 'genetic' process 'SIGINT' 'SIGQUIT' 'SIGTERM' 'SIGKILL' will call manage_system_int() function and exit 'genetic' instance safely #
  trap_system_int;

  # Start 'genetic' process main loop #
  while : ; do
    # Read 'genetic' user keyboard input (wait for 'SIGINT', 'SIGKILL', 'SIGQUIT' & 'SIGTERM') #
    read user_keyboard_input &

    echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
    echolog_debug "$DEBUG Start trapping 'genetic' process user keyboard input.";
    echolog_debug "$DEBUG =====================================================";

    # Check genetic directory structure. These dirs are needed for genetic to work properly #
    check_directory_structure "$LOCKDIR $GENETIC_TMP $GENETIC_SOURCES $GENETIC_PACKAGES $CACHEDIR $TMPDIR";

    # Lock genetic instance #
    #lock_instance;

    # Check genetic Operating System #
    check_os;

    # Setup genetic Terminal #
    setup_term_environment;

    # Check current user (genetic needs superuser privileges) #
    check_user;

    # Debug genetic environment #
    environment_debug;

    ### Get and parse genetic Command Line Actions & Options ###

    # Empty Command Line parameter displays help #
    if test -z "$*"; then
      help;
    else
      # Define $ACTION_OPTIONS #
      ACTION_OPTIONS=$($GETOPT \
         -l "find-deps,help,version,verbose,silent,force,rebuildb,color,print-architecture,noarch,noscripts,autotools,purge,update,bootstrap,installer,packagepool-clean,disable-gen-orig,disable-gen-source,disable-gen-debug,disable-gen-library,disable-gen-all,disable-distclean,disable-clean,repo:,uninstall:,install:,list:,listfiles:,unpack:,unpackdir:,build,source:,clean,configure-self-dir,configure:,packages:,prefix:,admindir:,instdir:" \
         -n "$0" \
         -o FhIvVBbcCPdDpRr:u:i:l:L:U:s: \
         -- "$@")

      eval set -- "$ACTION_OPTIONS"

      # Case $ACTION_OPTIONS #
      while : ; do
        case "$1" in
          --admindir) # Set alternative directory to store 'genetic' data #
              CACHEDIR="$2"; shift 2;
          ;;
          --color) # Switch genetic output <color>, defined in config files #
            if test "$ENABLE_COLOR" == "yes"; then
              ENABLE_COLOR="no";
            else
              ENABLE_COLOR="yes";
            fi;
            load_terminal_color_support;
            shift;
          ;;
          -h|--help) # <help> genetic help #
            startmsg;
            help; shift; break;
          ;;
          -V|--version) # Show genetic version #
            show_version; shift; break;
          ;;
          -p|--print-architecture) # <verbose> genetic output #
            genetic_print_architecture; shift;
          ;;
          -v|--verbose) # <verbose> genetic output #
            ENABLE_VERBOSE="yes"; shift;
          ;;
          --silent) # <silent> genetic output #
            ENABLE_SILENT="yes"; shift;
          ;;
          --force) # <force> genetic action #
            ENABLE_FORCE="yes"; shift;
          ;;
          -r|--repo) # Gen new repository #
            startmsg;
            REPO_LOCAL_INDEX_FILE="$2"; shift 2;
            gen_repo "$REPO_LOCAL_INDEX_FILE";
          ;;
          --update) # <update> genetic index files #
            startmsg;
            genetic_update; shift;
          ;;
          -R|--rebuildb) # Rebuild intalled packages genetic database #
            startmsg;
            update_genetic_packages_db; shift;
          ;;
          --noarch) # Creates <noarch> package #
            GENETIC_ARCH="noarch"; shift;
          ;;
          --autotools) # Use GNU <Autotools> #
            ENABLE_AUTOTOOLS="yes"; shift;
          ;;
          -B|--bootstrap) # <bootstrap> Genuine System #
            startmsg;
            gen_bootstrap; shift; break;
          ;;
          -I|--installer) # <gen_installer> Install Genuine System #
            startmsg;
            gen_installer; shift; break;
          ;;
          --prefix) # Package configure & install <prefix> directory #
            GEN_PKGRULES_PREFIX="$2"; shift 2;
          ;;
          --configure) # <configure> options for <source> package #
            CONFIGURE_OPTIONS="$2"; shift 2;
          ;;
          --configure-self-dir) # <configure-self-dir> configure package in it's self directory
            CONFIGURE_SELF_DIR="yes"; shift;
          ;;
          --packages) # <packages> to generate from <source> package #
            PACKAGES_TO_GENERATE="$2"; shift 2;
          ;;
          -s|--source) # <source> genetic package creation from "sourcepackage.tarball" #
            startmsg;
            gen_source_package "$2"; shift 2;
          ;;
          --disable-gen-orig) # Disable package.orig.tar.bz2 creation while building #
            DISABLE_GEN_ORIG="yes"; shift;
          ;;
          --disable-gen-debug) # Disable package.arch.dbg.gen creation while building #
            DISABLE_GEN_DEBUG="yes"; shift;
          ;;
          --disable-gen-source) # Disable sourcepackage.src.gen creation while building #
            DISABLE_GEN_SOURCE="yes"; shift;
          ;;
          --disable-gen-library) # Disable sourcepackage.src.gen creation while building #
            DISABLE_GEN_LIBRARY="yes"; shift;
          ;;
          -d|--disable-gen-all) # Disable sourcepackage.src.gen creation while building #
            DISABLE_GEN_ORIG="yes";
            DISABLE_GEN_DEBUG="yes";
            DISABLE_GEN_SOURCE="yes";
            DISABLE_GEN_LIBRARY="yes";
            DISABLE_GEN_ALL="yes"; shift;
          ;;
          -D|--disable-distclean) # Disable distclean before build #
            DISABLE_DISTCLEAN="${DISABLE_DISTCLEAN:-yes}"; 
            shift;
          ;;
          -C|--disable-clean) # Disable distclean before build #
            DISABLE_CLEAN="${DISABLE_CLEAN:-yes}"; shift;
          ;;
          -c|--clean) # Clean previous genetic package builds #
            startmsg;
            DISABLE_CLEAN="${DISABLE_CLEAN:-no}";
            DISABLE_DISTCLEAN="${DISABLE_DISTCLEAN:-no}";
            gen_clean_package "SrcInfo"; shift;
          ;;
          -P|--packagepool-clean) # PackagePool Clean #
            startmsg;
            gen_clean_packagepool; shift;
          ;;
          -F|--find-deps)
            startmsg;
            gen_find_package_deps; shift;
          ;;
          -b|--build) # <build> genetic packages from "sourcepackage SrcInfo file" #
            startmsg;
            gen_build_package "SrcInfo"; shift;
          ;;
          --noscripts) # Don't execute PostInst scripts #
            ENABLE_NOSCRIPTS="yes"; shift;
          ;;
          --instdir) # Set alternative install to extract 'genetic' package <rootfs> data #
            INSTDIR="$2"; shift 2;
          ;;
          -i|--install) # <install> genetic packages from "package.arch|src|dev|dbg.gen" #
            # Lock genetic instance #
            lock_instance;
            startmsg;

            INSTALL_PACKAGE="$2";
            shift 2;

            if test "$1" == "--"; then
                    shift;
                    gen_install_packages $INSTALL_PACKAGE "$@";
            else
                    gen_install_packages $INSTALL_PACKAGE;
            fi

            exit_instance $?;
          ;;
          --purge) # Purge configuration files (/etc) #
            ENABLE_PURGE="yes"; shift;
          ;;
          -u|--uninstall) # <uninstall> genetic packages from "package.arch|dev|dbg.gen" #
            # Lock genetic instance #
            lock_instance;
            startmsg;

            UNINSTALL_PACKAGE="$2";
            shift 2;

            if test "$1" == "--"; then
                    shift;
                    gen_uninstall_packages $UNINSTALL_PACKAGE "$@";
            else
                    gen_uninstall_packages $UNINSTALL_PACKAGE;
            fi
          
            exit_instance $?;
          ;;
          -l|--list) # <list> genetic installed packages from database #
            startmsg;
            list_installed_packages "$2"; shift 2;
          ;;
          -L|--listfiles) # <listfiles> genetic installed files from database #
            startmsg;
            list_installed_files "$2"; shift 2;
          ;;
          --unpackdir) # <unpack> genetic package into <unpackdir> instead of <$TMPDIR> #
            UNPACK_PATH="$2"; shift 2;
          ;;
          -U|--unpack) # <unpack> genetic packages from "package.arch|src|dev|dbg.gen" #
            startmsg;
            gen_unpack_package "$2"; shift 2;
          ;;
           --) shift; break;
          ;;
          -*) # Unknown option (show 'genetic' help) #
            startmsg;
            help; shift; break;
          ;;
          *) # Unknown option (show 'genetic' help) #
            startmsg;
            help; shift; break;
          ;;
        esac;
      done;
    fi;

    # Close genetic instance #
    exit_instance $?;
  done;

  # End '@PACKAGE' program main loop #
}

### -- 'genetic' OS funtions ###

# trap_system_int(): Trap 'genetic' process 'SIGINT' 'SIGQUIT' 'SIGTERM' 'SIGKILL' will call manage_system_int() function and exit 'genetic' instance safely #
trap_system_int() {
  $TRAP "manage_system_int" SIGINT SIGQUIT SIGTERM SIGKILL
}

# manage_system_int(): Manage 'genetic' process trapped system interruptions #
manage_system_int() {
  # Manage 'genetic' process system INTs #
  echolog "$ERROR Error! Process '${color_wht}genetic${color_reset}' (${color_wht}PID: ${color_red}$$${color_reset}) received an interrupt!";
  echolog "$ERROR Closing '${color_wht}genetic${color_reset}' now! Goodbye!";

  echolog_debug "$DEBUG ================= %%% genetic %%% ================";
  echolog_debug "$DEBUG Error! Process (PID: $$) received an interrupt!";
  echolog_debug "$DEBUG ==================================================";

  # Close genetic instance #
  exit_instance $false;
}

# lock_instance(): Lock genetic instance #
lock_instance() {
  # Create $LOCKDIR if not found #
  if test ! -d "$LOCKDIR"; then $MKDIR -p $LOCKDIR &>/dev/null; fi;

  # Check 'genetic' instance locks #
  if test ! -f "$LOCKFILE"; then
    # 'genetic' $LOCKFILE not found #
    echolog_debug "$DEBUG Process 'genetic' (PID: $$) unlocked, creating '$LOCKFILE'!";

    # Create 'genetic' $LOCKFILE #
    $TOUCH $LOCKFILE;

    echolog_debug "$DEBUG Process 'genetic' (PID: $$) locked!";

    return $true;
  else
    # 'genetic' $LOCKFILE found #
    echolog_debug "$DEBUG Process 'genetic' '$LOCKFILE' found!"
    echolog "$WARNING Warning! Process '${color_wht}genetic${color_reset}' '${color_wht}$LOCKFILE${color_reset}' found!"

    instance_PID=$($PIDOF genetic);

    if test -z "$instance_PID"; then
      echolog_debug "$DEBUG Error! A 'genetic' process instance (PID: $instance_PID) is already running!";

      echolog "$ERROR Error! A '${color_wht}genetic${color_reset}' process instance (${color_wht}PID: ${color_wht}$instance_PID${color_reset}) is already running!";

      # Close 'genetic' instance #
      exit_instance $false;
    fi;
  fi;

  return $true;
}

# unlock_instance(): Unlock genetic instance #
unlock_instance() {
  # Create $LOCKDIR if not found #
  if test ! -d "$LOCKDIR"; then $MKDIR -p $LOCKDIR &>/dev/null; fi;

  if test -f "$LOCKFILE"; then
    # 'genetic' $LOCKFILE found #
    echolog_debug "$DEBUG Process 'genetic' (PID: $$) locked, deleting '$LOCKFILE'!";

    # Delete 'genetic' $LOCKFILE #
    $RM -f $LOCKFILE;

    echolog_debug "$DEBUG Process 'genetic' (PID: $$) unlocked!";
  else
    # 'genetic' $LOCKFILE not found #
    echolog_debug "$DEBUG Process 'genetic' (PID: $$) not locked!";
  fi;
}

# exit_instance($exit_val): Close genetic instance #
exit_instance() {
  # Params: $exit_val #
  exit_val=$1;

  # Unlock 'genetic' instance #
  unlock_instance;

  # Reset terminal environment #
  reset_term_environment;

  if test -z "$!"; then
    echolog_debug "$DEBUG Process 'genetic' function (exit_instance) detected subprocess (PID: $!)!"
  else
    echolog_debug "$DEBUG No 'genetic' subprocess (PID) detected!"
  fi

  if test $exit_val -ne 0; then
    echolog "$ERROR Error! Process '${color_wht}genetic${color_reset}' (${color_wht}PID: ${color_red}$$${color_reset}) terminated (${color_wht}Err: ${color_red}$exit_val${color_reset}) check errors!";
    exit $exit_val ;
  else
    echolog_debug "$DEBUG Process 'genetic' (PID: $$) terminated (Err: $exit_val) Goodbye!";
    exit $true;
  fi;
}

# check_os(): Check Operating System (GNU/Linux | BSD | CygWin). #
# NOTE: If you plan to port genetic to other OS different than Linux, you will need to modify this function. #
check_os() {
  case "$HOST_SYSTEM" in
    Linux|LINUX|linux) HOST_SYSTEM="linux";
      return $true;
    ;;

    *BSD|*bsd) HOST_SYSTEM="bsd";
      return $true;
    ;;

    cygwin*|CygWin*|CYGWIN*|Cygwin*) HOST_SYSTEM="cygwin";
      return $true;
    ;;

    *) # '$HOST_SYSTEM' not supported #
      echolog "$ERROR Error! '${color_wht}genetic${color_reset}' cannot run on '${color_red}$HOST_SYSTEM${color_reset}'!";

      # Close genetic instance #
      exit_instance $false;
    ;;
  esac;

  echolog_debug_verbose "$DEBUG 'genetic' running on '$HOST_SYSTEM'!";
  return true;
}

# setup_term_environment(): Setting up terminal environment #
setup_term_environment() {
  if test "$HOST_SYSTEM" != "cygwin"; then
    echolog_debug_verbose "$DEBUG Setting up ($HOST_OS) terminal environment.";

    # Terminal Cursor off
    $SETTERM -cursor off;
  else
    echolog_debug_verbose "$DEBUG Warning! Running on '$HOST_SYSTEM' can't use '$SETTERM'!";
  fi;

  return $true;
}

# reset_term_environment(): Resetting up terminal environment #
reset_term_environment() {
  if test "$HOST_SYSTEM" != "cygwin"; then
    echolog_debug_verbose "$DEBUG Resetting up ($HOST_OS) terminal environment.";

    # Terminal Cursor on
    $SETTERM -cursor on;

    # Terminal Default Settings
    $SETTERM -default;

    # unset CFLAGS & CXXFLAGS #
    unset CFLAGS CXXFLAGS;
  else
    echolog_debug_verbose "$DEBUG Warning! Running on '$HOST_SYSTEM' can't use '$SETTERM'!";
  fi;

  return $true;
}

# check_user(): check superuser #
check_user() {
  if test "$HOST_SYSTEM" != "cygwin"; then
    if test "$USER" != "root"; then
      $ECHO;
      echolog "$WARNING Warning! You are not superuser (${color_red}$USER${color_reset})!";
      echolog "$ERROR Error! That's not good! '${color_wht}genetic${color_reset}' needs to be run as (${color_wht}root${color_reset})!";
      $ECHO;

      # Close genetic instance #
      exit_instance $false;
    else
      echolog_debug_verbose "$DEBUG Good! Process 'genetic' with (PID: $$) is running as superuser ($USER)."
    fi;
  else
    echolog_debug_verbose "$DEBUG Warning! Running on '$HOST_SYSTEM' can't be 'root'!";
  fi;

  return $true;
}

# errorcheck($error, $function): check for errors, if $false will exit (1) #
errorcheck() {
  # Params: $error, $function #
  error=$1;
  function=$2;

  if [[ $error -ne $true ]]; then
    # Check if $function is defined else define 'unknown' #
    if test -z "$function"; then function="unknown"; fi;

      echolog_debug "$ERROR Error! 'genetic' function ($function) exited (Err: $error)!";
      echolog "$ERROR Error! '${color_wht}genetic${color_reset}' function (${color_red}$function${color_reset}) exited (${color_wht}Err: ${color_red}$error${color_reset})!";

    if test "$ENABLE_FORCE" == "yes"; then
      echolog_debug "$ERROR Error! Override error because user is using '-f' or '--force' enabled option!";
      echolog "$ERROR Error! Override error because user is using '${color_wht}-f${color_reset}' or '${color_wht}--force${color_reset}' enabled option!";

      return $true;
    else
      # Close genetic instance #
      exit_instance $false;
    fi;
  else
    return $true;
  fi;
}

# check_file($FILE,$FILECMD): check if $FILE exists, if $true execute $FILECMD if requested #
check_file() {
  # Params: $FILE $FILECMD #
  FILE="$1";
  FILECMD="$2"; # (optional) #

  # Test $FILE #
  if test ! -f "$FILE"; then
    echolog_debug "$DEBUG Warning! Requested file '$FILE' was not found!";

    # Define $GENETIC_LAST_ERROR to be displayed in (errorcheck) #
    GENETIC_LAST_ERROR="$WARNING Warning! Requested file '$FILE' was not found!";

    return $false;
  else
    echolog_debug "$DEBUG Requested file '$FILE' exists.";

    # If $FILECMD is passed, then execute $FILECMD #
    if test ! -z "$FILECMD"; then
      echolog_debug "$DEBUG Function (check_file) executing: '$FILECMD'";
      $FILECMD &>/dev/null;
    fi;

    return $true;
  fi;
}

# check_dir($DIR,$DIRCMD): check if $DIR exists, if $true execute $DIRCMD if requested #
check_dir() {
  # Params: $DIR $DIRCMD #
  DIR="$1";
  DIRCMD="$2"; # (optional) #

  # Test $DIR #
        if test ! -d "$DIR"; then
                echolog_debug "$DEBUG Warning! Requested directory '$DIR' was not found!";

    # Define $GENETIC_LAST_ERROR to be displayed in (errorcheck) #
    GENETIC_LAST_ERROR="$WARNING Warning! Requested directory '$DIR' was not found!";

    return $false;
  else
    echolog_debug "$DEBUG Requested directory '$DIR' exists.";

    # If $DIRCMD is passed, then execute $DIRCMD #
    if test ! -z "$DIRCMD"; then
      echolog_debug "$DEBUG Function (check_dir) executing: '$DIRCMD'";
      $DIRCMD &>/dev/null;
    fi;
    return $true;
        fi;
}

# check_link($LINK,$LINKCMD): check if file $LINK exists, if $true execute $LINKCMD if requested #
check_link() {
  # Params: $LINK $LINKCMD #
  LINK="$1";
  LINKCMD="$2"; # (optional) #

  # Test $LINK #
  if test ! -L "$LINK"; then
    echolog_debug "$DEBUG Warning! Requested file link '$LINK' was not found!";

    # Define $GENETIC_LAST_ERROR to be displayed in (errorcheck) #
    GENETIC_LAST_ERROR="$WARNING Warning! Requested file link '$LINK' was not found!";

    return $false;
  else
    echolog_debug "$DEBUG Requested file link '$LINK' exists.";

    # If $LINKCMD is passed, then execute $FILECMD #
    if test ! -z "$LINKCMD"; then
      echolog_debug "$DEBUG Function (check_link) executing: '$LINKCMD'";
      $LINKCMD &>/dev/null;
    fi;

    return $true;
  fi;
}

# check_directory_structure($directories): check and create directory #
check_directory_structure() {
  # Params: $directories #
  directories="$1";

  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG Starting 'check_directory_structure($directories)'!";
  echolog_debug "$DEBUG =====================================================";

  # Create required '@PACKAGE' directories #
  for dir in $directories; do
    # Create $dir if not found #
    if test ! -d "$dir"; then
      echolog_debug "$DEBUG Warning! Requested directory '$dir' was not found!";
      
      $MKDIR -p $dir &>/dev/null;

      echolog_debug_verbose "$DEBUG Warning! Requested directory '$dir' has been created!";
    fi;
  done;
  
  return $true;
}

# new_work_spinner($SPINNER_CMD,$SPINNER_MSG|$SPINNER_EXIT_STATUS,$new_work_spinner_pid): Working spinner #
new_work_spinner() {
  # $1 start/stop
  #
  # on start: $2 display message
  # on stop : $2 process exit status
  #           $3 spinner function pid (supplied from stop_spinner)

  case $1 in
    start)
      new_spinner_count=1;
      SPINNER_DELAY=0.15;

      # Display SPINNER #
      echolog_debug "$DEBUG ================ %%% 'genetic' work_spinner %%% ===============";
      echolog_debug "$DEBUG Start: '$SPINNER_MSG'.";

      while :
      do
        case $new_spinner_count in
          1) $ECHO -ne "\\r$SPIN1 $SPINNER_MSG     ";
              new_spinner_count=$((new_spinner_count+1));
          ;;
          2) $ECHO -ne "\\r$SPIN2 $SPINNER_MSG .   ";
             new_spinner_count=$((new_spinner_count+1));
          ;;
          3) $ECHO -ne "\\r$SPIN3 $SPINNER_MSG ..  ";
             new_spinner_count=$((new_spinner_count+1));
          ;;
          4) $ECHO -ne "\\r$SPIN4 $SPINNER_MSG ... ";
             new_spinner_count=1;
          ;;
        esac;

        $SLEEP $SPINNER_DELAY;
      done

      return $true;
    ;;
    stop)
      if test -z $new_work_spinner_pid; then
        echolog_debug "$DEBUG Stop: was requested, but it's not running!";
        return $true;
      else
        echologn_debug "$DEBUG Stop: (PID: $new_work_spinner_pid) stopping ...";
        $KILL $new_work_spinner_pid &> /dev/null;

        # inform the user uppon success or failure
        if test $SPINNER_EXIT_STATUS -eq 0; then
          echolog_debug_nodate "$FINISHED";
          echolog_debug "$DEBUG =====================================================";

          $ECHO -e "$FINISHED";
          return $true;
        else
          echolog_debug_nodate "$FAILED";
          echolog_debug "$DEBUG =====================================================";

          $ECHO -e "$FAILED";
          exit_instance $false;
        fi;
      fi;
    ;;
    *)  echolog_debug "$DEBUG Called 'new_work_spinner' with an invalid argument, try {start_spinner|stop_spinner}!";
        exit_instance $false;
    ;;
  esac;

  return $true;
}

# new_work_spinner($SPINNER_MSG): Starts working spinner #
start_spinner() {
  SPINNER_MSG="$1";

  # Load spinner #
  new_work_spinner "start" "$SPINNER_MSG" &

  # set global work_spinner pid #
  new_work_spinner_pid=$!;

  disown;

  # Prevent running actions before spinner #
  sleep 0.5;
}

# new_work_spinner($SPINNER_EXIT_STATUS): Stops working spinner #
stop_spinner() {
  SPINNER_EXIT_STATUS=$1;

  # Prevent finishing actions before spinner #
  sleep 0.5;

  # If force is used set SPINNER_EXIT_STATUS to $true #
  if test "$ENABLE_FORCE" == "yes"; then
    SPINNER_EXIT_STATUS=$true;
  fi;

  # Load spinner #
  new_work_spinner "stop" $SPINNER_EXIT_STATUS $new_work_spinner_pid;

  # unset global work_spinner vars #
  unset new_work_spinner_pid;
  unset SPINNER_EXIT_STATUS;
  unset SPINNER_MSG;
}


### -- 'genetic' screen display and input functions -- ###
 
# continue_prompt: Ask user to continue or not #
continue_prompt() {
  echolog_debug "$DEBUG [ Continue? (y/n) ] Display user input prompt and wait for an answer!";

  # Display Continue Question #
  echolog "$QUESTION Do you want to continue? (${color_wht}y/n${color_reset}): ";

  # Read user input (y/n) #
  read input
  case $input in
       [Yy]*) ;;                      # "Yes", let's go! Continue! #
       [Nn]*) exit_instance $false ;; # "No" Call (exit_instance) and exit '@PACKAGE' now! #
       *) echolog "$QUESTION Do you want to continue? (${color_wht}y/n${color_reset}): " ;; # "Retry!" (y/n) Retry again!? #
  esac;

  return $true;
}

# show_version(): display 'genetic' version #
show_version() {
  $ECHO "$GENETIC_RELEASE";
}

# genetic_print_architecture(): display genetic arch $HOST_MACHINE
genetic_print_architecture() {
  $ECHO "$HOST_MACHINE";
}

# startmsg(): display start message to user #
startmsg() {

  # Debug always displays $starmsg #
  echolog_debug "$DEBUG ================== %%% genetic %%% ==================";
  echolog_debug "$DEBUG $genetic_VERSION";
  echolog_debug "$DEBUG =====================================================";

  if test "$ENABLE_STARTMSG" == "yes"; then
    # Display 'genetic' start message '$genetic_VERSION' #
    echolog_debug "$DEBUG Display 'genetic' start message.";
    $ECHO "$genetic_VERSION";
  else
    # Display 'genetic' start message '$genetic_VERSION' is disabled #
    echolog_debug "$DEBUG Display 'genetic' start message is disabled!";
  fi;

  # Warn user about --force or -f #
  if test "$ENABLE_FORCE" == "yes"; then
    echolog "$WARNING! Warning! Running '${color_wht}genetic${color_reset}' with '${color_wht}-f${color_reset}' or '${color_wht}--force${color_reset}' option enabled!!!"
    echolog "$WARNING! Warning! Forcing all '${color_wht}errorcheck${color_reset}' and '${color_wht}spinner${color_reset}' functions to end always with an exit '${color_wht}$true${color_reset}' status!";
  fi;

  return $true;
}

# help(): command line help #
help() {
  $ECHO;
  $ECHO "$HELP";
  $ECHO;

  # Close genetic instance #
  exit_instance $true;
}

# vi: syntax=bash ts=2 sw=2 sts=2 sr noet expandtab
# vi: filetype=bash
