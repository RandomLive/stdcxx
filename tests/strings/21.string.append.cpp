/***************************************************************************
 *
 * 21.string.append.cpp - string test exercising lib.string.append
 *
 * $Id$
 *
 ***************************************************************************
 *
 * Copyright 2006 The Apache Software Foundation or its licensors,
 * as applicable.
 *
 * Copyright 2006 Rogue Wave Software.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************/

#include <string>         // for string
#include <stdexcept>      // for out_of_range, length_error

#include <cstddef>        // for size_t

#include <21.strings.h>   // for StringMembers
#include <alg_test.h>     // for InputIter
#include <driver.h>       // for rw_test()
#include <rw_char.h>      // for rw_widen()
#include <rw_new.h>       // for bad_alloc, replacement operator new


/**************************************************************************/

// for convenience and brevity
#define LSTR            StringMembers::long_string
#define LLEN            StringMembers::long_string_len
#define Append(which)   StringMembers::append_ ## which

typedef StringMembers::OverloadId OverloadId;
typedef StringMembers::TestCase   TestCase;
typedef StringMembers::Test       Test;
typedef StringMembers::Function   MemFun;


static const char* const exceptions[] = {
    "unknown exception", "out_of_range", "length_error",
    "bad_alloc", "exception"
};

/**************************************************************************/

// exercises:
// append (const value_type*)
static const TestCase
ptr_test_cases [] = {

#undef TEST
#define TEST(str, arg, res, bthrow) {                           \
        __LINE__, -1, -1, -1, -1, -1,                           \
        str, sizeof str - 1,                                    \
        arg, sizeof arg - 1, res, sizeof res - 1, bthrow        \
    }

    //    +----------------------------------------- controlled sequence
    //    |             +--------------------------- sequence to be appended
    //    |             |             +------------- expected result sequence
    //    |             |             |        +---- exception info
    //    |             |             |        |         0 - no exception
    //    |             |             |        |         1 - out_of_range
    //    |             |             |        |         2 - length_error
    //    |             |             |        |        -1 - exc. safety
    //    |             |             |        |
    //    |             |             |        +--------------+
    //    V             V             V                       V
    TEST ("ab",         "c",          "abc",                  0),

    TEST ("",           "",           "",                     0),
    TEST ("",           "\0",         "",                     0),
    TEST ("",           "abc",        "abc",                  0),

    TEST ("\0",         "",           "\0",                   0),
    TEST ("\0",         "a",          "\0a",                  0),
    TEST ("\0",         "\0\0",       "\0",                   0),

    TEST ("ab",         "cd",         "abcd",                 0),
    TEST ("bcd",        "a",          "bcda",                 0),
    TEST ("cde",        "ab",         "cdeab",                0),
    TEST ("abc",        "",           "abc",                  0),
    TEST ("ab",         "c\0e",       "abc",                  0),

    TEST ("\0\0ab",     "cdefghij",   "\0\0abcdefghij",       0),
    TEST ("a\0\0b",     "cdefghij",   "a\0\0bcdefghij",       0),
    TEST ("ab\0\0",     "cdefghij",   "ab\0\0cdefghij",       0),
    TEST ("a\0b\0\0c",  "e\0",        "a\0b\0\0ce",           0),
    TEST ("\0ab\0\0c",  "e\0",        "\0ab\0\0ce",           0),
    TEST ("abcdefghij", "abcdefghij", "abcdefghijabcdefghij", 0),

    TEST ("",           LSTR,         LSTR,                   0),
    TEST (LSTR,         "",           LSTR,                   0),

    TEST ("",           0,            "",                     0),
    TEST ("abc",        0,            "abcabc",               0),
    TEST ("a\0\0bc",    0,            "a\0\0bca",             0),
    TEST ("\0\0abc",    0,            "\0\0abc",              0),
    TEST ("abc\0\0",    0,            "abc\0\0abc",           0),

    TEST ("",           LSTR,         LSTR,                  -1),

    TEST ("last",       "test",       "lasttest",             0)
};

/**************************************************************************/

// exercises:
// append (const basic_string&)
static const TestCase
str_test_cases [] = {

#undef TEST
#define TEST(s, arg, res, bthrow) {                             \
        __LINE__, -1, -1, -1, -1, -1,                           \
        s, sizeof s - 1,                                        \
        arg, sizeof arg - 1, res, sizeof res - 1, bthrow        \
    }

    //    +----------------------------------------- controlled sequence
    //    |             +--------------------------- sequence to be appended
    //    |             |             +------------- expected result sequence
    //    |             |             |        +---- exception info
    //    |             |             |        |         0 - no exception
    //    |             |             |        |         1 - out_of_range
    //    |             |             |        |         2 - length_error
    //    |             |             |        |        -1 - exc. safety
    //    |             |             |        |
    //    |             |             |        +--------------+
    //    V             V             V                       V
    TEST ("ab",         "c",          "abc",                  0),

    TEST ("",           "",           "",                     0),
    TEST ("",           "\0",         "\0",                   0),
    TEST ("",           "abc",        "abc",                  0),

    TEST ("\0",         "",           "\0",                   0),
    TEST ("\0",         "a",          "\0a",                  0),
    TEST ("\0",         "\0\0",       "\0\0\0",               0),

    TEST ("ab",         "cd",         "abcd",                 0),
    TEST ("bcd",        "a",          "bcda",                 0),
    TEST ("cde",        "ab",         "cdeab",                0),
    TEST ("abc",        "",           "abc",                  0),
    TEST ("ab",         "c\0e",       "abc\0e",               0),

    TEST ("\0\0ab",     "cdefghij",   "\0\0abcdefghij",       0),
    TEST ("a\0\0b",     "cdefghij",   "a\0\0bcdefghij",       0),
    TEST ("ab\0\0",     "cdefghij",   "ab\0\0cdefghij",       0),
    TEST ("a\0b\0\0c",  "e\0",        "a\0b\0\0ce\0",         0),
    TEST ("\0ab\0\0c",  "e\0",        "\0ab\0\0ce\0",         0),
    TEST ("ab\0\0c\0",  "\0e",        "ab\0\0c\0\0e",         0),
    TEST ("abcdefghij", "abcdefghij", "abcdefghijabcdefghij", 0),

    TEST ("",           LSTR,         LSTR,                   0),
    TEST (LSTR,         "",           LSTR,                   0),

    TEST ("",           0,            "",                     0),
    TEST ("abc",        0,            "abcabc",               0),
    TEST ("a\0\0bc",    0,            "a\0\0bca\0\0bc",       0),
    TEST ("\0\0abc",    0,            "\0\0abc\0\0abc",       0),
    TEST ("abc\0\0",    0,            "abc\0\0abc\0\0",       0),

    TEST ("",           LSTR,         LSTR,                  -1),

    TEST ("last",       "test",       "lasttest",             0)
};

/**************************************************************************/

// exercises:
// append (const value_type*, size_type)
static const TestCase
ptr_size_test_cases [] = {

#undef TEST
#define TEST(str, arg, size, res, bthrow) {                     \
        __LINE__, -1, size, -1, -1, -1,                         \
        str, sizeof str - 1,                                    \
        arg, sizeof arg - 1, res, sizeof res - 1, bthrow        \
    }

    //    +-------------------------------------- controlled sequence
    //    |            +------------------------- sequence to be appended
    //    |            |            +------------ append() n argument
    //    |            |            |  +--------- expected result sequence
    //    |            |            |  |     +--- exception info
    //    |            |            |  |     |      0 - no exception
    //    |            |            |  |     |      1 - out_of_range
    //    |            |            |  |     |      2 - length_error
    //    |            |            |  |     |     -1 - exc. safety
    //    |            |            |  |     |
    //    |            |            |  |     +------------+
    //    V            V            V  V                  V
    TEST ("ab",        "c",         1, "abc",             0),

    TEST ("",          "",          0,  "",               0),
    TEST ("",          "abc",       1,  "a",              0),
    TEST ("",          "\0",        1,  "\0",             0),

    TEST ("\0",        "",          0,  "\0",             0),
    TEST ("\0",        "a",         0,  "\0",             0),
    TEST ("\0",        "a",         1,  "\0a",            0),
    TEST ("\0",        "\0\0",      1,  "\0\0",           0),
    TEST ("\0",        "\0\0",      2,  "\0\0\0",         0),

    TEST ("cde",       "ab",        2,  "cdeab",          0),
    TEST ("cde",       "ab",        1,  "cdea",           0),

    TEST ("\0e\0",     "a\0b\0\0c", 0,  "\0e\0",          0),
    TEST ("\0e\0",     "\0ab\0\0c", 3,  "\0e\0\0ab",      0),

    TEST ("a\0b\0\0c", "\0e\0",     3,  "a\0b\0\0c\0e\0", 0),
    TEST ("a\0b\0\0c", "\0\0e\0",   2,  "a\0b\0\0c\0\0",  0),
    TEST ("\0ab\0\0c", "\0e\0",     1,  "\0ab\0\0c\0",    0),
    TEST ("a\0bc\0\0", "\0e",       2,  "a\0bc\0\0\0e",   0),

    TEST ("",          0,           0,  "",               0),
    TEST ("abc",       0,           0,  "abc",            0),
    TEST ("abc",       0,           1,  "abca",           0),
    TEST ("abc",       0,           2,  "abcab",          0),
    TEST ("a\0bc",     0,           2,  "a\0bca\0",       0),
    TEST ("\0abc\0\0", 0,           1,  "\0abc\0\0\0",    0),
    TEST ("a\0bc\0\0", 0,           3,  "a\0bc\0\0a\0b",  0),

    TEST ("",          LSTR, LLEN - 1,  LSTR,             0),
    TEST (LSTR,        "",          0,  LSTR,             0),

    TEST ("",          LSTR, LLEN - 1,  LSTR,            -1),

    TEST ("last",      "test",      4, "lasttest",        0)
};

/**************************************************************************/

// exercises:
// append (const basic_string&, size_type, size_type)
// append (InputIterator, InputIterator)
static const TestCase
range_test_cases [] = {

// range_test_cases serves a double duty
#define str_size_size_test_cases range_test_cases

#undef TEST
#define TEST(str, arg, off, size, res, bthrow) {                \
        __LINE__, off, size, -1, -1, -1,                        \
        str, sizeof str - 1,                                    \
        arg, sizeof arg - 1, res, sizeof res - 1, bthrow        \
    }

    //    +-------------------------------------- controlled sequence
    //    |            +------------------------- sequence to be appended
    //    |            |            +------------ append() pos argument
    //    |            |            |  +--------- append() n argument
    //    |            |            |  |  +------ expected result sequence
    //    |            |            |  |  |  +--- exception info
    //    |            |            |  |  |  |       0 - no exception
    //    |            |            |  |  |  |       1 - out_of_range
    //    |            |            |  |  |  |       2 - length_error
    //    |            |            |  |  |  |      -1 - exc. safety
    //    |            |            |  |  |  |
    //    |            |            |  |  |  +----------------+
    //    V            V            V  V  V                   V
    TEST ("ab",        "c",         0, 1, "abc",              0),

    TEST ("",          "",          0, 0,  "",                0),
    TEST ("",          "abc",       1, 1,  "b",               0),
    TEST ("",          "\0",        0, 1,  "\0",              0),

    TEST ("\0",        "",          0, 0,  "\0",              0),

    TEST ("abc",       "",          0, 0,  "abc",             0),

    TEST ("\0",        "a",         0, 1,  "\0a",             0),
    TEST ("\0",        "\0\0",      1, 1,  "\0\0",            0),
    TEST ("\0",        "\0\0",      0, 2,  "\0\0\0",          0),
    TEST ("\0",        "\0\0",      1, 5,  "\0\0",            0),

    TEST ("cde",       "ab",        0, 2,  "cdeab",           0),
    TEST ("cde",       "ab",        0, 1,  "cdea",            0),
    TEST ("cde",       "ab",        1, 5,  "cdeb",            0),

    TEST ("ab",        "c\0e",      0, 3,  "abc\0e",          0),
    TEST ("ab",        "c\0e",      1, 2,  "ab\0e",           0),
    TEST ("ab",        "c\0e",      0, 2,  "abc\0",           0),

    TEST ("\0e\0",     "\0ab\0\0c", 0, 9,  "\0e\0\0ab\0\0c",  0),
    TEST ("\0e\0",     "\0ab\0\0c", 0, 3,  "\0e\0\0ab",       0),
    TEST ("a\0b\0\0c", "\0e\0",     0, 3,  "a\0b\0\0c\0e\0",  0),
    TEST ("a\0b\0\0c", "\0\0e\0",   0, 2,  "a\0b\0\0c\0\0",   0),
    TEST ("\0ab\0\0c", "\0e\0",     2, 1,  "\0ab\0\0c\0",     0),
    TEST ("a\0bc\0\0", "\0e",       0, 2,  "a\0bc\0\0\0e",    0),

    TEST ("",          0,           0, 0,  "",                0),
    TEST ("abc",       0,           1, 0,  "abc",             0),
    TEST ("abc",       0,           1, 1,  "abcb",            0),
    TEST ("abc",       0,           0, 2,  "abcab",           0),
    TEST ("a\0bc\0\0", 0,           4, 2,  "a\0bc\0\0\0\0",   0),
    TEST ("a\0bc\0\0", 0,           1, 3,  "a\0bc\0\0\0bc",   0),
    TEST ("a\0bc\0\0", 0,           3, 9,  "a\0bc\0\0c\0\0",  0),
    TEST ("abcdef",    0,           1, 2,  "abcdefbc",        0),

    TEST ("",          LSTR,        0, LLEN, LSTR,            0),
    TEST (LSTR,        "",          0, 0,    LSTR,            0),

    TEST ("",          "\0",        2, 0,  "",                1),
    TEST ("",          "a",         2, 0,  "",                1),
    TEST ("",          LSTR,LLEN + 10, 0,  "",                1),

    TEST (LSTR,        0,           0, 0,  LSTR,             -1),

    TEST ("last",      "test",      0, 4, "lasttest",         0)
};

/**************************************************************************/

// exercises:
// append (value_type, size_type)
static const TestCase
size_val_test_cases [] = {

#undef TEST
#define TEST(str, size, val, res, bthrow) {     \
        __LINE__, -1, size, -1, -1, val,        \
        str, sizeof str - 1,                    \
        0, 0, res, sizeof res - 1, bthrow       \
    }

    //    +-------------------------------------- controlled sequence
    //    |            +------------------------- append() count argument
    //    |            |   +--------------------- character to be appended
    //    |            |   |   +----------------- expected result sequence
    //    |            |   |   |       +--------- exception info
    //    |            |   |   |       |             0 - no exception
    //    |            |   |   |       |             1 - out_of_range
    //    |            |   |   |       |             2 - length_error
    //    |            |   |   |       |            -1 - exc. safety
    //    |            |   |   |       |
    //    |            |   |   |       +-----------+
    //    V            V   V   V                   V
    TEST ("ab",        1, 'c', "abc",              0),

    TEST ("",          0, ' ',  "",                0),
    TEST ("",          1, 'b',  "b",               0),
    TEST ("",          3, 'b',  "bbb",             0),

    TEST ("\0",        0, ' ',  "\0",              0),
    TEST ("",          2, '\0', "\0\0",            0),

    TEST ("\0",        1, 'a',  "\0a",             0),
    TEST ("\0",        1, '\0', "\0\0",            0),
    TEST ("\0",        2, '\0', "\0\0\0",          0),
    TEST ("\0",        0, '\0', "\0",              0),

    TEST ("cde",       1, 'a',  "cdea",            0),
    TEST ("cde",       2, 'a',  "cdeaa",           0),
    TEST ("cde",       3, 'a',  "cdeaaa",          0),

    TEST ("ab",        2, '\0', "ab\0\0",          0),
    TEST ("ab",        1, '\0', "ab\0",            0),
    TEST ("ab",        2, '\0', "ab\0\0",          0),

    TEST ("a\0b\0\0c", 2, '\0', "a\0b\0\0c\0\0",   0),
    TEST ("a\0b\0\0c", 1, '\0', "a\0b\0\0c\0",     0),
    TEST ("\0ab\0\0c", 3, '\0', "\0ab\0\0c\0\0\0", 0),
    TEST ("a\0bc\0\0", 2, 'a',  "a\0bc\0\0aa",     0),

    TEST ("",          LLEN - 1, 'x', LSTR,        0),
    TEST (LSTR,        0,        'x', LSTR,        0),

    TEST ("",          LLEN - 1, 'x', LSTR,       -1),

    TEST ("last",      4, 't', "lasttttt",         0)
};

/**************************************************************************/

template <class charT, class Traits, class Iterator>
void test_append_range (charT          *wstr,
                        charT          *wsrc,
                        Traits*,
                        const Iterator &it,
                        const TestCase &tcase)
{
    typedef std::allocator<charT>                        Allocator;
    typedef std::basic_string <charT, Traits, Allocator> String;
    typedef typename String::iterator                    StringIter;

    const char* const itname =
        tcase.arg ? type_name (it, (charT*)0) : "basic_string::iterator";

    /* const */ String s_str (wstr, tcase.str_len);
    const       String s_arg (wsrc, tcase.arg_len);

    std::size_t off_last = tcase.off + tcase.size;

    if (tcase.arg) {
        if (off_last > s_arg.size ())
            off_last = s_arg.size ();

        const charT* const beg = wsrc + tcase.off;
        const charT* const end = wsrc + off_last;

        const Iterator first (beg, beg, end);
        const Iterator last  (end, beg, end);

        s_str.append (first, last);
    }
    else {
        const StringIter first (s_str.begin () + tcase.off);
        const StringIter last  (s_str.size () < off_last ?
                                s_str.end ()
                                : s_str.begin () + off_last);

        s_str.append (first, last);
    }

    const std::size_t match =
        rw_match (tcase.res, s_str.c_str(), tcase.nres);

    rw_assert (match == tcase.nres, 0, tcase.line,
               "line %d. %{$FUNCALL} expected %{#*s}, got %{/*.*Gs}, "
               "difference at offset %zu for %s",
               __LINE__, int (tcase.nres), tcase.res,
               int (sizeof (charT)), int (s_str.size ()), s_str.c_str (),
               match, itname);
}

/**************************************************************************/

template <class charT, class Traits>
void test_append_range (charT* wstr,
                        charT* wsrc,
                        Traits*,
                        const TestCase &tcase)
{
    if (tcase.bthrow)  // this method doesn't throw
        return;

    test_append_range (wstr, wsrc, (Traits*)0,
                       InputIter<charT>(0, 0, 0), tcase);

    // there is no need to call test_append_range
    // for other iterators in this case
    if (0 == tcase.arg)
        return;

    test_append_range (wstr, wsrc, (Traits*)0,
                       ConstFwdIter<charT>(0, 0, 0), tcase);

    test_append_range (wstr, wsrc, (Traits*)0,
                       ConstBidirIter<charT>(0, 0, 0), tcase);

    test_append_range (wstr, wsrc, (Traits*)0,
                       ConstRandomAccessIter<charT>(0, 0, 0), tcase);
}

/**************************************************************************/

template <class charT, class Traits>
void test_append (charT, Traits*,
                  OverloadId      which,
                  const TestCase &tcase)
{
    typedef std::allocator<charT>                        Allocator;
    typedef std::basic_string <charT, Traits, Allocator> TestString;
    typedef typename TestString::iterator                StringIter;
    typedef typename TestString::const_iterator          ConstStringIter;

    typedef typename UserTraits<charT>::MemFun UTMemFun;

    static charT wstr [LLEN];
    static charT wsrc [LLEN];

    rw_widen (wstr, tcase.str, tcase.str_len);
    rw_widen (wsrc, tcase.arg, tcase.arg_len);

    // special processing for append_range to exercise all iterators
    if (Append (range) == which) {
        test_append_range (wstr, wsrc, (Traits*)0, tcase);
        return;
    }

    /* const */ TestString s_str (wstr, tcase.str_len);
    const       TestString s_arg (wsrc, tcase.arg_len);

    std::size_t res_off     = 0;

    // save the state of the string object before the call
    // to detect wxception safety violations (changes to
    // the state of the object after an exception)
    const StringState str_state (rw_get_string_state (s_str));

    const charT* const ptr_arg = tcase.arg ? wsrc : s_str.c_str ();
    const TestString&  str_arg = tcase.arg ? s_arg : s_str;

    std::size_t total_length_calls = 0;
    std::size_t n_length_calls = 0;
    std::size_t* const rg_calls = rw_get_call_counters ((Traits*)0, (charT*)0);

    if (rg_calls)
        total_length_calls = rg_calls [UTMemFun::length];

    rwt_free_store* const pst = rwt_get_free_store (0);

    // iterate for`throw_after' starting at the next call to operator new,
    // forcing each call to throw an exception, until the function finally
    // succeeds (i.e, no exception is thrown)
    std::size_t throw_after;
    for (throw_after = 0; ; ++throw_after) {

        // (name of) expected and caught exception
        const char* expected = 0;
        const char* caught   = 0;

#ifndef _RWSTD_NO_EXCEPTIONS

        if (1 == tcase.bthrow && Append (str_size_size) == which)
            expected = exceptions [1];   // out_of_range
        else if (2 == tcase.bthrow)
            expected = exceptions [2];   // length_error
        else if (-1 == tcase.bthrow) {
            expected = exceptions [3];   // bad_alloc
            *pst->throw_at_calls_ [0] = pst->new_calls_ [0] + throw_after + 1;
        }

#else   // if defined (_RWSTD_NO_EXCEPTIONS)

        if (tcase.bthrow)
            return;

#endif   // _RWSTD_NO_EXCEPTIONS

        try {

            switch (which) {
            case Append (ptr): {
                const TestString& s_res = s_str.append (ptr_arg);
                res_off = &s_res - &s_str;
                if (rg_calls)
                    n_length_calls = rg_calls [UTMemFun::length];
                break;
            }

            case Append (str): {
                const TestString& s_res = s_str.append (str_arg);
                res_off = &s_res - &s_str;
                break;
            }

            case Append (ptr_size): {
                const TestString& s_res = s_str.append (ptr_arg, tcase.size);
                res_off = &s_res - &s_str;
                break;
            }

            case Append (str_size_size): {
                const TestString& s_res =
                    s_str.append (str_arg, tcase.off, tcase.size);
                res_off = &s_res - &s_str;
                break;
            }

            case Append (size_val): {
                const charT ch (make_char (char (tcase.val), (charT*)0));
                const TestString& s_res = s_str.append (tcase.size, ch);
                res_off = &s_res - &s_str;
                break;
            }

            default:
                RW_ASSERT (!"logic error: unknown append overload");
            }

            // verify the returned value
            rw_assert (0 == res_off, 0, tcase.line,
                       "line %d. %{$FUNCALL} returned invalid reference, "
                       "offset is %zu", __LINE__, res_off);

            // verify that strings are of equal length
            rw_assert (tcase.nres == s_str.size (), 0, tcase.line,
                       "line %d. %{$FUNCALL} expected %{#*s}, "
                       "length %zu, got %{/*.*Gs}, length %zu",
                       __LINE__, int (tcase.nres), tcase.res,
                       tcase.nres, int (sizeof (charT)),
                       int (s_str.size ()), s_str.c_str (), s_str.size ());

            if (tcase.nres == s_str.size ()) {
                // if the result length matches the expected length
                // (and only then), also verify that the modified
                // string matches the expected result
                const std::size_t match =
                    rw_match (tcase.res, s_str.c_str(), tcase.nres);

                rw_assert (match == tcase.nres, 0, tcase.line,
                           "line %d. %{$FUNCALL} expected %{#*s}, "
                           "got %{/*.*Gs}, difference at offset %zu",
                           __LINE__, int (tcase.nres), tcase.res,
                           int (sizeof (charT)), int (s_str.size ()),
                           s_str.c_str (), match);
            }

            // verify that Traits::length was used
            if (Append (ptr) == which && rg_calls) {
                rw_assert (n_length_calls - total_length_calls > 0, 
                           0, tcase.line, "line %d. %{$FUNCALL} doesn't "
                           "use traits::length()", __LINE__);
            }
        }

#ifndef _RWSTD_NO_EXCEPTIONS

        catch (const std::out_of_range &ex) {
            caught = exceptions [1];
            rw_assert (caught == expected, 0, tcase.line,
                       "line %d. %{$FUNCALL} %{?}expected %s,%{:}"
                       "unexpectedly%{;} caught std::%s(%#s)",
                       __LINE__, 0 != expected, expected, caught, ex.what ());
        }
        catch (const std::length_error &ex) {
            caught = exceptions [2];
            rw_assert (caught == expected, 0, tcase.line,
                       "line %d. %{$FUNCALL} %{?}expected %s,%{:}"
                       "unexpectedly%{;} caught std::%s(%#s)",
                       __LINE__, 0 != expected, expected, caught, ex.what ());
        }
        catch (const std::bad_alloc &ex) {
            caught = exceptions [3];
            rw_assert (-1 == tcase.bthrow, 0, tcase.line,
                       "line %d. %{$FUNCALL} %{?}expected %s,%{:}"
                       "unexpectedly%{;} caught std::%s(%#s)",
                       __LINE__, 0 != expected, expected, caught, ex.what ());
        }
        catch (const std::exception &ex) {
            caught = exceptions [4];
            rw_assert (0, 0, tcase.line,
                       "line %d. %{$FUNCALL} %{?}expected %s,%{:}"
                       "unexpectedly%{;} caught std::%s(%#s)",
                       __LINE__, 0 != expected, expected, caught, ex.what ());
        }
        catch (...) {
            caught = exceptions [0];
            rw_assert (0, 0, tcase.line,
                       "line %d. %{$FUNCALL} %{?}expected %s,%{:}"
                       "unexpectedly%{;} caught %s",
                       __LINE__, 0 != expected, expected, caught);
        }

#endif   // _RWSTD_NO_EXCEPTIONS

        if (caught) {
            // verify that an exception thrown during allocation
            // didn't cause a change in the state of the object
            str_state.assert_equal (rw_get_string_state (s_str),
                                    __LINE__, tcase.line, caught);

            if (-1 == tcase.bthrow) {
                // allow this call to operator new to succeed and try
                // to make the next one to fail during the next call
                // to the same function again
                continue;
            }
        }
        else if (-1 != tcase.bthrow) {
            rw_assert (caught == expected, 0, tcase.line,
                       "line %d. %{$FUNCALL} %{?}expected %s, caught %s"
                       "%{:}unexpectedly caught %s%{;}",
                       __LINE__, 0 != expected, expected, caught, caught);
        }

        break;
    }

#ifndef _RWSTD_NO_REPLACEABLE_NEW_DELETE

    // verify that if exceptions are enabled and when capacity changes
    // at least one exception is thrown
    rw_assert (   *pst->throw_at_calls_ [0] == std::size_t (-1)
               || throw_after,
               0, tcase.line,
               "line %d: %{$FUNCALL}: failed to throw an expected exception",
               __LINE__);

#endif   // _RWSTD_NO_REPLACEABLE_NEW_DELETE

    *pst->throw_at_calls_ [0] = std::size_t (-1);
}

/**************************************************************************/

DEFINE_TEST_DISPATCH (test_append);

int main (int argc, char** argv)
{
    static const StringMembers::Test
    tests [] = {

#undef TEST
#define TEST(tag) {                                             \
        StringMembers::append_ ## tag,                          \
        tag ## _test_cases,                                     \
        sizeof tag ## _test_cases / sizeof *tag ## _test_cases  \
    }

        TEST (ptr),
        TEST (str),
        TEST (ptr_size),
        TEST (str_size_size),
        TEST (size_val),
        TEST (range)
    };

    const std::size_t test_count = sizeof tests / sizeof *tests;

    return StringMembers::run_test (argc, argv, __FILE__,
                                    "lib.string.append",
                                    test_append, tests, test_count);
}
