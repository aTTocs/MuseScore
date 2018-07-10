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

#include "embellishmentdefinitions.h"

#include <QXmlStreamReader>

#include "melodynote.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   EmbellishmentDefinitions
//---------------------------------------------------------
EmbellishmentDefinitions::EmbellishmentDefinitions()
      {
      _rootCategory = Category::make();
      _rootCategory->_name = "Root";
      }

//---------------------------------------------------------
//   addEmbellishment
//---------------------------------------------------------
bool EmbellishmentDefinitions::addEmbellishment(Embellishment_SPtr embellishment)
      {
      if (_embellishmentsById.find(embellishment->id()) != _embellishmentsById.end())
            return false;

      // Add to ById map
      _embellishmentsById.emplace(embellishment->id(), embellishment);

      // Add to ByNotes map
      for (auto melodyNote : embellishment->melodyNotes()) {
            const QString& graceNoteString = melodyNote->graceNoteString();
            if (graceNoteString.isEmpty())
                  continue;

            if (_embellishmentsByNotes.find(graceNoteString) == _embellishmentsByNotes.end())
                  _embellishmentsByNotes.emplace(graceNoteString, Embellishment_SPtr_List());

            Embellishment_SPtr_List& embellishments = _embellishmentsByNotes.at(graceNoteString);

            embellishments.push_back(embellishment);
            }

      return true;
      }

//---------------------------------------------------------
//   embellishmentById
//---------------------------------------------------------
const Embellishment_SPtr EmbellishmentDefinitions::embellishmentById(const QString& id) const
      {
      if (_embellishmentsById.find(id) != _embellishmentsById.end())
            return _embellishmentsById.at(id);

      return nullptr;
      }

//---------------------------------------------------------
//   containsEmbellishmentsByNotes
//---------------------------------------------------------
bool EmbellishmentDefinitions::containsEmbellishmentsByNotes(const QString& notes) const
      {
      return _embellishmentsByNotes.find(notes) != _embellishmentsByNotes.end();
      }

//---------------------------------------------------------
//   embellishmentsByNotes
//---------------------------------------------------------
const Embellishment_SPtr_List& EmbellishmentDefinitions::embellishmentsByNotes(const QString& notes) const
      {
      return _embellishmentsByNotes.at(notes);
      }

//---------------------------------------------------------
//   addCategory
//---------------------------------------------------------
bool EmbellishmentDefinitions::addCategory(Category_SPtr category)
      {
      if (_categories.find(category->name()) != _categories.end())
            return false;

      _categories.emplace(category->name(), category);

      return true;
      }

//---------------------------------------------------------
//   getCategory
//---------------------------------------------------------
const Category_SPtr EmbellishmentDefinitions::category(const QString& name) const
      {
      if (_categories.find(name) != _categories.end())
            return _categories.at(name);

      return nullptr;
      }

//---------------------------------------------------------
//   EmbellishmentDefinitionsReader
//---------------------------------------------------------
EmbellishmentDefinitionsReader::EmbellishmentDefinitionsReader(EmbellishmentDefinitions_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::EmbellishmentDefinitions, {}, true)
      {}

//---------------------------------------------------------
//   readElement
//---------------------------------------------------------
void EmbellishmentDefinitionsReader::readElement(const QStringRef& tag)
      {
      if (tag == Tag::Durations) {
            readDurations();
            }
      else if (tag == Tag::Category) {
            Category_SPtr category(Category::make());
            category->setParent(item()._rootCategory);

            CategoryReader reader(category, fileReader());
            reader.readItem();

            if (!reader.hadErrors() && item().addCategory(category))
                  item()._rootCategory->_children.push_back(category);
            }
      else if (tag == Tag::Exercises) {
            problemFound("Skipping " + Tag::Exercises);
            fileReader().skipCurrentElement();
            }
      else
            elementUnknown(Tag::EmbellishmentDefinitions);
      }

//---------------------------------------------------------
//   readCompleted
//---------------------------------------------------------
void EmbellishmentDefinitionsReader::readCompleted()
      {
      // Sort embellishment lists by their matchCount, in descending order.
      Embellishment_SPtr_NotesMap::iterator iter = item()._embellishmentsByNotes.begin();
      for (; iter != item()._embellishmentsByNotes.end(); ++iter) {
            Embellishment_SPtr_List& embellishments = iter->second;
            if (embellishments.size() > 1)
                  embellishments.sort(Embellishment::compare);
            }
      }

//---------------------------------------------------------
//   readDurations
//---------------------------------------------------------
void EmbellishmentDefinitionsReader::readDurations()
      {
      Q_ASSERT(fileReader().name() == Tag::Durations);

      while (readNextStartElement()) {
            const QStringRef& tag(fileReader().name());

            if      (tag == Tag::CustomDuration) readCustomDuration();
            else if (tag == Tag::Default32nd) item().default32nd = intAttribute(Tag::milliseconds, true);
            else if (tag == Tag::Default16th) item().default16th = intAttribute(Tag::milliseconds, true);
            else if (tag == Tag::Default8th) item().default8th = intAttribute(Tag::milliseconds, true);
            else if (tag == Tag::PiobDefault32nd) item().piobDefault32nd = intAttribute(Tag::milliseconds, true);
            else if (tag == Tag::PiobDefault16th) item().piobDefault16th = intAttribute(Tag::milliseconds, true);
            else if (tag == Tag::PiobDefault8th) item().piobDefault8th = intAttribute(Tag::milliseconds, true);
            else if (tag == Tag::StrongBeat) item().strongBeat = doubleAttribute(Tag::factor, true);
            else if (tag == Tag::MediumBeat) item().mediumBeat = doubleAttribute(Tag::factor, true);
            else if (tag == Tag::WeakBeat) item().weakBeat = doubleAttribute(Tag::factor, true);
            else
                  elementUnknown(Tag::Durations);

            // Must call readNext when only attributes!
            readNext();
            }
      }

//---------------------------------------------------------
//   readCustomDuration
//---------------------------------------------------------
void EmbellishmentDefinitionsReader::readCustomDuration()
      {
      Q_ASSERT(fileReader().name() == Tag::CustomDuration);

      CustomDuration_SPtr custom(CustomDuration::make());

      CustomDurationReader reader(custom, fileReader());
      reader.readItem();

      bool valid = !reader.hadErrors();

      if (item()._customDurations.find(custom->id()) != item()._customDurations.end()) {
            elementIdAlreadyUsed(custom->id());
            valid = false;
            }

      if (valid)
            item()._customDurations[custom->id()] = custom;

      // readNext(); done by caller.
      }
} // namespace Bagpipe
