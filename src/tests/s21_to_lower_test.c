#include "../s21_string.h"
#include "string_suite.h"

START_TEST(test_s21_to_lower_normal) {
  char *str = "HELLO WORLD";
  char *expected = "hello world";
  char *result = s21_to_lower(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(test_s21_to_lower_empty_string) {
  char *str = "";
  char *expected = "";
  char *result = s21_to_lower(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(test_s21_to_lower_all_uppercase) {
  char *str = "hello world";
  char *expected = "hello world";
  char *result = s21_to_lower(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

void add_s21_to_lower_tests(Suite *suite) {
  TCase *tcase = tcase_create("s21_to_lower");

  // Добавление тестов для функции to_lower
  tcase_add_test(tcase, test_s21_to_lower_normal);
  tcase_add_test(tcase, test_s21_to_lower_empty_string);
  tcase_add_test(tcase, test_s21_to_lower_all_uppercase);

  // Добавление TCase в набор
  suite_add_tcase(suite, tcase);
}
