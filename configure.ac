AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (c) 2011-2015 Varnish Software AS])
AC_INIT([libvmod-example], [0.1])
AC_CONFIG_MACRO_DIR([m4])
m4_ifndef([VARNISH_VMOD_INCLUDES], AC_MSG_ERROR([Need varnish.m4 -- see README.rst]))
AC_CONFIG_SRCDIR(src/vmod_example.vcc)
AM_CONFIG_HEADER(config.h)

AC_CANONICAL_SYSTEM
AC_LANG(C)

AM_INIT_AUTOMAKE([foreign])

AC_GNU_SOURCE

AX_PTHREAD(,[AC_MSG_ERROR([Could not configure pthreads support])])

LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

LT_INIT([disable-static])
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET

AC_ARG_WITH([rst2man],
	AS_HELP_STRING(
		[--with-rst2man=PATH],
		[Location of rst2man (auto)]),
	[RST2MAN="$withval"],
	AC_CHECK_PROGS(RST2MAN, [rst2man rst2man.py], []))

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/stdlib.h])

VARNISH_PREREQ([5.0])
VARNISH_VMODS([example])

# inherit the prefix from Varnish.
# acessing ac_ variable because AC_PREFIX_DEFAULT acts too early
ac_default_prefix=$LIBVARNISHAPI_PREFIX

VMOD_TESTS="$(cd $srcdir/src && echo tests/*.vtc)"
AC_SUBST(VMOD_TESTS)

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
