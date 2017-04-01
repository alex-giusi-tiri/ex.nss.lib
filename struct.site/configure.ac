#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libnss-exo], [0.1])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([../src/tool.h])
AC_CONFIG_HEADER([config.h])
AC_PREFIX_DEFAULT([])


AC_ARG_ENABLE(debug, 
    AC_HELP_STRING([--enable-debug],
            [Enable debug statements using syslog]),
    AC_DEFINE([DEBUG], [], [Enable debugging])
    CFLAGS="$CFLAGS -g")

#AC_DEFINE_UNQUOTED(MODULE_NAME, "$MODULE_NAME", [The name of the NSS and PAM modules.])
#AC_DEFINE_UNQUOTED(NSS_NAME(NAME), [_nss_${MODULE_NAME}_##NAME], [Expand to NSS symbol name.])
#AC_DEFINE_UNQUOTED(PAM_NAME(NAME), [_pam_${MODULE_NAME}_##NAME], [Expand to PAM symbol name.])

#CFLAGS="$CFLAGS $(xml2-config --cflags) $(xml2-config --libs)"
CFLAGS="$CFLAGS -lczmq -lzmq"

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
#AC_CHECK_LIB([sqlite3], [sqlite3_open])
#AC_CHECK_LIB([czmq], [zmq])

# Checks for header files.
AC_HEADER_STDC
#AC_CHECK_HEADERS([errno.h grp.h malloc.h nss.h pthread.h pwd.h shadow.h sqlite3.h string.h syslog.h unistd.h],
AC_CHECK_HEADERS([stddef.h stdlib.h stdbool.h stdio.h unistd.h errno.h syslog.h string.h pthread.h malloc.h nss.h pwd.h grp.h shadow.h zmq.h],
	[], AC_MSG_ERROR([Missing headers]))

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strdup])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
