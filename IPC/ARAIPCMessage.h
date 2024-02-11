//------------------------------------------------------------------------------
//! \file       ARAIPCMessage.h
//!             Abstractions shared by both the ARA IPC proxy host and plug-in
//! \project    ARA SDK Library
//! \copyright  Copyright (c) 2021-2023, Celemony Software GmbH, All Rights Reserved.
//! \license    Licensed under the Apache License, Version 2.0 (the "License");
//!             you may not use this file except in compliance with the License.
//!             You may obtain a copy of the License at
//!
//!               http://www.apache.org/licenses/LICENSE-2.0
//!
//!             Unless required by applicable law or agreed to in writing, software
//!             distributed under the License is distributed on an "AS IS" BASIS,
//!             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//!             See the License for the specific language governing permissions and
//!             limitations under the License.
//------------------------------------------------------------------------------

#ifndef ARAIPCMessage_h
#define ARAIPCMessage_h

#include "ARA_Library/IPC/ARAIPC.h"

#if ARA_ENABLE_IPC


//! @addtogroup ARA_Library_IPC
//! @{

namespace ARA {
namespace IPC {


//! ID type for messages, IDs must be >= kMessageIDRangeStart and < kMessageIDRangeEnd
typedef ARAInt32 ARAIPCMessageID;
constexpr ARAIPCMessageID kARAIPCMessageIDRangeStart { 1 };
constexpr ARAIPCMessageID kARAIPCMessageIDRangeEnd { 8*16*16 - 1 };


//! key type for message dictionaries - negative keys are reserved for the implementation
typedef ARAInt32 ARAIPCMessageKey;


//! Message Encoder
//! @{
class ARAIPCMessageEncoder
{
public:
    virtual ~ARAIPCMessageEncoder() = default;

    //! number types
    //! The "size" variant will also be used for the pointer-sized ARA (host) refs.
    //@{
    virtual void appendInt32 (ARAIPCMessageKey argKey, int32_t argValue) = 0;
    virtual void appendInt64 (ARAIPCMessageKey argKey, int64_t argValue) = 0;
    virtual void appendSize (ARAIPCMessageKey argKey, size_t argValue) = 0;
    virtual void appendFloat (ARAIPCMessageKey argKey, float argValue) = 0;
    virtual void appendDouble (ARAIPCMessageKey argKey, double argValue) = 0;
    //@}

    //! UTF8-encoded C strings
    virtual void appendString (ARAIPCMessageKey argKey, const char* argValue) = 0;

    //! raw bytes
    //! As optimization, disable copying if the memory containing the bytes stays
    //! alive&unchanged until the message has been sent.
    virtual void appendBytes (ARAIPCMessageKey argKey, const uint8_t* argValue, size_t argSize, bool copy) = 0;

    //! sub-messages to encode compound types
    //! The caller is responsible for deleting the returned sub-encoder after use.
    virtual ARAIPCMessageEncoder* appendSubMessage (ARAIPCMessageKey argKey) = 0;
};
//! @}


//! Message Decoder
//! @{
class ARAIPCMessageDecoder
{
public:
    virtual ~ARAIPCMessageDecoder () = default;

    //! number types
    //! The "size" variant will also be used for the pointer-sized ARA (host) refs.
    //! Will return false and set argValue to 0 if key not found.
    //@{
    virtual bool readInt32 (ARAIPCMessageKey argKey, int32_t* argValue) const = 0;
    virtual bool readInt64 (ARAIPCMessageKey argKey, int64_t* argValue) const = 0;
    virtual bool readSize (ARAIPCMessageKey argKey, size_t* argValue) const = 0;
    virtual bool readFloat (ARAIPCMessageKey argKey, float* argValue) const = 0;
    virtual bool readDouble (ARAIPCMessageKey argKey, double* argValue) const = 0;
    //@}

    //! UTF8-encoded C strings
    //! Will return false and set argValue to NULL if key not found.
    //! Note that received string data is only valid as long as the message that
    //! provided them is alive.
    virtual bool readString (ARAIPCMessageKey argKey, const char ** argValue) const = 0;

    //! raw bytes
    //! first query size, then provide a buffer large enough to copy the bytes to.
    //! readBytesSize () will return false and set argSize to 0 if key not found.
    //@{
    virtual bool readBytesSize (ARAIPCMessageKey argKey, size_t* argSize) const = 0;
    virtual void readBytes (ARAIPCMessageKey argKey, uint8_t* argValue) const = 0;
    //@}

    //! sub-messages to decode compound types
    //! returns nullptr if key not found or if the value for the key is not representing a sub-message
    //! The caller is responsible for deleting the encoder after use.
    virtual ARAIPCMessageDecoder* readSubMessage (ARAIPCMessageKey argKey) const = 0;

    //! test whether a given argument is present in the message
    virtual bool hasDataForKey (ARAIPCMessageKey argKey) const = 0;
};
//! @}


}   // namespace IPC
}   // namespace ARA

//! @} ARA_Library_IPC


#endif // ARA_ENABLE_IPC

#endif // ARAIPCMessage_h
