// editnotesdialog.h
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

#ifndef EDITNOTESDIALOG_H
#define EDITNOTESDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QTextEdit>

class EditNotesDialog : public QDialog
{
  Q_OBJECT
 public:
  EditNotesDialog(QWidget *parent=0);
  QSize sizeHint() const;
  
 public slots:
  int exec(QString *html);

 private slots:
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *d_notes_label;
  QTextEdit *d_notes_edit;
  QPushButton *d_ok_button;
  QPushButton *d_cancel_button;
  QString *d_html;
};


#endif  // EDITNOTESDIALOG_H
