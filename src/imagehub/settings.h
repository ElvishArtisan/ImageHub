// settings.h
//
// DB-based settings.
//
//   (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>

#include "db.h"

class Settings
{
 public:
  static QString imageDirectory();
  static QString filesystemDirectory();
  static QString cardDevice();
  static QString imageFilename(int id);
  static QString imageFilename(const QString &pathname);
  static QString imagePathname(int id);
  static QString imagePathname(const QString &filename);
  static QString sha1Hash(const QString &pathname);
  static bool fileCopy(const QString &destfile,const QString &srcfile,
		       QString *err_msg);

 private:
  static QVariant GetValue(const QString &field);
};


#endif  // LISTDIALOG_H
