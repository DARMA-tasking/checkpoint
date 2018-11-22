#if KOKKOS_ENABLED_SERDES

#include "test_harness.h"
#include "test_commons.h"

#include "test_kokkos_1d_commons.h"

// Manual 1,2,3 dimension comparison

template <typename ParamT> struct KokkosViewTest1D : KokkosViewTest<ParamT> { };

TYPED_TEST_CASE_P(KokkosViewTest1D);

TYPED_TEST_P(KokkosViewTest1D, test_1d_any) {
  using namespace serialization::interface;

  using LayoutType = typename std::tuple_element<0,TypeParam>::type;
  using DataType   = typename std::tuple_element<1,TypeParam>::type;
  using ViewType   = Kokkos::View<DataType, LayoutType>;

  static constexpr size_t const N = 241;

  LayoutType layout = layout1d<LayoutType>(N);
  ViewType in_view("test", layout);

  init1d(in_view);

  auto ret = serialize<ViewType>(in_view);
  auto out_view = deserialize<ViewType>(ret->getBuffer(), ret->getSize());
  auto const& out_view_ref = *out_view;

#if SERDES_USE_ND_COMPARE
  compareND(in_view, out_view_ref);
#else
  compare1d(in_view, out_view_ref);
#endif
}


REGISTER_TYPED_TEST_CASE_P(KokkosViewTest1D, test_1d_any);

#if DO_UNIT_TESTS_FOR_VIEW

///////////////////////////////////////////////////////////////////////////////
// 1-D Kokkos::View Tests
///////////////////////////////////////////////////////////////////////////////

using Test1DTypes = std::tuple<
  int      *, int      [1], int      [4],
  double   *, double   [1], double   [4],
  float    *, float    [1], float    [4],
  int32_t  *, int32_t  [1], int32_t  [4],
  int64_t  *, int64_t  [1], int64_t  [4],
  unsigned *, unsigned [1], unsigned [4],
  long     *, long     [1], long     [4],
  long long*, long long[1], long long[4]
>;

using Test1DTypesLeft =
  typename TestFactory<Test1DTypes,Kokkos::LayoutLeft>::ResultType;
using Test1DTypesRight =
  typename TestFactory<Test1DTypes,Kokkos::LayoutRight>::ResultType;
using Test1DTypesStride =
  typename TestFactory<Test1DTypes,Kokkos::LayoutStride>::ResultType;

INSTANTIATE_TYPED_TEST_CASE_P(test_1d_L, KokkosViewTest1D, Test1DTypesLeft);
INSTANTIATE_TYPED_TEST_CASE_P(test_1d_R, KokkosViewTest1D, Test1DTypesRight);
INSTANTIATE_TYPED_TEST_CASE_P(test_1d_S, KokkosViewTest1D, Test1DTypesStride);

#endif

///////////////////////////////////////////////////////////////////////////////
// Kokkos::DynamicView Unit Tests: dynamic view is restricted to 1-D in kokkos
///////////////////////////////////////////////////////////////////////////////

template <typename ParamT>
struct KokkosDynamicViewTest : KokkosViewTest<ParamT> { };

TYPED_TEST_CASE_P(KokkosDynamicViewTest);

TYPED_TEST_P(KokkosDynamicViewTest, test_dynamic_1d) {
  using namespace serialization::interface;

  using DataType = TypeParam;
  using ViewType = Kokkos::Experimental::DynamicView<DataType>;

  static constexpr std::size_t const N = 64;
  static constexpr unsigned const min_chunk = 8;
  static constexpr unsigned const max_extent = 1024;

  ViewType in_view("my-dynamic-view", min_chunk, max_extent);
  in_view.resize_serial(N);

  // std::cout << "INIT size=" << in_view.size() << std::endl;

  init1d(in_view);

  auto ret = serialize<ViewType>(in_view);
  auto out_view = deserialize<ViewType>(ret->getBuffer(), ret->getSize());
  auto const& out_view_ref = *out_view;

  /*
   *  Uncomment these lines (one or both) to test the failure mode: ensure the
   *  view equality test code is operating correctly.
   *
   *   out_view_ref(3) = 10;
   *   out_view->resize_serial(N-1);
   *
   */

#if SERDES_USE_ND_COMPARE
  compareND(in_view, out_view_ref);
#else
  compare1d(in_view, out_view_ref);
#endif
}

REGISTER_TYPED_TEST_CASE_P(KokkosDynamicViewTest, test_dynamic_1d);

using DynamicTestTypes = testing::Types<
  int      *,
  double   *,
  float    *,
  int32_t  *,
  int64_t  *,
  unsigned *,
  long     *,
  long long*
>;

INSTANTIATE_TYPED_TEST_CASE_P(
  test_dynamic_view_1, KokkosDynamicViewTest, DynamicTestTypes
);



#endif