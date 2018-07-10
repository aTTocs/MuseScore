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
#ifndef BAGPIPE_DURATION_H
#define BAGPIPE_DURATION_H

#include <memory>
#include "../definitions/definitionselementreader.h"
#include "customduration.h"

namespace Bagpipe {
using namespace std;

class Duration;
using Duration_SPtr = shared_ptr<Duration>;

//---------------------------------------------------------
//   Duration
//---------------------------------------------------------
#pragma clang diagnostic ignored "-Wpadded"
class Duration
   {
public:
      Duration() = default;
      Duration(const QString& noteDuration);

      const QString& noteDuration() const { return _noteDuration; }

      QString setValues(const QString& duration, const CustomDuration_SPtr_Map& customDurations);

      static Duration_SPtr make() { return Duration_SPtr(new Duration()); }
      static Duration_SPtr make(const QString& noteDuration) { return Duration_SPtr(new Duration(noteDuration)); }

private:
      QString _noteDuration;
      int _milliseconds = 0;
      double _factor = 0.0;
      bool _equal = false;
      bool _half = false;
   };
} // namespace Bagpipe
#endif // BAGPIPE_DURATION_H
