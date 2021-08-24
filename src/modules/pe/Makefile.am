# Copyright 2021-2023 MarcosHCK
# This file is part of exview2.
#
# exview2 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# exview2 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with exview2. If not, see <http://www.gnu.org/licenses/>.
#

#
# Some variables
#

PACKAGE_RELEASE=@PACKAGE_RELEASE@
PACKAGE_VERSION_MAYOR=@PACKAGE_VERSION_MAYOR@
PACKAGE_VERSION_MINOR=@PACKAGE_VERSION_MINOR@
PACKAGE_VERSION_MICRO=@PACKAGE_VERSION_MICRO@
PACKAGE_VERSION_BUILD=@PACKAGE_VERSION_BUILD@

EXTRA_DIST=
VOID=

#
# Include paths
#

INCLUDE_PATH=\
	-I${top_builddir}/src/ \
	-I${top_builddir}/src/modules/ \
	$(VOID)

#
# Unlisted C headers
#

EXTRA_DIST+=\
	pe_parser.h \
	pe_structs.h \
	$(VOID)

#
# Binaries and libraries
#

noinst_LTLIBRARIES=libpe_module.la

libpe_module_la_SOURCES=\
	pe_parse_headers.c \
	pe_parser.c \
	$(VOID)

libpe_module_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(INCLUDE_PATH) \
	$(VOID)
libpe_module_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(VOID)
