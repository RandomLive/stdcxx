// checking if integer math traps

/***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  License); you may  not use  this file
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
 * Copyright 1999-2007 Rogue Wave Software, Inc.
 * 
 **************************************************************************/

#include "config.h"


#if (defined (_WIN32) || defined (_WIN64)) && !defined (__CYGWIN__)
#  define TRY             __try
#  define EXCEPT(expr)    __except (expr)
#else   // not Windows
#  define TRY               if (1)
#  define EXCEPT(ignore)    else if (0)
#endif   // _WIN{32,64}


int get_int ();

int main (int argc, char*[])
{
    int int_zero = get_int ();
    int int_one  = get_int ();

    int result;

    TRY {
        result = int_one / int_zero;
    }
    EXCEPT (1) {
        return 1;
    }

    // NEGATIVE test: successful exit status indicates a failure
    return argc < 2 ? 0 : result;
}

// foil optimizers
volatile int int_value = 0;

int get_int ()
{
    return int_value++;
}
