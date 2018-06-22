// editsettingsdialog.h
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

#ifndef EDITSETTINGSDIALOG_H
#define EDITSETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>

class EditSettingsDialog : public QDialog
{
  Q_OBJECT
 public:
  EditSettingsDialog(QWidget *parent=0);
  QSize sizeHint() const;
  
 public slots:
  int exec();

 private slots:
  void selectImageDirectoryData();
  void selectFilesystemDirectoryData();
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *d_card_device_label;
  QLineEdit *d_card_device_edit;
  QLabel *d_image_directory_label;
  QLineEdit *d_image_directory_edit;
  QPushButton *d_image_directory_button;
  QLabel *d_filesystem_directory_label;
  QLineEdit *d_filesystem_directory_edit;
  QPushButton *d_filesystem_directory_button;
  QPushButton *d_ok_button;
  QPushButton *d_cancel_button;
};


#endif  // EDITSETTINGSDIRECTORY_H

