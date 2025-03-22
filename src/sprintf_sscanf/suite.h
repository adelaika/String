#ifndef SPRINTF_SUITE_H
#define SPRINTF_SUITE_H
#include <check.h>
#include <limits.h>
#include <math.h>
#include <string.h>

// Прототип функции, добавляющей тесты для s21_sprintf
void add_s21_sprintf_tests(Suite *suite);
void add_s21_sscanf_tests(Suite *suite);

#endif  // SPRINTF_SUITE_H
