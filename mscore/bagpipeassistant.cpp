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

//#include "QSignalMapper.h"
#include "musescore.h"
#include "bagpipeassistant.h"
#include "bagpipedurations.h"

namespace Ms {

//---------------------------------------------------------
//   showBagpipeAssistant
//---------------------------------------------------------
void MuseScore::showBagpipeAssistant(bool visible)
      {
      QAction* action = getAction("bagpipeassistant");
      if (visible)
            if (!bagpipeAssistant) {
                  bagpipeAssistant = new BagpipeAssistant();
                  connect(bagpipeAssistant, SIGNAL(bagpipeAssistantVisible(bool)), action, SLOT(setChecked(bool)));
                  addDockWidget(Qt::RightDockWidgetArea, bagpipeAssistant);
                  }

      if (bagpipeAssistant) {
            bagpipeAssistant->setVisible(visible);
            if (visible)
                  bagpipeAssistant->update(cs);
            }

      action->setChecked(visible);
      }

//---------------------------------------------------------
//   BagpipeAssistant
//---------------------------------------------------------
BagpipeAssistant::BagpipeAssistant(QWidget* parent) : QDockWidget(parent)
      {
      setObjectName("bagpipeAssistant");
      setAllowedAreas(Qt::DockWidgetAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea));
      setWindowTitle(tr("Bagpipe Assistant"));

      QWidget* mainWidget = new QWidget;
      mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      setWidget(mainWidget);

      _ui.setupUi(mainWidget);

      _durations = new BagpipeDurations();

      connect(_ui.modifyDurationsPushButton, SIGNAL(clicked()), _durations, SLOT(modifyScore()));

      connect(_ui.baseDurationSpinBox, SIGNAL(valueChanged(int)), _durations, SLOT(setBaseDuration(int)));
      connect(_ui.goalTempoSpinBox, SIGNAL(valueChanged(int)), _durations, SLOT(setGoalTempoBpm(int)));
      connect(_ui.practiceTempoSpinBox, SIGNAL(valueChanged(int)), _durations, SLOT(setPracticeTempoPercent(int)));

      connect(_ui.beforeNoteStartRadioButton, SIGNAL(clicked()), SLOT(on_beforeNoteStartButton_clicked()));
      connect(_ui.variesNoteStartRadioButton, SIGNAL(clicked()), SLOT(on_variesNoteStartButton_clicked()));
      connect(_ui.afterNoteStartRadioButton, SIGNAL(clicked()), SLOT(on_afterNoteStartButton_clicked()));
//      _signalMapper = new QSignalMapper(this);
//      connect(_ui.beforeBeatRadioButton, SIGNAL(clicked()), _signalMapper, SLOT(map()));
//      connect(_ui.variedBeatRadioButton, SIGNAL(clicked()), _signalMapper, SLOT(map()));
//      connect(_ui.afterBeatRadioButton, SIGNAL(clicked()), _signalMapper, SLOT(map()));
//      _signalMapper.setMapping(_ui.beforeBeatRadioButton, PositionType::Before);
//      _signalMapper.setMapping(_ui.variedBeatRadioButton, PositionType::Varies);
//      _signalMapper.setMapping(_ui.afterBeatRadioButton, PositionType::After);
//      connect(_signalMapper, SIGNAL(mapped(int)), _assistant, SLOT(setPositionType(int)));

      QStringList items{ tr("Strong"), tr("Medium"), tr("Weak") };
      _ui.beatStress1ComboBox->addItems( items );
      _ui.beatStress2ComboBox->addItems( items );
      _ui.beatStress3ComboBox->addItems( items );
      _ui.beatStress4ComboBox->addItems( items );
      }

//---------------------------------------------------------
//   ~BagpipeAssistant
//---------------------------------------------------------
BagpipeAssistant::~BagpipeAssistant()
      {
      delete _durations;
      //delete _signalMapper;
      }

//---------------------------------------------------------
//   update
//---------------------------------------------------------
void BagpipeAssistant::update(Score* score)
      {
      _durations->setScore(score);

      _ui.goalTempoSpinBox->setValue(_durations->goalTempoBpm());
      }

void BagpipeAssistant::on_beforeNoteStartButton_clicked()
      {
      _durations->setPositionType(PositionType::Before);
      }

void BagpipeAssistant::on_variesNoteStartButton_clicked()
      {
      _durations->setPositionType(PositionType::Varies);
      }

void BagpipeAssistant::on_afterNoteStartButton_clicked()
      {
      _durations->setPositionType(PositionType::After);
      }
}
