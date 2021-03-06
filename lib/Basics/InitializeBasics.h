////////////////////////////////////////////////////////////////////////////////
/// @brief force symbols into program
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2014, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2009-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_BASICS_INITIALIZE_BASICS_H
#define ARANGODB_BASICS_INITIALIZE_BASICS_H 1

#include "Basics/Common.h"

namespace triagens {

////////////////////////////////////////////////////////////////////////////////
/// @brief the namespace containing the basic classes and functions
////////////////////////////////////////////////////////////////////////////////

  namespace basics {

////////////////////////////////////////////////////////////////////////////////
/// @brief initialize function
////////////////////////////////////////////////////////////////////////////////

    extern void InitializeBasics (int argv, char* argc[]);

////////////////////////////////////////////////////////////////////////////////
/// @brief shutdown function
////////////////////////////////////////////////////////////////////////////////

    extern void ShutdownBasics ();
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief initialize
////////////////////////////////////////////////////////////////////////////////

#define TRIAGENS_BASICS_INITIALIZE(a,b)                 \
  do {                                                  \
    triagens::basics::InitializeBasics((a), (b));       \
  } while (0)

////////////////////////////////////////////////////////////////////////////////
/// @brief shutdown
////////////////////////////////////////////////////////////////////////////////

#define TRIAGENS_BASICS_SHUTDOWN                \
  do {                                          \
    triagens::basics::ShutdownBasics();         \
  } while (0)

#endif
// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
