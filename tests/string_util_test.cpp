/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   string_util_test.cpp
 * Author: Markus
 *
 * Created on 14.02.2016, 11:06:08
 */

#include "string_util_test.h"

#include "../src/string_utils.h"



CPPUNIT_TEST_SUITE_REGISTRATION(string_util_test);

string_util_test::string_util_test() {
}

string_util_test::~string_util_test() {
}

void string_util_test::setUp() {
}

void string_util_test::tearDown() {
}

void string_util_test::test_trimLeft_simple() {
    // given
    string value = "  abc";
    string expected = "abc";

    // when 
    trimLeft(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, value);
}

void string_util_test::test_trimLeft_not_trim_right() {
    // given
    string value = "  abc ";
    string expected = "abc ";

    // when 
    trimLeft(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, value);
}

void string_util_test::test_trimLeft_empty_string() {
    // given
    string value = "";
    string expected = "";

    // when 
    trimLeft(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, value);
}

void string_util_test::test_trimRight_simple() {
    // given
    string value = "abc ";
    string expected = "abc";

    // when 
    trimRight(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, value);
}

void string_util_test::test_trimRight_not_trim_left() {
    // given
    string value = "  abc  ";
    string expected = "  abc";

    // when 
    trimRight(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, value);
}

void string_util_test::test_trimRight_empty_string() {
    // given
    string value = "";
    string expected = "";

    // when 
    trimRight(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, value);
}

void string_util_test::test_encode() {
    // given
    string value = "1234567890AaBbCc ,.-!";
    string expected = "MTIzNDU2Nzg5MEFhQmJDYyAsLi0h";

    // when 
    string result = base64_encode(value);
    // cout << "'" << result << "'" << endl;

    // then
    CPPUNIT_ASSERT_EQUAL(expected, result);
}

void string_util_test::test_decode() {
    // given
    string value = "MTIzNDU2Nzg5MEFhQmJDYyAsLi0h";
    string expected = "1234567890AaBbCc ,.-!";

    // when 
    string result = base64_decode(value);

    // then
    CPPUNIT_ASSERT_EQUAL(expected, result);
}

