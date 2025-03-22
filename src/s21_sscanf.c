#include "s21_sscanf.h"
char *parsStr(char *buffer, const char **str, int length);
long int strToInt(const char *str, variables *var);
long int sysNumber(char *buff, variables *var);
long double stringToFloat(const char *str, variables *var);
long unsigned int hexConvert(char *buff, variables *var);
void *pConvert(char *buff, variables *var);
long unsigned int octConvert(char *buff, variables *var);
int comprassionStr(char *buff, char *buffer);
void processInt(int *cnt, char *buff, const char **s, const char **p, int len,
                variables var, int *kill, int L, int h, int l, int z,
                va_list *args);
void processUnsigned(int *cnt, char *buff, const char **s, const char **p,
                     int len, variables var, int *kill, int L, int h, int l,
                     int z, va_list *args);
void processDot(int *cnt, char *buff, const char **s, int len, variables var,
                int *kill, int L, int h, int l, int z, va_list *args);
void processPoint(int *cnt, char *buff, const char **s, int len, variables var,
                  int *kill, int z, va_list *args);
void processChar(int *cnt, char *buff, const char **s, const char **p, int len,
                 int z, va_list *args);

int s21_sscanf(const char *str, const char *format, ...) {
  variables var = {0};
  va_list args;
  va_start(args, format);
  const char *p = format;
  const char *s = str;
  const char *start = str;
  int kill = 1, cnt = 0;
  while (*s && *p && kill) {
    int h = 0, l = 0, L = 0, z = 0, len = 0;
    char buff[1000] = {0};
    var.checkMiss = 0;
    if (*p == '%') {
      p++;
      while (*p >= '0' && *p <= '9' && *p) {
        len = len * 10 + (*p - '0');
        p++;
      }
      if (*p == '*') z = 1;
      if (*p == 'h')
        h = 1;
      else if (*p == 'l')
        l = 1;
      else if (*p == 'L')
        L = 1;
      if (z || h || l || L) p++;
      if (*p == 'd' || *p == 'i') {
        processInt(&cnt, buff, &s, &p, len, var, &kill, L, h, l, z, &args);
      } else if (*p == 'x' || *p == 'X' || *p == 'u' || *p == 'o') {
        processUnsigned(&cnt, buff, &s, &p, len, var, &kill, L, h, l, z, &args);
      } else if (*p == 'f' || *p == 'e' || *p == 'E' || *p == 'g' ||
                 *p == 'G') {
        processDot(&cnt, buff, &s, len, var, &kill, L, h, l, z, &args);
      } else if (*p == 'p') {
        processPoint(&cnt, buff, &s, len, var, &kill, z, &args);
      } else if (*p == 'c' || *p == 's') {
        processChar(&cnt, buff, &s, &p, len, z, &args);
      } else if (*p == 'n') {
        int *n = va_arg(args, int *);
        while (isspace(*s)) s++;
        *n = (int)(s - start);
      }
    }
    p++;
    len = 0;
  }
  va_end(args);
  return cnt;
}

void processChar(int *cnt, char *buff, const char **s, const char **p, int len,
                 int z, va_list *args) {
  if (**p == 'c') {
    char *c = va_arg(*args, char *);
    while (isspace(**s)) (*s)++;
    if (!z) *c = **s;
    (*s)++;
    if (*c != '\0' && !z) (*cnt)++;
  } else if (**p == 's') {
    parsStr(buff, s, len);
    if (!z) {
      char *d = va_arg(*args, char *);
      s21_strncpy(d, buff, s21_strlen(buff));
    }
    if (!z) (*cnt)++;
  }
}

void processPoint(int *cnt, char *buff, const char **s, int len, variables var,
                  int *kill, int z, va_list *args) {
  parsStr(buff, s, len);
  if (z)
    pConvert(buff, &var);
  else {
    void **p = va_arg(*args, void **);
    *p = pConvert(buff, &var);
  }
  if (var.checkMiss > 0) {
    *s -= s21_strlen(buff) - var.checkMiss;
    if (!z) (*cnt)++;
  } else if (var.checkMiss == 0)
    *kill = 0;
}

void processInt(int *cnt, char *buff, const char **s, const char **p, int len,
                variables var, int *kill, int L, int h, int l, int z,
                va_list *args) {
  parsStr(buff, s, len);
  if (z && **p == 'd')
    strToInt(buff, &var);
  else if (z && **p == 'i')
    sysNumber(buff, &var);
  else if (h == 1) {
    int *d = va_arg(*args, int *);
    if (**p == 'd')
      *d = (long int)(short int)strToInt(buff, &var);
    else if (**p == 'i')
      *d = (long int)(short int)sysNumber(buff, &var);
  } else if (l == 1) {
    long int *d = va_arg(*args, long int *);
    if (**p == 'd')
      *d = (long int)strToInt(buff, &var);
    else if (**p == 'i')
      *d = (long int)sysNumber(buff, &var);
  } else if (L == 1)
    *kill = 0;
  else {
    int *d = va_arg(*args, int *);
    if (**p == 'd')
      *d = (long int)(int)strToInt(buff, &var);
    else if (**p == 'i')
      *d = (long int)(int)sysNumber(buff, &var);
  }
  if (var.checkMiss > 0) {
    *s -= s21_strlen(buff) - var.checkMiss;
    if (!z) (*cnt)++;
  } else if (var.checkMiss == 0)
    *kill = 0;
}

void processUnsigned(int *cnt, char *buff, const char **s, const char **p,
                     int len, variables var, int *kill, int L, int h, int l,
                     int z, va_list *args) {
  parsStr(buff, s, len);
  if (z && (**p == 'x' || **p == 'X'))
    hexConvert(buff, &var);
  else if (z && **p == 'u')
    strToInt(buff, &var);
  else if (z && **p == 'o')
    octConvert(buff, &var);
  else if (h == 1) {
    unsigned int *h = va_arg(*args, unsigned int *);
    if (**p == 'x' || **p == 'X')
      *h = (long unsigned int)(short unsigned int)hexConvert(buff, &var);
    else if (**p == 'u')
      *h = (long unsigned int)(short unsigned int)strToInt(buff, &var);
    else if (**p == 'o')
      *h = (long unsigned int)(short unsigned int)octConvert(buff, &var);
  } else if (l == 1) {
    long unsigned int *h = va_arg(*args, long unsigned int *);
    if (**p == 'x' || **p == 'X')
      *h = (long unsigned int)hexConvert(buff, &var);
    else if (**p == 'u')
      *h = (long unsigned int)strToInt(buff, &var);
    else if (**p == 'o')
      *h = (long unsigned int)octConvert(buff, &var);
  } else if (L == 1)
    *kill = 0;
  else {
    unsigned int *h = va_arg(*args, unsigned int *);
    if (**p == 'x' || **p == 'X')
      *h = (long unsigned int)(unsigned int)hexConvert(buff, &var);
    else if (**p == 'u')
      *h = (long unsigned int)(unsigned int)strToInt(buff, &var);
    else if (**p == 'o')
      *h = (long unsigned int)(unsigned int)octConvert(buff, &var);
  }
  if (var.checkMiss > 0) {
    *s -= s21_strlen(buff) - var.checkMiss;
    if (!z) (*cnt)++;
  } else if (var.checkMiss == 0)
    *kill = 0;
}

void processDot(int *cnt, char *buff, const char **s, int len, variables var,
                int *kill, int L, int h, int l, int z, va_list *args) {
  parsStr(buff, s, len);
  if (z)
    stringToFloat(buff, &var);
  else if (L) {
    long double *f = va_arg(*args, long double *);
    *f = (long double)stringToFloat(buff, &var);
  } else if (l || h)
    *kill = 0;
  else {
    float *f = va_arg(*args, float *);
    *f = (long double)(float)stringToFloat(buff, &var);
  }
  if (var.checkMiss > 0) {
    *s -= s21_strlen(buff) - var.checkMiss;
    if (!z) (*cnt)++;
  } else if (var.checkMiss == 0)
    *kill = 0;
}

char *parsStr(char *buffer, const char **str, int length) {
  char *start = buffer;
  while (isspace(**str) && **str != '\0') {
    (*str)++;
  }
  while (!isspace(**str) && **str &&
         (length == 0 || (int)(buffer - start) < length)) {
    *buffer = **str;
    (*str)++;
    buffer++;
  }
  *buffer = '\0';
  return (char *)buffer;
}

long unsigned int hexConvert(char *buff, variables *var) {
  unsigned int res = 0;
  int f = 1;
  var->checkMiss = 0;
  while (*buff && f) {
    if (buff[0] == '0' && (buff[1] == 'x' || buff[1] == 'X')) {
      buff += 2;
      var->checkMiss += 2;
    }
    if (isdigit(*buff)) {
      res <<= 4;
      res += *buff - '0';
    } else if (*buff >= 'a' && *buff <= 'f') {
      res <<= 4;
      res += *buff - 'a' + 10;
    } else if (*buff >= 'A' && *buff <= 'F') {
      res <<= 4;
      res += *buff - 'A' + 10;
    } else
      f = 0;
    if (f) var->checkMiss++;
    buff++;
  }
  return res;
}

long unsigned int octConvert(char *buff, variables *var) {
  unsigned int res = 0;
  int f = 1;
  var->checkMiss = 0;
  while (*buff && f) {
    if (isdigit(*buff) && *buff >= '0' && *buff <= '7') {
      res = res * 8 + (*buff - '0');
      var->checkMiss++;
    } else
      f = 0;
    buff++;
  }
  return res;
}

void *pConvert(char *buff, variables *var) {
  uintptr_t res = 0;
  int f = 1;
  var->checkMiss = 0;
  if (*buff == '0' && (*(buff + 1) == 'x' || *(buff + 1) == 'X')) {
    buff += 2;
    var->checkMiss += 2;
  }
  while (*buff && f) {
    while (isspace(*buff)) buff++;
    if (isdigit(*buff)) {
      res <<= 4;
      res += *buff - '0';
      var->checkMiss++;
    } else if (*buff >= 'a' && *buff <= 'f') {
      res <<= 4;
      res += *buff - 'a' + 10;
      var->checkMiss++;
    } else if (*buff >= 'A' && *buff <= 'F') {
      res <<= 4;
      res += *buff - 'A' + 10;
      var->checkMiss++;
    } else
      f = 0;
    buff++;
  }
  return (void *)res;
}

long int strToInt(const char *str, variables *var) {
  int res = 0;
  int sign = 1;
  var->checkMiss = 0;
  while (isspace(*str)) {
    str++;
  }
  if (*str == '-') {
    sign = -1;
    str++;
    var->checkMiss++;
  } else if (*str == '+') {
    str++;
    var->checkMiss++;
  }

  while (*str != ' ' && *str && *str >= '0' && *str <= '9') {
    res = res * 10 + (*str - '0');
    str++;
    var->checkMiss++;
  }

  return res * sign;
}

long double stringToFloat(const char *str, variables *var) {
  int sign = 1;
  int d = 0, f = 1;
  float res = 0;
  float drob = 0;
  var->checkMiss = 0;
  while (isspace(*str)) str++;
  if (*str == '-') {
    sign = -1;
    str++;
    var->checkMiss++;
  } else if (*str == '+') {
    sign = 1;
    str++;
    var->checkMiss++;
  }
  while (*str != ' ' && *str && *str >= 48 && *str <= 57) {
    res = res * 10 + (*str - '0');
    str++;
    var->checkMiss++;
  }

  if (*str == '.' && f) {
    str++;
    var->checkMiss++;
    while (*str != ' ' && *str && *str >= 48 && *str <= 57) {
      d++;
      res += (*str - '0') / pow(10, d);
      str++;
      var->checkMiss++;
    }
    res += drob / d;
  }
  return res * sign;
}

long int sysNumber(char *buff, variables *var) {
  int i_r = 0;
  int base = 10;
  int sign = 1;
  var->checkMiss = 0;
  char resBuff[100] = {0};
  if (*buff == '-') {
    buff++;
    sign = -1;
    var->checkMiss++;
  } else if (*buff == '+') {
    buff++;
    var->checkMiss++;
  }
  if (*buff == '0' && (*(buff + 1) == 'x' || *(buff + 1) == 'X')) {
    base = 16;
    buff += 2;
    var->checkMiss += 2;
  } else if (buff[0] == '0' && (buff[1] != 'x' && buff[1] != 'X')) {
    base = 8;
    buff++;
    var->checkMiss++;
  }
  while ((*buff >= 48 && *buff <= 57) || (*buff >= 65 && *buff <= 70) ||
         (*buff >= 97 && *buff <= 102)) {
    resBuff[i_r] = *buff;
    var->checkMiss++;
    buff++;
    i_r++;
  }
  resBuff[i_r] = '\0';
  int result = (int)strtol(resBuff, S21_NULL, base);
  return sign * result;
}
