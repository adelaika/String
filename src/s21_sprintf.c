#include "s21_sprintf.h"

int s21_sprintf(char *buffer, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int j = 0;

  for (int i = 0; format[i] != '\0'; i++) {
    if (format[i] == '%') {
      if (format[i + 1] != '\0') {
        i = parse(buffer, &j, (char *)format, i + 1, args);
      }

    } else {
      buffer[j++] = format[i];  // Печатаем обычный символ
    }
  }
  buffer[j] = '\0';
  va_end(args);
  return j;
}

// Функция для инвертирования строки
void reverse_string(char *buffer, int length) {
  for (int i = 0; i < length / 2; i++) {
    char temp = buffer[i];
    buffer[i] = buffer[length - i - 1];
    buffer[length - i - 1] = temp;
  }
}

// Обработка LLONG_MIN
int handle_llong_min(char *buffer) {
  const char *llmin_str = "9223372036854775808";
  int index = 0;

  while (*llmin_str) {
    buffer[index++] = *llmin_str++;
  }

  buffer[index] = '\0';
  return index;
}

// Обработка беззнаковых чисел
int handle_unsigned(long long int number, char *buffer) {
  unsigned long int unsigned_number = (unsigned long int)number;
  int index = 0;

  while (unsigned_number > 0 || index == 0) {
    buffer[index++] = (unsigned_number % 10) + '0';
    unsigned_number /= 10;
  }

  buffer[index] = '\0';
  reverse_string(buffer, index);
  return index;
}

// Обработка знаковых чисел
int handle_signed(long long int number, char *buffer) {
  int index = 0;
  int is_negative = 0;

  if (number < 0) {
    is_negative = 1;
    number = -number;
  }

  if (number == 0) {
    buffer[index++] = '0';
  } else {
    while (number > 0) {
      buffer[index++] = (number % 10) + '0';
      number /= 10;
    }
  }

  if (is_negative) {
    buffer[index++] = '-';
  }

  buffer[index] = '\0';
  reverse_string(buffer, index);
  return index;
}

// Преобразование числа в строку
int int_to_string(long long int number, char *buffer, int flag) {
  int index;
  // int is_negative = 0;

  // Обработка LLONG_MIN
  if (number == LLONG_MIN) {
    index = handle_llong_min(buffer);
  } else

    // Проверка на беззнаковость
    if (flag & UNSIGNED_LONG) {
      index = handle_unsigned(number, buffer);
    } else {
      // Обработка знаковых чисел
      index = handle_signed(number, buffer);
    }
  return index;
}

void incrementNumberBeforeDot(char *ptr, int index) {
  for (int i = index - 1; i >= 0; i--) {
    if (ptr[i] != '.') {  // Проверяем, что это не точка
      if (ptr[i] == '9') {
        ptr[i] = '0';  // Если 9, то обнуляем
      } else {
        ptr[i]++;  // Увеличиваем цифру на 1
        i = -1;    // Принудительно выходим из цикла
      }
    }
  }
}

int frac_to_string(char spec, int precision, char *ptr, int index,
                   double number, int flag) {
  // Если точность больше нуля, то нужно записывать дробную часть
  if (precision > 0) {
    ptr[index++] = '.';  // Добавляем точку для дробной части

    // Записываем дробные разряды
    for (int i = 0; i < precision; i++) {
      number *= 10.0;
      int digit = (int)number;

      ptr[index++] = '0' + digit;  // Добавляем цифру в строку
      number -= digit;  // Убираем добавленную цифру
    }

    // Проверка для корректировки последнего разряда (округление)
    number *= 10.0;  // Умножаем на 10, чтобы проверить следующий разряд
    if ((int)number >= 5) {
      incrementNumberBeforeDot(ptr, index);
    }

    // Для спецификаторов 'g' и 'G' убираем лишние нули, если флаг не установлен
    if ((spec == 'g' || spec == 'G') && !(flag & OPTION_hash)) {
      trim_trailing_zeros(ptr, &index);  // Удаляем ненужные нули
    }

  } else if (flag & OPTION_hash) {
    // Если точность 0, но установлен флаг '#', то добавляем точку
    ptr[index++] = '.';
  }

  return index;
}

// Функция для округления дробной части до нужной точности
double round_fraction(double frac_part, int precision) {
  double multiplier = pow(10, precision);
  return round(frac_part * multiplier) / multiplier;
}

// Функция для вычисления точности для 'g'/'G' спецификаторов
int calculate_precision_for_g(double value, char *value_string, int precision) {
  int num_digits = s21_strlen(value_string);
  if (value != 0.0) {
    int num_not_nil = 0;
    for (int i = 0; value_string[i] != '\0' && value_string[i] == '0'; i++) {
      num_not_nil++;
    }
    if (num_digits - num_not_nil != 0) {
      precision -= (num_digits - num_not_nil);
    } else {
      double frac_part = value - (long long)value;
      frac_part *= 10.0;
      int digit = (int)frac_part;
      while (digit == 0) {
        frac_part *= 10.0;
        digit = (int)frac_part;
        precision++;
      }
    }
  }
  return precision;
}

// Основная функция для преобразования числа с плавающей точкой в строку
int float_to_string(char spec, long double value, char *value_string,
                    int precision, int flag) {
  int num_digits = 0;
  long long int_part = precision != 0 ? (long long)value : round(value);
  num_digits = int_to_string(int_part, value_string, flag);

  if ((spec == 'g' || spec == 'G') && num_digits > precision && precision > 0) {
    num_digits =
        double_to_exponential(spec, value, value_string, precision, flag);
  } else {
    if (precision < 0) {
      precision = 6;  // Стандартная точность
    }
    if ((spec == 'g' || spec == 'G')) {
      precision = calculate_precision_for_g(value, value_string, precision);
    }

    double frac_part = value - int_part;
    frac_part = round_fraction(frac_part, precision);

    if (frac_part >= 1.0) {  // Если дробная часть слишком мала
      int_part += 1;    // Увеличиваем целую часть
      frac_part = 0.0;  // Убираем дробную часть
      num_digits = int_to_string(int_part, value_string, flag);
    }

    num_digits = frac_to_string(spec, precision, value_string, num_digits,
                                frac_part, flag);
    value_string[num_digits] = '\0';  // Завершаем строку
  }

  return num_digits;
}

int decimal_to_octal(unsigned int decimal, char *buffer, int num_aft_dot,
                     int flag) {
  char octal[32];  // Массив для хранения результата (макс. 32 символа для
                   // больших чисел)
  int index = 0;  // Указатель на текущую позицию в массиве
  if (num_aft_dot != 0) {
    if (decimal == 0 && (!(flag & OPTION_hash))) {
      octal[index++] = '0';
    }

    while (decimal > 0) {
      int remainder = decimal % 8;  // Остаток от деления на 8
      octal[index++] = '0' + remainder;  // Преобразование числа в символ
      decimal /= 8;                      // Делим на 8
    }

    if (flag & OPTION_hash) {
      octal[index++] = '0';
    }

    // Результат в массиве хранится в обратном порядке, выводим его с конца
    for (int i = index - 1; i >= 0; i--) {
      buffer[index - i - 1] = octal[i];
    }
  }
  return index;
}

int decimal_to_hexadecimal(char specifier, unsigned long long dec, char *buffer,
                           int num_aft_dot) {
  char hex[32];  // Массив для хранения результата (макс. 32 символа для
                 // 128-битного числа)
  int index = 0;  // Указатель на текущую позицию в массиве

  int stop = 16;
  if (specifier == 'x' || specifier == 'X') {
    stop = 8;
  }

  if (dec == 0 && num_aft_dot != 0) {
    hex[index++] = '0';
  }
  int counter = 0;

  while (dec > 0 && counter < stop) {
    int remainder = dec % 16;  // Остаток от деления на 16
    if (remainder < 10) {
      hex[index++] = '0' + remainder;  // Преобразование числа в символ
    } else {
      hex[index++] =
          'a' + (remainder - 10);  // Преобразование 10-15 в символы 'A'-'F'
    }
    dec /= 16;  // Делим на 16
    counter++;
  }

  // Результат в массиве
  for (int i = index - 1; i >= 0; i--) {
    buffer[index - 1 - i] = hex[i];
  }
  buffer[index] = '\0';

  return index;
}

void trim_trailing_zeros(char *ptr, int *index) {
  int i = *index - 1;

  // Удаляем завершающие нули
  while (ptr[i] == '0') {
    i--;
  }

  // Если встретили десятичную точку, тоже удаляем её
  if (ptr[i] == '.') {
    i--;
  }

  // Завершаем строку
  ptr[i + 1] = '\0';

  // Обновляем индекс
  *index = i + 1;
}

// Округление числа до указанной точности
double round_number(double number, int precision) {
  double multiplier = pow(10, precision);
  return round(number * multiplier) / multiplier;
}

// Преобразование мантиссы в строку
int process_mantissa(double *number, char *buffer, int *index, int precision,
                     char spec, int flag) {
  int integer_part = (int)(*number);
  buffer[(*index)++] = '0' + integer_part;

  *number -= integer_part;
  return frac_to_string(spec, precision, buffer, *index, *number, flag);
}

// Обработка порядка (экспоненты)
int process_exponent(int exponent, char *buffer, int index, char spec) {
  buffer[index++] = (spec == 'e' || spec == 'g') ? 'e' : 'E';

  if (exponent >= 0) {
    buffer[index++] = '+';
  } else {
    buffer[index++] = '-';
    exponent = -exponent;  // Убираем знак для записи значения
  }

  char exponent_buffer[32];
  int exponent_length = int_to_string(exponent, exponent_buffer, 0);

  if (exponent_length < 2) {
    buffer[index++] = '0';
  }

  for (int i = 0; i < exponent_length; i++) {
    buffer[index++] = exponent_buffer[i];
  }

  return index;
}

// Основная функция преобразования числа в экспоненциальную форму
int double_to_exponential(char spec, double number, char *buffer, int precision,
                          int flag) {
  int index = 0;

  // Устанавливаем точность по умолчанию, если не указана
  if (precision < 0) {
    precision = 6;  // Стандартная точность
  }

  // Для форматов 'g' и 'G' уменьшаем точность на единицу
  if (spec == 'g' || spec == 'G') {
    precision--;
  }

  // Разделяем число на мантиссу и порядок
  int exponent = 0;
  if (number != 0.0) {
    // Приводим число к диапазону [1.0, 10.0)
    while (number >= 10.0) {
      number /= 10.0;
      exponent++;
    }
    while (number < 1.0) {
      number *= 10.0;
      exponent--;
    }
  }

  // Округляем мантиссу до указанной точности
  number = round_number(number, precision);

  if (number >= 10.0) {
    number /= 10.0;
    exponent++;
  }

  // Обрабатываем мантиссу
  index = process_mantissa(&number, buffer, &index, precision, spec, flag);

  // Добавляем экспоненту
  index = process_exponent(exponent, buffer, index, spec);

  // Завершаем строку
  buffer[index] = '\0';

  return index;
}

int pointer_to_string(char spec, void *ptr, char *buffer, size_t buffer_size,
                      int num_aft_dot) {
  int pointer = 0;
  if (buffer_size < 3) {  // Минимум 3 символа для "0x\0"
    if (buffer_size > 0) {
      buffer[0] = '\0';
    }
  }
  if (ptr != S21_NULL) {
    unsigned long long address =
        (unsigned long long)ptr;  // Преобразуем указатель в целое число

    char hex[32];
    int num_digits = decimal_to_hexadecimal(spec, address, hex, num_aft_dot);
    for (int i = 0, j = 0; i < num_digits; i++, j++) {
      buffer[j] = hex[i];
    }
    buffer[num_digits] = '\0';
    pointer = num_digits;
  } else {
    char nil_str[32] = "(nil)";
    int j = 0;
    for (int i = 0; nil_str[i] != '\0'; i++) {  // strcat
      buffer[j++] = nil_str[i];
    }
    pointer = j;
  }
  return pointer;
}

void handle_false_spec(char *buffer, int *buf_ptr, const char *string,
                       int *pointer, int num_bf_dot, int num_aft_dot,
                       int start) {
  int temp = *pointer;
  *pointer = start - 1;
  while (*pointer < temp) {
    if (string[*pointer] == '*' && string[*pointer - 1] == '.') {
      char buf[32];
      int j = 0;
      int_to_string(num_aft_dot, buf, 0);
      while (buf[j] != '\0') {
        buffer[(*buf_ptr)++] = buf[j++];
      }
      (*pointer)++;
    } else if (string[*pointer] == '*') {
      char buf[32];
      int j = 0;
      int_to_string(num_bf_dot, buf, 0);
      while (buf[j] != '\0') {
        buffer[(*buf_ptr)++] = buf[j++];
      }
      (*pointer)++;
    } else {
      buffer[(*buf_ptr)++] = string[(*pointer)++];
    }
  }
}

int parse_flags(const char *string, int *pointer) {
  int flag = 0;
  while (string[*pointer] == '-' || string[*pointer] == '#' ||
         string[*pointer] == '+' || string[*pointer] == ' ' ||
         string[*pointer] == '0') {
    if (string[*pointer] == '-') {
      flag |= OPTION_minus;
    } else if (string[*pointer] == '0') {
      flag |= OPTION_zero;
    } else if (string[*pointer] == '#') {
      flag |= OPTION_hash;
    } else if (string[*pointer] == '+') {
      flag |= OPTION_plus;
    } else if (string[*pointer] == ' ') {
      flag |= OPTION_space;
    }
    (*pointer)++;
  }

  if (flag & OPTION_minus && flag & OPTION_zero) {
    flag &= ~OPTION_zero;
  }

  return flag;
}

int parse_width(const char *string, int *pointer, va_list args) {
  int width = 0;
  if (string[*pointer] == '*') {
    width = va_arg(args, int);
    (*pointer)++;
  } else {
    while (string[*pointer] >= '0' && string[*pointer] <= '9') {
      width = width * 10 + (string[*pointer] - '0');
      (*pointer)++;
    }
  }
  return width;
}

int parse_precision(const char *string, int *pointer, va_list args) {
  int precision = -1;
  if (string[*pointer] == '.') {
    (*pointer)++;
    precision = 0;
    if (string[*pointer] == '*') {
      precision = va_arg(args, int);
      (*pointer)++;
    } else {
      while (string[*pointer] >= '0' && string[*pointer] <= '9') {
        precision = precision * 10 + (string[*pointer] - '0');
        (*pointer)++;
      }
    }
  }
  return precision;
}

int parse_length_modifier(const char *string, int *pointer) {
  int flag = 0;
  if (string[*pointer] == 'l') {
    flag |= OPTION_l;
    (*pointer)++;
  } else if (string[*pointer] == 'h') {
    flag |= OPTION_h;
    (*pointer)++;
  } else if (string[*pointer] == 'L') {
    flag |= OPTION_L;
    (*pointer)++;
  }
  return flag;
}

int handle_format_specifier(char *buffer, int *buf_ptr, const char *string,
                            int *pointer, int num_bf_dot, int num_aft_dot,
                            int flag, va_list args, int start) {
  char spec = string[*pointer];
  int handled = handle_specifier(buffer, buf_ptr, spec, num_bf_dot, num_aft_dot,
                                 args, flag);
  if (handled && string[*pointer + 1] != '\0') {
    handle_false_spec(buffer, buf_ptr, string, pointer, num_bf_dot, num_aft_dot,
                      start);
  }
  return handled;
}

int parse(char *buffer, int *buf_ptr, char *string, int pointer, va_list args) {
  int num_bf_dot = 0;
  int num_aft_dot = -1;
  int flag = 0;
  int start = pointer;

  // Parse flags
  flag = parse_flags(string, &pointer);

  // Parse width
  num_bf_dot = parse_width(string, &pointer, args);

  // Parse precision
  num_aft_dot = parse_precision(string, &pointer, args);

  // Parse length modifier
  flag |= parse_length_modifier(string, &pointer);

  // Handle format specifier
  handle_format_specifier(buffer, buf_ptr, string, &pointer, num_bf_dot,
                          num_aft_dot, flag, args, start);

  return pointer;
}

void add_to_buf(char *buffer, int *buf_ptr, char specifier,
                CommonFormatState *state, int flag) {
  if (!(flag & OPTION_minus)) {
    if (flag & OPTION_zero && (specifier != 's' && specifier != 'c')) {
      state->num_zeros += state->num_spaces;
    } else {
      for (int i = 0; i < state->num_spaces; i++) {
        buffer[(*buf_ptr)++] = ' ';
      }
    }
  }
  // Печать знака для отрицательных чисел
  if (state->is_negative) {
    buffer[(*buf_ptr)++] = '-';
  } else if (flag & OPTION_plus && specifier != 'u') {
    buffer[(*buf_ptr)++] = '+';
  } else if (flag & OPTION_space && specifier != 'c') {
    buffer[(*buf_ptr)++] = ' ';
  }

  // Обработка флага #
  if (((flag & OPTION_hash) && (flag & VALUE_NOT_NULL) &&
       (specifier == 'x' || specifier == 'X' || specifier == 'o')) ||
      ((specifier == 'p') && (flag & VALUE_NOT_NULL))) {
    buffer[(*buf_ptr)++] = '0';
    if (specifier == 'x' || specifier == 'X') {
      buffer[(*buf_ptr)++] = specifier;
    } else if (specifier == 'p') {
      buffer[(*buf_ptr)++] = 'x';
    }
  }

  // Печать ведущих нулей
  for (int i = 0; i < state->num_zeros; i++) {
    buffer[(*buf_ptr)++] = '0';
  }

  // Печать самого числа
  for (int i = 0; i <= (state->num_digits - 1); i++) {
    buffer[(*buf_ptr)++] = state->value_string[i];  // Печать цифры как символа
  }

  // Печать конечных пробелов
  if (flag & OPTION_minus) {
    for (int i = 0; i < state->num_spaces; i++) {
      buffer[(*buf_ptr)++] = ' ';
    }
  }
}

int is_minus_null(double a) {
  double b = 0.0;
  int result;

  // Получаем побитовое представление числа
  unsigned long *a_bits = (unsigned long *)&a;
  unsigned long *b_bits = (unsigned long *)&b;

  // Проверка знака: старший бит числа с плавающей точкой (bit 31) отвечает за
  // знак
  if ((*a_bits >> 63) != (*b_bits >> 63)) {
    result = 1;
  } else {
    result = 0;
  }

  return result;
}

void initialize_common_fields(CommonFormatState *state) {
  state->is_negative = 0;
  state->num_zeros = 0;
  state->num_spaces = 0;
  state->num_digits = 0;
  state->value_string[0] = '\0';
}

int process_specifier(char specifier, FloatFormatState *state, int num_aft_dot,
                      int flag) {
  if (specifier == 'f') {
    return float_to_string(specifier, state->double_value,
                           state->common.value_string, num_aft_dot, flag);
  } else if (specifier == 'e' || specifier == 'E') {
    return double_to_exponential(specifier, state->double_value,
                                 state->common.value_string, num_aft_dot, flag);
  } else if (specifier == 'g' || specifier == 'G') {
    if ((fabsl(state->double_value) < 1e-4 && state->double_value != 0.0) ||
        fabsl(state->double_value) >= 1e+9) {
      return double_to_exponential(specifier, state->double_value,
                                   state->common.value_string, num_aft_dot,
                                   flag);
    } else {
      return float_to_string(specifier, state->double_value,
                             state->common.value_string, num_aft_dot, flag);
    }
  }
  return 0;  // Если спецификатор не распознан, возвращаем 0
}

FloatFormatState *init_float_format(char specifier, va_list args,
                                    int num_aft_dot, int flag) {
  // Выделение памяти для структуры
  FloatFormatState *state =
      (FloatFormatState *)malloc(sizeof(FloatFormatState));
  if (state) {
    // Извлечение значения в зависимости от флага L
    double value =
        (flag & OPTION_L) ? va_arg(args, long double) : va_arg(args, double);

    // Инициализация общих полей
    initialize_common_fields(&(state->common));
    state->double_value = value;
    if (value < 0 || (value == 0.0 && is_minus_null(value))) {
      state->common.is_negative = 1;
      state->double_value = -value;
    }

    // Преобразование числа в строку в зависимости от спецификатора
    state->common.num_digits =
        process_specifier(specifier, state, num_aft_dot, flag);
  }

  return state;
}

// Функция инициализации строки с учетом только структуры
CommonFormatState *init_string_format(char specifier, va_list args,
                                      int num_aft_dot) {
  // Выделение памяти для структуры
  CommonFormatState *state =
      (CommonFormatState *)malloc(sizeof(CommonFormatState));
  if (state) {
    state->num_zeros = 0;
    state->num_spaces = 0;
    state->is_negative = 0;

    if (specifier == 'c') {  // Обработка символа
      char value = (char)va_arg(args, int);  // Символ передаётся как int
      state->value_string[0] = value;
      state->value_string[1] = '\0';  // Завершающий нуль-символ
      state->num_digits = 1;  // Для символа всегда 1 символ
    } else if (specifier == 's') {  // Обработка строки
      char *value =
          va_arg(args, char *);  // Строка передаётся как указатель на char
      if (value == S21_NULL && (num_aft_dot > 5 || num_aft_dot == -1)) {
        char nil_str[32] = "(null)";
        int j = 0;
        for (int i = 0; nil_str[i] != '\0'; i++) {  // strcat
          state->value_string[j++] = nil_str[i];
        }
        state->value_string[j] = '\0';  // Завершающий нуль-символ
        state->num_digits = j;
      } else if (value != S21_NULL) {
        int i = 0;
        while (value[i] != '\0' && i < (int)(sizeof(state->value_string) - 1)) {
          state->value_string[i] = value[i];  // Сохраняем символ в value_string
          i++;
        }
        state->value_string[i] = '\0';  // Завершающий нуль-символ
        state->num_digits = i;  // Количество символов в строке
      }
    }
  }

  return state;
}

// Обработка знака числа
void handle_signed_value(IntegerFormatState *state) {
  state->common.is_negative = (state->int_value < 0);
  if (state->common.is_negative) {
    state->int_value = -state->int_value;
  }
}

// Обработка шестнадцатеричного формата
void handle_hex_conversion(IntegerFormatState *state, char specifier,
                           int num_aft_dot, int *flag) {
  (*flag) &= ~(OPTION_plus | OPTION_space);
  state->common.num_digits = decimal_to_hexadecimal(
      specifier, state->int_value, state->common.value_string, num_aft_dot);
}

// Обработка восьмеричного формата
void handle_octal_conversion(IntegerFormatState *state, int num_aft_dot,
                             int *flag) {
  (*flag) &= ~(OPTION_plus | OPTION_space);
  state->common.num_digits = decimal_to_octal(
      state->int_value, state->common.value_string, num_aft_dot, *flag);
}

// Извлечение значения для длинных типов
void extract_long_value(IntegerFormatState *state, char specifier, va_list args,
                        int *flag) {
  if (specifier == 'u') {
    (*flag) |= UNSIGNED_LONG;
    state->int_value = va_arg(args, long unsigned int);
  } else {
    state->int_value = va_arg(args, long int);
  }
}

// Извлечение значения в зависимости от флагов
void extract_int_value(IntegerFormatState *state, char specifier, va_list args,
                       int *flag) {
  if ((*flag) & OPTION_h) {
    state->int_value = (short int)va_arg(args, int);
  } else if ((*flag) & OPTION_l) {
    extract_long_value(state, specifier, args, flag);
  } else if (specifier == 'u') {
    state->int_value = va_arg(args, unsigned int);
    (*flag) &= ~(OPTION_plus);
  } else {
    state->int_value = va_arg(args, int);
  }

  if (state->int_value != 0) {
    (*flag) |= VALUE_NOT_NULL;
  }
}

// Преобразование числа в строку
void convert_value_to_string(IntegerFormatState *state, char specifier,
                             int num_aft_dot, int *flag) {
  if (specifier == 'd' || specifier == 'i' || specifier == 'u') {
    state->common.num_digits =
        int_to_string(state->int_value, state->common.value_string, *flag);
  } else if (specifier == 'x' || specifier == 'X') {
    handle_hex_conversion(state, specifier, num_aft_dot, flag);
  } else if (specifier == 'o') {
    handle_octal_conversion(state, num_aft_dot, flag);
  }
}

// Проверка знака и преобразование числа
void process_sign_and_convert(IntegerFormatState *state, char specifier,
                              int num_aft_dot, int *flag) {
  if (specifier == 'd' || specifier == 'i') {
    handle_signed_value(state);
  }

  convert_value_to_string(state, specifier, num_aft_dot, flag);
}

// Функция инициализации IntegerFormatState
IntegerFormatState *init_int_format(char specifier, va_list args,
                                    int num_aft_dot, int *flag) {
  IntegerFormatState *state =
      (IntegerFormatState *)malloc(sizeof(IntegerFormatState));

  if (state) {
    initialize_common_fields(&(state->common));
    state->int_value = 0;
    extract_int_value(state, specifier, args, flag);
    process_sign_and_convert(state, specifier, num_aft_dot, flag);
  }
  return state;
}

// Функция инициализации для указателей
CommonFormatState *init_pointer_format(char spec, va_list args, int num_aft_dot,
                                       int *flag) {
  // Выделение памяти для структуры
  CommonFormatState *state =
      (CommonFormatState *)malloc(sizeof(CommonFormatState));
  if (state) {
    // Извлечение указателя из аргументов
    void *value = va_arg(args, void *);  // Указатель передается как void*

    if (value != S21_NULL) {
      (*flag) |= VALUE_NOT_NULL;
    }

    // Инициализация общих данных
    state->num_zeros = 0;  // Для указателя не используется
    state->num_spaces = 0;
    state->is_negative = 0;

    // Преобразуем указатель в строку
    state->num_digits =
        pointer_to_string(spec, value, state->value_string,
                          sizeof(state->value_string), num_aft_dot);
  }

  return state;
}

void adjust_flags_and_format(IntegerFormatState *state, char specifier,
                             int *flag, int num_aft_dot) {
  if (num_aft_dot >= 0) {
    *flag &= ~OPTION_zero;
  }

  if ((*flag & OPTION_space) && (*flag & OPTION_plus)) {
    *flag &= ~OPTION_space;
  }

  if (specifier == 'd' || specifier == 'i') {
    if (state->common.is_negative || (*flag & OPTION_plus) ||
        (*flag & OPTION_space)) {
      state->common.num_spaces--;
    }
  }

  if (*flag & OPTION_hash) {
    if (specifier == 'x' || specifier == 'X') {
      state->common.num_spaces -= 2;
    } else {
      *flag &= ~OPTION_hash;
    }
  }
}

void calculate_padding(IntegerFormatState *state, int num_bf_dot,
                       int num_aft_dot) {
  if (num_aft_dot > state->common.num_digits) {
    state->common.num_zeros = num_aft_dot - state->common.num_digits;
  }

  int total_width = state->common.num_digits + state->common.num_zeros +
                    state->common.is_negative;
  if (num_bf_dot > total_width) {
    state->common.num_spaces = num_bf_dot - total_width;
  }
}

void convert_to_uppercase_if_needed(IntegerFormatState *state, char specifier) {
  if (specifier == 'X') {
    for (int i = 0; i < state->common.num_digits; i++) {
      if (state->common.value_string[i] >= 'a' &&
          state->common.value_string[i] <= 'f') {
        state->common.value_string[i] -= 32;
      }
    }
  }
}

void handle_integer(char *buffer, int *buf_ptr, char specifier, int num_bf_dot,
                    int num_aft_dot, va_list args, int flag) {
  IntegerFormatState *state =
      init_int_format(specifier, args, num_aft_dot, &flag);
  if (state) {
    adjust_flags_and_format(state, specifier, &flag, num_aft_dot);
    calculate_padding(state, num_bf_dot, num_aft_dot);
    convert_to_uppercase_if_needed(state, specifier);

    add_to_buf(buffer, buf_ptr, specifier, &state->common, flag);
    free(state);
  }
  return;
}

void handle_float(char *buffer, int *buf_ptr, char specifier, int num_bf_dot,
                  int num_aft_dot, va_list args, int flag) {
  FloatFormatState *state =
      init_float_format(specifier, args, num_aft_dot, flag);

  if (state) {
    if (num_bf_dot > (state->common.num_digits + state->common.num_zeros)) {
      state->common.num_spaces =
          num_bf_dot - (state->common.num_digits +
                        state->common.num_zeros);  // Пробелы для выравнивания
    }

    if (state->common.is_negative) {
      state->common.num_spaces--;
    } else if (flag & OPTION_plus) {
      state->common.num_spaces--;
    } else if (flag & OPTION_space) {
      state->common.num_spaces--;
    }

    add_to_buf(buffer, buf_ptr, specifier, &state->common, flag);

    free(state);
  }
}

// Функция для обработки строки
void handle_string(char specifier, va_list args, int num_bf_dot,
                   int num_aft_dot, unsigned int flag, char *buffer,
                   int *buf_ptr) {
  // Инициализация состояния форматирования
  CommonFormatState *state = init_string_format(specifier, args, num_aft_dot);
  if (state) {
    // Если спецификатор - символ ('c'), и количество цифр больше текущего
    // количества символов
    if (specifier != 'c' && state->num_digits > num_aft_dot &&
        num_aft_dot >= 0) {
      state->num_digits = num_aft_dot;
    }

    // Вычисление количества пробелов для выравнивания
    if (num_bf_dot > state->num_digits) {
      state->num_spaces = num_bf_dot - state->num_digits;
    }

    // Вызов функции для печати с переданными параметрами
    add_to_buf(buffer, buf_ptr, specifier, state, flag);
    free(state);
  }
}

int handle_specifier(char *buffer, int *buf_ptr, char specifier, int num_bf_dot,
                     int num_aft_dot, va_list args, int flag) {
  int result = 0;
  if (specifier == 'd' || specifier == 'i' || specifier == 'u' ||
      specifier == 'x' || specifier == 'X' || specifier == 'o') {
    // Знаковое целое, восьмеричное число
    handle_integer(buffer, buf_ptr, specifier, num_bf_dot, num_aft_dot, args,
                   flag);
  } else if (specifier == 'f' || specifier == 'E' || specifier == 'e' ||
             specifier == 'g' || specifier == 'G') {
    // Число с плавающей точкой, экспоненциальная форма
    handle_float(buffer, buf_ptr, specifier, num_bf_dot, num_aft_dot, args,
                 flag);
  } else if (specifier == 'c' || specifier == 's') {
    // Строка
    handle_string(specifier, args, num_bf_dot, num_aft_dot, flag, buffer,
                  buf_ptr);
  } else if (specifier == 'p') {
    // Указатель
    CommonFormatState *state =
        init_pointer_format(specifier, args, num_aft_dot, &flag);
    // Вычисление количества пробелов для выравнивания
    if (num_bf_dot > state->num_digits) {
      state->num_spaces = num_bf_dot - state->num_digits;
    }
    state->num_spaces -= 2;
    if ((flag & OPTION_plus) || (flag & OPTION_space)) {
      state->num_spaces--;
    }
    add_to_buf(buffer, buf_ptr, specifier, state, flag);
    free(state);
  } else if (specifier == 'n') {
    // Запись количества напечатанных символ
    int *value = va_arg(args, int *);
    *value = *buf_ptr;
  } else if (specifier == '%') {
    // Символ '%'
    buffer[(*buf_ptr)++] = '%';
  } else {
    result = 1;
  }
  return result;
}
