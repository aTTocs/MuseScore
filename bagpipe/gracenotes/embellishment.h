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
#ifndef BAGPIPE_EMBELLISHMENT_H
#define BAGPIPE_EMBELLISHMENT_H

#include <memory>

#include "../definitions/definitionselementreader.h"
#include "sequence.h"

namespace Bagpipe {
using namespace std;

class Category;
using Category_SPtr = shared_ptr<Category>;

class Embellishment;
using Embellishment_SPtr = shared_ptr<Embellishment>;
using Embellishment_SPtr_List = list<Embellishment_SPtr>;
using Embellishment_SPtr_Vector = vector<Embellishment_SPtr>;
using Embellishment_SPtr_IdMap = map<QString, Embellishment_SPtr>;
using Embellishment_SPtr_NotesMap = map<QString, Embellishment_SPtr_List>;

//---------------------------------------------------------
//   Embellishment
//---------------------------------------------------------
class Embellishment
   {
public:
      Embellishment() = default;

      const QString& id() const { return _id; }
      const QString& displayName() const;
      const QString& bww() const { return _bww; }
      const QString& canntaireachd() const { return _canntaireachd; }

      uint matchCount() const { return _matchCount; }

      const Category_SPtr& category() const { return _category; }
      void setCategory(Category_SPtr category) { _category = category; }

      const MelodyNote_SPtr_Vector& melodyNotes() const { return _played->melodyNotes(); }

      static Embellishment_SPtr make() { return Embellishment_SPtr(new Embellishment()); }
      static bool compare(const Embellishment_SPtr& first, const Embellishment_SPtr& second) { return first->matchCount() > second->matchCount(); }

private:
      friend class EmbellishmentReader;
      friend class SequenceReader;

      QString _id;
      QString _displayName;
      QString _bww;
      QString _canntaireachd;
      Category_SPtr _category;

      Sequence_SPtr _played = nullptr;
      Sequence_SPtr _abbreviated = nullptr;

      uint _matchCount = 0;

      // When this embellishment is displayed, actually play another one.
      // e.g. D Throws are usually written light, but some people always play hard.
      Embellishment_SPtr _substitute;
   };

//---------------------------------------------------------
//   EmbellishmentReader
//---------------------------------------------------------
class EmbellishmentReader : public DefinitionsElementReader<Embellishment>
      {
public:
      EmbellishmentReader(Embellishment_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~EmbellishmentReader() = default;

protected:
      virtual void readAttributes();
      virtual void readElement(const QStringRef& tag);
      virtual void readCompleted();
      };
} // namespace Bagpipe
#endif // BAGPIPE_EMBELLISHMENT_H
