/*
//@HEADER
// *****************************************************************************
//
//                             memory_serializer.h
//                           DARMA Toolkit v. 1.0.0
//                 DARMA/checkpoint => Serialization Library
//
// Copyright 2019 National Technology & Engineering Solutions of Sandia, LLC
// (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact darma@sandia.gov
//
// *****************************************************************************
//@HEADER
*/

#if !defined INCLUDED_CHECKPOINT_SERIALIZERS_MEMORY_SERIALIZER_H
#define INCLUDED_CHECKPOINT_SERIALIZERS_MEMORY_SERIALIZER_H

#include "checkpoint/common.h"
#include "checkpoint/serializers/base_serializer.h"

namespace checkpoint {

struct MemorySerializer : Serializer {
  MemorySerializer(ModeType const& in_mode, SerialByteType* in_start)
    : Serializer(in_mode), start_(in_start), cur_(in_start)
  { }

  explicit MemorySerializer(ModeType const& in_mode)
    : Serializer(in_mode)
  { }

  SerialByteType* getBuffer() const {
    return start_;
  }

  void initializeBuffer(SerialByteType* const ptr) {
    start_ = cur_ = ptr;
  }

  SerialByteType* getSpotIncrement(SerialSizeType const inc) {
    SerialByteType* spot = cur_;
    cur_ += inc;
    return spot;
  }

protected:
  // The start of the memory buffer to serialize into or out of
  SerialByteType* start_ = nullptr;

  // The current location of the memory buffer for the next call
  SerialByteType* cur_ = nullptr;
};

} /* end namespace checkpoint */

#endif /*INCLUDED_CHECKPOINT_SERIALIZERS_MEMORY_SERIALIZER_H*/