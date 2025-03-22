#include <string.h>

#include "../s21_string.h"
#include "string_suite.h"

START_TEST(test_s21_insert_at_start) {
  char *src = "Hello ";
  char *str = "World";
  s21_size_t start_index = 6;
  char *expected = S21_NULL;
  char *result = s21_insert(src, str, start_index);
  ck_assert_ptr_eq(result, expected);
  if (result != S21_NULL) free(result);
}
END_TEST

START_TEST(insert_test1) {
  char str[] = "Shlepa";
  char src[] = "I love my . He is very kind!";
  s21_size_t index = 10;
  char expected[] = "I love my Shlepa. He is very kind!";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got != S21_NULL) free(got);
}
END_TEST

START_TEST(insert_test2) {
  char str[] = "Hello, ";
  char src[] = "Aboba!";
  s21_size_t index = 0;
  char expected[] = "Hello, Aboba!";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got != S21_NULL) free(got);
}
END_TEST

START_TEST(insert_test3) {
  char str[] = "Monkey";
  char src[] = "Space  ";
  s21_size_t index = 6;
  char expected[] = "Space Monkey ";
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_str_eq(got, expected);
  if (got != S21_NULL) free(got);
}


START_TEST(insert_test4) {
  char str[] = "Monkey";
  char *src = S21_NULL;
  s21_size_t index = 6;
  char *expected = S21_NULL;
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_ptr_eq(got, expected);
  if (got != S21_NULL) free(got);
}


START_TEST(insert_test5) {
  char *str = S21_NULL;
  char src[] = "Space  ";
  s21_size_t index = 6;
  char *expected = S21_NULL;
  char *got = (char *)s21_insert(src, str, index);
  ck_assert_ptr_eq(got, expected);
  if (got != S21_NULL) free(got);
}

START_TEST(test_s21_insert_empty_str) {
  char *src = "World";
  char *str = "";
  s21_size_t start_index = 0;
  char *expected =
      "World";  
  char *result = s21_insert(src, str, start_index);
  ck_assert_str_eq(result, expected);
  if (result != S21_NULL) free(result);
}
END_TEST

START_TEST(test_s21_insert_empty_src) {
  char *src = "";
  char *str = "World";
  s21_size_t start_index = 0;
  char *expected =
      "World";  
  char *result = s21_insert(src, str, start_index);
  ck_assert_str_eq(result, expected);
  if (result != S21_NULL) free(result);
}
END_TEST

void add_s21_insert_tests(Suite *suite) {
  TCase *tcase = tcase_create("s21_insert");

  // Добавление тестов для функции вставки
  tcase_add_test(tcase, test_s21_insert_at_start);
  tcase_add_test(tcase, test_s21_insert_empty_str);
  tcase_add_test(tcase, test_s21_insert_empty_src);
  tcase_add_test(tcase, insert_test2);
  tcase_add_test(tcase, insert_test3);
  tcase_add_test(tcase, insert_test4);
  tcase_add_test(tcase, insert_test5);
  tcase_add_test(tcase, insert_test1);

  // Добавление тестов в набор
  suite_add_tcase(suite, tcase);
}
