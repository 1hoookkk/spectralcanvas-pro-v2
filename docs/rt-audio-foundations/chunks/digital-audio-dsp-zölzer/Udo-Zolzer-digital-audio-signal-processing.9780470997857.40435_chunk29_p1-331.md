# Digital Audio DSP (Zölzer) — Chunk 29/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 15902

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
• Example 2: w = 8 → MZ = 28
TSO
TSI
= 28MI + (z − z0)
2827
257
(8.51)
With a precision of 15 bits, the averaging number is chosen as MO = 27 and the
number MI and the counter outputs have to be determined.
The sampling rates at the input and output of a sampling rate converter can be calculated
by evaluating the 8-bit increment of the counter for each output clock with
z = TSO
TSI
MZ = fSI
fSO
256,
(8.52)
as seen from Table 8.1.
Table 8.1 Counter increments for different sampling rate conversions.
Conversion/kHz
32 → 48
44.1 → 48
32 → 44.1
48 → 44.1
48 → 32
44.1 → 32
8-bit counter increment
170
235
185
278
384
352
8.4 Interpolation Methods
In the following sections, special interpolation methods are discussed. These methods
enable the calculation of time-variant ﬁlter coefﬁcients for sampling rate conversion and
need an oversampled input sequence as well as the time instant of the output sample. A
convolution of the oversampled input sequence with time-variant ﬁlter coefﬁcients gives the
output sample at the output sampling rate. This real-time computation of ﬁlter coefﬁcients
is not based on popular ﬁlter design methods. On the contrary, methods are presented for
calculating ﬁlter coefﬁcient sets for every input clock cycle where the ﬁlter coefﬁcients
are derived from the distance of output samples to the time grid of the oversampled input
sequence.
8.4.1 Polynomial Interpolation
The aim of a polynomial interpolation [Liu92] is to determine a polynomial
pN (x) =
N(cid:6)
i=0
aixi
(8.53)
258
Sampling Rate Conversion
of Nth order representing exactly a function f (x) at N + 1 uniformly spaced xi, i.e.
pN (xi) = f (xi) = yi for i = 0, . . . , N. This can be written as a set of linear equations






1
1
...
1
x2
x0
0
x2
x1
1
...
...
xN xN
N
· · ·
· · ·
· · ·
















a0
a1
...
aN
xN
0
xN
1
...
xN
N




 .





y0
y1
...
yN
=
(8.54)
The polynomial coefﬁcients ai as functions of y0, . . . , yN are obtained with the help of
Cramer’s rule according to
1
(cid:16)
(cid:16)
x2
x0
(cid:16)
0
(cid:16)
(cid:16)
x2
1
x1
(cid:16)
1
(cid:16)
...
...
...
(cid:16)
(cid:16)
(cid:16)
1 xN x2
N
· · ·
· · ·
· · ·
· · ·
ith column
· · ·
y0
y1
...
· · ·
yN · · ·
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
xN
0
xN
1
...
xN
N
ai =
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
x2
1
x0
0
x2
1
x1
1
...
...
...
1 xN x2
N
· · ·
· · ·
· · ·
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
xN
0
xN
1
...
xN
N
,
i = 0, 1, . . . , N.
(8.55)
For uniformly spaced xi = i with i = 0, 1, . . . , N the interpolation of an output sample
with distance α gives
y(n + α) =
N(cid:6)
i=0
ai(n + α)i .
(8.56)
In order to determine the relationship between the output sample y(n + α) and yi, a set of
time-variant coefﬁcients ci needs to be determined such that
y(n + α) =
N/2(cid:6)
i=−N/2
ci (α)y(n + i).
(8.57)
The calculation of time-variant coefﬁcients ci(α) will be illustrated by an example.
Example: Figure 8.20 shows the interpolation of an output sample of distance α with
N = 2 and using three samples which can be written as
y(n + α) =
2(cid:6)
i=0
ai(n + α)i .
(8.58)
8.4 Interpolation Methods
259
"7(cid:10)9(cid:28):
"7(cid:10)(cid:5)(cid:22):
"7(cid:10):
"7(cid:10)9(cid:22):
(cid:28)
’
’7(cid:10)(cid:5)(cid:22):
’7(cid:10):
’7(cid:10)9(cid:22):
Figure 8.20 Polynomial interpolation with three samples.
The samples y(n + i), with i = −1, 0, 1, can be expressed as
y(n + 1) =
y(n) =
y(n − 1) =
2(cid:6)
i=0
2(cid:6)
i=0
2(cid:6)
i=0
ai(n + 1)i,
α = 1,
aini ,
α = 0,
ai(n − 1)i,
α = −1,
(8.59)
or in matrix notation


1 (n + 1)
1
1 (n − 1)
n
(n + 1)2
n2
(n − 1)2





a0
a1
a2
 =


y(n + 1)
y(n)
y(n − 1)

 .
The coefﬁcients ai as functions of yi are then given by


 =


a0
a1
a2







n(n − 1)
2
− 2n − 1
2
1
2
1 − n2
2n
−1
n(n + 1)
2
− 2n + 1
2
1
2










y(n + 1)
y(n)
y(n − 1)

 ,
(8.60)
(8.61)
such that
y(n + α) = a0 + a1(n + α) + a2(n + α)2
(8.62)
is valid. The output sample y(n + α) can be written as
y(n + α) =
1(cid:6)
i=−1
ci(α)y(n + i)
= c−1y(n − 1) + c0y(n) + c1y(n + 1).
(8.63)
260
Sampling Rate Conversion
Equation (8.62) with ai from (8.61) leads to
(cid:4)
y(n + α) =
1
2
y(n + 1) − y(n) + 1
2
(cid:4)
(cid:5)
(n + α)2
y(n − 1)
y(n + 1) + 2ny(n) − 2n + 1
y(n + 1) + (1 − n2)y(n) + n(n + 1)
2
y(n − 1)
+
− 2n − 1
2
+ n(n − 1)
2
(cid:5)
(n + α)
y(n − 1).
(8.64)
2
Comparing the coefﬁcients from (8.63) and (8.64) for n = 0 gives the coefﬁcients
2 α(α − 1),
c−1 = 1
c0 = −(α − 1)(α + 1) = 1 − α2,
c1 = 1
2 α(α + 1).
8.4.2 Lagrange Interpolation
Lagrange interpolation for N + 1 samples makes use of the polynomials li (x) which have
the following properties (see Fig. 8.21):
li(xk) = δik =
(cid:21)
1,
0,
i = k,
elsewhere.
(8.65)
Based on the zeros of the polynomial li(x), it follows that
li(x) = ai(x − x0) · · ·(x − xi−1)(x − xi+1) · · ·(x − xN ).
(8.66)
With li(xi) = 1 the coefﬁcients are given by
ai(xi) =
1
(xi − x0) · · ·(x i − xi−1)(xi − xi+1) · · · (xi − xN )
.
(8.67)
The interpolation polynomial is expressed as
pN (x) =
N(cid:6)
i=0
li (x)yi = l0(x)y0 + · · · + lN (x)yN .
(8.68)
,
With a =
N
j =0(x − xj ), (8.66) can be written as
,
li (x) = ai
a
x − xi
=
,
1
N
j =0,j (cid:8)=i xi − xj
N
j =0 x − xj
x − xi
For uniformly spaced samples
xi = x0 + ih
and with the new variable α as given by
x = x0 + αh,
N%
=
j =0,j (cid:8)=i
x − xj
xi − xj
.
(8.69)
(8.70)
(8.71)
261
(8.72)
(8.73)
(8.74)
(8.75)
(8.76)
8.4 Interpolation Methods
we get
and hence
For even N we can write
and for odd N,
x − xj
xi − xj
= (x0 + αh) − (x0 + j h)
(x0 + ih) − (x0 + j h)
= α − j
i − j
li(x(α)) =
N%
j =0,j (cid:8)=i
α − j
i − j
.
li (x(α)) =
N/2%
j =−N/2,j (cid:8)=i
α − j
i − j
,
li(x(α)) =
N+1/2%
j =−N−1/2,j (cid:8)=i
α − j
i − j
.
The interpolation of an output sample is given by
y(n + α) =
N/2(cid:6)
i=−N/2
li (α)y(n + i).
(cid:4)(cid:14)7’:
(cid:16)
’
3
’
(cid:22)
’
(cid:23)
(cid:22)
’ (cid:16)
’
’
(cid:24)
Figure 8.21 Lagrange polynomial.
Example: For N = 2, 3 samples,
l−1(x(α)) =
1%
α − j
−1 − j
= 1
2
α(α − 1),
l0(x(α)) =
l1(x(α)) =
α − j
0 − j
j =−1,j (cid:8)=−1
1%
j =−1,j (cid:8)=0
1%
j =−1,j (cid:8)=1
α − j
1 − j
= 1
2
α(α + 1).
= −(α − 1)(α + 1) = 1 − α2,
8.4.3 Spline Interpolation
The interpolation using piecewise deﬁned functions that only exist over ﬁnite intervals
is called spline interpolation [Cuc91]. The goal is to compute the sample y(n + α) =
(cid:11)
N/2
i=−N/2 bN
i (α)y(n + i) from weighted samples y(n + i).
(8.77)
(8.78)
262
Sampling Rate Conversion
A B-spline M N
k (x) of Nth order using m + 1 samples is deﬁned in the interval [xk, . . . ,
xk+m] by
M N
k (x) =
with the truncated power functions
aiφi(x)
k+m(cid:6)
i=k
(cid:21)
φi(x) = (x − xi)N
+ =
0,
x < xi,
(x − xi)N , x ≥ xi.
m
i=0 aiφi(x) will be considered for k = 0 where M N
(cid:11)
0 (x) = 0
0 (x) =
In the following M N
0 (x) = 0 for x ≥ xm. Figure 8.22 shows the truncated power functions
for x < x0 and M N
and the B-spline of Nth order. With the deﬁnition of the truncated power functions we can
write
M N
0 (x) = a0φ0(x) + a1φ1(x) + · · · + amφm(x)
= a0(x − x0)N
+ + a1(x − x1)N
+ + · · · + am(x − xm)N
+ ,
(8.79)
and after some calculations we get
+ c1xN−1
0
+ c1xN−1
1
x + · · · + cN−1x0xN−1 + xN )
x + · · · + cN−1x1xN−1 + xN )
M N
0 (x) = a0(xN
0
+ a1(xN
1
...
+ am(xN
m
+ c1xN−1
m x + · · · + cN−1xmxN−1 + xN ).
(8.80)
(cid:30)
7’:
3
(cid:30)
7’:
(cid:22)
(cid:30)
7’:
(cid:23)
(cid:30)
(cid:12)
7’:
’
3
’
(cid:22)
’
(cid:23)
’
(cid:12)
’
’(cid:12)9(cid:22)
(cid:24)
(cid:14)7’:/3
’
3
’
(cid:22)
’
(cid:23)
’
(cid:12)
’
’(cid:12)9(cid:22)
Figure 8.22 Truncated power functions and the B-spline of Nth order.
With the condition M N
0 (x) = 0 for x ≥ xm, the following set of linear equations can be
written with (8.80) and the coefﬁcients of the powers of x:







1
x0
x2
0
...
xN
0
1
x1
x2
1
...
xN
1
· · ·
· · ·
· · ·
· · ·
1
xm
x2
m
...
xN
m





















a0
a1
a2
...
am
=








0

0


0


...

0
.
(8.81)
8.4 Interpolation Methods
263
The homogeneous set of linear equations has non-trivial solutions for m > N. The mini-
mum requirement results in m = N + 1. For m = N + 1, the coefﬁcients [Boe93] can be
obtained as follows:
ai =
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
1
x0
...
xN
0
1
x1
...
xN
1
1
x2
...
xN
2
· · ·
· · ·
· · ·
ith column
0 · · ·
0 · · ·
...
0 · · ·
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
1
xN+1
...
xN
N+1
(cid:16)
(cid:16)
1
(cid:16)
(cid:16)
x0
(cid:16)
(cid:16)
...
(cid:16)
(cid:16)
(cid:16)
xN+1
0
1
x1
...
xN+1
1
1
x2
...
xN+1
2
· · ·
· · ·
· · ·
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
1
xN+1
...
xN+1
N+1
,
i = 0, 1, . . . , N + 1.
(8.82)
Setting the ith column of the determinant in the numerator of (8.82) equal to zero cor-
responds to deleting the column. Computing both determinants of Vandermonde matrices
[Bar90] and division leads to the coefﬁcients
and hence
For some k we obtain
ai =
,
1
N+1
j =0,i(cid:8)=j (xi − xj )
M N
0 (x) =
N+1(cid:6)
i=0
,
(x − xi)N
+
N+1
j =0,i(cid:8)=j (xi − xj )
.
M N
k (x) =
k+N+1(cid:6)
i=k
,
(x − xi)N
+
N+1
j =0,i(cid:8)=j (xi − xj )
.
(8.83)
(8.84)
(8.85)
Since the functions M N
= (xk+N+1 − xk)M N
k (x) decrease with increasing N, a normalization of the form N N
k (x)
k is done, such that for equidistant samples we get
N N
k (x) = (N + 1) · M N
k (x).
(8.86)
The next example illustrates the computation of B-splines.
264
Sampling Rate Conversion
Example: For N = 3, m = 4, and ﬁve samples the coefﬁcients according to (8.83) are given
by
a0 =
a1 =
a2 =
a3 =
a4 =
1
(x0 − x4)(x0 − x3)(x0 − x2)(x0 − x1)
1
(x1 − x4)(x1 − x3)(x1 − x2)(x1 − x0)
1
(x2 − x4)(x2 − x3)(x2 − x1)(x2 − x0)
1
(x3 − x4)(x3 − x2)(x3 − x1)(x3 − x0)
1
(x4 − x3)(x4 − x2)(x2 − x1)(x3 − x0)
,
,
,
,
.
Figure 8.23a,b shows the truncated power functions and their summation for calculating
N 3
0 (x). In Fig. 8.23c the horizontally shifted N 3
i (x) are depicted.
a)
b)
c)
1
0
-1
0
1
0
-1
0
1
0
-1
0
(cid:73)(cid:19)
(cid:73)(cid:20)
(cid:73)(cid:21)
(cid:73)(cid:22)
(cid:73)(cid:23)
a(cid:19)
(cid:17)(cid:73)(cid:19)
3(x)
N0
a2
(cid:17)(cid:73)(cid:21)
a1
(cid:17)(cid:73)(cid:20)
a3
(cid:17)(cid:73)(cid:22)
3(x) N1
3(x) N2
3(x) N3
3(x)
N0
a4
(cid:17)(cid:73)(cid:23)
5
5
3(x)
N4
2
4
6
8
Figure 8.23 Third-order B-spline (N = 3, m = 4, 5 samples).
8.4 Interpolation Methods
265
y(n+ )(cid:31)
(cid:31)
2
N (x)
0
N1
2(x) N (x)
2
2
x-1
xn-3
x0
xn-2
x1
xn-1
x2
xn
x3
xn+1
x4
xn+2
x5
xn+3
x6
xn+4
x7
xn+5
y(n+ )(cid:31)
(cid:31)
3
N (x)
0
3
N (x)
1
3
N (x)
2
3
N (x)
3
x
x
x0
xn-3
x1
xn-2
x2
xn-1
x3
xn
x4
xn+1
x5
xn+2
x6
xn+3
x7
x8
xn+4
xn+5
Figure 8.24 Interpolation with B-splines of second and third order.
A linear combination of B-splines is called a spline. Figure 8.24 shows the interpolation
of sample y(n + α) for splines of second and third order. The shifted B-splines N N
i (x) are
evaluated at the vertical line representing the distance α. With sample y(n) and the normal-
ized B-splines N N
i (x), the second- and third-order splines are respectively expressed as
and
y(n + α) =
y(n + α) =
1(cid:6)
i=−1
2(cid:6)
i=−1
y(n + i)N 2
n−1+i (α)
(8.87)
y(n + i)N 3
n−2+i(α).
(8.88)
The computation of a second-order B-spline at the sample index α is based on the
symmetry properties of the B-spline, as depicted in Fig. 8.25. With (8.77), (8.86) and the
266
Sampling Rate Conversion
symmetry properties shown in Fig. 8.25, the B-splines can be written in the form
N 2
2 (α) = N 2
0 (α) = 3
N 2
1 (1 + α) = N 2
0 (1 + α) = 3
N 2
0 (2 + α) = N 2
0 (1 − α) = 3
3(cid:6)
i=0
3(cid:6)
i=0
3(cid:6)
i=0
ai(α − xi)2
+,
ai(1 + α − xi)2
+,
ai(2 + α − xi)2
+ = 3
3(cid:6)
i=0
ai(1 − α − xi)2
+.
(8.89)
With (8.83) we get the coefﬁcients
a0 =
a1 =
a2 =
and thus
1
(0 − 1)(−2)(−3)
1
(1 − 0)(1 − 2)(1 − 3)
1
(2 − 0)(2 − 1)(2 − 3)
,
= − 1
6
= 1
2
= − 1
2
,
,
(8.90)
N 2
N 2
N 2
2 α2,
2 (α) = 3[a0α2] = − 1
1 (α) = 3[a0(1 + α)2 + a1α2] = − 1
2 (1 − α)2.
0 (α) = 3[a0(1 − α)2] = − 1
2 (1 + α)2 + 3
2 α2,
(cid:31)
h(1)=N ( )
2 (cid:31)
2 (cid:31) =N ( )
0
2
0
1
2
1+(cid:31)
0
1
2
1-(cid:31)
0
1
2
2
h(2)=N (1+ )
1
2
(cid:31)
(cid:31) =N (1+ )
0
2
(cid:31) =
h(3)=N (2+ )
0
N (1- )=
(cid:31)
2
0
(8.91)
(cid:31)
N (1- )
2
2
Figure 8.25 Exploiting the symmetry properties of a second-order B-spline.
Owing to the symmetrical properties of the B-splines, the time-variant coefﬁcients of
the second-order B-spline can be derived as
N 2
N 2
N 2
2 (α) = h(1) = − 1
1 (α) = h(2) = − 1
0 (α) = h(3) = − 1
2 α2,
2 (1 + α)2 + 3
2 (1 − α)2.
2 α2,
(8.92)
(8.93)
(8.94)
8.4 Interpolation Methods
In the same way the time-variant coefﬁcients of a third-order B-spline are given by
N 3
N 3
N 3
N 3
3 (α) = h(1) = 1
2 (α) = h(2) = 1
1 (α) = h(3) = 1
0 (α) = h(4) = 1
6 α3,
6 (1 + α)3 − 2
6 (2 − α)3 − 2
6 (1 − α)3.
3 α3,
3 (1 − α)3,
Higher-order B-splines are given by
y(n + α) =
y(n + α) =
y(n + α) =
2(cid:6)
i=−2
3(cid:6)
i=−2
3(cid:6)
i=−3
y(n + i)N 4
n−2+i (α),
y(n + i)N 5
n−3+i (α),
y(n + i)N 6
n−3+i (α).
267
(8.95)
(8.96)
(8.97)
(8.98)
(8.99)
(8.100)
(8.101)
Similar sets of coefﬁcients can be derived here as well. Figure 8.26 illustrates this for
fourth- and sixth-order B-splines.
Generally, for even orders we get
y(n + α) =
N/2(cid:6)
i=−N/2
N N
N/2+i (α)y(n + i),
(8.102)
and for odd orders
y(n + α) =
(N+1)/2(cid:6)
i=−(N−1)/2
N N
(N−1)/2+i(α)y(n + i).
(8.103)
For the application of interpolation the properties in the frequency domain are important.
The zero-order B-spline is given by
N 0
0 (x) =
1(cid:6)
i=0


aiφi(x) =

0, x < 0,
1, 0 ≤ x < 1,
0, x ≥ 1,
(8.104)
and the Fourier transform gives the sinc function in the frequency domain. The ﬁrst-order
B-spline given by
N 1
0 (x) = 2
2(cid:6)
i=0
aiφi (x) =


0,
1
x,
2

1 − 1
2
0,
x < 0,
0 ≤ x < 1,
1 ≤ x < 2,
x ≥ 2,
x,
(8.105)
268
Sampling Rate Conversion
y(n+ )(cid:31)
(cid:31)
4
N (x)
2
4
N (x)
3
4
N (x)
4
N0
4(x) N (x)
4
1
x0
x1
xn-3
x2
xn-2
x3
xn-1
x4
xn
x5
x6
x7
x8
x9
xn+1
xn+2
xn+3
xn+4
xn+5
x
y(n+ )(cid:31)
(cid:31)
6
N (x)
0
6
N (x)
1
6
N (x)
2
6
N (x)
3
6
N (x)
4
6
N (x)
5
6
N (x)
6
x0
x1
x2
xn-4
x3
xn-3
x4
xn-2
x5
xn-1
x6
xn
x7
x8
x9
xn+1
xn+2
xn+3
x
Figure 8.26 Interpolation with B-splines of fourth and sixth order.
leads to a sinc2 function in the frequency domain. Higher-order B-splines can be derived
by repeated convolution [Chu92] as given by
N N (x) = N 0(x) ∗ N N−1(x).
(8.106)
Thus, the Fourier transform leads to
FT[N N (x)] = sincN+1(f ).
(8.107)
With the help of the properties in the frequency domain, the necessary order of the spline
interpolation can be determined. Owing to the attenuation properties of the sincN+1(f )
function and the simple real-time calculation of the coefﬁcients, spline interpolation is well
suited to time-variant conversion in the last stage of a multistage sampling rate conversion
system [Zöl94].
8.5 Exercises
8.5 Exercises
1. Basics
269
Consider a simple sampling rate conversion system with a conversion rate of 4
consists of two upsampling blocks, each by 2, and one downsampling block by 3.
3 . The system
1. What are anti-imaging and anti-aliasing ﬁlters and where do we need them in our
system?
2. Sketch the block diagram.
3. Sketch the input, intermediate and output spectra in the frequency domain.
4. How is the amplitude affected by the up- and downsampling and where does it come
from?
5. Sketch the frequency response of the anti-aliasing and anti-imaging ﬁlters needed for
this upsampling system.
2. Synchronous Conversion
Our system will now be upsampled directly by a factor of 4, and again downsampled by
a factor of 3, but with linear interpolation and decimation methods. The input signal is
x(n) = sin(nπ/6), n = 0, . . . , 48.
1. What are the impulse responses of the two interpolation ﬁlters? Sketch their magni-
tude responses.
2. Plot the signals (input, intermediate and output signal) in the time domain using
Matlab.
3. What is the delay resulting from the causal interpolation/decimation ﬁlters?
4. Show the error introduced by this interpolation/decimation method, in the frequency
domain.
3. Polyphase Representation
Now we extend our system using a polyphase decomposition of the interpolation/decima-
tion ﬁlters.
1. Sketch the idea of polyphase decomposition using a block diagram. What is the
beneﬁt of such decomposition?
2. Calculate the polyphase ﬁlters for up- and downsampling (using interpolation and
decimation).
3. Use Matlab to plot all resulting signals in the time and frequency domain.
270
Sampling Rate Conversion
4. Asynchronous Conversion
1. What is the basic concept of asynchronous sampling rate conversion?
2. Sketch the block diagram and discuss the individual operations.
3. What is the necessary oversampling factor L for a 20-bit resolution?
4. How can we simplify the oversampling operations?
5. How can we make use of polyphase ﬁltering?
6. Why are halfband ﬁlters an efﬁcient choice for the upsampling operation?
7. Which parameters determine the interpolation algorithms in the last stage of the
conversion?
References
[Ada92]
[Ada93]
R. Adams, T. Kwan: VLSI Architectures for Asynchronous Sample-Rate
Conversion, Proc. 93rd AES Convention, Preprint No. 3355, San Francisco,
October 1992.
R. Adams, T. Kwan: Theory and VLSI Implementations for Asynchronous
Sample-Rate Conversion, Proc. 94th AES Convention, Preprint No. 3570,
Berlin, March 1993.
[Bar90]
S. Barnett: Matrices – Methods and Applications, Oxford University Press,
Oxford, 1990.
[Boe93] W. Boehm, H. Prautzsch: Numerical Methods, Vieweg, Wiesbaden, 1993.
[Chu92]
C. K. Chui, Ed.: Wavelets: A Tutorial in Theory and Applications, Volume 2,
Academic Press, Boston, 1992.
[Cro83]
[Cuc91]
[Fli00]
[Hsi87]
[Kat85]
R. E. Crochiere, L. R. Rabiner: Multirate Digital Signal Processing, Prentice
Hall, Englewood Cliffs, NJ, 1983.
S. Cucchi, F. Desinan, G. Parladori, G. Sicuranza: DSP Implementation
of Arbitrary Sampling Frequency Conversion for High Quality Sound
Application, Proc. IEEE ICASSP-91, pp. 3609–3612, Toronto, May 1991.
N. Fliege: Multirate Digital Signal Processing, John Wiley & Sons Ltd,
Chichester, 2000.
C.-C. Hsiao: Polyphase Filter Matrix for Rational Sampling Rate Conversions,
Proc. IEEE ICASSP-87, pp. 2173–2176, Dallas, April 1987.
Y. Katsumata, O. Hamada: A Digital Audio Sampling Frequency Converter
Employing New Digital Signal Processors, Proc. 79th AES Convention,
Preprint No. 2272, New York, October 1985.
References
[Kat86]
[Lag81]
Y. Katsumata, O. Hamada: An Audio Sampling Frequency Conversion Using
Digital Signal Processors, Proc. IEEE ICASSP-86, pp. 33–36, Tokyo, 1986.
R. Lagadec, H. O. Kunz: A Universal, Digital Sampling Frequency Converter
for Digital Audio, Proc. IEEE ICASSP-81, pp. 595–598, Atlanta, April 1981.
271
[Lag82a] R. Lagadec, D. Pelloni, D. Weiss: A Two-Channel Professional Digital
Audio Sampling Frequency Converter, Proc. 71st AES Convention, Preprint
No. 1882, Montreux, March 1982.
[Lag82b] D. Lagadec, D. Pelloni, D. Weiss: A 2-Channel, 16-Bit Digital Sampling
Frequency Converter for Professional Digital Audio, Proc. IEEE ICASSP-82,
pp. 93–96, Paris, May 1982.
[Lag82c] R. Lagadec: Digital Sampling Frequency Conversion, Digital Audio, Collected
Papers from the AES Premier Conference, pp. 90–96, June 1982.
[Lag83]
[Liu92]
R. Lagadec, D. Pelloni, A. Koch: Single-Stage Sampling Frequency
Conversion, Proc. 74th AES Convention, Preprint No. 2039, New York,
October 1983.
G.-S. Liu, C.-H. Wei: A New Variable Fractional Delay Filter with Nonlinear
Interpolation, IEEE Trans. Circuits and Systems-II: Analog and Digital Signal
Processing, Vol. 39, No. 2, pp. 123–126, February 1992.
[Opp99]
A. V. Oppenheim, R. W. Schafer, J. R. Buck: Discrete Time Signal Processing,
2nd edn, Prentice Hall, Upper Saddle River, NJ, 1999.
[Par90]
S. Park, R. Robles: A Real-Time Method for Sample-Rate Conversion from CD
to DAT, Proc. IEEE Int. Conf. Consumer Electronics, pp. 360–361, Chicago,
June 1990.
[Par91a]
S. Park: Low Cost Sample Rate Converters, Proc. NAB Broadcast Engineering
Conference, Las Vegas, April 1991.
[Par91b]
S. Park, R. Robles: A Novel Structure for Real-Time Digital Sample-Rate
Converters with Finite Precision Error Analysis, Proc. IEEE ICASSP-91, pp.
3613–3616, Toronto, May 1991.
[Ram82]
T. A. Ramstad: Sample-Rate Conversion by Arbitrary Ratios, Proc. IEEE
ICASSP-82, pp. 101–104, Paris, May 1982.
[Ram84]
[Smi84]
[Sti91]
T. A. Ramstad: Digital Methods for Conversion between Arbitrary Sampling
Frequencies, IEEE Trans. on Acoustics, Speech and Signal Processing,
Vol. ASSP-32, No. 3, pp. 577–591, June 1984.
J. O. Smith, P. Gossett: A Flexible Sampling-Rate Conversion Method, Proc.
IEEE ICASSP-84, pp. 19.4.1–19.4.4, 1984.
E. F. Stikvoort: Digital Sampling Rate Converter with Interpolation in
Continuous Time, Proc. 90th AES Convention, Preprint No. 3018, Paris,
February 1991.
272
[Vai93]
[Zöl94]
Sampling Rate Conversion
P. P. Vaidyanathan: Multirate Systems and Filter Banks, Prentice Hall,
Englewood Cliffs, NJ, 1993.
U. Zölzer, T. Boltze: Interpolation Algorithms: Theory and Application, Proc.
97th AES Convention, Preprint No. 3898, San Francisco, November 1994.
Chapter 9
Audio Coding
For transmission and storage of audio signals, different methods for compressing data have
been investigated besides the pulse code modulation representation. The requirements of
different applications have led to a variety of audio coding methods which have become
international standards. In this chapter basic principles of audio coding are introduced and
the most important audio coding standards discussed. Audio coding can be divided into
two types: lossless and lossy. Lossless audio coding is based on a statistical model of the
signal amplitudes and coding of the audio signal (audio coder). The reconstruction of the
audio signal at the receiver allows a lossless resynthesis of the signal amplitudes of the
original audio signal (audio decoder). On the other hand, lossy audio coding makes use of a
psychoacoustic model of human acoustic perception to quantize and code the audio signal.
In this case only the acoustically relevant parts of the signal are coded and reconstructed
at the receiver. The samples of the original audio signal are not exactly reconstructed. The
objective of both audio coding methods is a data rate reduction or data compression for
transmission or storage compared to the original PCM signal.
9.1 Lossless Audio Coding
Lossless audio coding is based on linear prediction followed by entropy coding [Jay84] as
shown in Fig. 9.1:
• Linear Prediction. A quantized set of coefﬁcients P for a block of M samples is
determined which leads to an estimate ˆx(n) of the input sequence x(n). The aim is to
minimize the power of the difference signal d(n) without any additional quantization
errors, i.e. the word-length of the signal ˆx(n) must be equal to the word-length of
the input. An alternative approach [Han98, Han01] quantizes the prediction signal
ˆx(n) such that the word-length of the difference signals d(n) remains the same as the
input signal word-length. Figure 9.2 shows a signal block x(n) and the corresponding
spectrum |X(f )|. Filtering the input signal with the predictor ﬁlter transfer function
P (z) delivers the estimate ˆx(n). Subtracting input and prediction signal yields the
prediction error d(n), which is also shown in Fig. 9.2 and which has a considerably
lower power compared to the input power. The spectrum of this prediction error is
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
274
Audio Coding
nearly white (see Fig. 9.2, lower right). The prediction can be represented as a ﬁlter
operation with an analysis transfer function HA(z) = 1 − P (z) on the coder side.
x(n)
Linear
Prediction
Coder
P(M)
x^(n)
d(n)
Quantization
and
Coding
Multiplex
and
Frame
Packing
x c(n)
Statistical
Model
Decoder
x c(n)
DMUX
Decoding
x(n)
P(M)
Predictor
Figure 9.1 Lossless audio coding based on linear prediction and entropy coding.
• Entropy Coding. Quantization of signal d(n) due to the probability density func-
tion of the block. Samples d(n) of greater probability are coded with shorter data
words, whereas samples d(n) of lesser probability are coded with longer data words
[Huf52].
• Frame Packing. The frame packing uses the quantized and coded difference signal
and the coding of the M coefﬁcients of the predictor ﬁlter P (z) of order M.
• Decoder. On the decoder side the inverse synthesis transfer function HS(z) =
−1
A (z) = [1 − P (z)]−1 reconstructs the input signal with the coded difference sam-
H
ples and the M ﬁlter coefﬁcients. The frequency response of this synthesis ﬁlter rep-
resents the spectral envelope shown in the upper right part of Fig. 9.2. The synthesis
ﬁlter shapes the white spectrum of the difference (prediction error) signal with the
spectral envelope of the input spectrum.
The attainable compression rates depend on the statistics of the audio signal and allow a
compression rate of up to 2 [Bra92, Cel93, Rob94, Cra96, Cra97, Pur97, Han98, Han01,
Lie02, Raa02, Sch02]. Figure 9.3 illustrates examples of the necessary word-length for
lossless audio coding [Blo95, Sqa88]. Besides the local entropy of the signal (entropy com-
puted over a block length of 256), results for linear prediction followed by Huffman coding
[Huf52] are presented. Huffman coding is carried out with a ﬁxed code table [Pen93] and
a power-controlled choice of adapted code tables. It is observed from Fig. 9.3 that for high
signal powers, a reduction in word-length is possible if the choice is made from several
adapted code tables. Lossless compression methods are used for storage media with limited
word-length (16 bits in CD and DAT) which are used for recording audio signals of higher
word-lengths (more than 16 bits). Further applications are in the transmission and archiving
of audio signals.
9.2 Lossy Audio Coding
275
Input signal x(n)
|X(f)| in dB
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
n (cid:111)
Prediction error d(n)
200
400
600
800
1000
n (cid:111)
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:358)80
(cid:358)100
0
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:358)80
(cid:358)100
0
Spectral Envelope
2
4
f/kHz (cid:111)
6
8
|D(f)| in dB
2
4
f/kHz (cid:111)
6
8
Figure 9.2 Signals and spectra for linear prediction.
9.2 Lossy Audio Coding
Signiﬁcantly higher compression rates (of factor 4 to 8) can be obtained with lossy coding
methods. Psychoacoustic phenomena of human hearing are used for signal compression.
The ﬁelds of application have a wide range, from professional audio like source coding for
DAB to audio transmission via ISDN and home entertainment like DCC and MiniDisc.
An outline of the coding methods [Bra94] is standardized in an international speciﬁca-
tion ISO/IEC 11172-3 [ISO92], which is based on the following processing (see Fig. 9.4):
• subband decomposition with ﬁlter banks of short latency time;
• calculation of psychoacoustic model parameters based on short-time FFT;
• dynamic bit allocation due to psychoacoustic model parameters (signal-to-mask ratio
SMR);
• quantization and coding of subband signals;
• multiplex and frame packing.
276
Audio Coding
SQAM 67 (Mozart), fixed code table
(cid:111)
t
i
B
(cid:111)
t
i
B
(cid:111)
t
i
B
(cid:111)
t
i
B
16
14
12
10
8
6
4
2
0
0
16
14
12
10
8
6
4
2
0
0
16
14
12
10
8
6
4
2
0
0
16
14
12
10
8
6
4
2
0
0
2
4
6
t/s (cid:111)
8
10
12
SQAM 67 (Mozart), adapted code table
2
4
6
t/s (cid:111)
8
10
12
SQAM 66 (Stravinsky), fixed code table
1
2
3
t/s (cid:111)
4
5
6
SQAM 66 (Stravinsky), adapted code table
1
2
3
t/s (cid:111)
4
5
6
Figure 9.3 Lossless audio coding (Mozart, Stravinsky): word-length in bits versus time
(entropy - - , linear prediction with Huffman coding — ).
9.3 Psychoacoustics
277
x(n)
Analysis
Filter
Bank
x1
xN
ST-
FFT
Coder
Quantization
and
Coding
SMR
Psycho-
acoustic
Model
Multiplex
and
Frame
Packing
x c
Decoder
x c
De-
Multiplexer
and
Decoding
x1
xN
Synthesis
Filter
Bank
y(n)
Figure 9.4 Lossy audio coding based on subband coding and psychoacoustic models.
Owing to lossy audio coding, post-processing of such signals or several coding and
decoding steps is associated with some additional problems. The high compression rates
justify the use of lossy audio coding techniques in applications like transmission.
9.3 Psychoacoustics
In this section, basic principles of psychoacoustics are presented. The results of psychoa-
coustic investigations by Zwicker [Zwi82, Zwi90] form the basis for audio coding based on
models of human perception. These coded audio signals have a signiﬁcantly reduced data
rate compared to the linearly quantized PCM representation. The human auditory system
analyzes broad-band signals in so-called critical bands. The aim of psychoacoustic coding
of audio signals is to decompose the broad-band audio signal into subbands which are
matched to the critical bands and then perform quantization and coding of these subband
signals [Joh88a, Joh88b, Thei88]. Since the perception of sound below the absolute thresh-
old of hearing is not possible, subband signals below this threshold need neither be coded
nor transmitted. In addition to the perception in critical bands and the absolute threshold,
the effects of signal masking in human perception play an important role in signal coding.
These are explained in the following and their application to psychoacoustic coding is
discussed.
9.3.1 Critical Bands and Absolute Threshold
Critical Bands. Critical bands as investigated by Zwicker are listed in Table 9.1.
278
Audio Coding
Table 9.1 Critical bands as given by Zwicker [Zwi82].
z/Bark
fl/Hz
fu/Hz
fB /Hz
0
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
0
100
200
300
400
510
630
770
920
1080
1270
1480
1720
2000
2320
2700
3150
3700
4400
5300
6400
7700
9500
12000
15500
100
200
300
400
510
630
770
920
1080
1270
1480
1720
2000
2320
2700
3150
3700
4400
5300
6400
7700
9500
1200
15500
100
100
100
100
110
120
140
150
160
190
210
240
280
320
380
450
550
700
900
1100
1300
1800
2500
3500
fc/Hz
50
150
250
350
450
570
700
840
1000
1170
1370
1600
1850
2150
2500
2900
3400
4000
4800
5800
7000
8500
10500
13500
A transformation of the linear frequency scale into a hearing-adapted scale is given by
Zwicker [Zwi90] (units of z in Bark):
(cid:1)
z
Bark
= 13 arctan
0.76
(cid:2)
f
kHz
(cid:1)
+ 3.5 arctan
(cid:2)
2
.
f
7.5 kHz
The individual critical bands have bandwidths
(cid:1)
(cid:8)fB = 25 + 75
1 + 1.4
(cid:1)
f
kHz
(cid:2)
2
(cid:2)
0.69
.
(9.1)
(9.2)
Absolute Threshold. The absolute threshold LTq (threshold in quiet) denotes the curve
of sound pressure level L [Zwi82] versus frequency, which leads to the perception of a
sinusoidal tone. The absolute threshold is given by [Ter79]:
(cid:2)
2
(cid:2)−0.8
(cid:2)
4
(cid:2)
(cid:1)
(cid:1)
(cid:1)
(cid:1)
− 6.5 exp
−0.6
− 3.3
+ 10
−3
.
(9.3)
LTq
dB
= 3.64
f
kHz
f
kHz
f
kHz
Below the absolute threshold, no perception of signals is possible. Figure 9.5 shows the ab-
solute threshold versus frequency. Band-splitting in critical bands and the absolute thresh-
old allow the calculation of an offset between the signal level and the absolute threshold for
9.3 Psychoacoustics
279
every critical band. This offset is responsible for choosing appropriate quantization steps
for each critical band.
120
100
80
60
40
20
0
(cid:111)
B
d
/
L
q
T
20
50
100
200
500
1k
2k
5k
10k
20k
f/Hz (cid:111)
Figure 9.5 Absolute threshold (threshold in quiet).
9.3.2 Masking
For audio coding the use of sound perception in critical bands and absolute threshold only is
not sufﬁcient for high compression rates. The basis for further data reduction are the mask-
ing effects investigated by Zwicker [Zwi82, Zwi90]. For band-limited noise or a sinusoidal
signal, frequency-dependent masking thresholds can be given. These thresholds perform
masking of frequency components if these components are below a masking threshold (see
Fig. 9.6). The application of masking for perceptual coding is described in the following.
L/dB
100
50
0
Masking Threshold
20
100
1k
f/Hz
10k
20k
Figure 9.6 Masking threshold of band-limited noise.
Calculation of Signal Power in Band i. First, the sound pressure level within a critical
band is calculated. The short-time spectrum X(k) = DFT[x(n)] is used to calculate the
power density spectrum
Sp(ej (cid:4)) = Sp(ej (2πk/N)) = X2
Sp(k) = X2
R(ej (2πk/N)) + X2
R(k) + X2
I (k),
I (ej (2πk/N)),
0 ≤ k ≤ N − 1,
(9.4)
(9.5)
280
Audio Coding
with the help of an N-point FFT. The signal power in band i is calculated by the sum
Sp(i) =
(cid:4)ui(cid:6)
(cid:4)=(cid:4)li
Sp(k)
(9.6)
from the lower frequency up to the upper frequency of critical band i. The sound pressure
level in band i is given by LS(i) = 10 log10 Sp(i).
Absolute Threshold. The absolute threshold is set such that a 4 kHz signal with peak am-
plitude ±1 LSB for a 16-bit representation lies at the lower limit of the absolute threshold
curve. Every masking threshold calculated in individual critical bands, which lies below
the absolute threshold, is set to a value equal to the absolute threshold in the corresponding
band. Since the absolute threshold within a critical band varies for low and high frequen-
cies, it is necessary to make use of the mean absolute threshold within a band.
Masking Threshold. The offset between signal level and the masking threshold in critical
band i (see Fig. 9.7) is given by [Hel72]
O(i)
dB
= α(14.5 + i) + (1 − α)av,
(9.7)
where α denotes the tonality index and av is the masking index. The masking index [Kap92]
is given by
(cid:1)
av = −2 − 2.05 arctan
(cid:2)
f
4 kHz
(cid:1)
− 0.75 arctan
(cid:2)
.
f 2
2.56 kHz2
As an approximation,
O(i)
dB
= α(14.5 + i) + (1 − α)5.5
(9.8)
(9.9)
can be used [Joh88a, Joh88b]. If a tone is masking a noise-like signal (α = 1), the threshold
is set 14.5 + i dB below the value of LS(i). If a noise-like signal is masking a tone (α = 0),
the threshold is set 5.5 + i dB below LS(i). In order to recognize a tonal or noise-like
signal within a certain number of samples, the spectral ﬂatness measure SFM is estimated.
The SFM is deﬁned by the ratio of the geometric to arithmetic mean value of Sp(i) as



SFM = 10 log10
(cid:26),
N/2
k=1 Sp(ej (2πk/N))
N/2
1
k=1 Sp(ej (2πk/N))
N/2
(cid:11)
(cid:27)
1/(N/2)


 .
i
(9.10)
The SFM is compared with the SFM of a sinusoidal signal (deﬁnition SFMmax = −60 dB)
and the tonality index is calculated [Joh88a, Joh88b] by
(cid:2)
(cid:1)
α = min
SFM
SFMmax
, 1
.
(9.11)
SFM = 0 dB corresponds to a noise-like signal and leads to α = 0, whereas an SFM=75 dB
gives a tone-like signal (α = 1). With the sound pressure level LS(i) and the offset O(i)
9.3 Psychoacoustics
the masking threshold is given by
T (i) = 10
[LS(i)−O(i)]/10.
281
(9.12)
Masking across Critical Bands. Masking across critical bands can be carried out with the
help of the Bark scale. The masking threshold is of a triangular form which decreases at
S1 dB per Bark for the lower slope and at S2 dB per Bark for the upper slope, depending on
the sound pressure level Li and the center frequency fci in band i (see [Ter79]) according to
S1 = 27 dB/Bark,
(cid:1)
S2 = 24 + 0.23
(cid:2)−1
− 0.2
LS(i)
dB
dB/Bark.
(9.13)
(9.14)
fci
kHz
An approximation of the minimum masking within a critical band can be made using
Fig. 9.8 [Thei88, Sauv90]. Masking at the upper frequency fui in the critical band i is
responsible for masking the quantization noise with approximately 32 dB using the lower
masking threshold that decreases by 27 dB/Bark. The upper slope has a steepness which
depends on the sound pressure level. This steepness is lower than the steepness of the lower
slope. Masking across critical bands is presented in Fig. 9.9. The masking signal in critical
band i − 1 is responsible for masking the quantization noise in critical band i as well as the
masking signal in critical band i. This kind of masking across critical bands further reduces
the number of quantization steps within critical bands.
L/dB
LS
O(i)
Band i
fli
fui
f
Figure 9.7 Offset between signal level and masking threshold.
An analytical expression for masking across critical bands [Schr79] is given by
10 log10
[B((cid:8)i)] = 15.81 + 7.5((cid:8)i + 0.474) − 17.5[1 + ((cid:8)i + 0.474)2] 1
2 .
(9.15)
(cid:8)i denotes the distance between two critical bands in Bark. Expression (9.15) is called the
spreading function. With the help of this spreading function, masking of critical band i by
critical band j can be calculated [Joh88a, Joh88b] with abs(i − j ) ≤ 25 such that
Sm(i) =
24(cid:6)
j =0
B(i − j ) · Sp(i).
(9.16)
282
Audio Coding
L/dB
LS
LS -32 dB
5.5 dB
+27 dB
Quantization Noise
fli
i+1
fu
=
fl
i
Band i
f
Figure 9.8 Masking within a critical band.
L/dB
LS
L/dB
LS
LS -32 dB
LS -32 dB
Masked
Quantization Noise
fli-1
fli
fli+1
Band i
f
Masked
Quantization Noise
fli-1
fli
fli+1
Band i
f
Figure 9.9 Masking across critical bands.
The masking across critical bands can therefore be expressed as a matrix operation given by










=





Sm(0)
Sm(1)
...
Sm(24)
B(0) B(−1) B(−2)
B(−1)
B(0)
B(1)
...
...
...
B(24) B(23) B(22)
· · · B(−24)
· · · B(−23)
...
B(0)
· · ·














 .
Sp(0)
Sp(1)
...
Sp(24)
(9.17)
A renewed calculation of the masking threshold with (9.16) leads to the global masking
threshold
Tm(i) = 10log10 Sm(i)−O(i)/10.
For a clariﬁcation of the single steps for a psychoacoustic based audio coding we summa-
rize the operations with exempliﬁed analysis results:
(9.18)
• calculation of the signal power Sp(i) in critical bands → LS(i) in dB (Fig. 9.10a);
• calculation of masking across critical bands Tm(i) → LTm (i) in dB (Fig. 9.10b);
• masking with tonality index → LTm (i) in dB (Fig. 9.10c);
• calculation of global masking threshold with respect to threshold in quiet LTq
→
LTm,abs (i) in dB (Fig. 9.10d).
9.3 Psychoacoustics
283
a) Signal power in critical bands
→
B
d
/
)
i
(
S
L
80
60
40
20
0
FFT
L
(i)
s
-20
-40
20
50
100
200
500
1k
2k
5k
10k
20k
f/Hz →
b) Masking across critical bands
→
B
d
/
)
i
(
m
L
→
B
d
/
)
i
(
m
T
L
→
B
d
/
)
i
(
s
b
a
m
T
,
L
80
60
40
20
0
-20
-40
20
80
60
40
20
0
-20
-40
20
80
60
40
20
0
-20
-40
20
FFT
(i)
L
m
50
100
200
500
1k
2k
5k
10k
20k
f/Hz →
c) Masking with tonality index, SFM = − 47.20 dB,
α = 0.79
FFT
L
L
Tm
(i)
(i)
Tq
50
100
200
500
1k
2k
5k
10k
20k
f/Hz →
d) Masking with absolute threshold
FFT
L
L
Tq
(i)
(i)
Tm,abs
50
100
200
500
1k
2k
5k
10k
20k
f/Hz →
Figure 9.10 Stepwise calculation of psychoacoustic model.
284
Audio Coding
With the help of the global masking threshold LTm,abs (i) we calculate the signal-to-mask
ratio
SMR(i) = LS(i) − LTm,abs (i)
in dB
(9.19)
per Bark band. This signal-to-mask ratio deﬁnes the necessary number of bits per critical
band, such that masking of quantization noise is achieved. For the given example the signal
power and the global masking threshold are shown in Fig. 9.11a. The resulting signal-to-
mask ratio SMR(i) is shown in Fig. 9.11b. As soon as SMR(i) > 0, one has to allocate
bits to the critical band i. For SMR(i) < 0 the corresponding critical band will not be
transmitted. Figure 9.12 shows the masking thresholds in critical bands for a sinusoid of
440 Hz. Compared to the ﬁrst example, the inﬂuence of masking thresholds across critical
bands is easier to observe and interpret.
a) Signal power and masking threshold in critical bands
FFT
Signal power in critical bands
Masking threshold in critical bands
50
100
200
500
1k
2k
5k
10k
20k
f/Hz (cid:111)
b) Signal(cid:358)to(cid:358)mask ratio SMR in critical bands
(cid:111)
B
d
/
L
80
60
40
20
0
(cid:358)20
(cid:358)40
20
40
20
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:111)
B
d
R
M
S
/
20
50
100
200
500
1k
2k
5k
10k
20k
f/Hz (cid:111)
Figure 9.11 Calculation of the signal-to-mask ratio SMR.
9.4 ISO-MPEG-1 Audio Coding
In this section, the coding method for digital audio signals is described which is speciﬁed in
the standard ISO/IEC 11172-3 [ISO92]. The ﬁlter banks used for subband decomposition,
the psychoacoustic models, dynamic bit allocation and coding are discussed. A simpliﬁed
block diagram of the coder for implementing layers I and II of the standard is shown in
Fig. 9.13. The corresponding decoder is shown in Fig. 9.14. It uses the information from
the ISO-MPEG1 frame and feeds the decoded subband signals to a synthesis ﬁlter bank for
reconstructing the broad-band PCM signal. The complexity of the decoder is, in contrast
9.4 ISO-MPEG-1 Audio Coding
285
a) Signal power in critical bands
b) Masking across critical bands
(cid:111)
B
d
/
)
i
(
L
s
(cid:111)
B
d
/
)
i
(
m
T
L
100
80
60
40
20
0
(cid:358)20
(cid:358)40
(cid:358)60
20
100
80
60
40
20
0
(cid:358)20
(cid:358)40
(cid:358)60
20
(cid:111)
B
d
/
)
i
(
m
L
100
80
60
40
20
0
(cid:358)20
(cid:358)40
(cid:358)60
20
50
100 200
500
1k
2k
5k
10k 20k
f/Hz (cid:111)
50
100 200
500
1k
2k
5k
10k 20k
f/Hz (cid:111)
c) Masking with tonality index, SFM = (cid:358)90.82 dB,
d) Masking with absolute threshold
(cid:111)
B
d
/
)
i
(
s
b
a
,
m
T
L
100
80
60
40
20
0
(cid:358)20
(cid:358)40
(cid:358)60
20
50
100 200
500
1k
2k
5k
10k 20k
f/Hz (cid:111)
50
100 200
500
1k
2k
5k
10k 20k
f/Hz (cid:111)
Figure 9.12 Calculation of psychoacoustic model for a pure sinusoid with 440 Hz.
768 kbit/s
x(n)
Analysis
Filter
Bank
x1
x32
Quantization
and
Coding
Scalefactor
Calculation
ST-
FFT
Psycho-
acoustic
Model
SMR
Dynamic
Bit Allocation
192...32 kbit/s
x c
G
E
P
M
O
S
-
I
e
m
a
r
F
Figure 9.13 Simpliﬁed block diagram of an ISO-MPEG1 coder.
to the coder, signiﬁcantly lower. Prospective improvements of the coding method are being
made entirely for the coder.
9.4.1 Filter Banks
The subband decomposition is done with a pseudo-QMF ﬁlter bank (see Fig. 9.15). The
theoretical background is found in the related literature [Rot83, Mas85, Vai93]. The broad-
286
Audio Coding
x c
De-
multiplexer
and
Decoding
x1
xN
Synthesis
Filter
Bank
x(n)
Figure 9.14 Simpliﬁed block diagram of an ISO-MPEG1 decoder.
band signal is decomposed into M uniformly spaced subbands. The subbands are pro-
cessed further after a sampling rate reduction by a factor of M. The implementation of an
ISO-MPEG1 coder is based on M = 32 frequency bands. The individual band-pass ﬁlters
H0(z) · · ·H M−1(z) are designed using a prototype low-pass ﬁlter H (z) and frequency-
shifted versions. The frequency shifting of the prototype with cutoff frequency π/2M is
done by modulating the impulse response h(n) with a cosine term [Bos02] according to
(cid:1)
hk(n) = h(n) · cos
π
32
(k + 0,5)(n − 16)
(cid:1)
(cid:2)
,
(cid:2)
fk(n) = 32 · h(n) · cos
(k + 0,5)(n + 16)
,
π
32
(9.20)
(9.21)
with k = 0, . . . , 31 and n = 0, . . . , 511. The band-pass ﬁlters have bandwidth π/M. For
the synthesis ﬁlter bank, corresponding ﬁlters F0(z) · · · FM−1(z) give outputs which are
added together, resulting in a broad-band PCM signal. The prototype impulse response with
512 taps, the modulated band-pass impulse responses, and the corresponding magnitude
responses are shown in Fig. 9.16. The magnitude responses of all 32 band-pass ﬁlters are
also shown. The overlap of neighboring band-pass ﬁlters is limited to the lower and upper
ﬁlter band. This overlap reaches up to the center frequency of the neighboring bands. The
resulting aliasing after downsampling in each subband will be canceled in the synthesis
ﬁlter bank. The pseudo-QMF ﬁlter bank can be implemented by the combination of a
polyphase ﬁlter structure followed by a discrete cosine transform [Rot83, Vai93, Kon94].
To increase the frequency resolution, layer III of the standard decomposes each of the
32 subbands further into a maximum of 18 uniformly spaced subbands (see Fig. 9.17). The
decomposition is carried out with the help of an overlapped transform of windowed sub-
band samples. The method is based on a modiﬁed discrete cosine transform, also known as
the TDAC ﬁlter bank (Time Domain Aliasing Cancellation) and MLT (Modulated Lapped
Transform). An exact description is given in [Pri87, Mal92]. This extended ﬁlter bank
is referred to as the polyphase/modiﬁed discrete cosine transform (MDCT) hybrid ﬁlter
bank [Bra94]. The higher frequency resolution enables a higher coding gain but has the
disadvantage of a worse time resolution. This is observed for impulse-like signals. In order
to minimize these artifacts, the number of subbands per subband can be altered from 18
down to 6. Subband decompositions that are matched to the signal can be obtained by
specially designed window functions with overlapping transforms [Edl89, Edl95]. The
equivalence of overlapped transforms and ﬁlter banks is found in [Mal92, Glu93, Vai93,
Edl95, Vet95].
9.4 ISO-MPEG-1 Audio Coding
287
a)
x(n)
b)
x(n)
c)
d)
Analysis
Filter
Bank
x0
x M-1
Synthesis
Filter
Bank
y(n)
H0(z)
H1(z)
x0
x1
M
M
M
M
F0(z)
F1(z)
HM-1(z)
M
x M-1
M
FM-1 (z)
y(n)
|H(ej(cid:58))|
-(cid:83)/2M
(cid:83)/2M
(cid:58)
H0
H1
HM-1
(cid:83)/M
2(cid:83)/M
(cid:58)
(cid:83)
Figure 9.15 Pseudo-QMF ﬁlter bank.
9.4.2 Psychoacoustic Models
Two psychoacoustic models have been developed for layers I to III of the ISO-MPEG1
standard. Both models can be used independently of each other for all three layers. Psy-
choacoustic model 1 is used for layers I and II, whereas model 2 is used for layer III. Owing
to the numerous applications of layers I and II, we discuss psychoacoustic model 1 in this
subsection.
Bit allocation in each of the 32 subbands is carried out using the signal-to-mask ratio
SMR(i). This is based on the minimum masking threshold and the maximum signal level
within a subband. In order to calculate this ratio, the power density spectrum is estimated
with the help of a short-time FFT in parallel with the analysis ﬁlter bank. As a consequence,
a higher frequency resolution is obtained for estimating the power density spectrum in
contrast to the frequency resolution of the 32-band analysis ﬁlter bank. The signal-to-mask
ratio for every subband is determined as follows:
1. Calculate the power density spectrum of a block of N samples using FFT. After
windowing a block of N = 512 (N = 1024 for layer II) input samples, the power
density spectrum
X(k) = 10 log10
(cid:16)
(cid:16)
(cid:16)
(cid:16)
1
N
N−1(cid:6)
n=0
h(n)x(n)e
−jnk2π/N
2
(cid:16)
(cid:16)
(cid:16)
(cid:16)
in dB
(9.22)
288
Audio Coding
Prototype h(n) and modulated prototype h
(n)
(n) and h
1
0
h(n)
h
(n)
0
(n)
h
1
0.05
0
(cid:358)0.05
0
50
100
150
200
250
n (cid:111)
300
350
400
450
500
Magnitude responses
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:358)80
(cid:358)100
(cid:358)120
0
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:358)80
(cid:358)100
(cid:358)120
0
|H(ej(cid:58))|
(ej(cid:58))|
|H
0
(ej(cid:58))|
|H
1
0.005
0.01
0.015
0.02
0.025
(cid:58)/(cid:83) (cid:111)
0.03
0.035
0.04
0.045
0.05
Magnitude responses of 32 analysis filters
0.05
0.1
0.15
0.2
0.25
(cid:58)/(cid:83) (cid:111)
0.3
0.35
0.4
0.45
0.5
Figure 9.16 Impulse responses and magnitude responses of pseudo-QMF ﬁlter bank.
x(n)
H0(z)
32
H1(z)
32
x0
x1
MDCT
MDCT
H31(z)
32
x 31
MDCT
0
17
18
35
558
575
IMDCT
32
F0(z)
IMDCT
32
F1(z)
IMDCT
32
F31(z)
y(n)
Figure 9.17 Polyphase/MDCT hybrid ﬁlter bank.
9.4 ISO-MPEG-1 Audio Coding
289
is calculated. Then the window h(n) is displaced by 384 (12 · 32) samples and the
next block is processed.
2. Determine the sound pressure level in every subband. The sound pressure level is
derived from the calculated power density spectrum and by calculating a scaling
factor in the corresponding subband as given by
LS(i) = max[X(k), 20 log10
[SCFmax(i) · 32768] − 10]
in dB.
(9.23)
For X(k), the maximum of the spectral lines in a subband is used. The scaling factor
SCF(i) for subband i is calculated from the absolute value of the maximum of 12
consecutive subband samples. A nonlinear quantization to 64 levels is carried out
(layer I). For layer II, the sound pressure level is determined by choosing the largest
of the three scaling factors from 3 · 12 subband samples.
3. Consider the absolute threshold. The absolute threshold LT q (m) is speciﬁed for
different sampling rates in [ISO92]. The frequency index m is based on a reduction
of N/2 relevant frequencies with the FFT of index k (see Fig. 9.18). The subband
index is still i.
Subband index i
Frequency index m
k=0    k=1
FFT index  k
k=255
Figure 9.18 Nomenclature of frequency indices.
4. Calculate tonal Xtm(k) or non-tonal Xnm(k) masking components and determining
relevant masking components (for details see [ISO92]). These masking components
are denoted by Xtm[z(j )] and Xnm[z(j )]. With the index j , tonal and non-tonal
masking components are labeled. The variable z(m) is listed for reduced frequency
indices m in [ISO92]. It allows a ﬁner resolution of the 24 critical bands with the
frequency group index z.
5. Calculate the individual masking thresholds. For masking thresholds of tonal and
non-tonal masking components Xtm[z(j )] and Xnm[z(j )], the following calculation
is performed:
LT tm[z(j ), z(m)] = Xtm[z(j )] + avtm
LT nm[z(j ), z(m)] = Xnm[z(j )] + avnm
[z(j )] + vf [z(j ), z(m)] dB,
[z(j )] + vf [z(j ), z(m)] dB.
(9.24)
(9.25)
The masking index for tonal masking components is given by
avtm
= −1.525 − 0.275 · z(j ) − 4.5 in dB,
and the masking index for non-tonal masking components is
avnm
= −1.525 − 0.175 · z(j ) − 0.5 in dB.
(9.26)
(9.27)
290
Audio Coding
The masking function vf [z(j ), z(m)] with distance (cid:8)z = z(m) − z(j ) is given by

17 · ((cid:8)z + 1) − (0.4 · X[z(j )] + 6)

(0.4 · X[z(j )] + 6) · (cid:8)z
−17 · (cid:8)z

−((cid:8)z − 1) · (17 − 0.15 · X[z(j )]) − 17
vf =
in dB
−3 ≤ (cid:8)z < −1
−1 ≤ (cid:8)z < 0
0 ≤ (cid:8)z < 1
1 ≤ (cid:8)z < 8
in Bark.
This masking function vf [z(j ), z(m)] describes the masking of the frequency index
z(m) by the masking component z(j ).
6. Calculate the global masking threshold. For frequency index m, the global masking
threshold is calculated as the sum of all contributing masking components accord-
ing to
LT g(m) = 10 log10
(cid:4)
10LTq (m)/10 +
Tm(cid:6)
j =1
10LTtm[z(j ),z(m)]/10
(cid:5)
+
Rm(cid:6)
j =1
10LTnm[z(j ),z(m)]/10
dB.
(9.28)
The total number of tonal and non-tonal masking components are denoted as Tm and
Rm respectively. For a given subband i, only masking components that lie in the
range −8 to+ 3 Bark will be considered. Masking components outside this range are
neglected.
7. Determine the minimum masking threshold in every subband:
LT min(i) = min[LTg(m)] dB.
(9.29)
Several masking thresholds LT g(m) can occur in a subband as long as m lies within
the subband i.
8. Calculate the signal-to-mask ratio SMR(i) in every subband:
SMR(i) = LS(i) − LT min(i) dB.
(9.30)
The signal-to-mask ratio determines the dynamic range that has to be quantized in the
particular subband so that the level of quantization noise lies below the masking threshold.
The signal-to-mask ratio is the basis for the bit allocation procedure for quantizing the
subband signals.
9.4.3 Dynamic Bit Allocation and Coding
Dynamic Bit Allocation. Dynamic bit allocation is used to determine the number of bits
that are necessary for the individual subbands so that a transparent perception is possible.
The minimum number of bits in subband i can be determined from the difference between
scaling factor SCF(i) and the absolute threshold LT q (i) as b(i) = SCF(i) − LT q (i). With
9.5 MPEG-2 Audio Coding
291
this, quantization noise remains under the masking threshold. Masking across critical bands
is used for the implementation of the ISO-MPEG1 coding method.
For a given transmission rate, the maximum possible number of bits Bm for coding
subband signals and scaling factors is calculated as
Bm =
32(cid:6)
i=1
b(i) + SCF(i) + additional information.
(9.31)
The bit allocation is performed within an allocation frame consisting of 12 subband samples
(384 = 12 · 32 PCM samples) for layer I and 36 subband samples (1152 = 36 · 32 PCM
samples) for layer II.
The dynamic bit allocation for the subband signals is carried out as an iterative proce-
dure. At the beginning, the number of bits per subband is set to zero. First, the mask-to-
noise ratio
MNR(i) = SNR(i) − SMR(i)
(9.32)
is determined for every subband. The signal-to-mask ratio SMR(i) is the result of the
psychoacoustic model. The signal-to-noise ratio SNR(i) is deﬁned by a table in [ISO92],
in which for every number of bits a corresponding signal-to-noise ratio is speciﬁed. The
number of bits must be increased as long as the mask-to-noise ratio MNR is less than zero.
The iterative bit allocation is performed by the following steps.
1. Determination of the minimum MNR(i) of all subbands.
2. Increasing the number of bits of these subbands on to the next stage of the MPEG1
standard. Allocation of 6 bits for the scaling factor of the MPEG1 standard when the
number of bits is increased for the ﬁrst time.
3. New calculation of MNR(i) in this subband.
4. Calculation of the number of bits for all subbands and scaling factors and comparison
with the maximum number Bm. If the number of bits is smaller than the maximum
number, the iteration starts again with step 1.
Quantization and Coding of Subband Signals. The quantization of the subband signals
is done with the allocated bits for the corresponding subband. The 12 (36) subband samples
are divided by the corresponding scaling factor and then linearly quantized and coded (for
details see [ISO92]). This is followed by a frame packing. In the decoder, the procedure is
reversed. The decoded subband signals with different word-lengths are reconstructed into a
broad-band PCM signal with a synthesis ﬁlter bank (see Fig. 9.14). MPEG-1 audio coding
has a one- or a two-channel stereo mode with sampling frequencies of 32, 44.1, and 48 kHz
and a bit rate of 128 kbit/s per channel.
9.5 MPEG-2 Audio Coding
The aim of the introduction of MPEG-2 audio coding was the extension of MPEG-1 to
lower sampling frequencies and multichannel coding [Bos97]. Backward compatibility
292
Audio Coding
to existing MPEG-1 systems is achieved through the version MPEG-2 BC (Backward
Compatible) and the introduction toward lower sampling frequencies of 32, 22.05, 24 kHz
with version MPEG-2 LSF (Lower Sampling Frequencies). The bit rate for a ﬁve-channel
MPEG-2 BC coding with full bandwidth of all channels is 640–896 kBit/s.
9.6 MPEG-2 Advanced Audio Coding
To improve the coding of mono, stereo, and multichannel audio signals the MPEG-2 AAC
(Advanced Audio Coding) standard was speciﬁed. This coding standard is not backward
compatible with the MPEG-1 standard and forms the kernel for new extended coding
standards such as MPEG-4. The achievable bit rate for a ﬁve-channel coding is 320 kbit/s.
In the following the main signal processing steps for MPEG-2 AAC are introduced and
the principle functionalities explained. An extensive explanation can be found in [Bos97,
Bra98, Bos02]. The MPEG-2 AAC coder is shown in Fig. 9.19. The corresponding decoder
performs the functional units in reverse order with corresponding decoder functionali-
ties.
Pre-processing. The input signal will be band-limited according to the sampling frequency.
This step is used only in the scalable sampling rate proﬁle [Bos97, Bra98, Bos02].
Filter bank. The time-frequency decomposition into M = 1024 subbands with an over-
lapped MDCT [Pri86, Pri87] is based on blocks of N = 2048 input samples. A stepwise
explanation of the implementation is given. A graphical representation of the single steps
is shown in Fig. 9.20. The single steps are as follows:
1. Partitioning of the input signal x(n) with time index n into overlapped blocks
xm(r) = x(mM + r),
r = 0, . . . , N − 1; −∞ ≤ m ≤ ∞,
(9.33)
of length N with an overlap (hop size) of M = N/2. The time index inside a block
is denoted by r. The variable m denotes the block index.
2. Windowing of blocks with window function w(r) → xm(r) · w(r).
3. The MDCT
X(m, k) =
!
2
M
N−1(cid:6)
r=0
(cid:1)
xm(r)w(r) cos
(cid:1)
k + 1
2
π
M
(cid:2)(cid:1)
r + M + 1
2
(cid:2)(cid:2)
,
k = 0, . . . , M − 1,
(9.34)
yields, for every M input samples, M = N/2 spectral coefﬁcients from N windowed
input samples.
4. Quantization of spectral coefﬁcients X(m, k) leads to quantized spectral coefﬁcients
XQ(m, k) based on a psychoacoustic model.
9.6 MPEG-2 Advanced Audio Coding
293
Psycho-
acoustic
Model
Audio Signal
Pre-
processing
Filter
Bank
TNS
Intensity
Coding
Quantized
Spectrum of
Previous
Block
Prediction
Bitstream
m
a
e
r
t
s
t
i
B
n
o
i
t
a
r
e
n
e
G
m
a
e
r
t
s
t
i
B
l
r
e
x
e
p
i
t
l
u
m
e
D
M/S
Scale
Factors
Quan-
tizer
l
o
r
t
n
o
C
e
t
a
R
t
i
B
Iteration Loop
Decoding
Inverse
Quan-
tizer
Scale
Factors
M/S
Prediction
Intensity
Decoder
TNS
Filter
Bank
Audio Signal
Post-
Processing
Coder
Decoder
Figure 9.19 MPEG-2 AAC coder and decoder.
5. The IMDCT (Inverse Modiﬁed Discrete Cosine Transform)
ˆxm(r) =
!
2
M
M−1(cid:6)
k=0
(cid:1)
XQ(m, k) cos
(cid:1)
k + 1
2
π
M
(cid:2)(cid:1)
r + M + 1
2
(cid:2)(cid:2)
,
r = 0, . . . , N − 1,
(9.35)
yields, for every M input samples, N output samples in block ˆxm(r).
6. Windowing of inverse transformed block ˆxm(r) with window function w(r).
294
Audio Coding
M
2M
n
0
N
M
x(n)
x(n)
x (r)m
x (r)
0
.w
(r)
x (r)
1
.w
(r)
x (r)
2
.w
(r)
k
X(0,k)
X(M,k)
X(2M,k)
M-1
x
e
d
n
i
y
c
n
e
u
q
e
r
F
1
0
0
0
M
Time index
1
Block index
n
2M
m
2
MDCT
Quantization
IMDCT
N samples
Windowing with
analysis window
X(m,k)
X (m,k)
Q
^x (r)m
Windowing with
synthe
sis window
M
OLA
y(n)
Figure 9.20 Time-frequency decomposition with MDCT/IMDCT.
7. Reconstruction of output signal y(n) by overlap-add operation according to
y(n) =
∞(cid:6)
m=−∞
with overlap M.
ˆxm(r)w(r),
r = 0, . . . , N − 1,
(9.36)
In order to explain the procedural steps we consider the MDCT/IMDCT of a sine pulse
shown in Fig. 9.21. The left column shows from the top down the input signal and partitions
of the input signal of block length N = 256. The window function is a sine window. The
corresponding MDCT coefﬁcients of length M = 128 are shown in the middle column.
The IMDCT delivers the signals in the right column. One can observe that the inverse
transforms with the IMDCT do not exactly reconstruct the single input blocks. Moreover,
each output block consists of an input block and a special superposition of a time-reversed
and by M = N/2 circular shifted input block, which is denoted by time-domain aliasing
[Pri86, Pri87, Edl89]. The overlap-add operation of the single output blocks perfectly
recovers the input signal which is shown in the top signal of the right column (Fig. 9.21).
For a perfect reconstruction of the output signal, the window function of the analysis
and synthesis step has to fulﬁll the condition w2(r) + w2(r + M) = 1, r = 0, . . . , M − 1.
9.6 MPEG-2 Advanced Audio Coding
295
Input signal
Output signal
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
0
200
400
600
128 MDCT coefficients
0
200
400
600
Hop size = 128
10
MDCT
(cid:159)
0
1
0
IMDCT
(cid:159)
0
200
400
600
0
200
400
600
0
200
400
600
0
200
400
600
0
200
400
600
n (cid:111)
(cid:358)10
10
0
(cid:358)10
10
0
(cid:358)10
10
0
(cid:358)10
10
0
(cid:358)10
0
20
40
60
80
100
120
0
20
40
60
80
100
120
0
20
40
60
80
100
120
0
20
40
60
80
100
120
0
200
400
600
0
200
400
600
0
200
400
600
0
200
400
600
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
1
0
(cid:358)1
0
20
40
60
k (cid:111)
80
100
120
0
200
400
600
n (cid:111)
Figure 9.21 Signals of MDCT/IMDCT.
2 ) π
The Kaiser–Bessel derived window [Bos02] and a sine window h(n) = sin((n + 1
N ) with
n = 0, . . . , N − 1 [Mal92] are applied. Figure 9.22 shows both window functions with
N = 2048 and the corresponding magnitude responses for a sampling frequency of fS =
44100 Hz. The sine window has a smaller pass-band width but slower falling side lobes.
In contrast, the Kaiser–Bessel derived window shows a wider pass-band and a faster decay
of the side lobes. In order to demonstrate the ﬁlter bank properties and in particular the
frequency decomposition of MDCT, we derive the modulated band-pass impulse responses
of the window functions (prototype impulse response w(n) = h(n)) according to
(cid:1)
hk(n) = 2 · h(n) · cos
(cid:1)
n + M + 1
k + 1
2
2
k = 0, . . . , M − 1; n = 0, . . . , N − 1.
(9.37)
π
M
(cid:2)(cid:1)
(cid:2)(cid:2)
,
Figure 9.23 shows the normalized prototype impulse response of the sine window and
the ﬁrst two modulated band-pass impulse responses h0(n) and h1(n) and accordingly the
corresponding magnitude responses are depicted. Besides the increased frequency resolu-
tion with M = 1024 band-pass ﬁlters, the reduced stop-band attenuation can be observed.
A comparison of this magnitude response of the MDCT with the frequency resolution of
296
Audio Coding
1
0.8
0.6
0.4
0.2
0
0
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:358)80
(cid:358)100
(cid:358)120
Window functions with N=2048
KBD window
Sine window
200
400
600
800
1000
n (cid:111)
1200
1400
1600
1800
2000
Magnitude response
KBD window
Sine window
1
10
2
10
3
10
4
10
f/Hz (cid:111)
Figure 9.22 Kaiser–Bessel derived window and sine window for N = 2048 and magnitude responses
of the normalized window functions.
the PQMF ﬁlter bank with M = 32 in Fig. 9.16 points out the different properties of both
subband decompositions.
For adjusting the time and frequency resolution to the properties of an audio signal
several methods have been investigated. Signal-adaptive audio coding based on the wavelet
transform can be found in [Sin93, Ern00]. Window switching can be applied for achiev-
ing a time-variant time-frequency resolution for MDCT and IMDCT applications. For
stationary signals a high frequency resolution and a low time resolution are necessary.
This leads to long windows with N = 2048. Coding of attacks of instruments needs a
high time resolution (reduction of window length to N = 256) and thus reduces frequency
resolution (reduction of number of spectral coefﬁcients). A detailed description of switch-
ing between time-frequency resolution with the MDCT/IMDCT can be found in [Edl89,
Bos97, Bos02]. Examples of switching between different window functions and windows
of different length are shown in Fig. 9.24.
Temporal Noise Shaping. A further method for adapting the time-frequency resolution
of a ﬁlter bank and here an MDCT/IMDCT to the signal characteristic is based on linear
prediction along the spectral coefﬁcients in the frequency domain [Her96, Her99]. This
method is called temporal noise shaping (TNS) and is a weighting of the temporal envelope
of the time-domain signal. Weighting the temporal envelope in this way is demonstrated in
Fig. 9.25.
9.6 MPEG-2 Advanced Audio Coding
297
x 10(cid:358)3
2
1
0
(cid:358)1
(cid:358)2
0
0
(cid:358)20
(cid:358)40
(cid:358)60
(cid:358)80
(cid:358)100
(cid:358)120
0
Normalized prototype and modulated impulse responses
 h(n)
h(n)/(cid:54)n
(n)
h
0
(n)
h
1
200
400
600
800
1000
n (cid:111)
1200
1400
1600
1800
2000
Magnitude responses
|H(ej(cid:58))|
(ej(cid:58))|
|H
0
(ej(cid:58))|
|H
1
0.001
0.002
0.003
0.004
0.005
(cid:58)/(cid:83) (cid:111)
0.006
0.007
0.008
0.009
0.01
Figure 9.23 Normalized impulse responses of sine window for N = 2048, modulated band-pass
impulse responses, and magnitude responses.
K=1024
N=256, K=128
N=2048
N=2048
N=2048
Figure 9.24 Switching of window functions.
Figure 9.25a shows a signal from a castanet attack. Making use of the discrete cosine
transform (DCT, [Rao90])
XC(2)(k) =
!
2
N
ck
N−1(cid:6)
n=0
(cid:1)
x(n) cos
(cid:2)
(2n + 1)kπ
2N
,
k = 0, . . . , N − 1
(9.38)
298
Audio Coding
a) Signal x(n)
b) X(k)=DCT[x(n)]
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
n (cid:111)
c) y(n)=IDCT[Y(k)]
200
400
600
800
1000
n (cid:111)
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
k (cid:111)
d) Y(k)=[X(k)/X
]
Q
max
(cid:152) X
max
200
400
600
800
1000
k (cid:111)
1
0.5
0
(cid:358)0.5
(cid:358)1
0
e) Difference signal x(n)(cid:358)y(n)
200
400
600
800
1000
n (cid:111)
Figure 9.25 Attack of castanet and spectrum.
and the inverse discrete cosine transform (IDCT)
x(n) =
!
2
N
N−1(cid:6)
k=0
with ck =
(cid:1)
(2n + 1)kπ
2N
(cid:2)
,
n = 0, . . . , N − 1
ckXC(2)(k) cos
(cid:21)
√
(2),
1/
1,
k = 0,
otherwise,
(9.39)
9.6 MPEG-2 Advanced Audio Coding
299
the spectral coefﬁcients of the DCT of this castanet attack are represented in Fig. 9.25b.
After quantization of these spectral coefﬁcients X(k) to 4 bits (Fig. 9.25d) and IDCT of
the quantized spectral coefﬁcients, the time-domain signal in Fig. 9.25c and the difference
signal in Fig. 9.25e between input and output result. One can observe in the output and
difference signal that the error is spread along the entire block length. This means that
before the attack of the castanet happens, the error signal of the block is perceptible.
The time-domain masking, referred to as pre-masking [Zwi90], is not sufﬁcient. Ideally,
the spreading of the error signal should follow the time-domain envelope of the signal
itself. From forward linear prediction in the time domain it is known that the power spec-
tral density of the error signal after coding and decoding is weighted by the envelope
of the power spectral density of the input signal [Var06]. Performing a forward linear
prediction along the frequency axis in the frequency domain and quantization and coding
leads to an error signal in the time domain where the temporal envelope of the error
signal follows the time-domain envelope of the input signal [Her96]. To point out the
temporal weighting of the error signal we consider the forward prediction in the time
domain in Fig. 9.26a. For coding the input signal, x(n) is predicted by an impulse response
p(n). The output of the predictor is subtracted from the input signal x(n) and delivers
the signal d(n), which is then quantized to a reduced word-length. The quantized signal
dQ(n) = x(n) ∗ a(n) + e(n) is the sum of the convolution of x(n) with the impulse re-
sponse a(n) and the additive quantization error e(n). The power spectral density of the
coder output is SDQDQ (ej (cid:4)) = SXX(ej (cid:4)) · |A(ej (cid:4))|2 + SEE(ej (cid:4)). The decoding operation
performs the convolution of dQ(n) with the impulse response h(n) of the inverse system to
the coder. Therefore a(n) ∗ h(n) = δ(n) must hold and thus H (ej (cid:4)) = 1/A(ej (cid:4)). Hereby
the output signal y(n) = x(n) + e(n) ∗ h(n) is derived with the corresponding discrete
Fourier transform Y (k) = X(k) + E(k) · H (k). The power spectral density of the decoder
out signal is given by SYY (ej (cid:4)) = SXX(ej (cid:4)) + SEE(ej (cid:4)) · |H (ej (cid:4))|2. Here one can observe
the spectral weighting of the quantization error with the spectral envelope of the input
signal which is represented by |H (ej (cid:4))|. The same kind of forward prediction will now
be applied in the frequency domain to the spectral coefﬁcients X(k) = DCT[x(n)] for a
block of input samples x(n) shown in Fig. 9.26b. The output of the decoder is then given
by Y (k) = X(k) + E(k) ∗ H (k) with A(k) ∗ H (k) = δ(k). Thus, the corresponding time-
domain signal is y(n) = x(n) + e(n) · h(n), where the temporal weighting of the quantiza-
tion error with the temporal envelope of the input signal is clearly evident. The temporal
envelope is represented by the absolute value |h(n)| of the impulse response h(n). The
relation between the temporal signal envelope (absolute value of the analytical signal)
and the autocorrelation function of the analytical spectrum is discussed in [Her96]. The
dualities between forward linear prediction in time and frequency domain are summarized
in Table 9.2. Figure 9.27 demonstrates the operations for temporal noise shaping in the
coder, where the prediction is performed along the spectral coefﬁcients. The coefﬁcients of
the forward predictor have to be transmitted to the decoder, where the inverse ﬁltering is
performed along the spectral coefﬁcients.
The temporal weighting is ﬁnally demonstrated in Fig. 9.28, where the corresponding
signals with forward prediction in the frequency domain are shown. Figure 9.28a,b shows
the castanet signal x(n) and its corresponding spectral coefﬁcients X(k) of the applied
DCT. The forward prediction delivers D(k) in Fig. 9.28d and the quantized signal DQ(k)
in Fig. 9.28f. After the decoder the signal Y (k) in Fig. 9.28h is reconstructed by the inverse
x(n)
A(k)
a(n)
_
p(n)
Coder
x(n)
X(k)
a(n)
A(k)
_
P(k)
Coder
300
Audio Coding
a) Forward prediction in time domain
Error signal e(n) after decoder is
with spectral envelope H(k)=DFT[h(n)]
spectrally
weighted
H(k)=1/A(k)
Y(k)=X(k)+E(K).H(k)
d(n)
d (n)=x(n)*a(n)+e(n)
Q
h(n) with h(n) * a(n)=δ(n)
y(n)=x(n)+e(n)*h(n)
Q
e(n)
p(n)
Decoder
b) Forward prediction in frequency domain
Error signal e(n) after decoder is temporally weighted
with temporal envelope h(n)=IDFT[H(k)]
h(n)=1/a(n)
y(n)=x(n)+e(n).h(n)
D(k)
D (k)=X(k)*A(k)+E(k)
Q
H(k) with H(k) *A(k)= (k)
 δ
Y(k)=X(k)+E(k)*H(k)
Q
E(k)
P(k)
Decoder
Figure 9.26 Forward prediction in time and frequency domain.
_
Q
P(k)
P(k)
k
M-1
X(n,k)
X(n,k)
Y(n,k)
1
0
x(n)
T
C
D
M
k
M-1
1
0
0
M
2M
0
M
2M
y(n)
T
C
D
M
I
n
Figure 9.27 Temporal noise shaping with forward prediction in frequency domain.
transfer function. The IDCT of Y (k) ﬁnally results in the output signal y(n) in Fig. 9.28e.
The difference signal x(n) − y(n) in Fig. 9.28g demonstrates the temporal weighting of the
error signal with the temporal envelope from Fig. 9.28c. For this example, the order of the

9.6 MPEG-2 Advanced Audio Coding

301
a) Signal x(n)
b) X(k)=DCT[x(n)]
1
0.5
0
(cid:358)0.5
(cid:358)1
0
35
30
25
20
15
10
5
0
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
n (cid:111)
c) Temporal envelope |h(n)|
200
400
600
800
1000
n (cid:111)
e) y(n)=IDCT[Y(k)]
200
400
600
800
1000
n (cid:111)
g) Difference signal x(n)(cid:358)y(n)
200
400
600
800
1000
n (cid:111)
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
k (cid:111)
d) D(k)=X(k)(cid:13) A(k)
 = 16 dB
G
P
200
400
600
800
1000
k (cid:111)
f) D
(k)=[D(k)/D
Q
]
Q
max
(cid:152) D
max
200
400
600
800
1000
k (cid:111)
h) Y(k)=D
Q
(k) (cid:13) H(k)
200
400
600
800
1000
k (cid:111)
Figure 9.28 Temporal noise shaping: attack of castanet and spectrum.
302
Audio Coding
Table 9.2 Forward prediction in time and frequency domain.
Prediction in time domain
y(n) = x(n) + e(n) ∗ h(n)
Y (k) = X(k) + E(k) · H (k)
Prediction in frequency domain
y(n) = x(n) + e(n) · h(n)
Y (k) = X(k) + E(k) ∗ H (k)
predictor is chosen as 20 [Bos97] and the prediction along the spectral coefﬁcients X(k) is
performed by the Burg method. The prediction gain for this signal in the frequency domain
is Gp = 16 dB (see Fig. 9.28d).
Frequency-domain Prediction. A further compression of the band-pass signals is possible
by using linear prediction. A backward prediction [Var06] of the band-pass signals is
applied on the coder side (see Fig. 9.29). In using a backward prediction the predictor
coefﬁcients need not be coded and transmitted to the decoder, since the estimate of the input
sample is based on the quantized signal. The decoder derives the predictor coefﬁcients p(n)
in the same way from the quantized input. A second-order predictor is sufﬁcient, because
the bandwidth of the band-pass signals is very low [Bos97].
Q
_
p(n)
k
M-1
p(n)
k
M-1
x(n)
T
C
D
M
r
e
d
o
C
S
N
T
X(n,k)
X(n,k)
Y(n,k)
r
e
d
o
c
e
D
S
N
T
y(n)
T
C
D
M
I
1
0
0
M
2M
n
1
0
0
M
2M
n
Figure 9.29 Backward prediction of band-pass signals.
Mono/Side Coding. Coding of stereo signals with left and right signals xL(n) and xR(n)
can be achieved by coding a mono signal (M) xM (n) = (xL(n) + xR(n))/2 and a side (S,
difference) signal xS(n) = (xL(n) − xR(n))/2 (M/S coding). Since for highly correlated
left and right signals the power of the side signal is reduced, a reduction in bit rate for this
signal can be achieved. The decoder can reconstruct the left signal xL(n) = xM (n) + xS(n)
and the right signal xR(n) = xM (n) − xS(n), if no quantization and coding is applied to the
mono and side signal. This M/S coding is carried out for MPEG-2 AAC [Bra98, Bos02]
with the spectral coefﬁcients of a stereo signal (see Fig. 9.30).
9.6 MPEG-2 Advanced Audio Coding
303
X (k)L
X (k)M
X (k)M
X (k)L
0.5
_
0.5
_
X (k)R
X (k)S
X (k)S
X (k)R
M/S Coder
M/S Decoder
Figure 9.30 M/S coding in frequency domain.
Intensity Stereo Coding. For intensity stereo (IS) coding a mono signal xM (n) = xL(n) +
xR(n) and two temporal envelopes eL(n) and eR(n) of the left and right signals are coded
and transmitted. On the decoding side the left signal is reconstructed by yL(n) = xM (n) ·
eL(n) and the right signal by yR(n) = xM (n) · eR(n). This reconstruction is lossy. The IS
coding of MPEG-2 AAC [Bra98] is performed by summation of spectral coefﬁcients of
both signals and by coding of scale factors which represent the temporal envelope of both
signals (see Fig. 9.31). This type of stereo coding is only useful for higher frequency bands,
since the human perception for phase shifts is non-sensitive for frequencies above 2 kHz.
E (k)L
Env
E (k)L
X (k)L
Y (k)L
X (k)M
X (k)M
X (k)R
E (k)R
Env
E (k)R
Y (k)R
IS Coder
IS Decoder
Figure 9.31 Intensity stereo coding in frequency domain.
Quantization and Coding. During the last coding step the quantization and coding of the
spectral coefﬁcients takes place. The quantizers, which are used in the ﬁgures for prediction
along spectral coefﬁcients in frequency direction (Fig. 9.27) and prediction in the frequency
domain along band-pass signals (Fig. 9.29), are now combined into a single quantizer per
spectral coefﬁcient. This quantizer performs nonlinear quantization similar to a ﬂoating-
point quantizer of Chapter 2 such that a nearly constant signal-to-noise ratio over a wide
amplitude range is achieved. This ﬂoating-point quantization with a so-called scale factor
is applied to several frequency bands, in which several spectral coefﬁcients use a common
scale factor derived from an iteration loop (see Fig. 9.19). Finally, a Huffman coding of
the quantized spectral coefﬁcients is performed. An extensive presentation can be found in
[Bos97, Bra98, Bos02].
304
Audio Coding
9.7 MPEG-4 Audio Coding
The MPEG-4 audio coding standard consists of a family of audio and speech coding
methods for different bit rates and a variety of multimedia applications [Bos02, Her02].
Besides a higher coding efﬁciency, new functionalities such as scalability, object-oriented
representation of signals and interactive synthesis of signals at the decoder are integrated.
The MPEG-4 coding standard is based on the following speech and audio coders.
• Speech coders
– CELP: Code Excitated Linear Prediction (bit rate 4–24 kbit/s).
– HVXC: Harmonic Vector Excitation Coding (bit rate 1.4–4 kbit/s).
• Audio coders
– Parametric audio: representation of a signal as a sum of sinsoids, harmonic
components, and residual components (bit rate 4–16 kbit/s).
– Structured audio: synthetic signal generation at decoder (extension of the MIDI
standard1) (200 bit–4 kbit/s).
– Generalized audio: extension of MPEG-2 AAC with additional methods in the
time-frequency domain. The basic structure is depicted in Fig. 9.19 (bit rate
6–64 kbit/s).
Basics of speech coders can found in [Var06]. The speciﬁed audio coders allow coding with
lower bit rates (Parametric Audio and Structured Audio) and coding with higher quality at
lower bit rates compared to MPEG-2 AAC.
Compared to coding methods such as MPEG-1 and MPEG-2 introduced in previous
sections, the parametric audio coding is of special interest as an extension to the ﬁlter bank
methods [Pur99, Edl00]. A parametric audio coder is shown in Fig. 9.32. The analysis
of the audio signal leads to a decomposition into sinusoidal, harmonic and noise-like sig-
nal components and the quantization and coding of these signal components is based on
psychoacoustics [Pur02a]. According to an analysis/synthesis approach [McA86, Ser89,
Smi90, Geo92, Geo97, Rod97, Mar00a] shown in Fig. 9.33 the audio signal is represented
in a parametric form given by
x(n) =
M(cid:6)
i=1
(cid:1)
Ai(n) cos
2π
fi (n)
fA
(cid:2)
n + ϕi(n)
+ xn(n).
(9.40)
The ﬁrst term describes a sum of sinusoids with time-varying amplitudes Ai(n), fre-
quencies fi (n) and phases ϕi(n). The second term consists of a noise-like component
xn(n) with time-varying temporal envelope. This noise-like component xn(n) is derived by
subtracting the synthesized sinusoidal components from the input signal. With the help of
a further analysis step, harmonic components with a fundamental frequency and multiples
of this fundamental frequency are identiﬁed and grouped into harmonic components. The
extraction of deterministic and stochastic components from an audio signal can be found in
1http://www.midi.org/.
9.7 MPEG-4 Audio Coding
305
Psycho-
acoustic
Model
Harmonic
Components
Quant
Audio
Model-based
Signal
Decomposition
Sinusoid
Components
Quant
Mux
Bit Stream
Noise
Components
Quant
Figure 9.32 MPEG-4 parametric coder.
A , f ,
i
i
i#
x(n)
Analysis
Synthesis
x (n)
s
x (n)
n
Figure 9.33 Parameter extraction with analysis/synthesis.
[Alt99, Hai03, Kei01, Kei02, Mar00a, Mar00b, Lag02, Lev98, Lev99, Pur02b]. In addition
to the extraction of sinusoidal components, the modeling of noise-like components and
transient components is of speciﬁc importance [Lev98, Lev99]. Figure 9.34 exempliﬁes
the decomposition of an audio signal into a sum of sinusoids xs(n) and a noise-like sig-
nal xn(n). The spectrogram shown in Fig. 9.35 represents the short-time spectra of the
sinusoidal components. The extraction of the sinusoids has been achieved by a modiﬁed
FFT method [Mar00a] with an FFT length of N = 2048 and an analysis hop size of RA =
512.
The corresponding parametric MPEG-4 decoder is shown in Fig. 9.36 [Edl00, Mei02].
The synthesis of the three signal components can be achieved by inverse FFT and overlap-
add methods or can be directly performed by time-domain methods [Rod97, Mei02]. A
signiﬁcant advantage of parametric audio coding is the direct access at the decoder to the
three main signal components which allows effective post-processing for the generation of
a variety of audio effects [Zöl02]. Effects such as time and pitch scaling, virtual sources in
three-dimensional spaces and cross-synthesis of signals (karaoke) are just a few examples
of interactive sound design on the decoding side.
306
Audio Coding
(cid:111)
)
n
(
x
(cid:111)
)
n
(
x
s
(cid:111)
)
n
(
n
x
0.2
0.1
0
(cid:358)0.1
(cid:358)0.2
0
0.2
0.1
0
(cid:358)0.1
(cid:358)0.2
0
0.2
0.1
0
(cid:358)0.1
(cid:358)0.2
0
1
2
3
4
5
6
7
1
2
3
4
5
6
7
8
4
x 10
8
4
x 10
1
2
3
4
n (cid:111)
5
6
7
8
4
x 10
Figure 9.34 Original signal, sum of sinusoids and noise-like signal.
9.8 Spectral Band Replication
To further reduce the bit rate an extension of MPEG-1 Layer III with the name MP3pro
was introduced [Die02, Zie02]. The underlying method, called spectral band replication
(SBR), performs a low-pass and high-pass decomposition of the audio signal, where the
low-pass ﬁltered part is coded by a standard coding method (e.g. MPEG-1 Layer III) and
the high-pass part is represented by a spectral envelope and a difference signal [Eks02,
Zie03]. Figure 9.37 shows the functional units of an SBR coder. For the analysis of the
difference signal the high-pass part (HP Generator) is reconstructed from the low-pass part
and compared to the actual high-pass part. The difference is coded and transmitted. For
decoding (see Fig. 9.38) the decoded low-pass part of a standard decoder is used by the
HP generator to reconstruct the high-pass part. The additional coded difference signal is
added at the decoder. An equalizer provides the spectral envelope shaping for the high-pass
part. The spectral envelope of the high-pass signal can be achieved by a ﬁlter bank and
computing the RMS values of each band-pass signal [Eks02, Zie03]. The reconstruction
of the high-pass part (HP Generator) can also be achieved by a ﬁlter bank and substituting
the band-pass signals by using the low-pass parts [Schu96, Her98]. To code the difference
9.8 Spectral Band Replication
307
 = 2048, R
=512
A
FFT
Spectrogram with D-FFT, N
10000
9000
8000
7000
6000
5000
4000
3000
2000
1000
→
z
H
n
i
f
0
0
200
400
600
800
1000
t in ms →
1200
1400
1600
1800
Figure 9.35 Spectrogram of sinusoidal components.
Parameter
Decoding
Model-based
Synthesis
Dequant
Harmonic
Components
Demux
Dequant
Sinusoid
Components
Bit Stream
Audio
Dequant
Noise
Components
Figure 9.36 MPEG-4 parametric decoder.
