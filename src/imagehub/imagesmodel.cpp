// imagesmodel.cpp
//
// Model for ImageHub images.
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

#include "imagesmodel.h"

ImagesModel::ImagesModel(QObject *parent)
  : SqlTableModel(parent)
{
}


ImagesModel::~ImagesModel()
{
}


QVariant ImagesModel::data(const QModelIndex &index,int role) const
{
  if((role==Qt::BackgroundRole)&&
     (SqlTableModel::data(index.row(),
			  columnCount()-1).toString()==tr("Yes"))) {
    return QVariant(Qt::green);
  }
  return SqlTableModel::data(index,role);
}


QVariant ImagesModel::data(int row,int column,int role) const
{
  if((role==Qt::BackgroundRole)&&
     (SqlTableModel::data(row,
			  columnCount()-1).toString()==tr("Yes"))) {
    return QVariant(Qt::green);
  }
  return SqlTableModel::data(row,column,role);
}

