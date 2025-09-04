# Understanding DSP (Lyons) — Chunk 20/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 932

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Appendix B. Closed Form of a Geometric Series
In the literature of digital signal processing, we often encounter geometric series expressions like
or
(B-1)
(B-2)
Unfortunately, many authors make a statement like “and we know that” and drop Eqs. (B-1) or (B-2)
on  the  unsuspecting  reader  who’s  expected  to  accept  these  expressions  on  faith.  Assuming  that  you
don’t have a Ph.D. in mathematics, you may wonder exactly what arithmetic sleight of hand allows us
to arrive at Eqs. (B-1) or (B-2)? To answer this question, let’s consider a general expression for a
geometric series such as
(B-3)
where n, N, and p are integers and a and r are any constants. Multiplying Eq. (B-3) by r gives us
(B-4)
Subtracting Eq. (B-4) from Eq. (B-3) gives the expression
or
S – Sr = S(1 − r) = arp – arN,
(B-5)
So here’s what we’re after. The closed form of the series is
(B-6)
(By “closed form,” we mean taking an infinite series and converting it to a simpler mathematical form
without the summation.) When a = 1, Eq. (B-6) validates Eq. (B-1). We can quickly verify Eq. (B-6)
with an example. Letting N = 5, p = 0, a = 2, and r = 3, for example, we can create the following list:
Plugging our example N, p, a, and r values into Eq. (B-6),
(B-7)
which equals the sum of the rightmost column in the list above.
As a final step, the terms of our earlier Eq. (B-2) are in the form of Eq. (B-6) as p = 0, a = 1, and r =
e–j2πm/N.† So plugging those terms from Eq. (B-2) into Eq. (B-6) gives us
† From the math identity axy = (ax)y, we can say e–j2πnm/N = (e–j2πm/N)n, so r = e–j2πm/N.
(B-8)
confirming Eq. (B-2).

Appendix C. Time Reversal and the DFT
The notion of time reversal in discrete systems occasionally arises in the study of the discrete Fourier
transform  (DFT),  the  mathematical  analysis  of  digital  filters,  and  even  in  practice  (straight  time
reversal is used in a digital filtering scheme described in Section 13.12). We give the topic of time
reversal  some  deserved  attention  here  because  it  illustrates  one  of  the  truly  profound  differences
between the worlds of continuous and discrete systems. In addition, the spectral effects of reversing a
time sequence are (in my opinion) not obvious and warrant investigation.
Actually,  in  discrete-time  systems  there  are  two  forms  of  time  reversal  we  need  to  think  about.
Consider the 6-point x(n) time-domain sequence
(C-1)
Due to the periodicity properties of discrete sampled representations (discussed in Section 3.17), we
can  depict  the  x(n)  time  sequence  as  samples  on  a  circle  as  shown  in  Figure  C-1(a).  There  we
arbitrarily  assign  positive  time  flow  as  counterclockwise  rotation.  (For  our  UK  friends,
counterclockwise means your anticlockwise.)
Figure C-1 Circular representations of periodic sequences: (a) original x(n) sequence; (b) circular
time reversal of x(n).
Time reversal, as defined here for sequences that are treated as periodic, means traveling clockwise
around the circle (in the negative time direction), creating a new time sequence
(C-2)
We call xc(n) the circular time reversal of x(n), where the subscript “c” means circular reversal, and
depict xc(n) as in Figure C-1(b).
The  interesting  issue  here  is  that  for  real  N-point  time  sequences,  the  DFT  of  xc(n)  is  the  complex
conjugate of the DFT of x(n). That is,
(C-3)
where the DFT index is 0 ≤ m ≤ N–1. Due to the conjugate symmetry of DFTs of real sequences, we
should realize that X*(m) is a straight reversal of the X(m) samples.
Let’s illustrate Eq. (C-3)  with  an  example.  With  X(m)  representing  the  DFT  of  x(n),  we  can  write
down X(m)’s m = 4 sample X(4) as
(C-4)
Because e–j2πk/6 has a period of 6, we can write Eq. (C-4) as
(C-5)
Next, let’s write down the (circular-reversed) Xc(m)’s m = 4-sample Xc(4) as
or
(C-6)
(C-7)
Replacing Xc(4)’s negative angles with their positive-angle equivalents yields
(C-8)
which is the conjugate of Eq. (C-5), demonstrating that X(m) and Xc(m) are complex conjugates.
An alternate time reversal concept, which we’ll call straight time reversal, is the simple reversal of
Eq. (C-1)’s x(n), yielding an xs(n) sequence
(C-9)
where the subscript “s” means straight reversal. For real N-point time sequences, the DFT of xs(n) is
(C-10)
We can demonstrate Eq. (C-10) the same way we did Eq. (C-3), but consider Figure C-2. There we
show the samples of repeated revolutions around the xc(n) circle in Figure C-1(b), indicating both the
6-point xs(n) and the 6-point xc(n) sequences. Notice how xs(n) is shifted backward in time by five
samples from xc(n).
Figure C-2 Periodic sequences xs(n) and xc(n).
Using the principle of the DFT’s shifting theorem from Section 3.6, we know that Xs(m) is equal to
Xc(m)  times  a  linear  phase  shift  of  e–j2πm(5)/6  for  our  N  =  6  example.  So,  in  the  general  N-point
sequence case,
(C-11)
which validates Eq. (C-10).
