/***************************************************************************
 *
 * 20.tuple.rel.cpp - tests exercising tuple relational operators
 *
 * $Id$
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  "License"); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 2008 Rogue Wave Software, Inc.
 *
 **************************************************************************/

#include <rw_driver.h>

// compile out all test code if extensions disabled
#ifndef _RWSTD_NO_EXT_CXX_0X

#include <tuple>

#include "20.tuple.h"

/**************************************************************************/

static void
test_eq ()
{
    rw_info (0, __FILE__, __LINE__, "operator==");

    EmptyTuple nt1, nt2;
    rw_assert (nt1 == nt1, __FILE__, __LINE__,
               "nt1 == nt1, got false, expected true");
    rw_assert (nt1 == nt2, __FILE__, __LINE__,
               "nt1 == nt2, got true, expected false");

    IntTuple it1 (1), it2 (1);
    rw_assert (it1 == it1, __FILE__, __LINE__,
               "it1 == it1, got false, expected true");
    rw_assert (it1 == it2, __FILE__, __LINE__,
               "it1 == it2, got false, expected true");

    UserClass uc;
    UserTuple ut1 (uc), ut2 (uc);
    rw_assert (ut1 == ut1, __FILE__, __LINE__,
               "ut1 == ut1, got false, expected true");
    rw_assert (ut1 == ut2, __FILE__, __LINE__,
               "ut1 == ut2, got false, expected true");

    BigTuple bt1 (true, 'a', 256, 3.14159, &nt1, uc);
    BigTuple bt2 (true, 'a', 256, 3.14159, &nt1, uc);
    rw_assert (bt1 == bt1, __FILE__, __LINE__,
               "bt1 == bt1, got false, expected true");
    rw_assert (bt1 == bt2, __FILE__, __LINE__,
               "bt1 == bt2, got false, expected true");
}

/**************************************************************************/

static void
test_lt ()
{
    rw_info (0, __FILE__, __LINE__, "operator<");

    EmptyTuple nt1, nt2;
    rw_assert (!(nt1 < nt1), __FILE__, __LINE__,
               "nt1 < nt1, got true, expected false");
    rw_assert (!(nt1 < nt2), __FILE__, __LINE__,
               "nt1 < nt2, got true, expected false");

    IntTuple it1 (1), it2 (2);
    rw_assert (it1 < it2, __FILE__, __LINE__,
               "it1 < it2, got false, expected true");

    UserClass uc1, uc2;
    uc1.data_.val_ = 1, uc2.data_.val_ = 2;

    UserTuple ut1 (uc1), ut2 (uc2);
    rw_assert (ut1 < ut2, __FILE__, __LINE__,
               "ut1 < ut2, got false, expected true");

    BigTuple bt1 (true, 'a', 255, 3.14159, &nt1, uc1);
    BigTuple bt2 (true, 'a', 256, 3.14159, &nt1, uc1);
    rw_assert (bt1 < bt2, __FILE__, __LINE__,
               "bt1 < bt2, got false, expected true");
}

/**************************************************************************/

static int
run_test (int /*argc*/, char* /*argv*/ [])
{
    test_eq ();
    test_lt ();

    //test_ne ();
    //test_gt ();
    //test_le ();
    //test_ge ();

    return 0;
}

#else // !_RWSTD_NO_EXT_CXX_0X

static int
run_test (int, char*[])
{
    rw_info (0, 0, __LINE__,
             "tests for C++0x tuple extension disabled");
    return 0;
}

#endif // !_RWSTD_NO_EXT_CXX_0X

/*extern*/ int
main (int argc, char* argv [])
{
    return rw_test (argc, argv, __FILE__,
                    "[tuple.rel]", "20.3.1.6  Relational operators",
                    run_test, "", 0);
}

