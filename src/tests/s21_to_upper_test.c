#include "../s21_string.h"
#include "string_suite.h"

START_TEST(test_s21_to_upper_normal) {
  char *str = "hello world";
  char *expected = "HELLO WORLD";
  char *result = s21_to_upper(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(test_s21_to_upper_empty_string) {
  char *str = "";
  char *expected = "";
  char *result = s21_to_upper(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(test_s21_to_upper_all_uppercase) {
  char *str = "HELLO WORLD";
  char *expected = "HELLO WORLD";
  char *result = s21_to_upper(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

void add_s21_to_upper_tests(Suite *suite) {
  TCase *tcase = tcase_create("s21_to_upper");

  // Добавление тестов для функции to_upper
  tcase_add_test(tcase, test_s21_to_upper_normal);
  tcase_add_test(tcase, test_s21_to_upper_empty_string);
  tcase_add_test(tcase, test_s21_to_upper_all_uppercase);

  // Добавление TCase в набор
  suite_add_tcase(suite, tcase);
}
