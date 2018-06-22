## automake.am
##
## Makefile for the imagehub(1)
##
## (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing @QT4GUI_CFLAGS@ @OPENSSL_CFLAGS@
MOC = @QT4_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@MOC@ $< -o $@


bin_PROGRAMS = imagehub

dist_imagehub_SOURCES = cmdswitch.cpp cmdswitch.h\
                        combobox.cpp combobox.h\
                        config.cpp config.h\
                        db.cpp db.h\
                        editsettingsdialog.cpp editsettingsdialog.h\
                        imagehub.cpp imagehub.h\
                        imagesmodel.cpp imagesmodel.h\
                        importimagedialog.cpp importimagedialog.h\
                        listdialog.cpp listdialog.h\
                        listimagesdialog.cpp listimagesdialog.h\
                        sqltablemodel.cpp sqltablemodel.h

nodist_imagehub_SOURCES = moc_combobox.cpp\
                          moc_db.cpp\
                          moc_editsettingsdialog.cpp\
                          moc_imagehub.cpp\
                          moc_imagesmodel.cpp\
                          moc_importimagedialog.cpp\
                          moc_listdialog.cpp\
                          moc_listimagesdialog.cpp\
                          moc_sqltablemodel.cpp

imagehub_LDADD = @QT4GUI_LIBS@ @OPENSSL_LIBS@

CLEANFILES = *~\
             moc_*\
             *.obj\
             *.idb\
             *.pdb\
             *ilk
MAINTAINERCLEANFILES = *~\
                       Makefile.in
