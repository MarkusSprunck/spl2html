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

/* 
 * File:   string_compare_test.h
 * Author: Markus
 *
 * Created on 15. Februar 2016, 20:56
 */

#ifndef STRING_COMPARE_TEST_H
#define STRING_COMPARE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <list>

using namespace std;

class string_compare_test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(string_compare_test);
    CPPUNIT_TEST(test_compare_equal_strings);
    CPPUNIT_TEST(test_compare_not_equal_strings);
    CPPUNIT_TEST_SUITE_END();

public:
    string_compare_test();
    virtual ~string_compare_test();
    void setUp();
    void tearDown();

private:

    void test_compare_equal_strings();
    void test_compare_not_equal_strings();

    list<string> firstStringList;
    list<string> secondStringList;
};

#endif /* STRING_COMPARE_TEST_H */

