#include "math.h"

#include "stdarg.h"
#include "stddef.h"
#include "stdint.h"
#include "setjmp.h"
#include "cmocka.h"

static void test_add(void **state) {
  assert_int_equal(add(1, 3), 4);
  (void) state;  // unused
}

static void empty_test(void **state) {
  (void) state;  // unused
}


int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_add),
      cmocka_unit_test(empty_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
