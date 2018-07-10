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
#ifndef BAGPIPE_DURATIONSMODIFIER_H
#define BAGPIPE_DURATIONSMODIFIER_H

#include <memory>
#include <vector>
#include <QObject>
#include <QList>

#include "libmscore/score.h"
#include "libmscore/chord.h"

#include "../definitions/definitions.h"
#include "../gracenotes/embellishmentdefinitions.h"

namespace Ms {
using namespace Bagpipe;

class DurationsModifier;
using DurationsModifier_UPtr = unique_ptr<DurationsModifier>;

// Describes the played position of an embellishment relative to the start of its parent note.
enum /*class*/ PositionType /*: int*/ {
    Before = 1,  // end of embellishment will equal the end of the previous note, and the pervious note's duration will be decreased
    Varies, // where the embellishment is aligned is defined by each embellishment, and the parent note's and/or the previous note's duration will be decreased
    After  // start of embellishment will equal the start of the note, and the note's duration will be decreased
    };

//---------------------------------------------------------
//   DurationsModifier
//---------------------------------------------------------
#pragma clang diagnostic ignored "-Wpadded"
class DurationsModifier : public QObject
   {
#pragma clang diagnostic ignored "-Wunknown-warning-option"
   Q_OBJECT

public:
      DurationsModifier(const Definitions& definitions);
      ~DurationsModifier() = default;

      void setScore(Score* score);

      int goalTempoBpm() const { return static_cast<int>(_goalTempo_Bpm); }

      int practiceTempoRatio() const { return static_cast<int>(_practiceTempoFactor); }
      int practiceTempoBpm() const { return static_cast<int>(_practiceTempoFactor * _goalTempo_Bpm); }

      static DurationsModifier_UPtr make(const Definitions& definitions) { return DurationsModifier_UPtr(new DurationsModifier(definitions)); }

   public slots:
      void setBaseDuration(int duration);
      void setGoalTempoBpm(int tempoBpm);
      void setPracticeTempoPercent(int percent);
      void setPositionType(int type) { _positionType = static_cast<PositionType>(type); }
      void modifyScore();

private:
     static int _millichordsPerChord;
     static double _millisecondsPerSecond;

     Score*         _score = nullptr;
     double         _baseMillisecondsPerGraceNote = 50.0;
     double         _goalTempo_Bps = 0.0;
     double         _goalTempo_Bpm = 60.0;
     double         _practiceTempoFactor = 1.0;
     PositionType   _positionType = PositionType::After;

     const Definitions& _definitions;
     const EmbellishmentDefinitions& _embellishments;

     Element* selectedElement() const;
     const QList<Element*>* selectedElements() const;

     double millisecondsPerMillichord(Chord* chord) const;
     void setEvent(Chord* chord, int onTime, double length) const;
     void setOnTime(Chord* chord, int ontime) const;
     void setOffTime(Chord* chord, int offtime) const;
     void setAfterBeatAtStart(Chord* chord, vector<Chord*>& graceNotesPrev, vector<Chord*>& graceNotesThis) const;
     void setBeforeBeatAtEnd(Chord* chord, vector<Chord*>& graceNotesThis, vector<Chord*>& graceNotesNext) const;
     void modifyChord(Chord* chord, Chord* prevChord) const;
   };
} // namespace Ms
#endif // BAGPIPE_DURATIONSMODIFIER_H
