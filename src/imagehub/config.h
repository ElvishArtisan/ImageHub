// managementconfig.h
//
// Class for ImageHub configuration
//
// (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
//     All Rights Reserved.
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

#ifndef MANAGEMENTCONFIG_H
#define MANAGEMENTCONFIG_H

#include <QString>

#define IMAGEHUB_CONF_FILE QString("/etc/imagehub.conf")
#define IMAGEHUB_DEFAULT_MYSQL_HOSTNAME "localhost"
#define IMAGEHUB_DEFAULT_MYSQL_DBNAME "ImageHub"
#define IMAGEHUB_DEFAULT_MYSQL_USERNAME "imagehubuser"
#define IMAGEHUB_DEFAULT_MYSQL_PASSWORD "letmein"
#define IMAGEHUB_DEFAULT_MYSQL_ENGINE "InnoDB"
#define IMAGEHUB_DEFAULT_MYSQL_CHARSET "utf8"
#define IMAGEHUB_DEFAULT_MYSQL_COLLATION "utf8_general_ci"

class Config
{
 public:
  enum ImageType {BootImage=0,InstallableImage=1,RawImage=2,LastImage=3};
  enum FilesystemType {BootFilesystem=0,RootFilesystem=1,LastFilesystem=2};
  Config();
  QString mysqlHostname() const;
  QString mysqlDbname() const;
  QString mysqlUsername() const;
  QString mysqlPassword() const;
  QString mysqlEngine() const;
  QString mysqlCharset() const;
  QString mysqlCollation() const;
  QString createTablePostfix() const;
  bool openDb(QString *err_msg,bool schema_check);
  static QString typeString(ImageType type);
  static QString typeString(FilesystemType type);

 private:
  QString config_mysql_hostname;
  QString config_mysql_dbname;
  QString config_mysql_username;
  QString config_mysql_password;
  QString config_mysql_engine;
  QString config_mysql_charset;
  QString config_mysql_collation;
};


#endif  // CONFIG_H
