# Audio Plugins C++ (Pirkle) — Chunk 51/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2538

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Reverb Algorithms  365
 While  Figure 11.8  might look simple, the results certainly trend in the right direction. The
frequency response plot shows a set of resonant peaks that could correspond to a room’s
eigenfrequencies and the impulse response shows a set of decaying impulses, mimicking the
energy loss as the echoes bounce off of surfaces. The modal density for a comb ﬁ lter is given
by Jot and Chaigne (1991)  in  Equation 11.7 .
where
x(n)
∑
Md 5
Df  5
D
fs
fs
D
D 5  the delay length
  fs  5  the sample rate
(11.7)
y(n)
z–D
g
 Figure 11.7:    The basic comb ﬁ lter.
Im
+12.0dB
0.0 dB
–12.0dB
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
Re
2 kHz
4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
g
g2
3
g
g4
0
22
44
66
88
110
132
154
176
198
 Figure 11.8:    The poles in the  z -plane produce the resonances. The feedback that
creates those poles also recirculates and scales the echoes by  g . In this example,
  D  = 8 samples, feedback = 80%.

366  Chapter 11
 So, the comb ﬁ lter produces  D  resonances across DC to  f  s  (or  D /2 resonances from DC
to Nyquist) with each resonance separated by D  f  5  f  s  / D . This is exactly what we found
when we analyzed the comb ﬁ lter in  Chapter 7 . In order to get Schroeder’s desired
0.15 eigenfrequencies/Hz from DC to 20 kHz at  f  s  5 44,100, you would need  D  5 6622.
This also makes sense because we need about 3000 ( D /2) resonances to cover the range
from DC to 20 kHz. The problem is that while we will get the desired number of
resonances, they will be linearly spaced rather than piling up as the square of frequency,
as  Equation 11.5  predicts. Additionally, it will take 6622 samples (~150 mSec with a
44.1 kHz sample rate) before the reverberator begins outputting samples. Schroeder was
able to achieve high density by placing comb ﬁ lters in parallel, then summing the results.
This would then cause the comb ﬁ lter responses to overlap, however, care must be taken
when choosing the delay lengths—if they are mathematically related then resonances or
anti-resonances will color the sound.
  Figure 11.9  shows an example of four comb ﬁ lters in parallel. Each has its own delay time
( D 1– D 4) and gain ( g 1– g 4). However, care must be taken with the gain values. As you
remember from  Chapter 7 , the value of  g  alone determines the pole radii. If the poles of each
ﬁ lter have the same radii, their impulses will decay at the same rate. This is desirable since
it mimics an uncolored reverberation pattern. If the pole radii are different for the different
comb ﬁ lters then the resulting reverberation will have undesirable coloration in it. The pole
radii are given in  Equation 11.8  by Gardner (Kahrs and Bradenberg 1998 ). The relationship
between the pole radii, the delay  D  and the reverb time  RT   60  is given in  Equation 11.9 .
  where
or
or
   r 5 g1/D
 g  5  the feedback gain
D  5  the delay length
  RT60 5
3DTs
log11/g2
3DTs
RT60
1/g 5 10
23DTs
RT60
g 5 10
Ts is the sample period
(11.8)
(11.9)

Reverb Algorithms  367
∑
y(n)
x(n)
∑
∑
∑
∑
Z –D1
g1
Z –D2
Z –D3
Z –D4
g2
g3
g4
 Figure 11.9:    Four comb ﬁ lters in parallel.
 This means we can control the reverb time by using the gain factor  or  the delay length  D . The
tradeoff is that if we increase  g  to increase reverb time, the poles get very near the unit circle,
causing resonances. If we increase  D , then the echoes become distinctly audible rather than
smearing together. This means that there is a tradeoff of the modal density versus the echo
density in the design. The modal density for a parallel bank of  N  comb ﬁ lters in  Equation
11.10  is given by Jot (1991). From  Equation 11.10  you can see that the modal density remains
constant across all frequencies, which is not what happens in real rooms where the modal
density increases with the square of the frequency ( Equation 11.5 ).
N
Md 5 a
i50
DiTs
where
or
where
Di 5  the delay length of the  ith comb filter
Ts 5  the sample period
(11.10)
Md 5 NDTs
  D 5  the mean delay length of all filters averaged

368  Chapter 11
 The echo density for the parallel combs with delay lengths close to each other is given in
 Equation 11.11  (Jot 1991).
N
Ed 5 a
i50
1
DiTs
where
or
where
Di 5  the delay length of the ith comb filter
(11.11)
Ed 5
N
DTs
 D 5  the mean delay length of all filters averaged
 Knowing the desired  M  d  and  E  d  you can then calculate the number of parallel comb ﬁ lters  N
with average delay time  DT  s  with  Equation 11.12 .
N 5 "EdMd
DTs 5 "Md /Ed
(11.12)
 Plugging Schroeder’s values of  M  d  5 0.15 eigenfrequencies/Hz and  E  d  5 1000 echoes/sec
into  Equation 11.12  yields  N  5 12 and the average delay time  DT  s  5 12 mSec.
   11.4    The Delaying All-Pass Filter Reverberator
 Schroeder also proposed the delaying all-pass ﬁ lter (APF) as a reverberator unit. The impulse
response is a decaying set of echoes but the frequency response is technically ﬂ at. Schroeder’s
APF is shown in  Figure 11.10 . In  Figure 11.11 , you can see that we get echoes that decay
faster than the comb ﬁ lter and at a different rate. It has a ﬂ at magnitude response due to the
APF reciprocal zero/pole radii. However, as Gardner  points out, our ears only perform a
short-time integration, whereas the APF requires an inﬁ nite integration time to achieve the ﬂ at
response, so we will still hear some timbral coloration in this unit.
x(n)
-g
∑
y(n)
c(n)
∑
–D
Z
d(n)
1(cid:16)g2
g
 Figure 11.10:    Schroeder’s APF reverberator.

Reverb Algorithms  369
1- g2
g - g3
g2-g4
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
-g
0
2205
4410
6615
8820
11025
13230
15435
17640
19845
 Figure 11.11:    Impulse response of Schroeder’s APF reverberator; the frequency response is ﬂ at.
The delay time was  D  = 35 mSec with  g  = 0.6.
 Inspection of the block diagram in  Figure 11.10  reveals that this is a rather complex feed-
back/feed-forward structure. We need to extract the difference equation so we can synthesize
the reverb unit. To start, we label the nodes  c ( n ) and  d ( n ) in the block diagram, then fashion
the output  y ( n ) with respect to them:
c(n) 5 x(n)1gd(n)
d(n) 5 c(n)z2D
y(n) 5 2gx(n)1(12g2)d(n)
(11.13)
 Now, we expand out the internal nodes and use the familiar time and frequency shifting
properties of the  z  transform to continue  Equation 11.14 .
d(n) 5 c(n)z2D
          5 x(n2D) 1 gd(n2D)
y(n) 5 2gx(n)1(12g2)3x(n2D)1gd(n2D)4
(11.14)
 Examining the last term in  Equation 11.14 ,  d ( n 2 D ), we can rearrange  Equation 11.13  to
continue and get  Equation 11.15 :
d(n) 5
y(n)1gx(n)
(12g2)
so
(11.15)
d(n2D) 5
y(n2D)1gx(n2D)
(12g2)
 Substituting  Equation 11.15  back into  Equation 11.14  you get the difference equation in
 Equation 11.16 :
y(n) 5 2gx (n) 1 (12g2) cx (n2D) 1 ga
y (n2D) 1 gx(n2D)
(12g2)
b d
          5 2gx (n) 1 (12g2) x (n2d) 1 gy(n2D)  1 g2x (n2D)
          5 2gx (n)1x(n2d)1gy (n2D)
(11.16)

370  Chapter 11
   11.5    More Delaying All-Pass Filter Reverberators
 There are multiple ways to synthesize the delaying APF.  Figure 11.12  shows another one.
 D eriving the difference equation is easier for this version; we only need to deﬁ ne one extra
node,  w ( n ) in  Equation 11.17 :
w(n) 5 x (n) 1 gw(n2D)
y(n) 5 2gw(n)1w(n2D)
         5 2g3 x (n)1gw(n2D)4  13x (n2D) 1 gw(n22D)4
         5 2gx (n)2g2w (n2D) 1 x (n2D) 1 gw (n22D
        5 2gx (n) 1 x (n2D) 2 g2w (n2D) 1 gw (n22D)
grouping:
(11.17)
 In order to ﬁ nish, ﬁ nd  y ( n  2  D ) from the second line in  Equation 11.18  and notice that this is
the same as the last two terms in  Equation 11.17 .
        y(n) 5 2gw(n) 1 w(n2D)
then
and
y (n2D) 5 2gw (n2D) 1 w (n22D)
(11.18)
gy (n2D) 5 2g2w (n2D) 1 gw (n22D)
 Thus, the last two terms in  Equation 11.17  can be replaced by  gy ( n 2 D ), and the ﬁ nal
difference equation matches Schroeder’s APF:
  y(n) 5 2gx (n) 1 x (n2D) 1 gy (n2D)
(11.19)
  Figure 11.13  shows another version of the same APF. The proof that it has the same
difference equation is easy if you look at the node  w ( n ) in  Equation 11.20 :
w(n) 5 x (n) 1 gw (n2D)
y (n)  5 2gw (n) 1 w(n2D)
(11.20)
x(n)
∑
w(n)
z-D
g
-g
∑
y(n)
 Figure 11.12:    Another version of the delaying all-pass reverberator.

 These are the ﬁ rst two lines in the derivation for the delaying APF above.  Figure 11.14  shows
yet another structure. To ﬁ gure this one out, once again ﬁ nd the internal node  w ( n ) as it relates
to the output in  Equation 11.21 :
Reverb Algorithms  371
then
and
      w (n) 5 x (n) 1 gy (n)
        y(n) 5 2gx (n) 1 w (n2D)
w (n2D) 5 x (n2D) 1 gy (n2D)
       y (n) 5 2gx (n) 1 x (n2D) 1 gy (n2D)
(11.21)
  Equation 11.21  is identical to  Equation 11.20 , but care must be taken in the synthesis
of this in code—you must form the internal nodes ﬁ rst to avoid a zero-delay loop (the
 x ( n ) 1  gy ( n ) term above). Inverted APFs simply swap signs on the  g  coefﬁ cients. This has
the effect of inverting the impulse response while keeping the frequency response
theoretically ﬂ at. By combining both inverting and regular delaying APFs, you can try to
achieve a more uncorrelated echo density.
x(n)
∑
gw(n-D)
-gw(n)
y(n)
∑
w(n)
-g
g
z -D
w(n-D)
 Figure 11.13:    Alternate version of the delaying APF; the transfer function and difference
equations are identical to the other structures.
x(n)
y(n)
-g
g
w(n)
∑
gy(n)
z -D
-gx(n)
∑
w(n-D)
 Figure 11.14:    Another delaying APF structure.

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
