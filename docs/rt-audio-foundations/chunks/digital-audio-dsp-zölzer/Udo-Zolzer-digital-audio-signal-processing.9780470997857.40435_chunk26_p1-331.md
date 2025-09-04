# Digital Audio DSP (Zölzer) — Chunk 26/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 366

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
255
a)
b)
c)
d)
x(n)
y(m)
y(j)
0
0
fSI
CON
fSO
x(n)
2
2
32
256
y(j)
fSO
y(m)
1
1
n
m
j
j
j+1
Figure 8.16 Time-domain interpretation.
fSI
CON
fSO
x(n)
2 H (z)1
2 H (z)2
2w-3
y(m)
Figure 8.17 Sampling rate conversion with interpolation for calculating coefﬁcients of a time-variant
interpolation ﬁlter.
The measurement of TSO /TSI is illustrated in Fig. 8.19:
• The input sampling rate fSI
is increased to MZfSI using a frequency multiplier
where MZ = 2w. This input clock increase by the factor MZ triggers a w-bit counter.
The counter output z is evaluated every MO output sampling periods.
• Counting of MO output sampling periods.
• Simultaneous counting of the MI input sampling periods.
256
Sampling Rate Conversion
(cid:15)(cid:5)(cid:22)
(cid:16)
(cid:16)(cid:5)(cid:22)
 (cid:16)(cid:5)(cid:23)
3
(cid:7)
(cid:16)(cid:5)(cid:23)
(cid:22)
(cid:6)+
3
(cid:7)
(cid:16)(cid:5)(cid:22)
(cid:23)
(cid:6)+
3
(cid:7)
(cid:16)
(cid:6)+
(cid:27)
2
Figure 8.18 Calculation of ti .
- (cid:22)
(cid:6)+
(cid:27)
$(cid:11)(cid:13)(cid:10)(cid:7)(cid:2)(cid:8)
3
(cid:22)
%
(cid:6)
(cid:27)
%
(cid:6)
(
/(cid:14)(cid:14)(cid:5)(cid:22)
3
/(cid:27)
- M/
(cid:23)
+
( (cid:6)3
(cid:6)+
3
Figure 8.19 Measurement of TSO /TSI .
The time intervals d1 and d2 (see Fig. 8.19) are given by
+ z − z0
MZ
d1 = MI TSI
d2 = MOTSO ,
=
TSI
(cid:1)
MI + z − z0
MZ
(cid:2)
TSI ,
and with the requirement d1 = d2 we can write
(cid:2)
(cid:1)
MO TSO
TSO
TSI
=
MI + z − z0
TSI
MZ
= MI + (z − z0)/MZ
MO
= MZMI + (z − z0)
MZMO
.
• Example 1: w = 0 → MZ = 1
TSO
TSI
= MI
215
(8.47)
(8.48)
(8.49)
(8.50)
With a precision of 15 bits, the averaging number is chosen as MO = 215 and the
number MI has to be determined.

8.4 Interpolation Methods
