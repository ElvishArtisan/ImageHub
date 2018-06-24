// importimagedialog.h
//
// Import an existing image into ImageHub
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

#ifndef IMPORTIMAGEDIALOG_H
#define IMPORTIMAGEDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QTextEdit>

#include "combobox.h"

class ImportImageDialog : public QDialog
{
  Q_OBJECT
 public:
  ImportImageDialog(QWidget *parent=0);
  QSize sizeHint() const;
  
 public slots:
  int exec();

 private slots:
  void selectFilenameData();
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *d_filename_label;
  QLineEdit *d_filename_edit;
  QPushButton *d_filename_button;
  QLabel *d_type_label;
  ComboBox *d_type_box;
  QCheckBox *d_delete_check;
  QLabel *d_delete_label;
  QLabel *d_notes_label;
  QTextEdit *d_notes_edit;
  QPushButton *d_ok_button;
  QPushButton *d_cancel_button;
};


#endif  // IMPORTIMAGEDIRECTORY_H

