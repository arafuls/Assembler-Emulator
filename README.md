# Assembler-Emulator
An assembler and emulator for a decimal computer with 10,000 words worth of memory, where each word consists of 6 decimal digits.

**OPERATION CODE AND MEANINGS**
* ADD 01 ACC <-- c(ACC) + c(ADDR)
* SUBTRACT 02 ACC <-- c(ACC) - c(ADDR)
* MULTIPLY 03 ACC <-- c(ACC) * c(ADDR)
* DIVIDE 04 ACC <-- c(ACC) / c(ADDR)
* LOAD 05 ACC <-- c(ADDR)
* STORE 06 ADDR <-- c(ACC)
* READ 07 A line is read and its first 6 digits are placed in the specified address.
* WRITE 08 c(ADDR) is displayed
* BRANCH 09 go to ADDR for next instruction
* BRANCH MINUS 10 go to ADDR if c(ACC) < 0
* BRANCH ZERO 11 go to ADDR if c(ACC) = 0
* BRANCH POSITIVE 12 go to ADDR if c(ACC) > 0
* HALT 13 terminate execution
