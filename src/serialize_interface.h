/*
//@HEADER
// ************************************************************************
//
//                          serialize_interface.h
//                           checkpoint
//              Copyright (C) 2017 NTESS, LLC
//
// Under the terms of Contract DE-NA-0003525 with NTESS, LLC,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact darma@sandia.gov
//
// ************************************************************************
//@HEADER
*/

#if !defined INCLUDED_SERIALIZE_INTERFACE
#define INCLUDED_SERIALIZE_INTERFACE

#include <cstdlib>
#include <functional>
#include <memory>

namespace serialization { namespace interface {

using SizeType = size_t;
using SerialByteType = char;

using BufferCallbackType = std::function<SerialByteType*(SizeType size)>;

struct SerializedInfo {
  virtual SizeType getSize() const = 0;
  virtual SerialByteType* getBuffer() const = 0;
  virtual ~SerializedInfo() { }
};

using SerializedInfoPtrType = std::unique_ptr<SerializedInfo>;
using SerializedReturnType = SerializedInfoPtrType;

template <typename T>
SerializedReturnType serialize(T& target, BufferCallbackType fn = nullptr);

template <typename T>
T* deserialize(SerialByteType* buf, SizeType size, T* user_buf = nullptr);

template <typename T>
T* deserialize(SerializedReturnType&& in);

template <typename T>
SerializedReturnType serializePartial(T& target, BufferCallbackType fn = nullptr);

template <typename T>
T* deserializePartial(SerialByteType* buf, SizeType size, T* user_buf = nullptr);

}} /* end namespace serialization::interface */

#endif /*INCLUDED_SERIALIZE_INTERFACE*/
