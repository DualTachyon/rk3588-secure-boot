/* Copyright 2024 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include <stdint.h>
#include <string.h>

void *memcpy(void *a, const void *b, size_t n)
{
	uint8_t *dst = (uint8_t *)a;
	const uint8_t *src = (const uint8_t *)b;

	while (n-- > 0) {
		*dst++ = *src++;
	}

	return a;
}

void *memset(void *a, int b, size_t n)
{
	uint8_t *dst = (uint8_t *)a;

	while (n-- > 0)
		*dst++ = (uint8_t)b;

	return a;
}

int memcmp(const void *a, const void *b, size_t n)
{
	const uint8_t *c = (const uint8_t *)a;
	const uint8_t *d = (const uint8_t *)b;

	while (n-- > 0) {
		if (*c != *d) {
			return (*c - *d);
		}
		c++;
		d++;
	}

	return 0;
}

