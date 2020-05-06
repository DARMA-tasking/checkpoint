/*
//@HEADER
// *****************************************************************************
//
//                            checkpoint_api.impl.h
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

#if !defined INCLUDED_CHECKPOINT_CHECKPOINT_API_IMPL_H
#define INCLUDED_CHECKPOINT_CHECKPOINT_API_IMPL_H

#include "checkpoint/common.h"
#include <checkpoint/checkpoint.h>
#include "checkpoint/checkpoint_api.h"
#include "buffer/buffer.h"

#include <memory>

namespace checkpoint {

template <typename T>
SerializedReturnType serialize(T& target, BufferCallbackType fn) {
  auto ret = ::checkpoint::serializeType<T>(target, fn);
  auto& buf = std::get<0>(ret);
  std::unique_ptr<SerializedInfo> base_ptr(
    static_cast<SerializedInfo*>(buf.release())
  );
  return base_ptr;
}

template <typename T>
T* deserialize(SerialByteType* buf, SizeType size, T* user_buf) {
  return ::checkpoint::deserializeType<T>(buf, size, user_buf);
}

template <typename T>
T* deserialize(SerializedReturnType&& in) {
  return ::checkpoint::deserializeType<T>(in->getBuffer(), in->getSize());
}

template <typename T>
void deserializeInPlace(SerialByteType* buf, SizeType size, T* t) {
  return ::checkpoint::deserializeType<T>(::checkpoint::InPlaceTag{}, buf, size, t);
}

template <typename T>
SerializedReturnType serializePartial(T& target, BufferCallbackType fn) {
  auto ret = ::checkpoint::serializeTypePartial<T>(target, fn);
  auto& buf = std::get<0>(ret);
  std::unique_ptr<SerializedInfo> base_ptr(
    static_cast<SerializedInfo*>(buf.release())
  );
  return base_ptr;
}

template <typename T>
T* deserializePartial(SerialByteType* buf, SizeType size, T* user_buf) {
  return ::checkpoint::deserializeTypePartial<T>(buf, size, user_buf);
}

template <typename T>
std::size_t getSize(T& target) {
  return ::checkpoint::sizeType<T>(target);
}

} /* end namespace checkpoint */

#endif /*INCLUDED_CHECKPOINT_CHECKPOINT_API_IMPL_H*/
