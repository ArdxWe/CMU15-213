/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /*
    allone 得到全为 1 的 bit
    allzero 得到全为 0 的 bit
    两者取反与则使得不同的 bits 为 1 相同为 0
    达到取反的效果
  */
  int allone = x & y;
  int allzero = (~x) & (~y);

  int ans = (~allone) & (~allzero);
  
  return ans;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /*
  tmin 首位为 1 其余 bits 全 0
  */
  int ans = 1 << 31;

  return ans;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /*
  x 是 111111 和 011111 时 a 是 0
  x 是 111111 b 是 1
  */
  int a = !!(~((x + 1) ^ x));
  int b = !(~x);
  return !(a | b);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /*
  与 0xAAAAAAAA 与看是否等于 0xAAAAAAAA
  */
  int a = 0xAA;
  int b = a << 8;
  int c = b << 8;
  int d = c << 8;

  int f = a | b | c | d;  // 0xAAAAAAAA

  int ans = !(~((f & x) ^ (~f)));

  return ans; 
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  /*
  取反加 1
  */
  return (~x) + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*
  high 保证 3位以上正确
  three 为 1 则 two 和 one 必须为 0
  */

  int high = !(~(((~0x3F) ^ x) | 0xF));

  int three = x & 0x8;
  int two = x & 0x4;
  int one = x &0x2;

  return high & ((!three) | ((!two) & (!one)));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /*
  左边： x == 0 ===> z
        x != 0 ===> 0xFFFFFFFF
  右边  x == 0 ===> 0xFFFFFFFF
       x != 1 ===> y
  左右与运算
   */
  return (((~(!!x)) + 1) | z) & ((~(~(!!x) + 1)) | y);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {

  int f = 1 << 31;
  int xisfu =!!(x & f);
  int yisfu = !(y & f);

  int xaddnoy = (x + (~y) + 1);

  int isyaddnoxup = !!(xaddnoy & f);

  return (!xaddnoy) | (xisfu & (!yisfu) & isyaddnoxup) | ((!xisfu) & yisfu & isyaddnoxup) | (xisfu & yisfu); 
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /*
  倍数得到 x是否含有1
  含有1 =====> all == 1
  没有1 =====> all == 0
  */
  int all;

  all = (x >> 0x10) | x;
  all = (all >> 0x8) | all;
  all = (all >> 0x4) | all;
  all = (all >> 0x2) | all;
  all = (all >> 0x1) | all;

  all = all & 0x1;

  return all ^ 0x1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {

  int thirtytwo = (x >> 31) & 1;
  int thirtyone = ((x >> 30) & 1) ^ thirtytwo;
  int thirty = (thirtytwo ^ ((x >> 29) & 1)) | thirtyone;
  int twentynine = (thirtytwo ^ ((x >> 28) & 1)) | thirty;
  int twentyeight = (thirtytwo ^((x >> 27) & 1)) | twentynine;
  int twentyseven = (thirtytwo ^((x >> 26) & 1)) | twentyeight;
  int twentysix = (thirtytwo ^((x >> 25) & 1)) | twentyseven;
  int twentyfive = (thirtytwo ^((x >> 24) & 1)) | twentysix;
  int twentyfour = (thirtytwo ^((x >> 23) & 1)) | twentyfive;
  int twentythree = (thirtytwo ^((x >> 22) & 1)) | twentyfour;
  int twentytwo = (thirtytwo ^((x >> 21) & 1)) | twentythree;
  int twentyone = (thirtytwo ^((x >> 20) & 1)) | twentytwo;
  int twenty = (thirtytwo ^((x >> 19) & 1)) | twentyone;
  int onenine = (thirtytwo ^((x >> 18) & 1)) | twenty;
  int oneeight = (thirtytwo ^((x >> 17) & 1)) | onenine;
  int oneseven = (thirtytwo ^((x >> 16) & 1)) | oneeight;
  int onesix = (thirtytwo ^((x >> 15) & 1)) | oneseven;
  int onefive = (thirtytwo ^((x >> 14) & 1)) | onesix;
  int onefour = (thirtytwo ^((x >> 13) & 1)) | onefive;
  int onethree = (thirtytwo ^((x >> 12) & 1)) | onefour;
  int onetwo = (thirtytwo ^((x >> 11) & 1)) | onethree;
  int oneone = (thirtytwo ^((x >> 10) & 1)) | onetwo;
  int ten = (thirtytwo ^((x >> 9) & 1)) | oneone;
  int nine = (thirtytwo ^((x >> 8) & 1)) | ten;
  int eight = (thirtytwo ^((x >> 7) & 1)) | nine;
  int seven = (thirtytwo ^((x >> 6) & 1)) | eight;
  int six = (thirtytwo ^((x >> 5) & 1)) | seven;
  int five = (thirtytwo ^((x >> 4) & 1)) | six;
  int four = (thirtytwo ^((x >> 3) & 1)) | five;
  int three = (thirtytwo ^((x >> 2) & 1)) | four;
  int two = (thirtytwo ^((x >> 1) & 1)) | three;
  int one = (thirtytwo ^(x & 1)) | two;



  return one + two + three + four + five + six + seven + eight + nine + ten
         + oneone + onetwo + onethree + onefour + onefive + onesix + oneseven + oneeight + onenine
         + twenty + twentyone + twentytwo + twentythree + twentyfour + twentyfive + twentysix + twentyseven + twentyeight + twentynine
         + thirty + thirtyone + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned one = 0x1 << 31;
  unsigned none = ~(0x1 << 31);

  unsigned eight = 0xFF << 23;
  unsigned neight = ~(0xFF << 23);

  unsigned twothree = 0xFF | (0xFF << 8) | (0x7F << 16);
  unsigned ntwothree = ~(0xFF | (0xFF << 8) | (0x7F << 16));

  unsigned s = (one & uf) >> 31;
  unsigned e = (eight & uf) >> 23;
  unsigned m = twothree & uf;

  unsigned sign = uf | (~(1 << 31));

  if (e == 0){
    if (m == 0)return uf;
    unsigned newtwothree = (m << 1) | ntwothree;
    unsigned neweight = neight;

    if ((m & (1 << 22))) {
      neweight = (1 << 23) | one | twothree;
    }
    return ((uf | eight & neweight) | twothree) & newtwothree;
    
  }

  else if (e == 0xFE) {

    return ((uf | neight) & ntwothree & sign) | (1 << 23);
  }

  else if (e == 0xFF) {
    return uf;
  }
  
  else {
    return (uf | eight) & (((e + 1) << 23) | one | twothree);
  }
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  return 2;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    return 2;
}
