CC = gcc
CFLAGS_T = -Wall -Wextra -pedantic -std=c11 -fprofile-arcs -ftest-coverage -I/opt/homebrew/include
CFLAGS = -Wall -Wextra -std=c11 -pedantic -I/opt/homebrew/include
AR = ar
ARFLAGS = rcs

TARGET = s21_string.a
OBJECTS = s21_string.o s21_sscanf.o s21_sprintf.o # можно убрать s21_sprintf чтобы запустить

all: s21_string.a test gcov_report

s21_string.a: s21_string.o s21_sscanf.o s21_sprintf.o # можно убрать, s21_sprintf чтобы запустить
	$(AR) $(ARFLAGS) s21_string.a $^

s21_string.o: s21_string.c s21_string.h 
	$(CC) $(CFLAGS) -c $< -o $@

test: s21_string.a 
	$(CC) $(CFLAGS_T) -o test_runner tests/string_suite.c tests/s21_memchr_test.c tests/s21_memcmp_test.c tests/s21_strcspn_test.c tests/s21_strerror_test.c tests/s21_memset_test.c tests/s21_memcpy_test.c  tests/s21_insert_test.c tests/s21_strncat_test.c tests/s21_strlen_test.c tests/s21_strncmp_test.c tests/s21_strncpy_test.c tests/s21_strpbrk_test.c tests/s21_strrchr_test.c tests/s21_strtok_test.c tests/s21_strstr_test.c tests/s21_to_lower_test.c tests/s21_to_upper_test.c tests/s21_trim_test.c s21_string.c s21_sprintf.c -lcheck -lsubunit -lm -lpthread -lrt
	- ./test_runner  # Запуск тестов для получения покрытия
	$(CC) $(CFLAGS_T) -o bonus_runner s21_sprintf.c s21_sscanf.c sprintf_sscanf/s21_sprintf_test.c sprintf_sscanf/s21_sscanf_test.c sprintf_sscanf/suite.c s21_string.a -lcheck -lsubunit -lm -lpthread -lrt
	- ./bonus_runner  # Запуск тестов для получения покрытия

gcov_report: test
	gcov *.c  # Генерация отчетов gcov для всех .c файлов
	lcov --capture --directory . --output-file coverage.info  # Создание отчета lcov
	genhtml coverage.info --output-directory coverage_html
	open coverage_html/index.html   # Генерация HTML отчета

check:
	clang-format -style=Google -n *.c *.h
	
clean:
	rm -f $(OBJECTS) $(TARGET) test_runner bonus_runner *.gcda *.gcno *.gcov coverage.info
	rm -rf coverage_html tests/*.gcda tests/*.gcno


rebuild: clean all

