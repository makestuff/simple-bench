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
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>

static inline int count0(uint32_t x) {
	return (int)x;
}

static inline int count1(uint32_t x) {
    if ( x >= 1E5 ) {
        if ( x >= 1E7 ) {
            if ( x >= 1E8 ) {
                return (x >= 1E9) ? 10 : 9;
            } else {
                return 8;
            }
        } else {
            return (x >= 1E6) ? 7 : 6;
        }
    } else {
        if ( x >= 1E2 ) {
            if ( x >= 1E3 ) {
                return (x >= 1E4) ? 5 : 4;
            } else {
                return 3;
            }
        } else {
            return (x >= 1E1) ? 2 : 1;
        }
    }
}

static inline int count2(uint32_t x) {
    if ( x >= 1E9 ) {
        return 10;
    } else if ( x >= 1E8 ) {
        return 9;
    } else if ( x >= 1E7 ) {
        return 8;
    } else if ( x >= 1E6 ) {
        return 7;
    } else if ( x >= 1E5 ) {
        return 6;
    } else if ( x >= 1E4 ) {
        return 5;
    } else if ( x >= 1E3 ) {
        return 4;
    } else if ( x >= 1E2 ) {
        return 3;
    } else if ( x >= 1E1 ) {
        return 2;
    } else {
        return 1;
    }
}

static inline int count3(uint32_t x) {
	return (int)log10(x) + 1;
}

static inline int count4(uint32_t x) {
	uint64_t wtest = 10;
	int digits = 1;
	while ( x/wtest ) {
		wtest *= 10;
		digits++;
	}
	return digits;
}

static char printBuf[11];
static inline int count5(uint32_t x) {
	sprintf(printBuf, "%u", x);
	return strlen(printBuf);
}

void countDigits0(size_t length, const uint32_t *dataArray, int *countArray) {
	while ( length ) {
		*countArray = count0(*dataArray);
		countArray++;
		dataArray++;
		length--;
	}
}
void countDigits1(size_t length, const uint32_t *dataArray, int *countArray) {
	while ( length ) {
		*countArray = count1(*dataArray);
		countArray++;
		dataArray++;
		length--;
	}
}
void countDigits2(size_t length, const uint32_t *dataArray, int *countArray) {
	while ( length ) {
		*countArray = count2(*dataArray);
		countArray++;
		dataArray++;
		length--;
	}
}
void countDigits3(size_t length, const uint32_t *dataArray, int *countArray) {
	while ( length ) {
		*countArray = count3(*dataArray);
		countArray++;
		dataArray++;
		length--;
	}
}
void countDigits4(size_t length, const uint32_t *dataArray, int *countArray) {
	while ( length ) {
		*countArray = count4(*dataArray);
		countArray++;
		dataArray++;
		length--;
	}
}
void countDigits5(size_t length, const uint32_t *dataArray, int *countArray) {
	while ( length ) {
		*countArray = count5(*dataArray);
		countArray++;
		dataArray++;
		length--;
	}
}
void test(void) {
	size_t i;
	const uint32_t arr[] = {
		0, 1, 9,
		10, 11, 99,
		100, 101, 999,
		1000, 1001, 9999,
		10000, 10001, 99999,
		100000, 100001, 999999,
		1000000, 1000001, 9999999,
		10000000, 10000001, 99999999,
		100000000, 100000001, 999999999,
		1000000000, 1000000001, UINT_MAX
	};
	for ( i = 0; i < sizeof(arr)/sizeof(arr[0]); i++ ) {
		printf("%u: %d\n", arr[i], 23 /*count1(arr[i])*/);
	}
}
