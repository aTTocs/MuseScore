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

#include "duration.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   Duration
//---------------------------------------------------------
Duration::Duration(const QString& noteDuration)
      {
      _noteDuration = noteDuration;
      }

//---------------------------------------------------------
//   setValues
//---------------------------------------------------------
QString Duration::setValues(const QString& duration, const CustomDuration_SPtr_Map& customDurations)
      {
      QString error("Duration Error");

      QStringList list = duration.split(',', QString::SplitBehavior::SkipEmptyParts);
      for (auto item : list) {
            QString itemNote = item.trimmed();

            if (item == "1/4" || item == "1/8" || item == "1/16" || item == "1/32")
                  if (_noteDuration.isEmpty())
                        _noteDuration = item;
                  else {
                        error += ", multiple note lengths " + item;
                        }
            else if (item.toLower() == "equal")
                  _equal = true;
            else if (item.toLower() == "half")
                  _half = true;
            else {
                  // Custom duration
                  auto iter = customDurations.find(item);
                  if (iter != customDurations.end()) {
                        CustomDuration_SPtr custom = iter->second;

                        if (!custom)
                              error += ", unknown <Custom id-\"" + item + "\"";
                        else {
                              _milliseconds = custom->milliseconds();
                              _factor = custom->factor();
                              }
                        }
                  else
                        error += ", unknown duration value " + item;
                  }
            }

      if (error == "Duration Error")
            return "";

      return error;
      }
} // namespace Bagpipe
