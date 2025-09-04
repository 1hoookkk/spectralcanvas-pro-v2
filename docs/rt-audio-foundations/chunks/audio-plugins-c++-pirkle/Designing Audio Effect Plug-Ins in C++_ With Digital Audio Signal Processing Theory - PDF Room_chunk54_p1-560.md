# Audio Plugins C++ (Pirkle) — Chunk 54/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1724

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

386  Chapter 11
x(n)
b1
z-D1
Dry
c1
y(n)
g11
g12
b2
z-D2
c2
g22
g21
 Figure 11.31:    A two-delay-line feedback network.
 An equivalent way to look at  Figure 11.31  is shown in  Figure 11.32 .  Y ou can see that the
feedback coefﬁ cients have been grouped together in what looks like a linear algebra matrix.
The total energy is preserved if the matrix of coefﬁ cients is  unitary . A unitary matrix
multiplied by its  transpose matrix  results in the  identity matrix . The transpose matrix is
formed by turning the rows into columns and vice versa. These matrices are square. The
identity matrix is a square matrix whose diagonal values are all 1. An FDN that uses a unitary
matrix for its coefﬁ cients is called a  unitary feedback delay network  or UFDN.
 Examine the gain matrix  G  in  Equation 11.31 :
G 5 c
0   1
21   0
d
Then multiplying it by its transpose yields
  GGT 5 c
   0
21
d c
1
0
0 21
   0
1
d 5 c
1
0
0
1
d
(11.31)
Reverb Algorithms  387
g11
g21
g12
g22
Dry
x(n)
b1
∑
b2
∑
Z –D1
Z –D2
c1
∑
y(n)
c2
 Figure 11.32:    Another version of the two-delay-line FDN.
g11
g21
g12
g22
Dry
c1
c2
∑
y(n)
x(nn)
b 1
∑
Z –D1
k1
b 2
∑
Z –D2
k2
γ
 Figure 11.33:    The FDN with decay factor control.
 It is a unitary matrix. This reverberator would ring forever because of 1.0 gain values. Jot
proposed adding an absorptive coefﬁ cient  k  to each delay line. For a colorless reverb, the
value for  k  in  Equation 11.32  is given by Jot.
where
k 5 gM
M 5  the  length  of  the  delay
 g 5  the  decay  factor,  set  by  the  user
(11.32)
 In  Figure 11.33 , each delay line undergoes the proper attenuation to keep the reverb colorless.
However, it does not include the frequency dependent absorptive losses we noted from the
energy decay relief diagrams. To accomplish this, Jot then suggested inserting LPFs instead

388  Chapter 11
of the static attenuators. The LPF magnitudes are cleverly chosen in relation to the frequency-
dependent reverb time,  RT  60 (v) in  Equation 11.33 :
20 log 1 |h|2 5
260Ts
RT60
1 v 2  M
(11.33)
where
M 5 delay length
   h 5 the magnitude of the filter at some frequency v
 The problem with this setup is that the pole radii are no longer the same (circular in the  z
plane) and we know that this produces coloration in the signal. The solution (Jot 1992) was to
add a correction ﬁ lter at the end of the whole system,  T ( z ), whose magnitude is proportional
to the inverse square root of the frequency-dependent reverb time,  RT  60 (v). This is shown in
 Figure 11.34 .
 Finally,  Figure 11.35  shows a generalized version of the FDN. It does not include the absorptive
loss components (either static  k  values or  H  LPFs ) for clarity. The feedback matrix is always square,
 N  ×  N , where  N  is the number of delay lines. Because it is square, it can be made to be unitary.
 As an experiment, you can try to implement a four delay line version using the unitary matrix
 Equation 11.34  and gain coefﬁ cients proposed by Stautner and Puckette (1982), who were
researching multichannel reverb algorithms.
where
   G5
≥
g
"2
0   1   1      0
21   0   0   21
1   0   0   21
 0   1 21     0
¥
0 g 0 , 1.0
(11.34)
 You can also try to de-correlate the four delay line outputs by using Jot’s orthogonal matrix
and set the coefﬁ cients  c  N  according to  Equation 11.35 :
where
  C 5 g2
≥
1
21
1
21
¥
0 g2
0 , 1.0
(11.35)

Reverb Algorithms  389
∑
T(z)
∑
y(n)
g11  g21
g12  g22
Dry
c1
c2
x(n)
b1
 ∑
z–D1
H(z)
LP1
b2
∑
z –D2
H(z)
LP2
 Figure 11.34:    Addition of absorptive LPFs to the delay lines, plus a correction ﬁ lter  T ( z ) at the end.
g11  g21
g12  g22
gN1
gN2
g1N  g2N
gNN
Dry
c1
c2
cN
∑
y(n)
x(n)
b1
∑
∑
b2
bN
z–D1
z–D2
∑
z–DN
 Figure 11.35:    A generalized,  N -delay-line FDN.
   11.14    Other FDN Reverbs
 Smith (1985) developed a variation on the FDN theme with his waveguide reverberators.
Each waveguide consists of a two delay lines that move in opposite directions with
coupling coefﬁ cients for connecting waveguides together. Any number of waveguides may
be connected in any geometric pattern or shape, leading to a generalized set of scattering
matrices. The scattering matrices are similar to the feedback matrices—they contain the
coefﬁ cients that control the waveguide junction gains. The scattering matrices may also
