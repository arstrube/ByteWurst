/******************************************************************************
 * Copyright (c) 2014 Arnd R. Strube. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "CppUTest/SimpleString.h"
#include "ByteWurst/ByteWurst.h"

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}

TEST_GROUP( ByteWurst )
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST( ByteWurst, StringFromOK )
{
    char bytes [] = "0123456789@ABCabcd";
    char expected [] = "0x30313233343536373839404142436162636400";
    ByteWurst byteWurst ( sizeof( bytes) );
    byteWurst.Put (bytes, sizeof( bytes) );

    STRCMP_EQUAL( expected, StringFrom( byteWurst ).asCharString() );
}

TEST( ByteWurst, OperatorEqualTrueOK )
{
    unsigned char bytes1 [7] = {
        0x00, 0x01,0x02,0x03,0x04,0x05,0x06
    };
    unsigned char bytes2 [7] = {
        0x00, 0x01,0x02,0x03,0x04,0x05,0x06
    };
    ByteWurst salami ( sizeof( bytes1 ) );
    ByteWurst poloni ( sizeof( bytes2 ) );
    salami.Put( bytes1, sizeof( bytes1 ) );
    poloni.Put( bytes2, sizeof( bytes2 ) );

    CHECK_TRUE ( salami == poloni );
}

TEST( ByteWurst, OperatorEqualTrueSameOK )
{
    unsigned char bytes [7] = {
        0x00, 0x01,0x02,0x03,0x04,0x05,0x06
    };
    ByteWurst salami ( sizeof( bytes ) );
    salami.Put( bytes, sizeof( bytes ) );

    CHECK_TRUE ( salami == salami );
}

TEST( ByteWurst, OperatorEqualFalseContentsOK )
{
    unsigned char bytes1 [7] = {
        0x00, 0x01,0x02,0x03,0x04,0x05,0x06
    };
    unsigned char bytes2 [7] = {
        0x00, 0x01,0x32,0x03,0x04,0x05,0x06
    };
    ByteWurst salami ( sizeof( bytes1 ) );
    ByteWurst poloni ( sizeof( bytes2 ) );
    salami.Put( bytes1, sizeof( bytes1 ) );
    poloni.Put( bytes2, sizeof( bytes2 ) );

    CHECK_FALSE ( salami == poloni );
}

TEST( ByteWurst, OperatorEqualFalseSizeOK )
{
    unsigned char bytes1 [8] = {
        0x00, 0x01,0x02,0x03,0x04,0x05,0x06,0x07
    };
    unsigned char bytes2 [7] = {
        0x00, 0x01,0x02,0x03,0x04,0x05,0x06
    };
    ByteWurst salami ( sizeof( bytes1 ) );
    ByteWurst poloni ( sizeof( bytes2 ) );
    salami.Put( bytes1, sizeof( bytes1 ) );
    poloni.Put( bytes2, sizeof( bytes2 ) );

    CHECK_FALSE ( salami == poloni );
}

TEST( ByteWurst, PutAndGetSizedOK )
{
    char expected[] = "This is a salami";
    ByteWurst actual( sizeof( expected ) );
    actual.Put( expected, sizeof( expected ) );

    LONGS_EQUAL( sizeof( expected ), actual.Length() );
    STRCMP_EQUAL( expected, (char * const) actual.Get() );
}

TEST( ByteWurst, PutAndGetDefaultOK )
{
    char expected = 'X';
    ByteWurst * actual = new ByteWurst();
    CHECK_TRUE ( NULL != actual );
    actual->Put( &expected, 1 );
    BYTES_EQUAL( expected, * (char *) actual->Get() )
    delete actual;
}

TEST( ByteWurst, CreateDefaultOK )
{
    ByteWurst * actual = new ByteWurst();
    CHECK_TRUE ( NULL != actual );

    delete actual;
}

TEST( ByteWurst, IncorrectSizeHandledOK )
{
    char buff1[] = "000000";
    char buff2[] = "111111";
    ByteWurst test(sizeof(buff1));
    test.Put(buff1, sizeof(buff1));
    test.Put(buff2, 3);
    STRCMP_EQUAL("0x31313130303000", StringFrom(test).asCharString())
}

