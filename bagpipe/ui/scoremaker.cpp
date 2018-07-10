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

#include "scoremaker.h"

//#include "musescore.h"
//#include "libmscore/chord.h"
//#include "libmscore/chordrest.h"
//#include "libmscore/measure.h"
//#include "libmscore/scoreElement.h"
//#include "libmscore/tempo.h"
//#include "libmscore/tie.h"
//#include "libmscore/undo.h"

#include "../gracenotes/embellishment.h"
#include "../gracenotes/embellishmentdefinitions.h"
#include "treemodel.h"

namespace Bagpipe {

//---------------------------------------------------------
//   ScoreMaker
//---------------------------------------------------------
ScoreMaker::ScoreMaker(const Definitions& definitions)
      : _definitions(definitions)
      , _embellishments(_definitions.embellishments())
      {}

//void ScoreMaker::populateTable(QTableView* table)
//      {
//      _model = unique_ptr<MyModel>(new MyModel(nullptr));
//      table->setModel(_model.get());
//      table->show();
//      }

//---------------------------------------------------------
//   populateTree
//---------------------------------------------------------
void ScoreMaker::populateTree(QTreeView* treeView)
      {
      _model = unique_ptr<QStandardItemModel>(new QStandardItemModel());

//      tree->setModel(_model.get());
//      tree->show();


      QList<QStandardItem*> preparedRow = prepareRow("first", "second", "third");
      QStandardItem* item = _model->invisibleRootItem();
      // adding a row to the invisible root item produces a root element
      item->appendRow(preparedRow);

      QList<QStandardItem*> secondRow =prepareRow("111", "222", "333");
      // adding a row to an item starts a subtree
      preparedRow.first()->appendRow(secondRow);

      treeView->setModel(_model.get());
      treeView->expandAll();
      }

QList<QStandardItem *> ScoreMaker::prepareRow(const QString& first,
                                                const QString& second,
                                                const QString& third)
{
    QList<QStandardItem*> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}

////---------------------------------------------------------
////   setScore
////---------------------------------------------------------
//void ScoreMaker::setScore(Score* score)
//      {
//      _score = score;

//      if (!_score) {
//            // TBD clear stuff
//            return;
//            }
//      }
} // namespace Bagpipe
