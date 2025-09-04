# Audio Plugins C++ (Pirkle) — Chunk 18/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1834

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
104  Chapter 5
 The  transfer function  of the filter is the ratio of output to input. The  frequency response  of the filter
is the magnitude of the transfer function evaluated at different frequencies across its spectrum.
The  phase response  of the filter is the argument (or angle) of the transfer function evaluated at
different frequencies across its spectrum.
 To produce the frequency and phase response graphs, you evaluate the function for various val-
ues of v then find the magnitude and argument at each frequency. The evaluation uses Euler’s
equation to replace the  e  term and produce the real and imaginary components.
 What is so signiﬁ cant about this is that the transfer function is not dependent on time
even though the input and output signals are functions of time. The transfer function
( Equation 5.14 ) is only dependent on frequency v, so we call it H(v).
  H(v) 5 a0 1 a1e2jv1
(5.14)
 Notice that the transfer function is complex.
 But what values of v are to be used in the evaluation? We know that v 5 2p f , but do we
really care about the frequency in Hz? In  Chapter 4  when you analyzed the same ﬁ lter, you
applied DC, Nyquist, ½ Nyquist, and ¼ Nyquist without thinking about the actual sampling
frequency. This is called  normalized frequency  and is usually the way you want to proceed in
analyzing DSP ﬁ lters. The actual sampling rate determines Nyquist but the overall frequency
range (0 Hz to Nyquist) is what we care about. To normalize the frequency, you let  f  5 1 Hz
in v 5 2p f , then v varies from 0 to 2p or across a 2p range. There is also one detail we have
to be aware of: negative frequencies.
  5.4.1  Negative Frequencies
 You may have never thought a frequency could be negative, but it can. When you ﬁ rst learned
about the concept of a waveform’s frequency, you were taught that the frequency is 1/T,
where T is the period, as shown in  Figure 5.6 .
 The reason the frequencies came out as positive numbers is because the period is deﬁ ned as
 t  2  2  t  1 , which makes it a positive number. But, there’s no reason you couldn’t deﬁ ne the
period to be the other way around: T 5  t  1  2  t  2 , except that it implies that time is running
backwards. Mathematically, time can run backwards. This means that for every positive
frequency that exists, there is also a negative “twin” frequency. When you look at a frequency
response plot you generally only look at the positive side.  Figure 5.7  shows a low-pass
response up to the highest frequency in the system, Nyquist.
 However, in reality, the ﬁ lter also operates on the negative frequencies just the same in
a mirror image. In this case, as the negative frequencies get higher and higher, they are
attenuated just like their positive counterparts ( Figure 5.8 ). And it makes sense too. If

Basic DSP Theory  105
A
H
t
T = t2 -  t1
t
1
t 2
 Figure 5.6:    The classic way of deﬁ ning the
period,  T .
Nyquist
f
 Figure 5.7:    The classic way of showing a
frequency response plot only shows the positive
portion.
H
- f
-Nyquist
+f
+ Nyquist
 Figure 5.8:    The more complete frequency response plot contains
both positive and negative sides.
-Nyquist
H
+Nyquist
-π
-π/2
0
π/2
+π
 Figure 5.9:    One way to divide the 2p range of frequencies includes
both positive and negative frequencies.
you take an audio ﬁ le and reverse it in time, then run it through a low-pass ﬁ lter, the same
frequency ﬁ ltering still occurs.
 For ﬁ lter evaluation, v varies on a 0 to 2p radians/second range and one way to think about
this 2p range is to split it up into the range from 2p to 1p corresponding to 2Nyquist to
1Nyquist ( Figure 5.9 ).
106  Chapter 5
H
0
π/2
π
1/2 fs
3π/2
2π
fs
 Figure 5.10:     Mapping the 0 to 2p range of frequencies
across the 0 to fs  range.
 To evaluate the transfer function, let v vary from 0 to p and get the first half of the response.
The other half is a mirror image of the data.
   5.4.2  Frequencies Above and Below 6Nyquist
 The sampling theorem sets up the Nyquist criteria with regards to completely recovering the
original, band-limited signal without aliasing. However, frequencies above Nyquist and all
the way up to the sampling frequency are also allowed mathematically. And in theory, any
frequency could enter the system and you could sample it without limiting Nyquist. For a
frequency or phase response plot, the frequencies from Nyquist up to the sampling frequency
are a mirror image about Nyquist. This is another way to divide up the 2p range by going
from 0 Hz to the sampling frequency (Figure 5.10).
 Notice that in either method the same information is conveyed as we get both halves of the
curves, and in both cases, Nyquist maps to p and 0 Hz to 0 and positive frequencies map to
the range 0 to p.
    5.5    Evaluating the Transfer Function H(v)
 DSP ﬁ lter transfer functions will contain  e 2jvn   terms that need to be evaluated over the
range of 0 to p; the way to do this is by using Euler’s equation to decompose the sinusoid
into its real (cos) and imaginary (sin) components. Then, evaluate the cos and sin terms
at the frequency in question. In the last chapter you manually calculated the input/output
relationship of a ﬁ lter by cranking through the ﬁ lter operation, one step at a time. In this
improved method, you only need to solve the transfer function equation. Start with the block
diagram in  Figure 5.11 .
 The transfer function is as follows:
  H(v) 5 a0 1 a1e2jv1
(5.15 )

Basic DSP Theory  107
x(t)
a0
Σ
y(t)
-jωl
e
a1
 Figure 5.11:    First-order feed-forward
block diagram.
 Table 5.1:    Sine and cosine function evaluations at DC, ¼ Nyquist,
½ Nyquist, ¾ Nyquist, and Nyquist.
Frequency v
0
p/4
p/2
3p/4
p
cos(v)
1.0
0.707
0.0
0.707
21.0
sin(v)
0.0
0.707
1.0
20.707
0.0
 Use the ﬁ lter coefﬁ cients a 0  5 0.5, a 1  5 0.5. You can use  Table 5.1  to help with the
evaluation. Evaluate at the following frequencies:
•
•
•
•
   DC: 0
   Nyquist: p
   ½ Nyquist: p/2
   ¼ Nyquist: p/4
 Evaluation is a two-step process for each frequency:
1.
2.
   Use Euler’s equation to convert the  e  terms into real and imaginary components.
   Find the magnitude and argument of the complex equation.
  5.5.1  DC (0 Hz)
  H(v) 5 0.5 1 0.5e2jv1
 5 0.5 1 0.5(cos(v) 2 jsin(v))
 5 0.5 1 0.5(cos(0) 2 jsin(0))
  5 0.5 1 0.5(1 2 j0)
 5 1.0 1 j0
(5.16)

108  Chapter 5
  Find the magnitude and phase at this frequency:
 0 H(v) 0 5 "(a 1 jb)(a 2 jb)
 5 "(1 1 j0)(1 2 j0)
 5 1.0
  Arg(H) 5 tan21(b/a)
5 tan21(0/1)
 5 0.0+
(5.17)
 Compare these mathematical results ( Equations 5.16  and  5.17 ) with the graphical ones from
the last chapter ( Figure 5.12 ).
   5.5.2  Nyquist (p)
   H(v) 5 0.5 1 0.5e2jv1
 5 0.5 1 0.5(cos(v) 2 jsin(v))
 5 0.5 1 0.5(cos(p) 2 jsin(p))
 5 0.5 1 0.5 (21 2 j0)
 5 0 1 j0
   0 H(v) 0 5 "(a 1 jb)(a 2 jb)
 5 "(0 1 j0)(0 2 j0)
 5 0.0
  Arg(H) 5  tan21(b/a)
 5  tan21(0/0)
 5 0+
+ 1.0
-1.0
Input
+1.0
-1.0
 Figure 5.12:    The graphical results show the same information. The magnitude
is 1.0 and the phase shift is 0.
(5.18)
(5.19)
Output

+ 1.0
-1.0
Input
+1.0
-1.0
Basic DSP Theory  109
Output
 Figure 5.13:    The graphical results show the same information at Nyquist—the magnitude is 0
and there is no phase shift since there is nothing there to shift.
 The inverse tangent argument is 0/0 and the phase or Arg(H) is deﬁ ned to be 0 under this
condition. The C11 function you use is  arctan2 ( im,re ), which performs the inverse tangent
function; it will also evaluate to 0 in this case. Now, compare our results to the last chapter’s
graphical results ( Figure 5.13 ).
5.5.3     ½ Nyquist (p/2)
  H(v) 5 0.5 1 0.5e2jv1
 5 0.5 1 0.5(cos(v) 2 jsin(v))
 5 0.5 1 0.5(cos(p/2) 2 jsin(p/2))
 5 0.5 1 0.5(0 2 j1)
   5 0.5 2 j0.5
   0 H(v) 0 5 "(a 1 jb)(a 2 jb)
 5 "(0.5 1 j0.5)(0.5 2 j0.5)
 5 "0.25 1 0.25 5 "0.5
 5 0.707
   Arg(H  ) 5 tan21(b/a)
 5 tan21(20.5/0.5)
 5 245+
(5.20)
(5.21)
 Compare this to the last chapter’s graphical results ( Figure 5.14 ); the magnitude is 0.707 with
a phase shift of 245 degrees, and the results agree.
   5.5.4  1/4 Nyquist (p/4)
   H(v) 5 0.5 1 0.5e2jv1
 5 0.5 1 0.5(cos(v) 2 jsin(v))
(5.22)

110  Chapter 5
+ 1.0
-1.0
+ 1.0
Time
Smearing
-1.0
Phase Shift
Input
Output
 Figure 5.14:    Graphical results from the last
chapter at ½ Nyquist.
 5 0.5 1 0.5(cos(p/4) 2 jsin(p/4))
 5 0.5 1 0.5(0.707 2 j0.707)
 5 0.853 2 j0.353
 0 H(v) 0 5 "(a 1 jb)(a 2 jb)
 5 "(0.853 1 j0.353)(0.853 2 j0.353)
 5 "0.728 1 0.125 5 "0.853
 5 0.923
 Arg(H  ) 5  tan21(b/a)
 5  tan21(20.353/0.853)
 5 222.5+
(5.23)
 Compare to the last chapter’s graphical results ( Figure 5.15 ); you can see how much more
accuracy we get with the mathematical calculation. The magnitude and phase shift look about
right when compared to the graphs.
 Now, you can combine all the evaluations together and sketch out the frequency response of
the ﬁ lter ( Figure 5.16 ).
