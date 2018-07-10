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
#ifndef BAGPIPE_DEFINITIONSELEMENTREADER_H
#define BAGPIPE_DEFINITIONSELEMENTREADER_H

#include "../ms/elementreader.h"
#include "definitionsfilereader.h"

namespace Bagpipe {

//---------------------------------------------------------
//   DefinitionsElementReader
//---------------------------------------------------------
template<class TItem>
class DefinitionsElementReader : public Ms::ElementReader<TItem, DefinitionsFileReader>
      {
protected:
      DefinitionsElementReader(shared_ptr<TItem> item, DefinitionsFileReader& fileReader, QString itemTag, initializer_list<const QString> attributes, bool containsElements)
            : Ms::ElementReader<TItem, DefinitionsFileReader>(item, fileReader, itemTag, attributes, containsElements)
            {}
      };
} // namespace Bagpipe
#endif // BAGPIPE_DEFINITIONSELEMENTREADER_H
