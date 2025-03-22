#include "string_suite.h"

#include <check.h>

// Функция main для запуска всех тестов
int main(void) {
  Suite *suite;
  SRunner *runner;

  suite = suite_create("S21 String Tests");

  // Добавляем тесты для s21_memchr и s21_stlen
  add_s21_memchr_tests(suite);
  add_s21_strlen_tests(suite);
  add_s21_memcmp_tests(suite);
  add_s21_strerror_tests(suite);
  add_s21_insert_tests(suite);
  add_s21_memset_tests(suite);
  add_s21_memcpy_tests(suite);
  add_s21_strcspn_tests(suite);
  add_s21_strncat_tests(suite);
  add_s21_strncmp_tests(suite);
  add_s21_strncpy_tests(suite);
  add_s21_strpbrk_tests(suite);
  add_s21_strrchr_tests(suite);
  add_s21_strstr_tests(suite);
  add_s21_strtok_tests(suite);
  add_s21_to_lower_tests(suite);
  add_s21_to_upper_tests(suite);
  add_s21_trim_tests(suite);

  runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);

  int number_failed = srunner_ntests_failed(runner);

  srunner_free(runner);

  return (number_failed == 0) ? 0 : 1;
}
