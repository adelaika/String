#include <check.h>
#include <limits.h>

#include "../s21_string.h"
#include "suite.h"

START_TEST(sscanf_test_char) {
  char c1, c2;
  int result1 = sscanf("a", "%c", &c1);
  int result2 = s21_sscanf("a", "%c", &c2);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(c1, c2);
}
END_TEST

START_TEST(sscanf_test_int) {
  int i1, i2;
  int result1 = sscanf("123", "%d", &i1);
  int result2 = s21_sscanf("123", "%d", &i2);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(i1, i2);
}
END_TEST

START_TEST(sscanf_test_string) {
  char s1[10] = "", s2[10] = "";
  int result1 = sscanf("hello", "%5s", s1);
  int result2 = s21_sscanf("hello", "%5s", s2);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(s1, s2);
}
END_TEST

START_TEST(test_sscanf_int) {
  int x1, y1;
  int x2, y2;
  int result = s21_sscanf("123 456", "%d %d", &x1, &y1);
  int result2 = sscanf("123 456", "%d %d", &x2, &y2);
  ck_assert_int_eq(result, result2);
  ck_assert_int_eq(x1, x2);
  ck_assert_int_eq(y1, y2);
}
END_TEST

START_TEST(test_sscanf_zero_prefix) {
  int x1, y1;
  int x2, y2;
  int result = s21_sscanf("0123 0456", "%i %i", &x1, &y1);
  int result2 = sscanf("0123 0456", "%i %i", &x2, &y2);
  ck_assert_int_eq(result, result2);
  ck_assert_int_eq(x1, x2);
  ck_assert_int_eq(y1, y2);
}
END_TEST

START_TEST(test_sscanf_short_int) {
  short int x1, y1;
  short int x2, y2;
  int result = s21_sscanf("123 456", "%hi %hi", &x1, &y1);
  int result2 = sscanf("123 456", "%hi %hi", &x2, &y2);
  ck_assert_int_eq(result, result2);
  ck_assert_int_eq(x1, x2);
  ck_assert_int_eq(y1, y2);
}
END_TEST

START_TEST(test_sscanf_hexadecimal) {
  int x1, y1;
  int x2, y2;
  int result = s21_sscanf("0x12A 0x456", "%i %i", &x1, &y1);
  int result2 = sscanf("0x12A 0x456", "%i %i", &x2, &y2);
  ck_assert_int_eq(result, result2);
  ck_assert_int_eq(x1, x2);
  ck_assert_int_eq(y1, y2);
}
END_TEST

START_TEST(test_sscanf_spec_i_int1) {
  long int a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0, d1 = 0, d2 = 0;
  const char str[] = "100 500 -600 +700";
  const char fstr[] = "%li %li %li %li";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1, &d1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2, &d2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(d1, d2);
}
END_TEST

START_TEST(test_sscanf_spec_i_int2) {
  int a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0, d1 = 0, d2 = 0;
  const char str[] = "100500-600+700 123";
  const char fstr[] = "%i %d %d %i";

  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2, &d2);
  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1, &d1);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(d1, d2);
}
END_TEST

START_TEST(lower_hex_0X) {
  uint32_t a1, a2;
  const char str[] = "0X";
  const char fstr[] = "%x";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(lower_hex_fail) {
  uint32_t a1, a2;
  const char str[] = "qF";
  const char fstr[] = "%x";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(lower_hex_len) {
  uint32_t a1, a2;
  const char str[] = "123531FFF";
  const char fstr[] = "%2x";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(upeer_hex_spaces_tabs_sns) {
  uint32_t a1, a2;
  const char str[] = "          \n             \n     F";
  const char fstr[] = "%X";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(octal_0X) {
  uint32_t a1, a2;
  const char str[] = "0X";
  const char fstr[] = "%o";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(buff3) {
  int32_t a1, a2;
  int32_t b1 = 0, b2 = 0;
  const char str[] = "12keppa12";
  const char fstr[] = "%dkeppapos%d";
  int32_t res1 = s21_sscanf(str, fstr, &a1, &b1);
  int32_t res2 = sscanf(str, fstr, &a2, &b2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(octal_len) {
  uint32_t a1, a2;
  const char str[] = "123531";
  const char fstr[] = "%2o";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(only_ints1) {
  long a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0, d1 = 0, d2 = 0;
  const char fstr[] = "%ld %ld %ld %ld";
  const char str[] = "555 666 777 888";

  int32_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1, &d1);
  int32_t res2 = sscanf(str, fstr, &a2, &b2, &c2, &d2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(d1, d2);
}
END_TEST

START_TEST(stringToFloat) {
  const char *input = "  -123.456  ";
  long double expected_sscanf, expected_s21_sscanf;
  long double res1 = sscanf(input, "%Lf", &expected_sscanf);
  long double res2 = s21_sscanf(input, "%Lf", &expected_s21_sscanf);
  ck_assert_double_eq(res1, res2);
}
END_TEST

START_TEST(stringToFloat2) {
  const char *input = "  126.458  ";
  long double expected_sscanf, expected_s21_sscanf;
  long double res1 = sscanf(input, "%Lf", &expected_sscanf);
  long double res2 = s21_sscanf(input, "%Lf", &expected_s21_sscanf);
  ck_assert_double_eq(res1, res2);
}
END_TEST

START_TEST(pointer) {
  void *a1 = S21_NULL, *a2 = S21_NULL, *b1 = S21_NULL, *b2 = S21_NULL;
  void *c1 = S21_NULL, *c2 = S21_NULL, *d1 = S21_NULL, *d2 = S21_NULL;
  const char str[] = "0xFFFF 0xAAAA 0x7123 0xBBC1FF";
  const char fstr[] = "%p %p %p %p";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1, &d1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2, &d2);

  ck_assert_int_eq(res1, res2);
  ck_assert_ptr_eq(a1, a2);
  ck_assert_ptr_eq(b1, b2);
  ck_assert_ptr_eq(c1, c2);
  ck_assert_ptr_eq(d1, d2);
}
END_TEST

START_TEST(upeer_hex_long) {
  unsigned long int a1, a2;
  const char str[] = "F";
  const char fstr[] = "%lX";
  uint16_t res1 = s21_sscanf(str, fstr, &a1);
  uint16_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(upeer_hex_base_version) {
  uint32_t a1, a2;
  const char str[] = "F";
  const char fstr[] = "%X";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(lower_hex_lower) {
  uint32_t a1, a2;
  const char str[] = "abcdef";
  const char fstr[] = "%x";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(lower_hex_base_version) {
  uint32_t a1, a2;
  const char str[] = "f";
  const char fstr[] = "%x";
  uint32_t res1 = s21_sscanf(str, fstr, &a1);
  uint32_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

void add_s21_sscanf_tests(Suite *suite) {
  TCase *tcase = tcase_create("s21_sscanf");

  // Добавление тестов для функции sscanf
  tcase_add_test(tcase, sscanf_test_char);
  tcase_add_test(tcase, sscanf_test_int);
  tcase_add_test(tcase, sscanf_test_string);
  tcase_add_test(tcase, test_sscanf_int);
  tcase_add_test(tcase, test_sscanf_zero_prefix);
  tcase_add_test(tcase, test_sscanf_short_int);
  tcase_add_test(tcase, test_sscanf_hexadecimal);
  tcase_add_test(tcase, test_sscanf_spec_i_int1);
  tcase_add_test(tcase, test_sscanf_spec_i_int2);
  tcase_add_test(tcase, lower_hex_0X);
  tcase_add_test(tcase, lower_hex_fail);
  tcase_add_test(tcase, lower_hex_len);
  tcase_add_test(tcase, upeer_hex_spaces_tabs_sns);
  tcase_add_test(tcase, upeer_hex_long);
  tcase_add_test(tcase, upeer_hex_base_version);
  tcase_add_test(tcase, octal_0X);
  tcase_add_test(tcase, buff3);
  tcase_add_test(tcase, octal_len);
  tcase_add_test(tcase, only_ints1);
  tcase_add_test(tcase, stringToFloat);
  tcase_add_test(tcase, stringToFloat2);
  tcase_add_test(tcase, pointer);
  tcase_add_test(tcase, lower_hex_lower);
  tcase_add_test(tcase, lower_hex_base_version);

  // Добавление TCase в набор
  suite_add_tcase(suite, tcase);
}
