# Copyright 2021-2022 MarcosHCK
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

PACKAGE_NAME=@PACKAGE_NAME@
PACKAGE_RELEASE=@PACKAGE_RELEASE@
PACKAGE_VERSION_MAYOR=@PACKAGE_VERSION_MAYOR@
PACKAGE_VERSION_MINOR=@PACKAGE_VERSION_MINOR@
PACKAGE_VERSION_MICRO=@PACKAGE_VERSION_MICRO@
PACKAGE_VERSION_BUILD=@PACKAGE_VERSION_BUILD@

EXTRA_DIST=
VOID=

#
# Resources
#

RES_RESOURCES=\
	res/exview2.svg \
	$(VOID)

UI_RESOURCES=\
	ui/message.ui \
	ui/window.ui \
	$(VOID)

#
# Bundle
#

RESOURCES=\
	$(RES_RESOURCES) \
	$(UI_RESOURCES) \
	$(VOID)

EXTRA_DIST+=\
	$(RESOURCES) \
	$(VOID)

ev_resources.c: ev_resources.gresources.xml $(RESOURCES)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --generate-source $<
	$(GLIB_COMPILE_RESOURCES) --target=$(@:.c=.h) --generate-header $<
	
CLEANFILES=\
	ev_resources.c \
	ev_resources.h \
	$(VOID)

#
# Binaries and libraries
#

noinst_LTLIBRARIES=libbundle.la

libbundle_la_SOURCES=\
	ev_resources.c \
	$(VOID)

libbundle_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(VOID)

libbundle_la_LDFLAGS=\
	-static \
	$(VOID)

libbundle_la_LIBADD=\
	$(GIO_LIBS) \
	$(VOID)
