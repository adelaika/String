#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include <limits.h>
#include <math.h>
#include <stdarg.h>

#include "s21_string.h"

#define OPTION_minus (1 << 0)
#define OPTION_plus (1 << 1)
#define OPTION_hash (1 << 2)
#define OPTION_zero (1 << 3)
#define OPTION_space (1 << 4)
#define OPTION_l (1 << 5)
#define OPTION_h (1 << 6)
#define OPTION_L (1 << 7)
#define VALUE_NOT_NULL (1 << 8)
#define UNSIGNED_LONG (1 << 9)

typedef struct {
  int is_negative;
  int num_zeros;
  int num_spaces;
  char value_string[320];
  int num_digits;
} CommonFormatState;

typedef struct {
  CommonFormatState common;
  long double double_value;
} FloatFormatState;

typedef struct {
  CommonFormatState common;
  long long int int_value;
} IntegerFormatState;

int s21_sprintf(char *buffer, const char *format, ...);

void reverse_string(char *buffer, int length);

int handle_llong_min(char *buffer);

int handle_unsigned(long long int number, char *buffer);

int handle_signed(long long int number, char *buffer);

int int_to_string(long long int number, char *buffer, int flag);

void incrementNumberBeforeDot(char *ptr, int index);

int frac_to_string(char spec, int precision, char *ptr, int index,
                   double number, int flag);

double round_fraction(double frac_part, int precision);

int calculate_precision_for_g(double value, char *value_string, int precision);

int float_to_string(char spec, long double value, char *value_string,
                    int precision, int flag);

int decimal_to_octal(unsigned int decimal, char *buffer, int num_aft_dot,
                     int flag);

int decimal_to_hexadecimal(char specifier, unsigned long long dec, char *buffer,
                           int num_aft_dot);

void trim_trailing_zeros(char *ptr, int *index);

double round_number(double number, int precision);

int process_mantissa(double *number, char *buffer, int *index, int precision,
                     char spec, int flag);

int process_exponent(int exponent, char *buffer, int index, char spec);

int double_to_exponential(char spec, double number, char *buffer, int precision,
                          int flag);

int pointer_to_string(char spec, void *ptr, char *buffer, size_t buffer_size,
                      int num_aft_dot);

void handle_false_spec(char *buffer, int *buf_ptr, const char *string,
                       int *pointer, int num_bf_dot, int num_aft_dot,
                       int start);

int parse_flags(const char *string, int *pointer);

int parse_width(const char *string, int *pointer, va_list args);

int parse_precision(const char *string, int *pointer, va_list args);

int parse_length_modifier(const char *string, int *pointer);

int handle_format_specifier(char *buffer, int *buf_ptr, const char *string,
                            int *pointer, int num_bf_dot, int num_aft_dot,
                            int flag, va_list args, int start);

int parse(char *buffer, int *buf_ptr, char *string, int pointer, va_list args);

void add_to_buf(char *buffer, int *buf_ptr, char specifier,
                CommonFormatState *state, int flag);

int is_minus_null(double a);

void initialize_common_fields(CommonFormatState *state);

int process_specifier(char specifier, FloatFormatState *state, int num_aft_dot,
                      int flag);

FloatFormatState *init_float_format(char specifier, va_list args,
                                    int num_aft_dot, int flag);

CommonFormatState *init_string_format(char specifier, va_list args,
                                      int num_aft_dot);

void handle_signed_value(IntegerFormatState *state);

void handle_hex_conversion(IntegerFormatState *state, char specifier,
                           int num_aft_dot, int *flag);

void handle_octal_conversion(IntegerFormatState *state, int num_aft_dot,
                             int *flag);

void extract_long_value(IntegerFormatState *state, char specifier, va_list args,
                        int *flag);

void extract_int_value(IntegerFormatState *state, char specifier, va_list args,
                       int *flag);

void convert_value_to_string(IntegerFormatState *state, char specifier,
                             int num_aft_dot, int *flag);

void process_sign_and_convert(IntegerFormatState *state, char specifier,
                              int num_aft_dot, int *flag);

IntegerFormatState *init_int_format(char specifier, va_list args,
                                    int num_aft_dot, int *flag);

CommonFormatState *init_pointer_format(char spec, va_list args, int num_aft_dot,
                                       int *flag);

void adjust_flags_and_format(IntegerFormatState *state, char specifier,
                             int *flag, int num_aft_dot);

void calculate_padding(IntegerFormatState *state, int num_bf_dot,
                       int num_aft_dot);

void convert_to_uppercase_if_needed(IntegerFormatState *state, char specifier);

void handle_integer(char *buffer, int *buf_ptr, char specifier, int num_bf_dot,
                    int num_aft_dot, va_list args, int flag);

void handle_float(char *buffer, int *buf_ptr, char specifier, int num_bf_dot,
                  int num_aft_dot, va_list args, int flag);

void handle_string(char specifier, va_list args, int num_bf_dot,
                   int num_aft_dot, unsigned int flag, char *buffer,
                   int *buf_ptr);

int handle_specifier(char *buffer, int *buf_ptr, char specifier, int num_bf_dot,
                     int num_aft_dot, va_list args, int flag);

#endif  // S21_SPRINTF_H
