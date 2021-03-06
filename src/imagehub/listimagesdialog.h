// listimagesdialog.h
//
// List ImageHub images.
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

#ifndef LISTIMAGESDIALOG_H
#define LISTIMAGESDIALOG_H

#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>

#include "editnotesdialog.h"
#include "importimagedialog.h"
#include "imagesmodel.h"
#include "listdialog.h"
#include "tableview.h"

class ListImagesDialog : public ListDialog
{
  Q_OBJECT
 public:
  ListImagesDialog(QWidget *parent=0);
  ~ListImagesDialog();
  QSize sizeHint() const;
  
 public slots:
  int exec();

 private slots:
  void listClickedData(const QModelIndex &index);
  void importData();
  void decantData();
  void deleteData();
  void notesData();
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  ImportImageDialog *d_import_image_dialog;
  EditNotesDialog *d_edit_notes_dialog;
  ImagesModel *list_model;
  TableView *list_view;
  QPushButton *list_import_button;
  QPushButton *list_decant_button;
  QPushButton *list_delete_button;
  QPushButton *list_notes_button;
  QPushButton *list_close_button;
};


#endif  // LISTEVENTS_H
