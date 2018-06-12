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

#ifndef __BAGPIPEASSISTANT_H__
#define __BAGPIPEASSISTANT_H__

#include "qobjectdefs.h"
#include "ui_bagpipeassistant.h"

namespace Ms {

class BagpipeDurations;
class Score;
class Element;
//class QSignalMapper;

class BagpipeAssistant : public QDockWidget
      {
      Q_OBJECT

   public:
      explicit BagpipeAssistant(QWidget* parent = 0);
      ~BagpipeAssistant();

      void update(Score* score);

   signals:
      void bagpipeAssistantVisible(bool);

   private:
      Ui::BagpipeAssistant  _ui;
      //QSignalMapper*    _signalMapper = 0;
      BagpipeDurations* _durations = 0;
      Score*            _score = 0;
      Element*          _element = 0;

   private slots:
      void on_beforeNoteStartButton_clicked();
      void on_variesNoteStartButton_clicked();
      void on_afterNoteStartButton_clicked();
   };

}// namespace Ms
#endif // __BAGPIPEASSISTANT_H__
