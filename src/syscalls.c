/**
 * @file syscalls.c
 * @brief Minimal syscall implementations to suppress linker warnings
 * 
 * These functions are required by the C standard library but not used
 * in bare-metal embedded systems. They are implemented here only to
 * suppress linker warnings about unimplemented syscalls.
 */

#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;

/**
 * @brief Close a file (not implemented for bare-metal)
 * @return Always returns -1 with errno set to ENOSYS
 */
int _close(int file) {
    (void)file;
    errno = ENOSYS;
    return -1;
}

/**
 * @brief Seek in a file (not implemented for bare-metal)
 * @return Always returns -1 with errno set to ENOSYS
 */
int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    errno = ENOSYS;
    return -1;
}

/**
 * @brief Read from a file (not implemented for bare-metal)
 * @return Always returns -1 with errno set to ENOSYS
 */
int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    errno = ENOSYS;
    return -1;
}

/**
 * @brief Write to a file (not implemented for bare-metal)
 * @return Always returns -1 with errno set to ENOSYS
 */
int _write(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    errno = ENOSYS;
    return -1;
}

/**
 * @brief Get file status (not implemented for bare-metal)
 * @return Always returns -1 with errno set to ENOSYS
 */
int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    errno = ENOSYS;
    return -1;
}

/**
 * @brief Check if file descriptor is a terminal (not implemented for bare-metal)
 * @return Always returns 1 (pretend everything is a terminal)
 */
int _isatty(int file) {
    (void)file;
    return 1;
}

/**
 * @brief Get process ID (not implemented for bare-metal)
 * @return Always returns 1 (fake process ID)
 */
int _getpid(void) {
    return 1;
}

/**
 * @brief Send signal to process (not implemented for bare-metal)
 * @return Always returns -1 with errno set to ENOSYS
 */
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = ENOSYS;
    return -1;
}

/**
 * @brief Increase program data space (heap management)
 * @param incr Number of bytes to increase heap by
 * @return Pointer to start of new heap area, or -1 on error
 */
extern char _end;  /* Defined by the linker script */
extern char _estack;  /* Defined by the linker script */
static char *heap_end = 0;

void *_sbrk(int incr) {
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;

    /* Check if heap would collide with stack */
    if (heap_end + incr > &_estack) {
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end += incr;
    return (void *)prev_heap_end;
}
