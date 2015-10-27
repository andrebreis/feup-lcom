#include "i8042.h"

.global _scanCode

_scanCode:
.byte 0

.global _asmReadFromKBD

_asmReadFromKBD:
xor %al, %al
inb $OUT_BUF, %al
movb %al, %bl
inb $STAT_REG, %al
movb $PAR_ERR, %cl
or $TO_ERR, %cl
and %cl, %al
jnz end
movb %bl, _scanCode
end: ret
