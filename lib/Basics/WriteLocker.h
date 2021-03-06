////////////////////////////////////////////////////////////////////////////////
/// @brief write locker
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
/// @author Frank Celler
/// @author Achim Brandt
/// @author Copyright 2014, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2010-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_BASICS_WRITE_LOCKER_H
#define ARANGODB_BASICS_WRITE_LOCKER_H 1

#include "Basics/Common.h"
#include "Basics/ReadWriteLock.h"

#ifdef TRI_SHOW_LOCK_TIME
#include "Basics/logging.h"
#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                     public macros
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief construct locker with file and line information
///
/// Ones needs to use macros twice to get a unique variable based on the line
/// number.
////////////////////////////////////////////////////////////////////////////////

#define WRITE_LOCKER_VAR_A(a) _write_lock_variable ## a
#define WRITE_LOCKER_VAR_B(a) WRITE_LOCKER_VAR_A(a)

#ifdef TRI_SHOW_LOCK_TIME

#define WRITE_LOCKER(b) \
  triagens::basics::WriteLocker<std::remove_reference<decltype(b)>::type> WRITE_LOCKER_VAR_B(__LINE__)(&b, __FILE__, __LINE__)

#define WRITE_LOCKER_EVENTUAL(b, t) \
  triagens::basics::WriteLocker<std::remove_reference<decltype(b)>::type> WRITE_LOCKER_VAR_B(__LINE__)(&b, t, __FILE__, __LINE__)

#else

#define WRITE_LOCKER(b) \
  triagens::basics::WriteLocker<std::remove_reference<decltype(b)>::type> WRITE_LOCKER_VAR_B(__LINE__)(&b)

#define WRITE_LOCKER_EVENTUAL(b, t) \
  triagens::basics::WriteLocker<std::remove_reference<decltype(b)>::type> WRITE_LOCKER_VAR_B(__LINE__)(&b, t)

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                 class WriteLocker
// -----------------------------------------------------------------------------

namespace triagens {
  namespace basics {

////////////////////////////////////////////////////////////////////////////////
/// @brief write locker
///
/// A WriteLocker write-locks a read-write lock during its lifetime and unlocks
/// the lock when it is destroyed.
////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    class WriteLocker {
        WriteLocker (WriteLocker const&);
        WriteLocker& operator= (WriteLocker const&);

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief aquires a write-lock
///
/// The constructors aquires a write lock, the destructors unlocks the lock.
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_SHOW_LOCK_TIME

        WriteLocker (T* readWriteLock, char const* file, int line)
          : _readWriteLock(readWriteLock), _file(file), _line(line) {

          double t = TRI_microtime();
          _readWriteLock->writeLock();
          _time = TRI_microtime() - t;
        }

#else

        explicit WriteLocker (T* readWriteLock)
          : _readWriteLock(readWriteLock) {
          
          _readWriteLock->writeLock();
        }

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief aquires a write-lock, with periodic sleeps while not acquired
/// sleep time is specified in nanoseconds
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_SHOW_LOCK_TIME
        
        WriteLocker (T* readWriteLock, 
                     uint64_t sleepTime,
                     char const* file,
                     int line) 
          : _readWriteLock(readWriteLock), _file(file), _line(line) {
          
          double t = TRI_microtime();
          while (! _readWriteLock->tryWriteLock()) {
#ifdef _WIN32
            usleep((unsigned long) sleepTime);
#else
            usleep((useconds_t) sleepTime);
#endif
          }
          _time = TRI_microtime() - t;
        }

#else

        WriteLocker (T* readWriteLock, 
                     uint64_t sleepTime) 
          : _readWriteLock(readWriteLock) {
          
          while (! _readWriteLock->tryWriteLock()) {
#ifdef _WIN32
            usleep((unsigned long) sleepTime);
#else
            usleep((useconds_t) sleepTime);
#endif
          }
        }

#endif        

////////////////////////////////////////////////////////////////////////////////
/// @brief releases the write-lock
////////////////////////////////////////////////////////////////////////////////

        ~WriteLocker () {
          _readWriteLock->unlock();

#ifdef TRI_SHOW_LOCK_TIME
          if (_time > TRI_SHOW_LOCK_THRESHOLD) {
            LOG_WARNING("WriteLocker %s:%d took %f s", _file, _line, _time);
          }
#endif  
        }

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief the read-write lock
////////////////////////////////////////////////////////////////////////////////

        T* _readWriteLock;

#ifdef TRI_SHOW_LOCK_TIME

////////////////////////////////////////////////////////////////////////////////
/// @brief file
////////////////////////////////////////////////////////////////////////////////

        char const* _file;

////////////////////////////////////////////////////////////////////////////////
/// @brief line number
////////////////////////////////////////////////////////////////////////////////

        int _line;

////////////////////////////////////////////////////////////////////////////////
/// @brief lock time
////////////////////////////////////////////////////////////////////////////////

        double _time;

#endif        

    };
  }
}

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
