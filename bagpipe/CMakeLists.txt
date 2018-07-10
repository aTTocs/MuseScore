#=============================================================================
#  m_ms/core
#  Linux Music Score Editor
#  $Id:$
#
#  Copyright (C) 2018 by Werner Schweer and others
#
#  This program is free software; you can redistribute it and/or modify
#  it under the term_ms/ of the GNU General Public License version 2.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#=============================================================================

include (${PROJECT_SOURCE_DIR}/build/gch.cmake)

if (APPLE)
      file(GLOB_RECURSE INCS "*.h")
else (APPLE)
      set(INCS "")
endif (APPLE)

add_library (
      bagpipe STATIC
      ${PROJECT_BINARY_DIR}/all.h
      ${INCS}
      ${PCH}
      definitions/definitions.cpp               definitions/definitions.h
      definitions/definitionselementreader.h
      definitions/definitionsfilereader.cpp     definitions/definitionsfilereader.h
      definitions/definitionsreader.cpp         definitions/definitionsreader.h
      gracenotes/category.cpp                   gracenotes/category.h
      gracenotes/customduration.cpp             gracenotes/customduration.h
      gracenotes/duration.cpp                   gracenotes/duration.h
      gracenotes/embellishment.cpp              gracenotes/embellishment.h
      gracenotes/embellishmentdefinitions.cpp   gracenotes/embellishmentdefinitions.h
      gracenotes/gracenote.cpp                  gracenotes/gracenote.h
      gracenotes/match.cpp                      gracenotes/match.h
      gracenotes/melodynote.cpp                 gracenotes/melodynote.h
      gracenotes/noteconstraint.cpp             gracenotes/noteconstraint.h
      gracenotes/notedefinition.cpp             gracenotes/notedefinition.h
      gracenotes/notedefinitionmap.cpp          gracenotes/notedefinitionmap.h
      gracenotes/sequence.cpp                   gracenotes/sequence.h
      gracenotes/tags.cpp                       gracenotes/tags.h
      ms/elementreader.h
      ms/filereader.cpp                         ms/filereader.h
      ms/logging.h                              ms/logging.cpp
      ui/durationsmodifier.cpp                  ui/durationsmodifier.h
      ui/scoremaker.cpp                         ui/scoremaker.h
      ui/treeitem.cpp                           ui/treeitem.h
      ui/treemodel.cpp                          ui/treemodel.h
      )

##
## Code coverage. Only affects DEBUG builds.
##

set(COVERAGE_OPTIONS "")
# if (NOT APPLE AND NOT MINGW AND CMAKE_BUILD_TYPE MATCHES "DEBUG")
#    if (COVERAGE)
#       set(COVERAGE_OPTIONS "-O0 --coverage")
#       set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
#       target_link_libraries(bagpipe gcov)
#    endif (COVERAGE)
# endif (NOT APPLE AND NOT MINGW AND CMAKE_BUILD_TYPE MATCHES "DEBUG")
      
set_target_properties (
      bagpipe
      PROPERTIES
         COMPILE_FLAGS "${PCH_INCLUDE} -g -Wall -Wextra -Winvalid-pch -Wno-unknown-pragmas ${COVERAGE_OPTIONS}"
      )

xcode_pch(bagpipe all)

ADD_DEPENDENCIES(bagpipe mops1)
ADD_DEPENDENCIES(bagpipe mops2)

