// managementconfig.cpp
//
// Class for gnmc(1) and gnmd(8) configuration.
//
// (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>

#include "config.h"
#include "db.h"

Config::Config()
{
}


QString Config::mysqlHostname() const
{
  return config_mysql_hostname;
}


QString Config::mysqlDbname() const
{
  return config_mysql_dbname;
}


QString Config::mysqlUsername() const
{
  return config_mysql_username;
}


QString Config::mysqlPassword() const
{
  return config_mysql_password;
}


QString Config::mysqlEngine() const
{
  return config_mysql_engine;
}


QString Config::mysqlCharset() const
{
  return config_mysql_charset;
}


QString Config::mysqlCollation() const
{
  return config_mysql_collation;
}


QString Config::createTablePostfix() const
{
  return QString(" ENGINE ")+config_mysql_engine+" "+
    "CHARACTER SET "+config_mysql_charset+" "+
    "COLLATE "+config_mysql_collation;
}


bool Config::openDb(QString *err_msg,bool schema_check)
{
  QSettings s(IMAGEHUB_CONF_FILE,QSettings::IniFormat);

  config_mysql_hostname=
    s.value("MysqlHostname",IMAGEHUB_DEFAULT_MYSQL_HOSTNAME).toString();
  config_mysql_dbname=
    s.value("MysqlDbname",IMAGEHUB_DEFAULT_MYSQL_DBNAME).toString();
  config_mysql_username=
    s.value("MysqlUsername",IMAGEHUB_DEFAULT_MYSQL_USERNAME).toString();
  config_mysql_password=
    s.value("MysqlPassword",IMAGEHUB_DEFAULT_MYSQL_PASSWORD).toString();
  config_mysql_engine=
    s.value("MysqlEngine",IMAGEHUB_DEFAULT_MYSQL_ENGINE).toString();
  config_mysql_charset=
    s.value("MysqlCharset",IMAGEHUB_DEFAULT_MYSQL_CHARSET).toString();
  config_mysql_collation=
    s.value("MysqlCollation",IMAGEHUB_DEFAULT_MYSQL_COLLATION).toString();

  QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL3");
  if(!db.isValid()) {
    *err_msg=QObject::tr("Unable to add MySQL database");
    return false;
  }
  db.setHostName(mysqlHostname());
  db.setDatabaseName(mysqlDbname());
  db.setUserName(mysqlUsername());
  db.setPassword(mysqlPassword());
  if(!db.open()) {
    *err_msg=db.lastError().text();
    return false;
  }

  if(schema_check) {
    int schema=0;
    QString sql=QString("select DB from VERSION");
    SqlQuery *q=new SqlQuery(sql);
    if(q->first()) {
      schema=q->value(0).toInt();
    }
    delete q;
    if(schema!=IMAGEHUB_SCHEMA_VERSION) {
      *err_msg=QObject::tr("skewed database version");
      return false;
    }
  }
  *err_msg=QObject::tr("OK");
  return true;
}


QString Config::typeString(Config::ImageType type)
{
  QString ret=QObject::tr("Unknown");

  switch(type) {
  case Config::BootImage:
    ret=QObject::tr("Boot");
    break;

  case Config::InstallableImage:
    ret=QObject::tr("Installable");
    break;

  case Config::RawImage:
    ret=QObject::tr("Raw");
    break;

  case Config::LastImage:
    break;
  }

  return ret;
}


QString Config::typeString(Config::FilesystemType type)
{
  QString ret=QObject::tr("Unknown");

  switch(type) {
  case Config::BootFilesystem:
    ret=QObject::tr("Boot");
    break;

  case Config::RootFilesystem:
    ret=QObject::tr("Root");
    break;

  case Config::LastFilesystem:
    break;
  }

  return ret;
}
