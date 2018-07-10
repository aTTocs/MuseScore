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

#include "customduration.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   CustomDurationReader
//---------------------------------------------------------
CustomDurationReader::CustomDurationReader(CustomDuration_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::CustomDuration, {Tag::id, Tag::milliseconds, Tag::factor}, false)
      {}

//---------------------------------------------------------
//   readAttributes
//---------------------------------------------------------
void CustomDurationReader::readAttributes()
      {
      item()._id = attribute(Tag::id, true);
      item()._milliseconds = intAttribute(Tag::milliseconds, false);
      item()._factor = doubleAttribute(Tag::factor, false);

      if (   (item()._milliseconds == 0 && item()._factor == 0.0)   // has neither
          || (item()._milliseconds != 0 && item()._factor != 0.0))  // has both
            problemFound(QString("<Custom> must have either milliseconds=\"(int)\" or factor=\"(real)\""));
      }
} // namespace Bagpipe
