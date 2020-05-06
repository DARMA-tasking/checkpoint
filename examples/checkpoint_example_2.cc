/*
//@HEADER
// *****************************************************************************
//
//                           checkpoint_example_2.cc
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

#include <checkpoint/checkpoint.h>

#include <cstdio>

namespace checkpoint { namespace examples {

struct MyTest {
  int a = 29, b = 31;

  MyTest(int const) { }
  MyTest() = delete;

  static MyTest& reconstruct(void* buf) {
    printf("MyTest reconstruct\n");
    MyTest* a = new (buf) MyTest(100);
    return *a;
  }

  void print() {
    printf("MyTest: a=%d, b=%d\n", a, b);
  }

  template <typename Serializer>
  void serialize(Serializer& s) {
    printf("MyTest serialize\n");
    s | a;
    s | b;
  }
};

}} // end namespace checkpoint::examples

int main(int, char**) {
  using namespace checkpoint::examples;

  MyTest my_test_inst(10);

  my_test_inst.print();

  auto ret = checkpoint::serialize(my_test_inst);

  auto const& buf = ret->getBuffer();
  auto const& buf_size = ret->getSize();

  printf("ptr=%p, size=%ld\n", static_cast<void*>(buf), buf_size);

  auto t = checkpoint::deserialize<MyTest>(buf);
  t->print();

  return 0;
}