# Audio Plugins C++ (Pirkle) — Chunk 53/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2589

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Left In
APF1
APF1
Right In
APF1
APF1
Reverb Algorithms  377
(cid:153)
(cid:153)
(cid:153)
(cid:153)(cid:3)
(cid:153)
Left Out
Similar
for Right
Out (cid:3)
Comb 1
+0.7
Comb 2
+0.7(cid:3)
Comb 3
(cid:14)(cid:19)(cid:17)(cid:26)(cid:3)
Comb 4
+0.7
Comb 1
+0.7
Comb 2
-0.7
Comb 3
+0.7
Comb 4
-0.7
 Figure 11.19:    A stereo implementation for a Schroeder reverb—only one channel is shown for
clarity; the right output is fashioned in a similar way. Notice the mix matrix values do not have to
be 61.0 but need to follow the orthogonality of alternating signs.
   11.10    Gardner’s Nested APF Reverberators
 Schroeder experimented with reverbs made only from delaying APF modules in series. The
abundant time smearing suggested that this might be a viable option. Unfortunately the
consensus was that this reverb sounded metallic and that it didn’t build up the echo density
fast enough. Schroeder’s original series APFs started with the maximum delay in the ﬁ rst
module (about 100 mSec) and then decreased by 0.3 for each successive module, through ﬁ ve
modules. Gardner (1992) noted that Schroeder (1962), Gerzon (1972), and Moorer (1979) all
experimented with  nesting  multiple all-pass structures, but without success. He suggested a
strategy for nesting delaying APF modules along a single transversal delay line. In order to
understand how this works, re-fashion the delaying APF structure using the transversal delay
line structure you saw in  Chapter 8 .  Figure 11.20  shows the same delaying APF structure as
shown in  Figure 11.13  but with the delay replaced by a set of single delay elements.

378  Chapter 11
 The sequence of operation—reading before writing—is key for implementing this design.
Speciﬁ cally, you need to
1.
2.
3.
4.
   Read the output of the last delay cell,  x ( n  2  D ).
   Read the value of the ﬁ rst delay cell,  x ( n ).
   Form the new value for the last cell 2 gx ( n ) 1  x ( n  2  D ) and write it back into the cell as  y ( n ).
   Form the new value for the ﬁ rst cell  x ( n ) 5  x ( n ) 1  gy ( n ).
 Gardner’s idea was to nest multiple APF structures inside each other so that they  shared
the same delay line. This would produce layers of embedded echoes.  Figure 11.20  shows a
delaying APF with a total delay time of  z  25 . Nesting another APF with a delay of  z  23  inside it
results in  Figure 11.21 .
 Gardner also devised a new schematic representation of his nested ﬁ lter structures that
removes the clutter of the delay cells, gain, and summation components.  Figure 11.22  shows
a nested APF structure. The outer APF has a delay time  D 2 and gain  g  2  while the inner
APF uses  D 1 and  g  1  as its values. Additionally, pre- and post-delays may be added to the
transversal delay line before and after the nested structure as shown in  Figure 11.23 .
x(n)
-1 z
-1 z
-1 z
-1 z
-1 z
y(n)
(cid:153)
g
-g
(cid:153)
 Figure 11.20:    The delaying APF structure sitting across a transversal delay line.
-g
1
(cid:153)
1 g
(cid:153)
x(n)
-1 z
-1 z
(cid:153)
-1
z
2 g
-g 2
-1
z
-1 z
y(n)
(cid:153)
 Figure 11.21:    Two delaying APF structures sharing the same delay line.
Input
D2(g2)
D1(g1)
Reverb Algorithms  379
Output
 Figure 11.22:    A nested APF schematic; Gardner gave the delay times in mSec.
-g1
g1
x(n)
z -N
z -1
z -1
z -1
z -1
z -1
z -M
y(n)
g
2
-g
2
Input
Output
N
M
 Figure 11.23:    Pre- and post-delays of length  N  and  M  have been added to the nested APF
structure. The second diagram shows how Gardner would lay this out schematically.
 Consider the nested APF structure in  Figure 11.24 . It has an outer APF with a delay time of
35 mSec with  g  5 0.3 and two inner APFs with delay and gain values of 22 mSec ( g  5 0.4)
and 8.3 mSec ( g  5 0.6). This is actually the ﬁ rst part of one of Gardner’s larger designs.
 Figure 11.25  shows the impulse responses as each APF is added to the structure.
 You can certainly see how the echo density begins to grow as you add each APF unit. We note
several things about the nesting system:
(cid:129)
(cid:129)
   In the example, the ﬁ rst nested APF is 22 mSec in length; because of the commutative
property of the delay operator, it doesn’t matter where the 22 mSec delay is placed within
the 35 mSec outer element.
   The 8.3 mSec APF is not nested inside the 22 mSec APF; it comes anywhere after it but
still inside the outer APF.
380  Chapter 11
Input
35 (0.3)
Output
22 (0.4)
8.3  (0.6)
 Figure 11.24:    Three APFs—an outer ﬁ lter and two nested units.
(a)
0
4410
8820
13230
I7640
22050
26460
30870
35280
39690
(b)
0
4410
(cid:27)(cid:27)(cid:21)(cid:19)
13230
17640
22050
26460
308(cid:26)(cid:19)
35280
39690
(c)
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
0
4410
(cid:3)(cid:27)(cid:27)(cid:21)(cid:19)
(cid:20)(cid:22)(cid:21)(cid:22)(cid:19)(cid:3)
17640
22050
26460
30870
35280
39690
 Figure 11.25:    (a) The output of the single outer APF with delay of 35 mSec and gain of 0.3
shows the typical APF impulse response. (b) The ﬁ rst nested APF with delay of 22 mSec has
been added. (c) The second nested APF with delay of 8.3 mSec has been added to the ﬁ rst two.
(cid:129)
(cid:129)
(cid:129)
   Echo density piles up as time increases.
   The system can produce ringing or instabilities.
   The system still sounds metallic.
 Gardner’s solution to the ringing or metallic sound was to create a feedback path around
the entire system with an LPF in the loop, thus embedding the nested APFs in a massive
comb ﬁ lter. He also notes that his reverb designs were ultimately arrived at by trial-and-
error.  Figure 11.26  shows Gardner’s three reverb designs for small, medium, and large room
simulations. In each algorithm, the reverb time is controlled by the loop gain or “g” control
(notice there are two of them in the medium room algorithm).
Reverb Algorithms  381
Input
35  (0.3)
Small
66(0.1)
0.5
22  (0.4)
8.3  (0.6)
30  (0.4)
24
0.5
Output
fc=4.2kHz
g
LPF
Medium
Input
35  (0.3)
8.3  (0.7)
22  (0.5)
30  (0.5)
Input
39  (0.3)
9.8(0.6)
5
67
15
g
fc=  2.5  kHz
g
LPF
Large
Input
8  (0.3)
12(0.3)
62  (0.25)
76  (0.25)
30  (0.25)
87  (0.5)
120(0.5)
4
17
31
3
fc=  2.6  kHz
g
LPF
Output
Output
108
0.5
0.5
0.5
0.34
0  14
0.14
 Figure 11.26:    From top to bottom these are Gardner’s nested APF reverb algorithms for small,
medium, and large room simulations. Notice that the medium room algorithm in the middle has
two input locations. Also notice the use of pre- and post-delays on some of the nested modules.
   11.11    Modulated APF and Comb/APF Reverb
 The modulated delay line can be used to further increase time smearing and echo density
by using it in an APF module. The low-frequency oscillator (LFO) rate is kept low (<1 Hz)
and the depth also very low (10–20 samples) to ensure that the detuning and chorusing effect
is not overly obvious. In 1997, Dattorro presented a plate reverb algorithm “in the style of
Greisinger,” revealing the use of a modulated APF. He also notes that the modulated ﬁ lter
technically mimics a room whose walls are slowly moving back and forth.
 Frenette (2000) showed that the use of modulated delay lines could reduce the computational
requirements without a perceptual reduction in quality of reverb. Frenette used modulated
382  Chapter 11
comb ﬁ lters (chorus modules) in addition to modulated APFs to further reduce the overall
complexity and memory requirements. The modulated APF is shown in  Figure 11.27 .
 The modulated APF in  Figure 11.27  modulates only the very end of the delay line, producing
a delay described by  Equation 11.29 :
  Delay 5 z(2D 1 u(nT))
(11.29)
 where  u ( nT ) is the discrete time modulation signal (LFO). The inverting version can be
implemented with a swap of the  g  coefﬁ cients, as with the other APFs.
   11.12    Dattorro’s Plate Reverb
 Dattorro’s plate reverb algorithm in  Figure 11.28  has a block diagram that reveals its ﬁ gure-8
recirculating tank circuit. This ﬁ gure-8 circuit could be applied to the other reverb block
diagrams to generate countless variations on this theme.
(cid:91)(cid:11)(cid:81)(cid:12)(cid:3)
(cid:16)(cid:74)(cid:3)
(cid:92)(cid:11)(cid:81)(cid:12)(cid:3)
z-D
(cid:74)(cid:3)
 Figure 11.27:    A modulated APF.
(cid:91)(cid:11)(cid:81)(cid:12)(cid:3)
Pre-Delay
LPF1
APF1
APF2
APF3
APF4
Modulated  APF1
Delay 1
LPF2
APF5
Delay 2
a
(cid:74)5
(cid:74)5
b
c
Modulated  APF2
Delay 3
LPF3
APF6
Delay 4
d
e
f
 Figure 11.28:    Block diagram of Dattorro’s reverb.

Reverb Algorithms  383
 You might notice something strange about  Figure 11.29 —it has no output  y ( n ) node. In fact,
the left and right outputs are taken from various locations within the delay lines, marked a–f
in the diagram. This is a mono-in, stereo-out reverberator. The ﬁ rst LPF is marked “diffusion”
while the second pair (LPF2 and LPF3) are designated “damping.” The ﬁ rst ﬁ lter controls
the diffusion in the series APFs while the second pair controls the high-frequency roll-off
in the tank circuit. These LPFs are all DC -normalized single pole feedback ﬁ lters shown in
 Figure 11.30 .
  Table 11.2   gives the various values for the ﬁ lters, followed by the equations that give the left
and right outputs. The original design was for a sample rate of 29.8 kHz. Values for  f  s  5 44.1
kHz are calculated and given in the tables.  Figure 11.30  shows the entire reverb algorithm
block diagram. Table 11.3 lists the control ranges and defaults.
  Table 11.2:    Gain and delay values for Dattorro’s plate reverb.
APF Dx
Delay (samples)
Delay (samples)
g (index)
g (value)
1
2
3
4
9
10
Fixed Delay Dx
7
8
11
12
Mod APF Dx
5
6
(cid:153)
x(n)
1-g
fs = 29.8 k
142
fs = 44.1 k
210
107
379
277
2656
1800
158
561
410
3931
2664
Delay (samples)
fs = 29.8 k
4217
Delay (samples)
fs = 44.1 k
6241
4453
3136
3720
6590
4641
5505
Delay (samples)
fs = 29.8 k
908 +/–8
Delay (samples)
fs = 44.1 k
1343+/–12
672 +/–8
995+/–12
y(n)
x(n)
g
-1 z
1
1
2
2
3
3
–
–
–
–
–
0.75
0.75
0.625
0.625
0.5
0.5
–
–
–
–
–
g (index)
g (value)
0.7
0.7
4
4
(cid:153)
y(n)
-1 z
(a)
g
(b)
1-g
 Figure 11.29:    (a) DC-normalized one-pole ﬁ lter, easy to use in reverb algorithms, including the
comb ﬁ lter and LPF type module. (b) This version merely reverses the effect of the slider or control.
384  Chapter 11
Left  In
Right  In
Pre-Delay
z-D
bw
-D1
z
-D2
Z
g1
-g1
g1
-g1
-g3
z-D5
g3
z-D7
1-d
g5
Damping
z-1
d
g4
-g4
z-D9
Bandwidth
z-1
1-bW
-D3
z
-D4
Z
-g3
g2
-g2
g2
-g2
z-D6
g3
z-D8
1-d
g5
Damping
z-1
d
g4
-g4
-D10
z
-D11 z
g5
-D12
z
g 5
 Figure 11.30:    Dattorro’s plate reverb algorithm.
Reverb Algorithms  385
  Table 11.3:    Control values for Dattorro’s plate reverb.
Control
Decay (g5)
Bandwidth
Damping
Range
0.0–1.0
0.0–1.0
0.0–1.0
Default
0.5
0.9995
0.0005
 The left and right outputs ( Equation 11.30 ) are summed from points within the delay lines
labeled a–f in  Figure 11.28 .
  fs  5  29.8 kHz:
yL 5 a3266 4 1 a32974 4 2 b31913 4 1 c31996 4 2 d31990 4 2 e3187 4 2 f 31066 4
yR 5 d3353 4 1 d33627 4 2 e31228 4 1 f 32673 4 2 a32111 4 2 b3335 4 2 c3121 4
 (11.30)
  fs  5  44.1 kHz:
yL 5 a3394 4 1 a34401 4 2 b32831 4 1 c32954 4 2 d32945 4 2 e3277 4 2 f 31578 4
yR 5 d3522 4 1 d35368 4 2 e31817 4 1 f 33956 4 2 a33124 4 2 b3496 4 2 c3179 4
   11.13    Generalized Feedback Delay Network Reverbs
 Another approach to reverberation is to realize that the listener is experiencing dense
echoes caused by multiple reﬂ ections off of surfaces along with potentially constructive or
destructive interference. The generalized  feedback delay network  (FDN) approach began with
Gerzon’s work in 1972 on preserving energy in multichannel reverberators. It was continued
by Stautner and Puckette (1982) as well as Jot and Chaigne (1991 ). Generally speaking, the
idea is to model the room with some number of delay lines with potential feedback paths to
and from every delay line in the system. The inputs and outputs of each delay line are also
scaled by some values  b  and  c . Consider a simple version that consists of two delay lines with
feedback paths to and from each one shown in  Figure 11.31 .
 If you look at  Figure 11.31  and think about Schroeder’s parallel comb ﬁ lter bank, you can see
that this is a variation, indeed a generalization, on the structure. In the general FDN, every
possible feedback path is accounted for. Additionally, each delay line has input and output
amplitude controls. If you let  b 1,  b 2,  c 1, and  c 2 all equal 1.0 and set  g 12 and  g 22 to 0.0, you
get Schroeder’s parallel comb ﬁ lter network exactly. Notice how the feedback coefﬁ cients are
labeled:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
    g 11: Feedback from delay line 1 into delay line 1
    g 12: Feedback from delay line 1 into delay line 2
    g 21: Feedback from delay line 2 into delay line 1
    g 22: Feedback from delay line 2 into delay line 2
