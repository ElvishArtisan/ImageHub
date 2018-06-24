// listimagesdialog.cpp
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

#include <unistd.h>

#include <QMessageBox>

#include "db.h"
#include "imagesmodel.h"
#include "listimagesdialog.h"
#include "settings.h"

ListImagesDialog::ListImagesDialog(QWidget *parent)
  : ListDialog(parent)
{
  setWindowTitle("ImageHub - "+tr("List Images"));
  setMinimumSize(sizeHint());

  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Dialogs
  //
  d_edit_notes_dialog=new EditNotesDialog(this);
  d_import_image_dialog=new ImportImageDialog(this);

  //
  // Images List
  //
  list_model=new ImagesModel(this);
  QString sql=QString("select ")+
    "ID,"+               // 00
    "FILENAME,"+         // 01
    "TYPE,"+             // 02
    "SIZE,"+             // 03
    "ORIGIN_DATETIME,"+  // 04
    "SHA1_HASH "+        // 05
    "from IMAGES order by FILENAME";
  list_model->setQuery(sql);
  list_model->setHeaderData(0,Qt::Horizontal,tr("ID"));
  list_model->setHeaderData(1,Qt::Horizontal,"File Name");
  list_model->setHeaderData(2,Qt::Horizontal,"Type");
  list_model->setFieldType(2,SqlTableModel::ImageType);
  list_model->setHeaderData(3,Qt::Horizontal,"Size");
  list_model->setHeaderData(4,Qt::Horizontal,"Origin DateTime");
  list_model->setHeaderData(5,Qt::Horizontal,"SHA1 Hash");
  list_view=new TableView(this);
  list_view->setModel(list_model);
  list_view->hideColumn(0);
  list_view->resizeColumnsToContents();
  connect(list_view,SIGNAL(clicked(const QModelIndex &)),
	  this,SLOT(listClickedData(const QModelIndex &)));

  list_import_button=new QPushButton(tr("Import"),this);
  list_import_button->setFont(bold_font);
  connect(list_import_button,SIGNAL(clicked()),
	  this,SLOT(importData()));

  list_decant_button=new QPushButton(tr("Decant"),this);
  list_decant_button->setFont(bold_font);
  list_decant_button->setDisabled(true);
  connect(list_decant_button,SIGNAL(clicked()),this,SLOT(decantData()));

  list_delete_button=new QPushButton(tr("Delete"),this);
  list_delete_button->setFont(bold_font);
  list_delete_button->setDisabled(true);
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  list_notes_button=new QPushButton(tr("Edit")+"\n"+tr("Notes"),this);
  list_notes_button->setFont(bold_font);
  list_notes_button->setDisabled(true);
  connect(list_notes_button,SIGNAL(clicked()),this,SLOT(notesData()));

  list_close_button=new QPushButton(tr("Close"),this);
  list_close_button->setFont(bold_font);
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));
}


ListImagesDialog::~ListImagesDialog()
{
}


QSize ListImagesDialog::sizeHint() const
{
  return QSize(1000,600);
}


int ListImagesDialog::exec()
{
  list_model->update();
  list_view->resizeColumnsToContents();
  return QDialog::exec();
}


void ListImagesDialog::listClickedData(const QModelIndex &index)
{
  list_decant_button->setEnabled(index.isValid());
  list_delete_button->setEnabled(index.isValid());
  list_notes_button->setEnabled(index.isValid());
}


void ListImagesDialog::importData()
{
  if(d_import_image_dialog->exec()) {
    list_model->update();
  }
}


void ListImagesDialog::decantData()
{
  /*
  QItemSelectionModel *s=list_view->selectionModel();
  if(s->hasSelection()) {
    int event_id=s->selectedRows()[0].data().toInt();
    if(list_editevent_dialog->exec(&event_id)) {
      list_model->update();
      list_view->resizeColumnsToContents();
    }
  }
  */
}


void ListImagesDialog::deleteData()
{
  QString sql;

  QItemSelectionModel *s=list_view->selectionModel();
  if(s->hasSelection()) {
    int id=s->selectedRows()[0].data().toInt();
    if(QMessageBox::question(this,tr("ImageHub - Delete Image"),
			     tr("Are you sure you want to delete the")+
			     " \""+Settings::imageFilename(id)+"\" "+
			     tr("image?"),
			     QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) {
      unlink(Settings::imagePathname(id).toUtf8());
      sql=QString("delete from IMAGES where ")+
	QString().sprintf("ID=%d",id);
      SqlQuery::run(sql);
      list_model->update();
    }
  }
}


void ListImagesDialog::notesData()
{
  QString sql;
  SqlQuery *q;

  QItemSelectionModel *s=list_view->selectionModel();
  if(s->hasSelection()) {
    int id=s->selectedRows()[0].data().toInt();
    sql=QString("select NOTES from IMAGES where ")+
      QString().sprintf("ID=%d",id);
    q=new SqlQuery(sql);
    if(q->first()) {
      QString html=q->value(0).toString();
      if(d_edit_notes_dialog->exec(&html)) {
	sql=QString("update IMAGES set ")+
	  "NOTES=\""+SqlQuery::escape(html)+"\" where "+
	  QString().sprintf("ID=%d",id);
	SqlQuery::run(sql);
      }
    }
    delete q;
  }
}


void ListImagesDialog::closeData()
{
  done(true);
}


void ListImagesDialog::resizeEvent(QResizeEvent *e)
{
  list_view->setGeometry(10,32,size().width()-20,size().height()-112);

  list_import_button->setGeometry(10,size().height()-60,80,50);
  list_decant_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);

  list_notes_button->setGeometry(400,size().height()-60,80,50);

  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);

  ListDialog::resizeEvent(e);
}
