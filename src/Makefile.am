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

# Keep sync with settings/Makefile.am
schemasdir=${pkgdatadir}/schemas/

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
# Desktop files
#
#desktopdir = $(datadir)/applications
#desktop_in_files = org.hck.deusexmakina.desktop.in
#desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

#@INTLTOOL_DESKTOP_RULE@

check-local: $(desktop_DATA)
	@list=$(desktop_DATA); test -n "$(desktopdir)" || list=; \
	for file in $$list; do \
		echo $(DESKTOP_FILE_VALIDATE) "$$file";\
		$(DESKTOP_FILE_VALIDATE) "$$file"; \
	done

#CLEANFILES=\
#	$(desktop_DATA)

#
# Header files
#

EXTRA_DIST+=\
	ev2_application.h \
	ev2_message.h \
	ev2_module.h \
	ev2_module_manager.h \
	ev2_parser.h \
	ev2_settings.h \
	ev2_view_context.h \
	$(VOID)


#
# Binaries and libraries
#

bin_PROGRAMS=exview2
exview2_SOURCES=\
	ev2_application.c \
	ev2_message.vala \
	ev2_module.c \
	ev2_module_manager.c \
	ev2_module_self.c \
	ev2_parser.c \
	ev2_settings.vala \
	ev2_view_context.vala \
	ev2_window.vala \
	$(VOID)

exview2_CFLAGS=\
	$(GTK_CFLAGS) \
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GMODULE_CFLAGS) \
	-DGAPPNAME=\"org.hck.${PACKAGE_NAME}\" \
	-DGRESNAME=\"/org/hck/${PACKAGE_NAME}\" \
	-DPKGLIBDIR=\"${pkglibdir}\" \
	-DLOCALEDIR=\"${localedir}\" \
	-DSCHEMASDIR=\"${schemasdir}\" \
	-DABSTOPBUILDDIR=\"${abs_top_builddir}\" \
	-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\" \
	$(VOID)

exview2_LDFLAGS=\
	$(GTK_LIBS) \
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GMODULE_LIBS) \
	resources/libbundle.la \
	../modules/elf/libelf_module.la \
	$(VOID)

#
# VALA
#

VALA_PKG=\
	gtk+-3.0 \
	gio-2.0 \
	glib-2.0 \
	config \
	exview2 \
	$(VOID)

AM_VALAFLAGS=\
	--vapidir=../vapi/ \
	--gresources="resources/ev_resources.gresources.xml" \
	$(addprefix --pkg , ${VALA_PKG}) \
	$(VOID)

#
# Sub-directories
#

SUBDIRS=\
	resources
