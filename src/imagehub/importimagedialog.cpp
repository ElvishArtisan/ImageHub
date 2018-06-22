// importimagedialog.cpp
//
// Import an image into ImageHub
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

#include "config.h"
#include "db.h"
#include "importimagedialog.h"

ImportImageDialog::ImportImageDialog(QWidget *parent)
  : QDialog(parent)
{
  QFont label_font(font().family(),font().pointSize(),QFont::Bold);

  setWindowTitle("ImageHub - "+tr("Import Image"));

  //
  // Filename
  //
  d_filename_label=new QLabel(tr("Image Filename")+":",this);
  d_filename_label->setFont(label_font);
  d_filename_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  d_filename_edit=new QLineEdit(this);
  d_filename_button=new QPushButton(tr("Select"),this);
  connect(d_filename_button,SIGNAL(clicked()),
	  this,SLOT(selectFilenameData()));

  //
  // Image Type
  //
  d_type_label=new QLabel(tr("Image Type")+":",this);
  d_type_label->setFont(label_font);
  d_type_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  d_type_box=new ComboBox(this);
  for(int i=0;i<Config::LastImage;i++) {
    d_type_box->insertItem(d_type_box->count(),
			   Config::typeString((Config::ImageType)i),i);
  }

  //
  // Delete Source File
  //
  d_delete_check=new QCheckBox(this);
  d_delete_label=new QLabel(tr("Delete Original File After Import"),this);
  d_delete_label->setFont(label_font);
  d_delete_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

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


QSize ImportImageDialog::sizeHint() const
{
  return QSize(600,150);
}


void ImportImageDialog::selectFilenameData()
{
  QString filename=QFileDialog::
    getOpenFileName(this,"ImageHub - "+tr("Choose Image to Import"),
		    d_filename_edit->text(),
		    tr("Images (*.img)"));
  if(!filename.isEmpty()) {
    d_filename_edit->setText(filename);
  }
}


void ImportImageDialog::okData()
{
  done(true);
}


void ImportImageDialog::cancelData()
{
  done(false);
}


void ImportImageDialog::resizeEvent(QResizeEvent *e)
{
  d_filename_label->setGeometry(10,10,200,20);
  d_filename_edit->setGeometry(215,10,size().width()-285,20);
  d_filename_button->setGeometry(size().width()-60,6,50,26);

  d_type_label->setGeometry(10,40,200,20);
  d_type_box->setGeometry(215,40,150,20);

  d_delete_check->setGeometry(215,70,20,20);
  d_delete_label->setGeometry(240,70,size().width()-50,20);

  d_ok_button->setGeometry(size().width()-160,size().height()-50,70,40);

  d_cancel_button->setGeometry(size().width()-80,size().height()-50,70,40);
}
