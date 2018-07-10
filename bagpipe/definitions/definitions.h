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
#ifndef BAGPIPE_DEFINITIONS_H
#define BAGPIPE_DEFINITIONS_H

#include <memory>

#include "../gracenotes/notedefinitionmap.h"

namespace Bagpipe {
using namespace std;

class Definitions;
using Definitions_SPtr = shared_ptr<Definitions>;

class EmbellishmentDefinitions;
using EmbellishmentDefinitions_SPtr = shared_ptr<EmbellishmentDefinitions>;

//---------------------------------------------------------
//   Definitions
//---------------------------------------------------------
class Definitions
   {
public:
      Definitions();

      const INoteDefinitionMap& noteDefinitionMap() const { return *_noteDefinitionMap; }

      EmbellishmentDefinitions& embellishments() { return *_embellishments; }
      const EmbellishmentDefinitions& embellishments() const { return *_embellishments; }
      EmbellishmentDefinitions_SPtr embellishments_SPtr() { return _embellishments; }

      static Definitions_SPtr make() { return Definitions_SPtr(new Definitions()); }

private:
      friend class DefinitionsReader;

      INoteDefinitionMap_UPtr _noteDefinitionMap;
      EmbellishmentDefinitions_SPtr _embellishments;
   };

//Definitions_SPtr makeDefinitions() { return Definitions_SPtr(new Definitions()); }

} // namespace Bagpipe
#endif // BAGPIPE_DEFINITIONS_H
