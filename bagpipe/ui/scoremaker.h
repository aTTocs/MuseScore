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
#pragma once
#ifndef BAGPIPE_SCOREMAKER_H
#define BAGPIPE_SCOREMAKER_H

#include <memory>
#include <QObject>
#include <QTableView>
#include <QTreeView>

#include "../definitions/definitions.h"
#include "../gracenotes/embellishmentdefinitions.h"
#include "treemodel.h"

namespace Bagpipe {

class Score;
class Element;
class Chord;

class ScoreMaker;
using ScoreMaker_UPtr = unique_ptr<ScoreMaker>;

//---------------------------------------------------------
//   ScoreMaker
//---------------------------------------------------------
class ScoreMaker : public QObject
      {
#pragma clang diagnostic ignored "-Wunknown-warning-option"
      Q_OBJECT
public:
      ScoreMaker(const Definitions& definitions);
      ~ScoreMaker() = default;

      //void populateTable(QTableView* table);
      void populateTree(QTreeView* tree);

      //void setScore(Score* score);

      static ScoreMaker_UPtr make(const Definitions& definitions) { return ScoreMaker_UPtr(new ScoreMaker(definitions)); }

private:
      //Score* _score = nullptr;
      const Definitions& _definitions;
      const EmbellishmentDefinitions& _embellishments;

      //unique_ptr<MyModel> _model = nullptr;
      unique_ptr<QStandardItemModel> _model = nullptr;
      QList<QStandardItem*> prepareRow(const QString& first, const QString& second, const QString& third);
   };
} // namespace Bagpipe
#endif // BAGPIPE_SCOREMAKER_H
