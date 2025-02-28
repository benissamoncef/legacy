/*  minIni - Multi-Platform INI file parser, suitable for embedded systems
 *
 *  Copyright (c) ITB CompuPhase, 2008-2010
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not
 *  use this file except in compliance with the License. You may obtain a copy
 *  of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 *
 *  Version: $Id: minIni.h,v 1.1 2011/02/16 15:58:18 pc2dpdy Exp $
 */
#ifndef MININI_H
#define MININI_H

#include "minGlue.h"

#if (defined _UNICODE || defined __UNICODE__ || defined UNICODE) && !defined INI_ANSIONLY
  #include <tchar.h>
#elif !defined __T
  typedef char TCHAR;
#endif

#if !defined INI_BUFFERSIZE
  #define INI_BUFFERSIZE  512
#endif

#if defined __cplusplus
  extern "C" {
#endif

long ini_getl(const TCHAR *Section, const TCHAR *Key, long DefValue, const TCHAR *Filename);
int  ini_gets(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, TCHAR *Buffer, int BufferSize, const TCHAR *Filename);
int  ini_putl(const TCHAR *Section, const TCHAR *Key, long Value, const TCHAR *Filename);
int  ini_puts(const TCHAR *Section, const TCHAR *Key, const TCHAR *Value, const TCHAR *Filename);
int  ini_getsection(int idx, TCHAR *Buffer, int BufferSize, const TCHAR *Filename);
int  ini_getkey(const TCHAR *Section, int idx, TCHAR *Buffer, int BufferSize, const TCHAR *Filename);

#if defined __cplusplus
  }
#endif


#if defined __cplusplus

#if defined __WXWINDOWS__
	#include "wxMinIni.h"
#else
  #include <string>

  /* The C++ class in minIni.h was contributed by Steven Van Ingelgem. */
  class minIni
  {
  public:
    minIni(const std::string& filename) : iniFilename(filename)
      { }

    long getl(const std::string& Section, const std::string& Key, long DefValue=0) const
      { return ini_getl(Section.c_str(), Key.c_str(), DefValue, iniFilename.c_str()); }

    int geti(const std::string& Section, const std::string& Key, int DefValue=0) const
      { return static_cast<int>(this->getl(Section, Key, long(DefValue))); }

    std::string gets(const std::string& Section, const std::string& Key, const std::string& DefValue="") const
      {
        char buffer[INI_BUFFERSIZE];
        ini_gets(Section.c_str(), Key.c_str(), DefValue.c_str(), buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

    std::string getsection(int idx) const
      {
        char buffer[INI_BUFFERSIZE];
        ini_getsection(idx, buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

    std::string getkey(const std::string& Section, int idx) const
      {
        char buffer[INI_BUFFERSIZE];
        ini_getkey(Section.c_str(), idx, buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

#if ! defined INI_READONLY
    bool put(const std::string& Section, const std::string& Key, long Value) const
      { return (bool)ini_putl(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()); }

    bool put(const std::string& Section, const std::string& Key, int Value) const
      { return (bool)ini_putl(Section.c_str(), Key.c_str(), (long)Value, iniFilename.c_str()); }

    bool put(const std::string& Section, const std::string& Key, const std::string& Value) const
      { return (bool)ini_puts(Section.c_str(), Key.c_str(), Value.c_str(), iniFilename.c_str()); }

    bool del(const std::string& Section, const std::string& Key) const
      { return (bool)ini_puts(Section.c_str(), Key.c_str(), 0, iniFilename.c_str()); }

    bool del(const std::string& Section) const
      { return (bool)ini_puts(Section.c_str(), 0, 0, iniFilename.c_str()); }
#endif

  private:
    std::string iniFilename;
  };

#endif /* __WXWINDOWS__ */
#endif /* __cplusplus */

#endif /* MININI_H */
