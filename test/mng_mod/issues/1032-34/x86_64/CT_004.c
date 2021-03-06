#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#include "./test_chk.h"
#include "./test_rusage.h"

#define TEST_NAME "CT_004"

void *subth_func(void *arg)
{
	char *buf_subth = NULL;

	/* mmap 16MB */
	printf("    ----  mmap and access 16MB (%d KB) in sub thread  ----\n", 16 * 1024);
	buf_subth = mmap(0, 16 * M_BYTE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
	memset(buf_subth, 0xff, 16 * M_BYTE);

	printf("    ----  munmap 16MB (%d KB) in sub thread ----\n", 16 * 1024);
	/* unmap */
	munmap(buf_subth, 16 * M_BYTE);

	printf("    ----  sub thread exit  ----\n");
}

int main(int argc, char* argv[])
{
	int rc;
	char *buf = NULL;
	struct rusage cur_rusage;
	long cur_maxrss, prev_maxrss = 0, delta_maxrss;
	int status;
	pthread_t sub_th;

	printf("----  just started ----\n");
	/* check rusage 1st */
	rc = getrusage(RUSAGE_SELF, &cur_rusage);
	CHKANDJUMP(rc == -1, "getrusage 1st");

	cur_maxrss = get_rusage_maxrss(&cur_rusage);
	delta_maxrss = cur_maxrss - prev_maxrss;
	printf("[ RUSAGE_SELF ]\n");
	OKNG(cur_maxrss < 0 ,
			"  maxrss: %d KB (+ %d KB)", cur_maxrss, delta_maxrss);

	prev_maxrss = cur_maxrss;

	printf("----  create thread and join  ----\n");
	status = pthread_create(&sub_th, NULL, subth_func, NULL);
	CHKANDJUMP(status != 0, "pthread_create()");

	pthread_join(sub_th, NULL);

	/* check rusage 2nd */
	rc = getrusage(RUSAGE_SELF, &cur_rusage);
	CHKANDJUMP(rc == -1, "getrusage 2nd");

	cur_maxrss = get_rusage_maxrss(&cur_rusage);
	delta_maxrss = cur_maxrss - prev_maxrss;
	printf("[ RUSAGE_SELF ]\n");
	OKNG(cur_maxrss < (16 * 1024) ,
			"  maxrss: %d KB (+ %d KB)", cur_maxrss, delta_maxrss);

	prev_maxrss = cur_maxrss;

	printf("*** %s PASS\n\n", TEST_NAME);
	return 0;

fn_fail:

	printf("*** %s FAILED\n\n", TEST_NAME);
	return -1;
}
