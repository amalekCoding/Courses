#ifndef __MACRO_PROFILER__
#define __MACRO_PROFILER__

	#ifdef PROFILER_ENABLED

		#ifdef MAIN_FILE
			FILE *__log_file = NULL;
		#else
			extern FILE *__log_file;
		#endif

		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>
		#include <time.h>
		#include <unistd.h>
		#include <sys/time.h>

		#define PROFILE \
			struct timespec time_log; \
			clock_gettime(CLOCK_REALTIME, &time_log); \
			if(__log_file == NULL) \
			{ \
				if(NULL == (__log_file = fopen("profiler_result.log", "w"))) \
				{ \
					fprintf(stderr, "Error when opening / creating the log file\n"); \
					exit(EXIT_FAILURE); \
				} \
			} \
			fprintf(__log_file, "%s -- time : %.0f.%.0fs\n", __FUNCTION__, (double)time_log.tv_sec, (double)time_log.tv_nsec/1000.0);

		#define return \
			if(__log_file == NULL) \
			{ \
				fprintf(stderr, "Log file not open!\n"); \
				exit(EXIT_FAILURE); \
			} \
			clock_gettime(CLOCK_REALTIME, &time_log); \
			fprintf(__log_file, "END -- time : %.0f.%.0fs\n", (double)time_log.tv_sec, (double)time_log.tv_nsec/1000.0); \
			if(!strcmp("main", __FUNCTION__)) \
			{ \
				fprintf(stderr, "profiler_result.log created!\n"); \
				fclose(__log_file); \
			} \
			return

	#else

		#define PROFILE

	#endif

#endif
