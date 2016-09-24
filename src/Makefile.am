AM_CPPFLAGS = -Wall -Werror
AM_CFLAGS = $(VARNISHAPI_CFLAGS)
AM_LDFLAGS = $(VARNISHAPI_LIBS)

vmod_LTLIBRARIES = libvmod_example.la

dist_man_MANS = vmod_example.3

libvmod_example_la_SOURCES = \
	vmod_example.c

nodist_libvmod_example_la_SOURCES = \
	vcc_example_if.c \
	vcc_example_if.h

@BUILD_VMOD_EXAMPLE@

# Tests

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_example="$(VMOD_EXAMPLE)"

TESTS = @VMOD_TESTS@

@VMOD_TESTS@: vmod_example.lo

EXTRA_DIST = \
	vmod_example.vcc \
	$(VMOD_TESTS)
