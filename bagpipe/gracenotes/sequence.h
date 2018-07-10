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
#ifndef BAGPIPE_SEQUENCE_H
#define BAGPIPE_SEQUENCE_H

#include <memory>
#include <vector>
#include <QString>

#include "../definitions/definitionselementreader.h"

namespace Bagpipe {
using namespace std;

class Sequence;
using Sequence_SPtr = shared_ptr<Sequence>;

class MelodyNote;
using MelodyNote_SPtr = shared_ptr<MelodyNote>;
using MelodyNote_SPtr_Vector = vector<MelodyNote_SPtr>;

//---------------------------------------------------------
//   Sequence
//---------------------------------------------------------
class Sequence
   {
public:
      Sequence() = default;
      Sequence(const QString& bww);

      const QString& bww() const { return _bww; }
      const MelodyNote_SPtr_Vector& melodyNotes() const { return _melodyNotes; }

      uint matchCount() const;

      static Sequence_SPtr make() { return Sequence_SPtr(new Sequence()); }

private:
      friend class SequenceReader;

      QString _bww;
      bool _triplet;

#pragma clang diagnostic ignored "-Wpadded"
      MelodyNote_SPtr_Vector _melodyNotes;
   };

//---------------------------------------------------------
//   SequenceReader
//---------------------------------------------------------
class SequenceReader : public DefinitionsElementReader<Sequence>
      {
public:
      SequenceReader(Sequence_SPtr item, DefinitionsFileReader& fileReader, QString itemTag, const QString& defId);
      SequenceReader(Sequence_SPtr item, DefinitionsFileReader& fileReader, QString itemTag);
      virtual ~SequenceReader() = default;

protected:
      virtual void readAttributes();
      virtual void readElement(const QStringRef& tag);
      virtual void readCompleted();

      MelodyNote_SPtr _currentMelodyNote  = nullptr;
      bool _preBeat = true;

      QString _defId;
      };
} // namespace Bagpipe
#endif // BAGPIPE_SEQUENCE_H
