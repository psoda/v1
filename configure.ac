#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(psoda, 0.02, clement@cs.byu.edu)
dnl AC_CONFIG_SRCDIR([config.h.in])
dnl AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

#defaults

AC_ARG_ENABLE(debug,AS_HELP_STRING(--enable-debug,Enable -g option while
            compiling),CFLAGS="$CFLAGS -g3 -O0 -Wmissing-prototypes -Wextra -Wall -Wformat -Wformat-security" && CXXFLAGS="$CXXFLAGS -g3 -O0 -Wformat -Wformat-security  -Wextra -Wall -Wcast-qual -Wwrite-strings -Wcast-align -Wpointer-arith -Wconversion -Wredundant-decls  -Winline -Wfloat-equal -Wconversion -Wmissing-noreturn   -Wredundant-decls -Wctor-dtor-privacy -Woverloaded-virtual -Wno-error",CFLAGS="$CFLAGS -DNDEBUG -O3 -ffast-math -funroll-loops -Wall -Wextra" && CXXFLAGS="$CXXFLAGS -DNDEBUG  -O3 -ffast-math -funroll-loops -fmodulo-sched -freschedule-modulo-scheduled-loops -fgcse-sm -fgcse-las -Wformat -Wformat-security  -Wextra -Wall -Wcast-qual -Wwrite-strings -Wcast-align -Wpointer-arith -Wconversion -Wredundant-decls  -Winline -Wfloat-equal -Wconversion -Wmissing-noreturn   -Wredundant-decls -Wctor-dtor-privacy -Woverloaded-virtual -Wno-error")
AC_ARG_ENABLE(readline,AS_HELP_STRING(--enable-readline,Enable readline capabilities in interactive
            mode),CFLAGS="$CFLAGS -DUSEREADLINE" && CXXFLAGS="$CXXFLAGS -DUSEREADLINE" && LDFLAGS="$LDFLAGS -lncurses -lreadline -lhistory")
AC_ARG_ENABLE(profile,AS_HELP_STRING(--enable-profile,Enable -pg option while
            compiling),CFLAGS="$CFLAGS -pg" && CXXFLAGS="$CXXFLAGS -pg")
AC_ARG_ENABLE(gui,AS_HELP_STRING(--enable-gui,Enable Psoda GUI_FLAGS while
            compiling),CFLAGS="$CFLAGS -fPIC -D GUI" && CXXFLAGS="$CXXFLAGS -fPIC -D GUI")
AC_ARG_ENABLE(coverage,AS_HELP_STRING(--enable-coverage, Remove optimization and include -fprofile-arcs and -ftest-coverage when
            compiling),CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage" && CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage")

CXXFLAGS="$CXXFLAGS $GUI_FLAGS -fnon-call-exceptions -fasynchronous-unwind-tables"

CFLAGS="$CFLAGS  -fnon-call-exceptions -fasynchronous-unwind-tables"

#this is for libtool on the mac
if test -z "$host" ; then  
host=`./config.guess`
fi

if test -z "$target" ; then  
target=`./config.guess`
fi

# Checks for programs.
AC_PROG_INSTALL
AC_PROG_YACC
AC_PROG_LEX
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h inttypes.h limits.h stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([gettimeofday memset strdup])

AC_CONFIG_FILES([Makefile\
		 src/gui/Makefile\
                 src/Makefile])
AM_CONDITIONAL([LINUX], [test -n "`echo ${target} | grep linux`"])
AM_CONDITIONAL([MAC], [test -n "`echo ${target} | grep apple`"])
AM_CONDITIONAL([WINDOWS], [test -n "`echo ${target} | grep win32`"])
AC_OUTPUT
