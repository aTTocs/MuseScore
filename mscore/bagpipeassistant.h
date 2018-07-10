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
#ifndef BAGPIPE_BAGPIPEASSISTANT_H
#define BAGPIPE_BAGPIPEASSISTANT_H

#include <memory>
#include <QDockWidget>
#include <QSignalMapper>
#include "qobjectdefs.h"

#include "libmscore/score.h"
#include "libmscore/select.h"
#include "mscore/enableplayforwidget.h"

#include "ui_bagpipeassistant.h"
#include "bagpipe/ui/durationsmodifier.h"
#include "bagpipe/ui/scoremaker.h"

namespace Ms {
using namespace Bagpipe;

//---------------------------------------------------------
//   BagpipeAssistant
//---------------------------------------------------------
class BagpipeAssistant : public QWidget, private Ui::BagpipeAssistantBase //QDockWidget
      {
#pragma clang diagnostic ignored "-Wunknown-warning-option"
      Q_OBJECT

public:
      explicit BagpipeAssistant(QWidget* parent = nullptr);
      virtual ~BagpipeAssistant() override;

      void setScore(Score* score);

      void setTempo(double);
      void setRelTempo(qreal);

      void setEndpos(int);
      bool isTempoSliderPressed() {return tempoSliderIsPressed;}
      void changeSelection(Selection selection);

   signals:
      void relTempoChanged(double);
      void metronomeGainChanged(float);
      void posChange(int);
      void gainChange(float);
      void closed(bool);

   public slots:
      void setGain(float);
      void setPos(int);
      void heartBeat(int rpos, int apos, int samples);

protected:
      virtual void changeEvent(QEvent *event) override;
      void retranslate()  { retranslateUi(this); }

private:
      shared_ptr<QSignalMapper> _signalMapper = nullptr;

      int cachedTickPosition;
      int cachedTimePosition;
      bool tempoSliderIsPressed;
      EnablePlayForWidget* enablePlay = nullptr;

      Definitions_SPtr _definitions = nullptr;
      DurationsModifier_UPtr _durationsModifier = nullptr;
      ScoreMaker_UPtr _scoreMaker = nullptr;

      Score* _score = nullptr;

      virtual void closeEvent(QCloseEvent*) override;
      virtual void hideEvent (QHideEvent* event) override;
      virtual void showEvent(QShowEvent *) override;
      virtual bool eventFilter(QObject *, QEvent *) override;
      virtual void keyPressEvent(QKeyEvent*) override;
      void updateTimeLabel(int sec);
      void updatePosLabel(int utick);

private slots:
      void volumeChanged(double, int);
      void metronomeGainChanged(double val, int);
      void relTempoChanged(double, int);
      void relTempoChanged();
      void tempoSliderReleased(int);
      void tempoSliderPressed(int);

      void on_tabWidget_currentChanged(int index);
      };
} // namespace Ms
#endif // BAGPIPE_BAGPIPEASSISTANT_H
