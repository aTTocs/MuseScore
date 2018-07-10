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
#ifndef BAGPIPE_DEFINITIONSREADER_H
#define BAGPIPE_DEFINITIONSREADER_H

#include "definitionselementreader.h"

namespace Bagpipe {

//---------------------------------------------------------
//   DefinitionsReader
//---------------------------------------------------------
class DefinitionsReader : public DefinitionsElementReader<Definitions>
      {
public:
      DefinitionsReader(Definitions_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~DefinitionsReader() = default;

protected:
      virtual void readElement(const QStringRef& tag);

private:
      void readNotes();
      };
} // namespace Bagpipe
#endif // BAGPIPE_DEFINITIONSREADER_H
