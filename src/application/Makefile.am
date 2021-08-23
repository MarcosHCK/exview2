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
# Desktop files
#
desktopdir = $(datadir)/applications
desktop_in_files = org.hck.exview2.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

check-local: $(desktop_DATA)
	@list=$(desktop_DATA); test -n "$(desktopdir)" || list=; \
	for file in $$list; do \
		echo $(DESKTOP_FILE_VALIDATE) "$$file";\
		$(DESKTOP_FILE_VALIDATE) "$$file"; \
	done

CLEANFILES=\
	$(desktop_DATA)
