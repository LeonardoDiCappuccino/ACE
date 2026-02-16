#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "stm32f0xx_hal.h"

// Symbols provided by the linker script.
extern char _end;    // End of .bss
extern char _estack; // Top of stack

// UART handle for printf redirection (defined in application)
extern UART_HandleTypeDef huart1;

static char *heap_end;

caddr_t _sbrk(int incr)
{
    if (heap_end == 0)
    {
        heap_end = &_end;
    }

    char *prev_heap_end = heap_end;
    if ((heap_end + incr) > &_estack)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

// Direct write call to UART1
int _write(int fd, const void *buf, size_t count)
{
    if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, count, HAL_MAX_DELAY);
        return (int)count;
    }
    
    errno = EBADF;
    return -1;
}

int _read(int fd, void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;
    return 0;
}

int _close(int fd)
{
    (void)fd;
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    (void)fd;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    (void)fd;
    return 1;
}

int _lseek(int fd, int ptr, int dir)
{
    (void)fd;
    (void)ptr;
    (void)dir;
    return 0;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}

void _exit(int status)
{
    (void)status;
    while (1)
    {
    }
}
