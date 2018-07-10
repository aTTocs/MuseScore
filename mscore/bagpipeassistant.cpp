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

#include "bagpipeassistant.h"

#include <QSignalMapper>
#include <QKeyEvent>

#include "libmscore/measure.h"
#include "libmscore/sig.h"
#include "libmscore/repeatlist.h"
#include "synthesizer/msynthesizer.h"
#include "mscore/seq.h"
#include "mscore/musescore.h"

#include "bagpipe/definitions/definitions.h"
#include "bagpipe/definitions/definitionsfilereader.h"
#include "bagpipe/ui/durationsmodifier.h"

namespace Ms {
using namespace Bagpipe;

//---------------------------------------------------------
//   BagpipeAssistant
//---------------------------------------------------------
BagpipeAssistant::BagpipeAssistant(QWidget* parent)
      : QWidget(parent, Qt::Dialog) //QDockWidget(parent)
      {
      setObjectName("bagpipeAssistant");
      setWindowTitle(tr("Bagpipe Assistant"));

      cachedTickPosition = -1;
      cachedTimePosition = -1;
      tempoSliderIsPressed = false;

      setupUi(this);

      setWindowFlags(Qt::Tool);
      setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

      MuseScore::restoreGeometry(this);

//      playButton->setDefaultAction(getAction("play"));
//      rewindButton->setDefaultAction(getAction("rewind"));
//      countInButton->setDefaultAction(getAction("countin"));
//      metronomeButton->setDefaultAction(getAction("metronome"));
//      loopButton->setDefaultAction(getAction("loop"));
//      loopInButton->setDefaultAction(getAction("loop-in"));
//      loopOutButton->setDefaultAction(getAction("loop-out"));
      enablePlay = new EnablePlayForWidget(this);

//      tempoSlider->setDclickValue1(100.0);
//      tempoSlider->setDclickValue2(100.0);
//      tempoSlider->setUseActualValue(true);

//      mgainSlider->setValue(seq->metronomeGain());
//      mgainSlider->setDclickValue1(seq->metronomeGain() - 10.75f);
//      mgainSlider->setDclickValue2(seq->metronomeGain() - 10.75f);

      _definitions = Definitions::make();
      QFile* file = DefinitionsFileReader::getFile("bagpipeDefinitions.xml");
      DefinitionsFileReader(file, _definitions);
      delete file;

      // Durations
      _durationsModifier = DurationsModifier::make(*_definitions);

      connect(modifyDurationsPushButton, SIGNAL(clicked()), _durationsModifier.get(), SLOT(modifyScore()));
      connect(baseDurationSpinBox, SIGNAL(valueChanged(int)), _durationsModifier.get(), SLOT(setBaseDuration(int)));
      connect(goalTempoSpinBox, SIGNAL(valueChanged(int)), _durationsModifier.get(), SLOT(setGoalTempoBpm(int)));
      connect(practiceTempoSpinBox, SIGNAL(valueChanged(int)), _durationsModifier.get(), SLOT(setPracticeTempoPercent(int)));

      _signalMapper = shared_ptr<QSignalMapper>( new QSignalMapper(this) );
      connect(beforeNoteStartRadioButton, SIGNAL(clicked()), _signalMapper.get(), SLOT(map()));
      connect(variesNoteStartRadioButton, SIGNAL(clicked()), _signalMapper.get(), SLOT(map()));
      connect(afterNoteStartRadioButton, SIGNAL(clicked()), _signalMapper.get(), SLOT(map()));
      _signalMapper->setMapping(beforeNoteStartRadioButton, Before);
      _signalMapper->setMapping(variesNoteStartRadioButton, Varies);
      _signalMapper->setMapping(afterNoteStartRadioButton, After);
      connect(_signalMapper.get(), SIGNAL(mapped(int)), _durationsModifier.get(), SLOT(setPositionType(int)));

      QStringList items{ tr("Strong"), tr("Medium"), tr("Weak") };
      beatStress1ComboBox->addItems( items );
      beatStress2ComboBox->addItems( items );
      beatStress3ComboBox->addItems( items );
      beatStress4ComboBox->addItems( items );

      // Exercises
      _scoreMaker = ScoreMaker::make(*_definitions);

      _scoreMaker->populateTree(embellishmentsView);


//      connect(volumeSlider, SIGNAL(valueChanged(double,int)), SLOT(volumeChanged(double,int)));
//      connect(mgainSlider,  SIGNAL(valueChanged(double,int)), SLOT(metronomeGainChanged(double,int)));
//      connect(posSlider,    SIGNAL(sliderMoved(int)),         SLOT(setPos(int)));
//      connect(tempoSlider,  SIGNAL(valueChanged(double,int)), SLOT(relTempoChanged(double,int)));
//      connect(tempoSlider,  SIGNAL(sliderPressed(int)),       SLOT(tempoSliderPressed(int)));
//      connect(tempoSlider,  SIGNAL(sliderReleased(int)),      SLOT(tempoSliderReleased(int)));
//      connect(relTempoBox,  SIGNAL(valueChanged(double)),     SLOT(relTempoChanged()));
//      connect(seq,          SIGNAL(heartBeat(int,int,int)),   SLOT(heartBeat(int,int,int)));

      setScore(nullptr);
      }

//---------------------------------------------------------
//   ~BagpipeAssistant
//---------------------------------------------------------
BagpipeAssistant::~BagpipeAssistant()
      {
      // if widget is visible, store geometry and pos into settings
      // if widget is not visible/closed, pos is not reliable (and anyway
      // has been stored into settings when the widget has been hidden)
      if (isVisible()) {
            MuseScore::saveGeometry(this);
            }
      }

//---------------------------------------------------------
//   relTempoChanged
//---------------------------------------------------------
void BagpipeAssistant::relTempoChanged(double d, int)
      {
      double relTempo = d * .01;
      emit relTempoChanged(relTempo);
      // Snap tempo slider to 100% when it gets close
      if (relTempo < 1.01 && relTempo > 0.99) {
            relTempo = 1.00;
            }
      setTempo(seq->curTempo() * relTempo);
      setRelTempo(relTempo);
      }

//---------------------------------------------------------
//   relTempoChanged
//---------------------------------------------------------
void BagpipeAssistant::relTempoChanged()
      {
//      double v = relTempoBox->value();
//      tempoSlider->setValue(v);
//      relTempoChanged(v, 0);
      }

//---------------------------------------------------------
//   closeEvent
//
//    Called when the BagpipeAssistant is closed with its own button
//    but not when it is hidden with the main menu command
//---------------------------------------------------------
void BagpipeAssistant::closeEvent(QCloseEvent* ev)
      {
      emit closed(false);
      QWidget::closeEvent(ev);
      }

//---------------------------------------------------------
//   hideEvent
//
//    Called both when the BagpipeAssistant is closed with its own button and
//    when it is hidden via the main menu command
//
//    Stores widget geometry and position into settings.
//---------------------------------------------------------
void BagpipeAssistant::hideEvent(QHideEvent* ev)
      {
      MuseScore::saveGeometry(this);
      QWidget::hideEvent(ev);
      }

//---------------------------------------------------------
//   showEvent
//---------------------------------------------------------
void BagpipeAssistant::showEvent(QShowEvent* e)
      {
      enablePlay->showEvent(e);
      QWidget::showEvent(e);
      activateWindow();
      setFocus();
      }
//---------------------------------------------------------
//   eventFilter
//---------------------------------------------------------
bool BagpipeAssistant::eventFilter(QObject* obj, QEvent* e)
      {
      if (enablePlay->eventFilter(obj, e))
            return true;
      return QWidget::eventFilter(obj, e);
      }

//---------------------------------------------------------
//   keyPressEvent
//---------------------------------------------------------
void BagpipeAssistant::keyPressEvent(QKeyEvent* ev) {
      if (ev->key() == Qt::Key_Escape && ev->modifiers() == Qt::NoModifier) {
            close();
            return;
            }
      QWidget::keyPressEvent(ev);
      }

//---------------------------------------------------------
//   setScore
//---------------------------------------------------------
void BagpipeAssistant::setScore(Score* score)
      {
      if (_score && _score == score)
            return;

      _score = score;
      _durationsModifier->setScore(_score);

//      changeSelection(SelState::NONE);

      goalTempoSpinBox->setValue(_durationsModifier->goalTempoBpm());

//      bool enable = _score != nullptr;
//      volumeSlider->setEnabled(enable);
//      posSlider->setEnabled(enable);
//      tempoSlider->setEnabled(enable);

      if (_score && seq && seq->canStart()) {
            setTempo(_score->tempomap()->tempo(0));
            setRelTempo(_score->tempomap()->relTempo());
            setEndpos(_score->repeatList()->ticks());
            int tick = _score->pos(POS::CURRENT);
            heartBeat(tick, tick, 0);
            }
      else {
            setTempo(2.0);
            setRelTempo(1.0);
            setEndpos(0);
            heartBeat(0, 0, 0);
            updatePosLabel(0);
            }

      update();
      }

//---------------------------------------------------------
//   setEndpos
//---------------------------------------------------------
void BagpipeAssistant::setEndpos(int /*val*/)
      {
//      posSlider->setRange(0, val);
      }

//---------------------------------------------------------
//   setTempo
//---------------------------------------------------------
void BagpipeAssistant::setTempo(double /*val*/)
      {
//      int tempo = lrint(val * 60.0);
//      tempoLabel->setText(QString("%1 BPM").arg(tempo, 3, 10, QLatin1Char(' ')));
      }

//---------------------------------------------------------
//   setRelTempo
//---------------------------------------------------------
void BagpipeAssistant::setRelTempo(qreal /*val*/)
      {
//      val *= 100;
//      relTempoBox->setValue(val);
//      tempoSlider->setValue(val);
      }

//---------------------------------------------------------
//   setGain
//---------------------------------------------------------
void BagpipeAssistant::setGain(float /*val*/)
      {
//      volumeSlider->setValue(val);
      }

//---------------------------------------------------------
//   volumeChanged
//---------------------------------------------------------
void BagpipeAssistant::volumeChanged(double val, int)
      {
      emit gainChange(static_cast<float>(val));
      }

//---------------------------------------------------------
//   metronomeGainChanged
//---------------------------------------------------------
void BagpipeAssistant::metronomeGainChanged(double val, int)
      {
      emit metronomeGainChanged(static_cast<float>(val));
      }

//---------------------------------------------------------
//    setPos
//---------------------------------------------------------
void BagpipeAssistant::setPos(int utick)
      {
      if (!_score)
            return;

      if (cachedTickPosition != utick)
            emit posChange(utick);

      updatePosLabel(utick);
      updateTimeLabel(static_cast<int>(_score->utick2utime(utick)));
      }

//---------------------------------------------------------
//   heartBeat
//---------------------------------------------------------
void BagpipeAssistant::heartBeat(int /*tick*/, int utick, int samples)
      {
      if (cachedTickPosition != utick) {
            updatePosLabel(utick);
//            posSlider->setValue(utick);
            }

      int sec = samples / MScore::sampleRate;
      if (sec == cachedTimePosition)
            return;

      updateTimeLabel(sec);
      }

//---------------------------------------------------------
//   changeSelection
//---------------------------------------------------------
void BagpipeAssistant::changeSelection(Selection /*selection*/)
      {
      // From pianotools.cpp
//      for (PianoKeyItem* key : keys) {
//            key->setHighlighted(false);
//            key->setSelected(false);
//            }
//      for (Note* n : selection.uniqueNotes()) {
//            keys[n->pitch() - _firstKey]->setSelected(true);
//            for (Note* other : n->chord()->notes())
//                  keys[other->pitch() - _firstKey]->setHighlighted(true);
//            }
//      for (PianoKeyItem* key : keys)
//            key->update();

      // From drumroll.cpp
//      gv->scene()->blockSignals(true);
//      gv->scene()->clearSelection();
//      QList<QGraphicsItem*> il = gv->scene()->items();
//      foreach(QGraphicsItem* item, il) {
//            Note* note = static_cast<Note*>(item->data(0).value<void*>());
//            if (note)
//                  item->setSelected(note->selected());
//            }
//      gv->scene()->blockSignals(false);
      }

//---------------------------------------------------------
//   updateTime
//---------------------------------------------------------
void BagpipeAssistant::updateTimeLabel(int sec)
      {
      cachedTimePosition = sec;
      int m              = sec / 60;
      sec                = sec % 60;
      int h              = m / 60;
      m                  = m % 60;
      char buffer[32];
      sprintf(buffer, "%d:%02d:%02d", h, m, sec);
//      timeLabel->setText(QString(buffer));
      }

//---------------------------------------------------------
//   updatePos
//---------------------------------------------------------
void BagpipeAssistant::updatePosLabel(int utick)
      {
      cachedTickPosition = utick;
      int bar = 0;
      int beat = 0;
      int t = 0;
      int tick = 0;

      if (_score) {
            tick = _score->repeatList()->utick2tick(utick);
            _score->sigmap()->tickValues(tick, &bar, &beat, &t);
            double tpo = _score->tempomap()->tempo(tick) * _score->tempomap()->relTempo();
            setTempo(tpo);
            }

      char buffer[32];
      sprintf(buffer, "%03d.%02d", bar+1, beat+1);
//      posLabel->setText(QString(buffer));
      }

//---------------------------------------------------------
//   tempoSliderPressed
//---------------------------------------------------------
void BagpipeAssistant::tempoSliderPressed(int)
      {
      tempoSliderIsPressed = true;
      }

//---------------------------------------------------------
//   tempoSliderReleased
//---------------------------------------------------------
void BagpipeAssistant::tempoSliderReleased(int)
      {
      tempoSliderIsPressed = false;
      }

//---------------------------------------------------------
//   changeEvent
//---------------------------------------------------------
void BagpipeAssistant::changeEvent(QEvent *event)
      {
      QWidget::changeEvent(event);
      if (event->type() == QEvent::LanguageChange)
            retranslate();
      }

//---------------------------------------------------------
//   on_tabWidget_currentChanged
//---------------------------------------------------------
void Ms::BagpipeAssistant::on_tabWidget_currentChanged(int /*index*/)
      {

      }
}
