#include <string.h>

#include "../s21_string.h"
#include "string_suite.h"

START_TEST(test_s21_strtok_normal) {
  char str1[] = "- This, a sample string.";
  char str2[] = "- This, a sample string.";
  char *delim = " ,.-";

  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  while (s21_token != S21_NULL || std_token != S21_NULL) {
    ck_assert_str_eq(s21_token, std_token);
    s21_token = s21_strtok(S21_NULL, delim);
    std_token = strtok(S21_NULL, delim);
  }
}
END_TEST

START_TEST(test_s21_strtok_empty_string) {
  char str1[] = "";
  char str2[] = "";
  char *delim = " ,.-";
  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  ck_assert_ptr_eq(s21_token, std_token);
}
END_TEST

START_TEST(test_s21_strtok_empty_delim) {
  char str1[] = "This is a sample string.";
  char str2[] = "This is a sample string.";
  char *delim = "";
  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  while (s21_token != S21_NULL || std_token != S21_NULL) {
    ck_assert_str_eq(s21_token, std_token);
    s21_token = s21_strtok(S21_NULL, delim);
    std_token = strtok(S21_NULL, delim);
  }
}
END_TEST

START_TEST(test_s21_strtok_no_tokens) {
  char str[] = "   ";
  char *delim = " ";
  char *s21_token = s21_strtok(str, delim);
  char *std_token = strtok(str, delim);
  ck_assert_ptr_eq(s21_token, std_token);
}
END_TEST

START_TEST(test_s21_strtok_consecutive_delims) {
  char str1[] = "This  is  a  sample  string.";
  char str2[] = "This  is  a  sample  string.";
  char *delim = " ";

  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  while (s21_token != S21_NULL || std_token != S21_NULL) {
    ck_assert_str_eq(s21_token, std_token);
    s21_token = s21_strtok(S21_NULL, delim);
    std_token = strtok(S21_NULL, delim);
  }
}
END_TEST

START_TEST(test_s21_strtok_delim_at_start_and_end) {
  char str1[] = " This is a sample string. ";
  char str2[] = " This is a sample string. ";
  char *delim = " ";

  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  while (s21_token != S21_NULL || std_token != S21_NULL) {
    ck_assert_str_eq(s21_token, std_token);
    s21_token = s21_strtok(S21_NULL, delim);
    std_token = strtok(S21_NULL, delim);
  }
}
END_TEST

START_TEST(test_s21_strtok_multiple_delims) {
  char str1[] = "This,is,a,sample,string.";
  char str2[] = "This,is,a,sample,string.";
  char *delim = ",";

  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  while (s21_token != S21_NULL || std_token != S21_NULL) {
    ck_assert_str_eq(s21_token, std_token);
    s21_token = s21_strtok(S21_NULL, delim);
    std_token = strtok(S21_NULL, delim);
  }
}
END_TEST

void add_s21_strtok_tests(Suite *suite) {
  TCase *tcase = tcase_create("s21_strtok");

  // Добавление тестов для функции strtok
  tcase_add_test(tcase, test_s21_strtok_normal);
  tcase_add_test(tcase, test_s21_strtok_empty_string);
  tcase_add_test(tcase, test_s21_strtok_empty_delim);
  tcase_add_test(tcase, test_s21_strtok_no_tokens);
  tcase_add_test(tcase, test_s21_strtok_consecutive_delims);
  tcase_add_test(tcase, test_s21_strtok_delim_at_start_and_end);
  tcase_add_test(tcase, test_s21_strtok_multiple_delims);

  // Добавление TCase в набор
  suite_add_tcase(suite, tcase);
}
