#include "i8042.h"

.global _scanCode

_scanCode:
.byte 0


.global _asmReadFromKBD

_asmReadFromKBD:
xor %al, %al
inb $OUT_BUF, %al
movb %al, _scanCode
ret
