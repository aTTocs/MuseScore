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
#ifndef BAGPIPE_CUSTOMDURATION_H
#define BAGPIPE_CUSTOMDURATION_H

#include <memory>
#include "../definitions/definitionselementreader.h"

namespace Bagpipe {
using namespace std;

class CustomDuration;
using CustomDuration_SPtr = shared_ptr<CustomDuration>;
using CustomDuration_SPtr_Map = map<QString, CustomDuration_SPtr>;

//---------------------------------------------------------
//   CustomDuration
//---------------------------------------------------------
class CustomDuration
   {
public:
      const QString& id() const { return _id; }
      int milliseconds() const { return _milliseconds; }
      double factor() const { return _factor; }

      static CustomDuration_SPtr make() { return CustomDuration_SPtr(new CustomDuration()); }

private:
      friend class CustomDurationReader;

      QString _id;
      int _milliseconds = 0;
      double _factor = 0.0;
   };

//---------------------------------------------------------
//   CustomDurationReader
//---------------------------------------------------------
class CustomDurationReader : public DefinitionsElementReader<CustomDuration>
      {
public:
      CustomDurationReader(CustomDuration_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~CustomDurationReader() = default;

protected:
      virtual void readAttributes();
      };
} // namespace Bagpipe
#endif // BAGPIPE_CUSTOMDURATION_H
