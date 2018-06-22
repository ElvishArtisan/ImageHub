// editsettingsdialog.cpp
//
// Edit ImageHub Settings
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

#include <QFileDialog>

#include "db.h"
#include "editsettingsdialog.h"

EditSettingsDialog::EditSettingsDialog(QWidget *parent)
  : QDialog(parent)
{
  QFont label_font(font().family(),font().pointSize(),QFont::Bold);

  setWindowTitle("ImageHub - "+tr("Edit Settings"));

  //
  // Card Device
  //
  d_card_device_label=new QLabel(tr("Card Device")+":",this);
  d_card_device_label->setFont(label_font);
  d_card_device_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  d_card_device_edit=new QLineEdit(this);

  //
  // Image Directory
  //
  d_image_directory_label=new QLabel(tr("Image Directory")+":",this);
  d_image_directory_label->setFont(label_font);
  d_image_directory_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  d_image_directory_edit=new QLineEdit(this);
  d_image_directory_button=new QPushButton(tr("Select"),this);
  connect(d_image_directory_button,SIGNAL(clicked()),
	  this,SLOT(selectImageDirectoryData()));

  //
  // Filesystem Directory
  //
  d_filesystem_directory_label=new QLabel(tr("Filesystem Directory")+":",this);
  d_filesystem_directory_label->setFont(label_font);
  d_filesystem_directory_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  d_filesystem_directory_edit=new QLineEdit(this);
  d_filesystem_directory_button=new QPushButton(tr("Select"),this);
  connect(d_filesystem_directory_button,SIGNAL(clicked()),
	  this,SLOT(selectFilesystemDirectoryData()));

  //
  // OK Button
  //
  d_ok_button=new QPushButton(tr("OK"),this);
  d_ok_button->setFont(label_font);
  connect(d_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  d_cancel_button=new QPushButton(tr("Cancel"),this);
  d_cancel_button->setFont(label_font);
  connect(d_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize EditSettingsDialog::sizeHint() const
{
  return QSize(600,150);
}


int EditSettingsDialog::exec()
{
  QString sql=QString("select ")+
    "CARD_DEVICE,"+           // 00
    "IMAGE_DIRECTORY,"+       // 01
    "FILESYSTEM_DIRECTORY "+  // 02
    "from SETTINGS";
  SqlQuery *q=new SqlQuery(sql);
  if(q->first()) {
    d_card_device_edit->setText(q->value(0).toString());
    d_image_directory_edit->setText(q->value(1).toString());
    d_filesystem_directory_edit->setText(q->value(2).toString());
  }
  delete q;

  return QDialog::exec();
}


void EditSettingsDialog::selectImageDirectoryData()
{
  QString dir=QFileDialog::
    getExistingDirectory(this,"ImageHub - "+tr("Choose Directory"),
			 d_image_directory_edit->text(),
			 QFileDialog::ShowDirsOnly);
  if(!dir.isEmpty()) {
    d_image_directory_edit->setText(dir);
  }
}


void EditSettingsDialog::selectFilesystemDirectoryData()
{
  QString dir=QFileDialog::
    getExistingDirectory(this,"ImageHub - "+tr("Choose Directory"),
			 d_filesystem_directory_edit->text(),
			 QFileDialog::ShowDirsOnly);
  if(!dir.isEmpty()) {
    d_filesystem_directory_edit->setText(dir);
  }
}


void EditSettingsDialog::okData()
{
  QString sql=QString("update SETTINGS set ")+
    "CARD_DEVICE=\""+SqlQuery::escape(d_card_device_edit->text())+"\","+
    "IMAGE_DIRECTORY=\""+SqlQuery::escape(d_image_directory_edit->text())+"\","+
    "FILESYSTEM_DIRECTORY=\""+
    SqlQuery::escape(d_filesystem_directory_edit->text())+"\"";
  SqlQuery::run(sql);

  done(true);
}


void EditSettingsDialog::cancelData()
{
  done(false);
}


void EditSettingsDialog::resizeEvent(QResizeEvent *e)
{
  d_card_device_label->setGeometry(10,10,200,20);
  d_card_device_edit->setGeometry(215,10,200,20);

  d_image_directory_label->setGeometry(10,40,200,20);
  d_image_directory_edit->setGeometry(215,40,size().width()-285,20);
  d_image_directory_button->setGeometry(size().width()-60,36,50,26);

  d_filesystem_directory_label->setGeometry(10,70,200,20);
  d_filesystem_directory_edit->setGeometry(215,70,size().width()-285,20);
  d_filesystem_directory_button->setGeometry(size().width()-60,66,50,26);

  d_ok_button->setGeometry(size().width()-160,size().height()-50,70,40);

  d_cancel_button->setGeometry(size().width()-80,size().height()-50,70,40);
}
