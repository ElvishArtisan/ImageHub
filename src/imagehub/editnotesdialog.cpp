// editnotesdialog.cpp
//
// Edit Notes
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

#include <errno.h>
#include <stdio.h>

#include "config.h"
#include "db.h"
#include "editnotesdialog.h"
#include "settings.h"

EditNotesDialog::EditNotesDialog(QWidget *parent)
  : QDialog(parent)
{
  QFont label_font(font().family(),font().pointSize(),QFont::Bold);

  setWindowTitle("ImageHub - "+tr("Edit Notes"));

  //
  // Notes
  //
  d_notes_label=new QLabel(tr("Notes"),this);
  d_notes_label->setFont(label_font);
  d_notes_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  d_notes_edit=new QTextEdit(this);
  d_notes_edit->setAcceptRichText(true);

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


QSize EditNotesDialog::sizeHint() const
{
  return QSize(600,400);
}


int EditNotesDialog::exec(QString *html)
{
  d_notes_edit->setHtml(*html);
  d_html=html;

  return QDialog::exec();
}


void EditNotesDialog::okData()
{
  *d_html=d_notes_edit->toHtml();

  done(true);
}


void EditNotesDialog::cancelData()
{
  done(false);
}


void EditNotesDialog::resizeEvent(QResizeEvent *e)
{
  d_notes_label->setGeometry(15,10,size().width()-25,20);
  d_notes_edit->setGeometry(10,32,size().width()-20,size().height()-92);

  d_ok_button->setGeometry(size().width()-160,size().height()-50,70,40);

  d_cancel_button->setGeometry(size().width()-80,size().height()-50,70,40);
}
