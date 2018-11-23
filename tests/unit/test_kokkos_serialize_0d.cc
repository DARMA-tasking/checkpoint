#include "test_harness.h"
#include "test_commons.h"
#include "test_kokkos_0d_commons.h"

template <typename ParamT> struct KokkosViewTest0D : KokkosViewTest<ParamT> { };

TYPED_TEST_CASE_P(KokkosViewTest0D);

TYPED_TEST_P(KokkosViewTest0D, test_0d_any) {
  using namespace serialization::interface;

  using DataType = TypeParam;
  using ViewType = Kokkos::View<DataType>;

  static constexpr size_t const N = 241;

  ViewType in_view("test");

  init0d(in_view);

  auto ret = serialize<ViewType>(in_view);
  auto out_view = deserialize<ViewType>(ret->getBuffer(), ret->getSize());
  auto const& out_view_ref = *out_view;

#if SERDES_USE_ND_COMPARE
  compareND(in_view, out_view_ref);
#else
  compare0d(in_view, out_view_ref);
#endif
}

REGISTER_TYPED_TEST_CASE_P(KokkosViewTest0D, test_0d_any);

#if DO_UNIT_TESTS_FOR_VIEW

INSTANTIATE_TYPED_TEST_CASE_P(test_0d, KokkosViewTest0D, Test0DTypes);

#endif
