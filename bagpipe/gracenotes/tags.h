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
#ifndef BAGPIPE_TAGS_H
#define BAGPIPE_TAGS_H

#include <QString>

namespace Bagpipe {

struct Tag
      {
      static const QString Abbreviated;
      static const QString Beat;
      static const QString Category;
      static const QString CustomDuration;
      static const QString Default16th;
      static const QString Default32nd;
      static const QString Default8th;
      static const QString Definitions;
      static const QString Durations;
      static const QString Embellishment;
      static const QString EmbellishmentDefinitions;
      static const QString EmbellishmentReference;
      static const QString Exercises;
      static const QString GraceNote;
      static const QString MediumBeat;
      static const QString MelodyNote;
      static const QString NoteDefinition;
      static const QString Notes;
      static const QString PiobDefault16th;
      static const QString PiobDefault32nd;
      static const QString PiobDefault8th;
      static const QString Played;
      static const QString StrongBeat;
      static const QString Substitute;
      static const QString WeakBeat;
      static const QString beam;
      static const QString bww;
      static const QString cann;
      static const QString display;
      static const QString dotted;
      static const QString duration;
      static const QString equal;
      static const QString factor;
      static const QString fermata;
      static const QString id;
      static const QString left;
      static const QString midi;
      static const QString milliseconds;
      static const QString name;
      static const QString note;
      static const QString right;
      static const QString symbolposition;
      static const QString symbol;
      static const QString triplet;
      };

} // namespace Bagpipe
#endif // BAGPIPE_TAGS_H
