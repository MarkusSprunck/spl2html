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

#include "formater.h"
#include "formater_parameter.h"
#include "formater_replace.h"
#include "layer_counter.h"
#include "formater_mark_html.h"
#include "string_utils.h"
#include "line_status.h"

formater::formater() :
result("ok"),
createHtml(true),
spl_keywords(getCommand()),
replacePatternPreprocessing(getReplacePrepocessing()),
replacePatternPostprocessing(getReplacePostprocessing()),
replacePatternHtml(getReplaceHtml()) {
}

void formater::run(const string& inputFile) {
    // create text file
    createHtml = false;
    importAllLines(inputFile, lines, true);
    for_each(lines.begin(), lines.end(), trimLeft);
    for_each(lines.begin(), lines.end(), trimRight);
    formatPre();
    createNewLineIfNeeded();
    formatPost();
    string outputFileResult = string(inputFile).append(".result");
    exportAllLines(outputFileResult);
    cout << string("create 'file://").append(outputFileResult).append("' ").append(result) << endl;

    // create html file
    createHtml = true;
    importAllLines(inputFile, lines, true);
    for_each(lines.begin(), lines.end(), trimLeft);
    for_each(lines.begin(), lines.end(), trimRight);
    formatPre();
    createNewLineIfNeeded();
    formatPost();
    createHtmlDocument();
    string outputFileHtml = string(inputFile).append(".html");
    exportAllLines(outputFileHtml);
    cout << string("create 'file://").append(outputFileHtml).append("' ").append(result) << endl;
}

void formater::importAllLines(const string& file, list<string>& m_Lines, bool single) {
    fstream fin(file.c_str(), ios_base::in);
    if (fin.fail()) {
        result = "file open failed";
        return;
    }

    m_Lines.clear();
    string result;
    string line;
    while (getline(fin, line) && (!fin.fail())) {
        if (createHtml) {
            formater_replace::repeated_replace(line, "&", "&amp");
            formater_replace::repeated_replace(line, "<", "&lt");
            formater_replace::repeated_replace(line, ">", "&gt");
        }
        if (single) {
            result = result.append(line).append(" ");
        } else {
            m_Lines.push_back(line);
        }
    }
    if (single) {
        m_Lines.push_back(result);
    }
    fin.close();
}

void formater::exportAllLines(const string& file) {

    if (0 == result.compare("ok")) {
        fstream fout(file.c_str(), ios_base::out | ios_base::trunc);
        if (fout.fail()) {
            result = "file create failed";
            return;
        }

        copy(lines.begin(), lines.end(), ostream_iterator<string>(fout, "\n"));
    }
}

void formater::createHtmlDocument() {

    // add line breaks at the end of each line
    for (list <string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
        basic_stringstream<char> psz2;
        *iter = psz2.str().append(*iter).append("</p>");
    }

    // add css styles at the top of the document
    string css_settings;
    css_settings.append("<style TYPE='text/css'> <!-- ")
            .append("p { line-height: 100%; } ")
            .append("body { font-family: 'courier new'; line-height: 100%; } ")
            .append("--> </style>");
    lines.push_front(css_settings.c_str());
}

void formater::formatPre() {
    line_status ls;
    for (list <string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
        // encode strings with base64 to avoid formating
        parseLine(*iter, ls, true);

        // replace pattern
        *iter = replacePattern(replacePatternPreprocessing, *iter, 1);
    }
    for_each(lines.begin(), lines.end(), trimRight);
}

void formater::formatPost() {
    line_status ls;
    for (list <string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
        string line = *iter;

        // calcualte layer based on commands
        for_each(spl_keywords.begin(), spl_keywords.end(), layer_counter(&ls, line));

        // set special color for commands 
        if (createHtml) {
            line = for_each(spl_keywords.begin(), spl_keywords.end(), formater_mark_html(line));
        }

        // decode strings  
        parseLine(line, ls, false);

        // remove some strings
        line = replacePattern(replacePatternPostprocessing, line, 5);

        // create indenting (but not for first line)
        if (lines.begin() != iter) {
            createIndenting(line, ls);
        }

        // store current line
        *iter = line;

        // reset state for next line
        ls.SetLayerCountOnce(0);
    }

    for_each(lines.begin(), lines.end(), trimRight);
}

string formater::replacePattern(map_string pattern, string s1, int iterations) {
    string line = s1;
    typedef map<string, string>::iterator it_type;
    for (it_type iterator = pattern.begin(); iterator != pattern.end(); iterator++) {
        for (int i = 0; i < iterations; i++) {
            string s2 = iterator->first;
            string s3 = iterator->second;
            index_string anf = line.find(s2, 0);
            while (string::npos != anf) {
                string right(line.substr(anf + s2.length(), string::npos));
                string left(line.substr(0, anf));
                line = left.append(s3).append(right);
                anf = line.find(s2, left.length());
            }
        }
    }
    return line;
}

bool formater::parseLine(string &line, line_status & ls, bool encode) {
    index_string indexStart = 0;
    index_string index = 0;

    if (createHtml && !encode) {
        insertHtmlFont(index, line, ls);
    }

    do {
        if (ls.inCode()) {
            if (encode && (!ls.inBrackets() && line[index] == ',' || line[index] == '|' || line[index] == '[')) {
                line.insert(index, "\n");
                index++;
            }

            if (line[index] == '(') {
                ls.SetBracketsCount(ls.GetBracketsCount() + 1);
            }

            if (line[index] == '"') {
                ls.SetActiveDoubleQuoteString();
                if (createHtml && !encode) {
                    insertHtmlFont(index, line, ls);
                }
                indexStart = index;
            } else if (line[index] == '\'') {
                ls.SetActiveSingleQuoteString();
                if (createHtml && !encode) {
                    insertHtmlFont(index, line, ls);
                }
                indexStart = index;
            } else if (line[index] == '`') {
                ls.SetActiveMacro();
                if (createHtml && !encode) {
                    insertHtmlFont(index, line, ls);
                }
                indexStart = index;
            }
        } else if (ls.inDoubleQuoteString() || ls.inSingleQuoteString()) {
            if ((ls.inDoubleQuoteString() && line[index] == '"' && index > 1 && line[index - 1] != '\\') ||
                    (ls.inSingleQuoteString() && line[index] == '\'' && index > 1 && line[index - 1] != '\\')) {
                if (encode) {
                    int indexStart1 = indexStart + 1;
                    int string_length = (index - indexStart - 1);
                    string sub_string2 = line.substr(indexStart1, string_length);
                    string encoded2 = base64_encode(sub_string2);
                    line = line.replace(indexStart1, string_length, encoded2);
                    index -= sub_string2.length();
                    index += encoded2.length();
                } else {
                    int indexStart1 = indexStart + 1;
                    int string_length = (index - indexStart - 1);
                    string sub_string = line.substr(indexStart1, string_length);
                    string decoded = base64_decode(sub_string);
                    line = line.replace(indexStart1, string_length, decoded);
                    index -= sub_string.length();
                    index += decoded.length();
                }

                ls.SetActiveCode();
                if (createHtml && !encode) {
                    insertHtmlFont(++index, line, ls);
                }
                indexStart = index;
            }
        } else if (ls.inMacro()) {
            if (line[index] == '`') {
                if (encode) {
                    int indexStart1 = indexStart + 1;
                    int string_length = (index - indexStart - 1);
                    string sub_string2 = line.substr(indexStart1, string_length);
                    string encoded2 = base64_encode(sub_string2);
                    line = line.replace(indexStart1, string_length, encoded2);
                    index -= sub_string2.length();
                    index += encoded2.length();
                } else {
                    int indexStart1 = indexStart + 1;
                    int string_length = (index - indexStart - 1);
                    string sub_string = line.substr(indexStart1, string_length);
                    string decoded = base64_decode(sub_string);
                    line = line.replace(indexStart1, string_length, decoded);
                    index -= sub_string.length();
                    index += decoded.length();
                }

                ls.SetActiveCode();
                if (createHtml && !encode) {
                    insertHtmlFont(++index, line, ls);
                }
                indexStart = index;
            }
        }

        if (ls.inCode() && ls.inBrackets()) {
            if (line[index] == ')') {
                ls.SetBracketsCount(ls.GetBracketsCount() - 1);
            }
        }

    } while (++index <= line.size());

    if (ls.inDoubleQuoteString()) {
        result = "invalid state - double quote string";
    } else if (ls.inSingleQuoteString()) {
        result = "invalid state - single quote string";
    } else if (ls.inMacro()) {
        result = "invalid state - macro";
    } else if (ls.inBrackets()) {
        result = "invalid state - brackets";
    }

    return false;
}

void formater::createIndenting(string &line, line_status & ls) {
    for (int i = 0; i < ls.GetLayerCountTotal(); i++) {
        line.insert(0, createHtml ? "&nbsp;&nbsp;&nbsp;" : "   ");
    }
}

void formater::replaceSubstrings(const index_string& begin, index_string& end, string & s) {
    if (begin != string::npos && end != string::npos && begin < end) {
        string midNeu;
        string mid = s.substr(begin, end - begin + 1).c_str();

        if (createHtml) {
            midNeu = for_each(spl_keywords.begin(), spl_keywords.end(), formater_mark_html(mid));
        } else {
            midNeu = for_each(replacePatternPreprocessing.begin(), replacePatternPreprocessing.end(), formater_replace(mid));
        }
        s = s.substr(0, begin).append(midNeu).append(s.substr(end + 1, string::npos).c_str());
        end = begin + midNeu.length() - 1;
    }
}

void formater::createNewLineIfNeeded() {
    list<string> result;
    for (list <string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
        string line = *iter;
        std::vector<std::string> lines = split(line, '\n');
        for (string copied_line : lines) {
            result.push_back(copied_line);
        }
    }
    lines = result;
}

string formater::GetHtmlFontTag(unsigned long id) {
    const string g_sFont[] = {
        formater_mark_html::FONT_CODE,
        formater_mark_html::FONT_DOUBLE_QUOTE,
        formater_mark_html::FONT_SINGLE_QUOTE,
        formater_mark_html::FONT_MACRO
    };
    if (id <= 3)
        return g_sFont[id];
    else
        return "<FONT>";
}

void formater::insertHtmlFont(index_string& pos, string& s, line_status & ls) {
    s.insert(pos, GetHtmlFontTag(ls.GetStatus()));
    pos += GetHtmlFontTag(ls.GetStatus()).length();
}
