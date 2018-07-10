//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2018 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENSE.GPL
//=============================================================================

#include "treemodel.h"

#include <QBrush>
#include <QDebug>
#include <QFont>
#include <QStringList>
#include <QTime>

#include "treeitem.h"

namespace Bagpipe {

xMyModel::xMyModel(QObject *parent)
    :QStandardItemModel(parent)
      {
//      _timer = unique_ptr<QTimer>(new QTimer(this));
//      _timer->setInterval(1000);
//      connect(_timer.get(), SIGNAL(timeout()) , this, SLOT(timerHit()));
//      _timer->start();
      }

//int MyModel::rowCount(const QModelIndex & /*parent*/) const
//{
//   return 2;
//}

//int MyModel::columnCount(const QModelIndex & /*parent*/) const
//{
//    return 3;
//}

//QVariant MyModel::data(const QModelIndex &index, int role) const
//{
//      int row = index.row();
//      int col = index.column();
//      // generate a log message when this method gets called
//      qDebug() << QString("row %1, col%2, role %3")
//              .arg(row).arg(col).arg(role);

//      switch(role){
//      case Qt::DisplayRole:
//          if (row == 0 && col == 0) return QTime::currentTime().toString();
//          if (row == 0 && col == 1) return QString("<--left");
//          if (row == 1 && col == 1) return QString("right-->");

//          return QString("Row%1, Column%2")
//                  .arg(row + 1)
//                  .arg(col +1);
//          //break;
//      case Qt::FontRole:
//          if (row == 0 && col == 0) //change font only for cell(0,0)
//          {
//              QFont boldFont;
//              boldFont.setBold(true);
//              return boldFont;
//          }
//          break;
//      case Qt::BackgroundRole:

//          if (row == 1 && col == 2)  //change background only for cell(1,2)
//          {
//              QBrush redBackground(Qt::red);
//              return redBackground;
//          }
//          break;
//      case Qt::TextAlignmentRole:

//          if (row == 1 && col == 1) //change text alignment only for cell(1,1)
//          {
//              return Qt::AlignRight + Qt::AlignVCenter;
//          }
//          break;
//      case Qt::CheckStateRole:

//          if (row == 1 && col == 0) //add a checkbox to cell(1,0)
//          {
//              return Qt::Checked;
//          }
//      }
//      return QVariant();
//}

//void MyModel::timerHit()
//      {
//      //we identify the top left cell
//      QModelIndex topLeft = createIndex(0, 0);
//      //emit a signal to make the view reread identified data
//      emit dataChanged(topLeft, topLeft);
//      }
} // namespace Bagpipe
