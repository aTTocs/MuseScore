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

#ifndef __BAGPIPEDURATIONS_H__
#define __BAGPIPEDURATIONS_H__

namespace Ms {

class Score;
class Element;
class Chord;

// Describes the played position of an embellishment relative to the start of its parent note.
enum class PositionType : int {
    Before,  // end of embellishment will equal the end of the previous note, and the pervious note's duration will be decreased
    Varies, // where the embellishment is aligned is defined by each embellishment, and the parent note's and/or the previous note's duration will be decreased
    After  // start of embellishment will equal the start of the note, and the note's duration will be decreased
    };

//---------------------------------------------------------
//   BagpipeDurations
//---------------------------------------------------------
class BagpipeDurations : public QObject
   {
   Q_OBJECT

   public:
      BagpipeDurations();

      void setScore(Score* score);

      int goalTempoBpm() const { return _goalTempo_Bpm; }

      int practiceTempoRatio() const { return _practiceTempoFactor; }
      int practiceTempoBpm() const { return _practiceTempoFactor * _goalTempo_Bpm; }

   public slots:
      void setBaseDuration(int duration);
      void setGoalTempoBpm(int tempoBpm);
      void setPracticeTempoPercent(int percent);
      void setPositionType(PositionType type) { _positionType = type; }
      void modifyScore();

   private:
     static int _millichordsPerChord;
     static int _millisecondsPerSecond;

     Score*         _score = 0;
     int            _baseMillisecondsPerGraceNote = 50;
     qreal          _goalTempo_Bps = 0.0;
     int            _goalTempo_Bpm = 60;
     qreal          _practiceTempoFactor = 1.0;
     PositionType   _positionType = PositionType::After;

     Element* selectedElement() const;
     const QList<Element*>* selectedElements() const;

     float millisecondsPerMillichord(Chord* chord) const;
     void setEvent(Chord* chord, int onTime, int length) const;
     void setOnTime(Chord* chord, int ontime) const;
     void setOffTime(Chord* chord, int offtime) const;
     void setAfterBeatAtStart(Chord* chord, QVector<Chord*>& graceNotesPrev, QVector<Chord*>& graceNotesThis) const;
     void setBeforeBeatAtEnd(Chord* chord, QVector<Chord*>& graceNotesThis, QVector<Chord*>& graceNotesNext) const;
     void modifyChord(Chord* chord, Ms::Chord* prevChord) const;
   };
}
#endif // __BAGPIPEDURATIONS_H__
