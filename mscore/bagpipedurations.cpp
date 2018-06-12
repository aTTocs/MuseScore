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

#include "musescore.h"
#include "libmscore/chord.h"
#include "libmscore/chordrest.h"
#include "libmscore/measure.h"
#include "libmscore/scoreElement.h"
#include "libmscore/tempo.h"
#include "libmscore/tie.h"
#include "libmscore/undo.h"
#include "bagpipedurations.h"

#define Constrain(x, min, max) (x = x < min ? min : (x > max ? max : x))

namespace Ms {

int BagpipeDurations::_millichordsPerChord = 1000; // See NoteEvent - "1/1000 of nominal note len"
int BagpipeDurations::_millisecondsPerSecond = 1000;

#pragma GCC diagnostic push // TEMP
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

//---------------------------------------------------------
//   BagpipeDurations
//---------------------------------------------------------
BagpipeDurations::BagpipeDurations()
      {
      }

//---------------------------------------------------------
//   setScore
//---------------------------------------------------------
void BagpipeDurations::setScore(Score* score)
      {
      _score = score;

      if (!_score) {
            // TBD clear stuff
            return;
            }

      // Set Tempo
      TempoMap::const_iterator iter = _score->tempomap()->begin();
      for (;iter != _score->tempomap()->begin(); iter++) {
            int tick = iter->first;
            TEvent event = iter->second;

            _goalTempo_Bps = event.tempo;
            _goalTempo_Bpm = 60 * _goalTempo_Bps;
            }
      }

//---------------------------------------------------------
//   setBaseDuration
//---------------------------------------------------------
void BagpipeDurations::setBaseDuration(int duration)
      {
      Constrain(duration, 1, 500 );

      _baseMillisecondsPerGraceNote = duration;
      }

//---------------------------------------------------------
//   setGoalTempoBpm
//---------------------------------------------------------
void BagpipeDurations::setGoalTempoBpm(int tempoBpm)
      {
      Constrain(tempoBpm, 10, 200);

      _goalTempo_Bpm = tempoBpm;
      _goalTempo_Bps = tempoBpm / 60.0;
      }

//---------------------------------------------------------
//   setPracticeTempoPercent
//---------------------------------------------------------
void BagpipeDurations::setPracticeTempoPercent(int percent)
      {
      Constrain(percent, 10, 200);

      _practiceTempoFactor = percent / 100.0;
      }

//---------------------------------------------------------
//   selectedElement
//---------------------------------------------------------
Element* BagpipeDurations::selectedElement() const
      {
      return selectedElements() && !selectedElements()->empty() ? (*selectedElements())[0] : 0;
      }

//---------------------------------------------------------
//   selectedElements
//---------------------------------------------------------
const QList<Element*>* BagpipeDurations::selectedElements() const
      {
      return _score ? &_score->selection().elements() : 0;
      }

//---------------------------------------------------------
//   millisecondsPerMillichord
//---------------------------------------------------------
float BagpipeDurations::millisecondsPerMillichord(Chord* chord) const
      {
      if (!chord)
            return 0.0;

      float quarterNotesPerBeat = 1.5; // 6/8

      float practice_BeatsPerSecond = _practiceTempoFactor * _goalTempo_Bps;

      float millisecondsPerBeat = _millisecondsPerSecond / practice_BeatsPerSecond;

      float ticksPerChord = 480.0;
      switch (chord->durationType().type()) {
            case TDuration::DurationType::V_BREVE:   ticksPerChord = 3840.0; break;
            case TDuration::DurationType::V_WHOLE:   ticksPerChord = 1920.0; break;
            case TDuration::DurationType::V_HALF:    ticksPerChord = 960.0; break;
            case TDuration::DurationType::V_QUARTER: ticksPerChord = 480.0; break;
            case TDuration::DurationType::V_EIGHTH:  ticksPerChord = 240.0; break;
            case TDuration::DurationType::V_16TH:    ticksPerChord = 120.0; break;
            case TDuration::DurationType::V_32ND:    ticksPerChord = 60.0; break;
            case TDuration::DurationType::V_64TH:    ticksPerChord = 30.0; break;
            case TDuration::DurationType::V_128TH:   ticksPerChord = 15.0; break;
            case TDuration::DurationType::V_256TH:   ticksPerChord = 7.5; break;
            case TDuration::DurationType::V_512TH:   ticksPerChord = 3.75; break;
            case TDuration::DurationType::V_1024TH:  ticksPerChord = 1.875; break;
            case TDuration::DurationType::V_ZERO:    ticksPerChord = 0.0; break;

            case TDuration::DurationType::V_LONG:
            case TDuration::DurationType::V_MEASURE:
            case TDuration::DurationType::V_INVALID:
                  return 0.0;
            }

      switch (chord->dots()) {
            case 1: ticksPerChord *= 1.5; break;
            case 2: ticksPerChord *= 1.75; break;
            case 3: ticksPerChord *= 1.875; break;
            case 4: ticksPerChord *= 1.9375; break;

            default:
                  break;
            }

      float quarterNotesPerChord = ticksPerChord / 480.0;

      float beatsPerChord = quarterNotesPerChord / quarterNotesPerBeat;
      float millisecondsPerChord = beatsPerChord * millisecondsPerBeat;

      return millisecondsPerChord / _millichordsPerChord;
      }

//---------------------------------------------------------
//   setEvent
//---------------------------------------------------------
void BagpipeDurations::setEvent(Chord* chord, int onTime, int length) const
      {
      if (!chord)
            return;

      chord->setPlayEventType(PlayEventType::User);

      for (Note* note : chord->notes()) {
            for (NoteEvent& event : note->playEvents()) {
                  NoteEvent newEvent = event;
                  newEvent.setOntime(onTime);
                  newEvent.setLen(length);

                  _score->undo(new ChangeNoteEvent(note, &event, newEvent));
                  }
            }
      }

//---------------------------------------------------------
//   setOnTime
//---------------------------------------------------------
void BagpipeDurations::setOnTime(Chord* chord, int ontime) const
      {
      if (!chord)
            return;

      for (Note* note : chord->notes()) {
            for (NoteEvent& event : note->playEvents()) {
                  NoteEvent newEvent = event;

                  int offtime = _millichordsPerChord;

                  newEvent.setOntime(ontime);
                  newEvent.setLen(offtime - ontime);

                  _score->undo(new ChangeNoteEvent(note, &event, newEvent));
                  }
            }
      }

//---------------------------------------------------------
//   setOffTime
//---------------------------------------------------------
void BagpipeDurations::setOffTime(Chord* chord, int offtime) const
      {
      if (!chord)
            return;

      for (Note* note : chord->notes()) {
            for (NoteEvent& event : note->playEvents()) {
                  NoteEvent newEvent = event;
                  // keep event.ontime() as is.

                  // set length to offtime - ontime
                  newEvent.setLen(offtime - event.ontime());

                  _score->undo(new ChangeNoteEvent(note, &event, newEvent));
                  }
            }
      }

//---------------------------------------------------------
//   setAfterBeatAtStart
//---------------------------------------------------------
void BagpipeDurations::setAfterBeatAtStart(Chord* chord, QVector<Chord*>& graceNotesPrev, QVector<Chord*>& graceNotesThis) const
      {
      if (!chord)
            return;

      if (graceNotesPrev.isEmpty() && graceNotesThis.isEmpty())
            setOnTime(chord, 0);

      Chord* prevChord = 0;

      int onTimePrev = _millichordsPerChord;
      int onTimeThis = 0;

      int millichordsPerGraceNoteThis = 0;

      // Set durations of previous chord's grace notes, with on time starting at _millichordsPerChord(1000)
      for (auto iter = graceNotesPrev.begin(); iter != graceNotesPrev.end(); ++iter) {
            Chord* graceChord = *iter;

            prevChord = toChord(graceChord->parent());

            int millichordsPerGraceNotePrev = _baseMillisecondsPerGraceNote / millisecondsPerMillichord(prevChord);
            millichordsPerGraceNoteThis = _baseMillisecondsPerGraceNote / millisecondsPerMillichord(chord);

            setEvent(graceChord, onTimePrev, millichordsPerGraceNotePrev);

            onTimePrev += millichordsPerGraceNotePrev;
            onTimeThis += millichordsPerGraceNoteThis;
            }

      // Set duration of previous chord, by setting off time to _millichordsPerChord(1000)
      setOffTime(prevChord, _millichordsPerChord);

      // Set duration of chord's grace notes
      for (auto iter = graceNotesThis.begin(); iter != graceNotesThis.end(); ++iter) {
            Chord* graceChord = *iter;

            millichordsPerGraceNoteThis = _baseMillisecondsPerGraceNote / millisecondsPerMillichord(chord);

            setEvent(graceChord, onTimeThis, millichordsPerGraceNoteThis);

            onTimeThis += millichordsPerGraceNoteThis;
            }

      // Set duration of chord, but setting its on time
      setOnTime(chord, onTimeThis);
      }

//---------------------------------------------------------
//   setBeforeBeatAtEnd
//---------------------------------------------------------
void BagpipeDurations::setBeforeBeatAtEnd(Chord* chord, QVector<Chord*>& graceNotesThis, QVector<Chord*>& graceNotesNext) const
      {
      if (!chord)
            return;

      if (graceNotesThis.isEmpty() && graceNotesNext.isEmpty())
            setOffTime(chord, _millichordsPerChord);

      int onTimeThis = _millichordsPerChord;
      int onTimeNext = 0;

      Chord* nextChord = 0;

      int millichordsPerGraceNoteThis = 0;

      // Set durations of next chord's grace notes, in reverse
      for (auto iter = graceNotesNext.rbegin(); iter != graceNotesNext.rend(); ++iter) {
            Chord* graceChord = *iter;

            nextChord = toChord(graceChord->parent());

            millichordsPerGraceNoteThis = _baseMillisecondsPerGraceNote / millisecondsPerMillichord(chord);
            int millichordsPerGraceNoteNext = _baseMillisecondsPerGraceNote / millisecondsPerMillichord(nextChord);

            onTimeThis -= millichordsPerGraceNoteThis;
            onTimeNext -= millichordsPerGraceNoteNext;

            setEvent(graceChord, onTimeNext, millichordsPerGraceNoteNext);
            }

      // Set duration of next chord - done in calling function, after calling this function

      // Set duration of chord's grace notes, in reverse
      for (auto iter = graceNotesThis.rbegin(); iter != graceNotesThis.rend(); ++iter) {
            Chord* graceChord = *iter;

            millichordsPerGraceNoteThis = _baseMillisecondsPerGraceNote / millisecondsPerMillichord(chord);
            onTimeThis -= millichordsPerGraceNoteThis;

            setEvent(graceChord, onTimeThis, millichordsPerGraceNoteThis);
            }

      // Set duration of chord, by setting off time
      setOffTime(chord, onTimeThis);
      }

//---------------------------------------------------------
//   modifyChord
//---------------------------------------------------------
void BagpipeDurations::modifyChord(Chord* chord, Chord* prevChord) const
      {      
      if (!chord)
            return;

      QVector<Chord*> graceNotesAll;
      QVector<Chord*> graceNotesPrev;
      QVector<Chord*> graceNotesThis;

      // Collect 'after' grace notes of previous chord.
      if (prevChord)
            for (Chord* graceChord : prevChord->graceNotesAfter()) {
                  graceNotesAll.push_back(graceChord);
                  graceNotesPrev.push_back(graceChord);
                  }

      // Collect 'before' grace notes of this chord.
      for (Chord* graceChord : chord->graceNotesBefore()) {
            graceNotesAll.push_back(graceChord);
            graceNotesThis.push_back(graceChord);
            }

      switch (_positionType) {
            case PositionType::Before:
                  setBeforeBeatAtEnd(prevChord, graceNotesPrev, graceNotesThis);
                  setOnTime(chord, 0);
                  break;

            case PositionType::Varies: {
                  //QVector<Chord*> graceNotesBefore;
                  //QVector<Chord*> graceNotesAfter;

                  //// TBD: split graceNotesAll between graceNotesBefore and graceNotesAfter

                  //setAtEnd(prevChord, graceNotesBefore);
                  //setAtStart(chord, graceNotesAfter);
                  }
                  break;

            case PositionType::After:
                  setOffTime(prevChord, _millichordsPerChord);
                  setAfterBeatAtStart(chord, graceNotesPrev, graceNotesThis);
                  break;

            default:
                  break;
            }
      }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

//---------------------------------------------------------
//   modifyScore
//---------------------------------------------------------
void BagpipeDurations::modifyScore()
      {
      if (!_score->firstMeasure())
            return;

      _score->startCmd();

      auto selection = _score->selection();

      Segment* startSegment = selection.startSegment();
      Segment* endSegment = selection.endSegment();
      if (!startSegment && !endSegment) {
            startSegment = _score->firstMeasure()->first();
            endSegment = _score->lastMeasure()->last();
            }

      Chord* chord = 0;
      Chord* prevChord = 0;

      for (Segment* segment = startSegment; segment != endSegment; segment = segment->next1()) {

            switch (segment->segmentType()) {
                  case SegmentType::TimeSig:
                        break;

                  case SegmentType::ChordRest:
                        foreach(Element* element, segment->elist()) {
                              if (!element)
                                    continue;

                              if (element->isChordRest()) {
                                    auto tick = segment->tick();
                                    BeatType beatType = _score->tick2beatType( tick );

                                    ChordRest* chordRest = toChordRest(element);
                                    Fraction duration = chordRest->duration();
                                    Tuplet* tuplet = chordRest->tuplet();

                                    if (element->isChord()) {
                                          chord = toChord(chordRest);

                                          modifyChord(chord, prevChord);

                                          prevChord = chord;
                                          }
                                    }

                              break;
                        }
                  }
            }

      _score->endCmd();
      }

#pragma GCC diagnostic pop
#pragma GCC diagnostic pop // TEMP
}
