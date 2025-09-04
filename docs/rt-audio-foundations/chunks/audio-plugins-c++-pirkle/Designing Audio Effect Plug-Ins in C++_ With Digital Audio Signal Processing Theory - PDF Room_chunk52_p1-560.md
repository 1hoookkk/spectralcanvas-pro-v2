# Audio Plugins C++ (Pirkle) — Chunk 52/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1508

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
372  Chapter 11
   11.6    Schroeder’s Reverberator
 Schroeder combined a parallel comb ﬁ lter bank with two APFs to create his ﬁ rst design. The
comb ﬁ lters produce the long series of echoes and the APFs multiply the echoes, overlaying
their own decaying impulse response on top of each comb echo. The resulting reverberation
unit, shown in  Figure 11.15 , sounds marginal but it is very simple to implement.
 Schroeder suggests that the ratio of smallest to largest delay in the comb ﬁ lters should be
about 1:1.5 and originally speciﬁ ed a range of 30–45 mSec in total. The 1:1.5 ratio rule turns
out to be useful for just about any set of parallel comb ﬁ lters.
 The comb ﬁ lters should have the following properties:
(cid:129)
(cid:129)
(cid:129)
   Choose the delays to have the 1:1.5 ratio above.
   Choose delay times that have no common factors (e.g., 2191, 2971, 3253, 3307).
   Set the gain values according to  Equation 11.9 .
x(n)
∑
–D1
Z
∑
∑
-g5
∑
–D5
Z
–D5
Z
g5
g6
∑
-g6
∑
y(n)
g1
∑
-D2
Z
g2
∑
–D3
Z
g3
∑
Z–D4
g4
 Figure 11.15:    Schroeder’s original reverb design.
 The APFs should have the following properties:
(cid:129)
(cid:129)
   Choose delays that are much shorter than the comb ﬁ lters, 1 mSec to 5 mSec.
   Set both gain values the same, between 0.5 and 0.707.
Reverb Algorithms  373
   11.7    The Low-Pass Filter–Comb Reverberator
 One of the things missing from the original Schroeder reverb is the fact that in a real
room, high frequencies decay much more rapidly than low frequencies, as shown in the
EDRs of  Figure 11.6  (Moorer 1979). Placing a low-pass ﬁ lter (LPF) in the comb ﬁ lter’s
feedback path will roll off the high-frequency content of successive echoes exponentially,
which is what we want. The LPF–comb reverberator block diagram is shown in
 Figure 11.16 .
 The LPF is chosen to be a one-pole feedback type (inﬁ nite impulse response [IIR]) so that it
will introduce not only low-pass ﬁ ltering, but also its own impulse response into the echoes
going through the feedback path. In  Figure 11.17  the LPF is shown in the dotted box; notice
that it is turned around backward to follow the ﬂ ow of the feedback path.
 To ﬁ nd the difference equation, it is easier to start in the frequency domain with the  z
transforms of the comb and ﬁ rst-order feed-forward ﬁ lter, since we are already familiar with
them by now ( Equation 11.22 ).
 HC(z) 5
HLP(z) 5
z2D
12g1z2D
1
12g2z21
(11.22)
x(n)
∑
(cid:93)(cid:177)D
y(n)
g1
LPF
 Figure 11.16:    The LPF and comb ﬁ lter combination.

374  Chapter 11
x(n)
∑
z–D
y(n)
g1
∑
z–1
g2
 Figure 11.17:    The LPF–comb ﬁ lter expanded.
 Filtering the feedback loop in the frequency domain is done by simply multiplying the LPF
transfer function by the feedback term  g  1  z  2D  in  Equation 11.23 .
H(z) 5
           5
           5
Y(z)
X(z)
5
z2D
12HLP(z)g1z2D
z2D
12g1z2D
1
12g2z21
z2D
g1z2D
12g2z21
z2D
g1z2D
12g2z21
12
12
 The next step is to separate variables and multiply out the denominator:
                Y(z) c12
d 5 X(z)z2D
g1z2D
12g2z21
   Y(z)312g2z21 2 g1Y(z)z2D 4 5 X(z)z2D(12g2z21)
Y(z) 2 g2Y(z)z21 2 g1Y(z)z2D 5 X(z)z2D 2g2X(z)z2Dz21
Y(z) 2 g2Y(z)z21 2 g1Y(z)z2D 5 X(z)z2D 2g2X(z)z2D21
(11.23)
(11.24)

Reverb Algorithms  375
 Lastly, take the inverse  z  transform by inspection:
Y (z) 2 g2Y(z)z21 2 g1Y(z)z2D 5 X(z)z2D 2 g2 X(z)z2D21
then
and
y (n) 2 g2y (n21) 2 g1y(n2D) 5 x (n2D) 2 g2x (n2D21)
(11.25)
y (n) 5 x (n2D) 2 g2x (n2D21) 1 g2y (n21) 1 g1y(n2D)
 In order for the ﬁ lter combination to remain stable (after all, it is a massive feedback system)
 g  1  and  g  2  can be related as in  Equation 11.26 :
where
g 5
g2
12g1
g , 1.0
(11.26)
 Because the  RT  60  determines the pole radii, and therefore the value of  g  1 , you can rearrange
 Equation 11.26  as  Equation 11.27 :
where
   11.8    Moorer’s Reverberator
g2 5 g(12g1)
g , 1.0
(11.27)
 Moorer proposed a similar design to Schroeder’s reverberator which uses a parallel bank of
LPF–comb ﬁ lters. Because the LPFs remove energy from the system, more units are needed
in parallel for a given reverb time. Moorer’s reverb sounds better because it mimics the high-
frequency losses of a real room.
 In  Figure 11.18  you can see the differences from Schroeder’s reverb—there are more comb
units and only one all-pass on the output. The same care must be taken to ensure the pole radii
of the comb ﬁ lters are still the same, using  Equation 11.9  to set them according to the desired
reverb time.  Table 11.1  shows Moorer’s preset values for a reverb time of about 2 seconds.
  Table 11.1:    Some settings for Moorer’s reverb.
Comb Filter
1
2
3
4
5
6
All-Pass Filter
1
Delay (mSec)
50
56
61
68
72
78
Delay (mSec)
6
gCOMB (48 kHz)
0.46
0.47
0.475
0.48
0.49
0.50
g
0.7
gLPF (48 kHz)
0.4482
0.4399
0.4350
0.4316
0.4233
0.3735
–
–
   Note:  RT  60  2 seconds, total  g  5 0.83

376  Chapter 11
x(n)
∑
-D1
Z
∑
∑
-g7
∑
y(n)
g 1
LPF
∑
-D2
Z
-D7
Z
g 7
∑
∑
∑
∑
  g 2
LPF
 -D3
Z
 g3
LPF
-D4
Z
 g4
LPF
 -D5
Z
 g5
LPF
 -D6
Z
 g6
LPF
 Figure 11.18:    Moorer’s reverb.
   11.9    Stereo Reverberation
 Conducting listening tests, Schroeder (1984) found that listeners overwhelmingly preferred
a stereo reverb to a mono version of the same algorithm. Both Schroeder and Moorer’s
reverbs can be adapted for stereo usage. In fact, this scheme can be used with just about any
reverb consisting of comb and APFs. The ﬁ rst thing to note is that mathematically, there is
no reason why you can’t place the APFs before the comb ﬁ lters since their transfer functions
multiply in the  z  domain. Then, the individual outputs of the comb ﬁ lters can be combined
through a mixing matrix to provide the left and right outputs. The mixing matrix is an array
of weighting values for each comb ﬁ lter’s output. Jot proposed that the matrix should be
orthogonal for the most uncorrelated output. The mixing matrix is shown in  Equation 11.28 .
The rows are the outputs of the comb ﬁ lters and the columns are left and right.  Figure 11.19
shows a mixing matrix for the left channel of a Schroeder reverb unit.
  gD
11   11
11   21
11   11
11   21
T
(11.28)

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
