// imagehub.cpp
//
// imagehub(1)
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

#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QMessageBox>

#include "cmdswitch.h"
#include "db.h"
#include "imagehub.h"

MainWidget::MainWidget(QWidget *parent)
  : QMainWindow(parent)
{
  QString err_msg;

  CmdSwitch *cmd=
    new CmdSwitch(qApp->argc(),qApp->argv(),"imagehub",VERSION,IMAGEHUB_USAGE);
  for(unsigned i=0;i<(cmd->keys());i++) {
    if(!cmd->processed(i)) {
      fprintf(stderr,"imagehub: unknown option\n");
      exit(256);
    }
  }

  setWindowTitle(tr("ImageHub")+" v"+VERSION);

  QFont label_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Open (and perhaps create) Database
  //
  hub_config=new Config();
  if(!hub_config->openDb(&err_msg,false)) {
    QMessageBox::warning(this,tr("ImageHub - Error"),
			 tr("Unable to open database")+" ["+err_msg+"].");
    exit(256);
  }
  if(!CheckDb(&err_msg)) {
    QMessageBox::warning(this,tr("ImageHub - Error"),
			 tr("Unable to open database")+" ["+err_msg+"].");
    exit(256);
  }
  DbHeartbeat(this);

  //
  // Dialogs
  //
  hub_settings_dialog=new EditSettingsDialog(this);
  hub_list_images_dialog=new ListImagesDialog(this);

  //
  // Manage Images Button
  //
  hub_list_images_button=new QPushButton(tr("Manage Images"),this);
  hub_list_images_button->setFont(label_font);
  connect(hub_list_images_button,SIGNAL(clicked()),
	  hub_list_images_dialog,SLOT(exec()));

  //
  // Edit Settings Button
  //
  hub_edit_settings_button=new QPushButton(tr("Edit Settings"),this);
  hub_edit_settings_button->setFont(label_font);
  connect(hub_edit_settings_button,SIGNAL(clicked()),
	  hub_settings_dialog,SLOT(exec()));

  //
  // Close Button
  //
  hub_close_button=new QPushButton(tr("Close"),this);
  hub_close_button->setFont(label_font);
  connect(hub_close_button,SIGNAL(clicked()),qApp,SLOT(quit()));
}


QSize MainWidget::sizeHint() const
{
  return QSize(370,261);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  exit(0);
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  hub_list_images_button->setGeometry(45,10,size().width()-90,60);

  hub_edit_settings_button->setGeometry(45,80,size().width()-90,60);

  hub_close_button->setGeometry(45,196,size().width()-90,60);
}


bool MainWidget::CheckDb(QString *err_msg)
{
  QString sql=QString("select DB from VERSION");
  SqlQuery *q=new SqlQuery(sql);
  if(!q->first()) {
    if(!CreateDb()) {
      *err_msg=tr("unable to create database");
      return false;
    }
  }
  delete q;
  if(!CheckSchema()) {
    *err_msg=tr("invalid/unrecognized database schema");
    return false;
  }

  return true;
}


bool MainWidget::CreateDb()
{
  QString sql;
  bool ok=false;

  sql=QString("create table VERSION(DB int not null)")+
    hub_config->createTablePostfix();
  SqlQuery::run(sql,&ok);
  if(!ok) {
    return false;
  }
  sql=QString("insert into VERSION values(0)");
  SqlQuery::run(sql,&ok);

  return ok;
}


bool MainWidget::CheckSchema()
{
  int schema;
  bool ok=false;
  QString sql;
  SqlQuery *q=NULL;

  sql=QString("select DB from VERSION");
  q=new SqlQuery(sql);
  if(!q->first()) {
    delete q;
    return false;
  }
  schema=q->value(0).toInt();
  delete  q;

  if(schema>IMAGEHUB_SCHEMA_VERSION) {
    QMessageBox::warning(this,tr("ImageHub - DB Schema Skew"),
	   tr("This version of ImageHub is incompatible with schema version")+
			 QString().sprintf(" %d.",schema));
    exit(256);
  }

  if(schema<IMAGEHUB_SCHEMA_VERSION) {
    if(QMessageBox::question(this,tr("ImageHub - DB Update"),
			     tr("The DB schema needs to be updated.")+"\n"+
			     tr("Proceed?"),
			     QMessageBox::No,QMessageBox::Yes)!=
       QMessageBox::Yes) {
      exit(0);
    }
  }

  if(schema<1) {
    sql=QString("create table IMAGES (")+
      "ID integer primary key auto_increment,"+
      "FILENAME char(255) unique not null,"+
      "TYPE int not null,"+
      "SIZE int,"+
      "SHA1_HASH char(40),"+
      "NOTES text,"+
      "index FILENAME_IDX(FILENAME))"+
      hub_config->createTablePostfix();
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }

    sql=QString("create table FILESYSTEMS (")+
      "ID integer primary key auto_increment,"+
      "DIRECTORY char(255) unique not null,"+
      "TYPE int not null,"+
      "NOTES text,"+
      "index DIRECTORY_IDX(DIRECTORY),"+
      "index TYPE_IDX(TYPE))"+
      hub_config->createTablePostfix();
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }

    sql=QString("create table SETTINGS (")+
      "ID integer primary key auto_increment,"+
      "IMAGE_DIRECTORY char(255) not null default '/var/lib/imagehub/images',"+
      "FILESYSTEM_DIRECTORY char(255) not null default '/var/lib/imagehub/filesystems',"+
      "CARD_DEVICE char(255) not null default '/dev/null')"+
      hub_config->createTablePostfix();
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }
    sql=QString("insert into SETTINGS set ")+
      "CARD_DEVICE=\"/dev/null\"";
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }
  }


  //
  // *** End of schema updates ***
  //

  sql=QString("update VERSION set ")+
    QString().sprintf("DB=%d",IMAGEHUB_SCHEMA_VERSION);
  SqlQuery::run(sql,&ok);

  return ok;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);

  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
