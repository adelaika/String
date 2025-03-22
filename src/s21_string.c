#include "s21_string.h"

int separator(char c, const char *delim);

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *ptr = (const unsigned char *)str;
  void *result = S21_NULL;
  int f = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (ptr[i] == (unsigned char)c && !f) {
      result = (void *)(ptr + i);
      f = 1;
    }
  }
  return result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const char *ptr1 = str1;
  const char *ptr2 = str2;
  int f = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if ((ptr1[i] == '\0' || ptr2[i] == '\0') && !f){
      f = ptr1[i] - ptr2[i];
    }
    if (ptr1[i] != ptr2[i] && !f) {
      f = ptr1[i] - ptr2[i];
    }
  }
  return f;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  const char *s = src;
  char *d = dest;
  for (s21_size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }
  return d;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  char *s = str;
  for (s21_size_t i = 0; i < n; i++) {
    s[i] = c;
  }
  return s;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *d = dest;
  while (*d != '\0') {
    d++;
  }
  for (s21_size_t i = 0; i < n && *src != '\0'; i++) {
    d[i] = src[i];
  }
  d[n] = '\0';
  return dest;
}

char *s21_strchr(const char *str, int c) {
  int f = 0;
  char *result = S21_NULL;
  while (*str != '\0') {
    if (((unsigned char)*str == (unsigned char)c) && !f) {
      result = (char *)str;
      f = 1;
    }
    str++;
  }
  if (c == '\0') {
    result = (char *)str;
  }
  return result;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int f = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if ((str1[i] == '\0' || str2[i] == '\0') && !f){
      f = (unsigned char)str1[i] - (unsigned char)str2[i];
    }
    if (str1[i] != str2[i] && !f) {
      f = (unsigned char)str1[i] - (unsigned char)str2[i];
    }
  }
  return f;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i;
  for (i = 0; i < n; i++) {
    if (src[i] != '\0')
      dest[i] = src[i];
    else
      dest[i] = '\0';
  }
  while (i < n) {
    dest[i] = '\0';
    i++;
  }
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *s1 = str1;
  s21_size_t l = 0;
  int f = 1;
  while (*s1 != '\0' && f) {
    const char *s2 = str2;
    while (*s2 != '\0' && f) {
      if (*s1 == *s2) f = 0;
      s2++;
    }
    if (f) {
      s1++;
      l++;
    }
  }
  return l;
}

char *s21_strerror(int errnum) {
  char *error = S21_NULL;
#ifdef __linux__
  static char *linux_errors[] = {
      "Success",
      "Operation not permitted",
      "No such file or directory",
      "No such process",
      "Interrupted system call",
      "Input/output error",
      "No such device or address",
      "Argument list too long",
      "Exec format error",
      "Bad file descriptor",
      "No child processes",
      "Resource temporarily unavailable",
      "Cannot allocate memory",
      "Permission denied",
      "Bad address",
      "Block device required",
      "Device or resource busy",
      "File exists",
      "Invalid cross-device link",
      "No such device",
      "Not a directory",
      "Is a directory",
      "Invalid argument",
      "Too many open files in system",
      "Too many open files",
      "Inappropriate ioctl for device",
      "Text file busy",
      "File too large",
      "No space left on device",
      "Illegal seek",
      "Read-only file system",
      "Too many links",
      "Broken pipe",
      "Numerical argument out of domain",
      "Numerical result out of range",
      "Resource deadlock avoided",
      "File name too long",
      "No locks available",
      "Function not implemented",
      "Directory not empty",
      "Too many levels of symbolic links",
      "Unknown error 41",
      "No message of desired type",
      "Identifier removed",
      "Channel number out of range",
      "Level 2 not synchronized",
      "Level 3 halted",
      "Level 3 reset",
      "Link number out of range",
      "Protocol driver not attached",
      "No CSI structure available",
      "Level 2 halted",
      "Invalid exchange",
      "Invalid request descriptor",
      "Exchange full",
      "No anode",
      "Invalid request code",
      "Invalid slot",
      "Unknown error 58",
      "Bad font file format",
      "Device not a stream",
      "No data available",
      "Timer expired",
      "Out of streams resources",
      "Machine is not on the network",
      "Package not installed",
      "Object is remote",
      "Link has been severed",
      "Advertise error",
      "Srmount error",
      "Communication error on send",
      "Protocol error",
      "Multihop attempted",
      "RFS specific error",
      "Bad message",
      "Value too large for defined data type",
      "Name not unique on network",
      "File descriptor in bad state",
      "Remote address changed",
      "Can not access a needed shared library",
      "Accessing a corrupted shared library",
      ".lib section in a.out corrupted",
      "Attempting to link in too many shared libraries",
      "Cannot exec a shared library directly",
      "Invalid or incomplete multibyte or wide character",
      "Interrupted system call should be restarted",
      "Streams pipe error",
      "Too many users",
      "Socket operation on non-socket",
      "Destination address required",
      "Message too long",
      "Protocol wrong type for socket",
      "Protocol not available",
      "Protocol not supported",
      "Socket type not supported",
      "Operation not supported",
      "Protocol family not supported",
      "Address family not supported by protocol",
      "Address already in use",
      "Cannot assign requested address",
      "Network is down",
      "Network is unreachable",
      "Network dropped connection on reset",
      "Software caused connection abort",
      "Connection reset by peer",
      "No buffer space available",
      "Transport endpoint is already connected",
      "Transport endpoint is not connected",
      "Cannot send after transport endpoint shutdown",
      "Too many references: cannot splice",
      "Connection timed out",
      "Connection refused",
      "Host is down",
      "No route to host",
      "Operation already in progress",
      "Operation now in progress",
      "Stale file handle",
      "Structure needs cleaning",
      "Not a XENIX named type file",
      "No XENIX semaphores available",
      "Is a named type file",
      "Remote I/O error",
      "Disk quota exceeded",
      "No medium found",
      "Wrong medium type",
      "Operation canceled",
      "Required key not available",
      "Key has expired",
      "Key has been revoked",
      "Key was rejected by service",
      "Owner died",
      "State not recoverable",
      "Operation not possible due to RF-kill",
      "Memory page has hardware error"};
#elif (__APPLE__)
  static char *mac_errors[] = {
      "Undefined error: 0",
      "Operation not permitted",
      "No such file or directory",
      "No such process",
      "Interrupted system call",
      "Input/output error",
      "Device not configured",
      "Argument list too long",
      "Exec format error",
      "Bad file descriptor",
      "No child processes",
      "Resource deadlock avoided",
      "Cannot allocate memory",
      "Permission denied",
      "Bad address",
      "Block device required",
      "Resource busy",
      "File exists",
      "Cross-device link",
      "Operation not supported by device",
      "Not a directory",
      "Is a directory",
      "Invalid argument",
      "Too many open files in system",
      "Too many open files",
      "Inappropriate ioctl for device",
      "Text file busy",
      "File too large",
      "No space left on device",
      "Illegal seek",
      "Read-only file system",
      "Too many links",
      "Broken pipe",
      "Numerical argument out of domain",
      "Result too large",
      "Resource temporarily unavailable",
      "Operation now in progress",
      "Operation already in progress",
      "Socket operation on non-socket",
      "Destination address required",
      "Message too long",
      "Protocol wrong type for socket",
      "Protocol not available",
      "Protocol not supported",
      "Socket type not supported",
      "Operation not supported",
      "Protocol family not supported",
      "Address family not supported by protocol family",
      "Address already in use",
      "Can't assign requested address",
      "Network is down",
      "Network is unreachable",
      "Network dropped connection on reset",
      "Software caused connection abort",
      "Connection reset by peer",
      "No buffer space available",
      "Socket is already connected",
      "Socket is not connected",
      "Can't send after socket shutdown",
      "Too many references: can't splice",
      "Operation timed out",
      "Connection refused",
      "Too many levels of symbolic links",
      "File name too long",
      "Host is down",
      "No route to host",
      "Directory not empty",
      "Too many processes",
      "Too many users",
      "Disc quota exceeded",
      "Stale NFS file handle",
      "Too many levels of remote in path",
      "RPC struct is bad",
      "RPC version wrong",
      "RPC prog. not avail",
      "Program version wrong",
      "Bad procedure for program",
      "No locks available",
      "Function not implemented",
      "Inappropriate file type or format",
      "Authentication error",
      "Need authenticator",
      "Device power is off",
      "Device error",
      "Value too large to be stored in data type",
      "Bad executable (or shared library)",
      "Bad CPU type in executable",
      "Shared library version mismatch",
      "Malformed Mach-o file",
      "Operation canceled",
      "Identifier removed",
      "No message of desired type",
      "Illegal byte sequence",
      "Attribute not found",
      "Bad message",
      "EMULTIHOP (Reserved)",
      "No message available on STREAM",
      "ENOLINK (Reserved)",
      "No STREAM resources",
      "Not a STREAM",
      "Protocol error",
      "STREAM ioctl timeout",
      "Operation not supported on socket",
      "Policy not found",
      "State not recoverable",
      "Previous owner died",
      "Interface output queue is full"};
#endif
  if (errnum >= 0) {
#ifdef __linux__
    if (errnum < (int)(sizeof(linux_errors) / sizeof(linux_errors[0]))) {
      error = linux_errors[errnum];
    }
#elif (__APPLE__)
    if (errnum < (int)(sizeof(mac_errors) / sizeof(mac_errors[0]))) {
      error = mac_errors[errnum];
    }
#endif
  }
#ifdef __linux__
  if (error == S21_NULL) {
    static char unknown[50];
    s21_sprintf(unknown, "Unknown error %d", errnum);
    error = unknown;
  }
#elif (__APPLE__)
  if (error == S21_NULL) {
    static char unknown[50];
    s21_sprintf(unknown, "Unknown error: %d", errnum);
    error = unknown;
  }
#endif
  return error;
}

s21_size_t s21_strlen(const char *str) {
  const char *s = str;
  s21_size_t l = 0;
  for (s21_size_t i = 0; s[i] != '\0'; i++) {
    l += 1;
  }
  return l;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  const char *s1 = str1;
  const char *s = S21_NULL;
  int f = 1;
  while (*s1 != '\0') {
    const char *s2 = str2;
    while (*s2 != '\0' && f) {
      if (*s1 == *s2) {
        s = s1;
        f = 0;
      }
      s2++;
    }
    s1++;
  }
  return (char *)s;
}

char *s21_strrchr(const char *str, int c) {
  const char *last = S21_NULL;
  while (*str != '\0') {
    if (*str == (char)c) {
      last = str;
    }
    str++;
  }
  if (c == '\0') {
    last = (char *)str;
  }
  return (char *)last;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = S21_NULL;
  if (*needle == '\0') res = (char *)haystack;
  while (*haystack != '\0') {
    const char *h = haystack;
    const char *n = needle;
    while (*h != '\0' && *n != '\0' && *h == *n) {
      h++;
      n++;
    }
    if (*n == '\0') res = (char *)haystack;
    haystack++;
  }
  return (char *)res;
}

char *s21_strtok(char *str, const char *delim) {
  static char *last = S21_NULL;
  char *token = S21_NULL;
  if (str != S21_NULL) last = str;
  if (last != S21_NULL) {
    while (*last != '\0' && separator(*last, delim)) last++;
    if (*last != '\0') {
      token = last;
      while (*last != '\0' && !separator(*last, delim)) last++;
      if (*last != '\0') {
        *last = '\0';
        last++;
      }
    }
  }
  return token;
}

int separator(char c, const char *delim) {
  int res = 0;
  while (*delim) {
    if (c == *delim) {
      res = 1;
    }
    delim++;
  }
  return res;
}

void *s21_to_upper(const char *str) {
  if (str == S21_NULL) return S21_NULL;
  int step = 32;
  s21_size_t length = s21_strlen(str);
  char *buffer = malloc(length + 1);
  s21_size_t i = 0;
  while (str[i] != '\0') {
    if (str[i] >= 'a' && str[i] <= 'z') {
      buffer[i] = str[i] - step;
    } else {
      buffer[i] = str[i];
    }
    i++;
  }
  buffer[i] = '\0';
  return buffer;
}

void *s21_to_lower(const char *str) {
  if (str == S21_NULL) return S21_NULL;
  int step = 32;
  s21_size_t i = 0;
  s21_size_t length = s21_strlen(str);
  char *buffer = malloc(length + 1);
  while (str[i]) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      buffer[i] = str[i] + step;
    } else {
      buffer[i] = str[i];
    }
    i++;
  }
  buffer[i] = '\0';
  return buffer;
}

void *s21_insert(const char *src, const char *str, size_t start_index) {
    char * result = S21_NULL;
    if (src != S21_NULL && (str != S21_NULL) && (start_index < s21_strlen(src) || start_index == 0)) {
      s21_size_t str_len = s21_strlen(str);
      s21_size_t i = 0;
      s21_size_t src_len = s21_strlen(src);
      result = (char *)malloc(src_len + str_len + 1); 
      if (result != S21_NULL) {
        for (s21_size_t j = 0; j < start_index; j++) {
          result[i++] = src[j];
        }
        for (s21_size_t j = 0; j < str_len; j++) {
          result[i++] = str[j];
        }
        for (s21_size_t j = start_index; j < src_len; j++) {
          result[i++] = src[j];
        }
        result[i] = '\0';

      }
    }
    return result;
}


void *s21_trim(const char *src, const char *trim_chars) {
  char * res = S21_NULL;
  if (src != S21_NULL && trim_chars != S21_NULL) {
    const char *start = src;
    const char *end = src + s21_strlen(src) - 1;
    while (*start && s21_strchr(trim_chars, *start)) start++;
    while (end > start && s21_strchr(trim_chars, *end)) end--;
    s21_size_t len = end >= start ? (end - start + 1) : 0;
    res = (char *)malloc(len + 1);
    if (res != S21_NULL) {
      s21_strncpy(res, start, len);
      res[len] = '\0';
    }
  }
  return res;

}

