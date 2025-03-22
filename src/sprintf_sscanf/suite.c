#include "suite.h"

#include <check.h>

int main(void) {
  Suite *suite = suite_create("Bonus Tests");

  add_s21_sprintf_tests(suite);
  add_s21_sscanf_tests(suite);

  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);

  int number_failed = srunner_ntests_failed(runner);

  srunner_free(runner);

  return (number_failed == 0) ? 0 : 1;
}
