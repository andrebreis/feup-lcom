#pragma once

#define BIT(n) (0x01 << n)
#define swap(type, i, j) {type t = i; i = j; j = t;}

int fileExists(const char* filename);
