# Understanding DSP (Lyons) — Chunk 17/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 60935

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter Twelve. Digital Data Formats and Their Effects
In digital signal processing, there are many ways to represent numerical data in computing hardware.
These representations, known as data formats,  have  a  profound  effect  on  the  accuracy  and  ease  of
implementation  of  any  given  signal  processing  algorithm.  The  simpler  data  formats  enable
uncomplicated  hardware  designs  to  be  used  at  the  expense  of  a  restricted  range  of  number
representation and susceptibility to arithmetic errors. The more elaborate data formats are somewhat
difficult to implement in hardware, but they allow us to manipulate very large and very small numbers
while  providing  immunity  to  many  problems  associated  with  digital  arithmetic.  The  data  format
chosen for any given application can mean the difference between processing success and failure—
it’s where our algorithmic rubber meets the road.
In this chapter, we’ll introduce the most common types of fixed-point digital data formats and show
why and when they’re used. Next, we’ll use analog-to-digital (A/D) converter operations to establish
the precision and dynamic range afforded by these fixed-point formats along with the inherent errors
encountered  with  their  use.  Finally,  we’ll  cover  the  interesting  subject  of  floating-point  binary
formats.
12.1 Fixed-Point Binary Formats
Within digital hardware, numbers are represented by binary digits known as bits—in fact, the term bit
originated from the words Binary digIT. A single bit can be in only one of two possible states: either
a one or a zero.† A six-bit binary number could, for example, take the form 101101, with the leftmost
bit known as the most significant bit (msb); the rightmost bit is called the least significant bit (lsb).
The number of bits in a binary number is known as the word length—hence 101101 has a word length
of six. Like the decimal number system so familiar to us, the binary number system assumes a weight
associated  with  each  digit  in  the  number.  That  weight  is  the  base  of  the  system  (two  for  binary
numbers  and  ten  for  decimal  numbers)  raised  to  an  integral  power.  To  illustrate  this  with  a  simple
example, the decimal number 4631 is
† Binary numbers are used because early electronic computer pioneers quickly realized that it was much more practical and reliable to
use electrical devices (relays, vacuum tubes, transistors, etc.) that had only two states, on or off. Thus, the on/off state of a device could
represent a single binary digit.
(12-1)
The factors 103, 102, 101, and 100  are  the  digit  weights  in  Eq. (12-1).  Similarly,  the  six-bit  binary
number 101101 is equal to decimal 45 as shown by
(12-2)
Using subscripts to signify the base of a number, we can write Eq. (12-2) as 1011012 = 4510. Equation
(12-2)  shows  us  that,  like  decimal  numbers,  binary  numbers  use  the  place value  system  where  the
position of a digit signifies its weight. If we use B to denote a number system’s base, the place value
representation of the four-digit number a3a2a1a0 is
(12-3)
In Eq. (12-3), Bn  is  the  weight  multiplier  for  the  digit  an,  where  0  ≤  an  ≤  B−1.  (This  place  value
system of representing numbers is very old—so old, in fact, that its origin is obscure. However, with
its  inherent  positioning  of  the  decimal  or  binary  point,  this  number  system  is  so  convenient  and
powerful that its importance has been compared to that of the alphabet[1].)
12.1.1 Octal Numbers
As the use of minicomputers and microprocessors rapidly expanded in the 1960s, people grew tired
of manipulating long strings of ones and zeros on paper and began to use more convenient ways to
represent binary numbers. One way to express a binary number is an octal format, with its base of
eight.  (Of  course,  the  only  valid  digits  in  the  octal  format  are  0  to  7—the  digits  8  and  9  have  no
meaning in octal representation.)
Converting  from  binary  to  octal  is  as  simple  as  separating  the  binary  number  into  three-bit  groups
starting from the right. For example, the binary number 101010012 can be converted to octal format as
101010012 →    10 | 101 | 001 = 2518.
Thus  the  octal  format  enables  us  to  represent  an  eight-digit  binary  value  with  a  simpler  three-digit
octal  value.  However,  the  relentless  march  of  technology  is  pushing  octal  numbers,  like  wooden
tennis rackets, into extinction.
12.1.2 Hexadecimal Numbers
Today the predominant binary number representation format is the hexadecimal number format using
16  as  its  base.  Converting  from  binary  to  hexadecimal  is  done,  this  time,  by  separating  the  binary
number  into  four-bit  groups  starting  from  the  right.  The  binary  number  101010012  is  converted  to
hexadecimal format as
101010012 →    1010 | 1001 = A916.
If you haven’t seen the hexadecimal format used before, don’t let the A9  digits  confuse  you.  In  this
format, the characters A, B, C, D, E, and F represent the digits whose decimal values are 10, 11, 12,
13, 14, and 15 respectively. We convert the two groups of bits above to two hexadecimal digits by
starting with the left group of bits, 10102 = 1010 = A16, and 10012 = 910 = 916. Hexadecimal format
numbers also use the place value system, meaning that A916 = (A · 161 + 9 · 160). For convenience,
then, we can represent the eight-digit 101010012 with the two-digit number A916. Table 12-1 lists the
permissible digit representations in the number systems discussed thus far.
Table 12-1 Allowable Digit Representations versus Number System Base
In  the  above  example  we  used  a  subscripted  16  to  signify  a  hexadecimal  number.  Note  that  it’s
common,  in  the  literature  of  binary  number  formats,  to  have  hexadecimal  numbers  preceded  by
special  characters  to  signify  that  indeed  they  are  hexadecimal.  You  may  see,  for  example,  numbers
like $A9 or 0xA9 where the “$” and “0x” characters specify the follow-on digits to be hexadecimal.
12.1.3 Sign-Magnitude Binary Format
For  binary  numbers  to  be  at  all  useful  in  practice,  they  must  be  able  to  represent  negative  values.
Binary numbers do this by dedicating one of the bits in a binary word to indicate the sign of a number.
Let’s  consider  a  popular  binary  format  known  as  sign  magnitude.  Here,  we  assume  that  a  binary
word’s  leftmost  bit  is  a  sign  bit  and  the  remaining  bits  represent  the  magnitude  of  a  number  that  is
always  positive.  For  example,  we  can  say  that  the  four-bit  number  00112  is  +310  and  the  binary
number 10112 is equal to −310, or
Of course, using one of the bits as a sign bit reduces the magnitude of the numbers we can represent. If
an  unsigned  binary  number’s  word  length  is  b  bits,  the  number  of  different  values  that  can  be
represented is 2b. An eight-bit word, for example, can represent 28 = 256 different integral values.
With  zero  being  one  of  the  values  we  have  to  express,  a  b-bit  unsigned  binary  word  can  represent
integers from 0 to 2b−1. The largest value represented by an unsigned eight-bit word is 28−1 = 25510
= 111111112. In the sign-magnitude binary format a b-bit word can represent only a magnitude of ±2b
−1−1, so the largest positive or negative value we can represent by an eight-bit sign-magnitude word
is ±28−1−1 = ±127.
12.1.4 Two’s Complement Format
Another  common  binary  number  scheme,  known  as  the  two’s  complement  format,  also  uses  the
leftmost  bit  as  a  sign  bit.  The  two’s  complement  format  is  the  most  convenient  numbering  scheme
from a hardware design standpoint and has been used for decades. It enables computers to perform
both addition and subtraction using the same hardware adder logic. To obtain the negative version of
a positive two’s complement number, we merely complement (change a one to a zero, and change a
zero  to  a  one)  each  bit,  add  a  binary  one  to  the  complemented  word,  and  discard  any  bits  carried
beyond  the  original  word  length.  For  example,  with  00112  representing  a  decimal  3  in  two’s
complement format, we obtain a negative decimal 3 through the following steps:
In the two’s complement format, a b-bit word can represent positive amplitudes as great as 2b−1−1,
and  negative  amplitudes  as  large  as  −2b−1.  Table  12-2  shows  four-bit  word  examples  of  sign-
magnitude and two’s complement binary formats.
Table 12-2 Integer Binary Number Formats
While using two’s complement numbers, we have to be careful when adding two numbers of different
word lengths. Consider the case where a four-bit number is added to an eight-bit number:
No problem so far. The trouble occurs when our four-bit number is negative. Instead of adding a +3 to
the +15, let’s try to add a −3 to the +15:
The above arithmetic error can be avoided by performing what’s called a sign-extend operation on
the four-bit number. This process, typically performed automatically in hardware, extends the sign bit
of the four-bit negative number to the left, making it an eight-bit negative number. If we sign-extend
the −3 and then perform the addition, we’ll get the correct answer:
12.1.5 Offset Binary Format
Another useful binary number scheme is known as the offset binary format. While this format is not as
common as two’s complement, it still shows up in some hardware devices. Table 12-2 shows offset
binary format examples for four-bit words. Offset binary represents numbers by subtracting 2b−1 from
an unsigned binary value. For example, in the second row of Table 12-2, the offset binary number is
11102. When this number is treated as an unsigned binary number, it’s equivalent to 1410. For four-bit
words b = 4 and 2b−1 = 8, so 1410 − 810 = 610,  which  is  the  decimal  equivalent  of  11102  in  offset
binary. The difference between the unsigned binary equivalent and the actual decimal equivalent of
the offset binary numbers in Table 12-2 is always −8. This kind of offset is sometimes referred to as a
bias when the offset binary format is used. (It may interest the reader that we can convert back and
forth  between  the  two’s  complement  and  offset  binary  formats  merely  by  complementing  a  word’s
most significant bit.)
The  history,  arithmetic,  and  utility  of  the  many  available  number  formats  is  a  very  broad  field  of
study. A thorough and very readable discussion of the subject is given by Knuth in reference [2].
12.1.6 Fractional Binary Numbers
All of the binary numbers we’ve considered so far had integer decimal values. Noninteger decimal
numbers, numbers with nonzero digits to the right of the decimal point, can also be represented with
binary  numbers  if  we  use  a  binary  point,  also  called  a  radix  point,  identical  in  function  to  our
familiar decimal point. (As such, in the binary numbers we’ve discussed so far, the binary point is
assumed  to  be  fixed  just  to  the  right  of  the  rightmost,  lsb,  bit.)  For  example,  using  the  symbol  ◊  to
denote  a  binary  point,  the  six-bit  unsigned  binary  number  11◊01012  is  equal  to  decimal  3.3125  as
shown by
(12-4)
For our 11◊01012 example in Eq. (12-4)  the  binary  point  is  set  between  the  second  and  third  most
significant bits and we call that binary number a fractional number. Having a stationary position for
the binary point is why this binary number format is called fixed-point binary. The unsigned number
11◊01012 has two integer bits and four fractional bits, so, in the parlance of binary numbers, such a
number is said to have a 2.4, “two dot four,” format (two integer bits and four fractional bits).
Two’s complement binary numbers can also have this integer plus fraction format, and Table 12-3
shows, for example, the decimal value ranges for all possible eight-bit two’s complement fractional
binary numbers. Notice how the 8.0-format row in Table 12-3 shows the decimal values associated
with  an  eight-bit  two’s  complement  binary  number  whose  binary  point  is  to  the  right  of  the  lsb,
signifying an all-integer binary number. On the other hand, the 1.7-format row in Table 12-3 shows
the decimal values associated with an eight-bit two’s complement binary number whose binary point
is just to the right of the msb (the sign bit), signifying an all-fraction binary number.
Table 12-3 Eight-Bit, Two’s Complement, Fractional Format Values
The decimal value range of a general fractional two’s complement binary number is
(12-5)
where the “# of integer bits” notation means the number of bits to the left of the binary point and “# of
fraction bits” means the number of bits to the right of the binary point.
Table 12-3 teaches us two important lessons. First, we can place the implied binary point anywhere
we wish in the eight-bit word, just so long as everyone accessing the data agrees on that binary point
placement  and  the  designer  keeps  track  of  that  placement  throughout  all  of  the  system’s  arithmetic
computations.  Binary  arithmetic  hardware  behavior  does  not  depend  on  the  “agreed  upon”  binary
point placement. Stated in different words, binary point placement does not affect two’s complement
binary arithmetic operations. That is, adding or multiplying two binary numbers will yield the same
binary  result  regardless  of  the  implied  binary  point  location  within  the  data  words.  We  leave  an
example of this behavior as a homework problem.
Second, for a fixed number of bits, fractional two’s complement binary numbers allow us to represent
decimal  numbers  with  poor  precision  over  a  wide  range  of  values,  or  we  can  represent  decimal
numbers with fine precision but only over a narrow range of values. In practice you must “pick your
poison” by choosing the position of the binary point based on what’s more important to you, number
range or number precision.
Due to their 16-bit internal data paths, it’s very common for programmable 16-bit DSP chips to use a
1.15 format (one integer bit to represent sign, and 15 fractional bits) to represent two’s complement
numbers. These 16-bit signed all-fraction binary numbers are particularly useful because multiplying
two  such  numbers  results  in  an  all-fraction  product,  avoiding  any  unpleasant  binary  overflow
problems,  to  be  discussed  shortly.  (Be  aware  that  this  1.15  format  is  also  called  Q15  format.)
Because the 1.15-format is so commonly used in programmable hardware, we give examples of it and
other 16-bit formats in Table 12-4. In that table, the “resolution” is the decimal value of the format’s
lsb.
Table 12-4 16-Bit Format Values
Multiplication  of  two  1.15  binary  words  results  in  a  2.30-format  (also  called  a  Q30-format)
fractional  number.  That  32-bit  product  word  contains  two  sign  bits  and  30  fractional  bits,  with  the
msb being called an extended sign bit. We have two ways to convert (truncate) such a 32-bit product
to the 1.15 format so that it can be stored as a 16-bit word. They are
• shifting the 32-bit word left by one bit and storing the upper 16 bits, and
• shifting the 32-bit word right by 15 bits and storing the lower 16 bits.
To  conclude  this  fractional  binary  discussion,  we  provide  the  steps  to  convert  a  decimal  number
whose magnitude is less than one, such as an FIR digital filter coefficient, to the 1.15 binary format.
As an example, to convert the decimal value 0.452 to the two’s complement 1.15 binary format:
1.  Multiply  the  absolute  value  of  the  original  decimal  number  0.452  by  32768  (215),  yielding  a
scaled decimal 14811.136.
2.  Round  the  value  14811.136  to  an  integer,  using  your  preferred  rounding  method,  producing  a
scaled decimal value of 14811.
3. Convert the decimal 14811 to a binary integer and place the binary point to the right of the msb,
yielding 0◊011 1001 1101 1011 (39DB16).
4. If the original decimal value was positive, stop now. If the original decimal value was negative,
implement a two’s complement conversion by inverting Step 3’s binary bits and add one.
If  you,  unfortunately,  do  not  have  software  to  perform  the  above  positive  decimal  integer  to  1.15
binary conversion in Step 3, here’s how the conversion can be done (painfully) by hand:
3.1. Divide 14811 by 2, obtaining integer 7405 plus a remainder of 0.5. Because the remainder is not
zero, place a one as the lsb of the desired binary number. Our binary number is 1.
3.2. Divide 7405 by 2, obtaining integer 3702 plus a remainder of 0.5. Because the remainder is not
zero,  place  a  one  as  the  bit  to  the  left  of  the  lsb  bit  established  in  Step  3.1  above.  Our  binary
number is now 11.
3.3.  Divide  3702  by  2,  obtaining  integer  1851  plus  a  remainder  of  zero.  Because  the  remainder  is
zero, place a zero as the bit to the left of the bit established in Step 3.2 above. Our binary number
is now 011.
3.4.  Continue  this  process  until  the  integer  portion  of  the  divide-by-two  quotient  is  zero.  Append
zeros to the left of the binary word to extend its length to 16 bits.
Using the above steps to convert decimal 1481110 to binary 1.15 format proceeds as shown in Table
12-5, producing our desired binary number of 0◊011 1001 1101 1011 (39DB16).
Table 12-5 Decimal 14811 to Binary 1.15 Conversion Example
12.2 Binary Number Precision and Dynamic Range
As  we  implied  earlier,  for  any  binary  number  format,  the  number  of  bits  in  a  data  word  is  a  key
consideration. The more bits used in the word, the better the resolution of the number, and the larger
the maximum value that can be represented.† Assuming that a binary word represents the amplitude of
a signal, digital signal processing practitioners find it useful to quantify the dynamic range of various
binary  number  schemes.  For  a  signed  integer  binary  word  length  of  b+1  bits  (one  sign  bit  and  b
magnitude bits), the dynamic range is defined by
† Some computers use 64-bit words. Now, 264 is approximately equal to 1.8 · 1019—that’s a pretty large number. So large, in fact, that if
we started incrementing a 64-bit counter once per second at the beginning of the universe (≈20 billion years ago), the most significant
four bits of this counter would still be all zeros today.
(12-6)
(12-6′)
The dynamic range measured in dB is
When 2b is much larger than 1, we can ignore the −1 in Eq. (12-6′) and state that
(12-6″)
Equation (12-6″), dimensioned in dB, tells us that the dynamic range of our number system is directly
proportional to the word length. Thus, an eight-bit two’s complement word, with seven bits available
to represent signal magnitude, has a dynamic range of 6.02 · 7 = 42.14 dB. Most people simplify Eq.
(12-6″) by using the rule of thumb that the dynamic range is equal to “6 dB per bit.”
12.3 Effects of Finite Fixed-Point Binary Word Length
The  effects  of  finite  binary  word  lengths  touch  all  aspects  of  digital  signal  processing.  Using  finite
word lengths prevents us from representing values with infinite precision, increases the background
noise in our spectral estimation techniques, creates nonideal digital filter responses, induces noise in
analog-to-digital  (A/D)  converter  outputs,  and  can  (if  we’re  not  careful)  lead  to  wildly  inaccurate
arithmetic results. The smaller the word lengths, the greater these problems will be. Fortunately, these
finite,  word-length  effects  are  rather  well  understood.  We  can  predict  their  consequences  and  take
steps  to  minimize  any  unpleasant  surprises.  The  first  finite,  word-length  effect  we’ll  cover  is  the
errors that occur during the A/D conversion process.
12.3.1 A/D Converter Quantization Errors
Practical  A/D  converters  are  constrained  to  have  binary  output  words  of  finite  length.  Commercial
A/D converters are categorized by their output word lengths, which are normally in the range from 8
to 16 bits. A typical A/D converter input analog voltage range is from −1 to +1 volt. If we used such
an A/D converter having 8-bit output words, the least significant bit would represent
(12-7)
What  this  means  is  that  we  can  represent  continuous  (analog)  voltages  perfectly  as  long  as  they’re
integral multiples of 7.81 millivolts—any intermediate input voltage will cause the A/D converter to
output a best estimate  digital  data  value.  The  inaccuracies  in  this  process  are  called  quantization
errors because an A/D output least significant bit is an indivisible quantity. We illustrate this situation
in Figure 12-1(a), where the continuous waveform is being digitized by an 8-bit A/D converter whose
output is in the sign-magnitude format. When we start sampling at time t = 0, the continuous waveform
happens  to  have  a  value  of  31.25  millivolts  (mv),  and  our  A/D  output  data  word  will  be  exactly
correct  for  sample  x(0).  At  time  T  when  we  get  the  second  A/D  output  word  for  sample  x(1),  the
continuous voltage is between 0 and −7.81 mv. In this case, the A/D converter outputs a sample value
of 10000001, representing −7.81 mv, even though the continuous input was not quite as negative as
−7.81 mv. The 10000001 A/D output word contains some quantization error. Each successive sample
contains quantization error because the A/D’s digitized output values must lie on the horizontal line in
Figure 12-1(a). The difference between the actual continuous input voltage and the A/D converter’s
representation  of  the  input  is  shown  as  the  quantization  error  in  Figure 12-1(b).  For  an  ideal  A/D
converter, the quantization error, a kind of roundoff noise, can never be greater than ±1/2 an lsb, or
±3.905 mv.
Figure 12-1 Quantization errors: (a) digitized x(n) values of a continuous signal; (b) quantization
error between the actual analog signal values and the digitized signal values.
While Figure 12-1(b) shows A/D quantization noise in the time domain, we can also illustrate this
noise in the frequency domain. Figure 12-2(a) depicts a continuous sinewave of one cycle over the
sample interval shown as the dashed line and a quantized version of the time-domain samples of that
wave as the dots. Notice how the quantized version of the wave is constrained to have only integral
values, giving it a stair-step effect oscillating above and below the true unquantized sinewave. The
quantization here is four bits, meaning that we have a sign bit and three bits to represent the magnitude
of the wave. With three bits, the maximum peak values for the wave are ±7. Figure 12-2(b) shows the
discrete Fourier transform (DFT) of a discrete version of the sinewave whose time-domain sample
values are not forced to be integers but have high precision. Notice in this case that the DFT has a
nonzero value only at m  =  1.  On  the  other  hand,  Figure 12-2(c)  shows  the  spectrum  of  the  four-bit
quantized  samples  in  Figure  12-2(a),  where  quantization  effects  have  induced  noise  components
across  the  entire  spectral  band.  If  the  quantization  noise  depictions  in  Figures 12-1(b)  and  12-2(c)
look random, that’s because they are. As it turns out, even though A/D quantization noise is random,
we can still quantify its effects in a useful way.
Figure 12-2 Quantization noise effects: (a) input sinewave applied to a 64-point DFT; (b) theoretical
DFT magnitude of high-precision sinewave samples; (c) DFT magnitude of a sinewave quantized to
four bits.
In the field of communications, people often use the notion of output signal-to-noise ratio, or SNR =
(signal power)/(noise power), to judge the usefulness of a process or device. We can do likewise and
obtain an important expression for the output SNR of an ideal A/D converter, SNRA/D, accounting for
finite  word-length  quantization  effects.  Because  quantization  noise  is  random,  we  can’t  explicitly
represent  its  power  level,  but  we  can  use  its  statistical  equivalent  of  variance  to  define  SNRA/D
measured in dB as
(12-8)
Next,  we’ll  determine  an  A/D  converter’s  quantization  noise  variance  relative  to  the  converter’s
maximum input peak voltage Vp. If the full-scale (−Vp to +Vp volts) continuous input range of a b-bit
A/D converter is 2Vp,  a  single  quantization  level  q  is  that  voltage  range  divided  by  the  number  of
possible A/D output binary values, or q = 2Vp/2b. (In Figure 12-1, for example, the quantization level
q is the lsb value of 7.81 mv.) A depiction of the likelihood of encountering any given quantization
error value, called the probability density function p(e) of the quantization error, is shown in Figure
12-3.
Figure 12-3 Probability density function of A/D conversion roundoff error (noise).
This simple rectangular function has much to tell us. It indicates that there’s an equal chance that any
error  value  between  −q/2 and +q/2  can  occur.  By  definition,  because  probability  density  functions
have an area of unity (i.e., the probability is 100 percent that the error will be somewhere under the
curve), the amplitude of the p(e) density function must be the area divided by the width, or p(e) = 1/q.
From Figure D-7 and Eq. (D-29) in Appendix D, the variance of our uniform p(e) is
(12-9)
We can now express the A/D noise error variance in terms of A/D parameters by replacing q in Eq.
(12-9) with q = 2Vp/2b to get
(12-10)
OK, we’re halfway to our goal—with Eq. (12-10) giving us the denominator of Eq. (12-8), we need
the  numerator.  To  arrive  at  a  general  result,  let’s  express  the  input  signal  in  terms  of  its  root  mean
square (rms), the A/D converter’s peak voltage, and a loading factor LF defined as
(12-11)
† As covered in Appendix D, Section D.2, although the variance σ2  is  associated  with  the  power  of  a  signal,  the  standard  deviation  is
associated with the rms value of a signal.
With the loading factor defined as the input rms voltage over the A/D converter’s peak input voltage,
we square and rearrange Eq. (12-11) to show the signal variance
 as
(12-12)
Substituting Eqs. (12-10) and (12-12) in Eq. (12-8),
(12-13)
Eq. (12-13) gives us the SNRA/D of an ideal b-bit A/D converter in terms of the loading factor and the
number of bits b. Figure 12-4 plots Eq. (12-13)  for  various  A/D  word  lengths  as  a  function  of  the
loading factor. Notice that the loading factor in Figure 12-4 is never greater than −3 dB, because the
maximum  continuous  A/D  input  peak  value  must  not  be  greater  than  Vp  volts.  Thus,  for  a  sinusoid
input, its rms value must not be greater than
 volts (3 dB below Vp).
Figure 12-4 SNRA/D of ideal A/D converters as a function of loading factor in dB.
When the input sinewave’s peak amplitude is equal to the A/D converter’s full-scale voltage Vp, the
full-scale LF is
(12-14)
Under this condition, the maximum A/D output SNR from Eq. (12-13) is
(12-15)
This discussion of SNR relative to A/D converters means three important things to us:
1. An ideal A/D converter will have an SNRA/D defined by Eq. (12-13), so any discrete x(n) signal
produced by a b-bit A/D converter can never have an SNR greater than Eq. (12-13). (Appendix D
dicusses methods for computing the SNR of discrete signals.) For example, let’s say we want to
digitize a continuous signal whose SNR is 55 dB. Using an ideal eight-bit A/D converter with its
full-scale SNRA/D  of  6.02  ·  8  +  1.76  =  49.9  dB  from  Eq.  (12-15),  the  quantization  noise  will
contaminate the digitized values, and the resultant digital signal’s SNR can be no better than 49.9
dB. We’ll have lost signal SNR through the A/D conversion process. (A ten-bit A/D, with its ideal
SNRA/D  ≈  62  dB,  could  be  used  to  digitize  a  55  dB  SNR  continuous  signal  to  reduce  the  SNR
degradation  caused  by  quantization  noise.)  Equations  (12-13)  and  (12-15)  apply  to  ideal  A/D
converters and don’t take into account such additional A/D noise sources as aperture jitter error,
missing output bit patterns, and other nonlinearities. So actual A/D converters are likely to have
SNRs  that  are  lower  than  that  indicated  by  theoretical  Eq. (12-13).  To  be  safe  in  practice,  it’s
sensible to assume that SNRA/D-max is 3 to 6 dB lower than indicated by Eq. (12-15).
2. Equation (12-15) is often expressed in the literature, but it can be a little misleading because it’s
imprudent to force an A/D converter’s input to full scale. It’s wise to drive an A/D converter to
some level below full scale because inadvertent overdriving will lead to signal clipping and will
induce distortion in the A/D’s output. So Eq. (12-15) is overly optimistic, and, in practice, A/D
converter SNRs will be less than indicated by Eq. (12-15). The best approximation for an A/D’s
SNR  is  to  determine  the  input  signal’s  rms  value  that  will  never  (or  rarely)  overdrive  the
converter input, and plug that value into Eq. (12-11) to get the loading factor value for use in Eq.
(12-13).† Again, using an A/D converter with a wider word length will alleviate this problem by
increasing the available SNRA/D.
† By the way, some folks use the term crest factor to describe how hard an A/D converter’s input is being driven. The crest factor is the
reciprocal of the loading factor, or CF = Vp/(rms of the input signal).
3. Remember, now, real-world continuous signals always have their own inherent continuous SNR,
so using an A/D converter whose SNRA/D is a great deal larger than the continuous signal’s SNR
serves no purpose. In this case, we would be wasting A/D converter bits by digitizing the analog
signal’s noise to a high degree of accuracy, which does not improve our digital signal’s overall
SNR. In general, we want the converter’s SNRA/D value to be approximately 6 dB greater than an
analog signal’s SNR.
A word of caution is appropriate here concerning our analysis of A/D converter quantization errors.
The derivations of Eqs. (12-13) and (12-15) are based upon three assumptions:
1. The cause of A/D quantization errors is a stationary random process; that is, the performance of
the A/D converter does not change over time. Given the same continuous input voltage, we always
expect an A/D converter to provide exactly the same output binary code.
2. The probability density function of the A/D quantization error is uniform. We’re assuming that the
A/D converter is ideal in its operation and all possible errors between −q/2 and +q/2 are equally
likely. An A/D converter having stuck bits or missing output codes would violate this assumption.
High-quality  A/D  converters  being  driven  by  continuous  signals  that  cross  many  quantization
levels will result in our desired uniform quantization noise probability density function.
3.  The  A/D  quantization  errors  are  uncorrelated  with  the  continuous  input  signal.  If  we  were  to
digitize  a  single  continuous  sinewave  whose  frequency  was  harmonically  related  to  the  A/D
sample rate, we’d end up sampling the same input voltage repeatedly and the quantization error
sequence would not be random. The quantization error would be predictable and repetitive, and
our quantization noise variance derivation would be invalid. In practice, complicated continuous
signals such as music or speech, with their rich spectral content, avoid this problem.
To  conclude  our  discussion  of  A/D  converters,  let’s  consider  one  last  topic.  In  the  literature  the
reader is likely to encounter the expression
(12-16)
Equation (12-16) is used by test equipment manufacturers to specify the sensitivity of test instruments
using  a  beff  parameter  known  as  the  number  of  effective bits,  or  effective  number  of  bits  (ENOB)
[3–8]. Equation (12-16) is merely Eq. (12-15) solved for b and is based on the assumption that the
A/D  converter’s  analog  input  peak-peak  voltage  spans  roughly  90  percent  of  the  converter’s  full-
scale voltage range. Test equipment manufacturers measure the actual SNR of their product, indicating
its  ability  to  capture  continuous  input  signals  relative  to  the  instrument’s  inherent  noise
characteristics.  Given  this  true  SNR,  they  use  Eq.  (12-16)  to  determine  the  beff  value  for
advertisement in their product literature. The larger the beff, the greater the continuous voltage that can
be accurately digitized relative to the equipment’s intrinsic quantization noise.
12.3.2 Data Overflow
The next finite, word-length effect we’ll consider is called overflow. Overflow is what happens when
the result of an arithmetic operation has too many bits, or digits, to be represented in the hardware
registers designed to contain that result. We can demonstrate this situation to ourselves rather easily
using a simple four-function, eight-digit pocket calculator. The sum of a decimal 9.9999999 plus 1.0
is 10.9999999, but on an eight-digit calculator the sum is 10.999999 as
The  hardware  registers,  which  contain  the  arithmetic  result  and  drive  the  calculator’s  display,  can
hold  only  eight  decimal  digits;  so  the  least  significant  digit  is  discarded  (of  course).  Although  the
above error is less than one part in ten million, overflow effects can be striking when we work with
large numbers. If we use our calculator to add 99,999,999 plus 1, instead of getting the correct result
of 100 million, we’ll get a result of 1. Now that’s an authentic overflow error!
Let’s  illustrate  overflow  effects  with  examples  more  closely  related  to  our  discussion  of  binary
number  formats.  First,  adding  two  unsigned  binary  numbers  is  as  straightforward  as  adding  two
decimal numbers. The sum of 42 plus 39 is 81, or
In this case, two 6-bit binary numbers required 7 bits to represent the results. The general rule is the
sum of m individual b-bit binary numbers can require as many as [b + log2(m)] bits to represent
the results. So, for example, a 24-bit result register (accumulator) is needed to accumulate the sum of
sixteen  20-bit  binary  numbers,  or  20  +  log2(16)  =  24.  The  sum  of  256  eight-bit  words  requires  an
accumulator  whose  word  length  is  [8  +  log2(256)],  or  16  bits,  to  ensure  that  no  overflow  errors
occur.
In the preceding example, if our accumulator word length was six bits, an overflow error occurs as
Here, the most significant bit of the result overflowed the six-bit accumulator, and an error occurred.
With  regard  to  overflow  errors,  the  two’s  complement  binary  format  has  two  interesting
characteristics. First, under certain conditions, overflow during the summation of two numbers causes
no error. Second, with multiple summations, intermediate overflow errors cause no problems if the
final magnitude of the sum of the b-bit two’s complement numbers is less than 2b−1. Let’s illustrate
these properties by considering the four-bit two’s complement format in Figure 12-5, whose binary
values are taken from Table 12-2.
Figure 12-5 Four-bit two’s complement binary numbers.
The first property of two’s complement overflow, which sometimes causes no errors, can be shown
by the following examples:
Then  again,  the  following  examples  show  how  two’s  complement  overflow  sometimes  does  cause
errors:
The  rule  with  two’s  complement  addition  is  if  the  carry  bit  into  the  sign  bit  is  the  same  as  the
overflow bit out of the sign bit, the overflow bit can be ignored, causing no errors; if the carry bit
into the sign bit is different from the overflow bit out of the sign bit, the result is invalid. An even
more interesting property of two’s complement numbers is that a series of b-bit word summations can
be performed where intermediate sums are invalid, but the final sum will be correct if its magnitude
is less than 2b−1. We show this by the following example. If we add a +6 to a +7, and then add a −7,
we’ll encounter an intermediate overflow error but our final sum will be correct, as
The  magnitude  of  the  sum  of  the  three  four-bit  numbers  was  less  than  24−1  (<8),  so  our  result  was
valid. If we add a +6 to a +7, and next add a −5, we’ll encounter an intermediate overflow error, and
our final sum will also be in error because its magnitude is not less than 8.
Another  situation  where  overflow  problems  are  conspicuous  is  during  the  calculation  of  the  fast
Fourier transform (FFT). It’s difficult at first to imagine that multiplying complex numbers by sines
and  cosines  can  lead  to  excessive  data  word  growth—particularly  because  sines  and  cosines  are
never  greater  than  unity.  Well,  we  can  show  how  FFT  data  word  growth  occurs  by  considering  a
decimation-in-time FFT butterfly from Figure 4-14(c), repeated here as Figure 12-6(a), and grinding
through a little algebra. The expression for the x’ output of this FFT butterfly, from Eq. (4-26), is
(12-17)
Figure 12-6 Data overflow scenarios: (a) single decimation-in-time FFT butterfly; (b) 2nd-order IIR
filter.
Breaking up the butterfly’s x and y inputs into their real and imaginary parts and remembering that
, we can express Eq. (12-17) as
(12-18)
If  we  let  α  be  the  twiddle  factor  angle  of  2πk/N,  and  recall  that  e−jα  =  cos(α)  −  jsin(α),  we  can
simplify Eq. (12-18) as
(12-19)
If we look, for example, at just the real part of the x’ output, x’real, it comprises the three terms
(12-20)
If xreal, yreal, and yimag are of unity value when they enter the butterfly and the twiddle factor angle α =
2πk/N happens to be π/4 = 45°, then, x’real can be greater than 2 as
(12-21)
So we see that the real part of a complex number can more than double in magnitude in a single stage
of an FFT. The imaginary part of a complex number is equally likely to more than double in magnitude
in a single FFT stage. Without mitigating this word growth problem, overflow errors could render an
FFT algorithm useless.
Overflow problems can also be troublesome for fixed-point systems containing feedback as shown in
Figure  12-6(b).  Examples  of  such  networks  are  infinite  impulse  response  (IIR)  filters,  cascaded
integrator-comb  (CIC)  filters,  and  exponential  averagers.  The  hardware  register  (accumulator)
containing w(n) must have a binary word width that will hold data values as large as the network’s
DC (zero Hz) gain G times the input signal, or G · x(n). To avoid data overflow, the number of bits in
the w(n)-results register must be at least
(12-22)
 means that if log2(G) is not an integer, round it up to the next larger integer. (As a quick
where
reminder, we can determine the DC gain of a digital network by substituting z = 1 in the network’s z-
domain transfer function.)
OK, overflow problems are handled in one of two ways—by truncation or rounding—each inducing
its own individual kind of quantization errors, as we shall see.
12.3.3 Truncation
Truncation  is  the  process  where  some  number  of  least  significant  bits  are  discarded  from  a  binary
number. A practical example of truncation is the situation where the results of a processing system are
16-bit  signal  samples  that  must  be  passed  on  to  a  12-bit  digital-to-analog  converter.  To  avoid
overflowing  the  converter’s  12-bit  input  register,  the  least  significant  4  bits  of  the  16-bit  signal
samples must be discarded. Thinking about decimal numbers, if we’re quantizing to decimal integer
values, for example, the real value 1.2 would be quantized to 1.
An example of truncation to integer values is shown in Figure 12-7(a), where all values of x in the
range of 0 ≤ x < 1 are set equal to 0, values of x in the range of 1 ≤ x < 2 are set equal to 1, and so on.
The  quantization  level  (value),  in  that  figure,  is  q  =  1.  The  quantization  error  induced  by  this
truncation is the vertical distance between the horizontal bold lines and the dashed diagonal line in
Figure 12-7(a).
Figure 12-7 Truncation: (a) quantization nonlinearities; (b) error probability density function; (c)
binary truncation.
As  we  did  with  A/D  converter  quantization  errors,  we  can  call  upon  the  concept  of  probability
density functions to characterize the quantization errors induced by truncation. The probability density
function  of  truncation  errors,  in  terms  of  the  quantization  level  q,  is  shown  in  Figure  12-7(b).  In
Figure 12-7(a) the quantization level q is 1, so in this case we can have truncation errors as great as
−1. Drawing upon our results from Eqs. (D-11) and (D-12) in Appendix D, the mean and variance of
our uniform truncation error probability density function are expressed as
and
(12-23)
(12-24)
The notion of binary number truncation is shown in Figure 12-7(c), where the ten-bit binary word W
is to be truncated to six bits by discarding the four Truncate bits. So in this binary truncation situation,
q  in  Figure 12-7(b)  is  equal  to  the  least  significant  bit  (lsb)  value  (bit  R0)  of  the  retained  binary
word.
In  a  sense,  truncation  error  is  the  price  we  pay  for  the  privilege  of  using  integer  binary  arithmetic.
One  aspect  of  this  is  the  error  introduced  when  we  use  truncation  to  implement  division  by  some
integer power of two. A quick way of dividing a binary value by 2K is to shift a binary word K bits to
the  right;  that  is,  we’re  truncating  the  data  value  (not  the  binary  word  width)  by  discarding  the
rightmost K bits after the right shift.
For example, let’s say we have the value 31 represented by the six-bit binary number 0111112, and
we  want  to  divide  it  by  16  through  shifting  the  bits  K  =  4  places  to  the  right  and  discarding  those
shifted bits. After the right shift we have a binary quotient of 0000012. Well, we see the significance
of  the  problem  because  this  type  of  division  gave  us  a  result  of  one  instead  of  the  correct  quotient
31/16 = 1.9375. Our division-by-truncation error here is roughly 50 percent of the correct quotient.
Had our original dividend been 63 represented by the six-bit binary number 1111112, dividing it by
16 through a four-bit shift would give us an answer of binary 0000112, or decimal three. The correct
answer,  of  course,  is  63/16  =  3.9375.  In  this  case  the  percentage  error  is  0.9375/3.9375,  or  about
23.8 percent. So, the larger the dividend, the lower the truncation error.
If we study these kinds of errors, we’ll find that truncation error depends on three things: the number
of value bits shifted and discarded, the values of the discarded bits (were those dropped bits ones or
zeros?), and the magnitude of the binary number left over after shifting. Although a complete analysis
of these truncation errors is beyond the scope of this book, a practical example of how division by
truncation can cause serious numerical errors is given in reference [9].
Unfortunately, truncation induces a DC bias (an error whose average is a nonzero negative number)
on the truncated signal samples, as predicted by Eq. (12-23). We see this behavior in Figure 12-7(b)
where the truncation error is always negative. Inducing a constant (DC) error to a signal sequence can
be  troublesome  in  many  applications  because  the  always-negative  truncation  error  can  grow  to  an
unacceptable  level  in  subsequent  computations.  So,  in  an  effort  to  avoid  overflow  errors,  rounding
(discussed in the next section) is often preferred over truncation.
12.3.4 Data Rounding
Rounding  is  where  a  binary  number  requiring  truncation  is  slightly  modified  before  the  truncation
operation  is  performed.  Let’s  review  the  behavior  of  rounding  by  first  defining  rounding  as  the
process wherein a number is modified such that it is subsequently represented by, or rounded off to,
its nearest quantization level. For example, if we’re quantizing to integer values, the decimal number
1.2 would be quantized to 1, and the number 1.6 would be quantized to 2. This is shown in Figure 12-
8(a), where all values of x in the range of −0.5 ≤ x < 0.5 are set equal to 0, values of x in the range of
0.5 ≤ x < 1.5 are set equal to 1, and so on.
Figure 12-8 Rounding: (a) quantization nonlinearities; (b) error probability density function.
The quantization error induced by such a rounding operation is the vertical distance between the bold
horizontal lines and the dashed diagonal line in Figure 12-8(a). The probability density function of the
error induced by rounding, in terms of the quantization level q, is shown in Figure 12-8(b). In Figure
12-8(a) the quantization level is q = 1, so in this case we can have quantization error magnitudes no
greater than q/2, or 1/2. Using our Eqs. (D-11) and (D-12) results from Appendix D, the mean and
variance of our uniform rounding probability density function are expressed as
and
(12-25)
(12-26)
The notion of binary number rounding can be described using Figure 12-7(c), where the binary word
W  is  to  be  truncated  by  discarding  the  four  Truncate  bits.  With  rounding,  the  binary  word  W  is
modified  before  the  Truncate  bits  are  discarded.  So  with  binary  rounding,  q  in  Figure  12-8(b)  is
equal to the lsb value of the preserved binary word R0.
Let’s  not  forget:  the  purpose  of  rounding,  its  goal  in  life,  is  to  avoid  data  overflow  errors  while
reducing  the  DC  bias  error  (an  error  whose  average  is  not  zero)  induced  by  simple  truncation.
Rounding achieves this goal because, in theory, its average error is zero as shown by Eq. (12-25).
Next we discuss two popular methods of data rounding.
A common form of binary data rounding is straightforward to implement. Called round-to-nearest, it
comprises the two-step process of adding one to the most significant (leftmost) of the lsb bits to be
discarded, bit T3 of word W in Figure 12-7(c), and then discarding the appropriate Truncate bits. For
an example of this rounding method, let’s say we have 16-bit signal samples destined to be routed to a
12-bit digital-to-analog converter. To avoid overflowing the converter’s 12-bit input register, we add
a  binary  value  of  10002  (decimal  810  =  23)  to  the  original  16-bit  sample  value  and  then  truncate
(discard) the sum’s least significant 4 bits. As another example of round-to-nearest rounding, if a 32-
bit “long” word is rounded to 16 bits, a value of 215 is added to the long word before discarding the
sum’s 16 least significant bits.
Stated  in  different  words,  this  round-to-nearest  rounding  method  means:  If  the  T3  bit  is  a  one,
increment the R bits by one. Then shift the R bits to the right, discarding the Truncate bits.
The round-to-nearest method does reduce the average (DC bias) of the quantization error induced by
simple  truncation;  however  the  round-to-nearest  method’s  average  error  bias  is  close  to  but  not
exactly equal to zero. (That’s because the R bits, in Figure 12-7(c), are always incremented when the
value of the Truncate bits is equal to the value R0/2. This means that over time the R bits are rounded
up  slightly  more  often  than  they  are  rounded  down.)  With  additional  bit  checking  we  can  force  the
average rounding error to be exactly zero using a scheme called convergent rounding.
Convergent rounding, also called round to even, is a slightly more complicated method of rounding,
but one that yields zero-average rounding error on the rounded binary signal samples. Similar to the
round-to-nearest method, convergent rounding does not always increment Figure 12-7(c)’s R bits (the
value Retain) when the value of the Truncate bits is equal to R0/2. In the convergent rounding scheme,
when Truncate = R0/2, the value Retain is only incremented if its original value was an odd number.
This clever process is shown in Figure 12-9.
Figure 12-9 Convergent rounding.
OK, here’s what we’ve learned about rounding: Relative to simple truncation, rounding requires more
computations, but rounding both minimizes the constant-level (DC bias) quantization error induced by
truncation  alone,  and  rounding  has  a  lower  maximum  quantization  error.  So  rounding  is  often  the
preferred method used to avoid binary data overflow errors. The above two rounding methods can,
by the way, be used in two’s complement number format systems.
As a practical rule, to retain maximum numerical precision, all necessary full-width binary arithmetic
should  be  performed  first  and  then  rounding  (or  truncation)  should  be  performed  as  the  very  last
operation. For example, if we must add twenty 16-bit binary numbers followed by rounding the sum
to  12  bits,  we  should  perform  the  additions  at  full  16-bit  precision  and,  as  a  final  step,  round  the
summation result to 12 bits.
In digital signal processing, statistical analysis of quantization error effects is complicated because
quantization is a nonlinear process. Analytical results depend on the types of quantization errors, the
magnitude of the data being represented, the numerical format used, and which of the many FFT or
digital filter structures we are implementing. Be that as it may, digital signal processing experts have
developed simplified error models whose analysis has proved useful. Although discussion of these
analysis techniques and their results is beyond the scope of this introductory text, many references are
available for the energetic reader[10–18]. (Reference [11] has an extensive reference list of its own
on the topic of quantization error analysis.)
Again,  the  overflow  problems  using  fixed-point  binary  formats—which  we  try  to  alleviate  with
truncation  or  rounding—arise  because  so  many  digital  signal  processing  algorithms  comprise  large
numbers of additions or multiplications. This obstacle, particularly in hardware implementations of
digital filters and the FFT, is avoided by hardware designers through the use of floating-point binary
number formats.
12.4 Floating-Point Binary Formats
Floating-point binary formats allow us to overcome most of the limitations of precision and dynamic
range mandated by fixed-point binary formats, particularly in reducing the ill effects of overflow[19].
Floating-point  formats  segment  a  data  word  into  two  parts:  a  mantissa  m and an exponent e.  Using
these parts, the value of a binary floating-point number n is evaluated as
(12-27)
that is, the number’s value is the product of the mantissa and 2 raised to the power of the exponent.
(Mantissa  is  a  somewhat  unfortunate  choice  of  terms  because  it  has  a  meaning  here  very  different
from  that  in  the  mathematics  of  logarithms.  Mantissa  originally  meant  the  decimal  fraction  of  a
logarithm.†  However,  due  to  its  abundance  in  the  literature  we’ll  continue  using  the  term  mantissa
here.) Of course, both the mantissa and the exponent in Eq. (12-27) can be either positive or negative
numbers.
†  For  example,  the  common  logarithm  (log  to  the  base  10)  of  256  is  2.4082.  The  2  to  the  left  of  the  decimal  point  is  called  the
characteristic of the logarithm and the 4082 digits are called the mantissa. The 2 in 2.4082 does not mean that we multiply .4082 by 102.
The 2 means that we take the antilog of .4082 to get 2.56 and multiply that by 102 to get 256.
Let’s assume that a b-bit floating-point number will use be bits for the fixed-point signed exponent and
bm  bits  for  the  fixed-point  signed  mantissa.  The  greater  the  number  of  be  bits  used,  the  larger  the
dynamic range of the number. The more bits used for bm, the better the resolution, or precision, of the
number.  Early  computer  simulations  conducted  by  the  developers  of  b-bit  floating-point  formats
indicated that the best trade-off occurred with be ≈ b/4 and bm ≈ 3b/4. We’ll see that for typical 32-bit
floating-point formats used today, be ≈ 8 bits and bm ≈ 24 bits.
To take advantage of a mantissa’s full dynamic range, most implementations of floating-point numbers
treat the mantissa as a fractional fixed-point binary number, shift the mantissa bits to the right or left,
so that the most significant bit is a one, and adjust the exponent accordingly. The process of shifting a
binary  bit  pattern  so  that  the  most  significant  bit  is  a  one  is  called  bit  normalization.  When
normalized, the mantissa bits are typically called the fraction of the floating-point number, instead of
the  mantissa.  For  example,  the  decimal  value  3.687510  can  be  represented  by  the  fractional  binary
number 11.10112. If we use a two-bit exponent with a six-bit fraction floating-point word, we can just
as well represent 11.10112 by shifting it to the right two places and setting the exponent to two as
(12-28)
The floating-point word above can be evaluated to retrieve our decimal number again as
(12-29)
After  some  experience  using  floating-point  normalization,  users  soon  realized  that  always  having  a
one in the most significant bit of the fraction was wasteful. That redundant one was taking up a single
bit position in all data words and serving no purpose. So practical implementations of floating-point
formats discard that one, assume its existence, and increase the useful number of fraction bits by one.
This is why the term hidden bit is used to describe some floating-point formats. While increasing the
fraction’s precision, this scheme uses less memory because the hidden bit is merely accounted for in
the hardware arithmetic logic. Using a hidden bit, the fraction in Eq. (12-28)’s floating-point number
is shifted to the left one place and would now be
(12-30)
Recall  that  the  exponent  and  mantissa  bits  were  fixed-point  signed  binary  numbers,  and  we’ve
discussed  several  formats  for  representing  signed  binary  numbers,  i.e.,  sign  magnitude,  two’s
complement, and offset binary. As it turns out, all three signed binary formats are used in industry-
standard floating-point formats. The most common floating-point formats, all using 32-bit words, are
listed in Table 12-6.
Table 12-6 Floating–Point Number Formats
The IEEE P754 floating-point format is the most popular because so many manufacturers of floating-
point integrated circuits comply with this standard[8,20–22]. Its exponent e is offset binary (biased
exponent), and its fraction is a sign-magnitude binary number with a hidden bit that’s assumed to be
20. The decimal value of a normalized IEEE P754 floating-point number is evaluated as
(12-31)
where f is the decimal-formatted value of the fractional bits divided by 223. Value e is the decimal
value of the floating-point number’s exponent bits.
The IBM floating-point format differs somewhat from the other floating-point formats because it uses
a  base  of  16  rather  than  2.  Its  exponent  is  offset  binary,  and  its  fraction  is  sign  magnitude  with  no
hidden bit. The decimal value of a normalized IBM floating-point number is evaluated as
(12-32)
The DEC floating-point format uses an offset binary exponent, and its fraction is sign magnitude with
a hidden bit that’s assumed to be 2−1. The decimal value of a normalized DEC floating-point number
is evaluated as
(12-33)
MIL-STD  1750A  is  a  United  States  Military  Airborne  floating-point  standard.  Its  exponent  e  is  a
two’s  complement  binary  number  residing  in  the  least  significant  eight  bits.  MIL-STD  1750A’s
fraction  is  also  a  two’s  complement  number  (with  no  hidden  bit),  and  that’s  why  no  sign  bit  is
specifically indicated in Table 12-6. The decimal value of a MIL-STD 1750A floating-point number
is evaluated as
(12-34)
Notice how the floating-point formats in Table 12-6 all have word lengths of 32 bits. This was not
accidental.  Using  32-bit  words  makes  these  formats  easier  to  handle  using  8-,  16-,  and  32-bit
hardware processors. That fact not withstanding and given the advantages afforded by floating-point
number formats, these formats do require a significant amount of logical comparisons and branching
to correctly perform arithmetic operations. Reference [23] provides useful flow charts showing what
procedural steps must be taken when floating-point numbers are added and multiplied.
12.4.1 Floating-Point Dynamic Range
Attempting  to  determine  the  dynamic  range  of  an  arbitrary  floating-point  number  format  is  a
challenging exercise. We start by repeating the expression for a number system’s dynamic range from
Eq. (12-6) as
(12-35)
When  we  attempt  to  determine  the  largest  and  smallest  possible  values  for  a  floating-point  number
format, we quickly see that they depend on such factors as
• the position of the binary point
• whether a hidden bit is used or not (If used, its position relative to the binary point is important.)
• the base value of the floating-point number format
• the signed binary format used for the exponent and the fraction (For example, recall from Table
12-2 that the binary two’s complement format can represent larger negative numbers than the sign-
magnitude format.)
• how unnormalized fractions are handled, if at all (Unnormalized, also called gradual underflow,
means  a  nonzero  number  that’s  less  than  the  minimum  normalized  format  but  can  still  be
represented when the exponent and hidden bit are both zero.)
• how exponents are handled when they’re either all ones or all zeros. (For example, the IEEE P754
format treats a number having an all-ones exponent and a nonzero fraction as an invalid number,
whereas  the  DEC  format  handles  a  number  having  a  sign  =  1  and  a  zero  exponent  as  a  special
instruction instead of a valid number.)
Trying to develop a dynamic range expression that accounts for all the possible combinations of the
above factors is impractical. What we can do is derive a rule-of-thumb expression for dynamic range
that’s often used in practice[8,22,24].
Let’s  assume  the  following  for  our  derivation:  the  exponent  is  a  be-bit  offset  binary  number,  the
fraction is a normalized sign-magnitude number having a sign bit and bm magnitude bits, and a hidden
bit is used just left of the binary point. Our hypothetical floating-point word takes the following form:
First we’ll determine what the largest value can be for our floating-point word. The largest fraction is
a one in the hidden bit, and the remaining bm fraction bits are all ones. This would make fraction f =
[1 + (1 − 2−bm)]. The first 1 in this expression is the hidden bit to the left of the binary point, and the
value in parentheses is all bm bits equal to ones to the right of the binary point. The greatest positive
value we can have for the be-bit offset binary exponent is 2(2be−1−1). So the largest value that can be
represented  with  the  floating-point  number  is  the  largest  fraction  raised  to  the  largest  positive
exponent, or
(12-36)
The smallest value we can represent with our floating-point word is a one in the hidden bit times two
raised to the exponent’s most negative value, 2−(2be−1), or
(12-37)
Plugging Eqs. (12-36) and (12-37) into Eq. (12-35),
(12-38)
Now here’s where the thumb comes in—when bm is large, say over seven, the 2−bm value approaches
zero; that is, as bm increases, the all-ones fraction (1 − 2−bm) value in the numerator approaches 1.
Assuming this, Eq. (12-38) becomes
(12-39)
Using Eq. (12-39),  we  can  estimate,  for  example,  the  dynamic  range  of  the  single-precision  IEEE
P754 standard floating-point format with its eight-bit exponent:
(12-40)
Although we’ve introduced the major features of the most common floating-point formats, there are
still more details to learn about floating-point numbers. For the interested reader, the references given
in this section provide a good place to start.
12.5 Block Floating-Point Binary Format
A  marriage  of  fixed-point  and  floating-point  binary  formats  is  known  as  block floating point.  This
scheme  is  used,  particularly  in  dedicated  FFT  integrated  circuits,  when  large  arrays,  or  blocks,  of
associated  data  are  to  be  manipulated  mathematically.  Block  floating-point  schemes  begin  by
examining  all  the  words  in  a  block  of  data,  normalizing  the  largest-valued  word’s  fraction,  and
establishing  the  correct  exponent.  This  normalization  takes  advantage  of  the  fraction’s  full  dynamic
range. Next, the fractions of the remaining data words are shifted appropriately, so that they can use
the exponent of the largest word. In this way, all of the data words use the same exponent value to
conserve hardware memory.
In  FFT  implementations,  the  arithmetic  is  performed  treating  the  block  normalized  data  values  as
fixed-point binary. However, when an addition causes an overflow condition, all of the data words
are  shifted  one  bit  to  the  right  (division  by  two),  and  the  exponent  is  incremented  by  one.  As  the
reader may have guessed, block floating-point formats have increased dynamic range and avoid the
overflow  problems  inherent  in  fixed-point  formats  but  do  not  reach  the  performance  level  of  true
floating-point formats[8,25,26].
References
[1] Neugebauer, O. “The History of Ancient Astronomy,” Journal of Near Eastern Studies, Vol. 4,
1945, p. 12.
[2]  Knuth,  D.  E.  The  Art  of  Computer  Programming:  Seminumerical  Methods,  Vol.  2,  Addison-
Wesley, Reading, Massachusetts, 1981, Section 4.1, p. 179.
[3] Kester, W. “Peripheral Circuits Can Make or Break Sampling-ADC Systems,” EDN Magazine,
October 1, 1992.
[4] Grove, M. “Measuring Frequency Response and Effective Bits Using Digital Signal Processing
Techniques,” Hewlett-Packard Journal, February 1992.
[5]  Tektronix.  “Effective  Bits  Testing  Evaluates  Dynamic  Range  Performance  of  Digitizing
Instruments,” Tektronix Application Note, No. 45W-7527, December 1989.
[6] Ushani, R. “Subranging ADCs Operate at High Speed with High Resolution,” EDN  Magazine,
April 11, 1991.
[7] Demler, M. “Time-Domain Techniques Enhance Testing of High-Speed ADCs,” EDN Magazine,
March 30, 1992.
[8]  Hilton,  H.  “A  10-MHz  Analog-to-Digital  Converter  with  110-dB  Linearity,”  Hewlett-Packard
Journal, October 1993.
[9] Lyons, R. G. “Providing Software Flexibility for Optical Processor Noise Analysis,” Computer
Design, July 1978, p. 95.
[10] Knuth, D. E. The  Art  of  Computer  Programming:  Seminumerical  Methods,  Vol.  2,  Addison-
Wesley, Reading, Massachusetts, 1981, Section 4.2, p. 198.
[11] Rabiner, L. R., and Gold, B. Theory and Application of Digital Signal Processing, Chapter 5,
Prentice Hall, Englewood Cliffs, New Jersey, 1975, p. 353.
[12]  Jackson,  L.  B.  “An  Analysis  of  Limit  Cycles  Due  to  Multiplicative  Rounding  in  Recursive
Digital Filters,” Proc. 7th Allerton Conf. Circuit System Theory, 1969, pp. 69–78.
[13] Kan, E. P. F., and Aggarwal, J. K. “Error Analysis of Digital Filters Employing Floating Point
Arithmetic,” IEEE Trans. Circuit Theory, Vol. CT-18, November 1971, pp. 678–686.
[14]  Crochiere,  R.  E.  “Digital  Ladder  Structures  and  Coefficient  Sensitivity,”  IEEE  Trans.  Audio
Electroacoustics, Vol. AU-20, October 1972, pp. 240–246.
[15]  Jackson,  L.  B.  “On  the  Interaction  of  Roundoff  Noise  and  Dynamic  Range  in  Digital  Filters,”
Bell System Technical Journal, Vol. 49, February 1970, pp. 159–184.
[16]  Roberts,  R.  A.,  and  Mullis,  C.  T.  Digital  Signal  Processing,  Addison-Wesley,  Reading,
Massachusetts, 1987, p. 277.
[17] Jackson, L. B. “Roundoff Noise Analysis for Fixed-Point Digital Filters Realized in Cascade or
Parallel Form,” IEEE Trans. Audio Electroacoustics, Vol. AU-18, June 1970, pp. 107–122.
[18]  Oppenheim,  A.  V.,  and  Schafer,  R.  W.  Discrete-Time  Signal  Processing,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1989, Sections 9.7 and 9.8.
[19] Larimer, J., and Chen, D. “Fixed or Floating? A Pointed Question in DSPs,” EDN Magazine,
August 3, 1995.
[20] Ashton, C. “Floating Point Math Handles Iterative and Recursive Algorithms,” EDN Magazine,
January 9, 1986.
[21]  Windsor,  B.,  and  Wilson,  J.  “Arithmetic  Duo  Excels  in  Computing  Floating  Point  Products,”
Electronic Design, May 17, 1984.
[22] Windsor, W. A. “IEEE Floating Point Chips Implement DSP Architectures,” Computer Design,
January 1985.
[23]  Texas  Instruments  Inc.  Digital  Signal  Processing  Applications  with  the  TMS320  Family:
Theory, Algorithms, and Implementations, SPRA012A, Texas Instruments, Dallas, Texas, 1986.
[24]  Strauss,  W.  I.  “Integer  or  Floating  Point?  Making  the  Choice,”  Computer  Design  Magazine,
April 1, 1990, p. 85.
[25] Oppenheim, A. V., and Weinstein, C. J. “Effects of Finite Register Length in Digital Filtering and
the Fast Fourier Transform,” Proceedings of the IEEE, August 1972, pp. 957–976.
[26]  Woods,  R.  E.  “Transform-Based  Processing:  How  Much  Precision  Is  Needed?”  ESD:  The
Electronic System Design Magazine, February 1987.
Chapter 12 Problems
12.1 Given their specified format, convert the following integer binary numbers to decimal format:
(a) 1100 0111, unsigned,
(b) 1100 0111, sign magnitude,
(c) 1100 0111, two’s complement,
(d) 1100 0111, offset binary.
12.2  Convert  the  following  unsigned  integer  binary  numbers,  given  here  in  hexadecimal  format,  to
decimal:
(a) $A231,
(b) 0x71F.
12.3 Given the hexadecimal integer numbers $07 and $E2 in two’s complement format, what is the
decimal value of $07 minus $E2? Show your work.
12.4 Sign-extend the following two’s complement integer numbers, given in hexadecimal format, to
16 bits and express the results in hexadecimal format:
(a) $45,
(b) $B3.
12.5 Show that the binary addition operation
gives the correct decimal results when the two binary addends and the sum are in the following
two’s complement fractional formats:
(a) 7.1 (7 integer bits and 1 fractional bit),
(b) 6.2 (6 integer bits and 2 fractional bits),
(c) 4.4 (4 integer bits and 4 fractional bits).
12.6 Microchip Technology Inc. produces a microcontroller chip (Part #PIC24F) that accommodates
16-bit  data  words.  When  using  a  two’s  complement  integer  number  format,  what  are  the  most
positive and most negative decimal numbers that can be represented by the microcontroller’s data
word?
12.7  Consider  four-bit  unsigned  binary  words  using  a  2.2  (“two  dot  two”)  “integer  plus  fraction”
format. List all 16 possible binary words in this format and give their decimal equivalents.
12.8 The annual residential property tax in California is 0.0165 times the assessed dollar value of the
property. What is this 0.0165 tax rate factor in a two’s complement 1.15 format? Give the answer
in both binary and hexadecimal representations. Show how you arrived at your solution.
12.9 The decimal number 1/3 cannot be represented exactly with a finite number of decimal digits,
nor with a finite number of binary bits. What would be the base of a number system that would
allow decimal 1/3 to be exactly represented with a finite number of digits?
12.10 If the number 42736 is in a base 6 numbering system, what would be its decimal value?
12.11 Think about a 32-bit two’s complement fixed-point binary number having 31 fractional bits (a
“1.31”  two’s  complement  number).  This  number  format  is  very  common  in  today’s  high-
performance programmable DSP chips.
(a)  What  is  the  most  positive  decimal  value  that  can  be  represented  by  such  a  binary  number?
Show how you arrived at your solution.
(b) What is the most negative decimal value?
12.12 As of this writing, Analog Devices Inc. produces an integrated circuit (Part #AD9958), called
a direct digital synthesizer, that generates high-precision analog sinewaves. The AD9958 uses a
31-bit  binary  word  to  control  the  device’s  output  frequency.  When  the  control  word  is  at  its
minimum  value,  the  device’s  output  frequency  is  zero  Hz.  When  the  control  word  is  at  its
maximum value, the output frequency is 250 MHz. What is the frequency resolution (the frequency
step size) of this sinusoidal signal generator in Hz?
12.13  The  first  commercial  audio  compact  disc  (CD)  players  used  16-bit  samples  to  represent  an
analog audio signal. Their sample rate was fs = 44.1 kHz. Those 16-bit samples were applied to a
digital-to-analog  (D/A)  converter  whose  analog  output  was  routed  to  a  speaker.  What  is  the
combined  data  output  rate  of  the  digital  portion,  measured  in  bytes  (8-bit  binary  words)  per
second, of a stereo CD player?
12.14 When implementing a digital filter using a fixed-point binary number format, care must be taken
to avoid arithmetic overflow errors. With that notion in mind, if the x(n) input samples in Figure
P12-14  are  eight-bit  binary  words,  how  many  bits  are  needed  to  represent  the  y(n)  output
sequence to avoid any data overflow errors? Show how you arrived at your answer.
Figure P12-14
Hint: Review the last portion of the text’s Section 12.3.2.
12.15 Review the brief description of allpass filters in Appendix F. One form of an allpass filter is
shown in Figure P12-15(a). For the filter to have the desired constant magnitude response over its
full operating frequency, coefficient A must be equal to
Figure P12-15
If  the  filter  is  designed  such  that  B  =  2.5,  show  why  we  cannot  achieve  the  desired  constant
frequency  magnitude  response  when  coefficients  A  and  B  are  quantized  using  four-bit  unsigned
binary words in a 2.2 (“two dot two”) “integer plus fraction” format, where AQ and BQ are the
quantized coefficients as shown in Figure P12-15(b).
12.16 National Semiconductors Inc. produces a digital tuner chip (Part #CLC5903), used for building
digital  receivers,  that  has  the  capability  to  amplify  its  output  signal  by  shifting  its  binary  signal
sample values to the left by as few as one bit to as many as seven bits. What is the maximum gain,
measured in dB (decibels), of this tuner’s bit-shifting amplification capability?
12.17  Figure  P12-17  shows  an  algorithm  that  approximates  the  operation  of  dividing  a  sign-
magnitude  binary  number  x(n)  by  an  integer  value  K.  (A  block  containing  the  “—>  2”  symbol
means truncation by way of a binary right shift by two bits.) What is the value of integer K? Show
your work.
Figure P12-17
12.18  When  using  programmable  DSP  chips,  multiplication  is  a  simple  straightforward  operation.
However, when using field-programmable gate arrays (FPGAs), multiplier hardware is typically
difficult to implement and should be avoided whenever possible. Figure P12-18 shows how we
can multiply a binary x(n) input sequence by 54, without the need for multiplier hardware. What
are the values for A and B in Figure P12-18 so that y(n) equals 54 times x(n)?
Figure P12-18
12.19 Consider the network shown in Figure P12-19 which approximates a 2nd-order differentiation
operation. In many DSP implementations (using field-programmable gate arrays, for example) it is
advantageous to minimize the number of multiplications. Assuming that all the sequences in Figure
P12-19 use a binary two’s complement integer number format, what data bit manipulations must
be implemented to eliminate the two multipliers?
Figure P12-19
12.20  Agilent  Inc.  produces  an  A/D  converter  (Model  #DP1400)  whose  sample  rate  is  2×109
samples/second (fs = 2 GHz). This digitizer provides super-fine time resolution samples of analog
signals whose durations are T = 5×10−6 seconds (5 microseconds) as shown in Figure P12-20. If
each converter output sample is stored in one memory location of a computer, how many memory
locations  are  required  to  store  the  converter’s  x(n)  output  sequence  representing  the  5-
microsecond-duration x(t) signal?
Figure P12-20
12.21 Here is a problem often encountered by DSP engineers. Assume we sample exactly three cycles
of a continuous x(t) sinewave resulting in a block of 1024 x(n) time samples and compute a 1024-
point  fast  Fourier  transform  (FFT)  to  obtain  the  FFT  magnitude  samples.  Also  assume  that  we
repeat the sampling and FFT magnitude computations many times and average the FFT magnitude
sequences  to  produce  the  average  magnitude  samples,  |Xave(m)|,  shown  in  Figure  P12-21.  (We
averaged multiple FFT magnitude sequences to increase the accuracy, by reducing the variance, of
our  final  |Xave(m)|  sequence.)  If  the  A/D  converter  produces  ten-bit  binary  words  in  sign-
magnitude format and has an input full-scale bipolar voltage range of ±5 volts, what is the peak
value of the continuous x(t) sinewave? Justify your answer.
Figure P12-21
12.22 Suppose we have a 12-bit A/D converter that operates over an input voltage range of ±5 volts
(10 volts peak-peak). Assume the A/D converter is ideal in its operation and its transfer function
is  that  shown  in  Figure  P12-22  where  the  tick  mark  spacing  of  the  x(t)  and  x(n)  axes  is  the
converter’s quantization-level q.
Figure P12-22
(a) What is the A/D converter’s quantization-level q (least significant bit) voltage?
(b)  What  are  the  A/D  converter’s  maximum  positive  and  maximum  negative  quantization  error
voltages?
(c) If we apply a 7-volt peak-peak sinusoidal voltage to the converter’s analog input, what A/D
output  signal-to-quantization  noise  value,  SNRA/D  in  dB,  should  we  expect?  Show  how  you
arrived at your answer.
12.23 Suppose an A/D converter manufacturer applies a 10-volt peak-peak sinusoidal voltage to their
12-bit  converter’s  analog  input,  conducts  careful  testing,  and  measures  the  converter’s  overall
signal-to-noise level to be 67 dB. What is the effective number of bits value, beff, for their A/D
converter?
12.24 Let’s reinforce our understanding of the quantization errors induced by typical A/D converters.
(a) Figure P12-24 shows the quantized x(n) output integer values of truncating and rounding A/D
converters as a function of their continuous x(t)  input  voltage.  It’s  sensible  to  call  those  bold
stair-step curves the “transfer functions” of the A/D converters. The curves are normalized to
the  A/D  converter’s  quantization-level  voltage  q,  such  that  an  x(t)  value  of  2  represents  a
voltage of 2q  volts.  Draw  the  curves  of  the  quantization  error  as  a  function  of  the  continuous
x(t) input for both truncating and rounding A/D converters.
Figure P12-24
(b) Fill in the following table of important A/D converter quantization error properties in terms of
the A/D converters’ quantization-level voltage q.
12.25  Assume  we  want  to  digitize  the  output  voltage  of  a  temperature  measurement  system,
monitoring  the  internal  temperature  of  an  automobile  radiator,  as  shown  in  Figure  P12-25.  The
system’s manufacturer states that its output voltage v(t) will represent the thermocouple’s junction
temperature  with  an  accuracy  of  2  degrees  Fahrenheit  (1.1  degrees  Celsius),  and  its  operating
range covers temperatures as low as just-freezing water to twice the temperature of boiling water.
To accommodate the precision and operating range of the temperature measurement system, how
many bits, b, do we need for our A/D converter? Show your work.
Figure P12-25
12.26 One useful way to test the performance of A/D converters is to apply a specific analog signal to
the A/D converter’s analog input and perform a histogram of the converter’s output samples. For
example,  if  an  analog  squarewave-like  signal  is  applied  to  an  A/D  converter,  the  converter’s
output sequence might be that shown in the left panel of Figure P12-26(a), and the histogram of the
converter’s output samples is shown in the right panel of Figure P12-26(a). That histogram shows
that there are many converter output samples whose values are −0.2, and many converter output
samples  whose  values  are  0.5,  and  no  sample  values  other  than  −0.2  and  0.5.  The  shape  of  the
histogram curve will indicate any severe defects in the converter’s performance.
Figure P12-26
If a triangular analog signal is applied to an A/D converter, the converter’s output sequence would
be  that  shown  in  the  left  panel  of  Figure P12-26(b) and  the  histogram  of  the  converter’s  output
samples  is  shown  in  the  right  panel  of  Figure  P12-26(b).  This  histogram  shows  that  there  are
(ideally)  an  equal  number  of  samples  at  all  amplitudes  between  −1  and  +1,  which  happens  to
indicate correct converter behavior.
In  the  testing  of  high-frequency  A/D  converters,  high-frequency  analog  square  and  triangular
waves are difficult to generate, so A/D converter engineers use high-frequency analog sinewaves
to test their converters. Assuming that an analog sinewave is used as an input for A/D converter
histogram testing and the converter output samples are those shown in the left panel of Figure P12-
26(c), draw a rough sketch of the histogram of converter output samples.
12.27  In  the  text  we  discussed  how  to  use  the  concept  of  a  uniform  probability  density  function
(PDF), described in Section D.3 of Appendix D, to help us determine the variance (a measure of
power)  of  random  A/D-converter  quantization  noise.  Sometimes  we  want  to  generate  random
noise samples, for testing purposes, that have a uniform PDF such as that shown in Figure P12-27.
What is the value of A for a uniform PDF random sequence whose variance is equal to 2?
Figure P12-27
12.28  Assume  we  have  a  single  numerical  data  sample  value  in  floating-point  binary  format.  What
two  bit  manipulation  methods  exist  to  multiply  that  sample  by  4  without  using  any  multiplier
hardware circuitry?
12.29 Convert the following IEEE P754 floating-point number, given here in hexadecimal format, to a
decimal number:
$C2ED0000
Show your work.
Hint: Don’t forget to account for the hidden one in the IEEE P754 format.

Chapter Thirteen. Digital Signal Processing Tricks
As we study the literature of digital signal processing, we’ll encounter some creative techniques that
professionals  use  to  make  their  algorithms  more  efficient.  These  practical  techniques  are
straightforward examples of the philosophy “Don’t work hard, work smart,” and studying them will
give us a deeper understanding of the underlying mathematical subtleties of DSP. In this chapter, we
present a collection of these tricks of the trade, in no particular order, and explore several of them in
detail because doing so reinforces the lessons we’ve learned in previous chapters.
13.1 Frequency Translation without Multiplication
Frequency  translation  is  often  called  for  in  digital  signal  processing  algorithms.  There  are  simple
schemes for inducing frequency translation by 1/2 and 1/4 of the signal sequence sample rate. Let’s
take a look at these mixing schemes.
13.1.1 Frequency Translation by fs/2
First  we’ll  consider  a  technique  for  frequency  translating  an  input  sequence  by  fs/2  by  merely
multiplying a sequence by (−1)n = 1,−1,1,−1, ..., etc., where fs is the signal sample rate in Hz. This
process  may  seem  a  bit  mysterious  at  first,  but  it  can  be  explained  in  a  straightforward  way  if  we
review Figure 13-1(a).  There  we  see  that  multiplying  a  time-domain  signal  sequence  by  the  (−1)n
mixing sequence is equivalent to multiplying the signal sequence by a sampled cosinusoid where the
mixing  sequence  samples  are  shown  as  the  dots  in  Figure 13-1(a).  Because  the  mixing  sequence’s
cosine repeats every two sample values, its frequency is fs/2. Figures 13-1(b) and 13-1(c) show the
discrete Fourier transform (DFT) magnitude and phase of a 32-sample (−1)n sequence. As such, the
right half of those figures represents the negative frequency range.
Figure 13-1 Mixing sequence comprising (−1)n = 1,−1,1,−1, etc.: (a) time-domain sequence; (b)
frequency-domain magnitudes for 32 samples; (c) frequency-domain phase.
Let’s demonstrate this (−1)n mixing with an example. Consider a real x(n) signal sequence having 32
samples of the sum of three sinusoids whose |X(m)| frequency magnitude and ϕ(m) phase spectra are
as  shown  in  Figures  13-2(a)  and  13-2(b).  If  we  multiply  that  time  signal  sequence  by  (−1)n,  the
resulting x1,−1(n) time sequence will have the magnitude and phase spectra that are shown in Figures
13-2(c) and 13-2(d). Multiplying a time signal by our (−1)n cosine shifts half its spectral energy up
by fs/2 and half its spectral energy down by −fs/2. Notice in these non-circular frequency depictions
that as we count up, or down, in frequency, we wrap around the end points.
Figure 13-2 A signal and its frequency translation by fs/2: (a) original signal magnitude spectrum; (b)
original phase; (c) the magnitude spectrum of the translated signal; (d) translated phase.
Here’s a terrific opportunity for the DSP novice to convolve the (−1)n spectrum in Figure 13-1 with
the  X(m)  spectrum  to  obtain  the  frequency-translated  X1,−1(m)  signal  spectrum.  Please  do  so;  that
exercise  will  help  you  comprehend  the  nature  of  discrete  sequences  and  their  time-  and  frequency-
domain relationships by way of the convolution theorem.
Remember,  now,  we  didn’t  really  perform  any  explicit  multiplications—the  whole  idea  here  is  to
avoid multiplications; we merely changed the sign of alternating x(n) samples to get x1,−1(n). One way
to look at the X1,−1(m) magnitudes in Figure 13-2(c) is to see that multiplication by the (−1)n mixing
sequence flips the positive-frequency band of X(m) (X(0) to X(16)) about the fs/4 Hz point and flips
the negative-frequency band of X(m) (X(17) to X(31)) about the −fs/4 Hz sample. This process can be
used to invert the spectra of real signals when bandpass sampling is used as described in Section 2.4.
By the way, in the DSP literature be aware that some clever authors may represent the (−1)n sequence
with its equivalent expressions of
(13-1)
13.1.2 Frequency Translation by −fs/4
Two other simple mixing sequences form the real and imaginary parts of a complex −fs/4 oscillator
used for frequency down-conversion to obtain a quadrature version (complex and centered at 0 Hz) of
a real bandpass signal originally centered at fs/4. The real (in-phase) mixing sequence is cos(πn/2) =
1,0,−1,0, etc., shown in Figure 13-3(a). That mixing sequence’s quadrature companion is −sin(πn/2)
=  0,−1,0,1,  etc.,  as  shown  in  Figure 13-3(b).  The  spectral  magnitudes  of  those  two  sequences  are
identical  as  shown  in  Figure  13-3(c),  but  their  phase  spectrum  has  a  90-degree  shift  relationship
(what we call quadrature).
Figure 13-3 Quadrature mixing sequences for down-conversion by fs/4: (a) in-phase mixing
sequence; (b) quadrature-phase mixing sequence; (c) the frequency magnitudes of both sequences for
N = 32 samples; (d) the phase of the cosine sequence; (e) phase of the sine sequence.
If we multiply the x(n) sequence whose spectrum is that shown in Figures 13-2(a) and 13-2(b) by the
in-phase (cosine) mixing sequence, the product will have the I(m) spectrum shown in Figures 13-4(a)
and 13-4(b). Again, X(m)’s spectral energy is translated up and down in frequency, only this time the
translation  is  by  ±fs/4.  Multiplying  x(n)  by  the  quadrature-phase  (sine)  sequence  yields  the  Q(m)
spectrum in Figures 13-4(a) and 13-4(c).
Figure 13-4 Spectra after translation down by fs/4: (a) I(m) and Q(m) spectral magnitudes; (b) phase
of I(m) ; (c) phase of Q(m).
Because their time sample values are merely 1, −1, and 0, the quadrature mixing sequences are useful
because  down-conversion  by  fs/4  can  be  implemented  without  multiplication.  That’s  why  these
mixing  sequences  are  of  so  much  interest:  down-conversion  of  an  input  time  sequence  is
accomplished merely with data assignment, or signal routing.
To down-convert a general x(n) = xreal(n) + jximag(n) sequence by fs/4, the value assignments are
(13-2)
If  your  implementation  is  hardwired  gates,  the  above  data  assignments  are  performed  by  means  of
routing signals (and their negatives). Although we’ve focused on down-conversion so far, it’s worth
mentioning that up-conversion of a general x(n) sequence by fs/4 can be performed with the following
data assignments:
(13-3)
We  notify  the  reader,  at  this  point,  that  Section  13.29  presents  an  interesting  trick  for  performing
frequency translation using decimation rather than multiplication.
13.1.3 Filtering and Decimation after fs/4 Down-Conversion
There’s  an  efficient  way  to  perform  the  complex  down-conversion,  by  fs/4,  and  filtering  of  a  real
signal  process  that  we  discussed  for  the  quadrature  sampling  scheme  in  Section 8.9.  We  can  use  a
novel  technique  to  greatly  reduce  the  computational  workload  of  the  linear-phase  lowpass
filters[1–3]. In addition, decimation of the complex down-converted sequence by a factor of two is
inherent, with no effort on our part, in this process.
Considering Figure 13-5(a), notice that if an original x(n) sequence was real-only, and its spectrum is
centered  at  fs/4,  multiplying  x(n)  by  cos(πn/2)  =  1,0,−1,0,  for  the  in-phase  path  and  −sin(πn/2)  =
0,−1,0,1,  for  the  quadrature-phase  path  to  down-convert  x(n)’s  spectrum  to  0  Hz  yields  the  new
complex sequence xnew(n) = xi(n) + xq(n), or
(13-4)
Figure 13-5 Complex down-conversion by fs/4 and filtering by a 5-tap LPF: (a) the process; (b) in-
phase filter data; (c) quadrature-phase filter data.
Next, we want to lowpass filter (LPF) both the xi(n) and xq(n) sequences followed by decimation by a
factor of two.
Here’s the trick. Let’s say we’re using 5-tap FIR filters and at the n = 4 time index the data residing in
the two lowpass filters would be that shown in Figures 13-5(b) and 13-5(c). Due to the alternating
zero-valued  samples  in  the  xi(n) and xq(n)  sequences,  we  see  that  only  five  nonzero  multiplies  are
being performed at this time instant. Those computations, at time index n = 4, are shown in the third
row  of  the  rightmost  column  in  Table 13-1. Because we’re  decimating  by  two,  we  ignore  the  time
index n = 5 computations. The necessary computations during the next time index (n = 6) are given in
the fourth row of Table 13-1, where again only five nonzero multiplies are computed.
Table 13-1 Filter Data and Necessary Computations after Decimation by Two
A  review  of  Table  13-1  tells  us  we  can  multiplex  the  real-valued  x(n)  sequence,  multiply  the
multiplexed sequences by the repeating mixing sequence 1,−1, ..., etc., and apply the resulting xi(n)
and xq(n)  sequences  to  two  filters,  as  shown  in  Figure 13-6(a).  Those  two  filters  have  decimated
coefficients in the sense that their coefficients are the alternating h(k) coefficients from the original
lowpass  filter  in  Figure  13-5.  The  two  new  filters  are  depicted  in  Figure  13-6(b),  showing  the
necessary  computations  at  time  index  n  =  4.  Using  this  new  process,  we’ve  reduced  our
multiplication workload by a factor of two. The original data multiplexing in Figure 13-6(a) is what
implemented our desired decimation by two.
Figure 13-6 Efficient down-conversion, filtering by a 5-tap LPF, and decimation: (a) process block
diagram; (b) the modified filters and data at time n = 4; (c) process when a half-band filter is used.
Here’s  another  feature  of  this  efficient  down-conversion  structure.  If  half-band  filters  are  used  in
Figure 13-5(a), then only one of the coefficients in the modified quadrature lowpass filter is nonzero.
This means we can implement the quadrature-path filtering as K unit delays, a single multiply by the
original half-band filter’s center coefficient, followed by another K delay as depicted in Figure 13-
6(c).  For  an  original  N-tap  half-band  filter,  K  is  the  integer  part  of  N/4.  If  the  original  half-band
filter’s h(N−1)/2 center coefficient is 0.5, as is often the case, we can implement its multiply by an
arithmetic right shift of the delayed xq(n).
This down-conversion process is indeed slick. Here’s another attribute. If the original lowpass filter
in Figure 13-5(a) has an odd number of taps, the coefficients of the modified filters in Figure 13-6(b)
will be symmetrical, and we can use the folded FIR filter scheme (Section 13.7) to reduce the number
of multipliers by almost another factor of two!
Finally, if we need to invert the output xc(n′) spectrum, there are two ways to do so. We can negate the
1,−1, sequence driving the mixer in the quadrature path, or we can swap the order of the single unit
delay and the mixer in the quadrature path.
13.2 High-Speed Vector Magnitude Approximation
The quadrature processing techniques employed in spectrum analysis, computer graphics, and digital
communications  routinely  require  high-speed  determination  of  the  magnitude  of  a  complex  number
(vector V) given its real and imaginary parts, i.e., the in-phase part I and the quadrature-phase part Q.
This magnitude calculation requires a square root operation because the magnitude of V is
(13-5)
Assuming  that  the  sum  I2  +  Q2  is  available,  the  problem  is  to  efficiently  perform  the  square  root
computation.
There are several ways to obtain square roots, but the optimum technique depends on the capabilities
of the available hardware and software. For example, when performing a square root using a high-
level  software  language,  we  employ  whatever  software  square  root  function  is  available.  Accurate
software  square  root  routines,  however,  require  many  floating-point  arithmetic  computations.  In
contrast, if a system must accomplish a square root operation in just a few system clock cycles, high-
speed  magnitude  approximations  are  required[4,5].  Let’s  look  at  a  neat  magnitude  approximation
scheme that avoids the dreaded square root operation.
There  is  a  technique  called  the  αMax+βMin  (read  as  “alpha  max  plus  beta  min”)  algorithm  for
estimating  the  magnitude  of  a  complex  vector.†  It’s  a  linear  approximation  to  the  vector  magnitude
problem that requires the determination of which orthogonal vector, I or Q, has the greater absolute
value. If the maximum absolute value of I or Q is designated by Max, and the minimum absolute value
of either I or Q is Min, an approximation of |V| using the αMax+βMin algorithm is expressed as
†A “Max+βMin” algorithm had been in use, but in 1988 this author suggested expanding it to the αMax+βMin form where α could be a
value other than unity[6].
(13-6)
There are several pairs for the α and β constants that provide varying degrees of vector magnitude
approximation accuracy to within 0.1 dB[4,7]. The αMax+βMin algorithms in reference [8] determine
a  vector  magnitude  at  whatever  speed  it  takes  a  system  to  perform  a  magnitude  comparison,  two
multiplications, and one addition. But those algorithms require, as a minimum, a 16-bit multiplier to
achieve  reasonably  accurate  results.  If,  however,  hardware  multipliers  are  not  available,  all  is  not
lost. By restricting the α and β constants to reciprocals of integer powers of two, Eq. (13-6) lends
itself well to implementation in binary integer arithmetic. A prevailing application of the αMax+βMin
algorithm uses α = 1.0 and β = 0.5. The 0.5 multiplication operation is performed by shifting the value
Min to the right by one bit. We can gauge the accuracy of any vector magnitude estimation algorithm
by plotting its |V| as a function of vector phase angle. Let’s do that. The Max + 0.5Min estimate for a
complex vector of unity magnitude, over the vector angular range of 0 to 90 degrees, is shown as the
solid curve in Figure 13-7. (The curves in Figure 13-7 repeat every 90 degrees.)
Figure 13-7 αMax+βMin estimation performance.
An ideal estimation curve for a unity magnitude vector would have a value of one, and we’ll use this
ideal curve as a yardstick to measure the merit of various αMax+βMin algorithms. Let’s make sure
we know what the solid curve in Figure 13-7 is telling us. That curve indicates that a unity magnitude
vector oriented at an angle of approximately 26 degrees will be estimated by Eq. (13-6) to have a
magnitude  of  1.118  instead  of  the  correct  magnitude  of  one.  The  error  then,  at  26  degrees,  is  11.8
percent. For comparison, two other magnitude approximation curves for various values of α and β are
shown in Figure 13-7.
Although the values for α and β in Figure 13-7 yield somewhat accurate vector magnitude estimates,
there are other values for α and β that deserve our attention because they result in smaller magnitude
estimation  errors.  The  α  =  15/16  and  β  =  15/32  solid  curve  in  Figure  13-8  is  an  example  of  a
reduced-error algorithm. Multiplications by those values of α and β can be performed by multiplying
by 15 and using binary right shifts to implement the divisions by 16 and 32. A mathematically simple,
single-multiply, α = 1 and β = 0.4 algorithm is also shown as the dashed curve[9]. For the interested
reader, the performance of the optimum values for α and β is shown as the dotted curve in Figure 13-
8. (The word optimum, as used here, means minimizing the magnitude estimation error fluctuations
both above and below the ideal unity line.)
Figure 13-8 Alternate αMax+βMin algorithm performance.
To  add  to  our  catalog  of  magnitude  estimation  algorithms,  at  the  expense  of  an  additional
multiply/shift and a compare operation, an accurate magnitude estimation scheme is that defined by
Eq. (13-7)[10]:
(13-7)
Again, the divisions in Eq. (13-7) are implemented as binary right shifts. In a similar vein we mention
an  algorithm  that  exhibits  a  maximum  error  of  a  mere  1  percent,  when  floating-point  arithmetic  is
used, as defined by Eq. (13-7′)[11]:
(13-7′)
The performance curves of the last two magnitude estimation algorithms are shown in Figure 13-9.
Figure 13-9 Additional αMax+βMin algorithm performance.
To summarize the behavior of the magnitude estimation algorithms we just covered so far, the relative
performances of the various algorithms are shown in Table 13-2. The table lists the magnitude of the
algorithms’ maximum error in both percent and decibels. The rightmost column of Table 13-2 is the
mean  squared  error  (MSE)  of  the  algorithms.  That  MSE  value  indicates  how  much  the  algorithms’
results fluctuate about the ideal result of one, and we’d like to have that MSE value be as close to
zero (a flat line) as possible.
Table 13-2 αMax+βMin Algorithm Performance Comparisons
So, the αMax+βMin algorithms enable high-speed vector magnitude computation without the need for
performing  square  root  operations.  Of  course,  with  the  availability  of  floating-point  multiplier
integrated circuits—with their ability to multiply in one or two clock cycles—the α and β coefficients
need not always be restricted to multiples of reciprocals of integer powers of two.
13.3 Frequency-Domain Windowing
There’s an interesting technique for minimizing the calculations necessary to implement windowing of
FFT input data to reduce spectral leakage. There are times when we need the FFT of unwindowed
time-domain data, while at the same time we also want the FFT of that same time-domain data with a
window  function  applied.  In  this  situation,  we  don’t  have  to  perform  two  separate  FFTs.  We  can
perform the FFT of the unwindowed data, and then we can perform frequency-domain windowing on
that FFT result to reduce leakage. Let’s see how.
Recall  from  Section  3.9  that  the  expressions  for  the  Hanning  and  the  Hamming  windows  were
wHan(n)  =  0.5  −0.5cos(2πn/N)  and  wHam(n)  =  0.54  −0.46cos(2πn/N),  respectively,  where  N  is  a
window sequence length. They both have the general cosine function form of
(13-8)
for n = 0, 1, 2, ..., N−1. Looking at the frequency response of the general cosine window function,
using the definition of the DFT, the transform of Eq. (13-8) is
(13-9)
Because
, Eq. (13-9) can be written as
(13-10)
Equation (13-10) looks pretty complicated, but using the derivation from Section 3.13 for expressions
like those summations, we find that Eq. (13-10) merely results in the superposition of three sin(x)/x
functions in the frequency domain. Their amplitudes are shown in Figure 13-10.
Figure 13-10 General cosine window frequency response amplitude.
Notice that the two translated sin(x)/x functions have sidelobes with opposite phase from that of the
center sin(x)/x function. This means that Nα times the mth bin output, minus Nβ/2 times the (m−1)th
bin output, minus β/2 times the (m+1)th bin output will minimize the sidelobes of the mth bin. This
frequency-domain  convolution  process  is  equivalent  to  multiplying  the  input  time  data  sequence  by
the N-valued window function w(n) in Eq. (13-8)[12–14].
For example, let’s say the output of the mth FFT bin is X(m) = am + jbm, and the outputs of its two
neighboring  bins  are  X(m−1)  =  a−1  +  jb−1  and  X(m+1)  =  a+1  +  jb+1.  Then  frequency-domain
windowing for the mth bin of the unwindowed X(m) is as follows:
(13-11)
To  compute  a  windowed  N-point  FFT,  Xthree-term(m),  we  can  apply  Eq.  (13-11),  requiring  4N
additions and 3N multiplications, to the unwindowed N-point FFT result X(m) and avoid having to
perform  the  N  multiplications  of  time-domain  windowing  and  a  second  FFT  with  its  Nlog2(N)
additions and 2Nlog2(N) multiplications. (In this case, we called our windowed results Xthree-term(m)
because we’re performing a convolution of a three-term W(m) sequence with the X(m) sequence.)
To accommodate the m = 0 beginning and the m = N−1 end of our N-point FFT, we effectively wrap
the  FFT  samples  back  on  themselves.  That  is,  due  to  the  circular  nature  of  FFT  samples  based  on
real-valued time sequences, we use
and
(13-11′)
(13-11″)
Now if the FFT’s x(n) input sequence is real-only, then X(0) = a0, and Eq. (13-11′) simplifies to a
real-only Xthree-term (0) = αa0 − βa1.
The neat situation here is the frequency-domain coefficients, values, α and β, for the Hanning window.
They’re  both  0.5,  and  the  multiplications  in  Eq.  (13-11)  can  be  performed  in  hardware  with  two
binary right shifts by a single bit for α = 0.5 and two shifts for each of the two β/2 = 0.25 factors, for
a total of six binary shifts. If a gain of four is acceptable, we can get away with only two left shifts
(one for the real and one for the imaginary parts of X(m)) using
(13-12)
In  application-specific  integrated  circuit  (ASIC)  and  field-programmable  gate  array  (FPGA)
hardware  implementations,  where  multiplies  are  to  be  avoided,  the  binary  shifts  can  be  eliminated
through  hardwired  data  routing.  Thus  only  additions  are  necessary  to  implement  frequency-domain
Hanning  windowing.  The  issues  we  need  to  consider  are  which  window  function  is  best  for  the
application,  and  the  efficiency  of  available  hardware  in  performing  the  frequency-domain
multiplications. Frequency-domain Hamming windowing can be implemented but, unfortunately, not
with simple binary shifts.
Along  with  the  Hanning  and  Hamming  windows,  reference  [14]  describes  a  family  of  windows
known as Blackman windows that provide further FFT spectral leakage reduction when performing
frequency-domain windowing. (Note: Reference [14] reportedly has two typographical errors in the
4-Term  (−74  dB)  window  coefficients  column  on  its  page  65.  Reference  [15]  specifies  those
coefficients to be 0.40217, 0.49703, 0.09892, and 0.00188.) Blackman windows have five nonzero
frequency-domain coefficients, and their use requires the following five-term convolution:
(13-13)
Table 13-3 provides the frequency-domain coefficients for several common window functions.
Table 13-3 Frequency-Domain Windowing Coefficients
Let’s  end  our  discussion  of  the  frequency-domain  windowing  trick  by  saying  this  scheme  can  be
efficient  because  we  don’t  have  to  window  the  entire  set  of  FFT  data;  windowing  need  only  be
performed on those FFT bin outputs of interest to us. An application of frequency-domain windowing
is presented in Section 13.18.
13.4 Fast Multiplication of Complex Numbers
The multiplication of two complex numbers is one of the most common functions performed in digital
signal processing. It’s mandatory in all discrete and fast Fourier transformation algorithms, necessary
for  graphics  transformations,  and  used  in  processing  digital  communications  signals.  Be  it  in
hardware or software, it’s always to our benefit to streamline the processing necessary to perform a
complex multiply whenever we can. If the available hardware can perform three additions faster than
a single multiplication, there’s a way to speed up a complex multiply operation[16].
The multiplication of two complex numbers, a + jb and c + jd, results in the complex product
(13-14)
We can see that Eq. (13-14) requires four multiplications and two additions. (From a computational
standpoint we’ll assume a subtraction is equivalent to an addition.) Instead of using Eq. (13-14), we
can calculate the following intermediate values:
(13-15)
We then perform the following operations to get the final R and I:
(13-16)
The  reader  is  invited  to  plug  the  k  values  from  Eq.  (13-15)  into  Eq.  (13-16)  to  verify  that  the
expressions  in  Eq. (13-16)  are  equivalent  to  Eq.  (13-14).  The  intermediate  values  in  Eq.  (13-15)
required three additions and three multiplications, while the results in Eq. (13-16) required two more
additions.  So  we  traded  one  of  the  multiplications  required  in  Eq.  (13-14)  for  three  addition
operations needed by Eqs. (13-15) and (13-16). If our hardware uses fewer clock cycles to perform
three additions than a single multiplication, we may well gain overall processing speed by using Eqs.
(13-15) and (13-16) instead of Eq. (13-14) for complex multiplication.
13.5 Efficiently Performing the FFT of Real Sequences
Upon  recognizing  its  linearity  property  and  understanding  the  odd  and  even  symmetries  of  the
transform’s  output,  the  early  investigators  of  the  fast  Fourier  transform  (FFT)  realized  that  two
separate, real N-point input data sequences could be transformed using a single N-point complex FFT.
They also developed a technique using a single N-point complex FFT to transform a 2N-point  real
input sequence. Let’s see how these two techniques work.
13.5.1 Performing Two N-Point Real FFTs
The  standard  FFT  algorithms  were  developed  to  accept  complex  inputs;  that  is,  the  FFT’s  normal
input x(n) sequence is assumed to comprise real and imaginary parts, such as
(13-17)
In typical signal processing schemes, FFT input data sequences are usually real. The most common
example  of  this  is  the  FFT  input  samples  coming  from  an  A/D  converter  that  provides  real  integer
values of some continuous (analog) signal. In this case the FFT’s imaginary xi(n)’s inputs are all zero.
So  initial  FFT  computations  performed  on  the  xi(n)  inputs  represent  wasted  operations.  Early  FFT
pioneers  recognized  this  inefficiency,  studied  the  problem,  and  developed  a  technique  where  two
independent N-point, real  input  data  sequences  could  be  transformed  by  a  single  N-point  complex
FFT. We call this scheme the Two N-Point Real FFTs algorithm. The derivation of this technique is
straightforward and described in the literature[17–19]. If two N-point, real input sequences are a(n)
and b(n),  they’ll  have  discrete  Fourier  transforms  represented  by  Xa(m) and Xb(m).  If  we  treat  the
a(n) sequence as the real part of an FFT input and the b(n) sequence as the imaginary part of the FFT
input, then
(13-18)
Applying the x(n) values from Eq. (13-18) to the standard DFT,
(13-19)
we’ll get a DFT output X(m) where m goes from 0 to N−1. (We’re assuming, of course, that the DFT
is  implemented  by  way  of  an  FFT  algorithm.)  Using  the  superscript  “*”  symbol  to  represent  the
complex conjugate, we can extract the two desired FFT outputs Xa(m) and Xb(m) from X(m) by using
the following:
(13-20)
and
(13-21)
Let’s break Eqs. (13-20) and (13-21) into their real and imaginary parts to get expressions for Xa(m)
and Xb(m) that are easier to understand and implement. Using the notation showing X(m)’s real and
imaginary parts, where X(m) = Xr(m) + jXi(m), we can rewrite Eq. (13-20) as
(13-22)
where m = 1, 2, 3, . . ., N−1. What about the first Xa(m), when m = 0? Well, this is where we run into
a bind if we actually try to implement Eq. (13-20) directly. Letting m = 0 in Eq. (13-20), we quickly
realize that the first term in the numerator, X*(N−0) = X*(N), isn’t available because the X(N) sample
does not exist in the output of an N-point FFT! We resolve this problem by remembering that X(m) is
periodic with a period N, so X(N) = X(0).† When m = 0, Eq. (13-20) becomes
† This fact is illustrated in Section 3.8 during the discussion of spectral leakage in DFTs.
Next, simplifying Eq. (13-21),
(13-23)
(13-24)
where, again, m = 1, 2, 3, . . ., N−1. By the same argument used for Eq. (13-23), when m = 0, Xb(0) in
Eq. (13-24) becomes
(13-25)
This discussion brings up a good point for beginners to keep in mind. In the literature Eqs. (13-20)
and (13-21) are often presented without any discussion of the m = 0 problem. So, whenever you’re
grinding  through  an  algebraic  derivation  or  have  some  equations  tossed  out  at  you,  be  a  little
skeptical. Try the equations out on an example—see if they’re true. (After all, both authors and book
typesetters are human and sometimes make mistakes. We had an old saying in Ohio for this situation:
“Trust everybody, but cut the cards.”) Following this advice, let’s prove that this Two N-Point Real
FFTs  algorithm  really  does  work  by  applying  the  8-point  data  sequences  from  Chapter  3’s  DFT
examples to Eqs. (13-22) through (13-25). Taking the 8-point input data sequence from Section 3.1’s
DFT Example 1 and denoting it a(n),
(13-26)
Taking the 8-point input data sequence from Section 3.6’s DFT Example 2 and calling it b(n),
(13-27)
Combining the sequences in Eqs. (13-26) and (13-27) into a single complex sequence x(n),
(13-28)
Now, taking the 8-point FFT of the complex sequence in Eq. (13-28), we get
(13-29)
So from Eq. (13-23),
Xa(0) = Xr(0) = 0.
To get the rest of Xa(m), we have to plug the FFT output’s X(m) and X(N−m) values into Eq. (13-22).†
Doing so,
† Remember, when the FFT’s input is complex, the FFT outputs may not be conjugate symmetric; that is, we can’t assume that F(m) is
equal to F*(N−m) when the FFT input sequence’s real and imaginary parts are both nonzero.
So Eq. (13-22) really does extract Xa(m) from the X(m) sequence in Eq. (13-29). We can see that we
need not solve Eq. (13-22) when m is greater than 4 (or N/2) because Xa(m) will always be conjugate
symmetric.  Because  Xa(7)  =  Xa(1),  Xa(6)  =  Xa(2),  etc.,  only  the  first  N/2  elements  in  Xa(m)  are
independent and need be calculated.
OK, let’s keep going and use Eqs. (13-24) and (13-25) to extract Xb(m) from the FFT output. From
Eq. (13-25),
Plugging the FFT’s output values into Eq. (13-24) to get the next four Xb(m)s, we have
Xb(0) = Xi(0) = 0.
The question arises “With the additional processing required by Eqs. (13-22) and (13-24) after the
initial  FFT,  how  much  computational  saving  (or  loss)  is  to  be  had  by  this  Two  N-Point  Real  FFTs
algorithm?” We can estimate the efficiency of this algorithm by considering the number of arithmetic
operations required relative to two separate N-point radix-2 FFTs. First, we estimate the number of
arithmetic operations in two separate N-point complex FFTs.
From Section 4.6, we know that a standard radix-2 N-point complex FFT comprises (N/2)  ·  log2N
butterfly operations. If we use the optimized butterfly structure, each butterfly requires one complex
multiplication  and  two  complex  additions.  Now,  one  complex  multiplication  requires  two  real
additions and four real multiplications, and one complex addition requires two real additions.† So a
single FFT butterfly operation comprises four real multiplications and six real additions. This means
that a single N-point complex FFT requires (4N/2) · log2N real multiplications, and (6N/2) · log2N
real additions. Finally, we can say that two separate N-point complex radix-2 FFTs require
† The complex addition (a+jb) + (c+jd) = (a+c) + j(b+d) requires two real additions. A complex multiplication (a+jb) · (c+jd) = ac−bd
+ j(ad+bc) requires two real additions and four real multiplications.
(13-30)
(13-30′)
Next, we need to determine the computational workload of the Two N-Point Real FFTs algorithm. If
we add up the number of real multiplications and real additions required by the algorithm’s N-point
complex FFT, plus those required by Eq. (13-22) to get Xa(m), and those required by Eq. (13-24) to
get Xb(m), the Two N-Point Real FFTs algorithm requires
(13-31)
(13-31′)
Equations (13-31) and (13-31′) assume that we’re calculating only the first N/2 independent elements
of Xa(m) and Xb(m). The single N term in Eq. (13-31) accounts for the N/2 divide by 2 operations in
Eq. (13-22) and the N/2 divide by 2 operations in Eq. (13-24).
OK,  now  we  can  find  out  how  efficient  the  Two  N-Point  Real  FFTs  algorithm  is  compared  to  two
separate complex N-point radix-2 FFTs. This comparison, however, depends on the hardware used
for  the  calculations.  If  our  arithmetic  hardware  takes  many  more  clock  cycles  to  perform  a
multiplication than an addition, then the difference between multiplications in Eqs. (13-30) and (13-
31) is the most important comparison. In this case, the percentage gain in computational saving of the
Two N-Point Real FFTs algorithm relative to two separate N-point complex FFTs is the difference in
their  necessary  multiplications  over  the  number  of  multiplications  needed  for  two  separate  N-point
complex FFTs, or
(13-32)
The computational (multiplications only) saving from Eq. (13-32) is plotted as the top curve of Figure
13-11. In terms of multiplications, for N≥32, the Two N-Point Real FFTs algorithm saves us over 45
percent in computational workload compared to two separate N-point complex FFTs.
Figure 13-11 Computational saving of the Two N-Point Real FFTs algorithm over that of two
separate N-point complex FFTs. The top curve indicates the saving when only multiplications are
considered. The bottom curve is the saving when both additions and multiplications are used in the
comparison.
For  hardware  using  high-speed  multiplier  integrated  circuits,  multiplication  and  addition  can  take
roughly equivalent clock cycles. This makes addition operations just as important and time consuming
as multiplications. Thus the difference between those combined arithmetic operations in Eqs. (13-30)
plus  (13-30′)  and  Eqs.  (13-31)  plus  (13-31′)  is  the  appropriate  comparison.  In  this  case,  the
percentage  gain  in  computational  saving  of  our  algorithm  over  two  FFTs  is  their  total  arithmetic
operational difference over the total arithmetic operations in two separate N-point complex FFTs, or
(13-33)
The  full  computational  (multiplications  and  additions)  saving  from  Eq.  (13-33)  is  plotted  as  the
bottom curve of Figure 13-11. This concludes our discussion and illustration of how a single N-point
complex FFT can be used to transform two separate N-point real input data sequences.
13.5.2 Performing a 2N-Point Real FFT
Similar to the scheme above where two separate N-point real data sequences are transformed using a
single N-point  FFT,  a  technique  exists  where  a  2N-point  real  sequence  can  be  transformed  with  a
single complex N-point FFT. This 2N-Point Real FFT algorithm, whose derivation is also described
in the literature, requires that the 2N-sample real input sequence be separated into two parts[19,20]—
not broken in two, but unzipped—separating the even and odd sequence samples. The N even-indexed
input samples are loaded into the real part of a complex N-point input sequence x(n). Likewise, the
input’s N odd-indexed samples are loaded into x(n)’s imaginary parts. To illustrate this process, let’s
say we have a 2N-sample real input data sequence a(n) where 0 ≤ n ≤ 2N−1. We want a(n)’s 2N-
point  transform  Xa(m).  Loading  a(n)’s  odd/even  sequence  values  appropriately  into  an  N-point
complex FFT’s input sequence, x(n),
(13-34)
Applying the N complex values in Eq. (13-34) to an N-point complex FFT, we’ll get an FFT output
X(m)  =  Xr(m)  +  jXi(m),  where  m  goes  from  0  to  N−1.  To  extract  the  desired  2N-Point  Real  FFT
algorithm output Xa(m) = Xa,real(m) + jXa,imag(m) from X(m), let’s define the following relationships:
(13-35)
(13-36)
(13-37)
(13-38)
For the reasons presented following Eq. (13-22) in the last section, in the above expressions recall
that Xr(N) = Xr(0), and Xi(N) = Xi(0).  The  values  resulting  from  Eqs. (13-35) through (13-38)  are,
then, used as factors in the following expressions to obtain the real and imaginary parts of our final
Xa(m):
and
(13-39)
(13-40)
Remember, now, the original a(n) input index n  goes  from  0  to  2N−1,  and  our  N-point  FFT  output
index m goes from 0 to N−1. We apply 2N real input time-domain samples to this algorithm and get
back  N  complex  frequency-domain  samples  representing  the  first  half  of  the  equivalent  2N-point
complex FFT, Xa(0) through Xa(N−1). Because this algorithm’s a(n) input is constrained to be real,
Xa(N+1)  through  Xa(2N−1)  are  merely  the  complex  conjugates  of  their  Xa(1)  through  Xa(N−1)
counterparts and need not be calculated.
The above process does not compute the Xa(N) sample. The Xa(N) sample, which is real-only, is
(13-40′)
To help us keep all of this straight, Figure 13-12 depicts the computational steps of the 2N-Point Real
FFT algorithm.
Figure 13-12 Computational flow of the 2N-Point Real FFT algorithm.
To demonstrate this process by way of example, let’s apply the 8-point data sequence from Eq. (13-
26) to the 2N-Point Real FFT algorithm. Partitioning those Eq. (13-26), samples as dictated by Eq.
(13-34), we have our new FFT input sequence:
(13-41)
With N = 4 in this example, taking the 4-point FFT of the complex sequence in Eq. (13-41), we get
(13-42)
Using  these  values,  we  now  get  the  intermediate  factors  from  Eqs.  (13-35)  through  (13-38).
Calculating our first Xr
X(4) = X(0), and Xr
+(0) value, again we’re reminded that X(m) is periodic with a period N, so
+(0) = [Xr (0) + Xr (0)]/2 = 0. Continuing to use Eqs. (13-35) through (13-38),
(13-43)
Using the intermediate values from Eq. (13-43) in Eqs. (13-39) and (13-40),
(13-44)
Evaluating the sine and cosine terms in Eq. (13-44),
(13-45)
Combining the results of the terms in Eq. (13-45), we have our final correct answer of
(13-46)
After going through all the steps required by Eqs. (13-35) through (13-40), the reader might question
the efficiency of this 2N-Point Real FFT algorithm. Using the same process as the above Two N-Point
Real FFTs algorithm analysis, let’s show that the 2N-Point Real FFT algorithm does provide some
modest computational saving. First, we know that a single 2N-point radix-2 FFT has (2N/2) · log22N
= N · (log2N+1) butterflies and requires
and
(13-47)
(13-47′)
If we add up the number of real multiplications and real additions required by the algorithm’s N-point
complex FFT, plus those required by Eqs. (13-35) through (13-38) and those required by Eqs. (13-
39) and (13-40), the complete 2N-Point Real FFT algorithm requires
and
(13-48)
(13-48′)
OK, using the same hardware considerations (multiplications only) we used to arrive at Eq. (13-32),
the percentage gain in multiplication saving of the 2N-Point Real FFT algorithm relative to a 2N-point
complex FFT is
(13-49)
The computational (multiplications only) saving from Eq. (13-49) is plotted as the bottom curve of
Figure 13-13. In terms of multiplications, the 2N-Point Real FFT algorithm provides a saving of >30
percent when N ≥ 128 or whenever we transform input data sequences whose lengths are ≥256.
Figure 13-13 Computational saving of the 2N-Point Real FFT algorithm over that of a single 2N-
point complex FFT. The top curve is the saving when both additions and multiplications are used in
the comparison. The bottom curve indicates the saving when only multiplications are considered.
Again,  for  hardware  using  high-speed  multipliers,  we  consider  both  multiplication  and  addition
operations. The difference between those combined arithmetic operations in Eqs. (13-47) plus (13-
47′) and Eqs. (13-48) plus (13-48′) is the appropriate comparison. In this case, the percentage gain in
computational saving of our algorithm is
(13-50)
The  full  computational  (multiplications  and  additions)  saving  from  Eq.  (13-50)  is  plotted  as  a
function of N in the top curve of Figure 13-13.
13.6 Computing the Inverse FFT Using the Forward FFT
There  are  many  signal  processing  applications  where  the  capability  to  perform  the  inverse  FFT  is
necessary.  This  can  be  a  problem  if  available  hardware,  or  software  routines,  have  only  the
capability to perform the forward FFT. Fortunately, there are two slick ways to perform the inverse
FFT using the forward FFT algorithm.
13.6.1 Inverse FFT Method 1
The  first  inverse  FFT  calculation  scheme  is  implemented  following  the  processes  shown  in  Figure
13-14.
Figure 13-14 Processing for first inverse FFT calculation method.
To see how this works, consider the expressions for the forward and inverse DFTs. They are
(13-51)
(13-52)
To reiterate our goal, we want to use the process in Eq. (13-51) to implement Eq. (13-52). The first
step  of  our  approach  is  to  use  complex  conjugation.  Remember,  conjugation  (represented  by  the
superscript “*” symbol) is the reversal of the sign of a complex number’s imaginary exponent—if x =
ejø, then x* = e−jø. So, as a first step we take the complex conjugate of both sides of Eq. (13-52) to
give us
(13-53)
One  of  the  properties  of  complex  numbers,  discussed  in  Appendix  A,  is  that  the  conjugate  of  a
product is equal to the product of the conjugates. That is, if c = ab, then c* = (ab)* = a*b*. Using this,
we can show the conjugate of the right side of Eq. (13-53) to be
(13-54)
Hold  on;  we’re  almost  there.  Notice  the  similarity  of  Eq.  (13-54)  to  our  original  forward  DFT
expression, Eq. (13-51). If we perform a forward DFT on the conjugate of the X(m) in Eq. (13-54),
and  divide  the  results  by  N,  we  get  the  conjugate  of  our  desired  time  samples  x(n).  Taking  the
conjugate of both sides of Eq. (13-54), we get a more straightforward expression for x(n):
(13-55)
13.6.2 Inverse FFT Method 2
The  second  inverse  FFT  calculation  technique  is  implemented  following  the  interesting  data  flow
shown in Figure 13-15.
Figure 13-15 Processing for second inverse FFT calculation method.
In  this  clever  inverse  FFT  scheme  we  don’t  bother  with  conjugation.  Instead,  we  merely  swap  the
real and imaginary parts of sequences of complex data[21]. To see why this process works, let’s look
at the inverse DFT equation again while separating the input X(m)  term  into  its  real  and  imaginary
parts and remembering that ejø = cos(ø) + jsin(ø).
(13-56)
Multiplying the complex terms in Eq. (13-56) gives us
(13-57)
Equation (13-57) is the general expression for the inverse DFT, and we’ll now quickly show that the
process in Figure 13-15 implements this equation. With X(m) = Xreal(m) + jXimag(m), then swapping
these terms gives us
The forward DFT of our Xswap(m) is
(13-58)
(13-59)
Multiplying the complex terms in Eq. (13-59) gives us
(13-60)
Swapping the real and imaginary parts of the results of this forward DFT gives us what we’re after:
(13-61)
If we divided Eq. (13-61) by N, it would be exactly equal to the inverse DFT expression in Eq. (13-
57), and that’s what we set out to show.
13.7 Simplified FIR Filter Structure
If  we  implement  a  linear-phase  FIR  digital  filter  using  the  standard  structure  in  Figure  13-16(a),
there’s  a  way  to  reduce  the  number  of  multipliers  when  the  filter  has  an  odd  number  of  taps.  Let’s
look at the top of Figure 13-16(a) where the 5-tap FIR filter coefficients are h(0) through h(4) and the
y(n) output is
(13-62)
Figure 13-16 Conventional and simplified structures of an FIR filter: (a) with an odd number of taps;
(b) with an even number of taps.
If the FIR filter’s coefficients are symmetrical, we can reduce the number of necessary multipliers.
That is, if h(4) = h(0), and h(3) = h(1), we can implement Eq. (13-62) by
(13-63)
where only three multiplications are necessary as shown at the bottom of Figure 13-16(a). In our 5-
tap  filter  case,  we’ve  eliminated  two  multipliers.  This  minimum-multiplier  structure  is  called  a
folded FIR filter.
So in the case of an odd number of taps, we need only perform (S−1)/2 + 1 multiplications for each
filter output sample. For an even number of symmetrical taps as shown in Figure 13-16(b), the saving
afforded by this technique reduces the necessary number of multiplications to S/2. Some commercial
programmable  DSP  chips  have  specialized  instructions,  and  dual  multiply-and-accumulate  (MAC)
units, that take advantage of the folded FIR filter implementation.
13.8 Reducing A/D Converter Quantization Noise
In Section 12.3 we discussed the mathematical details, and ill effects, of quantization noise in analog-
to-digital  (A/D)  converters.  DSP  practitioners  commonly  use  two  tricks  to  reduce  converter
quantization noise. Those schemes are called oversampling and dithering.
13.8.1 Oversampling
The  process  of  oversampling  to  reduce  A/D  converter  quantization  noise  is  straightforward.  We
merely sample an analog signal at an fs sample rate higher than the minimum rate needed to satisfy the
Nyquist  criterion  (twice  the  analog  signal’s  bandwidth),  and  then  lowpass  filter.  What  could  be
simpler?  The  theory  behind  oversampling  is  based  on  the  assumption  that  an  A/D  converter’s  total
quantization noise power (variance) is the converter’s least significant bit (lsb) value squared over
12, or
(13-64)
We derived that expression in Section 12.3. The next assumptions are: The quantization noise values
are  truly  random,  and  in  the  frequency  domain  the  quantization  noise  has  a  flat  spectrum.  (These
assumptions are valid if the A/D converter is being driven by an analog signal that covers most of the
converter’s  analog  input  voltage  range  and  is  not  highly  periodic.)  Next  we  consider  the  notion  of
quantization noise power spectral density (PSD), a frequency-domain characterization of quantization
noise measured in noise power per hertz as shown in Figure 13-17. Thus we can consider the idea
that quantization noise can be represented as a certain amount of power (watts, if we wish) per unit
bandwidth.
Figure 13-17 Frequency-domain power spectral density of an ideal A/D converter.
In our world of discrete systems, the flat noise spectrum assumption results in the total quantization
noise (a fixed value based on the converter’s lsb voltage) being distributed equally in the frequency
domain,  from  −fs/2  to  +fs/2  as  indicated  in  Figure 13-17.  The  amplitude  of  this  quantization  noise
PSD is the rectangle area (total quantization noise power) divided by the rectangle width (fs), or
(13-65)
measured in watts/Hz.
The  next  question  is:  “How  can  we  reduce  the  PSDnoise  level  defined  by  Eq. (13-65)?”  We  could
reduce  the  lsb  value  (volts)  in  the  numerator  by  using  an  A/D  converter  with  additional  bits.  That
would  make  the  lsb  value  smaller  and  certainly  reduce  PSDnoise,  but  that’s  an  expensive  solution.
Extra  converter  bits  cost  money.  Better  yet,  let’s  increase  the  denominator  of  Eq.  (13-65)  by
increasing the sample rate fs.
Consider a low-level discrete signal of interest whose spectrum is depicted in Figure 13-18(a). By
increasing the fs,old sample rate to some larger value fs,new (oversampling), we spread the total noise
power (a fixed value) over a wider frequency range as shown in Figure 13-18(b). The areas under the
shaded curves in Figures 13-18(a) and 13-18(b)  are  equal.  Next  we  lowpass  filter  the  converter’s
output samples. At the output of the filter, the quantization noise level contaminating our signal will be
reduced from that at the input of the filter.
Figure 13-18 Oversampling example: (a) noise PSD at an fs,old samples rate; (b) noise PSD at the
higher fs,new samples rate; (c) processing steps.
The improvement in signal-to-quantization-noise ratio, measured in dB, achieved by oversampling is
(13-66)
For example, if fs,old = 100 kHz, and fs,new = 400 kHz, the SNRA/D-gain = 10log10(4) = 6.02 dB. Thus
oversampling  by  a  factor  of  four  (and  filtering),  we  gain  a  single  bit’s  worth  of  quantization  noise
reduction. Consequently we can achieve N+1-bit performance from an N-bit A/D converter, because
we gain signal amplitude resolution at the expense of higher sampling speed. After digital filtering,
we can decimate to the lower fs,old without degrading the improved SNR. Of course, the number of
bits used for the lowpass filter’s coefficients and registers must exceed the original number of A/D
converter bits, or this oversampling scheme doesn’t work.
With the use of a digital lowpass filter, depending on the interfering analog noise in x(t), it’s possible
to use a lower-performance (simpler) analog anti-aliasing filter relative to the analog filter necessary
at the lower sampling rate.
13.8.2 Dithering
Dithering, another technique used to minimize the effects of A/D quantization noise, is the process of
adding noise to our analog signal prior to A/D conversion. This scheme, which doesn’t seem at all
like a good idea, can indeed be useful and is easily illustrated with an example. Consider digitizing
the low-level analog sinusoid shown in Figure 13-19(a),  whose  peak  voltage  just  exceeds  a  single
A/D converter least significant bit (lsb) voltage level, yielding the converter output x1(n) samples in
Figure 13-19(b). The x1(n) output sequence is clipped. This generates all sorts of spectral harmonics.
Another way to explain the spectral harmonics is to recognize the periodicity of the quantization noise
in Figure 13-19(c).
Figure 13-19 Dithering: (a) a low-level analog signal; (b) the A/D converter output sequence; (c) the
quantization error in the converter’s output.
We show the spectrum of x1(n) in Figure 13-20(a) where the spurious quantization noise harmonics
are apparent. It’s worthwhile to note that averaging multiple spectra will not enable us to pull some
spectral  component  of  interest  up  above  those  spurious  harmonics  in  Figure 13-20(a).  Because  the
quantization noise is highly correlated with our input sinewave—the quantization noise has the same
time  period  as  the  input  sinewave—spectral  averaging  will  also  raise  the  noise  harmonic  levels.
Dithering to the rescue.
Figure 13-20 Spectra of a low-level discrete sinusoid: (a) with no dithering; (b) with dithering.
Dithering is the technique where random analog noise is added to the analog input sinusoid before it
is  digitized.  This  technique  results  in  a  noisy  analog  signal  that  crosses  additional  converter  lsb
boundaries  and  yields  a  quantization  noise  that’s  much  more  random,  with  a  reduced  level  of
undesirable  spectral  harmonics  as  shown  in  Figure 13-20(b).  Dithering  raises  the  average  spectral
noise  floor  but  increases  our  signal-to-noise  ratio  SNR2.  Dithering  forces  the  quantization  noise  to
lose  its  coherence  with  the  original  input  signal,  and  we  could  then  perform  signal  averaging  if
desired.
Dithering is indeed useful when we’re digitizing
• low-amplitude analog signals,
• highly periodic analog signals (like a sinewave with an even number of cycles in the sample time
interval), and
• slowly varying (very low frequency, including DC) analog signals.
The standard implementation of dithering is shown in Figure 13-21(a). The typical amount of random
wideband analog noise used in this process, provided by a noise diode or noise generator ICs, has an
rms  (root  mean  squared)  level  equivalent  to  1/3  to  1  lsb  voltage  level.  The  system-level  effect  of
adding the analog dithering signal is to linearize the undithered stair-step transfer function of an A/D
converter as shown in Figure 13-21(c).
Figure 13-21 Dithering implementations: (a) standard dithering process; (b) advanced dithering with
noise subtraction; (c) improved transfer function due to dithering.
For  high-performance  audio  applications,  engineers  have  found  that  adding  dither  noise  from  two
separate noise generators improves background audio low-level noise suppression. The probability
density function (PDF) of the sum of two noise sources (having rectangular PDFs) is the convolution
of their individual PDFs. Because the convolution of two rectangular functions is triangular, this dual-
noise-source  dithering  scheme  is  called  triangular  dither.  Typical  triangular  dither  noise  has  rms
levels equivalent to, roughly, 2 lsb voltage levels.
In the situation where our signal of interest occupies some well-defined portion of the full frequency
band, injecting narrowband dither noise having an rms level equivalent to 4 to 6 lsb voltage levels,
whose  spectral  energy  is  outside  that  signal  band,  would  be  advantageous.  (Remember,  though:  the
dither  signal  can’t  be  too  narrowband,  like  a  sinewave.  Quantization  noise  from  a  sinewave  signal
would  generate  more  spurious  harmonics!)  That  narrowband  dither  noise  can  then  be  removed  by
follow-on digital filtering.
One last note about dithering: To improve our ability to detect low-level signals, we could add the
analog dither noise and then subtract that noise from the digitized data, as shown in Figure 13-21(b).
This way, we randomize the quantization noise but reduce the amount of total noise power injected in
the analog signal. This scheme is used in commercial analog test equipment[22,23].
13.9 A/D Converter Testing Techniques
We  can  take  advantage  of  digital  signal  processing  techniques  to  facilitate  the  testing  of  A/D
converters.  In  this  section  we  present  two  schemes  for  measuring  converter  performance:  first,  a
technique using the FFT to estimate overall converter noise, and second, a histogram analysis scheme
to detect missing converter output codes.
13.9.1 Estimating A/D Quantization Noise with the FFT
The  combination  of  A/D  converter  quantization  noise,  missing  bits,  harmonic  distortion,  and  other
nonlinearities  can  be  characterized  by  analyzing  the  spectral  content  of  the  converter’s  output.
Converter  performance  degradation  caused  by  these  nonlinearities  is  not  difficult  to  recognize
because they show up as spurious spectral components and increased background noise levels in the
A/D  converter’s  output  samples.  The  traditional  test  method  involves  applying  a  sinusoidal  analog
voltage  to  an  A/D  converter’s  input  and  examining  the  spectrum  of  the  converter’s  digitized  time-
domain output samples. We can use the FFT to compute the spectrum of an A/D converter’s output
samples,  but  we  have  to  minimize  FFT  spectral  leakage  to  improve  the  sensitivity  of  our  spectral
measurements.  Traditional  time-domain  windowing,  however,  often  provides  insufficient  FFT
leakage reduction for high-performance A/D converter testing.
The trick to circumvent this FFT leakage problem is to use a sinusoidal analog input voltage whose
frequency is a rational factor of the A/D converter’s clock frequency as shown in Figure 13-22(a).
That frequency is mfs/N where m is an integer, fs is the clock frequency (sample rate), and N is the
FFT size. Figure 13-22(a) shows the x(n) time-domain output of an ideal 5-bit A/D converter when
its analog input is a sinewave having exactly m = 4 cycles over N = 64 converter output samples. In
this case, the analog input frequency is 4fs/64 Hz. Recall from Chapter 3  that  the  expression  mfs/N
defined  the  analysis  frequencies,  or  bin  centers,  of  the  DFT,  and  a  DFT  input  sinusoid  whose
frequency is at a bin center causes no spectral leakage.
Figure 13-22 A/D converter (5-bit) output with an analog 4fs/64 Hz sinewave input: (a) m = 4-cycle
sinusoidal time samples; (b) spectral magnitude in dB.
The magnitudes of the first half of an N = 64-point FFT of x(n) are shown in the logarithmic plot in
Figure 13-22(b) where the analog input spectral component lies exactly at the m = 4 bin center. (The
additional  nonzero  spectral  samples  are  not  due  to  FFT  leakage;  they  represent  A/D  converter
quantization  noise.)  Specifically,  if  the  sample  rate  were  1  MHz,  then  the  A/D’s  input  analog
sinewave’s  frequency  is  4(106/64)  =  62.5  kHz.  In  order  to  implement  this  A/D  testing  scheme  we
must  ensure  that  the  analog  test-signal  generator  is  synchronized,  exactly,  with  the  A/D  converter’s
clock frequency of fs Hz. Achieving this synchronization is why this A/D converter testing procedure
is referred to as coherent sampling[24–26]. That is, the analog signal generator and the A/D clock
generator providing fs must not drift in frequency relative to each other—they must remain coherent.
(Here  we  must  take  care  from  a  semantic  viewpoint  because  the  quadrature  sampling  schemes
described in Chapter 8 are also sometimes called coherent sampling, and they are unrelated to this
A/D converter testing procedure.)
As it turns out, some values of m are more advantageous than others. Notice in Figure 13-22(a), that
when m = 4, only ten different binary output values, output codes, are output by the A/D converter.
Those  values  are  repeated  over  and  over,  and  the  quantization  noise  is  far  from  being  random.  As
shown in Figure 13-23(a), when m = 5, we exercise more than ten different A/D output codes, and the
quantization noise in Figure 13-23(b) is much more random than when m = 4.
Figure 13-23 A/D converter (5-bit) output with an analog 5fs/64 Hz sinewave input: (a) m = 5-cycle
time samples; (b) spectral magnitude in dB; (c) FFT results interpretation.
Because it’s best to test as many A/D output codes as possible, while keeping the quantization noise
sufficiently  random,  users  of  this  A/D  testing  scheme  have  discovered  another  trick;  they  found
making m an odd prime number (3, 5, 7, 11, etc.) minimizes the number of redundant A/D output code
values and makes the quantization noise more random, which is what we want. The larger m is, the
more  codes  that  are  exercised.  (We  can  use  histogram  testing,  discussed  in  the  next  section,  to
determine how many of a b-bit A/D converter’s 2b possible output codes have been exercised.)
While examining the quantization noise level in Figure 13-23(b), we might be tempted to say the A/D
converter  has  a  signal-to-quantization-noise  ratio  of  40  to  50  dB.  As  it  turns  out,  the  true  A/D
converter  noise  levels  will  be  higher  than  those  indicated  by  Figure  13-23(b).  That’s  because  the
inherent  processing  gain  of  the  FFT  (discussed  in  Section  3.12.1)  will  pull  the  high-level  m  =  5
signal  spectral  component  up  out  of  the  background  converter  noise,  making  that  m  =  5  spectral
magnitude  sample  appear  higher  above  the  background  noise  than  is  correct.  Consequently,  when
viewing  Figure  13-23(b),  we  must  keep  in  mind  an  N  =  64-point  FFT’s  processing  gain  of
10log10(64/2). Our interpretation of A/D performance based on the FFT magnitude results is given in
Figure 13-23(c).
There  is  a  technique  used  to  characterize  an  A/D  converter’s  true  signal-to-noise  ratio  (including
quantization  noise,  harmonic  distortion,  and  other  nonlinearities).  That  testing  technique  measures
what is commonly called an A/D converter’s SINAD—for signal-to-noise-and-distortion—and does
not  require  us  to  consider  FFT  processing  gain.  The  SINAD  value  for  an  A/D  converter,  based  on
spectral power samples, is
(13-66′)
The  SINAD  value  for  an  A/D  converter  is  a  good  quantitative  indicator  of  a  converter’s  overall
dynamic performance. The steps to compute SINAD are:
1. Compute an N-point FFT of an A/D converter’s output sequence. Discard the negative-frequency
samples of the FFT results.
2. Over the positive-frequency range of the FFT results, compute the total signal spectral power by
summing the squares of all signal-only spectral magnitude samples. For our Figure 13-23 example
that’s simply squaring the FFT’s |X(5)| magnitude value. (We square the linear |X(5)| value and not
the value of |X(5)| in dB!)
3. Over the positive-frequency range of the FFT results, sum the squares of all noise-only spectral
magnitude samples, including any signal harmonics, but excluding the zero-Hz X(0) sample. This
summation result represents total noise power, which includes harmonic distortion.
4. Perform the computation given in Eq. (13-66′).
Performing those steps on the spectrum in Figure 13-23(b) yields a SINAD value of 31.6 dB. This
result  is  reasonable  for  our  simulated  5-bit  A/D  converter  because  its  signal-to-quantization-noise
ratio would ideally be 6·5 + 1.7 = 31.7 dB.
Figure 13-24(a)  illustrates  an  extreme  example  of  nonlinear  A/D  converter  operation  with  several
binary output codes (words) having dropped bits in the time-domain x(n) sequence with m = 5. The
FFT  magnitudes,  provided  in  Figure  13-24(b),  indicate  severe  A/D  converter  nonlinear  distortion
because we can see the increased background noise level compared to Figure 13-23(b). Performing
Eq. (13-66′) for this noisy A/D gives us a measured SINAD value of 15.2 dB, which is drastically
smaller than the ideal 5-bit A/D converter’s SINAD = 31.6 dB. The point here is that we can quickly
measure an A/D converter’s performance using FFTs and Eq. (13-66′).
Figure 13-24 Nonideal A/D converter output showing several dropped bits: (a) time samples; (b)
spectral magnitude in dB.
To fully characterize the dynamic performance of an A/D converter we’d need to perform this SINAD
testing technique at many different input frequencies and amplitudes. (The analog sinewave applied to
an A/D converter must, of course, be as pure as possible. Any distortion inherent in the analog signal
will show up in the final FFT output and could be mistaken for A/D nonlinearity.) The key issue here
is that when any input frequency is mfs/N, where m is less than N/2 to satisfy the Nyquist sampling
criterion,  we  can  take  full  advantage  of  the  FFT’s  processing  capability  while  minimizing  spectral
leakage.
For  completeness,  we  mention  that  what  we  called  SINAD  in  Eq.  (13-66′)  is  sometimes  called
SNDR. In addition, there is a measurement scheme called SINAD used by RF engineers to quantify
the sensitivity of radio receivers. That receiver SINAD concept is quite different from our Eq. (13-
66′) A/D converter SINAD estimation process and will not be discussed here.
13.9.2 Estimating A/D Dynamic Range
In  this  section  we  describe  a  technique  of  applying  the  sum  of  two  analog  sinewaves  to  an  A/D
converter’s input to quantify the intermodulation distortion performance of a converter, which in turn
measures the converter’s dynamic range. That dynamic range is called the converter’s spurious free
dynamic  range  (SFDR).  In  this  testing  scheme  both  input  sinewaves  must  comply  with  the  mfs/N
restriction. Figure 13-25(a) shows the test configuration.
Figure 13-25 A/D converter SFDR testing: (a) hardware test configuration; (b) example test results.
The  SFDR  test  starts  by  applying  the  sum  of  two  equal-amplitude  analog  sinewaves  to  an  A/D
converter  and  monitoring  the  spectrum  of  the  converter’s  output  samples.  Next  we  increase  both
analog  sinewaves’  amplitudes  until  we  see  a  spurious  spectral  component  rising  above  the
converter’s  background  spectral  noise  as  shown  in  Figure  13-25(b).  Finally  we  measure  the
converter’s  SFDR  as  the  dB  difference  between  a  high-level  signal  spectral  magnitude  sample  and
the spurious signal’s spectral magnitude.
For this SFDR testing it’s prudent to use bandpass filters (BPFs) to improve the spectral purity of the
sinewave  generators’  outputs,  and  small-valued  fixed  attenuators  (pads)  are  used  to  keep  the
generators from adversely interacting with each other. (I recommend 3 dB fixed attenuators for this.)
The  power  combiner  is  typically  an  analog  power  splitter  driven  backward,  and  the  A/D  clock
generator  output  is  a  squarewave.  The  dashed  lines  in  Figure  13-25(a)  indicate  that  all  three
generators are synchronized to the same reference frequency source.
13.9.3 Detecting Missing Codes
One problem that can plague A/D converters is missing codes. This defect occurs when a converter
is incapable of outputting a specific binary word (a code). Think about driving an eight-bit converter
with an analog sinusoid and the effect when its output should be the binary word 00100001 (decimal
33); its output is actually the word 00100000 (decimal 32) as shown in Figure 13-26(a). The binary
word representing decimal 33 is a missing code. This subtle nonlinearity is very difficult to detect by
examining  time-domain  samples  or  performing  spectrum  analysis.  Fortunately  there  is  a  simple,
reliable way to detect the missing 33 using histogram analysis.
Figure 13-26 Eight-bit converter missing codes: (a) missing code of binary 00100001, decimal 33;
(b) histogram plot.
The histogram testing technique merely involves collecting many A/D converter output samples and
plotting the number of occurrences of each sample value versus that sample value as shown in Figure
13-26(b). Any missing code (like our missing 33) would show up in the histogram as a zero value.
That is, there were zero occurrences of the binary code representing a decimal 33.
Additional  useful  information  can  be  obtained  from  our  histogram  results.  That  is,  counting  the
number  of  nonzero  samples  in  Figure  13-26(b)  tells  us  how  many  actual  different  A/D  converter
output codes (out of a possible 2b codes) have been exercised.
In practice, the input analog sinewave must have an amplitude that’s somewhat greater than the analog
signal  that  we  intend  to  digitize  in  an  actual  application,  and  a  frequency  that  is  unrelated  to
(incoherent  with)  the  fs  sampling  rate.  In  an  effort  to  exercise  (test)  all  of  the  converter’s  output
codes, we digitize as many cycles of the input sinewave as possible for our histogram test.
13.10 Fast FIR Filtering Using the FFT
In the late 1960s, while contemplating the notion of time-domain convolution, DSP pioneer Thomas
Stockham  (digital  audio  expert  and  inventor  of  the  compact  disc)  realized  that  time-domain
convolution could sometimes be performed much more efficiently using fast Fourier transform (FFT)
algorithms  rather  than  using  the  direct  convolution  implemented  with  tapped-delay  line  FIR  filters.
The principle behind this FFT-based convolution scheme, called fast convolution (also called block
convolution or FFT convolution), is diagrammed in Figure 13-27(a). In that figure x(n) is an input
signal sequence and h(k) is the Q-length impulse response (coefficients) of a tapped-delay line FIR
filter. Figure 13-27(a) is a graphical depiction of one form of the convolution theorem: Multiplication
in the frequency domain is equivalent to convolution in the time domain.
Figure 13-27 Fast convolution: (a) basic process; (b) computational workloads for various FIR filter
tap lengths Q.
The  standard  convolution  equation,  for  a  Q-tap  FIR  filter,  given  in  Eq.  (5-6)  is  repeated  here  for
reference as
(13-67)
where the symbol “*” means convolution. When the filter’s h(k) impulse response has a length greater
than 40 to 80 (depending on the hardware and software being used), the process in Figure 13-27(a)
requires  fewer  computations  than  directly  implementing  the  convolution  expression  in  Eq.  (13-67).
Consequently,  this  fast  convolution  technique  is  a  computationally  efficient  signal  processing  tool,
particularly  when  used  for  digital  filtering.  Fast  convolution’s  gain  in  computational  efficiency
becomes quite significant when the lengths of h(k) and x(n) are large.
Figure 13-27(b)  indicates  the  reduction  in  the  fast  convolution  algorithm’s  computational  workload
relative to the standard (tapped-delay line) time-domain convolution method, Eq. (13-67), versus the
length of the x(n) sequence for various filter impulse response lengths Q. (Please do not view Figure
13-27(b)  as  any  sort  of  gospel  truth.  That  figure  is  merely  an  indicator  of  fast  convolution’s
computational efficiency.)
The necessary forward and inverse FFT sizes, N, in Figure 13-27(a) must of course be equal and are
dependent upon the length of the original h(k) and x(n) sequences. Recall from Eq. (5-29) that if h(k)
is of length Q and x(n) is of length P, the length of the final y(n) sequence will be L where
(13-67′)
For this fast convolution technique to yield valid results, the forward and inverse FFT sizes must be
equal  to  or  greater  than  L.  So,  to  implement  fast  convolution  we  must  choose  an  N-point  FFT  size
such that N ≥ L, and zero-pad h(k) and x(n) so they have new lengths equal to N. The desired y(n)
output is the real part of the first L samples of the inverse FFT. Note that the H(m) sequence, the FFT
of the FIR filter’s h(k) impulse response, need only be computed once and stored in memory.
Now if the x(n) input sequence length P is so large that FFT processing becomes impractical, or your
hardware memory buffer can only hold small segments of the x(n) time samples, then x(n)  must  be
partitioned  into  multiple  blocks  of  samples  and  each  sample  block  processed  individually.  If  the
partitioned-x(n)  block  lengths  are  N,  a  straightforward  implementation  of  Figure 13-27(a)  leads  to
time-domain aliasing errors in y(n) due to the circular nature (spectral wraparound) of the discrete
Fourier  transform  (and  the  FFT).  Two  techniques  are  used  to  avoid  that  time-domain  aliasing
problem, the overlap-and-save method and the overlap-and-add method. Of these two methods, let’s
first  have  a  look  at  the  overlap-and-save  fast  convolution  filtering  technique  shown  in  Figure  13-
28(a).
Figure 13-28 Fast convolution block processing (continues).
Given that the desired FIR filter’s h(k) impulse response length is Q and the x(n) filter input sequence
is of length P, the steps to perform overlap-and-save fast convolution filtering are as follows:
1. Choose an FFT size of N, where N is an integer power of two equal to roughly four times Q.
2. Append (N−Q) zero-valued samples to the end of the h(k) impulse response and perform an N-
point FFT on the extended sequence, producing the complex H(m) sequence.
3. Compute integer M using M = N−(Q−1).
4. Insert (Q−1) zero-valued samples prior to the first M samples of x(n), creating the first N-point
FFT input sequence x1(n).
5. Perform an N-point FFT on x1(n), multiply that FFT result by the H(m) sequence, and perform an
N-point inverse FFT on the product. Discard the first (Q−1) samples of the inverse FFT results to
generate the first M-point output block of data y1(n).
6.  Attach  the  last  (Q−1)  samples  of  x1(n)  to  the  beginning  of  the  second  M-length  block  of  the
original x(n) sequence, creating the second N-point FFT input sequence x2(n) as shown in Figure
13-28(a).
7. Perform an N-point FFT on x2(n), multiply that FFT result by the H(m) sequence, and perform an
N-point inverse FFT on the product. Discard the first (Q−1) samples of the inverse FFT results to
generate the second M-point output block of data y2(n).
8. Repeat Steps 6 and 7 until we have gone through the entire original x(n) filter input sequence.
Depending on the length P of the original x(n) input sequence and the chosen value for N, we must
append  anywhere  from  Q−1  to  N−1  zero-valued  samples  to  the  end  of  the  original  x(n)  input
samples in order to accommodate the final block of forward and inverse FFT processing.
9.  Concatenate  the  y1(n),  y2(n),  y3(n),  .  .  .  sequences  shown  in  Figure  13-28(a),  discarding  any
unnecessary  trailing  zero-valued  samples,  to  generate  your  final  linear-convolution  filter  output
y(n) sequence.
10. Finally, experiment with different values of N to see if there exists an optimum N that minimizes
the computational workload for your hardware and software implementation. In any case, N must
not  be  less  than  (M+Q−1).  (Smaller  N  means  many  small-sized  FFTs  are  needed,  and  large  N
means fewer, but larger-sized, FFTs are necessary. Pick your poison.)
The second fast convolution method, the overlap-and-add technique, is shown in Figure 13-28(b). In
this method, the x(n) input sequence is partitioned (segmented) into data blocks of length M, and our
data overlapping takes place in the inverse FFT time-domain sequences. Given that the desired FIR
filter’s h(k) impulse response length is Q and the x(n) filter input sequence is of length P, the steps to
perform overlap-and-add fast convolution filtering are as follows:
1. Choose an FFT size of N, where N is an integer power of two equal to roughly two times Q.
2. Append (N−Q) zero-valued samples to the end of the h(k) impulse response and perform an N-
point FFT on the extended sequence, producing the complex H(m) sequence.
3. Compute integer M using M = N−(Q−1).
4. Append (Q−1) zero-valued samples to the end of the first M samples, x1(n), of the original x(n)
sequence, creating the first N-point FFT input sequence.
5. Perform an N-point FFT on the first N-point FFT input sequence, multiply that FFT result by the
H(m) sequence, and perform an N-point inverse FFT on the product. Retain the first M samples of
the inverse FFT sequence, generating the first M-point output block of data y1(n).
6. Append (Q−1) zero-valued samples to the end of the second M samples, x2(n),  of  the  original
x(n) sequence, creating the second N-point FFT input sequence.
7. Perform an N-point FFT on the second N-point FFT input sequence, multiply that FFT result by
the  H(m)  sequence,  and  perform  an  N-point  inverse  FFT  on  the  product.  Add  the  last  (Q−1)
samples  from  the  previous  inverse  FFT  to  the  first  (Q−1)  samples  of  the  current  inverse  FFT
sequence. Retain the first M samples of the sequence resulting from the (Q−1)-element  addition
process, generating the second M-point output block of data y2(n).
8. Repeat Steps 6 and 7 until we have gone through the entire original x(n) filter input sequence.
Depending on the length P of the original x(n) input sequence and the chosen value for N, we must
append  anywhere  from  Q−1  to  N−1  zero-valued  samples  to  the  end  of  the  original  x(n)  input
samples in order to accommodate the final block of forward and inverse FFT processing.
9.  Concatenate  the  y1(n),  y2(n),  y3(n),  .  .  .  sequences  shown  in  Figure  13-28(b),  discarding  any
unnecessary  trailing  zero-valued  samples,  to  generate  your  final  linear-convolution  filter  output
y(n) sequence.
10. Finally, experiment with different values of N to see if there exists an optimum N that minimizes
the computational workload for your hardware and software implementation. N must not be less
than (M+Q−1). (Again, smaller N means many small-sized FFTs are needed, and large N means
fewer, but larger-sized, FFTs are necessary.)
It’s useful to realize that the computational workload of these fast convolution filtering schemes does
not change as Q increases in length up to a value of N. Another interesting aspect of fast convolution,
from a hardware standpoint, is that the FFT indexing bit-reversal problem discussed in Sections 4.5
and 4.6 is not an issue here. If the FFTs result in X(m) and H(m) having bit-reversed output sample
indices,  the  multiplication  can  still  be  performed  directly  on  the  scrambled  H(m)  and  X(m)
sequences.  Then  an  appropriate  inverse  FFT  structure  can  be  used  that  expects  bit-reversed  input
data.  That  inverse  FFT  then  provides  an  output  sequence  whose  time-domain  indexing  is  in  the
correct order. Neat!
By the way, it’s worth knowing that there are no restrictions on the filter’s finite-length h(k) impulse
response—h(k)  is  not  limited  to  being  real-valued  and  symmetrical  as  is  traditional  with  tapped-
delay  line  FIR  filters.  Sequence  h(k)  can  be  complex-valued,  asymmetrical  (to  achieve  nonlinear-
phase filtering), or whatever you choose.
One last issue to bear in mind: the complex amplitudes of the standard radix-2 FFT’s output samples
are proportional to the FFT sizes, N, so the product of two FFT outputs will have a gain proportional
to N2. The inverse FFT has a normalizing gain reduction of only 1/N. As such, our fast convolution
filtering  methods  will  have  an  overall  gain  that  is  not  unity.  We  suggest  that  practitioners  give  this
gain normalization topic some thought during the design of their fast convolution system.
To summarize this frequency-domain filtering discussion, the two fast convolution filtering schemes
can be computationally efficient, compared to standard tapped-delay line FIR convolution filtering,
particularly when the x(n) input sequence is large and high-performance filtering is needed (requiring
many  filter  taps,  i.e.,  Q  =  40  to  80).  As  for  which  method,  overlap-and-save  or  overlap-and-add,
should be used in any given situation, there is no simple answer. Choosing a fast convolution method
depends  on  many  factors:  the  fixed/floating-point  arithmetic  used,  memory  size  and  access  latency,
computational hardware architecture, and specialized built-in filtering instructions, etc.
13.11 Generating Normally Distributed Random Data
Section D.7 in Appendix D  discusses  the  normal  distribution  curve  as  it  relates  to  random  data.  A
problem  we  may  encounter  is  how  actually  to  generate  random  data  samples  whose  distribution
follows that normal (Gaussian) curve. There’s a straightforward way to solve this problem using any
software package that can generate uniformly distributed random data, as most of them do[27]. Figure
13-29 shows our situation pictorially where we require random data that’s distributed normally with
a  mean  (average)  of  μ′  and  a  standard  deviation  of  σ′,  as  in  Figure  13-29(a),  and  all  we  have
available is a software routine that generates random data that’s uniformly distributed between zero
and one as in Figure 13-29(b).
Figure 13-29 Probability distribution functions: (a) normal distribution with mean = μ′ and standard
deviation σ′; (b) uniform distribution between zero and one.
As  it  turns  out,  there’s  a  principle  in  advanced  probability  theory,  known  as  the  Central  Limit
Theorem, that says when random data from an arbitrary distribution is summed over M samples, the
probability distribution of the sum begins to approach a normal distribution as M increases[28–30].
In other words, if we generate a set of N random samples that are uniformly distributed between zero
and  one,  we  can  begin  adding  other  sets  of  N  samples  to  the  first  set.  As  we  continue  summing
additional sets, the distribution of the N-element set of sums becomes more and more normal. We can
sound impressive and state that “the sum becomes asymptotically normal.” Experience has shown that
for practical purposes, if we sum M ≥ 30 times, the summed data distribution is essentially normal.
With this rule in mind, we’re halfway to solving our problem.
After summing M sets of uniformly distributed samples, the summed set ysum will have a distribution
as shown in Figure 13-30.
Figure 13-30 Probability distribution of the summed set of random data derived from uniformly
distributed data.
Because we’ve summed M data sets whose mean values were all 0.5, the mean of ysum is the sum of
those M means, or μ = M/2. From Section D.6 of Appendix D we know the variance of a single data
sample set, having the probability distribution in Figure 13-29(b), is 1/12. Because the variance of
the sum of M data sets is equal to the sum of their individual variances, we can say
and
(13-68)
(13-69)
So, here’s the trick: To convert the ysum  data  set  to  our  desired  data  set  having  a  mean  of  μ′  and  a
standard deviation of σ′, we
1. subtract M/2 from each element of ysum to shift its mean to zero;
2.  scale  ysum  so  that  its  standard  deviation  is  the  desired  σ′,  by  multiplying  each  sample  in  the
shifted data set by σ′/σ; and
3. finally, center the new data set at the desired μ′ value by adding μ′ to each sample of the scaled
data set.
If we call our desired normally distributed random data set ydesired, then the nth element of that set is
described mathematically as
(13-70)
Our discussion thus far has had a decidedly software algorithm flavor, but hardware designers also
occasionally need to generate normally distributed random data at high speeds in their designs. For
you hardware designers, reference [30] presents an efficient hardware design technique to generate
normally distributed random data using fixed-point arithmetic integrated circuits.
The above method for generating normally distributed random numbers works reasonably well, but its
results are not perfect because the tails of the probability distribution curve in Figure 13-30 are not
perfectly Gaussian.† An advanced, and more statistically correct (improved randomness), technique
that you may want to explore is called the Ziggurat method[31–33].
† I thank my DSP pal Dr. Peter Kootsookos, of UTC Fire and Security, Farmington, Connecticut, for his advice on this issue.
13.12 Zero-Phase Filtering
You can cancel the nonlinear phase effects of an IIR filter by following the process shown in Figure
13-31(a). The y(n) output will be a filtered version of x(n) with no filter-induced phase distortion.
The  same  IIR  filter  is  used  twice  in  this  scheme,  and  the  time  reversal  step  is  a  straight  left-right
flipping of a time-domain sequence. Consider the following. If some spectral component in x(n) has
an arbitrary phase of α degrees, and the first filter induces a phase shift of −β degrees, that spectral
component’s  phase  at  node  A  will  be  α−β  degrees.  The  first  time  reversal  step  will  conjugate  that
phase  and  induce  an  additional  phase  shift  of  −θ  degrees.  (Appendix  C  explains  this  effect.)
Consequently, the component’s phase at node B will be −α+β−θ degrees. The second filter’s phase
shift  of  −β  degrees  yields  a  phase  of  −α−θ  degrees  at  node  C.  The  final  time  reversal  step  (often
omitted  in  literary  descriptions  of  this  zero-phase  filtering  process)  will  conjugate  that  phase  and
again induce an additional phase shift of −θ degrees. Thankfully, the spectral component’s phase in
y(n)  will  be  α+θ−θ  =  α  degrees,  the  same  phase  as  in  x(n).  This  property  yields  an  overall  filter
whose phase response is zero degrees over the entire frequency range.
Figure 13-31 Two equivalent zero-phase filtering techniques.
An  equivalent  zero-phase  filter  is  presented  in  Figure 13-31(b).  Of  course,  these  methods  of  zero-
phase filtering cannot be performed in real time because we can’t reverse the flow of time (at least
not  in  our  universe).  This  filtering  is  a  block processing,  or  off-line,  process,  such  as  filtering  an
audio  file  stored  in  a  computer.  We  must  have  all  the  time  samples  available  before  we  start
processing. The initial time reversal in Figure 13-31(b) illustrates this restriction.
There  will  be  filter  transient  effects  at  the  beginning  and  end  of  the  filtered  sequences.  If  transient
effects are bothersome in a given application, consider discarding L samples from the beginning and
end of the final y(n) time sequence, where L is four (or five) times the order of the IIR filter.
By the way, the final peak-to-peak passband ripple (in dB) of this zero-phase filtering process will be
twice  the  peak-to-peak  passband  ripple  of  the  single  IIR  filter.  The  final  stopband  attenuation  will
also be double that of the single filter.
13.13 Sharpened FIR Filters
Here’s  an  interesting  technique  for  improving  the  stopband  attenuation  of  a  digital  filter  under  the
condition that we’re unable, for whatever reason, to modify that filter’s coefficients. Actually, we can
double  a  filter’s  stopband  attenuation  by  cascading  the  filter  with  itself.  This  works,  as  shown  in
Figure 13-32(a), where the frequency magnitude response of a single filter is a dashed curve |H(m)|
and  the  response  of  the  filter  cascaded  with  itself  is  represented  by  the  solid  curve  |H2(m)|.  The
problem  with  this  simple  cascade  idea  is  that  it  also  doubles  the  passband  peak-to-peak  ripple  as
shown in Figure 13-32(b). The frequency axis in Figure 13-32 is normalized such that a value of 0.5
represents half the signal sample rate.
Figure 13-32 Frequency magnitude responses of a single filter and that filter cascaded with itself: (a)
full response; (b) passband detail.
Well,  there’s  a  better  scheme  for  improving  the  stopband  attenuation  performance  of  a  filter  and
avoiding  passband  ripple  degradation  without  actually  changing  the  filter’s  coefficients.  The
technique is called filter sharpening[34] and is shown as Hs in Figure 13-33.
Figure 13-33 Filter sharpening process.
The  delay  element  in  Figure  13-33  is  equal  to  (N−1)/2  samples  where  N  is  the  number  of  h(k)
coefficients,  the  unit-impulse  response  length,  in  the  original  H(m)  FIR  filter.  Using  the  sharpening
process results in the improved |Hs(m)| filter performance shown as the solid curve in Figure 13-34,
where we see the increased stopband attenuation and reduced passband ripple beyond that afforded
by the original H(m) filter. Because of the delayed time-alignment constraint, filter sharpening is not
applicable  to  filters  having  non-constant  group  delay,  such  as  minimum-phase  FIR  filters  or  IIR
filters.
Figure 13-34 |H(m)| and |Hs(m)| performance: (a) full frequency response; (b) passband detail.
If need be, we can eliminate the multipliers shown in Figure 13-33. The multiply by two operation
can be implemented with an arithmetic left shift by one binary bit. The multiply by three operation can
be implemented by adding the Delay output sample to a shifted-left-by-one-bit version of itself.
Be aware that the gain factors in Figure 13-33 are based on the assumption that the original h(k) filter
to be sharpened has a passband gain of one. If the h(k) filter has a non-unity passband gain of G ≠ 1,
then the network in Figure 13-35(a) should be used, where the alternate constant gain factors provide
optimum  filter  sharpening.  On  the  other  hand,  the  Figure 13-35(a)  gain  factors  can  be  modified  to
some extent if doing so simplifies the filter implementation. For example, if 2/G2 = 1.7, for ease of
implementation, the practitioner should try using a factor of 2 in place of the factor 1.7. Using a gain
factor of 2 will not be optimum but it may well be acceptable, depending on the characteristics of the
filter to be sharpened. Software modeling will resolve this issue.
Figure 13-35 Non-unity gain filter sharpening: (a) low-order sharpening; (b) higher-order sharpening
for increased stopband attenuation.
If additional stopband attenuation is needed, then the process shown in Figure 13-35(b) can be used,
where again the Delay element is equal to (N-1)/2 unit delays.
In  real-time  applications,  the  filter  sharpening  networks  we  presented  are  straightforward  and
applicable to linear-phase lowpass, bandpass, and highpass FIR filters, just so long as the original
filter’s  H(f)  has  an  integer  group  delay.  (That  restriction  is  necessary  because  the  number  of  unit
delays of the Delay element, needed for time synchronization in real-time systems, in the parallel path
must be an integer.) This sharpening procedure is particularly useful if the original filter hardware is
constrained  to  have  some  fixed  number  of  bits  to  represent  its  coefficients.  If  an  FIR  filter’s
coefficient  bit  width  is  b  bits,  the  filter  sharpening  process  in  Figure  13-33  can,  luckily  for  us,
achieve  the  performance  of  filters  having  (b  +  4)-bit  coefficients.  So,  if  our  hardware  forces  us  to
use, say, 8-bit coefficients, we can achieve roughly 12-bit-coefficient filter performance.
Filter  sharpening  can  be  used  whenever  a  given  filter  response  cannot  be  modified,  such  as  an
unchangeable software subroutine, and can even be applied to cascaded integrator-comb (CIC) filters
to  flatten  their  passband  responses,  as  well  as  FIR  fixed-point  multiplierless  filters  where  the
coefficients are constrained to be powers of two[35,36].
As a historical aside, filter sharpening  is  a  process  refined  and  expanded  by  the  accomplished  R.
Hamming (of Hamming window fame) based on an idea originally proposed by the great American
mathematician John Tukey, the inventor of the radix-2 fast Fourier transform (FFT).
13.14 Interpolating a Bandpass Signal
There are many digital communications applications where a real signal is centered at one-fourth the
sample  rate,  or  fs/4.  This  condition  makes  quadrature  down-conversion  particularly  simple.  (See
Sections 8.9 and 13.1.) In the event that you’d like to generate an interpolated (increased sample rate)
version  of  the  bandpass  signal  but  maintain  its  fs/4  center  frequency,  there’s  an  efficient  way  to  do
so[37]. Suppose we want to interpolate by a factor of two so the output sample rate is twice the input
sample  rate,  fs-out  =  2fs-in.  In  this  case  the  process  is:  quadrature  down-conversion  by  fs-in/4,
interpolation factor of two, quadrature up-conversion by fs-out/4, and then take only the real part of the
complex upconverted sequence. The implementation of this scheme is shown at the top of Figure 13-
36.
Figure 13-36 Bandpass signal interpolation scheme, and spectra.
The  sequences  applied  to  the  first  multiplier  in  the  top  signal  path  are  the  real  x(n)  input  and  the
repeating  mixing  sequence  1,0,−1,0.  That  mixing  sequence  is  the  real  (or  in-phase)  part  of  the
complex exponential
(13-71)
needed  for  quadrature  down-conversion  by  fs/4.  Likewise,  the  repeating  mixing  sequence  0,−1,0,1
applied  to  the  first  multiplier  in  the  bottom  path  is  the  imaginary  (or  quadrature  phase)  part  of  the
complex down-conversion exponential e−j2π(fs-in/4)ts-in. The “↑2” symbol means insert one zero-valued
sample between each sample at the A nodes. The final subtraction to obtain y(n) is how we extract the
real part of the complex sequence at Node D. (That is, we’re extracting the real part of the product of
the complex signal at Node C times ej2π(1/4).) The spectra at various nodes of this process are shown
at the bottom of Figure 13-35. The shaded spectra indicate true spectral components, while the white
spectra  represent  spectral  replications.  Of  course,  the  same  lowpass  filter  must  be  used  in  both
processing paths to maintain the proper time delay and orthogonal phase relationships.
There are several additional issues worth considering regarding this interpolation process[38]. If the
amplitude  loss,  inherent  in  interpolation,  of  a  factor  of  two  is  bothersome,  we  can  make  the  final
mixing sequences 2,0,−2,0 and 0,2,0,−2 to compensate for that loss. Because there are so many zeros
in  the  sequences  at  Node  B  (three-fourths  of  the  samples),  we  should  consider  those  efficient
polyphase filters for the lowpass filtering. Finally, if it’s sensible in your implementation, consider
replacing the final adder with a multiplexer (because alternate samples of the sequences at Node D
are zeros). In this case, the mixing sequence in the bottom path would be changed to 0,−1,0,1.
13.15 Spectral Peak Location Algorithm
In  the  practical  world  of  discrete  spectrum  analysis,  we  often  want  to  estimate  the  frequency  of  a
sinusoid (or the center frequency of a very narrowband signal of interest). Upon applying the radix-2
fast Fourier transform (FFT), our narrowband signals of interest rarely reside exactly on an FFT bin
center whose frequency is exactly known. As such, due to the FFT’s leakage properties, the discrete
spectrum of a sinusoid having N time-domain samples may look like the magnitude samples shown in
Figure 13-37(a). There we see the sinusoid’s spectral peak residing between the FFT’s m = 5 and m
= 6 bin centers. (Variable m is an N-point  FFT’s  frequency-domain  index.  The  FFT  bin  spacing  is
fs/N where, as always, fs is the sample rate.) Close examination of Figure 13-37(a) allows us to say
the sinusoid lies in the range of m = 5 and m = 5.5, because we see that the maximum spectral sample
is closer to the m = 5 bin center than the m = 6 bin center. The real-valued sinusoidal time signal has,
in this example, a frequency of 5.25fs/N Hz. In this situation, our frequency estimation resolution is
half the FFT bin spacing. We often need better frequency estimation resolution, and there are indeed
several ways to improve that resolution.
Figure 13-37 Spectral magnitudes: (a) N-point FFT; (b) 4N-point FFT.
We  could  collect,  say,  4N  time-domain  signal  samples  and  perform  a  4N-point  FFT,  yielding  a
reduced bin spacing of fs/4N. Or we could pad (append to the end of the original time samples) the
original N time samples with 3N zero-valued samples and perform a 4N-point FFT on the lengthened
time sequence. That would also provide an improved spectral peak estimation granularity of fs/4N, as
shown in Figure 13-37(b). With the spectral peak located at bin mpeak = 21, we estimate the signal’s
center frequency, in Hz, using fpeak = mpeakfs/4N.
Both  schemes,  collect  more  data  and  zero  padding,  are  computationally  expensive.  Many  other
techniques for enhanced-precision tone frequency measurement have been described in the scientific
literature—from the close-to-home field of geophysics to the lofty studies of astrophysics—but most
of  those  schemes  seek  precision  without  regard  to  computational  complexity.  Here  we  describe
several computationally simple frequency estimation schemes.
Assume we have the X(m) spectral samples from an N-point FFT of a sinusoidal time signal, whose
magnitudes  are  shown  in  Figure 13-38(a).  (The  vertical  magnitude  axis  is  linear,  not  logarithmic.)
The notation in the figure is that mk is the integer index of the largest magnitude sample |X(mk)|. The
value mpeak, which in general will not be an integer, is the value we wish to estimate and use in
(13-72)
Figure 13-38 Spectral peak detection: (a) FFT magnitudes; (b) mpeak error by naive assignment; (c)
mpeak algorithm error performance.
to accurately estimate the sinusoid’s center frequency in Hz.
Next,  let’s  say  the  FFT’s  input  sinusoid  sweeps  in  frequency  starting  at  the  FFT’s  mk  bin  center
frequency to the center frequency of the mk+1 bin and we assign mpeak to be equal to the index value
(either mk or mk+1) of the highest spectral magnitude sample. The error in that mpeak value will be that
shown in Figure 13-38(b). The maximum error in that naive mpeak assignment scheme is 0.5 FFT bins
(half the FFT bin spacing). Happily for us, there are more accurate methods for estimating mpeak.
As it turns out, we can estimate the signal’s index-based center frequency, mpeak, using
(13-73)
where Ci  is  a  scalar  correction  factor  in  the  range  of  −0.5  ≤  Ci  ≤  0.5.  There  are  many  algorithms,
based on fitting a generic parabolic curve to the |X(m)| samples, floating around in the literature of
DSP for estimating Ci. Those algorithms have varying degrees of accuracy depending on the window
function applied to the FFT’s input samples.
A noteworthy correction factor expression is
(13-74)
This  complex-valued  spectral  peak  location  estimation  algorithm  is  quite  accurate  for  its
simplicity[3]. Its maximum frequency estimation error is roughly 0.06, 0.04, and 0.03 bin widths for
signal-to-noise ratios of 3, 6, and 9 dB respectively. Not bad at all! The nice features of the algorithm
are that it does not require the original time samples to be windowed, as do some other spectral peak
location algorithms; and it does not require computation of FFT magnitude samples.
If  a  time-domain  window  sequence  has  been  applied  to  the  FFT’s  input  samples,  then  other  Ci
correction  factor  expressions  should  be  used  in  place  of  Eq.  (13-74).  Three  notable  candidate
expressions for Ci are
(13-75)
(13-75′)
(13-75″)
where  again  we  use  subscripts  on  C  merely  to  identify  the  different  expressions  for  the  correction
factor Ci. The above window-dependent P and Q factors, determined empirically, are
• Hamming, P = 1.22, Q = 0.60;
• Hanning, P = 1.36, Q = 0.55;
• Blackman, P = 1.75, Q = 0.55; and
• Blackman-Harris (3-term), P = 1.72, Q = 0.56.
Equation (13-75) is the best known peak location algorithm and has been used in the DSP business for
decades.  The  lesser-known  Eq.  (13-75′)  provides  a  more  accurate  windowed-FFT  peak  location
estimate than Eq. (13-75)[39]. Inspired by Eqs. (13-74) and (13-75′), the author has developed Eq.
(13-75″) which can be used in case the FFT magnitude samples are unavailable for use in Eq. (13-
75′). Equation (13-75″) is also more accurate than the better-known Eq. (13-75).
The  solid  curve  in  Figure  13-38(c)  shows  the  mpeak  error  in  using  Eq.  (13-75′)  with  Blackman-
windowed  time-domain  samples  whose  signal-to-noise  ratio  is  9  dB.  For  comparison,  the  dashed
curve is the mpeak error when using Eq. (13-75). Equation (13-75″)’s accuracy is very similar to that
of Eq. (13-75′).
Equations  (13-74)  and  (13-75″)  have  the  advantage  that  FFT  magnitude  calculations,  with  their
computationally  costly  square  root  operations,  are  not  required  as  is  necessary  with  other  spectral
peak  location  algorithms  described  above.  However,  the  question  naturally  arises,  “How  do  we
determine  the  index  mk  of  the  largest-magnitude  FFT  sample,  |X(mk)|,  in  Figure  13-38(a)  without
computing  square  roots  to  obtain  FFT  magnitudes?”  The  answer  is  that  we  can  use  the  complex
vector-magnitude approximations, requiring no square root computations, described in Section 13.2.
Be aware that the above spectral peak location methods are only applicable when the majority of the
signal’s spectral energy lies within a single FFT bin width (fs/N), and the FFT spectral samples are
not substantially contaminated by leakage from another spectral component.
13.16 Computing FFT Twiddle Factors
Typical applications using an N-point radix-2 FFT accept N x(n) input time samples and compute N
X(m)  frequency-domain  samples.  However,  there  are  non-standard  FFT  applications  (for  example,
specialized harmonic analysis, or perhaps using an FFT to implement a bank of filters) where only a
subset  of  the  full  X(m)  results  is  required.  Consider  Figure  13-39  which  shows  the  butterfly
operations  for  an  8-point  radix-2  decimation-in-frequency  FFT.  Notice  that  the  FFT  butterflies  in
Figure 13-39 are the optimized butterflies introduced in Figure 4-14. Assuming we are only interested
in  the  X(3)  and  X(7)  output  samples,  rather  than  compute  the  entire  FFT  we  perform  only  the
computations indicated by the bold lines in the figure.
Figure 13-39 Eight-point decimation-in-frequency FFT signal-flow diagram.
Reduced-computation  FFTs  are  often  called  pruned  FFTs[40-43].  To  implement  pruned  FFTs  we
need to know the twiddle phase angles associated with each necessary butterfly computation in the
paths  of  any  bold  signal-flow  line  in  Figure  13-39.  (As  we  did  in  Chapter  4  for  simplicity,  the
butterflies  in  Figure 13-39  only  show  the  twiddle  phase-angle  factors  and  not  the  entire  complex-
valued twiddle factors.) Here we show how to compute those individual twiddle phase angles.
13.16.1 Decimation-in-Frequency FFT Twiddle Factors
For the decimation-in-frequency (DIF) radix-2 FFT using the optimized butterflies:
• The N-point DIF FFT has log2(N) stages, numbered P = 1, 2, ..., log2(N).
• Each stage comprises N/2 butterflies.
• Not counting the −1 twiddle factors, the Pth stage has N/2P unique twiddle factors, numbered k =
0, 1, 2, ... , N/2P−1 as indicated by the upward arrows at the bottom of Figure 13-39.
Given those characteristics, the kth unique twiddle factor phase angle for the Pth stage is computed
using
(13-76)
where 0 ≤ k ≤ N/2P−1. For example, for the second stage (P  =  2)  of  an  N  =  8-point  DIF  FFT,  the
unique twiddle factor angles are
k = 0, angle = 0·2P/2 = 0·4/2 = 0
k = 1, angle = 1·2P/2 = 1·4/2 = 2.
13.16.2 Decimation-in-Time FFT Twiddle Factors
Here  we  present  an  interesting  algorithm  for  computing  the  individual  twiddle  factor  angles  of  a
radix-2 decimation-in-time (DIT) FFT[44]. Consider Figure 13-40 showing the butterfly signal flow
of an 8-point DIT FFT.
Figure 13-40 Eight-point decimation-in-time FFT signal-flow diagram.
For the decimation-in-time (DIT) FFT using the optimized butterflies:
• The N-point DIT FFT has log2(N) stages, numbered P = 1, 2, . . ., log2(N).
• Each stage comprises N/2 butterflies.
• Not counting the −1 twiddle factors, the Pth stage has N/2 twiddle factors, numbered k = 0, 1, 2, ...
, N/2−1 as indicated by the upward arrows at the bottom of Figure 13-40.
Given those characteristics, the kth twiddle factor phase angle for the Pth stage is computed using
(13-76′)
 operation means the integer part of q. The [z]bit-rev function represents
where 0 ≤ k ≤ N/2−1. The
the three-step operation of: convert decimal integer z to a binary number represented by log2(N)−1
binary bits, perform bit reversal on the binary number as discussed in Section 4.5, and convert the
bit-reversed number back to a decimal integer.
As an example of using Eq. (13-76′), for the second stage (P = 2) of an N = 8-point DIT FFT, the k =
3 twiddle factor angle is
The above [1]bit-rev operation is: Take the decimal number 1 and represent it with log2(N)−1 = 2 bits,
i.e., as 012.  Next,  reverse  those  bits  to  a  binary  102  and  convert  that  binary  number  to  our  desired
decimal result of 2.
13.17 Single Tone Detection
In this section we present an IIR filter structure used to perform spectrum analysis in the detection and
measurement  of  single  sinusoidal  tones.  The  standard  method  for  spectral  energy  is  the  discrete
Fourier  transform  (DFT),  typically  implemented  using  a  fast  Fourier  transform  (FFT)  algorithm.
However, there are applications that require spectrum analysis only over a subset of the N bin-center
frequencies of an N-point DFT. A popular, as well as efficient, technique for computing sparse FFT
results is the Goertzel algorithm, using an IIR filter implementation to compute a single complex DFT
spectral bin value based upon N input time samples. The most common application of this process is
to  detect  the  presence  of  a  single  continuous-wave  sinusoidal  tone.  With  that  in  mind,  let’s  look
briefly at tone detection.
It’s  certainly  possible  to  use  the  FFT  to  detect  the  presence  of  a  single  sinusoidal  tone  in  a  time-
domain sequence x(n). For example, if we wanted to detect a 30 kHz tone in a time-domain sequence
whose  sample  rate  was  fs  =  128  kHz,  we  could  start  by  performing  a  64-point  FFT  as  shown  in
Figure  13-41.  Then  we  would  examine  the  magnitude  of  the  X(15)  complex  sample  to  see  if  it
exceeds some predefined threshold.
Figure 13-41 DFT method, using an FFT algorithm, to detect a 30 kHz tone.
This  FFT  method  is  very  inefficient.  In  our  example,  we’d  be  performing  192,  (64/2)(log264),
complex multiplies to obtain the 64-point complex X(m) in order to compute the one X(15) in which
we’re interested. We discarded 98 percent of our computation results! We could be more efficient and
calculate our desired X(15) using the single-point discrete Fourier transform (DFT) in Eq. (13-77),
which requires N = 64 complex multiplies using
(13-77)
That would be an improvement but, happily, there’s a better way. It’s called the Goertzel algorithm
(pronounced ‘girt-zel).
13.17.1 Goertzel Algorithm
The Goertzel algorithm is implemented in the form of a 2nd-order IIR filter, with two real feedback
coefficients and a single complex feedforward coefficient, as shown in Figure 13-42. (Although we
don’t use this process as a traditional filter, common terminology refers to the structure as a filter.)
This filter computes a single-bin DFT output (the mth bin of an N-point DFT) defined by
(13-78)
Figure 13-42 IIR filter implementation of the Goertzel algorithm.
The filter’s y(n) output is equal to the DFT output frequency coefficient, X(m), at the time index n =
N, where the first time index value is n = 0. For emphasis, we remind the reader that the filter’s y(n)
output is not equal to X(m) at any time index when n ≠ N. To be equivalent to the DFT, the frequency-
domain index m must an integer in the range 0 ≤ m ≤ N−1. You’re welcome to think of the Goertzel
algorithm  as  a  single-bin  DFT.  The  derivation  of  this  filter  (this  algorithm)  structure  is  readily
available in the literature[45–47].
The z-domain transfer function of the Goertzel filter is
(13-79)
with a single z-domain zero located at z = e−j2πm/N and conjugate poles at z = e±j2πm/N as shown in
Figure 13-43(a). The pole/zero pair at z = e−j2πm/N cancel each other. Having a filter pole on the unit
circle  is  typically  a  risky  thing  to  do  for  stability  reasons,  but  not  so  with  the  Goertzel  algorithm.
Because  it  processes  N+1-length  blocks  of  time  samples  (where  N  is  usually  in  the  hundreds),  the
filter remains stable for such short time sequences because its internal data storage registers, w(n−1)
and w(n−2), are reset to zero at the beginning of each new block of input data. The filter’s frequency
magnitude  response,  provided  in  Figure  13-43(b),  shows  resonance  centered  at  a  normalized
frequency of 2πm/N, corresponding to a cyclic frequency of mfs/N Hz (where fs is the signal sample
rate).
Figure 13-43 Goertzel filter: (a) z-domain pole/zero locations; (b) frequency magnitude response.
The  Goertzel  algorithm  is  implemented  with  a  complex  resonator  having  an  infinite-length  unit
impulse response, h(n) = ej2πnm/N, and that’s why its frequency magnitude response is so narrow. The
time-domain difference equations for the Goertzel filter are
(13-80)
(13-81)
An advantage of the Goertzel filter in computing an N-point X(m) DFT bin value is that Eq. (13-80) is
implemented  N  times  while  Eq.  (13-81),  the  feedforward  path  in  Figure  13-42,  need  only  be
computed once after the arrival of the Nth input sample. Thus for real x(n) inputs the filter requires
N+2 real multiplies and 2N+1 real adds to compute an N-point X(m). However, when modeling the
Goertzel filter, if the time index begins at n = 0, the filter must process N+1 time samples with x(N) =
0 to compute X(m).
In  typical  applications,  to  minimize  spectral  leakage,  we  choose  N  so  there’s  an  integer  number  of
cycles in our input sequence of the tone we’re trying to detect. N can be any integer, and the larger N
is,  the  better  the  frequency  resolution  and  noise  immunity.  However,  larger  N  means  more
computations.
It’s worth noting that while the typical Goertzel algorithm description in the literature specifies the
frequency resonance variable m to be an integer (making the Goertzel filter’s output equivalent to an
N-point DFT bin output), the m in Figure 13-42 and Eq. (13-79) can in fact be any value between 0
and N−1, giving us full flexibility in specifying our filter’s resonant frequency.
13.17.2 Goertzel Example
Let’s use Goertzel to calculate the spectral magnitude of that ftone = 30 kHz tone from the Figure 13-41
example. When fs = 128 kHz and N = 64, our resonant frequency integer m is
(13-82)
The Goertzel filter and the necessary computations for our 30 kHz detection example are provided in
Figure 13-44.
Figure 13-44 Filter, coefficients, and computations to detect the 30 kHz tone.
It’s  useful  to  know  that  if  we  want  to  compute  the  power  of  X(15), |X(15)2|,  the  final  feedforward
complex calculations can be avoided by computing
In our example, Eq. (13-83) becomes
(13-83)
(13-84)
13.17.3 Goertzel Advantages over the FFT
Here are some implementation advantages of the Goertzel algorithm over the standard radix-2 FFT
for single tone detection:
• N does not need to be an integer power of two.
• The resonant frequency can be any value between zero and fs Hz.
• The amount of filter coefficient (versus FFT twiddle factor) storage is reduced. If Eq. (13-83) is
used, only one coefficient need be stored.
•  No  storing  a  block  of  input  data  is  needed  before  processing  can  begin  (as  with  the  FFT).
Processing can begin with the first input time sample.
• No data bit reversal is needed for Goertzel.
•  If  you  implement  the  Goertzel  algorithm  M  times  to  detect  M  different  tones,  Goertzel  is  more
efficient (fewer multiplies) than the FFT when M < log2N.
•  Computational  requirements  to  detect  a  single  tone  (assuming  real-only  x(n)  input)  are  given  in
Table 13-4.
Table 13-4 Single-Bin DFT Computational Comparisons
As  a  final  note,  although  the  Goertzel  algorithm  is  implemented  with  a  complex  resonating  filter
structure,  it’s  not  used  as  a  typical  filter  where  we  retain  each  output  sample.  For  the  Goertzel
algorithm  we  retain  only  every  Nth,  or  (N+1)th,  output  sample.  As  such,  the  frequency  magnitude
response of the Goertzel algorithm when treated as a black-box process is equivalent to the |sin(x)/x|-
like magnitude response of a single bin of an N-point DFT, a portion of which is shown in Figure 13-
45.
Figure 13-45 Goertzel algorithm frequency magnitude response.
13.18 The Sliding DFT
The above Goertzel algorithm computes a single complex DFT spectral bin value for every N input
time samples. Here we describe a sliding DFT process whose spectral bin output rate is equal to the
input data rate, on a sample-by-sample basis, with the advantage that it requires fewer computations
than the Goertzel algorithm for real-time spectral analysis. In applications where a new DFT output
spectrum is desired every sample, or every few samples, the sliding DFT is computationally simpler
than the traditional radix-2 FFT.
13.18.1 The Sliding DFT Algorithm
The sliding DFT (SDFT) algorithm computes a single bin result of an N-point DFT on time samples
within a sliding window. That is, for the mth bin of an N-point DFT, the SDFT computes
(13-85)
Let’s  take  care  to  understand  the  notation  of  Xm(q).  Typically,  as  in  Chapter 3,  the  index  of  a  DFT
result value was the frequency-domain index m. In Eq. (13-85) the index of the DFT result is a time-
domain index q = 0, 1, 2, 3, ..., such that our first mth-bin SDFT is Xm(0), our second SDFT is Xm(1),
and so on.
An example SDFT analysis time window is shown in Figure 13-46(a) where Xm(0) is computed for
the N = 16 time samples x(0) to x(15). The time window is then advanced one sample, as in Figure
13-46(b), and the new Xm(1) is calculated. The value of this process is that each new DFT result is
efficiently  computed  directly  from  the  result  of  the  previous  DFT.  The  incremental  advance  of  the
time window for each output computation leads to the name sliding DFT or sliding-window DFT.
Figure 13-46 Analysis window for two 16-point DFTs: (a) data samples in the first computation; (b)
second computation samples.
We  can  develop  the  mathematical  expression  for  the  SDFT  as  follows:  the  standard  N-point  DFT
equation, of the mth DFT bin, for the qth DFT of the time sequence x(q), x(q+1), ..., x(q+N−1) is
(13-86)
(Variable m is the frequency-domain index, where m = 0, 1, 2, ..., N−1.) Likewise, the expression for
the next DFT, the (q+1)th DFT performed on time samples x(q+1), x(q+2), ..., x(q+N), is
Letting p = n+1 in Eq. (13-87), we can write
(13-87)
(13-88)
Shifting the limits of summation in Eq. (13-88), and including the appropriate terms (subtract the p =
0 term and add the p = N term) to compensate for the shifted limits, we write
(13-89)
Factoring the common exponential term (ej2πm/N), we write
(13-90)
Recognizing the summation in the brackets being equal to the previous Xm(q) in Eq. (13-86), and e
−j2πm = 1, we write the desired recursive expression for the sliding N-point DFT as
(13-91)
where Xm(q+1) is the new single-bin DFT result and Xm(q) is the previous single-bin DFT value. The
superscript m reminds us that the Xm(q) spectral samples are those associated with the mth DFT bin.
Let’s plug some numbers into Eq. (13-91) to reveal the nature of its time indexing. If N = 20, then 20
time samples (x(0) to x(19)) are needed to compute the first result Xm(0). The computation of Xm(1) is
then
(13-92)
Due to our derivation method’s time indexing, Eq. (13-92) appears compelled to look into the future
for x(20) to compute Xm(1). With no loss in generality, we can modify Eq. (13-91)’s time indexing so
that the x(n) input samples and the Xm(q) output samples use the same time index n. That modification
yields our SDFT time-domain difference equation of
(13-93)
Equation (13-93) reveals the value of this process in computing real-time spectra. We compute Xm(n)
by subtracting the x(n−N)  sample  and  adding  the  current  x(n) sample to the previous Xm(n−1),  and
phase shifting the result. Thus the SDFT requires only two real additions and one complex multiply
per  output  sample.  Not  bad  at  all!  Equation  (13-93)  leads  to  the  single-bin  SDFT  filter
implementation shown in Figure 13-47.
Figure 13-47 Single-bin sliding DFT filter structure.
The  single-bin  SDFT  algorithm  is  implemented  as  an  IIR  filter  with  a  comb  filter  followed  by  a
complex resonator. (If you need to compute all N DFT spectral components, N resonators with m = 0
to N−1 will be needed, all driven by a single comb filter.) The comb filter delay of N samples forces
the  SDFT  filter’s  transient  response  to  be  N  samples  in  length,  so  the  output  will  not  reach  steady
state until the Xm(N−1) sample. The output will not be valid, or equivalent to Eq. (13-86)’s Xm(q),
until N input samples have been processed. The z-transform of Eq. (13-93) is
(13-94)
where factors of Xm(z) and X(z) are collected, yielding the z-domain transfer function for the mth bin
of the SDFT filter as
(13-95)
This complex filter has N zeros equally spaced around the z-domain’s unit circle, due to the N-delay
comb filter, as well as a single pole canceling the zero at z = ej2πm/N. The SDFT filter’s complex unit
impulse response h(n) and pole/zero locations are shown in Figure 13-48 for the example where m =
2 and N = 20.
Figure 13-48 Sliding DFT characteristics for m = 2 and N = 20: (a) complex impulse response; (b)
pole/zero locations.
Because of the comb subfilter, the SDFT filter’s complex sinusoidal unit impulse response is finite in
length—truncated in time to N samples—and that property makes the frequency magnitude response of
the SDFT filter identical to the sin(Nx)/sin(x) response of a single DFT bin centered at a frequency of
2πm/N.
One of the attributes of the SDFT is that once an Xm(n) is obtained, the number of computations to
compute Xm(n+1) is fixed and independent of N. A computational workload comparison between the
Goertzel and SDFT filters is provided later in this section. Unlike the radix-2 FFT, the SDFT’s N can
be any positive integer, giving us greater flexibility to tune the SDFT’s center frequency by defining
integer m such that m = Nfi/fs, when fi is a frequency of interest in Hz and fs is the signal sample rate in
Hz.  In  addition,  the  SDFT  requires  no  bit-reversal  processing  as  does  the  FFT.  Like  the  Goertzel
algorithm, the SDFT is especially efficient for narrowband spectrum analysis.
For completeness, we mention that a radix-2 sliding FFT technique exists for computing all N bins of
Xm(q) in Eq. (13-85)[48,49]. That technique is computationally attractive because it requires only N
complex  multiplies  to  update  the  N-point  FFT  for  all  N  bins;  however,  it  requires  3N  memory
locations  (2N  for  data  and  N  for  twiddle  coefficients).  Unlike  the  SDFT,  the  radix-2  sliding  FFT
scheme requires address bit-reversal processing and restricts N to be an integer power of two.
13.18.2 SDFT Stability
The SDFT filter is only marginally stable because its pole resides on the z-domain’s unit circle. If
filter coefficient numerical rounding error is not severe, the SDFT is bounded-input-bounded-output
stable.  Filter  instability  can  be  a  problem,  however,  if  numerical  coefficient  rounding  causes  the
filter’s pole to move outside the unit circle. We can use a damping factor r to force the pole and zeros
in Figure 13-48(b) to be at a radius of r just slightly inside the unit circle and guarantee stability using
a transfer function of
(13-96)
with the subscript “gs” meaning guaranteed-stable. (Section 7.5.3 provides the mathematical details
of moving a filter’s poles and zeros inside the unit circle.) The stabilized feedforward and feedback
coefficients become −rN and rej2πm/N, respectively. The difference equation for the stable SDFT filter
becomes
(13-97)
with  the  stabilized-filter  structure  shown  in  Figure  13-49.  In  this  case,  we  perform  five  real
multiplies and four real additions per output sample.
Figure 13-49 Guaranteed-stable sliding DFT filter structure.
Using a damping factor as in Figure 13-49 guarantees stability, but the Xm(q) output, defined by
(13-98)
is no longer exactly equal to the mth bin of an N-point DFT in Eq. (13-85). While the error is reduced
by  making  r  very  close  to  (but  less  than)  unity,  a  scheme  does  exist  for  eliminating  that  error
completely  once  every  N  output  samples  at  the  expense  of  additional  conditional  logic
operations[50]. Determining if the damping factor r is necessary for a particular SDFT application
requires careful empirical investigation. As is so often the case in the world of DSP, this means you
have to test your SDFT implementation very thoroughly and carefully!
Another stabilization method worth consideration is decrementing the largest component (either real
or imaginary) of the filter’s ej2πm/N feedback coefficient by one least significant bit. This technique
can be applied selectively to problematic output bins and is effective in combating instability due to
rounding errors that result in finite-precision ej2πm/N coefficients having magnitudes greater than unity.
Like the DFT, the SDFT’s output is proportional to N, so in fixed-point binary implementations the
designer must allocate sufficiently wide registers to hold the computed results.
13.18.3 SDFT Leakage Reduction
Being equivalent to the DFT, the SDFT also suffers from spectral leakage effects. As with the DFT,
SDFT leakage can be reduced by the standard concept of windowing the x(n) input time samples as
discussed in Section 3.9.  However,  windowing  by  time-domain  multiplication  would  ruin  the  real-
time computational simplicity of the SDFT. Thanks to the convolution theorem properties of discrete
systems, we can implement time-domain windowing by means of frequency-domain convolution, as
discussed in Section 13.3.
Spectral  leakage  reduction  performed  in  the  frequency  domain  is  accomplished  by  convolving
adjacent  Xm(q)  values  with  the  DFT  of  a  window  function.  For  example,  the  DFT  of  a  Hamming
window  comprises  only  three  nonzero  values,  −0.23,  0.54,  and  −0.23.  As  such,  we  can  compute  a
Hamming-windowed Xm(q) with a three-point convolution using
(13-99)
Figure 13-50 shows this process using three resonators, each tuned to adjacent DFT bins (m−1, m,
and m+1). The comb filter stage need only be implemented once.
Figure 13-50 Three-resonator structure to compute a single Hamming-windowed Xm(q).
Table 13-5 provides a computational workload comparison of various spectrum analysis schemes in
computing an initial Xm(n) value and computing a subsequent Xm(n+1) value.
Table 13-5 Single-Bin DFT Computation Comparison
To  compute  the  initial  windowed  Xm(n)  values  in  Table  13-5,  the  three-term  frequency-domain
convolution  need  only  be  performed  once,  upon  arrival  of  the  Nth  time  sample.  However,  the
convolution needs to be performed for all subsequent computations
We remind the reader that Section 13.3  discusses  several  implementation  issues  regarding  Hanning
windowing in the frequency domain, using binary shifts to eliminate the multiplications in Eq. (13-
99), as well as the use of other window functions.
13.18.4 A Little-Known SDFT Property
The  SDFT  has  a  special  property  that’s  not  widely  known  but  is  very  important.  If  we  change  the
SDFT’s comb filter feedforward coefficient (in Figure 13-47) from −1 to +1, the comb’s zeros will
be rotated counterclockwise around the unit circle by an angle of π/N radians. This situation, for N =
8, is shown on the right side of Figure 13-51(a). The zeros are located at angles of 2π(m + 1/2)/N
radians. The m = 0 zeros are shown as solid dots. Figure 13-51(b) shows the zeros locations for an N
= 9 SDFT under the two conditions of the comb filter’s feedforward coefficient being −1 and +1.
Figure 13-51 Four possible orientations of comb filter zeros on the unit circle.
This alternate situation is useful: we can now expand our set of spectrum analysis center frequencies
to more than just N angular frequency points around the unit circle. The analysis frequencies can be
either 2πm/N or 2π(m+1/2)/N, where integer m is in the range 0 ≤ m ≤ N−1. Thus we can build an
SDFT  analyzer  that  resonates  at  any  one  of  2N  frequencies  between  0  and  fs  Hz.  Of  course,  if  the
comb  filter’s  feedforward  coefficient  is  set  to  +1,  the  resonator’s  feedforward  coefficient  must  be
ej2π(m+1/2)/N to achieve pole/zero cancellation.
13.19 The Zoom FFT
The  Zoom  FFT  is  a  spectrum  analysis  method  that  blends  complex  down-conversion,  lowpass
filtering, and sample rate change by way of decimation. The Zoom FFT scheme (also called the zoom
transform or spectral vernier) is used when fine-grained spectral resolution is needed within a small
portion  of  a  signal’s  overall  frequency  bandwidth  range.  In  some  spectrum  analysis  situations,  this
technique can be more efficient than the traditional FFT. The Zoom FFT can also be useful if we’re
constrained,  for  some  reason,  to  use  software  that  performs  N-point  FFTs  for  spectrum  analysis  of
signal sequences whose lengths are greater than N.
Think of the spectral analysis situation where we require fine frequency resolution, closely spaced
FFT bins, over the frequency range occupied by the signal of interest shown in Figure 13-52(a). (The
other signals are of no interest to us.) We could collect many time samples and perform a large-size
radix-2  FFT  to  satisfy  our  fine  spectral  resolution  requirement.  This  solution  is  inefficient  because
we’d be discarding most of our FFT results. The Zoom FFT can help us improve our computational
efficiency through
Figure 13-52 Zoom FFT spectra: (a) input spectrum; (b) processing scheme; (c) down-converted
spectrum; (d) filtered and decimated spectrum.
• frequency translation by means of complex down-conversion,
• lowpass filtering,
• decimation, and finally
• performing a smaller-size FFT.
The process begins with the continuous x(t) signal being digitized at a sample rate of fs1 by an analog-
to-digital  (A/D)  converter,  yielding  the  N-point  x(n)  time  sequence  whose  spectral  magnitude  is
|X(m)| in Figure 13-52(a). The Zoom FFT technique requires narrowband filtering and decimation in
order to reduce the number of time samples prior to the final FFT, as shown in Figure 13-52(b). The
down-converted signal’s spectrum, centered at zero Hz, is the |Xc(m)| shown in Figure 13-52(c). (The
lowpass  filter’s  frequency  response  is  the  dashed  curve.)  After  lowpass  filtering  xc(n),  the  filter’s
output is decimated by an integer factor D, yielding a time sequence x′c(n) whose sample rate is fs2 =
fs1/D prior to the FFT operation. The key here is that the length of x′c(n) is N/D, allowing a reduced-
size FFT. (N/D must be an integer power of two to enable the use of radix-2 FFTs.) We perform the
FFT  only  over  the  decimated  signal’s  bandwidth.  It’s  of  interest  to  note  that,  because  its  input  is
complex,  the  N/D-point  FFT  has  a  non-redundant  frequency  analysis  range  from  −fs2/2  to  +fs2/2
(unlike the case of real inputs, where the positive- and negative-frequency ranges are redundant).
The implementation of the Zoom FFT is given in Figure 13-53, where all discrete sequences are real-
valued.
Figure 13-53 Zoom FFT processing details.
Relating  the  discrete  sequences  in  Figure  13-52(b)  and  Figure  13-53,  the  complex  time  sequence
xc(n) is represented mathematically as
(13-100)
while the complex decimated sequence x′c(n) is
(13-101)
The complex mixing sequence e−j2πfcnts1, where ts1 = 1/fs1, can be represented in the two forms of
(13-102)
Relative  to  FFT  computations,  we  see  that  an  N/D-point  Zoom  FFT  yields  a  reduction  in
computations compared to a standard N-point FFT for spectrum analysis of a narrowband portion of
some X(m) spectrum—and the computational savings improve as the decimation factor D increases.
Ah, but here’s the rub. As D increases, the lowpass filters must become narrower, which increases
their computational workload, and this is the trade-off we face. What we must ask ourselves is “Does
the Zoom FFT’s reduced FFT size compensate for the additional quadrature mixing and dual filtering
computational workload?” (It certainly would if a large-size FFT is impossible with your available
FFT hardware or software.)
To gain a rough appreciation for the computational savings gained by using an N/D-point Zoom FFT,
compared  to  a  standard  N-point  FFT,  let’s  look  at  Figure  13-54.  That  figure  shows  the  percent
computational savings of a Zoom FFT versus a standard N-point FFT for various decimation factors
D.
Figure 13-54 Zoom FFT computation reduction.
The curves were computed using the following definition for percent computation reduction
(13-103)
under  the  assumptions  that  the  time  sequences  applied  to  the  FFTs  were  windowed,  and  the  Zoom
FFT’s lowpass filters were 24th-order (25 multiplications per output sample) tapped-delay line FIR
filters using folded FIR structures. In Eq. (13-103) a single real multiply and a single real addition
are both considered as a single computation.
The  range  where  Figure  13-54’s  curves  have  negative  values  means  that  the  Zoom  FFT  is  less
efficient (more computations) than a standard N-point FFT. As it turns out, the curves in Figure 13-54
quickly move downward in efficiency as the order of the lowpass filters increases. So it’s in our best
interest  to  make  the  lowpass  filters  as  computationally  efficient  as  possible.  Some  ways  to  do  this
are:
• Partition the lowpass filtering/decimation process into multiple stages (multistage decimation) as
discussed in Chapter 10.
•  Incorporate  cascaded  integrator-comb  (CIC)  filters  into  the  lowpass  filtering  if  the  spectrum  of
interest is very narrowband relative to the fs1.
• Use interpolated FIR filters as discussed in Chapter 7.
• Use polyphase filters as discussed in Chapter 10.
• Restrict the decimation factor D to be an integer power of two such that efficient half-band filters
can be used.
• Use IIR filters, if spectral phase distortion can be tolerated.
13.20 A Practical Spectrum Analyzer
Here’s a clever trick for implementing a practical spectrum analyzer by modifying the time-domain
data before applying a radix-2 FFT algorithm.
Let’s say we need to build a spectrum analyzer to display, in some manner, the spectral magnitude of a
time-domain  sequence.  We’d  like  our  spectrum  analyzer,  a  bank  of  bandpass  filters,  to  have  a
frequency magnitude response something like that shown in Figure 13-55(a). For spectrum analysis,
the  radix-2  FFT  algorithm  comes  to  mind  first,  as  it  should.  However,  the  frequency  response  of
individual FFT bins is that shown in Figure 13-55(b), with their non-flat passbands, unpleasantly high
sidelobes due to spectral leakage, and overlapped main lobes. We can reduce the leakage sidelobe
levels  by  windowing  the  time-domain  sequence,  but  that  leads  to  the  increased  main  lobe  overlap
shown in Figure 13-55(c) and degraded frequency resolution, and we still have considerable droop in
the passband response.
Figure 13-55 Spectrum analyzer: (a) desired frequency response; (b) frequency response of standard
FFT bins; (c) windowed-data FFT frequency response.
Here’s how we can solve our problem. Consider an x(n) sequence of time samples of length M whose
M-point DFT is
(13-104)
Next,  consider  partitioning  x(n)  into  P  subsequences,  each  of  length  N.  Thus  PN  =  M.  If  we  add,
element for element, the P subsequences, we’ll obtain a new y(n) sequence of length N whose N-point
DFT is
The good news is that
(13-105)
(13-106)
That is, the DFT magnitudes of sequence y(n) are equal to a subset of the longer DFT magnitudes of
x(n). Y(m) is equal to a decimated-by-P version of X(k). The relationship between |Y(m)| and |X(Pm)|
doesn’t seem too important, but here’s how we’ll take advantage of that equality. We’ll create an M-
point window sequence whose single-bin frequency response, of an M-point FFT, is the bold curve in
Figure 13-56(a). Instead of computing all M FFT outputs, we’ll only compute every Pth output of the
M-point  FFT,  implementing  Eq.  (13-105),  giving  us  the  decimated  FFT  bins  shown  in  Figure  13-
56(b). In that figure P = 5.
Figure 13-56 FFT spectrum analyzer frequency responses.
That  decimation  of  the  frequency-domain  |X(k)|  spectrum  is  accomplished  in  the  time  domain  by  a
time-aliasing operation as shown in Figure 13-57, where again, for example, P = 5. We partition the
M-sample windowed-x(n) time sequence into P = 5 subsequences and sum the subsequences element
for element to obtain the time-aliased N-sample y(n) sequence. Next, the |Y(m)| spectral magnitudes
are computed using the radix-2 FFT.
Figure 13-57 FFT spectrum analyzer process.
This  process,  sweet  in  its  simplicity,  is  called  the  weighted  overlap-add  structure[51,52]  and  is
alternatively  referred  to  as  the  window-presum  FFT[53].  The  most  difficult  part  of  building  this
analyzer is designing the M-point window sequence used to window the original x(n) sequence. We
do that by specifying the window’s frequency-domain characteristics, just as if it were a digital filter
frequency response, and using our favorite filter design software to compute the filter’s time-domain
impulse response. That impulse response is the window sequence. With the signal sample rate being
fs,  the  window’s  passband  width  will  be  just  less  than  fs/N.  This  makes  the  filter’s  one-sided
passband width roughly fs/2N.
Figure 13-58 illustrates an example FFT analyzer with fs = 1 MHz, N = 64, with P = 5 making M =
320. The FFT bin spacing is 15.63 kHz, so the window design was set for a passband width of 10
kHz  (thus  the  filter’s  one-sided  bandwidth  was  specified  as  5  kHz  in  a  Parks-McClellan  design
routine). Figure 13-58(a) is the 320-point window sequence, while Figure 13-58(b) shows the FFT
analyzer’s response for the m = 3, 4, and 5 bins, with the |Y(4)| response being the solid curve.
Figure 13-58 FFT analyzer example: (a) window sequence; (b) analyzer response for 64-point FFT
bins |Y(3)|, |Y(4)|, and |Y(5)|.
The  width  of  the  spectrum  analyzer’s  passbands  is  primarily  controlled  by  the  window’s  passband
width. The center frequencies of the analyzer’s individual passbands are defined by fs/N. What this
means  is  that  the  amount  of  overlap  in  the  analyzer’s  passbands  depends  on  both  the  window’s
passband width, fs, and N. The dynamic range of the analyzer can be increased by increasing P, which
increases M and lengthens the x(n) sequence. As M is increased, the longer window sequence will
yield  analyzer  passbands  having  a  more  rectangular  shape,  lower  sidelobes,  and  reduced  passband
ripple.
Again, to implement this radix-2 FFT spectrum analyzer, the length of the time-domain sequence (M)
must be an integer multiple (P) of an integer power of two (N).
13.21 An Efficient Arctangent Approximation
Fast and accurate methods for computing the arctangent of a complex number x = I + jQ have been the
subject of extensive study because estimating the angle θ of a complex value has so many applications
in the field of signal processing. The angle of x is defined as θ = tan−1(Q/I).
Practitioners  interested  in  computing  high-speed  (minimum  computations)  arctangents  typically  use
look-up  tables  where  the  value  Q/I  specifies  a  memory  address  in  read-only  memory  (ROM)
containing an approximation of angle θ. For high accuracy, though, this method may require very large
ROM  tables.  Those  folk  interested  in  enhanced  accuracy  implement  compute-intensive  high-order
algebraic polynomials, where Chebyshev polynomials seem to be more popular than Taylor series, to
approximate angle θ. But this polynomial method requires many computations. Unfortunately, because
it  is  such  a  nonlinear  function,  the  arctangent  is  resistant  to  accurate  reasonable-length  polynomial
approximations.  There  is  a  processing  method  called  “CORDIC”  (an  acronym  for  COordinate
Rotation  DIgital  Computer)  that  can  compute  accurate  arctangents  using  only  binary  shifts  and
additions,  but  this  technique  can  require  long  processing  times.  So,  sadly,  we  end  up  choosing  the
least undesirable method for computing arctangents.
If you want to become famous in the field of signal processing, all you have to do is produce a very
accurate arctangent algorithm that requires very few computations. (After solving that problem, you
can then apply your skills to developing a perpetual-motion machine.)
Here’s  another  contender  in  the  arctangent  approximation  race  that  uses  neither  look-up  tables  nor
high-order  polynomials.  We  can  estimate  the  angle  θ,  in  radians,  of  x = I + jQ  using  the  following
approximation:
(13-107)
where −1 ≤ Q/I ≤ 1. That is, θ is in the range −45 to +45 degrees (−π/4 ≤ θ ≤ +π/4 radians). Equation
(13-107) has surprisingly good performance, particularly for a 90-degree (π/2 radians) angle range.
Figure 13-59 shows the maximum error is 0.28 degrees using Eq. (13-107) when the true angle θ is
within the angular range of −45 to +45 degrees
Figure 13-59 Estimated angle θ’ error in degrees.
A nice feature of this θ’ computation is that it can be written as
(13-108)
eliminating  Eq.  (13-107)’s  Q/I  division  operation,  at  the  expense  of  two  additional  multiplies.
Another attribute of Eq. (13-108) is that a single multiply can be eliminated with binary right shifts.
The  product  0.28125Q2  is  equal  to  (1/4+1/32)Q2,  so  we  can  implement  the  product  by  adding  Q2
shifted right by two bits to Q2 shifted right by five bits. This arctangent scheme may be useful in a
digital receiver application where I2 and Q2 have been previously computed in conjunction with an
AM (amplitude modulation) demodulation process or envelope detection associated with automatic
gain control (AGC).
We can extend the angle range over which our approximation operates. If we break up a circle into
eight 45-degree octants, with the first octant being 0 to 45 degrees, we can compute the arctangent of
a complex number residing in any octant. We do this by using the rotational symmetry properties of
the arctangent:
(13-109)
(13-110)
Those properties allow us to create Table 13-6.
Table 13-6 Octant Location versus Arctangent Expressions
So we have to check the signs of Q and I, and see if |Q| > |I|, to determine the octant location, and then
use the appropriate approximation in Table 13-6. Section 13.38 gives a method for determining the
octant of the original θ. The maximum angle approximation error is 0.28 degrees for all octants.
13.22 Frequency Demodulation Algorithms
In  Section  9.2  we  discussed  the  notion  of  measuring  the  instantaneous  frequency  of  a  complex
sinusoidal  signal  by  computing  the  derivative  of  the  signal’s  instantaneous  θ(n)  phase  as  shown  in
Figure 13-60. This is the traditional discrete-signal FM demodulation method, and it works fine. The
demodulator’s instantaneous output frequency is
(13-111)
Figure 13-60 Frequency demodulator using an arctangent function.
where fs is the sample rate in Hz.
Computing instantaneous phase θ(n) requires an arctangent operation, which is difficult to implement
accurately without considerable computational resources. Here’s a scheme for computing Δθ(n) for
use  in  Eq.  (13-111)  without  the  intermediate  θ(n)  phase  computation  (and  its  pesky  arctangent)
[54,55].  We  derive  the  Δθ(n)  computation  algorithm  as  follows,  initially  using  continuous-time
variables based on the following definitions:
(13-112)
The  following  algorithm  is  based  on  the  assumption  that  the  spectrum  of  the  i(t)  +  jq(t)  signal  is
centered at zero Hz. First, we let r(t) = q(t)/i(t) be the signal for which we’re trying to compute the
derivative of its arctangent. The time derivative of tan−1[r(t)], a calculus identity, is
(13-113)
Because d[r(t)]/dt = d[q(t)/i(t)]/dt, we use the calculus identity for the derivative of a ratio to write
(13-114)
Plugging Eq. (13-114)’s result into Eq. (13-113), we have
(13-115)
Replacing r(t) in Eq. (13-115) with q(t)/i(t) yields
(13-116)
We’re getting there. Next we multiply the numerator and denominator of the first ratio in Eq. (13-116)
by i2(t) and replace t with our discrete time variable index n to arrive at our final result of
(13-117)
The  implementation  of  this  algorithm,  where  the  derivatives  of  i(n)  and  q(n)  are  i′(n)  and  q′(n)
respectively,  is  shown  in  Figure  13-61(a).  The  Δϕ(n)  output  sequence  is  used  in  Eq.  (13-111)  to
compute instantaneous frequency.
Figure 13-61 Frequency demodulator without arctangent: (a) standard process; (b) simplified
process.
The Differentiators are tapped-delay line FIR differentiating filters with an odd number of taps. The z
−D delay elements in Figure 13-61(a) are used to time-align the input i(n) and q(n) sequences with the
outputs of the differentiators. The delay is D = (K−1)/2 samples when a K-tap differentiator is used.
In practice, those z−D delays can be obtained by tapping off the center tap of the differentiating filter
as shown in Figure 13-61(b), where the differentiator is an FIR filter having 1,0,−1 as coefficients,
and D = 1 in this case[55]. Such a differentiator is the simple “central-difference differentiator” we
discussed in Chapter 7, and its optimum performance occurs when the input signal is low frequency
relative to the demodulator’s input fs sample rate. Reference [55] reports acceptable results using the
differentiator in Figure 13-61(b), but that’s only true if the complex input signal has a bandwidth no
greater than fs/10.
If  the  i(n)+jq(n)  signal  is  purely  FM  and  hard  limited  such  that  i2(n)+q2(n)  =  Constant,  the
denominator  computations  in  Eq.  (13-117)  need  not  be  performed.  In  this  case,  using  the  1,0,−1
coefficient differentiators, the FM demodulator is simplified to that shown in Figure 13-61(b), where
the Scaling operation is multiplication by the reciprocal of Constant.
Two final things to consider: First, in practice we may want to detect the unusual situation where both
i(n) and q(n) are zero-valued, making the denominator of Eq. (13-117) equal to zero. We should set
Δθ(n) to zero in that case. Second, for real-world noisy signals it may be prudent to apply the Δθ(n)
output to a lowpass filter to reduce unwanted high-frequency noise.
13.23 DC Removal
When  we  digitize  analog  signals  using  an  analog-to-digital  (A/D)  converter,  the  converter’s  output
typically contains some small DC bias; that is, the average of the digitized time samples is not zero.
That DC bias may have come from the original analog signal or from imperfections within the A/D
converter. Another source of DC bias contamination in DSP is when we truncate a discrete sequence
from a B-bit representation to word widths less than B bits. Whatever the source, unwanted DC bias
on a signal can cause problems. When we’re performing spectrum analysis, any DC bias on the signal
shows up in the frequency domain as energy at zero Hz, the X(0) spectral sample. For an N-point FFT
the X(0) spectral value is proportional to N and becomes inconveniently large for large-sized FFTs.
When we plot our spectral magnitudes, the plotting software will accommodate any large X(0) value
and squash down the remainder of the spectrum in which we are more interested.
A  nonzero  DC  bias  level  in  audio  signals  is  particularly  troublesome  because  concatenating  two
audio signals, or switching between two audio signals, results in unpleasant audible clicks. In modern
digital communications systems, a DC bias on quadrature signals degrades system performance and
increases bit error rates. With that said, it’s clear that methods for DC removal are of interest to many
DSP practitioners.
13.23.1 Block-Data DC Removal
If  you’re  processing  in  non-real  time,  and  the  signal  data  is  acquired  in  blocks  (fixed-length
sequences) of block length N, DC removal is straightforward. We merely compute the average of our
N  time  samples  and  subtract  that  average  value  from  each  original  sample  to  yield  a  new  time
sequence whose DC bias will be extremely small.
This  scheme,  although  very  effective,  is  not  compatible  with  continuous-throughput  (real-time)
systems. For real-time systems we’re forced to use filters for DC removal.
13.23.2 Real-Time DC Removal
The author has encountered three proposed filters for DC removal[56–58]; their structures are shown
in Figures 13-62(a), 13-62(b), and 13-62(c).
Figure 13-62 Filters used for DC bias removal.
Ignoring the constant gains of those DC-removal filters, all three filters have identical performance
with the general DC-removal filter structure in Figure 13-62(d) having a z-domain transfer function
of
(13-118)
(It’s not immediately obvious that the filters in Figures 13-62(c) and 13-62(d) are equivalent. You can
verify that equivalency by writing the time-domain difference equations relating the various nodes in
the feedback path of Figure 13-62(c)’s filter. Next, convert those equations to z-transform expressions
and solve for Y(z)/X(z) to yield Eq. (13-118)).
Because  the  DC-removal  filters  can  be  modeled  with  the  general  DC-removal  filter  in  Figure  13-
62(d), we provide the general filter’s frequency magnitude and phase responses in Figures 13-63(a)
and 13-63(b) for α = 0.95. The filter’s pole/zero locations are given in Figure 13-63(c), where a zero
resides  at  z  =  1  providing  infinite  attenuation  at  DC  (zero  Hz)  and  a  pole  at  z  =  α  making  the
magnitude  notch  at  DC  very  sharp.  The  closer  α  is  to  unity,  the  narrower  the  frequency  magnitude
notch centered at zero Hz. Figure 13-63(d) shows the general filter’s unit-sample impulse response.
Figure 13-63 DC-removal filter, α = 0.95: (a) magnitude response; (b) phase response; (c) pole/zero
locations; (d) impulse response.
Figure 13-64 shows the time-domain input/output performance of the general DC-removal filter (with
α  =  0.95)  when  its  input  is  a  sinusoid  suddenly  contaminated  with  a  DC  bias  of  2  beginning  at  the
100th time sample and disappearing at the 200th sample. The DC-removal filter works well.
Figure 13-64 DC-removal filter performance: (a) filter input with sudden DC bias; (b) filter output.
13.23.3 Real-Time DC Removal with Quantization
Because  the  general  DC-removal  filter  has  feedback,  the  y(n)  output  samples  may  require  wider
binary  word  widths  than  those  used  for  the  x(n)  input  samples.  This  could  result  in  overflows  in
fixed-point binary implementations. The scaling factors of (1+α)/2 and K, in Figures 13-62(a) and 13-
62(b), are less than one to minimize the chance of y(n) binary overflow.
In fixed-point hardware the y(n) samples are often truncated to the same word width as the input x(n).
This quantization (by means of truncation) will induce a negative DC bias onto the quantized output
samples, degrading our desired DC removal. When we truncate a binary sample value, by discarding
some of its least significant bits, we induce a negative error in the truncated sample. Fortunately, that
error value is available for us to add to the next unquantized signal sample, increasing its positive DC
bias. When that next sample is truncated, the positive error we’ve added minimizes the negative error
induced by truncation of the next sample.
Figure 13-65(a) shows the addition of a quantizing sigma-delta modulator to the feedback path of the
DC-removal  filter  given  in  Figure 13-62(c).  The  positive  error  induced  by  truncation  quantization
(the  Q  block)  is  delayed  by  one  sample  time  and  fed  back  to  the  quantizer  input.  Because  the
modulator  has  a  noise shaping  property  where  quantization  error  noise  is  shifted  up  in  frequency,
away from zero Hz (DC), the overall DC bias at the output of the filter is minimized[57].
Figure 13-65 Two DC-removal filters using fixed-point quantization to avoid data overflow.
An  equivalent  quantization  noise  shaping  process  can  be  applied  to  a  Direct  Form  I  version  of  the
Figure  13-62(d)  general  DC-removal  filter  as  shown  in  Figure  13-65(b).  Again,  the  positive
quantization  error  is  delayed  by  one  sample  time  and  added  to  the  quantizer  input[59–61].  To
reiterate, the DC-removal filters in Figure 13-65 are used to avoid binary data overflow, by means of
quantization, without the use of scaling multipliers.
Later in this chapter we discuss a DC-removal filter whose frequency response exhibits linear phase.
13.24 Improving Traditional CIC Filters
A  major  design  goal  for  cascaded  integrator-comb  (CIC)  filters,  as  introduced  in  Chapter  10  in
conjunction  with  sample  rate  conversion,  is  to  minimize  their  hardware  power  consumption  by
reducing  data  word  width  and  reducing  data  clock  rates  wherever  possible.  Here  we  introduce  a
clever  trick  that  reduces  CIC  filter  power  consumption  using  nonrecursive  structures,  by  means  of
polynomial factoring, easing the word width growth problem. These nonrecursive structures require
that the sample rate change R be an integer power of two, enhancing computational simplicity through
polyphase  decomposition,  transposed  structures,  simplified  multiplication,  and  substructure
sharing[62–64]. (These processes are not complicated; they merely have fancy names.) Next, we’ll
review  a  nonrecursive  scheme  that  enables  sample  rate  changes  other  than  powers  of  two.  The
following discussion assumes that the reader is familiar with the CIC filter material in Chapter 10.
13.24.1 Nonrecursive CIC Filters
Recall that the structures of 1st-order (M = 1) and 3rd-order (M = 3) CIC decimation filters, having a
comb delay equal to the sample rate change factor R, are those shown in Figure 13-66. As presented
in Chapter 10, the transfer function of an Mth-order decimating CIC filter can be expressed in either a
recursive form or a nonrecursive form, as indicated in Eq. (13-119). (You could, if you wish, use the
geometric  series  discussion  in  Appendix  B  to  show  the  equality  of  the  two  forms  of  the  filter’s
transfer function.)
(13-119)
(13-119′)
Figure 13-66 Recursive decimation CIC filters: (a) 1st-order filter; (b) 3rd-order filter.
Now if the sample rate change factor R is an integer power of two, R = 2K where K is some positive
integer, the Eq. (13-119′) Mth-order nonrecursive polynomial form of Hcic(z) can be factored as
(13-120)
The reward for this factorization is that the CIC filter can then be implemented with K nonrecursive
stages  as  shown  in  Figure  13-67.  This  implementation  eliminates  filter  feedback  loops  with  their
unpleasant  binary  word  width  growth.  The  data  word  width  does  increase  in  this  nonrecursive
structure by M bits for each stage, but the sampling rate is reduced by a factor of two for each stage.
This  nonrecursive  structure  has  been  shown  to  consume  less  power  than  the  Figure  13-66(b)
recursive  implementation  for  filter  orders  greater  than  three  and  decimation/interpolation  factors
larger than eight[64]. Thus the power savings from sample rate reduction are greater than the power
consumption increase due to data word width growth.
Figure 13-67 Multistage Mth-order nonrecursive CIC structure.
Happily,  further  improvements  are  possible  with  each  stage  of  this  nonrecursive  structure[63].  For
example,  assume  we  desire  an  M  =  5th-order  decimating  CIC  for  Stage  1  in  Figure 13-67.  In  that
case, the stage’s transfer function is
(13-121)
The second step in Eq. (13-121), known as polyphase decomposition[65–69], enables a polyphase
implementation having two parallel paths as shown in Figure 13-68. The initial delay element and the
dual decimation-by-two operations are implemented by routing the odd-indexed input samples to FA
′(z),  and  the  even-indexed  samples  to  FB′(z).  Because  we  implement  decimation  by  two  before  the
filtering, the new polyphase components are FA′(z) = 1 + 10z−1 + 5z−2, and FB′(z) = 5 + 10z−1 + z−2
implemented  at  half  the  data  rate  into  the  stage.  (Reducing  data  rates  as  early  as  possible  is  a  key
design goal in the implementation of CIC decimation filters.)
Figure 13-68 Polyphase structure of a single nonrecursive 5th-order CIC stage.
The  FA′(z)  and  FB′(z)  polyphase  components  are  implemented  in  a  tapped-delay  line  fashion  and,
fortunately, further simplifications are possible. Let’s consider the FA′(z) polyphase filter component,
in a tapped-delay line configuration, shown in Figure 13-69(a). The transposed version of this filter
is presented in Figure 13-69(b) with its flipped coefficient sequence. The adder in Figure 13-69(a)
must perform two additions per input data sample, while in the transposed structure no adder need
perform  more  than  one  add  per  data  sample.  Thus  the  transposed  structure  can  operate  at  a  higher
speed.
Figure 13-69 Filter component FA′(z): (a) delay line structure; (b) transposed structure; (c)
simplified multiplication; (d) substructure sharing.
The  next  improvement  uses  simplified  multiplication,  as  shown  in  Figure  13-69(c),  by  means  of
arithmetic  shifts  and  adds.  Thus  a  factor  of  five  is  implemented  as  22  +  1,  eliminating  all
multiplications. Finally, because of the transposed structure, we can use the technique of substructure
sharing in Figure 13-69(d) to reduce the hardware component count. Pretty slick! By the way, these
nonrecursive  filters  are  still  called  cascaded  integrator-comb  filters,  even  though  they  have  no
integrators. Go figure.
Table 13-7 is provided to help the reader avoid computing the polynomial equivalent of several Mth-
order nonrecursive stages, as was performed in Eq. (13-121).
Table 13-7 Expansions of (1 + z−1)M
13.24.2 Nonrecursive Prime-Factor-R CIC Filters
The nonrecursive CIC decimation filters described above have the restriction that the R decimation
factor  must  be  an  integer  power  of  two.  That  constraint  is  loosened  due  to  a  clever  scheme  of
factoring R into a product of prime numbers[70]. This multiple prime-factor-R technique is based on
the process of factoring integer R into the form R = 2p3q5r7s11t ..., where 2, 3, 5, 7, 11 are the prime
numbers.  (This  process  is  called  prime  factorization,  or  prime  decomposition,  and  has  been  of
interest  since  the  days  of  Euclid.)  Then  the  appropriate  number  of  CIC  subfilters  are  cascaded  as
shown in Figure 13-70(a). The fortunate condition is that those Mth-order CIC filters are described
by
(13-122)
Figure 13-70 Multiple prime-factor nonrecursive CIC example: (a) cascaded-stage structure; (b)
2nd-order, R = 90, nonrecursive CIC example.
and so on, enabling nonrecursive implementations.
Due to space constraints, the elegant and arduous derivation of this technique is not given here; but
this process can be illustrated with an example. Assume we desire a 2nd-order (M = 2) CIC filter
with a decimation factor of R = 90. That decimation rate is factored as 90 = (2)(3)(3)(5), so p = 1, q
= 2, and r = 1. Our composite CIC filter is implemented as H2(z)H3(z)H3(z)H5(z) shown in Figure 13-
70(b).
At first glance the many additions of the Figure 13-70(b)  CIC  filter  appear  to  aggravate  the  power
the  reduced  sample  rates  significantly  reduce  power
consumption  of  such  a  filter,  but
requirements[70].  If  one  addition  in  Section  1  of  Figure 13-70(b) consumes P  units  of  power,  then
Section 1 consumes 2P units of power, and each addition in the first portion of Section 2 consumes
P/2 units of power. Each addition in the second portion of Section 2 consumes P/6 of units power,
while each addition in Section 3 consumes P/18 units of power.
We have flexibility here because the subfilters in each section of Figure 13-70(b) can be implemented
recursively  or  nonrecursively,  as  indicated  in  Eq.  (13-122).  In  nonrecursive  implementations  the
polyphase  decomposition,  transposed  structures,  simplified  multiplication,  and  substructure  sharing
schemes can be applied. CIC filter design certainly has come a long way since its introduction in the
early 1980s.
13.25 Smoothing Impulsive Noise
In  practice  we  may  be  required  to  make  precise  measurements  in  the  presence  of  high  noise  or
interference. Without some sort of analog signal conditioning, or digital signal processing, it can be
difficult  to  obtain  stable  and  repeatable  measurements.  This  impulsive-noise  smoothing  trick,
originally  developed  to  detect  microampere  changes  in  milliampere  signals,  describes  a  smoothing
algorithm  that  improves  the  stability  of  precision  measurements  in  the  presence  of  impulsive
noise[71].
Practical  noise-reduction  methods  often  involve  multiple-sample  averaging  (block averaging)  of  a
sequence of measured values, x(n), to compute a sequence of N-sample arithmetic means, M(q). As
such, the block-averaged sequence M(q) is defined by
(13-123)
where the time index of the averaging process is q = 0, 1, 2, 3, etc. When N = 10, for example, for the
first  block  of  data  (q  =  0),  time  samples  x(0) through x(9)  are  averaged  to  compute  M(0).  For  the
second block of data (q = 1), time samples x(10) through x(19) are averaged to compute M(1), and so
on[72].
The  following  impulsive-noise  smoothing  algorithm  processes  a  block  of  time-domain  samples,
obtained  through  periodic  sampling,  and  the  number  of  samples,  N,  may  be  varied  according  to
individual  needs  and  processing  resources.  The  processing  of  a  single  block  of  N  time  samples
proceeds  as  follows:  Collect  N+2  samples  of  x(n),  discard  the  maximum  (most  positive)  and
minimum (most negative) samples to obtain an N-sample block of data, and compute the arithmetic
mean, M(q), of the N samples. Each sample in the block is then compared to the mean. The direction
of  each  sample  relative  to  the  mean  (greater  than,  or  less  than)  is  accumulated,  as  well  as  the
cumulative  magnitude  of  the  deviation  of  the  samples  in  one  direction  (which,  by  definition  of  the
mean, equals that of the other direction). This data is used to compute a correction term that is added
to the mean according to the following formula,
(13-124)
where A(q) is the corrected mean, M(q) is the arithmetic mean (average) from Eq. (13-123), Pos is
the number of samples greater than M(q), Neg is the number of samples less than M(q), and Dtotal is
the sum of deviations from the mean (absolute values and one direction only). Dtotal, then, is the sum
of the differences between the Pos samples and M(q).
For an example, consider a system acquiring ten measured samples of 10, 10, 11, 9, 10, 10, 13, 10,
10,  and  10.  The  mean  is  M  =  10.3,  the  total  number  of  samples  positive  is  Pos  =  2,  and  the  total
number of samples negative is Neg = 8 (so Pos−Neg = −6). The total deviation in either direction from
the mean is 3.4 (using the eight samples less than the mean, (10.3−10) times 7 plus (10.3−9); or using
the  two  samples  greater  than  the  mean,  (13−10.3)  plus  (11−10.3)).  With  Dtotal  =  3.4,  Eq.  (13-124)
yields an improved result of A = 10.096.
The smoothing algorithm’s performance, relative to traditional block averaging, can be illustrated by
example.  Figure  13-71(a)  shows  a  measured  300-sample  x(n)  signal  sequence  comprising  a  step
signal  of  amplitude  one  contaminated  with  random  noise  (with  a  variance  of  0.1)  and  two  large
impulsive-noise spike samples.
Figure 13-71 Noise smoothing for N = 10: (a) input x(n) signal; (b) block average output (white) and
impulsive-noise smoothing algorithm output (solid).
A few meaningful issues regarding this noise smoothing process are:
•  The  block  size  (N)  used  in  the  smoothing  algorithm  can  be  any  integer,  but  for  real-time  fixed
binary-point implementations it’s beneficial to set N equal to an integer power of two. In that case
the compute-intensive division operations in Eqs. (13-123) and (13-124) can be accomplished by
binary arithmetic right shifts to reduce the computational workload.
•  If  there’s  a  possibility  that  more  than  two  large  noise  spikes  are  contained  in  a  block  of  input
samples, then we collect more than N+2 samples of x(n)  and  discard  the  appropriate  number  of
maximum and minimum samples to eliminate the large impulsive noise samples.
•  We  could  forgo  the  Eq. (13-124)  processing  and  merely  perform  Eq.  (13-123)  to  compute  the
mean M(q). In that case, for a given N, the standard deviation of M(q) would be roughly 15 to 20
percent greater than A(q).
As  pointed  out  by  M.  Givens,  impulsive  noise  can  also  be  reduced  by  a  class  of  filters  known  as
median filters[73]. Median filters, not covered in this text, are typically used in noise reduction of
two-dimensional signals (images). However, median filters can also be implemented to process one-
dimensional signals, such as our x(n) signal here, and should be considered in any impulsive-noise
reduction application.
13.26 Efficient Polynomial Evaluation
On  the  off  chance  that  you  didn’t  know,  there  are  two  popular  tricks  used  to  speed  up  polynomial
evaluations  (computations),  known  as  Horner’s Rule  and  Estrin’s Method.  We  illustrate  those  two
techniques below.
13.26.1 Floating-Point Horner’s Rule
Horner’s Rule uses nested operations to reduce the number of multiply operations needed to compute
polynomials.  An  example  of  a  polynomial  computation  is,  for  example,  using  the  following
expression to compute the arctangent of x:
(13-125)
To  see  how  the  computational  workload  of  polynomial  evaluations  can  be  reduced,  consider  the
following kth-order polynomial:
It can be rewritten as
(13-126)
(13-127)
where the “H” subscript means Horner. Using this method to compute polynomials
• reduces the number of necessary multiply operations, and
•  is  straightforward  to  implement  using  programmable  DSP  chips  with  their  multiply  and
accumulate (MAC) architectures.
For example, consider the 5th-order polynomial
(13-128)
Evaluated  in  the  standard  way,  Eq.  (13-128)  would  require  nine  multiplies  and  five  additions,
whereas the Horner version
(13-128′)
requires only five multiplies and five adds when the computations begin with the innermost multiply
and add operations (c5x + c4).
Here are a few examples of polynomials in the Horner format:
(13-129)
(13-130)
(13-131)
By the way, the multiplications and additions cannot be performed in parallel. Because Horner’s Rule
is inherently serial, we need the result of the last multiplication before we can start the next addition,
and that addition result is needed before the follow-on multiplication.
Horner’s  Rule  is  another  of  those  handy  computer  techniques  we  use  whose  origins  are  very  old.
Chinese  mathematicians  described  it  in  the  1200s.  European  mathematicians  (including  William
Horner) rediscovered and publicized it in the early 1800s. However, it seems Sir Isaac Newton also
invented and used it in the 1600s.
13.26.2 Horner’s Rule in Binary Shift Multiplication/Division
The Horner’s Rule method of nested multiplications has special significance for us when we evaluate
polynomials  using  fixed-point  number  formats.  Using  Horner’s  Rule  enables  us  to  minimize  the
truncation quantization error when we use binary right shifts to implement fractional multiplications.
For  example,  if  we  are  using  fractional  fixed-point  numbers  in  the  1.15  format,  as  described  in
Section 12.1.6, and we want to multiply an x(n) sample by 0.3125, we can perform that multiplication
as
(13-132)
Those scaling factors on the right side of Eq. (13-132) can be implemented using binary right shifts by
two and four bits. The larger the right shifts, however, the greater the truncation quantization errors in
this type of fractional multiplication. Using Horner’s Rule, we can implement Eq. (13-132) as
(13-132′)
where  the  maximum  binary  right  shift  is  by  two  bits,  reducing  the  resultant  truncation  quantization
error.
13.26.3 Estrin’s Method
If  your  computing  hardware  is  able  to  perform  multiple  parallel  (simultaneous)  multiply  and
accumulate (MAC) operations, we can increase the computational speed of Horner’s Rule by using
parallel processing in a technique called Estrin’s Method.
Here’s how Estrin’s Method works: Various kth-order polynomials, such as that in Eq. (13-126), can
be evaluated using
The  above  expressions  look  complicated,  but  they’re  really  not.  The  terms  inside  parentheses,
brackets, and curly brackets are nested sub-expressions of the form axq + b—precisely what we need
for  MAC  operations.  For  example,  the  sub-expressions  within  parentheses  can  be  computed
simultaneously with a DSP processor’s parallel MAC operations.
To  illustrate  Estrin’s  Method,  if  your  processing  hardware  can  perform  four  simultaneous  MAC
operations, and assuming value x2 has been previously computed, we can evaluate polynomial f7(x) in
the following three steps:
1. U = (c7x +c6), V = (c5x +c4), W = (c3x +c2), and X = (c1x +c0)
2. Y = (Ux2 +V), Z = (Wx2 +X), x4 = (x2x2 +0)
3. f7(x) = (Yx4 +Z)
The four computations in Step 1 are performed simultaneously. Likewise, the three computations in
Step 2 are performed simultaneously. The final Step 3 is a single MAC operation.
Yes, Estrin’s Method requires multiple processing steps, but this method is able to avoid much of the
inherent  (slow)  serial  processing  dictated  by  Horner’s  Rule.  The  bottom  line  here  is  that  while
Estrin’s Method does not reduce the computational workload (number of multiplies and additions) of
Horner’s  Rule,  it  does  increase  the  computational  speed  of  polynomial  evaluations  by  taking
advantage of modern-day parallel processing hardware architectures.
13.27 Designing Very High-Order FIR Filters
There  are  linear-phase  filtering  applications  wherein  we’re  interested  in  designing  very  high-
performance  (very  narrow  passband  widths,  and/or  very  high  attenuation)  nonrecursive  FIR  filters.
Consider the possibility that you’ve used Eq. (7-34), or some other algorithm, to determine that you
need to implement a 2000-tap linear-phase FIR filter. Then when you try to design such a filter using
your  trusty  Parks-McClellan  Exchange-based  (Remez)  filter  design  software,  you  obtain  unusable
design results. It happens that some software incarnations of the Parks-McClellan Exchange algorithm
have  convergence  problems  (inaccurate  results)  when  the  number  of  filter  taps,  or  filter  order,
exceeds  400  to  500.  There’s  a  slick  way  around  this  high-order  FIR  filter  design  problem  using  a
frequency-domain zero-stuffing technique.†
† I thank my DSP pal Eric Jacobsen, Minister of Algorithms at Abineau Communications, for publicizing this technique.
If  our  FIR  filter  design  software  cannot  generate  FIR  coefficient  sets  whose  lengths  are  in  the
thousands,  then  we  can  design  a  shorter-length  set  of  coefficients  and  interpolate  those  coefficients
(time-domain  impulse  response)  to  whatever  length  we  desire.  Rather  than  use  time-domain
interpolation schemes and account for their inaccuracies, we can simplify the process by performing
time-domain interpolation by means of frequency-domain zero stuffing.
An example of the process is as follows: Assume that we have a signal sampled at a rate of fs = 1000
Hz. We want a lowpass filter whose cutoff frequency is 20 Hz with 60 dB of stopband attenuation.
Compounding  the  problem  are  the  requirements  for  linear  phase  and  removal  of  any  DC  (zero  Hz)
component from the signal. (Those last two requirements preclude using the DC-removal schemes in
Section 13.23.) First, we design a prototype nonrecursive FIR filter having, say, N = 168 coefficients
whose desired frequency response magnitude is shown in Figure 13-72(a); its hp(k) coefficients are
depicted  in  Figure  13-72(b).  Next,  we  compute  a  168-point  DFT  of  the  coefficients  to  obtain  the
frequency-domain samples Hp(m) whose magnitudes are shown in Figure 13-72(c).
Figure 13-72 Prototype FIR filter: (a) magnitude response; (b) hp(k) coefficients; (c) |Hp(m)|
magnitudes of the 168-point DFT of hp(k).
Under  the  assumption  that  our  final  desired  filter  requires  roughly  1600  taps,  we’ll  interpolate  the
hp(k) prototype impulse response by a factor of M = 10. We perform the interpolation by inserting (M
−1)N  zeros  in  the  center  of  the  Hp(m)  frequency-domain  samples,  yielding  a  1680-point  H(m)
frequency-domain sequence whose magnitudes are shown in Figure 13-73(a). Finally, we perform a
1680-point  inverse  DFT  on  H(m)  to  obtain  the  interpolated  h(k)  impulse  response  (coefficients),
shown in Figure 13-73(b), for our desired filter. (The ten-fold compression of the Hp(m)  passband
samples  results  in  a  ten-fold  expansion  of  the  hp(k)  impulse  response  samples.)  The  frequency
magnitude response of our final very high-order FIR filter, over the frequency range of −30 to 30 Hz,
is provided in Figure 13-73(c).
Figure 13-73 Desired FIR filter: (a) magnitude of zero-stuffed Hp(m); (b) interpolated h(k)
coefficients; (c) magnitude of desired frequency response.
With  this  process,  the  prototype  filter’s  hp(k)  coefficients  are  preserved  within  the  interpolated
filter’s  coefficients  if  the  Hp(N/2)  sample  (fs/2)  is  zero.  That  condition  ensures  that  H(m)  exhibits
conjugate symmetry and forces the h(k) coefficients to be real-only.
The design steps for this high-order filter design scheme are:
• With the desired filter requiring MN taps, set the number of prototype filter coefficients, N, to an
integer value small enough so your FIR filter design software provides usable results. The integer
interpolation factor M equals the number of desired taps divided by N.
• Design the N-tap prototype FIR filter accounting for the M-fold frequency compression in the final
filter.  (That  is,  cutoff  frequencies  for  the  prototype  filter  are  M  times  the  desired  final  cutoff
frequencies.)
• Perform an N-point DFT on the prototype filter’s hp(k) coefficients to obtain Hp(m).
• Insert M−1 zero-valued samples just before the Hp(N/2) sample of Hp(m) to obtain the new MN-
point H(m) frequency response.
• Compute the MN-point inverse DFT of H(m), yielding an MN-length interpolated h(k) coefficient
set. (Due to computational errors, discard the imaginary part of h(k), making it real-only.)
• Multiply h(k) by M to compensate for the 1/M amplitude loss induced by interpolation.
•  Test  the  h(k)  coefficient  set  to  determine  its  actual  frequency  response  using  standard  filter
analysis methods. (One method: append thousands of zeros to h(k) and perform a very large FFT
on the expanded sequence.)
An  example  application  of  this  filter  design  is  when  you’re  building  a  high-performance  lowpass
polyphase  filter,  as  discussed  in  Chapter 10.  (The  structures  of  the  high-performance  interpolated
FIR  and  frequency  sampling  lowpass  filters  don’t  permit  their  decomposition  into  polyphase
subfilters for such an application.)
13.28 Time-Domain Interpolation Using the FFT
The  thoughtful  reader  may  have  looked  at  the  above  Section  13.27  FIR  filter  impulse  response
interpolation  scheme  and  wondered,  “If  we  can  interpolate  time-domain  impulse  responses,  we
should  be  able  to  interpolate  time-domain  signals  using  the  same  frequency-domain  zero-stuffing
method.” To quote Rocky Balboa, “This is very true.” In fact, the Section 13.27 interpolation-by-M
process applied to time signals is sometimes called exact interpolation because its performance is
equivalent to using an ideal, infinite-stopband attenuation, time-domain interpolation filter. Let’s see
how this interpolation scheme works.
To establish our notation, let’s say we compute the FFT of an N-point x(n) time sequence to produce
its X(m) frequency-domain samples. Next we stuff (M-1)N zeros in the middle of X(m) to yield the
MN-length Xint(m) frequency samples, where MN  is  an  integer  power  of  two.  Then  we  perform  an
MN-point  inverse  FFT  on  Xint(m)  to  obtain  the  interpolated-by-M xint(n)  times  samples.  Using  this
frequency-domain zero stuffing to implement time-domain signal interpolation involves two important
issues upon which we now focus.
13.28.1 Computing Interpolated Real Signals
The first issue: to ensure the interpolated xint(n) time sequence is real-only, conjugate symmetry must
be  maintained  in  the  zero-stuffed  Xint(m)  frequency  samples.  If  the  X(m)  sequence  has  a  nonzero
sample  at  Xint(N/2),  the  fs/2  frequency  component,  we  must  use  the  following  steps  in  computing
Xint(m) to guarantee conjugate symmetry:
• Perform an N-point FFT on an N-point x(n) time sequence, yielding N frequency samples, X(m).
• Create an MN-point spectral sequence Xint(m) initially set to all zeros.
• Assign Xint(m) = X(m), for 0 ≤ m ≤ (N/2)−1.
•  Assign  both  Xint(N/2)  and  Xint(MN−N/2)  equal  to  X(N/2)/2.  (This  step,  to  maintain  conjugate
symmetry and improve interpolation accuracy, is not so well known[74].)
• Assign Xint(m) = X(q), where MN−(N/2)+1 ≤ m ≤ MN−1, and (N/2)+1 ≤ q ≤ N−1.
•  Compute  the  real  part  of  the  MN-point  inverse  FFT  of  Xint(m),  yielding  the  desired  MN-length
interpolated xint(n) sequence.
•  Finally,  if  desired,  multiply  xint(n)  by  M  to  compensate  for  the  1/M  amplitude  loss  induced  by
interpolation.
Whew! Our mathematical notation makes this signal interpolation scheme look complicated, but it’s
really not so bad. Table 13-8 shows the frequency-domain Xint(m) sample assignments, where 0 ≤ m ≤
15, to interpolate an N = 8-point x(n) sequence by a factor of M = 2.
Table 13-8 Xint(m) Assignments for Interpolation by Two
One of the nice properties of the above algorithm is that every Mth xint(n) sample coincides with the
original x(n) samples. In practice, due to our finite-precision computing, the imaginary parts of our
final xint(n) may have small nonzero values. As such, we take xint(n) to be the real part of the inverse
FFT of Xint(m).
Here’s  the  second  issue  regarding  time-domain  real  signal  interpolation.  This  technique  of
interpolation  using  FFT  zero  stuffing  only  provides  acceptable  results  when  the  original  x(n)  time
sequence has a spectrum having negligible spectral energy in the vicinity of ±fs/2, as shown in Figure
13-74  for  lowpass  and  bandpass  signals.  By  negligible  we  mean  spectral  magnitudes  that  are,  say,
below a discrete signal’s quantization noise background spectral level.
Figure 13-74 Spectral restrictions for interpolation using the FFT: (a) lowpass signal case; (b)
bandpass signal case.
An  example  of  violating  the  above  spectral  restriction  is  when  x(n)  is  a  sinusoidal  sequence
containing  a  noninteger  number  of  cycles.  That  signal’s  positive-frequency  spectrum  will  have
nonzero  spectral  energy  extending  from  zero  Hz  to  fs/2  Hz  caused  by  spectral  leakage.  Trying  to
interpolate such an x(n) using this FFT zero-stuffing scheme will yield an interpolated time sequence
with unacceptably high amplitude errors at the beginning and end of the interpolated sequence.
With  the  advent  of  fast  hardware  DSP  chips  and  pipelined  FFT  techniques,  the  above  time-domain
interpolation  algorithm  may  be  viable  for  a  number  of  applications,  such  as  computing  selectable
sample  rate  time  sequences  of  a  test  signal  that  has  a  fixed  spectral  envelope  shape;  providing
interpolation, by selectable factors, of signals that were filtered in the frequency domain using the fast
convolution method (Section 13.10); or digital image resampling. One scenario to consider is using
the efficient 2N-Point Real FFT technique, described in Section 13.5.2, to compute the forward FFT
of the real-valued x(n). Of course, the prudent engineer would conduct a literature search to see what
algorithms are available for efficiently performing inverse FFTs when many of the frequency-domain
samples are zeros.
13.28.2 Computing Interpolated Analytic Signals
We  can  use  the  frequency-domain  zero-stuffing  scheme  to  generate  an  interpolated-by-M  analytic
(complex-valued) time signal based upon the real N-point time sequence x(n), if N is even[75]. The
process is as follows:
• Perform an N-point FFT on an N-point real xr(n) time sequence, yielding N frequency samples,
Xr(m).
• Create an MN-point spectral sequence Xint(m) initially set to all zeros, where MN  is  an  integer
power of two.
• Assign Xint(0) = Xr(0), and Xint(N/2) = Xr(N/2).
• Assign Xint(m) = 2Xr(m), for 1 ≤ m ≤ = (N/2)−1.
•  Compute  the  MN-point  inverse  FFT  of  Xint(m),  yielding  the  desired  MN-length  interpolated
analytic (complex) xc,int(n) sequence.
• Finally, if desired, multiply xc,int(n) by M to compensate for the 1/M amplitude loss induced by
interpolation.
To minimize the interpolation error in the complex xc,int(n) sequence, the original xr(n) sequence must
have  negligible  spectral  energy  in  the  vicinity  of  ±fs/2,  as  described  earlier  for  real-valued
interpolation.
13.29 Frequency Translation Using Decimation
In this section we show tricks for implementing multiplierless frequency translation of both real and
complex signals using simple decimation.
13.29.1 Translation of Real Signals Using Decimation
We can frequency translate a real bandpass signal toward zero Hz, converting it to a lowpass signal,
without the need for mixing multipliers. We do this by performing decimation by an integer factor D
as  shown  in  Figure 13-75(a).  If  the  bandpass  filter  provides  an  output  signal  of  bandwidth  B  Hz,
located as shown in Figures 13-75(b) and 13-75(d) where k is a positive integer, decimation by D
will yield lowpass signals whose spectra are shown in Figures 13-75(c) and 13-75(e), depending on
whether  integer  k  is  odd  or  even.  Take  care  to  notice  the  inverted  spectra  in  Figure  13-75(e).  To
avoid decimated-output aliasing errors, we must satisfy the Nyquist criterion and ensure that xBP(n)’s
bandwidth B is not greater than fs/(2D).
Figure 13-75 Real and complex bandpass signal translation using decimation by D.
13.29.2 Translation of Complex Signals Using Decimation
It’s  possible  to  frequency  translate  a  complex  bandpass  signal,  without  the  need  for  mixing
multipliers, so that a spectral replication is centered at zero Hz. The process we’re describing here is
called complex down-conversion. The left side of Figure 13-75(f) shows the spectrum of a complex
baseband  signal  whose  |XCB(m)|  spectral  magnitude  contains  only  positive-frequency  spectral
components.
If we individually decimate the real and imaginary parts of the complex time sequence xCB(n), whose
spectrum  is  XCB(m),  by  D,  the  resulting  complex  sequence  will  have  a  spectral  image  centered
exactly  at  zero  Hz  as  shown  by  |X′CB(m)|  in  Figure 13-75(f).  The  key  stipulation  here,  as  you  may
have  guessed,  is  that  the  original  pre-decimated  |XCB(m)|  spectral  energy  must  be  centered  at  an
integer multiple of fs/D.
13.30 Automatic Gain Control (AGC)
Since the early days of vacuum tube radios, circuits were needed to automatically adjust a receiver’s
gain,  as  an  input  signal  varied  in  amplitude,  to  maintain  a  (relatively)  constant  output  signal  level.
These  feedback  mechanisms,  called  automatic  gain  control  (AGC)  circuits,  are  an  important
component  of  modern  analog  and  digital  communications  receivers.  Figure  13-76(a)  illustrates  a
simple  digital  AGC  process[76,77].  Its  operation  is  straightforward:  The  output  signal  power  is
sampled and compared to a reference level R (the desired output amplitude rms level). If the output
signal level is too high (low), a negative (positive) signal is fed back, reducing (increasing) the gain.
The  control  parameter  α  regulates  the  amplitude  of  the  feedback  signal  and  is  used  to  control  the
AGC’s time constant (how rapidly gain changes take effect).
Figure 13-76 AGC process: (a) linear AGC circuit; (b) example input x(n) with amplitude
fluctuations; (c) y(n) output for α = 0.01 and R = 1.
Given  an  input  signal  x(n)  in  Figure  13-76(b)  whose  amplitude  envelope  is  fluctuating,  the  AGC
structure provides the relatively constant amplitude y(n) output shown in Figure 13-76(c).
We called Figure 13-76(a) a “simple AGC process,” but AGC is not all that simple. The process is a
nonlinear,  time-varying,  signal-dependent  feedback  system.  As  such,  it’s  highly  resistant  to  normal
time-domain or z-domain analysis. This is why AGC analysis is empirical rather than mathematical
and explains why there’s so little discussion of AGC in the DSP literature.
Depending on the nature of x(n), the feedback signal may fluctuate rapidly and the feedback loop will
attempt  to  adjust  the  system  gain  too  often.  This  will  cause  a  mild  AM  modulation  effect,  inducing
low-level harmonics in the y(n) output. That problem can be minimized by inserting a simple lowpass
filter in the feedback loop just before, or just after, the R adder. But such filtering does not remedy the
circuit’s  main  drawback.  The  time  constant  (attack  time)  of  this  AGC  scheme  is  input  signal  level
dependent  and  is  different  depending  on  whether  the  x(n)  is  increasing  or  decreasing.  These
properties  drastically  reduce  our  desired  control  over  the  system’s  time  constant.  To  solve  this
problem, we follow the lead of venerable radio AGC designs and enter the logarithmic domain.
We can obtain complete control of the AGC’s time constant, and increase our AGC’s dynamic range,
by using logarithms as shown in Figure 13-77(a). As is typical in practice, this log AGC process has
a lowpass filter (LPF) to eliminate too-rapid gain changes[78].  That  filter  can  be  a  simple  moving
average filter, a cascaded integrator-comb (CIC) filter, or a more traditional lowpass filter having a
sin(x)/x impulse response.
Figure 13-77 AGC process: (a) logarithmic AGC circuit; (b) y(n) output for α = 0.01 and R = 1.
For  the  logarithmic  AGC  scheme,  the  feedback  loop’s  time  constant  is  dependent  solely  on  α  and
independent of the input signal level, as can be seen in Figure 13-77(b) when the x(n) input is that in
Figure 13-76(b). The Log and Antilog operations can be implemented as log2(x) and 2x, respectively.
13.31 Approximate Envelope Detection
In  this  section,  we  present  a  crude  (but  simple  to  implement)  complex  signal  envelope  detection
scheme.  By  “envelope  detection”  we  mean  estimating  the  instantaneous  magnitude  of  a  complex
signal xc(n). The process is straightforward: we sum the absolute values of a complex signal’s real
and imaginary parts and apply that sum to a simple 1st-order lowpass IIR filter to obtain an envelope
signal E(n) as shown in Figure 13-78(a). The filter’s feedback coefficient α is in the range of 0 to 1.
(That  lowpass  filter  is  our  exponential  averager  discussed  in  Section 11.6,  which  some  DSP  folks
call a leaky integrator.) The E(n) sequence is proportional to the desired instantaneous magnitude of
xc(n), or
(13-133)
Figure 13-78 Envelope detection: (a) block diagram; (b) |xr(n)|+|xi(n)| adder output, and E(n) for α =
0.4; (c) E(n) for α = 0.2 and α = 0.05.
To  gauge  the  envelope  detector’s  performance,  consider  a  sampled  version  of  an  amplitude-
modulated sinusoid such as the xr(n) in Figure 9-7(a) from which a sampled analytic (complex) xc(n)
can  be  generated.  If  xc(n)  is  applied  to  our  envelope  detection  process,  the  processing  results  are
shown  in  Figures  13-78(b)  and  13-78(c),  where  the  solid  curves  represent  E(n)  and  the  dashed
curves are the true magnitude of xc(n). Notice how the amount of smoothing of the E(n) fluctuations
depends on the value of α.
If the scaling coefficient α/2 can take the form
(13-133′)
where K is a positive integer, then we can eliminate the multipliers in Figure 13-78(a). If we satisfy
Eq. (13-133′), the multiply by α/2 can be replaced by two binary right shifts and a subtract operation,
and the multiply by (1−α) can be replaced by a single binary right-shift operation. This situation gives
us a multiplierless envelope detector.
Sequence xr(n) must be used to generate a complex analytic xc(n) sequence (using one of the methods
discussed in Sections 9.4 and 9.5)  upon  which  this  envelope  detector  scheme  can  be  applied.  The
advantage  of  this  envelope  detection  process  is  that,  of  course,  no  squaring  or  square  root
computations  in  Eq.  (13-133),  nor  the  |xr(n)|  and  |xi(n)|  comparisons  in  the  vector  magnitude
approximations in Section 13.2, need be performed.
Whether this envelope approximation technique yields sufficiently accurate results is for the user to
decide.  Its  accuracy  may  be  below  the  requirements  of  most  AM  (amplitude  modulation)  detection
requirements,  but  the  process  may  well  be  useful  for  estimating  signal  magnitude  in  automatic  gain
control (AGC) or energy detection applications.
13.32 A Quadrature Oscillator
Here we present a well-behaved digital quadrature oscillator, whose output is yi(n) + jyq(n), having
the structure shown in Figure 13-79(a). If you’re new to digital oscillators, that structure looks a little
complicated but it’s really not so bad. If you look carefully, you see the computations are
Figure 13-79 Quadrature oscillators: (a) standard structure; (b) structure with AGC.
and
(13-134)
(13-134′)
Those computations are merely the rectangular form of multiplying the previous complex output by a
complex exponential ejθ as
(13-135)
So the theory of operation is simple. Each new complex output sample is the previous output sample
rotated  by  θ  radians,  where  θ  is  2πft/fs with ft  and  fs  being  the  oscillator  tuning  frequency  and  the
sample rate, respectively, in Hz.
To  start  the  oscillator,  we  set  the  initial  conditions  of  yi(n−1)  =  1  and  yq(n−1)  =  0  and  repeatedly
compute  new  outputs,  as  time  index  n  advances,  using  Eq.  (13-134).  This  oscillator  is  called  a
coupled quadrature oscillator because both of its previous outputs are used to compute each new in-
phase  and  each  new  quadrature  output.  It’s  a  useful  oscillator  because  the  full  range  of  tuning
frequencies  is  available  (from  nearly  zero  Hz  up  to  roughly  fs/2),  and  its  outputs  are  equal  in
amplitude, unlike some other quadrature oscillator structures[79]. The tough part, however, is making
this oscillator stable in fixed-point arithmetic implementations.
Depending  on  the  binary  word  widths,  and  the  value  θ,  the  output  amplitudes  can  either  grow  or
decay as time increases because it’s not possible to represent ejθ having a magnitude of exactly one,
over the full range of θ, using fixed-point number formats. The solution to amplitude variations is to
compute yi′(n−1)  and  yq′(n−1)  and  multiply  those  samples  by  an  instantaneous  gain  factor  G(n)  as
shown in Figure 13-79(b). The trick here is how to compute the gain samples G(n).
We can use a linear automatic gain control (AGC) method, described in Section 13.30, as shown in
Figure 13-80(a) where α is a small value, say, α = 0.01. The value R is the desired rms value of the
oscillator outputs. This AGC method greatly enhances the stability of our oscillator. However, there’s
a  computationally  simpler  AGC  scheme  for  our  oscillator  that  can  be  developed  using  the  Taylor
series approximation we learned in school. Here’s how.
Figure 13-80 AGC schemes: (a) linear AGC; (b) simplified AGC.
Using an approach similar to reference [80], we can define the desired gain as
(13-136)
This is the desired output signal magnitude Mdes over the actual output magnitude Mact. We can also
represent the gain using power as
(13-137)
where the constant Pdes is the desired output signal power and Pact is the actual output power. The
right side of Eq. (13-137) shows Pact replaced by the desired power Pdes plus an error component E,
and that’s the ratio we’ll compute. To avoid square root computations and because the error E will be
small, we’ll approximate that ratio with a two-term Taylor series expansion about E = 0 using
(13-138)
Computing the Taylor series’ coefficients to be a0 = 1 and a1 = −1/2Pdes, and recalling that E = Pact
−Pdes, we estimate the instantaneous gain as
(13-139)
If  we  let  the  quadrature  output  peak  amplitudes  equal
division in Eq. (13-139), obtaining
,  Pdes  equals  1/2  and  we  eliminate  the
(13-140)
The simplified structure of the G(n) computation is shown in Figure 13-80(b).
As for practical issues, to avoid gain values greater than one (for those fixed-point fractional number
systems  that  don’t  allow  numbers  ≥1),  we  use  the  clever  recommendation  from  reference  [79]  of
multiplying  by  G(n)/2  and  doubling  the  products  in  Figure  13-79(b).  Reference  [80]  recommends
using  rounding,  instead  of  truncation,  for  all  intermediate  computations  to  improve  output  spectral
purity.  Rounding  also  provides  a  slight  improvement  in  tuning  frequency  control.  Because  this
oscillator  is  guaranteed  stable,  and  can  be  dynamically  tuned,  it’s  definitely  worth  considering  for
real-valued as well as quadrature oscillator applications[79].
13.33 Specialized Exponential Averaging
In Chapter 11 we discussed the behavior and utility of using an exponential averaging lowpass filter,
also called a leaky integrator, to reduce noise fluctuations that contaminate constant-amplitude signal
measurements.  In  this  section  we  present  three  specialized  exponential  averaging  techniques  in  the
form of
• single-multiply averaging,
• multiplier-free averaging, and
• dual-mode averaging.
13.33.1 Single-Multiply Exponential Averaging
This  DSP  trick  shows  how  to  reduce  the  computational  workload  of  the  standard  exponential
averager[81]. An exponential averager’s difference equation is
(13-141)
where  α  is  a  constant  called  the  averager’s  weighting factor,  in  the  range  0  <  α  <  1.  The  process
requires two multiplies per y(n) output sample as shown in Figure 13-81(a).
Figure 13-81 Exponential averaging: (a) standard network; (b) single-multiply network; (c)
multiplierless network; (d) possible values for α; (e) dual-mode averaging.
We can rearrange Eq. (13-141) to the form
(13-141′)
which eliminates one of the averager’s multiplies, at the expense of an additional adder, giving us a
single-multiply exponential averager shown in Figure 13-81(b). This neat single-multiply exponential
averager  maintains  the  DC  (zero  Hz)  gain  of  unity  exhibited  by  the  traditional  two-multiply
exponential averager in Figure 13-81(a).
13.33.2 Multiplier-Free Exponential Averaging
It is possible to eliminate the multiplier in Figure 13-81(b) if we place restrictions on the permissible
values  of  α.  For  example,  if  α  =  0.125  =  1/8,  then  the  output  of  the  multiplier  is  merely  the
multiplier’s input sample shifted right by three bits.
On the other hand, if α takes the form
(13-142)
where L = 0, 1, 2, 3, ... , and M = 1, 2, 3, ... , we can replace the multiplication by α in Figure 13-
81(b) with two binary right shifts and a subtract operation as shown in Figure 13-81(c). In that figure
the “BRS,L” block means an arithmetic, or hardwired, Binary Right Shift by L bits.
For  example,  if  L  =  2  and  M  =  5,  then  from  Eq.  (13-142),  α  =  0.2188.  The  sequence  w(n)  =
0.2188u(n) = (1/4 − 1/32)u(n) is computed by subtracting u(n) shifted right by M = 5 bits from u(n)
shifted right by L = 2 bits.
The tick marks in Figure 13-81(d) show the possible values for the weighting factor α over the range
of 0 ″ L ″ 5, where for each L, M is in the range L+1 ″ M ″ 6 in Eq. (13-142). That figure tells us that
we have a reasonable selection of α values for our noise-reduction filtering applications.
The point is, for fixed-point implementation of exponential averaging, check to see if your desired α
weighting factor can be represented by the difference of various reciprocals of integer powers of two.
If so, then binary word shifting enables us to implement a multiplierless exponential averager.
13.33.3 Dual-Mode Averaging
Here’s a clever exponential averaging scheme that blends both the quick time response of a moving
averager and the noise-reduction control of an exponential averager.† The structure of this dual-mode
averager is depicted in Figure 13-81(e). The averager operates as follows: The switch remains open
for K input samples after which the y(n) output is equal to the K-point average of the x(n) input. Just
prior to the arrival of the K+1 input sample the switch closes, converting the moving average filter to
an exponential averager, giving us control over the filter’s noise-reduction properties as described in
Section 11.6.
† We thank DSP guru Fred Harris for recommending this dual-mode averager.
Of course, K does not have to be an integer. In this case we can still implement dual-mode averaging
by closing the switch just prior to the arrival of the x(
 means the integer
part of K. After the Kth input sample has arrived, the averager’s gain at zero Hz (DC gain) is unity. As
discussed in the previous section, if the weighting factor 1/K can be represented by the difference of
various  reciprocals  of  integer  powers  of  two,  then  we  can  implement  a  multiplierless  dual-mode
noise-reduction filter.
) input sample, where
13.34 Filtering Narrowband Noise Using Filter Nulls
Here we present two filter design tricks that take advantage of the frequency-domain magnitude nulls
of simple FIR filters. These schemes are particularly useful when used in AM and FM demodulation
systems.
The  first  example  uses  a  simple  3-tap  nonrecursive  FIR  filter.  Such  a  filter  can  be  both
computationally  efficient,  and  useful,  for  narrowband-noise  reduction.  Here’s  how.  Think  about  the
x(n) time-domain signal, contaminated with high-frequency noise, shown in Figure 13-82(a) with its
spectrum provided in Figure 13-82(b). The sample rate of the signal is 10 kHz. Let’s assume we want
to  recover  the  low-frequency  signal  of  interest  (centered  at  zero  Hz)  without  inducing  phase
distortion, and we need to attenuate the narrowband high-frequency noise, centered at 4.1 kHz, by at
least 50 dB. Our solution, of course, is to pass our noisy signal through a linear-phase lowpass FIR
filter whose frequency magnitude response is indicated as the dashed curve in Figure 13-82(b).
Figure 13-82 A noisy x(n): (a) time signal; (b) its X(f) spectral magnitude.
Seeking  the  most  computationally  efficient  filter  possible,  let’s  say  we’re  clever  and  recall  the
special characteristic of a half-band FIR filter in which roughly half its coefficients are zero-valued.
So  we  could  design  a  9-tap  half-band  FIR  filter,  having  only  five  nonzero-valued  coefficients,  and
that  solution  would  be  acceptable.  Here’s  where  our  trick  comes  in;  we  decide  to  use  the  linear-
phase 3-tap FIR filter shown in Figure 13-83(a) with its single non-unity coefficient h1.
Figure 13-83 A 3-tap FIR filter: (a) filter structure; (b) pole locations; (c) frequency magnitude
response.
If |h1| ≤ 2, the 3-tap FIR filter’s transfer function will have two zeros on the z-plane at angles ±ωn as
shown  in  Figure 13-83(b).  The  frequency  magnitude  response  of  the  filter  is  shown  in  Figure  13-
83(c). (Here, the normalized frequency axis value of π corresponds to a continuous-time frequency of
half the sample rate, fs/2.) Our goal, then, is to choose the h1 coefficient such that the filter’s positive-
frequency  magnitude  null  lands  right  on  the  4.1  kHz  center  frequency  of  the  narrowband  noise  in
Figure 13-82(b).
Our  3-tap  filter  design  problem  is  easy  because  we  have  an  expression  for  the  h1  coefficient  as  a
function of the desired fn null frequency in Hz. The h1 coefficient value is
(13-143)
With fn = 4.1 kHz and fs = 10 kHz, our h1 coefficient is 1.69. (The derivation of Eq. (13-143) was left
as a homework problem in Chapter 5.) The H(f) frequency magnitude response of the h1 = 1.69 filter
is shown as the dotted curve in Figure 13-84(a). The Y(f) spectrum of the filter’s output is shown in
Figure 13-84(b) where the narrowband noise has been attenuated by roughly 54 dB. (Recall that the
noise  magnitudes  in  the  original  X(f)  spectrum  were  approximately  12  dB  above  the  signal’s  peak
magnitude in Figure 13-82(b).) The filter’s time-domain y(n) output signal, our signal of interest, is
shown in Figure 13-84(c). It’s instructive to compare that output signal to the filter’s x(n) input signal
in Figure 13-82(a).
Figure 13-84 Three-tap filter performance: (a) |H(f)| response; (b) filter output spectrum; (c) filter
time-domain output signal.
So we solved our narrowband noise filtering problem with a linear-phase FIR filter requiring only
two additions and one multiply per filter output sample. Neat, huh?
Our  second  example  of  this  filter  design  approach  that  takes  advantage  of  the  frequency-domain
magnitude nulls of simple FIR filters involves the attenuation of multiple narrowband noise spectral
components  whose  center  frequencies  are  harmonically  related  in  a  frequency  shift  keying  (FSK)
demodulation system[82]. Think about a signal of interest centered at 455 kHz as shown in Figure 13-
85(a). That signal, sampled at fs = 10 MHz, is contaminated with unwanted high-amplitude DC (zero
Hz) bias noise and narrowband spectral noise components at multiples of 455 kHz. Removing the DC
bias, whose magnitude is 0 dB in Figure 13-85(a), and extracting the signal of interest from the noise
appears to require some sort of bandpass filter centered at 455 kHz.
Figure 13-85 Harmonic noise example: (a) |X(f)| spectrum; (b) filter output spectrum.
However, the trick is to use a standard FIR comb filter to remove the unwanted DC bias and attenuate
the harmonic noise components. A comb filter is shown in Figure 13-86(a) where the z−N operation is
merely a delay of N samples. The |H(f)| frequency magnitude response of an N = 8, for example, comb
filter is provided in Figure 13-86(b) where fs is the sample rate.
Figure 13-86 Standard N-delay FIR comb filter: (a) filter structure; (b) frequency magnitude response
when N = 8.
For N-delay comb filter design purposes, the following two equations give us the frequency locations
of the magnitude nulls (fnull) and magnitude peaks (fpeak) in the filter’s |H(f)|,
(13-144)
(13-144′)
where
N so long as N is larger than one.
 means the integer part of X. These fnull and fpeak expressions are valid for both odd and even
For this noise-reduction problem, we need a comb filter that provides a magnitude null at zero Hz and
a magnitude peak at 455 kHz. Rearranging Eq. (13-144′) to find a candidate value for the comb delay
N for k = 0, we have
(13-145)
so we select N  to  be  11.  The  filter’s  output  spectrum,  when  N  =  11,  is  shown  in  Figure  13-85(b).
There  we  see  the  dramatic  reduction  in  the  unwanted  DC  bias  as  well  as  the  narrowband  noise
located  at  910  kHz.  (For  reference  purposes,  we’ve  included  the  N  =  11  comb  filter’s  magnitude
response in Figure 13-85(b).)
So in this example we simplified our overall narrowband noise filtering problem using a linear-phase
FIR  comb  filter  requiring  only  one  addition  per  filter  output  sample.  In  practice  the  comb  filter  is
followed  by  a  low-order  lowpass  filter,  whose  stopband  would  begin  in  the  vicinity  of  1365  kHz.
That follow-on filter will have a significantly reduced computational workload compared to the case
where the pre-filtering by the simple comb filter was not performed.
For completeness, we must mention here that an alternate comb filter can be built using the network
in Figure 13-87(a) where addition is performed as opposed to the subtraction in Figure 13-86(a).
Figure 13-87 Alternate N-delay FIR comb filter: (a) filter structure; (b) |Halt(f)| frequency magnitude
response when N = 8.
For  the  alternate  comb  filter  in  Figure 13-87(a)  the  following  two  equations  give  us  the  frequency
locations of the magnitude nulls (fnull,alt) and magnitude peaks (fpeak,alt) in this N-delay comb filter’s
|Halt(f)|,
(13-146)
(13-146′)
where
because it passes low-frequency signals due to a frequency magnitude peak at zero Hz (DC).
 means the integer part of X. This alternate comb filter gives us a bit of design flexibility
13.35 Efficient Computation of Signal Variance
In  this  section  we  show  how  to  reduce  the  computational  workload,  and  required  data  storage,  in
computing  the  unbiased  and  biased  variances  of  a  signal  sequence.  (Definitions  of  biased  and
unbiased variances can be found in Appendix D.)
We start our discussion with the traditional definition of the unbiased variance of x(n), a sequence of
N samples, written as
(13-147)
where xave  is  the  average  of  the  N-length x(n) sequence. Because N  is  a  constant,  we  can  treat  the
divide by N  needed  to  compute  xave,  and  the  above  divide  by  (N−1),  as  multiplies  by  reciprocals,
allowing us to say that Eq. (13-147) requires 3N−2 addition and N+2 multiply operations. As it turns
out,  we  can  obtain  an  equivalent  expression  for  Varunbiased  that  has  a  reduced  number  of  arithmetic
operations[83]. Here’s how.
First, we square the bracketed term in the summation in Eq. (13-147) and write
(13-148)
Because the center summation in Eq. (13-148) is
we can rewrite Varunbiased as
(13-148′)
Next, we arrive at our desired expression by combining terms and write Varunbiased as
(13-149)
The  efficient  Eq.  (13-149)  requires  only  2N−1  addition  and  N+4  multiply  operations.  So  at  the
expense of two extra multiplies, we’ve reduced the number of additions needed to compute Varunbiased
by roughly N relative to Eq. (13-147).
There is a second advantage in using Eq. (13-149) instead of Eq. (13-147) in computing the variance
of N incoming x(n) samples. When using Eq. (13-147) to compute Varunbiased, we first compute xave
and  must  retain,  in  memory,  the  N-length  x(n)  sequence  in  order  to  compute  the  [x(n)  −  xave]2
sequence. When using Eq. (13-149) to compute Varunbiased, we can simultaneously accumulate (sum)
the N incoming x(n) samples and accumulate the N computed x(n)2 samples without having to keep
past  x(n)  samples  in  memory.  Thus  Eq.  (13-149)  reduces  the  amount  of  data  storage  needed  to
compute Varunbiased.
The traditional definition for the biased variance of N x(n) samples is written as
(13-150)
Using a derivation similar to how we arrived at Eq. (13-149), we can write an efficient expression
for computing a biased variance as
(13-150′)
Equation (13-150′) requires 2N−1 addition and N+2 multiply operations. Here again, we’ve reduced
the  number  of  additions  needed  to  compute  Varbiased  by  roughly  N  and  reduced  the  necessary  data
storage,  relative  to  Eq.  (13-150).  In  the  next  section  we  discuss  the  hardware  implementation  of
variance computations with a focus on real-time processing.
13.36 Real-time Computation of Signal Averages and Variances
In  this  section  we  present  techniques  for  the  efficient  computation  of  real-time  estimates  of  signal
average and variance. By “real-time” we mean a continuing sequence of statistical estimates will be
generated,  in  time,  as  a  continuing  sequence  of  input  signal  samples  is  applied  to  our  processing
networks.
13.36.1 Computing Moving Averages and Variances
Figure 13-88(a)  shows  a  5-point  real-time  recursive  moving  averager  that  we  analyzed  in  Section
11.5.  For  the  reasons  discussed  in  Section  11.5  the  recursive  moving  averager  is  the  most
computationally efficient method for computing moving averages.
Figure 13-88 Real-time recursive N-point moving averager: (a) standard N = 5 implementation; (b)
alternate implementation; (c) general recursive depiction.
Figure 13-88(b) shows an alternate, but equivalent, recursive moving averager where the integrator
now  precedes  the  1/5  multiplication.  In  this  alternate  arrangement  the  binary  register  holding  the
integrator’s  accumulation  results  must  be  large  enough  to  accommodate  values  in  the  range  of  five
(number of unit-delay registers) times the true average of the most recent N x(n) input samples.
In Figure 13-88(c) we redraw the alternate recursive moving averager in order to show the network
of a general N-point recursive moving averager. There we use a single z−N delay element symbol to
represent an N-length delay line. In that figure we show a network inside the dashed-line box, which
we’ll use later for other statistical computations, called a recursive running sum (RRS).
Focusing  now  on  the  second  topic  of  this  section,  there  is  a  way  to  estimate  the  real-time  N-point
moving  unbiased  variance  of  a  signal,  xvar,unbiased(n)[84].  (A  definition  of  unbiased  variance  is
provided in Appendix D.) To see how, we start with the expression for the unbiased variance of N
time samples, Eq. (13-149) from the previous section, rewritten here as
(13-151)
where xave(n) is the average of the most recent N input x(n) samples. The limits on the summation in
Eq. (13-151) are such that we’re summing a sliding-in-time block of N samples of x(n)2.
The implementation of Eq. (13-151) is shown in Figure 13-89(a) where the process uses two N-point
RRS networks from Figure 13-88(c) to compute the N-point moving unbiased variance xvar,unbiased(n)
and the xave(n) N-point moving average of x(n)[83,85]. Note that the xvar,unbiased(n) and xave(n) outputs
are not valid until the N-stage delay lines are filled with input data.
Figure 13-89 Real-time N-point moving variance networks.
To estimate the real-time N-point moving biased variance of a signal, xvar,biased(n), we compute
(13-152)
using the network shown in Figure 13-89(b).
From a practical standpoint, in fixed-point systems, note that the binary word width of the upper RRS
delay lines, in Figure 13-89, must be twice as wide as the lower RRS delay lines.
The above real-time fixed-length moving average and moving variance networks require data memory
to  implement  their  N-point  delay  lines.  The  following  section  describes  techniques  for  estimating
cumulative averages and variances with reduced data memory requirements.
13.36.2 Computing Exponential Moving Average and Variance
An alternate method to generate estimates of both the real-time moving average and real-time moving
variance of a signal is to use the exponential averager that we discussed in Section 11.6, shown in
Figure 13-90(a).  The  coefficient  α  is  the  exponential  averager’s  weighting factor  that  controls  the
amount of averaging that takes place at the output of the network.
Figure 13-90 Exponential moving average and exponential moving variance: (a) standard exponential
averaging network; (b) full structure.
To generate our desired exponential moving average and exponential moving variance, we use two
independent exponential averaging (EA) networks as shown in Figure 13-90(b). The two weighting
factors, α1 and α2, are constants in the range of zero to one.
The process in Figure 13-90(b) has several attractive properties. The α1 and α2 coefficients permit
control over the averaging behavior of the process; and the Figure 13-90(b) process requires fewer
computations  per  output  sample,  and  reduced  delay-line  element  (data  memory)  requirements,
relative to the networks in Figure 13-89.
13.37 Building Hilbert Transformers from Half-band Filters
This section discusses two techniques for obtaining the coefficients of a Hilbert transformer from the
coefficients  of  an  N-point  nonrecursive  FIR  half-band  filter[86,87].  The  first  scheme  is  useful  for
someone  who  needs  to  design  a  Hilbert  transformer  when  only  generic  lowpass  FIR  filter  design
software is available. The second scheme is useful for those unfortunate folks who have no FIR filter
design software at hand but have available the coefficients of a half-band filter.
13.37.1 Half-band Filter Frequency Translation
We can design a Hilbert transformer by first designing an N-tap half-band filter using our favorite FIR
filter design software, with the restriction that N+1 is an integer multiple of four. Let’s call the half-
band filter’s coefficients hhb(k), where the coefficients’ index variable k is 0, 1, 2, ..., N−1. Next we
obtain the Hilbert transformer’s hhilb(k) coefficients using
(13-153)
Figure 13-91(a)  shows  the  coefficients  of  a  simple  7-tap  half-band  filter  whose  DC  gain  is  unity.
Figure 13-91(b) shows the Hilbert transformer’s hhilb(k) coefficients obtained from Eq. (13-153). The
network  using  those  hhilb(k)  coefficients  to  generate  a  complex  (analytic)  xc(n)  =  xI(n)+jxQ(n)
sequence from an original real-valued xR(n) sequence is shown in Figure 13-91(c). (Notice the z−2
delay blocks comprising two unit-delay elements.)
Figure 13-91 Seven-tap half-band FIR transformation: (a) hhb(k); (b) hhilb(k); (c) complex bandpass
filter structure.
Let’s call the network in Figure 13-91(c) a complex bandpass filter and describe its characteristics a
bit further. Figure 13-92(a) shows the |Hhb(f)| frequency magnitude response of a half-band filter, and
Figure 13-92(b) shows us that the complex bandpass filter’s |Hcbp(f)| frequency magnitude response is
|Hhb(f)| translated up in frequency by fs/4. However, notice that |Hcbp(f)|’s passband gain and ripple, as
well  as  its  stopband  ripple,  are  twice  that  of  |Hhb(f)|.  To  make  the  complex  bandpass  filter’s  gain
unity, rather than two, we decrease its coefficients by a factor of two and multiply the xI(n) sequence
in Figure 13-91(c) by 0.5. That 0.5 multiply could, of course, be implemented with an arithmetic right
shift of the xI(n) samples.
Figure 13-92 Frequency magnitude responses: (a) half-band filter; (b) complex bandpass filter.
The  nifty  part  of  this  complex  bandpass  filter  is  as  follows:  To  build  a  complex  nonrecursive  FIR
filter  having  the  performance  (transition  region  width,  stopband  attenuation,  etc.)  of  a  real  N-tap
lowpass  FIR  filter,  we  typically  must  implement  two  real  N-tap  FIR  filters  having  an  overall
computational workload of 2(N−1) additions and 2N multiplications per complex output sample, as
well as provide 2N memory locations to store the complex coefficients. The complex bandpass filter
in Figure 13-91(c) reduces those computations and the memory requirement by a factor of two.
Here’s another attribute: because the complex filter’s coefficients are antisymmetrical, we can use the
folded  FIR  filter  scheme  described  in  Section 13.7  to  reduce  the  number  of  multipliers  by  another
factor of two!
13.37.2 Half-band Filter Coefficient Modification
This  second  half-band  filter  to  Hilbert  transformer  conversion  scheme  is  useful  for  those
unfortunate folks who have no nonrecursive FIR filter design software at hand but do happen to have
the coefficients of a half-band filter. We can obtain the hhilb(k) coefficients of a Hilbert transformer
with a straightforward modification of the half-band FIR filter’s hhb(k) coefficients. The modification
steps are as follows:
1. Identify the center coefficient of hhb(k); call it hcenter.
2. Make the signs (polarity) of all nonzero coefficients before hcenter negative.
3. Make the signs (polarity) of all nonzero coefficients after hcenter positive.
4. Set the hcenter coefficient equal to zero.
An example of this half-band filter coefficient modification process is shown for an 11-tap FIR half-
band filter’s hhb(k)  in  Figure 13-93.  In  order  to  use  the  Figure 13-93(b) hhilb(k)  coefficients  in  the
complex bandpass filter in Figure 13-92(c), those hhilb(k) coefficients will need to be multiplied by a
factor of two, or the xI(n) sequence in Figure 13-91(c) must be multiplied by 0.5. The 0.5 multiply
can be implemented with an arithmetic right shift of the x′r(n) samples if desired.
Figure 13-93 Half-band filter coefficient modification: (a) original hhb(k) coefficients; (b) hhilb(k)
coefficients.
13.38 Complex Vector Rotation with Arctangents
It’s  often  the  case  in  quadrature  (I/Q)  processing  systems  that  we  want  to  compute  the  angle  of  a
complex time-domain sample. That angle computation for a complex sample C = I + jQ is, of course,
(13-154)
As  it  turns  out,  the  arctangent  computation  in  Eq.  (13-154)  can  be  performed  in  many  ways—
anywhere  from  slow  but  accurate  computationally  intensive  high-order  polynomial  evaluation,  to
high-speed crude-accuracy table look-up methods. However, regardless of the method used, we can
improve  the  accuracy  and  speed  of  an  arctangent  computation  if  we  limit  the  angular  range  over
which it is performed. And that’s where the vector rotation tricks presented here come into play.
13.38.1 Vector Rotation to the 1st Octant
Plotted  on  the  complex  plane,  a  complex  sample  C = I + jQ  can  reside  in  any  of  the  eight  octants
shown in Figure 13-94(a). When performing arctangents, please know that arctangent algorithms, be
they  high  precision  and  computationally  expensive  or  be  they  computationally  simple  and  lower
precision, are more accurate for small angles. (That is because the problematic arctangent function is
only approximately linear for small angles.) So what does this mean to us? It means that if we can
effectively rotate the angle of complex sample C into Figure 13-94(a)’s 1st or 8th octant, a smaller
angle, arctangent algorithms will provide more accurate results.
Figure 13-94 Complex vector rotation: (a) octant definitions; (b) vector rotation from the 7th octant
to the 1st octant.
For example, consider the complex number represented by vector C in Figure 13-94(b) residing in the
7th octant. The angle θ we want to compute is more negative than −π/4 radians (−45 degrees). Our
trick is to rotate C to a new (and smaller) angle θ1st, compute θ1st with an arctangent algorithm, and
add θ1st to −π/2 to obtain the desired value for θ.
Rotating vector C can be implemented as follows:
• If vector C‘s Q component is negative (C is in the 5th through the 8th octant), we can rotate C by
180 degrees by negating both the I and Q components.
• If vector C is in the 3rd or 4th octant, we can rotate C clockwise by 90 degrees by setting the new
I equal to the old Q value, and setting the new Q equal to the negative of the old I value. (Note that
the negative of the old I value is equal to the absolute value of the old I value.)
• If vector C is in the 2nd octant, we can rotate C clockwise by 45 degrees by swapping the I and Q
components.
Using the above rotation operations for our Figure 13-94(b) example, we can rotate the original 7th-
octant C = I + jQ to the 3rd octant by creating vector C3rd = −I − jQ. Next we rotate C3rd to the 1st
octant by creating vector C1st = −Q + jI. We compute θ1st as
(13-155)
using an arctangent algorithm and finally add θ1st to −π/2 to obtain our desired value for θ.
OK,  here’s  the  neat  part  of  this  trick.  We  don’t  actually  have  to  perform  any  of  the  above  vector
rotations to obtain angle θ1st. We merely need to find the signs of the original I and Q components and
determine  which  component  has  the  larger  magnitude.  With  those  three  pieces  of  information  we
determine in which octant vector C is located by using Table 13-9.
Table 13-9 Octant Identification
Once  we  know  vector  C‘s  octant,  we  take  advantage  of  the  following  rotational  symmetries  of
arctangents
(13-156)
(13-156′)
and compute our desired θ value using the appropriate expression in Table 13-10.
Table 13-10 Arctan Computation
Given  that  this  arctangent  process  is  implemented  with  programmable  hardware,  we’ll  have  Table
13-10’s four different arctangent approximation routines located at four different memory locations to
which we’ll jump. The process to determine the necessary two jump address index bits (b1,b0) based
on vector C‘s octant is shown in Figure 13-95.
Figure 13-95 Octant and jump address identification flow.
To  avoid  division  by  zero  when  using  the  algorithms  in  Table  13-10,  it’s  prudent  to  precede  the
Figure 13-95 processing with checking to see if I or Q is zero:
• If I = 0, θ is set to π/2 or −π/2 depending on the sign of Q.
• If Q = 0, θ is set to 0 or π depending on the sign of I.
• If I and Q are both zero, set θ to 0.
Again, this section does not present any specific arctangent algorithms. This material shows how to
make a given arctangent algorithm more accurate.
13.38.2 Vector Rotation by ±π/8
While we’re on the subject of vector rotation, if a 1st-octant vector C1st resides in the angle range of
π/8 ≤ θ1st ≤ π/4 radians (Range 1 in Figure 13-96(a), 22.5° ≤ θ1st ≤ 45°), we can rotate that vector by
−π/8 radians (−22.5°), forcing the new vector into Region 2. We may want to perform this rotation
because arctangent algorithms have improved accuracy in Region 2.
Figure 13-96 Angle ranges of the 1st and 8th octants.
We  rotate  a  vector  C1st  =  I1st  +  jQ1st  residing  in  Range  1  to  Range  2  by  multiplying  C1st  by  the
complex number e−jπ/8 = (A −jB), where
(13-157)
We can simplify the complex multiply by dividing A and B by 0.923879, yielding
(13-158)
This  gives  us  a  new  (A′ −jB′)  =  (1  −j0.414213)  multiplier,  reducing  the  number  of  necessary  real
multiplies in this −π/8 rotation process[88]. However, be aware that this (A′ −jB′) rotation induces a
vector magnitude gain of 1.0824 (0.69 dB) in the rotated vector.
Here’s how we decide if the 1st-octant vector C1st lies in the Range 1 of π/8 ≤ θ1st ≤ π/4 radians. If
the minimum of I1st or Q1st is less than 0.414213 times the maximum of I1st or Q1st, then C1st lies in
Region 1, in which case vector rotation by (A′ −jB′) multiplication is performed. Otherwise the 1st-
octant vector is in Range 2, requiring no rotation.
In  a  similar  manner,  if  an  8th-octant  vector  C8th  resides  in  the  angle  range  of  −π/4  ≤  θ8th  ≤  −π/8
radians (Range 4 in Figure 13-96(b)), we can rotate that vector by π/8 radians (22.5°), forcing the
new vector into Region 3 by multiplying C8th by (A′ + jB′).
Again, the angle range reduction schemes in this section allow us to use arctangent algorithms that are
computationally simpler (and thus faster) for a given accuracy. Of course, this technique forces us to
perform  additional  angle  range  checking  and  to  compute  products  such  as  (I1st  +  jQ1st)(A′  −jB′).
Perhaps this scheme is most useful when used with an arctangent look-up table method. You make the
call.
13.39 An Efficient Differentiating Network
This  section  presents  a  computationally  efficient  differentiating  network  that  approximates  the
process of taking the derivative of a discrete time-domain sequence. In Chapter 7 we introduced the
central-difference differentiator, defined by
(13-159)
as  a  simple  differentiating  network  that  has  desirable  high-frequency  (noise)  attenuation  behavior.
The  frequency  magnitude  response  of  that  differentiator  is  the  dashed  |HCd(f)|  curve  in  Figure  13-
97(a). (For comparison, we show an ideal differentiator’s straight-line |HIdeal(f)| magnitude response
in  Figure  13-97(a).  The  frequency  axis  in  that  figure  covers  the  positive-frequency  range  0≤ω≤π
samples/radian, corresponding to a continuous-time frequency range of 0 to fs/2, where fs is the input
sample rate in Hz.) The central-difference differentiator’s frequency range of linear operation is from
zero to roughly 0.08fs Hz.
Figure 13-97 Proposed differentiator: (a) performance; (b) standard structure; (c) folded structure.
Here  we  recommend  a  computationally  efficient  differentiator  that  maintains  the  central-difference
differentiator’s  beneficial  high-frequency  attenuation  behavior  but  extends  its  frequency  range  of
linear operation. The proposed differentiator is defined by
(13-160)
The Eq. (13-160) differentiator’s frequency magnitude response is the solid |HPr(f)| curve in Figure
13-97(a), where its frequency range of linear operation extends from zero to approximately 0.17fs Hz,
roughly twice the usable frequency range of the central-difference differentiator. The differentiator in
Eq. (13-160) has a gain greater than that of the central-difference differentiator, so the solid curve in
Figure 13-97(a) was scaled for easy comparison of |Hcd(f)| and |Hdif(f)|. The |Hdif(f)| curve is the DFT
of 0.6 · ydif(n).
The  structure  of  the  proposed  differentiator  is  shown  in  Figure  13-97(b)  where  a  delay  block
comprises  two  unit-delay  elements.  The  folded-FIR  structure  for  this  differentiator  is  presented  in
Figure 13-97(c) where only a single multiply need be performed per yPr(n) output sample. The really
slick aspect of the yPr(n) differentiator is that its non-unity coefficients (±1/16) are integer powers of
two.  This  means  that  a  multiplication  in  Figure 13-97  can  be  implemented  with  an  arithmetic  right
shift  by  four  bits.  Happily,  such  a  binary  right-shift  implementation  is  a  linear-phase  multiplierless
differentiator.
Another  valuable  feature  of  the  yPr(n)  differentiator  is  that  its  time  delay  (group  delay)  is  exactly
three  samples.  Such  an  integer  delay  makes  this  differentiator  convenient  for  use  when  the  ypr(n)
output must be time-synchronized with other signals within a system. For fairness, we point out that
the disadvantage of this very efficient differentiator is that for proper operation its x(n) input signals
must be low frequency, less than one-fifth the input sample rate.
In  terms  of  performance  and  computational  efficiency,  the  only  contender  to  the  proposed
differentiator is the first narrowband “super Lanczos low-noise differentiator” discussed in Chapter
7. However, the yPr(n) differentiator proposed here has better high-frequency noise attenuation than
the Lanczos differentiator.
13.40 Linear-Phase DC-Removal Filter
In this section we introduce a linear-phase DC-removal filter useful for removing the DC bias from a
time-domain signal. The filter is based on the notion of subtracting an input signal’s moving average
(DC bias) from that signal, as shown in Figure 13-98(a).
Figure 13-98 DC-removal filter: (a) filter concept; (b) filter structure; (c) filter frequency response.
In order to reduce the delay line length of a standard tapped-delay line moving average network, we
use  the  D-point  recursive  moving  averager  (MA),  shown  in  Figure  13-98(b).  The  bottom  path,  in
Figure 13-98(b), is a simple delay line having a length equal to the averager’s group delay, (D−1)/2
samples.  This  enables  us  to  time-synchronize  the  averager’s  v(n)  output  with  the  x(n)  input  in
preparation for the subtraction operation. There are two delay lines in Figure 13-98(b): the D-length z
−D delay line in the top path and the bottom path’s (D−1)/2-length delay line.
The D-point recursive moving averager (MA) in Figure 13-98(b) has a transfer function defined by
(13-161)
This DC-removal network’s passband performance, when D = 31, is shown in Figure 13-98(c). (The
frequency axis value of 0.5 corresponds to a cyclic frequency of half the input signal’s fs sample rate.)
While  the  network  has  the  desired  infinite  attenuation  at  zero  Hz,  its  passband  peak-peak  ripple  is
unpleasantly large at 2.9 dB. We can do better, as we shall see.
If D is an integer power of two, the 1/D scaling in (1) can be performed using a binary right shift by
log2(D) bits, making Figure 13-98(b) a multiplier-free network. However, in that scenario the MA’s
group delay is not an integer number of samples, making it difficult to synchronize the delayed x(n)
and the v(n) sequences. To solve this problem we can use two cascaded D-point MAs as shown in
Figure 13-99(a). Because the cascaded MAs have an integer group delay of D−1 samples, we can be
clever and tap off the first moving averager’s comb delay line, eliminating the bottom-path delay line
in 13-98(b). This way we still only need implement two delay lines in Figure 13-99(a), one z−D delay
line in each MA.
Figure 13-99 Dual-MA filter: (a) filter structure; (b) filter frequency response.
The magnitude response of the Figure 13-99(a) dual-MA DC-removal network, for D = 32, is shown
in  Figure  13-99(b).  In  that  figure  we  show  the  DC-removal  filter’s  passband  with  its  narrower
transition region width and a much improved peak-peak ripple of 0.42 dB. What we’ve created, then,
is  a  linear-phase,  multiplierless,  DC-removal  network  having  a  narrow  transition  region  near  zero
Hz.
Happily, it’s worth noting that standard tapped-delay line, linear-phase, highpass FIR filter designs
using least-squares error minimization, or the Parks-McClellan method, require more than 100 taps to
approximate our D = 32 DC-removal filter’s performance.
On  a  practical  note,  the  MAs  in  Figure  13-99(a)  contain  integrators  that  can  experience  data
overflow.  (An  integrator’s  gain  is  infinite  at  DC.)  Using  two’s  complement  fixed-point  arithmetic
avoids integrator overflow errors if we ensure that the integrator (accumulator) bit width is at least
(13-162)
where q(n) is the input sequence to an accumulator, and
to the next larger integer.
For  an  even  narrower  filter  transition  region  width,  in  the  vicinity  of  zero  Hz,  than  that  shown  in
Figure 13-99(b), we can set D  to  a  larger  integer  power  of  two;  however,  this  will  not  reduce  the
DC-removal filter’s passband ripple.
 means that if k is not an integer, round it up
At the expense of three additional delay lines, and four new addition operations per output sample,
we  can  implement  the  linear-phase  DC-removal  filter  shown  in  Figure  13-100(a).  That  quad-MA
implementation,  having  a  group  delay  of  2D−2  samples,  yields  an  improved  passband  peak-peak
ripple of only 0.02 dB, as shown in Figure 13-100(b), as well as a reduced-width transition region
relative to the dual-MA implementation.
Figure 13-100 Quad-MA filter: (a) filter structure; (b) filter frequency response.
The DC removal network in Figure 13-100(a) contains four 1/D scaling operations which, of course,
can be combined and implemented as a single binary right shift by 4log2(D) bits. So the bottom line
here is that at the expense of multiple delay lines, it is possible to efficiently perform linear-phase DC
removal.
13.41 Avoiding Overflow in Magnitude Computations
Here we present a little trick to help avoid a common problem when computing the magnitude of a
complex  number  using  fixed-point  binary  number  formats.  Let’s  say  we  have  a  complex  number  c
represented by c = R + jI, and we want to compute the magnitude c using the familiar expression
(13-163)
Equation (13-163) is troublesome because the R2 and I2 terms will cause data word overflow errors
if either R or I  is  greater  than  the  square  root  of  your  fixed-point  number  format’s  largest  positive
number.  For  example,  in  a  signed  16-bit  number  format,  |R|  and  |I|  must  be  less  than  181  to  avoid
overflow  errors.  At  the  expense  of  absolute  value  comparison,  branch,  and  divide  operations,  Eq.
(13-164) alleviates overflow problems[89]:
(13-164)
13.42 Efficient Linear Interpolation
In this section we present a computationally efficient linear interpolation trick that’s useful because it
performs  linear  interpolation  requiring  at  most  one  multiply  per  output  sample[90].  For  example,
given  the  x(n)  time  sequence  in  Figure  13-101(a),  this  linear  interpolator  will  generate  the  y(n)
sequence shown in Figure 13-101(b) when the interpolation factor is L = 3. Notice how the original
x(n) samples are preserved in the y(n) output sequence.
Figure 13-101 Linear interpolation: (a) input sequence; (b) L = 3 interpolated sequence; (c)
interpolator structure.
The  block  diagram  of  this  efficient  linear  interpolator  is  that  in  Figure 13-101(c).  That  mysterious
block labeled “Hold Interpolator, L” is merely the operation where each input sample to the block is
repeated L−1 times. For example, if the input to the Hold Interpolator operation is {1,4,3}, and L = 3,
the output of the Hold Interpolator is {1,1,1,4,4,4,3,3,3}.
In fixed-point binary implementations if we’re able to select L to be an integer power of two, then,
happily, the final 1/L multiplication can be implemented with a binary arithmetic right shift by log2L
bits,  yielding  a  multiplierless  linear  interpolator.  Of  course,  if  a  gain  of  L  is  acceptable,  no  1/L
scaling need be performed at all.
The  neat  part  of  this  interpolator  is  that  the  computational  workload,  the  number  of  additions  and
multiplies per output sample, remains fixed regardless of the value of interpolation factor L.
The  experienced  reader  might  now  say,  “Ah,  while  this  network  is  computationally  simple,  linear
interpolation  is  certainly  not  the  most  accurate  method  of  interpolation,  particularly  for  large
interpolation factors of L.” That is true, but if interpolation is being done in multiple sections, using
this efficient linear interpolation as the final section at the highest data rate (when the signal samples
are already very close together) will introduce only a small interpolation error.
13.43 Alternate Complex Down-conversion Schemes
Here  we  present  two  interesting  complex  down-conversion  and  decimation  techniques  used  to
generate  an  analytic  (complex)  version,  centered  at  zero  Hz,  of  a  real  bandpass  signal  that  was
originally  centered  at  ±fs/4  (one-fourth  the  sample  rate).  Both  methods  perform  signal  frequency
translation by way of decimation.
13.43.1 Half-band Filter Down-conversion
The  first  complex  down-conversion  method  makes  use  of  computationally  efficient  half-band
filters[91]. The process is shown in Figure 13-102(a), where we use indices n, p, and m to clarify the
multirate  nature  of  this  process.  The  real  xR(n)  input  signal  has  the  spectrum  shown  in  Figure  13-
102(b),  and  for  our  example  the  sample  rate  is  fs  =  24  kHz.  The  Delay/Hilbert  transform  filter
combination attenuates the negative-frequency spectral components of XR(f) to produce the complex
uI(n)  +  juQ(n)  signal  whose  spectrum  is  provided  in  Figure  13-102(c).  (The  Delay  function  is  a
cascade of unit-delay elements, whose length is the group delay of the Hilbert filter, needed to time-
synchronize the uI(n) and uQ(n) sequences.) The follow-on downsample by two, discard every other
sample, produces the complex v(p) sequence having the spectrum shown in Figure 13-102(d) where
the new sample rate is 12 kHz.
Figure 13-102 Analytic signal generation and decimation by two.
Next, sequences vI(p) and vQ(p) are applied to two identical real-valued highpass half-band filters,
each having the frequency magnitude response shown in Figure 13-103(a), yielding the complex w(p)
=  wI(p)  and  wQ(p)  whose  spectrum  is  that  in  Figure  13-103(b).  The  final  step  in  this  down-
conversion process is another decimation by two, producing the desired xc(m) sequence having the
spectrum given in Figure 13-103(c) where the output sample rate is 6 kHz. Due to the nature of half-
band filters there will be some amount of spectral overlap in Xc(f) as shown in Figure 13-103(c). The
amount of spectral overlap is proportional to the transition region width of an hhp(k) filter (inversely
proportional to the number of filter taps).
Figure 13-103 Highpass filtering, down-conversion, and decimation by two.
There  are  three  useful  aspects  to  this  first  complex  down-conversion  scheme  that  enhance  its
computational efficiency:
• If the Hilbert transform filter has an odd number of taps, roughly half of its coefficients will be
zero-valued, and the Delay function is an integer number of unit-delay elements.
•  Roughly  half  of  the  coefficients  of  the  highpass  half-band  filters,  with  their  transition  regions
centered at fs/4 and 3fs/4, will be zero-valued.
•  Because  the  coefficients  of  the  filters  in  Figure  13-102(a)  are  either  symmetrical  or
antisymmetrical, we can use the folded FIR filter scheme described in Section 13.7 to reduce the
number of multipliers by another factor of two.
13.43.2 Efficient Single-Decimation Down-conversion
Our  second  complex  down-conversion  trick  is  a  very  computationally  efficient  scheme,  shown  in
Figure 13-104(a), that operates on real xR(n) signals centered at ±fs/4. Just as in Figure 13-102(a),
the Delay/Hilbert transform filter combination attenuates the negative-frequency spectral components
of  xR(n)  to  produce  a  complex  analytic  signal  whose  spectrum  is  centered  at  fs/4  (6  kHz).  The
downsample-by-four, retain every fourth sample, operation down-converts (frequency translates) the
desired  complex  signal  originally  centered  at  fs/4  Hz  down  to  a  center  frequency  of  zero  Hz.  The
compensation filter is used to compensate for the non-flat frequency magnitude response of the simple
2-tap  Hilbert  filter  in  order  to  widen  the  down-converter’s  usable  passband  width.  (The  Delay
function  after  the  downsampling  in  the  top  path  is  needed  to  time-synchronize  the  xI(m)  and  xQ(m)
sequences.) The detailed block diagram of the down-converter is shown in Figure 13-104(b), where
the compensation filter’s coefficients are hc(0) = −1/32, and hc(1) = 1/2 + 1/16.
Figure 13-104 High-efficiency complex down-conversion: (a) process; (b) detailed structure.
If the xR(n) input signal’s bandwidth is no greater than fs/6, then the Hilbert filter attenuates xR(n)’s
undesired negative-frequency spectral components, at the xc(n) output, by approximately 35 dB. That
much  attenuation  may  not  be  something  to  write  home  about,  but  keep  in  mind  that  this  down-
converter  requires  no  multipliers  because  the  multiplies  by  the  hc(0) and hc(1)  coefficients  can  be
implemented  with  binary  shifts  and  adds.  At  the  expense  of  two  multiplies  per  output  sample,  the
compensation  filter  coefficients  can  be  set  to  hc(0)  =  −0.02148  and  hc(1)  =  0.54128  to  attenuate
xR(n)’s undesired negative-frequency spectral components by roughly 45 dB.
13.44 Signal Transition Detection
When  we  are  tasked  to  build  a  system  that  must  detect  transitions  in  a  pulsed  signal,  we  generally
look  to  a  digital  differentiator  as  the  solution  to  our  problem.  However,  when  a  pulsed  signal’s
transition  spans  many  samples,  and  particularly  if  the  signal  is  noisy,  digital  differentiators  do  not
provide reliable signal transition detection. One compelling solution to this problem uses a standard
tapped-delay  line  (time-domain  convolution)  filtering  scheme  developed  by  C.  Turner[92].  Called
time-domain  slope  filtering,  the  transition  detection  tapped-delay  line  filter  uses  N  coefficients
defined by
(13-165)
where the coefficient index k covers the range 0 ≤ k ≤ N−1.
For any integer N, the slope filtering Ck coefficients comprise a linear ramp, making that sequence
quite useful for detecting linear transitions in an input signal. Figure 13-105(a) shows the output of the
time-domain slope filtering process, when N = 53. In that figure we see that the slope filter performs
well in detecting the transitions of the Input signal. The dotted curve in Figure 13-105(a) is the output
of a traditional tapped-delay line digital differentiator having 53 taps. (The frequency magnitude of
the  traditional  digital  differentiator,  specifically  designed  to  attenuate  high-frequency  noise,  is
provided in Figure 13-105(b).)
Figure 13-105 Time-domain slope filtering: (a) pulsed input performance; (b) digital differentiator
magnitude response; (c) high-noise input performance.
The  superiority  of  the  time-domain  slope  filtering  scheme  over  traditional  differentiation  is  further
illustrated in Figure 13-105(c) where the pulsed Input signal is contaminated with high-level noise.
Concerning two practical issues, if the number of samples in a pulsed input signal’s transition is L,
the value for N, found empirically, is generally in the range of L/4 to L. It’s convenient to set N to be
an odd integer, forcing the filter’s delay to be an integer number, (N−1)/2, of samples. This facilitates
the  time  synchronization  of  the  filter’s  output  to  other  sequences  in  a  system.  Also,  if  the  Ck
coefficients  are  to  be  used  in  correlation  processing  (as  opposed  to  the  convolution  processing
discussed  above),  the  correlation’s  Ck  coefficients  should  be  the  coefficients  from  Eq.  (13-165)
reversed in time order.
13.45 Spectral Flipping around Signal Center Frequency
In Section 2.4, we discussed a super-simple method of spectral flipping (spectral inversion) of a real
signal  where  the  center  of  spectral  rotation  was  fs/4.  In  this  section  we  discuss  a  different  kind  of
spectral flipping process.
Consider the situation where we need to process a real-valued x(n) time signal, whose X(f) spectrum
is shown in Figure 13-106(a), to obtain a real-valued y(n) time signal whose spectrum is the flipped
Y(f)  spectrum  shown  in  Figure 13-106(b).  Notice  that  the  center  of  rotation  of  the  desired  spectral
flipping  is  not  fs/4  Hz  but  is  instead  the  x(n)  signal’s  fc  center  frequency.  The  spectral  flipping
process described in Section 2.4 does not solve our problem because that process would result in the
undesirable  spectrum  shown  in  Figure  13-106(c),  where  the  original  X(f)  spectrum  is  the  dashed
curve.
Figure 13-106 Spectral flipping, centered at fc: (a) original spectrum; (b) desired spectrum; (c)
incorrect spectrum.
There are two methods to solve our fc-centered spectral flipping problem. Figure 13-107(a) shows
the  first  method,  comprising  a  multirate  processing  technique.  In  considering  this  spectral  flipping
method, the user should keep in mind that:
• The two lowpass filters (LPFs) have passbands that extend from zero Hz to fH Hz. (Note that the
sample rate for both filters is 2fs Hz.) The second LFP’s transition region width is less than 2fL.
• The cosine mixing sequence uses the upsampled-by-two time index variable n′.
• The multiply-by-four operation compensates for the sequence u(n′) amplitude loss by a factor of
two  caused  by  interpolation,  and  the  amplitude  loss  by  another  factor  of  two  due  to  the  cosine
mixing.
Figure 13-107 Spectral flipping techniques: (a) first method; (b) second method.
Of course, a smart engineer will eliminate the multiply-by-four operation altogether by increasing the
DC (zero Hz) gain of one of the lowpass filters by four.
The second method we could use to obtain a signal having the desired Figure 13-106(b) spectrum,
promoted  by  D.  Bell,  is  the  process  shown  in  Figure  13-107(b)[93].  While  somewhat  more
computationally  intensive  than  the  above  multirate  method,  this  technique  works  well  and  deserves
mention here. The first complex multiplication and the Complex LPF are identical to the quadrature
sampling operations we discussed in Figure 8-18(a). The two identical lowpass filters, comprising
the  Complex  LPF,  have  passbands  that  extend  from  zero  Hz  to  (fH−fL)/2  Hz,  and  transition  region
widths of less than 2fL. The Real Part operation merely means take the real part of sequence v(n).
We can eliminate the multiply-by-two operation by increasing the DC (zero Hz) gain of the complex
filter by two. In this method, as Bell recommends, we can combine the second complex multiply and
Real Part extraction stages by computing only the real part of sequence u(n), yielding sequence v(n).
The multiply-by-two operation compensates for the amplitude loss by a factor of two caused by the
Real Part operation.
13.46 Computing Missing Signal Samples
Consider the situation where we need to process a time-domain signal that has been corrupted such
that every Qth sample is missing from the desired signal sequence. This section provides a trick for
how to recover periodically spaced missing samples of a corrupted time sequence[94].
To explain our problem, assume we want to process an x0(n) time sequence, whose sample rate is fs
Hz, but all we have available to us is a corrupted xq(n) sequence where:
• xq(n) is equal to the desired x0(n) with every Qth sample of x0(n) missing. The missing samples in
xq(n), xq(pQ) where p = 0, 1, 2, ... are represented by zero-valued samples.
• x0(n) is band-limited with negligible energy above B Hz where
(13-166)
for some integer Q ≥ 2 where fs is the data sample rate in Hz.
As an example, when Q = 5, if the desired x0(n) is the sequence in Figure 13-108(a), then xq(n) is the
corrupted sequence shown in Figure 13-108(b). Our job, then, is to recover (interpolate) the missing
samples in xq(n), xq(0), xq(5), xq(10), ... etc., to reconstruct the desired x0(n) sequence.
Figure 13-108 Time sequences: (a) original x0(n); (b) corrupted xq(n) when Q = 5.
The  solution  to  our  problem  is  to  apply  the  xq(n)  sequence  to  the  tapped-delay  line  reconstruction
filter shown in Figure 13-109. Describing Figure 13-109’s operation in words: our desired xo(n−K)
samples are the xq(n−K) samples at the center tap of the filter unless that xq(n−K) sample is a zero-
valued missing sample, in which case the switches toggle and we compute the estimated xo(n−K) =
xo(pQ).
Figure 13-109 Reconstruction filter implementation.
The filter’s c(k) coefficients are determined by first evaluating the following expression:
(13-167)
where integer index k is in the range −K ≤ k ≤ K, sinc(x) = sin(πx)/πx, and w(k) is a time-symmetric
window sequence of length 2K+1. Next, we use h(k) to compute our desired filter coefficients as
(13-168)
This missing sample recovery process can also be applied to complex xq(n) signals, in which case
the real and imaginary parts of a complex xq(n) must be filtered separately.
There  are  two  practical  considerations  to  keep  in  mind  when  using  this  missing  sample  recovery
process. The first consideration is to be aware that the maximum bandwidth B given in Eq. (13-166)
is based on the assumption that the reconstruction filter has an infinite number of taps. As such, for
practical-length filters the B  bandwidth  requirement  must  be  reduced.  To  show  this,  Figure  13-110
illustrates the missing sample recovery error when Q = 5, B = 0.4fs, using a Chebyshev window with
−100 dB sidelobes, for various values of K. The input signal is a noiseless sinusoid, with unity peak
amplitude, swept in frequency from a very low frequency up to fs/2 (half the sample rate).
Figure 13-110 Recovery error curves, for various K, versus input tone frequency.
In that figure we see that a K = 3 filter (7 taps) exhibits low missing sample recovery error until the
input signal’s frequency approaches roughly 0.25fs, where the recovery error starts to become large.
When K = 5, the recovery error doesn’t become large until the input signal’s frequency approaches
roughly 0.3fs. (The unlabeled curve in Figure 13-110 is a K  =  7  curve.)  So  what  we  see  is  that  to
minimize  our  missing  sample  recovery  error  for  short-length  filters,  the  maximum  input  signal
bandwidth must be kept substantially lower than the B Hz specified in Eq. (13-166).
The second practical consideration to consider when using this missing sample recovery process is
the  w(k)  window  sequence  in  Eq.  (13-167).  There  seems  to  be  no  “best”  window  sequence  that
minimizes the recovery error for all real-world signals that we might encounter. So experimentation,
using various window functions, becomes necessary. A good place to start is to use either Kaiser or
Chebyshev window sequences whose control parameters are set such that the windows’ frequency-
domain sidelobes are very low relative to their main lobe levels.
We  conclude  this  section  by  mentioning  that  reference  [95]  describes  a  missing  sample  recovery
technique that is applicable when the pattern of missing samples is more complicated than the simple
every Qth sample described here.
13.47 Computing Large DFTs Using Small FFTs
It  is  possible  to  compute  N-point  discrete  Fourier  transforms  (DFTs)  using  radix-2  fast  Fourier
transforms (FFTs) whose sizes are less than N. For example, let’s say the largest size FFT software
routine we have available is a 1024-point FFT. With the following trick we can combine the results
of multiple 1024-point FFTs to compute DFTs whose sizes are greater than 1024.
The  simplest  form  of  this  idea  is  computing  an  N-point  DFT  using  two  N/2-point  FFT  operations.
Here’s how the trick works for computing a 16-point DFT, of a 16-sample x(n) input sequence, using
two 8-point FFTs. First we perform an 8-point FFT on the x(n) samples where n = 0, 2, 4, ..., 14.
We’ll call those FFT results X0(k). Then we store two copies of X0(k) in Memory Array 1 as shown
in Figure 13-111. Next we compute an 8-point FFT on the x(n) samples where n = 1, 3, 5, ..., 15. We
call those FFT results X1(k). We store two copies of X1(k) in Memory Array 3 in Figure 13-111.
Figure 13-111 A 16-point DFT using two 8-point FFTs.
In  Memory  Array  2  we  have  stored  16  samples  of  one  cycle  of  the  complex  exponential  e−j2πm/N,
where N = 16, and 0 ≤ m ≤ 15. Finally we compute our desired 16-point X(m) samples by performing
the arithmetic shown in Figure 13-111 on the horizontal rows of the memory arrays. That is,
The desired X(m) DFT results are stored in Memory Array 4.
We describe the above process, algebraically, as
(13-169)
and
(13-169′)
for k in the range 0 ≤ k ≤ 7.
Notice that we did nothing to reduce the size of Memory Array 2 due to redundancies in the complex
exponential sequence e−j2πm/N. As it turns out, for an N-point DFT, only N/4 complex values need be
stored in Memory Array 2. The reason for this is that
(13-170)
which involves a simple sign change on e−j2πm/N. In addition,
(13-170′)
which is merely swapping the real and imaginary parts of e−j2πm/N plus a sign change of the resulting
imaginary part. So Eqs. (13-170) and (13-170′) tell us that only the values e−j2πm/N for 0 ≤ m ≤ N/4−1
need be stored in Memory Array 2. With that reduced storage idea aside, to be clear regarding exactly
what  computations  are  needed  for  our  “multiple-FFTs”  technique,  we  leave  Memory  Array  2
unchanged from that in Figure 13-111.
The  neat  part  of  this  “multiple-FFTs”  scheme  is  that  our  DFT  length,  N,  is  not  restricted  to  be  an
integer  power  of  two.  We  can  use  computationally  efficient  radix-2  FFTs  to  compute  DFTs  whose
lengths are any integer multiple of an integer power of two. For example, we can compute an N = 24-
point DFT using three 8-point FFTs. To do so, we perform an 8-point FFT on the x(n) samples, where
n = 0, 3, 6, ..., 21, to obtain X0(k). Next we compute an 8-point FFT on the x(n) samples, where n = 1,
4, 7, ..., 22, to yield X1(k). And then we perform an 8-point FFT on the x(n) samples, where n = 2, 5,
8, ..., 23, to obtain an X2(k) sequence. Finally, we compute our desired 24-point DFT results using
and
(13-171)
(13-171′)
(13-171″)
for k in the range 0 ≤ k ≤ 7. The memory-array depiction of this process is shown in Figure 13-112,
with our final 24-point DFT results residing in Memory Array 6. Memory Array 2 contains N = 24
samples  of  one  cycle  of  the  complex  exponential  e−j2πm/24,  where  0  ≤  m  ≤  23.  Memory  Array  4
contains 24 samples of two cycles of the complex exponential e−j2π(2m)/24.
Figure 13-112 A 24-point DFT using three 8-point FFTs.
To  conclude  this  section,  we  state  that  the  larger  the  size  of  the  FFTs,  the  more  computationally
efficient  is  this  “multiple-FFTs”  spectrum  analysis  technique.  This  behavior  is  illustrated  in  Figure
13-113 where we show the number of complex multiplies required by the “multiple-FFTs” algorithm
versus the desired DFT size (N). The top bold curve is the number of complex multiplies required by
the  standard  (inefficient)  DFT  algorithm,  and  the  bottom  dashed  curve  is  the  number  of  complex
multiplies required by a single N-point radix-2 FFT. The curves in the center of the figure show the
number of complex multiplies required by the “multiple-FFTs” algorithm when various FFT sizes (P)
are used to compute an N-point DFT. For example, if we must perform a 4096-point DFT using this
“multiple-FFTs”  algorithm,  it’s  better  for  us  to  perform  sixteen  256-point  FFTs  rather  than  one
hundred twenty-eight 32-point FFTs.
Figure 13-113 Number of complex multiplies versus N.
13.48 Computing Filter Group Delay without Arctangents
Here we present an interesting scheme used to compute the group delay of digital filters that does not
require the phase unwrapping process needed when computing arctangents in traditional group delay
measurement  algorithms.  The  technique  is  based  on  the  following:  Assume  we  have  the  N-sample
h(k) impulse response of a digital filter, with k (0″k″N−1) being our time-domain index, and that we
represent the filter’s discrete-time Fourier transform (DTFT), H(ω), in polar form as
(13-172)
In Eq. (13-172), M(ω)  is  the  frequency  magnitude  response  of  the  filter,  ϕ(ω)  is  the  filter’s  phase
response, and ω is continuous frequency measured in radians/second. Taking the derivative of H(ω)
with respect to ω, and performing a variety of algebraic acrobatics, we can write
(13-173)
So what does that puzzling gibberish in Eq. (13-173) tell us? As it turns out, it tells us a lot if we
recall the following items:
• jd[H(ω)]/dω = the DTFT of k · h(k)
• M(ω) · ejϕ(ω) = H(ω) = the DTFT of h(k)
• −d[ϕ(ω)]/dω = group delay of the filter
Now we are able to translate Eq. (13-173) into the meaningful expression
(13-173′)
Discretizing expression (13-173′) by replacing the DTFT with the discrete Fourier transform (DFT),
we arrive at our scheme for computing the group delay of a digital filter, measured in samples:
(13-174)
So, starting with a filter’s N-sample h(k) impulse response, performing two N-point DFTs and an N-
sample complex division, we can compute the filter’s passband group delay. (Of course, to improve
our group delay granularity we can zero-pad our original h(k) before computing the DFTs). Again, the
advantage  of  the  process  in  expression  (13-174)  is  that  the  phase  unwrapping  process  needed  in
traditional  group  delay  algorithms  is  not  needed  here.  Note  that  in  implementing  the  process  in
expression (13-174), we must be prepared to accommodate the situation where a frequency-domain
DFT[h(k)] sample is zero-valued, which will make a group delay sample unrealistically large.
As an example, the square dots in Figure 13-114(a) show the N = 25-sample h(k) impulse response of
a 2nd-order IIR lowpass filter. A 25-sample filter group delay estimation, using expression (13-174),
is shown in Figure 13-114(b). When we zero-pad the h(k) and k · h(k) sequences to a length of 64
samples (0≤k≤63), expression (13-174) yields the group delay estimate in Figure 13-114(c).
Figure 13-114 Group delay computation: (a) 25-sample h(k) and k · h(k); (b) 25-point group delay;
(c) 64-point group delay.
13.49 Computing a Forward and Inverse FFT Using a Single FFT
In Section 13.5 we described the processes of using a single N-point complex FFT to perform both a
2N-Point Real FFT and two independent N-Point Real FFTs. This section presents the algorithm for
simultaneously computing a forward FFT and an inverse FFT using a single radix-2 FFT[96].
Our algorithm is depicted by the seven steps, S1 through S7, shown in Figure 13-115. In that figure,
we  compute  the  x(n)  inverse  FFT  of  the  N-point  frequency-domain  conjugate-symmetric  input
sequence X(m),  as  well  as  compute  the  Y(m)  forward  FFT  of  the  N-point  time-domain  real-valued
input sequence y(n)  using  the  single  complex  FFT  in  Step  S4.  Sample  indices  n  and  m  both  range
from 0 to N−1 where N is an integer power of two.
Figure 13-115 Simultaneous FFT and inverse FFT algorithm.
At first glance Figure 13-115 looks more complicated than it actually is, and here’s why:
• Steps S1 and S2 create a complex sequence that we call v(n).
• Step S1 generates the first N/2+1 samples of v(n) based on the real-valued input sequence y(n).
• Step S2 extends v(n) to a length of N samples and forces v(n) to be conjugate symmetric. The “*”
symbol in Step S2 means conjugation.
• Step S3 combines the conjugate-symmetric sequences X(m) and v(n) to create a sequence we call
z(n). (Sequence z(n) is not conjugate symmetric.)
• Step S4 is the algorithm’s single radix-2 FFT operation, generating complex sequence Z(m).
• Step S5 generates the desired real-valued x(n) time sequence by performing a circular reversal of
the  real  part  of  Z(m).  (That  is,  other  than  the  first  sample,  the  real  parts  of  Z(m)  samples  are
reversed in order to produce x(n). This type of sequence reversal is discussed in Appendix C.)
• Steps S6 and S7 generate the desired frequency-domain Y(m) sequence.
• Step S6 generates the first N/2+1 samples of Y(m).
•  Step  S7  extends  the  sequence  from  Step  S6  to  a  length  of  N  samples  and  forces  conjugate
symmetry, to produce Y(m). The “*” symbol in Step S7 means conjugation.
The Figure 13-115 algorithm’s computational workload is one complex N-point FFT and roughly 2N
additions/subtractions.
13.50 Improved Narrowband Lowpass IIR Filters
Due to their resistance to quantized-coefficient errors, traditional 2nd-order infinite impulse response
(IIR)  filters  are  the  fundamental  building  blocks  in  computationally  efficient  high-order  IIR  digital
filter implementations. However, when used in fixed-point number systems, the inherent properties of
quantized-coefficient  2nd-order  IIR  filters  do  not  readily  permit  their  use  in  narrowband  lowpass
filtering  applications.  Narrowband  lowpass  IIR  filters  have  traditionally  had  a  bad  reputation—for
example,  MATLAB’s  Signal  Processing  Toolbox  documentation  warns:  “All  classical  IIR  lowpass
filters are ill-conditioned for extremely low cutoff frequencies.”
This  section  presents  a  neat  trick  to  overcome  the  shortcomings  of  narrowband  2nd-order  lowpass
IIR  filters,  with  no  increase  in  filter  coefficient  bit  widths  and  no  increase  in  the  number  of  filter
multiplies per output sample.
13.50.1 The Problem with Narrowband Lowpass IIR Filters
Narrowband lowpass IIR filters are difficult to implement because of intrinsic limitations on their z-
plane pole locations. Let’s examine the restrictions on the z-plane pole locations of a standard 2nd-
order IIR filter whose structure is shown in Figure 13-116(a).
Figure 13-116 Second-order IIR filters: (a) standard form; (b) coupled form.
Such an IIR filter, having a transfer function given by
(13-175)
has a pair of conjugate poles located at radii of r, at angles of ±θ radians. (For filter stability reasons,
we  always  ensure  that  r  <  1.)  In  fixed-point  implementations,  quantizing  the  2rcos(θ)  and  −r2
coefficients restricts the possible pole locations[97,98]. On the z-plane, a pole can only reside at the
intersection  of  a  vertical  line  defined  by  the  quantized  value  of  2rcos(θ)  and  a  concentric  circle
whose radius is defined by the square root of the quantized value of r2. For example, Figure 13-117
shows the first quadrant of possible z-plane pole locations using five magnitude bits to represent the
filter’s  two  coefficients.  Notice  the  irregular  spacing  of  those  permissible  pole  locations.  (Due  to
trigonometric  symmetry,  the  pole  locations  in  the  other  three  quadrants  of  the  z-plane  are  mirror
images of those shown in Figure 13-117.)
Figure 13-117 Possible pole locations for five magnitude bit coefficient quantization.
So here’s the problem we have with standard 2nd-order IIR filters: If we use floating-point software
to  design  a  very  narrowband  (high-order)  lowpass  IIR  filter  (implemented  as  cascaded  2nd-order
filters)  having  poles  residing  in  the  shaded  area  near  z  =  1,  subsequent  quantizing  of  the  designed
filter coefficients to five magnitude bits will make the poles shift to one of the locations shown by the
dots on the border of the shaded region in Figure 13-117. Unfortunately that pole shifting, inherent in
the Figure 13-116(a) IIR filter implementation due to coefficient quantization in fixed-point systems,
prevents us from realizing the desired narrowband lowpass filter. We can always reduce the size of
the  shaded  forbidden  zone  near  z  =  1  in  Figure  13-117  by  increasing  the  number  of  bits  used  to
represent  the  2nd-order  filters’  coefficients.  However,  in  some  filter  implementation  scenarios
increasing coefficient binary-word bit widths may not be a viable option.
One  solution  to  the  above  problem  is  to  use  the  so-called  coupled-form  IIR  filter  (also  called  the
Gold-Rader filter[99]) structure, shown in Figure 13-116(b), having a transfer function given by
(13-176)
Because the coupled-form filter’s quantized coefficients in Figure 13-116(b) are linear in rcos(θ) and
rsin(θ),  its  possible  pole  locations  are  on  a  regularly  spaced  grid  on  the  z-plane  defined  by  z  =
rcos(θ) + jrsin(θ). This enables us to build 2nd-order narrowband lowpass IIR filters with poles in
the desired shaded region of Figure 13-117.
This pole placement behavior is a beautiful example of the difference between apparently equivalent
filter implementations. With infinite-precision coefficients the standard and coupled-form IIR filters,
having identical denominators in their transfer functions, will have identical z-plane pole locations.
But with quantized coefficients the two filters will have different pole locations.
Back  to  our  problem.  While  the  coupled-form  IIR  filter  gives  us  increased  flexibility  in  placing  z-
plane poles for lowpass filtering, unfortunately, this coupled-form implementation requires twice the
number of multiplies needed by the standard 2nd-order IIR filter in Figure 13-116(a).
In the following material we describe a slick narrowband lowpass IIR filter structure, proposed by
Harris and Loudermilk, having poles residing in the shaded region of Figure 13-117 with no increase
in  coefficient  bit  width  and  no  additional  multiplication  operations  beyond  those  needed  for  a
standard 2nd-order IIR filter[100].
13.50.2 An Improved Narrowband Lowpass IIR Filter
The improved lowpass IIR filter is created by replacing each unit-delay element in a standard 2nd-
order IIR filter with multiple unit-delay elements as shown in the left portion of Figure 13-118. This
zero-stuffed IIR filter will retain its original lowpass passband and have multiple passband images,
exactly as did the interpolated finite impulse response (IFIR) filters that we studied in Chapter 7. The
zero-stuffed  IIR  filter  is  followed  by  a  lowpass  image-reject  filter  that  attenuates  those  unwanted
passband images. Given this cascaded structure, which we’ll demonstrate shortly, we call the filter
combination in Figure 13-118 an interpolated infinite impulse response (interpolated-IIR) filter.
Figure 13-118 Interpolated-IIR filter.
The M-length delay lines, where M is a positive integer, in the zero-stuffed IIR filter shift a standard
IIR filter’s conjugate poles, originally located at z = re±jθ, to the new locations of
(13-177)
That is, the new conjugate pole locations are at radii of the Mth root of r, at angles of ±θ /M radians.
Happily, those interpolated-IIR filter pole locations can now reside in the desired shaded region of
Figure 13-117 without using more bits to represent the zero-stuffed IIR filter’s coefficients.
If  the  original  Figure  13-116(a)  2nd-order  IIR  filter  contains  feedforward  coefficients,  those
coefficients are also delayed by M-length delay lines.
13.50.3 Interpolated-IIR Filter Example
Let’s  show  an  example  of  an  interpolated-IIR  filter  in  action.  With  fs  representing  a  filter’s  input
signal sample rate in Hz, assume we want to implement a recursive lowpass filter whose one-sided
passband width is 0.005fs with a stopband attenuation greater than 60 dB. If we choose to set M = 4,
then  we  start  our  interpolated-IIR  filter  design  process  by  designing  a  standard  IIR  filter  having  a
one-sided passband width of M · 0.005fs = 0.02fs. Using our favorite IIR filter design software (for
an elliptic IIR filter in this example), we obtain a 5th-order prototype IIR filter. Partitioning that 5th-
order prototype IIR filter into two 2nd-order and one single-order IIR filter sections, all in cascade
and having coefficients represented by 12-bit words, yields the frequency magnitude response shown
in Figure 13-119(a).
Figure 13-119 Frequency magnitude responses: (a) original IIR prototype filter; (b) zero-stuffed
interpolated-IIR filter and CIC filters (dashed); (c) final narrowband 12-bit coefficient filter.
Next, replacing the unit-delay elements in the filter sections with M = 4 unit-delay elements results in
the frequency magnitude response shown in Figure 13-119(b). There we see the multiple narrowband
passband images induced by the M = 4-length delay lines of the interpolated-IIR filter. Our final job
is to attenuate those unwanted passband images. We can do so by following the cascaded increased-
delay IIR filter sections with a cascaded integrator-comb (CIC) filter, whose structure is shown on
the right side of Figure 13-118. (The CIC filter is computationally advantageous because it requires
no  multiplications.)  To  satisfy  our  desired  60  dB  stopband  attenuation  requirement,  we  use  a  2nd-
order CIC filter—two 1st-order CIC filters in cascade—to attenuate the passband images in Figure
13-119(b).  The  result  of  our  design  is  the  interpolated-IIR  and  CIC  filter  combination  whose
composite frequency magnitude response meets our filter requirements as shown Figure 13-119(c).
In  practice,  2nd-order  subfilters  may  have  large  gains  requiring  unpleasantly  large  bit-width
multipliers  and  large  bit-width  registers  to  store  intermediate  results.  For  this  reason  it  may  be
necessary to scale the IIR subfilters’ coefficients as discussed in Chapter 6, or truncate the subfilters’
output samples, in order to avoid undesirably large bit-width processing.
To  recap  this  material,  we  discussed  the  limitations  encountered  when  using  traditional  2nd-order
quantized-coefficient IIR filters to perform narrowband lowpass filtering and mentioned the coupled-
form IIR filter that reduced those limitations albeit with an increased computational cost of doubling
the  number  of  multiplies  per  filter  output  sample.  Next  we  described,  and  then  demonstrated,  an
interpolated-IIR  filter  that  overcomes  the  shortcomings  of  traditional  lowpass  IIR  filters.  The
interpolated-IIR filter provides improved lowpass IIR filter performance while requiring no increase
in  filter  coefficient  bit  widths  and  no  additional  multiply  operations  beyond  a  traditional  IIR  filter.
When it comes to narrowband lowpass IIR filters, there’s a new sheriff in town.
13.51 A Stable Goertzel Algorithm
In  Section  13.17.1  we  discussed  the  computational  value  of  the  Goertzel  algorithm  for  computing
discrete Fourier transform (DFT) spectral components. However, we also mentioned that the Figure
13-42 complex resonator implementation of the Goertzel algorithm places resonator z-domain poles
on the z-plane’s unit circle. Having a resonator pole on the unit circle leads to potential instability
problems  because  we  cannot  represent  the  resonator’s  coefficients  with  infinite  precision.  We’re
forced to represent the coefficients as accurately as a fixed number of binary bits allows. This means
the resonator’s poles will not lie exactly on the unit circle. If an imprecise binary representation of
the  coefficient  2cos(2πm/N)  places  the  poles  slightly  inside  the  z-plane’s  unit  circle,  then  the
computed  X(m)  spectral  sample  will  contain  a  small  error.  Even  worse,  if  an  imprecise  binary
representation of 2cos(2πm/N) places the poles slightly outside the unit circle, then the resonator is
unstable. For this reason, typical applications of the Goertzel algorithm restrict the transform length N
to be in the hundreds.
One way to avoid those potential stability problems, and let N be any value we wish, is by way of a
heterodyning scheme. That is, instead of building an imperfect resonator centered at our frequency of
interest, 2′m/N radians/sample, we frequency translate our signal of interest down to zero frequency
where  we  can  build  a  perfect  resonator  as  shown  in  Figure 13-120(a).  We  say  “perfect  resonator”
because that resonator, centered at zero frequency (frequency index m = 0), has coefficients of two
and one, which can be represented by binary words with perfect precision.
Figure 13-120 Stable Goertzel algorithm: (a) resonator implementation; (b) z-plane poles and zero.
Such a resonator has a z-domain transfer function of
(13-178)
with a single z-domain zero located at z = 1 and two poles at z = 1 as shown in Figure 13-120(b).
One of the poles cancels the zero at z = 1. The advantages of the network in Figure 13-120(a) are that
it is guaranteed stable, and it exhibits no output error due to a pole or zero being slightly inside or
outside the z-plane unit circle.
Now the perceptive reader would factor Eq. (13-178) as
(13-179)
and redraw Figure 13-120(a) as shown in Figure 13-121(a).
Figure 13-121 Simplified stable Goertzel algorithm: (a) simplified resonator implementation; (b) z-
plane pole.
Figure 13-121(a) tells us that our desired X(m) = y(n) spectral sample is equal to the sum of the N
samples  output  by  the  multiplier  in  Figure 13-121(a).  (This  makes  perfect  sense  because  the  zero-
frequency spectral sample of an N-point DFT, X(0), is computed by merely summing a DFT’s N input
samples.) So our “stable Goertzel algorithm” now becomes quite simple.
Ah, but there’s trouble in paradise. The “weak link in the chain” of the Figure 13-121(a) network is
that we’re assuming the heterodyning sequence e−j2πm/N is ideal in its precision. If you’ve ever tried
to generate a complex e−j2πm/N sequence using binary arithmetic, you know that your sequence must be
quantized to some fixed number of bits, and thus have imperfect precision. That means the output of
your e−j2πm/N oscillator will either increase in magnitude, or decrease in magnitude, as time index n
increases.  However,  we  solve  that  problem  by  using  the  guaranteed-stable  quadrature  oscillator
described in Section 13.32.
It’s  fair  to  copy  a  slogan  from  the  Aston  Martin  automobile  company  and  say  that  the  Figure  13-
121(a)  Goertzel  algorithm,  using  the  stable  quadrature  oscillator,  is  “engineered  to  exceed  all
expectations.”
References
[1] Powell, S. “Design and Implementation Issues of All Digital Broadband Modems,” DSP World
Workshop Proceedings, Toronto, Canada, September 13–16, 1998, pp. 127–142.
[2]  Frerking,  M.  Digital  Signal  Processing  in  Communications  Systems,  Chapman  &  Hall,  New
York, 1994, p. 330.
[3]  Jacobsen,  E.,  Minister  of  Algorithms,  Abineau  Communications,  private  communication,
September 11, 2003.
[4] Palacherls, A. “DSP-mP Routine Computes Magnitude,” EDN, October 26, 1989.
[5] Mikami, N., Kobayashi, M., and Yokoyama, Y. “A New DSP-Oriented Algorithm for Calculation
of the Square Root Using a Nonlinear Digital Filter,” IEEE Trans. on Signal Processing, Vol. 40,
No. 7, July 1992.
[6]  Lyons,  R.  “Turbocharge  Your  Graphics  Algorithm,”  ESD:  The  Electronic  System  Design
Magazine, October 1988.
[7]  Adams  W.,  and  Brady,  J.  “Magnitude  Approximations  for  Microprocessor  Implementation,”
IEEE Micro, Vol. 3, No. 5, October 1983.
[8] Harris Semiconductor Corp. HSP50110 Digital Quadrature Tuner Data Sheet, File Number 3651,
February 1994.
[9] Sabin, W., and Schoenike, E., eds., Single Sideband Systems and Circuits, McGraw-Hill, New
York, 1987.
[10] Schreiner, C. “Subject: Re: Approximation for Sum of Two Squares,” Usenet group comp.dsp
post, October 4, 1999.
[11]  Filip,  A.  “Linear  Approximations  to  sqrt(x2+y2)  Having  Equiripple  Error  Characteristics,”
IEEE Trans. on Audio and Electroacoustics, December 1973, pp. 554–556.
[12] Bingham, C., Godfrey, M., and Tukey, J. “Modern Techniques for Power Spectrum Estimation,”
IEEE Trans. on Audio and Electroacoust., Vol. AU-15, No. 2, June 1967.
[13] Bergland, G. “A Guided Tour of the Fast Fourier Transform,” IEEE Spectrum Magazine, July
1969, p. 47.
[14]  Harris,  F.  “On  the  Use  of  Windows  for  Harmonic  Analysis  with  the  Discrete  Fourier
Transform,” Proceedings of the IEEE, Vol. 66, No. 1, January 1978.
[15]  Nuttall,  A.  “Some  Windows  with  Very  Good  Sidelobe  Behavior,”  IEEE  Trans.  on  Acoust.
Speech, and Signal Proc., Vol. ASSP-29, No. 1, February 1981.
[16] Cox, R. “Complex-Multiply Code Saves Clocks Cycles,” EDN, June 25, 1987.
[17] Rabiner, L., and Gold, B. Theory and Application of Digital Signal Processing, Prentice Hall,
Englewood Cliffs, New Jersey, 1975.
[18]  Sorenson,  H.,  Jones,  D.,  Heideman,  M.,  and  Burrus,  C.  “Real-Valued  Fast  Fourier  Transform
Algorithms,” IEEE Trans. on Acoust. Speech, and Signal Proc., Vol. ASSP-35, No. 6, June 1987.
[19]  Cooley,  J.,  Lewis,  P.,  and  Welch,  P.  “The  Fast  Fourier  Transform  Algorithm:  Programming
Considerations in the Calculation of Sine, Cosine and Laplace Transforms,” Journal Sound Vib.,
Vol. 12, July 1970.
[20]  Brigham,  E.  The  Fast  Fourier  Transform  and  Its  Applications,  Prentice  Hall,  Englewood
Cliffs, New Jersey, 1988.
[21] Burrus, C., et al. Computer-Based Exercises for Signal Processing, Prentice Hall, Englewood
Cliffs, New Jersey, 1994, p. 53.
[22]  Hewlett-Packard,  “The  Dynamic  Range  Benefits  of  Large-Scale  Dithered  Analog-to-Digital
Conversion, HP Product Note: 89400-7.
[23] Blesser, B., and Locanthi, B. “The Application of Narrow-Band Dither Operating at the Nyquist
Frequency  in  Digital  Systems  to  Provide  Improved  Signal-to-Noise  Ratio  over  Conventional
Dithering,” J. Audio Eng. Soc., Vol. 35, June 1987.
[24] Coleman, B., et al. “Coherent Sampling Helps When Specifying DSP A/D Converters,” EDN,
October 1987.
[25]  Ushani,  R.  “Classical  Tests  Are  Inadequate  for  Modern  High-Speed  Converters,”  EDN
Magazine, May 9, 1991.
[26]  Meehan,  P.,  and  Reidy,  J.  “FFT  Techniques  Give  Birth  to  Digital  Spectrum  Analyzer,”
Electronic Design, August 11, 1988, p. 120.
[27] Beadle, E. “Algorithm Converts Random Variables to Normal,” EDN Magazine, May 11, 1995.
[28] Spiegel, M. Theory  and  Problems  of  Statistics,  Shaum’s  Outline  Series,  McGraw-Hill,  New
York, 1961, p. 142.
[29] Davenport, W., Jr., and Root, W. Random Signals and Noise, McGraw-Hill, New York, 1958.
[30] Salibrici, B. “Fixed-Point DSP Chip Can Generate Real-Time Random Noise,” EDN Magazine,
April 29, 1993.
[31] Marsaglia, G., and Tsang, W. “The Ziggurat Method for Generating Random Variables,” Journal
of Statistical Software, Vol. 5, No. 8, 2000.
[32] http://finmath.uchicago.edu/~wilder/Code/random/Papers/Marsaglia_00_ZMGRV.pdf.
[33] http://www.jstatsoft.org/v05/i08/ziggurat.pdf.
[34]  Donadio,  M.  “Lost  Knowledge  Refound:  Sharpened  FIR  Filters,”  IEEE  Signal  Processing
Magazine, Vol. 20, No. 5, September 2003, pp. 61–63.
[35] Kwentus, A., et al. “Application of Filter Sharpening to Cascaded Integrator-Comb Decimation
Filters,” IEEE Transactions on Signal Processing, Vol. 45, February 1997, pp. 457–467.
[36]  Gentili,  P.,  et  al.  “Improved  Power-of-Two  Sharpening  Filter  Design  by  Genetic  Algorithm,”
1996  IEEE  Conference  on  Acoustics,  Speech  and  Signal  Processing  (ICASSP  ‘96),  Atlanta,
Georgia, Vol. 3, 1996, p. 1375.
[37]  Graychip  Inc.  “Upconverting  Signals  with  the  GC2011  for  Easier  Digital  to  Analog
Conversion,” Application Note: GC2011-AN9804, December 20, 1998.
[38] Donadio, M., private communication, September 11, 2003.
[39]  Jacobsen,  E.,  and  Kootsookos,  P.  “Fast,  Accurate  Frequency  Estimators,”  IEEE  Signal
Processing Magazine, “DSP Tips & Tricks” column, Vol. 24, No. 3, May 2007.
[40] Nagai, K. “Pruning the Decimation-in-Time FFT Algorithm with Frequency Shift,” IEEE Trans.
on ASSP, Vol. ASSP-34, August 1986, pp. 1008–1010.
[41]  Skinner,  D.  “Pruning  the  Decimation-in-Time  FFT  Algorithm,”  IEEE  Trans.  on  ASSP,  Vol.
ASSP-24, April 1976, pp. 193–194.
[42]  Markel,  J.  D.  “FFT  Pruning,”  IEEE  Trans  on  Audio  Electroacoust.,  Vol.  AU-19,  December
1971, pp. 305–311.
[43] Sreenivas, T., and Rao, P. “FFT Algorithm for Both Input and Ouput Pruning,” IEEE Trans. on
ASSP, Vol. ASSP-27, June 1979, pp. 291–292.
[44] Lyons, R. “Program Aids Analysis of FFT Algorithms,” EDN Magazine, August 6, 1987.
[45] Goertzel, G. “An Algorithm for the Evaluation of Finite Trigonometric Series,” American Math.
Monthly, Vol. 65, 1958, pp. 34–35.
[46]  Proakis,  J.,  and  Manolakis,  D.  Digital  Signal  Processing:  Principles,  Algorithms,  and
Applications, 3rd ed., Prentice Hall, Upper Saddle River, New Jersey, 1996, pp. 480–481.
[47] Oppenheim, A., Schafer, R., and Buck, J. Discrete-Time Signal Processing, 2nd ed., Prentice
Hall, Upper Saddle River, New Jersey, 1999, pp. 633–634.
[48] Farhang-Boroujeny, B., and Lim, Y. “A Comment on the Computational Complexity of Sliding
FFT,” IEEE Trans. Circuits and Syst. II, Vol. 39, No. 12, December 1992, pp. 875–876.
[49]  Farhang-Boroujeny,  B.,  and  Gazor,  S.  “Generalized  Sliding  FFT  and  Its  Application  to
Implementation of Block LMS Adaptive Filters,” IEEE Trans. Sig. Proc., Vol. 42, No. 3, March
1994, pp. 532–538.
[50] Douglas, S., and Soh, J. “A Numerically-Stable Sliding-Window Estimator and Its Application
to  Adaptive  Filters,”  Proc.  31st  Annual  Asilomar  Conf.  on  Signals,  Systems,  and  Computers,
Pacific Grove, California, Vol. 1, November 1997, pp. 111–115.
[51] Crochiere, R., and Rabiner, L. Multirate Digital Signal Processing, Prentice Hall, Englewood
Cliffs, New Jersey, 1983, pp. 315–319.
[52] Zoran Corp. “Vernier Spectral Analysis with the ZR34161 Vector Signal Processor,” Tech. Note
ZAN34003, Santa Clara, California, 1989.
[53]  Gumas,  C.  “Window-Presum  FFT  Achieves  High-Dynamic  Range,  Resolution,”  Personal
Engineering and Instrumentation News, July 1997, pp. 58–64.
[54] Hack, T. “IQ Sampling Yields Flexible Demodulators,” RF Design, April 1991.
[55] Bateman, A. “Quadrature Frequency Discriminator,” GlobalDSP Magazine, October 2002.
[56] http://aulos.calarts.edu/pipermail/test/1998-March/001028.html.
[57]  Dick,  C.,  and  Harris,  F.  “FPGA  Signal  Processing  Using  Sigma-Delta  Modulation,”  IEEE
Signal Proc. Magazine, Vol. 17, No. 1, January 2000.
[58]  Bateman,  A.  “Implementing  a  Digital  AC  Coupling  Filter,”  GlobalDSP  Magazine,  February
2003.
[59]  Shenoi,  K.  Digital  Signal  Processing  in  Communications  Systems,  Chapman  &  Hall,  New
York, 1994, p. 275.
[60]  Bristow-Johnson,  R.  “Subject:  Fixed-PointDC  Blocking  Filter  with  Noise  Shaping,”  Usenet
group comp.dsp post, June 22, 2000.
[61] Bristow-Johnson, R. “Subject: Virtues of Noise Shaping,” Usenet group comp.dsp post, August
21, 2001.
[62] Ascari, L., et al. “Low Power Implementation of a Sigma Delta Decimation Filter for Cardiac
Applications,”  IEEE  Instrumentation  and  Measurement  Technology  Conference,  Budapest,
Hungary, May 21–23, 2001, pp. 750–755.
[63] Gao, Y., et al. “Low-Power Implementation of a Fifth-Order Comb Decimation Filter for Multi-
Standard  Transceiver  Applications,”  Int.  Conf.  on  Signal  Proc.  Applications  and  Technology
(ICSPAT), Orlando, Florida, 1999.
[64] Gao, Y., et al. “A Comparison Design of Comb Decimators for Sigma-Delta Analog-to-Digital
Converters,” Int. Journal: Analog Integrated Circuits and Signal Processing, Kluwer Academic
Publishers, ISSN: 0925–1030, 1999.
[65]  Ballanger,  M.,  et  al.  “Digital  Filtering  by  Polyphase  Network:  Application  to  Sample-Rate
Alteration and Filter Banks,” IEEE  Trans.  on  Acoust.  Speech,  and  Signal  Proc.,  Vol.  ASSP-24,
No. 2, April 1976, pp. 109–114.
[66]  Brandt,  B.,  and  Wooley,  B.  “A  Low-Power  Area-Efficient  Digital  Filter  for  Decimation  and
Interpolation,” IEEE Journ. of Solid-State Circuits, Vol. 29, June 1994, pp. 679–687.
[67]  Willson,  A.,  Jr.  “A  Programmable  Interpolation  Filter  for  Digital  Communications
Applications,” Final report for MICRO Project 96–149, UCLA, 1996–1997.
[68] Dumonteix, Y., et al. “Low Power Comb Decimation Filter Using Polyphase Decomposition for
Mono-Bit  ΣΔ  Analog-to-Digital  Converters,”  Int.  Conf.  on  Signal  Processing  Applications  and
Technology (ICSPAT), San Jose, California, 2000.
[69]  Yang,  H.,  and  Snelgrove,  W.  “High  Speed  Polyphase  CIC  Decimation  Filters,”  IEEE  Int.
Symposium on Circuits and Systems, Vol. 2, 1996, pp. 229–232.
[70]  Jang,  Y.,  and  Yang,  S.  “Non-Recursive  Cascaded  Integrator-Comb  Decimation  Filters  with
Integer Multiple Factors,” 44th IEEE Midwest Symposium on Circuits and Systems (MWSCAS),
Dayton, Ohio, August 2001.
[71]  Dvorak,  R.  “Software  Filter  Boosts  Signal-Measurement  Stability,  Precision,”  Electronic
Design, February 3, 2003.
[72] Lynn, P., and Fuerst, W. Introductory Digital Signal Processing, with Computer Applications,
John Wiley and Sons, New York, 1997, pp. 285–297.
[73] Givens, M., private communication, October 12, 2004.
[74] Fraser, D. “Interpolation by the FFT Revisited—An Experimental Investigation,” IEEE Trans.
on Acoust. Speech, and Signal Proc., Vol. ASSP-37, No. 5, May 1989, pp. 665–676.
[75]  Marple,  S.,  Jr.  “Computing  the  Discrete-Time  ‘Analytic’  Signal  via  FFT,”  IEEE  Trans.  on
Signal Proc., Vol. 47, No. 9, September 1999, pp. 2600–2603.
[76]  Harris,  F.  “T102:  Digital  Signal  Processing  for  Digital  Modems,”  DSP  World  Spring  Design
Conf., Santa Clara, California, April 1999.
[77]  Harris,  F.  “On  the  Design,  Implementation,  and  Performance  of  a  Microprocessor-Controlled
AGC System for a Digital Receiver,” IEEE Military Communications Conf., San Diego, Caliornia,
October 1988.
[78]  Analog  Devices,  Inc.  “80  MSPS,  Dual-Channel  WCDMA  Receive  Signal  Processor  (RSP)
AD6634,” Data Sheet Rev. 0, 2002, pp. 28–34.
[79]  Turner,  C.  “Recursive  Discrete-Time  Sinusoidal  Oscillators,”  IEEE  Signal  Processing
Magazine, Vol. 20, No. 3, May 2003, pp. 103–111.
[80]  Paillard,  B.,  and  Boudreau,  A.  “Fast,  Continuous,  Sinewave  Generator,”  GlobalDSP  On-line
Magazine, December 2003.
[81] Vassilevsky, V. “Efficient Multi-tone Detection,” IEEE Signal Processing Magazine, “DSP Tips
& Tricks” column, Vol. 24 , No. 2, March 2007.
[82]  Shiung,  D.,  Ferng,  H.,  and  Lyons,  R.  “Filtering  Tricks  for  FSK  Demodulation,”  IEEE  Signal
Processing Magazine, “DSP Tips & Tricks” column, Vol. 22, No. 3, May 2005.
[83] Spiegel, M. Statistics, Shaum’s Outline Series, McGraw-Hill, New York, 1961, p. 77.
[84]  Hadstate,  J.  “Subject:  Re:  Question  about  Computing  a  “Moving  Variance,”  Usenet  group
comp.dsp post, March 1, 2005.
[85]  Turner,  C.  “Subject:  Re:  Question  About  Computing  a  “Moving  Variance,”  Usenet  group
comp.dsp post, February 27, 2005.
[86] Jackson, L. “On the Relationship Between Digital Hilbert Transformers and Certain Low-Pass
Filters,” IEEE Trans. on Acoust. Speech, and Signal Proc., Vol. ASSP-23, No. 4, August 1975.
[87]  Harris,  F.  Multirate  Signal  Processing  for  Communication  Systems,  Prentice  Hall,  Upper
Saddle River, New Jersey, 2004, pp. 210–212.
[88]  Turner,  C.  “Subject:  How  Do  You  Rotate  a  Phasor  by  pi/8  Radians,”  Usenet  group  comp.dsp
post, May 29, 2002.
[89] Press, W., et al., Numerical Recipes in C: The Art of Scientific Computing, 2nd ed. Cambridge
University Press, New York, 1992, p. 177.
[90]  Losada,  R.,  and  Lyons,  R.  “Reducing  CIC  Filter  Complexity,”  IEEE  Signal  Processing
Magazine, “DSP Tips and Tricks” column, Vol. 23, No. 4, July 2006.
[91] Ohlsson, H., et al. “Design of a Digital Down Converter Using High Speed Digital Filters,” in
Proc. Symp. on Gigahertz Electronics, Gothenburg, Sweden, March 13–14, 2000, pp. 309–312.
[92] Turner, C. “Slope Filtering: An FIR Approach to Linear Regression,” IEEE Signal Processing
Magazine, “DSP Tips & Tricks” column, Vol. 25, No. 6, November 2008.
[93] Bell, D. “Subject: Re: Frequency Inversion,” Usenet group comp.dsp post, August 30, 2006.
[94]  Adams,  R.  “Nonuniform  Sampling  of  Audio  Signals,”  J.  Audio  Eng.  Soc.,  Vol.  40,  No.  11,
November 1992, pp. 886–894.
[95]  Bariska,  A.  “Recovering  Periodically-Spaced  Missing  Samples,”  IEEE  Signal  Processing
Magazine, “DSP Tips and Tricks” column, Vol. 24, No. 6, November 2007.
[96]  Moshe,  S.,  and  Hertz,  D.  “On  Computing  DFT  of  Real  N-Point  Vector  and  IDFT  of  DFT-
Transformed Real N-Point Vector via Single DFT,” IEEE Signal Processing Letters, IEEE, Vol. 6,
No. 6, June 1999, p. 141.
[97]  Proakis,  J.,  and  Manolakis,  D.  Digital  Signal  Processing:  Principles,  Algorithms,  and
Applications, 3rd ed., Prentice Hall, Upper Saddle River, New Jersey, 1996, pp. 572–576.
[98]  Oppenheim,  A.,  and  Schafer,  R.  Discrete-Time  Signal  Processing,  2nd  ed.,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1989, pp. 382–386.
[99]  Gold,  B.,  and  Rader,  C.  “Effects  of  Parameter  Quantization  on  the  Poles  of  a  Digital  Filter,”
Proceedings of the IEEE, Vol. 55, May 1967, pp. 688–689.
[100] Harris, F., and Lowdermilk, W. “Implementing Recursive Filters with Large Ratio of Sample
Rate  to  Bandwidth,”  in  Conference  Record  of  the  Forty-first  Asilomar  Conference  on  Signals,
Systems and Computers, Pacific Grove, California, November 4–7, 2007, pp. 1149–1153.
