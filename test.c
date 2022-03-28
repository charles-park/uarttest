//------------------------------------------------------------------------------
//
// 2022.03.28 uart communication library(chalres-park)
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
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
int main(int argc, char **argv)
{
    parse_opts(argc, argv);

	if (opt_info)
		dump_fb_info((void *)OPT_DEVICE_NAME);

	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
