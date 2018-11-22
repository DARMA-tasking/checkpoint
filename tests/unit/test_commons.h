#ifndef TEST_COMMONS_H
#define TEST_COMMONS_H

#include "test_harness.h"

#include <serdes_headers.h>
#include <serialization_library_headers.h>
#include <container/array_serialize.h>
#include <container/view_serialize.h>
#include <container/string_serialize.h>
#include <container/vector_serialize.h>
#include <container/tuple_serialize.h>
#include <container/view_equality.h>

#include <Kokkos_Core.hpp>
#include <Kokkos_View.hpp>
#include <Kokkos_DynamicView.hpp>
#include <Kokkos_Serial.hpp>

/*
 * Compiling all the unit tests for Kokkos::View takes a long time, thus a
 * compile-time option to disable the unit tests if needed
 */
#define DO_UNIT_TESTS_FOR_VIEW 1

// By default, using manual compare...should I switch this?
#define SERDES_USE_ND_COMPARE 0

/*
 * This manual compare code should be removed once serdes::ViewEquality is fully
 * tested on target platforms
 */

struct GTestEquality {
  template <typename T>
  bool operator()(T&& a, T&& b) const {
    EXPECT_EQ(a,b);
    return a == b;
  }
};

template <typename ViewT>
static void compareBasic(ViewT const& k1, ViewT const& k2) {
  using EqualityType = serdes::ViewEquality<ViewT>;
  EqualityType::template compareStaticDim<GTestEquality>(k1);
  EqualityType::template compareStaticDim<GTestEquality>(k2);
  EqualityType::template compareMeta<GTestEquality>(k1,k2);
}

template <typename ParamT>
struct KokkosViewTest : ::testing::TestWithParam<ParamT> {
  virtual void SetUp() override {
    Kokkos::initialize();
  }
  virtual void TearDown() override {
    Kokkos::finalize();
  }
};

using lsType = std::size_t;

// SpliceTypeMeta: zip a set of types with ValueT

template <int N, typename ValueT, typename TypeList>
struct SpliceTypeMeta {
  using ResultType = decltype(
    std::tuple_cat(
      std::declval<
        std::tuple<
          std::tuple<ValueT,typename std::tuple_element<N-1,TypeList>::type>
        >
      >(),
      std::declval<typename SpliceTypeMeta<N-1,ValueT,TypeList>::ResultType>()
    )
  );
};

template <typename ValueT, typename TypeList>
struct SpliceTypeMeta<0,ValueT,TypeList> {
  using ResultType = decltype(std::declval<std::tuple<>>());
};

template <typename TypeList, typename ValueT>
struct SpliceMeta {
  using ResultTupleType = typename SpliceTypeMeta<
    std::tuple_size<TypeList>::value,ValueT,TypeList
  >::ResultType;
};

//
// Debug code for SpliceTypeMeta
//
// struct A { A() = delete; };
// using TestA = std::tuple<int, float>;
// using TestB = std::tuple<std::tuple<A,float>, std::tuple<A,int>>;
// using TestC = typename SpliceMeta<TestA,A>::ResultTupleType;
// int static_test_of() { TestC{}; }
//
// static_assert(
//   std::is_same<TestB,TestC>::value, "Not same"
// );
//

// Type conversion from std::tuple<X,Y,Z> to testing::Types<X,Y,Z>
template <typename Tuple1, template <typename...> class Tuple2>
struct ConvertTupleType { };

template <template <typename...> class Tuple2, typename... Args>
struct ConvertTupleType<std::tuple<Args...>,Tuple2> {
  using ResultType = Tuple2<Args...>;
};

// Factory for constructed generated type param list for testing::Types
template <typename TypeList, typename ValueT>
struct TestFactory {
  static constexpr auto const tup_size = std::tuple_size<TypeList>::value;
  static constexpr auto const I = std::make_index_sequence<tup_size>{};
  using ResultTupleType = typename SpliceMeta<TypeList,ValueT>::ResultTupleType;
  using ResultType =
    typename ConvertTupleType<ResultTupleType,testing::Types>::ResultType;
};

#endif // TEST_COMMONS_H