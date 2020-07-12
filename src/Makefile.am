include $(top_srcdir)/aminclude_static.am

AM_CFLAGS = \
	-Wall -Wextra -Werror
AM_CXXFLAGS = \
	-Wall -Wextra -Werror

if HAVE_CXX
manifestoxx_headers = \
	manifestoxx/BundleClass.h \
	manifestoxx/BundleDescriptor.h \
	manifestoxx/CPortDescriptor.h \
	manifestoxx/Descriptor.h \
	manifestoxx/InterfaceDescriptor.h \
	manifestoxx/Manifest.h \
	manifestoxx/ManifestHeader.h \
	manifestoxx/MnfbGenerator.h \
	manifestoxx/MnfsParser.h \
	manifestoxx/StringDescriptor.h

libmanifestoxx_la_SOURCES = \
	manifestoxx/BundleDescriptor.cpp \
	manifestoxx/CPortDescriptor.cpp \
	manifestoxx/Descriptor.cpp \
	manifestoxx/InterfaceDescriptor.cpp \
	manifestoxx/Manifest.cpp \
	manifestoxx/ManifestHeader.cpp \
	manifestoxx/MnfbGenerator.cpp \
	manifestoxx/MnfsParser.cpp \
	manifestoxx/StringDescriptor.cpp
libmanifestoxx_la_CPPFLAGS = \
	$(CODE_COVERAGE_CPPFLAGS)
libmanifestoxx_la_CFLAGS = \
	$(CODE_COVERAGE_CFLAGS)
libmanifestoxx_la_LIBADD = \
	$(CODE_COVERAGE_LIBS)

# variables used by the rest of automake
manifestoxxla = libmanifestoxx.la
manifestoxx_pc = manifestoxx.pc
endif

lib_LTLIBRARIES =  \
	libmanifesto.la     \
	$(manifestoxxla)

libmanifesto_la_SOURCES = \
	manifecto/manifest.c \
	manifecto/manifest-generator.c \
	manifecto/manifest-parser.c \
	manifecto/sgetline.c
libmanifesto_la_CPPFLAGS = \
	$(CODE_COVERAGE_CPPFLAGS)
libmanifesto_la_CFLAGS = \
	$(CODE_COVERAGE_CFLAGS)
libmanifesto_la_LIBADD = \
	$(CODE_COVERAGE_LIBS)

nobase_include_HEADERS = \
	manifecto/_cdefs.h \
	manifecto/manifest.h \
	$(manifestoxx_headers)

pkgconfig_DATA = \
	manifesto.pc      \
	$(manifestoxx_pc)
