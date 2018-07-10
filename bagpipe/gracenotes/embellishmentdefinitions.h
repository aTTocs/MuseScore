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
#ifndef BAGPIPE_EMBELLISHMENTDEFINITIONS_H
#define BAGPIPE_EMBELLISHMENTDEFINITIONS_H

#include "../definitions/definitionselementreader.h"
#include "category.h"
#include "customduration.h"
#include "embellishment.h"

namespace Bagpipe {
using namespace std;

class EmbellishmentDefinitions;
using EmbellishmentDefinitions_SPtr = shared_ptr<EmbellishmentDefinitions>;

//---------------------------------------------------------
//   EmbellishmentDefinitions
//---------------------------------------------------------
class EmbellishmentDefinitions
   {
public:
      EmbellishmentDefinitions();

      // Default Grace Note Durations, in milliseconds
      int default32nd;
      int default16th;
      int default8th;
      int piobDefault32nd;
      int piobDefault16th;
      int piobDefault8th;

      // Default Beat Factors
      double strongBeat;
      double mediumBeat;
      double weakBeat;

      bool addEmbellishment(Embellishment_SPtr embellishmentById);
      const Embellishment_SPtr embellishmentById(const QString& id) const;

      bool containsEmbellishmentsByNotes(const QString& notes) const;
      const Embellishment_SPtr_List& embellishmentsByNotes(const QString& notes) const;

      bool addCategory(Category_SPtr category);
      const Category_SPtr category(const QString& name) const;

      const Category& rootCategory() const { return *_rootCategory; }

      const CustomDuration_SPtr_Map& customDurations() const { return _customDurations; }

      static EmbellishmentDefinitions_SPtr make() { return EmbellishmentDefinitions_SPtr(new EmbellishmentDefinitions()); }

private:
      friend class EmbellishmentDefinitionsReader;

      Embellishment_SPtr_IdMap _embellishmentsById;
      Embellishment_SPtr_NotesMap _embellishmentsByNotes;
      Category_SPtr_Map _categories;
      CustomDuration_SPtr_Map _customDurations;
      Category_SPtr _rootCategory;
   };

//---------------------------------------------------------
//   EmbellishmentDefinitionsReader
//---------------------------------------------------------
class EmbellishmentDefinitionsReader : public DefinitionsElementReader<EmbellishmentDefinitions>
      {
public:
      EmbellishmentDefinitionsReader(EmbellishmentDefinitions_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~EmbellishmentDefinitionsReader() = default;

protected:
      virtual void readElement(const QStringRef& tag);
      virtual void readCompleted();

private:
      void readDurations();
      void readCustomDuration();
      };
} // namespace Bagpipe
#endif // BAGPIPE_EMBELLISHMENTDEFINITIONS_H
