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
 ********************************************************************************/

#include "CppUTest/SimpleString.h" /* For method StringFrom */
#include "ByteWurst/ByteWurst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ByteWurst::ByteWurst()
{
    Init( 1 );
}

ByteWurst::ByteWurst ( size_t length )
{
    Init ( length );
}

ByteWurst::ByteWurst( ByteWurst const& byteWurst) {
    Init ( byteWurst.mLength );
    mLength = byteWurst.mLength;
    memcpy ( mBytes, byteWurst.mBytes, mLength );
}

ByteWurst::~ByteWurst ()
{
    delete[] mBytes;
    mBytes = NULL;
}

void ByteWurst::Put( const void * const pBytes, size_t length )
{
    size_t safe_length = length;

    if ( safe_length > mLength )
    {
        safe_length = mLength;
    }

    memcpy( mBytes, pBytes, safe_length );
}

void * const ByteWurst::Get( void )
{
    return mBytes;
}

size_t ByteWurst::Length( void )
{
    return mLength;
}

bool operator== ( ByteWurst const& wurst1, ByteWurst const& wurst2 )
{
    if ( &wurst1 == &wurst2 ) return true;

    if ( wurst1.mLength != wurst2.mLength) return false;

    size_t length = wurst1.mLength;
   	for ( size_t i = 0; i < length; i++ )
   	{
		if ( wurst1.mBytes[i] != wurst2.mBytes[i] ) return false;
   	}

    return true;
}

bool operator!= ( ByteWurst const& wurst1, ByteWurst const& wurst2 )
{
    return ! ( &wurst1 == &wurst2 );
}

SimpleString StringFrom ( ByteWurst const& byteWurst )
{
#define MAX_INPUT_SIZE 1536
#define MAX_HEX_SIZE MAX_INPUT_SIZE * 2 + 2 + 3/* "0x" & "..." */
#define BUFFER_SIZE MAX_HEX_SIZE + 1
    char string [ BUFFER_SIZE ];
    size_t written = 0;
    size_t offset = 0;
    size_t i;

    ByteWurst poloni = byteWurst;
    size_t length = poloni.Length();
    if ( length > MAX_INPUT_SIZE ) length = MAX_INPUT_SIZE;
    unsigned char * bytes = (unsigned char *) poloni.Get();
    written = snprintf( string + offset, BUFFER_SIZE - offset, "0x" );
    offset = written;
    for ( i = 0; i < length; i++ )
    {
        written = snprintf( string + offset,
            BUFFER_SIZE - offset, "%02X", bytes[i] );
        offset += written;
    }
    if (i == MAX_INPUT_SIZE) {
        written = snprintf( string + offset,
            BUFFER_SIZE - offset, "..." );
    }
    return SimpleString ( string );
}

/**
 * private
 */

void ByteWurst::Init ( size_t length )
{
    mBytes = new unsigned char [length];
    mLength = length;
}
