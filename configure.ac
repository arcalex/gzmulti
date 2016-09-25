#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gzmulti], [0.1], [archive@bibalex.org])
AM_INIT_AUTOMAKE
LT_INIT
AC_CONFIG_SRCDIR([include/gzmulti/gzmulti.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([z], [inflateInit2])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 include/gzmulti/Makefile
                 libgzmulti/Makefile
                 gzmulti/Makefile])
AC_OUTPUT
