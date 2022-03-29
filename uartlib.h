//------------------------------------------------------------------------------
// UART LIB Header file
//------------------------------------------------------------------------------
#ifndef __UARTLIB_H__
#define __UARTLIB_H__

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

//------------------------------------------------------------------------------
typedef unsigned char       byte;
typedef unsigned char       uchar;
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef unsigned short      word;
typedef unsigned long       ulong;
typedef enum {false, true}  bool;

//------------------------------------------------------------------------------
typedef struct byte__t {
    unsigned char   bit0 : 1;   // LSB
    unsigned char   bit1 : 1;
    unsigned char   bit2 : 1;
    unsigned char   bit3 : 1;
    unsigned char   bit4 : 1;
    unsigned char   bit5 : 1;
    unsigned char   bit6 : 1;
    unsigned char   bit7 : 1;
}   byte_t;

typedef union byte__u {
    unsigned char   b;
    byte_t          bits;
}   byte_u;

//------------------------------------------------------------------------------
typedef struct protocol_variable__t {
	word	p_sp;
	word	p_ep;
	word	size;
	byte	open;
	byte	*buf;
}   ptc_var_t;

typedef struct protocol_function__t {
    ptc_var_t   var;
    int         (*pcheck)(ptc_var_t *p);
    int         (*pcatch)(ptc_var_t *p);
}   ptc_func_t;

typedef struct protocol_group__t {
    byte        pcnt;
	ptc_func_t  *p;
}   ptc_grp_t;

//------------------------------------------------------------------------------
extern  void        ptc_set_status (ptc_grp_t *ptc_grp, byte ptc_num, byte status);
extern  void        ptc_q (ptc_grp_t *ptc_grp, byte ptc_num, byte idata);
extern  void        ptc_event (ptc_grp_t *ptc_grp, byte idata);
extern  byte        ptc_func_init (ptc_grp_t *ptc_grp, byte ptc_num, byte ptc_size, 
    int (*chk_func)(ptc_var_t *var), int (*cat_func)(ptc_var_t *var));
extern  ptc_grp_t   *ptc_grp_init (byte ptc_count);
extern  void        ptc_grp_close (ptc_grp_t *ptc_grp);
//------------------------------------------------------------------------------
extern  int     uart_init (const char *dev_name, speed_t baud);
extern  void    uart_close (int fd);

//------------------------------------------------------------------------------
#endif  // #define __UARTLIB_H__

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
