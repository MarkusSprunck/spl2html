#pragma once
/**
 * Copyright (C) 2016, Markus Sprunck
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 *
 * - The name of its contributor may be used to endorse or promote
 *   products derived from this software without specific prior
 *   written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "std_typedef.h"

static const string g_sVersion = "spl2html v0.1";

class line_status;
class formater {
public:
   explicit formater();
   
   void run(const string& inputFile);

protected:
   
   // file input & output
   void importLines(const string& file, list<string>& m_Lines);
   void exportLines(const string& file);
   
   // each line
   void formatAll();
   void removeEmptyAll();
   void wrapLines(string pattern);
   void appendLineNumbersToHtmlAll();
   
   // single line
   bool parseLine(string &line, line_status& ls);
   void replaceSubstrings(const index_string& begin, index_string& end, string &s);
   static void createIndenting(string &line, line_status& ls);
   
   // support for string manipulation
   static void trimRight(string &s);
   static void trimLeft(string &s);
   
   // succsses information
   string m_sResult;
   
   // create a html file
   bool m_bCreateHtml;
   
   // file content
   list<string> m_Lines;
   
   // commands for each 'string pattern'
   map<string, long> m_commands;
   
   // string replace map
   map<string, string> m_replacePatterns, m_replacePatternsHtml;
};
