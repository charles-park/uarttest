//------------------------------------------------------------------------------
//
// 2022.03.28 uart communication library(chalres-park)
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <getopt.h>
#include <termios.h> // Contains POSIX terminal control definitions

#include "uartlib.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const char *OPT_DEVICE_NAME = "/dev/ttyS0";
char opt_info = 0;

//------------------------------------------------------------------------------
#if defined(__DEBUG__)
	#define	dbg(fmt, args...)	fprintf(stderr,"%s(%d) : " fmt, __func__, __LINE__, ##args)
#else
	#define	dbg(fmt, args...)
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void print_usage(const char *prog)
{
	printf("Usage: %s [-Di]\n", prog);
	puts("  -D --device    device to use (default /dev/ttyS0)\n"
	     "  -i --info      framebuffer info display.\n"
	);
	exit(1);
}

//------------------------------------------------------------------------------
static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  	1, 0, 'D' },
			{ "info",		0, 0, 'i' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:i", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			OPT_DEVICE_NAME = optarg;
			break;
		case 'i':
            opt_info = 1;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void dump_fb_info (void *pdata)
{
	fprintf(stdout, "INFO : %s\n", (char *)pdata);
}

//------------------------------------------------------------------------------
int chk_func(ptc_var_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != '@')	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != '@')	return 0;
	return 1;
}

int cat_func (ptc_var_t *var)
{
	int i;
	for (i = 1; i < var->size -1; i++)
		printf ("buf[%d] = 0x%02x\n", i, var->buf[(var->p_sp +i)]);

	return 1;
}
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int fd;
	ptc_grp_t *ptc_grp;
	byte idata, icount = 0;;

    parse_opts(argc, argv);

	if (opt_info)
		dump_fb_info((void *)OPT_DEVICE_NAME);

	fd = uart_init (OPT_DEVICE_NAME, B115200);

	ptc_grp = ptc_grp_init (1);
	if (ptc_grp) {
		if (!ptc_func_init (ptc_grp, 0, 10, chk_func, cat_func))
			goto out;
	}

	while (ptc_grp->p[0].var.open) {
		if (read(fd, &idata, 1)) {
			fprintf(stdout, "%d : %c\n", icount++, idata);
			ptc_event (ptc_grp, idata);
		}
	}

out:
	uart_close(fd);
	ptc_grp_close(ptc_grp);
	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
