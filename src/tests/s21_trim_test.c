#include <string.h>

#include "../s21_string.h"
#include "string_suite.h"

START_TEST(test_s21_trim_normal) {
  char *src = "   Hello World   ";
  char *trim_chars = " ";
  char *expected = "Hello World";
  char *result = s21_trim(src, trim_chars);
  ck_assert_str_eq(result, expected);
  if (result) free(result);
}
END_TEST

START_TEST(test_s21_trim_leading_and_trailing) {
  char *src = "###Hello World###";
  char *trim_chars = "#";
  char *expected = "Hello World";
  char *result = s21_trim(src, trim_chars);
  ck_assert_str_eq(result, expected);
  if (result) free(result);
}
END_TEST

START_TEST(test_s21_trim_no_trim_chars) {
  char *src = "Hello World";
  char *trim_chars = " ";
  char *expected = "Hello World";  // Ожидаемое поведение: строка не изменяется
  char *result = s21_trim(src, trim_chars);
  ck_assert_str_eq(result, expected);
  if (result) free(result);
}
END_TEST

START_TEST(trim_test2) {
  char str[] = "Ab000cd0";
  char trim_ch[] = "003";
  char expected[] = "Ab000cd";
  char *got = s21_trim(str, trim_ch);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(trim_test3) {
  char str[] = "DoNotTouch";
  char trim_ch[] = "Not";
  char expected[] = "DoNotTouch";
  char *got = s21_trim(str, trim_ch);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(trim_test4) {
  char str[] = "&* !!sc21 * **";
  char trim_ch[] = "&!* ";
  char expected[] = "sc21";
  char *got = s21_trim(str, trim_ch);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(trim_test5) {
  char str[] = " Good morning!    ";
  char trim_ch[] = " ";
  char expected[] = "Good morning!";
  char *got = s21_trim(str, trim_ch);
  ck_assert_str_eq(got, expected);
  if (got) free(got);
}
END_TEST

START_TEST(trim_1) {
    char *str1 = "Hello World";
    char *str2 = "H";
    char *str3 = (char *)s21_trim(str1, str2);
    ck_assert_str_eq(str3, "ello World");
    if (str3) free(str3);
} END_TEST

START_TEST(trim_2) {
    char *str1 = "Hello World";
    char *str2 = "Hedl";
    char *str3 = (char *)s21_trim(str1, str2);
    ck_assert_str_eq(str3, "o Wor");
    if (str3) free(str3);
} END_TEST

START_TEST(trim_3) {
    char *str1 = "";
    char *str2 = "";
    char *str3 = (char *)s21_trim(str1, str2);
    ck_assert_str_eq(str3, "");
    if (str3) free(str3);
} END_TEST

START_TEST(trim_4) {
    char *str = "empty";
    char *str2 = "empty";
    char *str3 = (char *)s21_trim(str, str2);
    ck_assert_str_eq(str3, "");
    if (str3) free(str3);
} END_TEST

START_TEST(trim_5) {
    char *str = S21_NULL;
    char *str2 = S21_NULL;
    char *str3 = (char *)s21_trim(str, str2);
    ck_assert_ptr_eq(str3, S21_NULL);
    if (str3) free(str3);
} END_TEST

START_TEST(trim_6) {
    char *str = "";
    char *str2 = S21_NULL;
    char *str3 = (char *)s21_trim(str, str2);
    ck_assert_ptr_eq(str3, S21_NULL);
    if (str3) free(str3);
} END_TEST

START_TEST(trim_7) {
    char *str = S21_NULL;
    char *str2 = "          ";
    char *str3 = (char *)s21_trim(str, str2);
    ck_assert_ptr_eq(str3, S21_NULL);
    if (str3) free(str3);
} END_TEST

START_TEST(trim_8) {
    char *str = "empty";
    char *str2 = "t";
    char *str3 = (char *)s21_trim(str, str2);
    ck_assert_str_ne(str3, "em");
    if (str3) free(str3);
} END_TEST


void add_s21_trim_tests(Suite *suite) {
  TCase *tcase = tcase_create("s21_trim");

  // Добавление тестов для функции trim
  tcase_add_test(tcase, test_s21_trim_normal);
  tcase_add_test(tcase, test_s21_trim_leading_and_trailing);
  tcase_add_test(tcase, test_s21_trim_no_trim_chars);
  tcase_add_test(tcase, trim_test2);
  tcase_add_test(tcase, trim_test3);
  tcase_add_test(tcase, trim_test4);
  tcase_add_test(tcase, trim_test5);
      tcase_add_test(tcase, trim_1);
    tcase_add_test(tcase, trim_2);
    tcase_add_test(tcase, trim_3);
    tcase_add_test(tcase, trim_4);
    tcase_add_test(tcase, trim_5);
    tcase_add_test(tcase, trim_6);
    tcase_add_test(tcase, trim_7);
    tcase_add_test(tcase, trim_8);

  // Добавление TCase в набор
  suite_add_tcase(suite, tcase);
}
