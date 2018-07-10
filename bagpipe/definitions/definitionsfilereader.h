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
#ifndef BAGPIPE_DEFINITIONSFILEREADER_H
#define BAGPIPE_DEFINITIONSFILEREADER_H

#include <memory>
#include <QFileInfo>

#include "../ms/filereader.h"
#include "../gracenotes/tags.h"

namespace Bagpipe {
using namespace std;

class Definitions;
using Definitions_SPtr = shared_ptr<Definitions>;

//---------------------------------------------------------
//   DefinitionsFileReader
//---------------------------------------------------------
class DefinitionsFileReader : public Ms::FileReader
   {
public:
      DefinitionsFileReader(QFile* file, Definitions_SPtr definitions);
      ~DefinitionsFileReader() = default;

      Definitions& definitions() const { return *_definitions; }

      static QFile* getFile(const QString& filename);

private:
      static QFileInfo styleFile(const QString& filename);

      Definitions_SPtr _definitions;
   };
} // namespace Bagpipe
#endif // BAGPIPE_DEFINITIONSFILEREADER_H
