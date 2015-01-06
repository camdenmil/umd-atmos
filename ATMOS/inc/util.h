#ifndef UTIL_H_
#define UTIL_H_

#define noinline __attribute__ ((__noinline__)) //Fewer underscores to type

#define delay(ms) _delay_ms(ms) //Arduino style delay commands
#define delay_us(us) _delay_us(us)

#define CONCAT(a, b) a ## b //Easier to read than the concatenation operator

//Some quick defines for the PORTS
#define B0	B, 0
#define B1	B, 1
#define B2	B, 2
#define B3	B, 3
#define B4	B, 4
#define B5	B, 5
#define B6	B, 6
#define B7	B, 7

#define D0	D, 0
#define D1	D, 1
#define D2	D, 2
#define D3	D, 3
#define D4	D, 4
#define D5	D, 5
#define D6	D, 6
#define D7	D, 7

#define E0	E, 0
#define E1	E, 1
#define E2	E, 2
#define E3	E, 3
#define E4	E, 4
#define E5	E, 5
#define E6	E, 6
#define E7	E, 7

#define F0	F, 0
#define F1	F, 1
#define F2	F, 2
#define F3	F, 3
#define F4	F, 4
#define F5	F, 5
#define F6	F, 6
#define F7	F, 7

#define G1	G, 1
#define G3	G, 2
#define G4	G, 3

#define bit_set_concat(reg, reg2, bit)		(CONCAT(reg, reg2) |= _BV(bit)) //
#define bit_clr_concat(reg, reg2, bit)		(CONCAT(reg, reg2) &= ~_BV(bit))
#define bit_tog_concat(reg, reg2, bit)		(CONCAT(reg, reg2) ^= _BV(bit))
#define bit_is_set_concat(reg, reg2, bit)	(bit_is_set(CONCAT(reg, reg2), bit))

#define HIGH			bit_set_concat
#define LOW				bit_clr_concat
#define TOGGLE			bit_tog_concat
#define pinWrite(x, s)	s(PORT, x)

#define OUTPUT			bit_set_concat
#define INPUT			bit_clr_concat
#define pinMode(x, s)	s(DDR, x)

#define PULLUP_ENABLE	bit_set_concat
#define PULLUP_DISABLE	bit_clr_concat
#define pinPullup(x, s) s(PORT, x)

#define PINHIGH			true
#define PINLOW			false
#define pinRead(x)		bit_is_set_concat(PIN, x)

#define bits_set(data, bits)	((data) |= (bits))
#define bits_clr(data, bits)	((data) &= (~(bits)))
#define bit_set(data, x)		bits_set((data), _BV(x))
#define bit_clr(data, x)		bits_clr((data), _BV(x))

#endif