/* 
 * Copyright (C) 2014 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

uint8_t *loadFile(const char *name, size_t *length);
void countDigits0(size_t length, const uint32_t *dataArray, int *countArray);
void countDigits1(size_t length, const uint32_t *dataArray, int *countArray);
void countDigits2(size_t length, const uint32_t *dataArray, int *countArray);
void countDigits3(size_t length, const uint32_t *dataArray, int *countArray);
void countDigits4(size_t length, const uint32_t *dataArray, int *countArray);
void countDigits5(size_t length, const uint32_t *dataArray, int *countArray);
typedef void (*CDFunc)(size_t length, const uint32_t *dataArray, int *countArray);
CDFunc impls[] = {countDigits0, countDigits1, countDigits2, countDigits3, countDigits4, countDigits5};

void test(void);

int main2(void) {
	test();
	return 0;
}

int main(void) {
	double totalTime, d;
	int i, j;
	int *digitsArray;
	const char *const fileName = "random.dat";
	size_t fileLen = 0;
	const uint32_t *fileData;
	#ifdef WIN32
		LARGE_INTEGER tvStart, tvEnd, freq;
		DWORD_PTR mask = 1;
		SetThreadAffinityMask(GetCurrentThread(), mask);
		QueryPerformanceFrequency(&freq);
	#else
		struct timeval tvStart, tvEnd;
		long long startTime, endTime;
	#endif
	fileData = (const uint32_t *)loadFile(fileName, &fileLen);
	if ( !fileData ) {
		fprintf(stderr, "Unable to load %s!\n", fileName);
		exit(1);
	}
	if ( fileLen & 3 ) {
		fprintf(stderr, "File length must be divisible by four!\n");
		free((void*)fileData);
		exit(2);
	}
	digitsArray = (int *)malloc(fileLen);
	if ( !digitsArray ) {
		free((void*)fileData);
		exit(3);
	}
	fileLen >>= 2;
	for ( i = 0; i < 4; i++ ) {
		printf("Impl %d:\n", i);
		d = 0;
		printf("  Warm-up...\n");
		(*impls[i])(fileLen, fileData, digitsArray);
		for ( j = 0; j < 10; j++ ) {
			#ifdef WIN32
				QueryPerformanceCounter(&tvStart);
				(*impls[i])(fileLen, fileData, digitsArray);
				QueryPerformanceCounter(&tvEnd);
				totalTime = (double)(tvEnd.QuadPart - tvStart.QuadPart);
				totalTime /= freq.QuadPart;
			#else
				gettimeofday(&tvStart, NULL);
				(*impls[i])(fileLen, fileData, digitsArray);
				gettimeofday(&tvEnd, NULL);
				startTime = tvStart.tv_sec;
				startTime *= 1000000;
				startTime += tvStart.tv_usec;
				endTime = tvEnd.tv_sec;
				endTime *= 1000000;
				endTime += tvEnd.tv_usec;
				totalTime = (double)(endTime - startTime);
				totalTime /= 1000000;  // convert from uS to S.
			#endif
			printf("  Run %d: %f\n", j, totalTime);
			d += totalTime;
		}
		printf("  Avg: %f\n", d / 10.0);
	}
	free((void*)digitsArray);
	free((void*)fileData);
	return 0;
}

uint8_t *loadFile(const char *name, size_t *length) {
	FILE *file;
	uint8_t *buffer;
	size_t fileLen;
	size_t returnCode;
	file = fopen(name, "rb");
	if ( !file ) {
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	fileLen = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = (uint8_t *)malloc(fileLen + 1);
	if ( !buffer ) {
		fclose(file);
		return NULL;
	}
	returnCode = fread(buffer, 1, fileLen, file);
	if ( returnCode == fileLen && length != NULL ) {
		*length = fileLen;
	}
	buffer[fileLen] = '\0';
	fclose(file);
	return buffer;
}
