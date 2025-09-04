# Audio Plugins C++ (Pirkle) — Chunk 31/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1756

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
204  Chapter 6
g0 5 V2
s 1
1
Qp
 Vs 1 1
a0 5 V2
s 1
"g1
Qz
 Vs 1 g1
a1  5 21 V2
s 2 g1
2
b1  5 21 V2
s 2 12
a2  5 V2
s 2
"g1
Qz
 Vs 1 g1
b2  5 V2
s 2
1
Qp
 Vs 1 1
(6.48)
a0   5
a1   5
a2   5
a0
g0
a1
g0
a2
g0
b1   5
b2   5
b1
g0
b2
g0
 Challenge: Modify your digital resonant LPF plug-in to add the Massberg ﬁ lter option, then
experiment with high-ﬁ delity music and listen for the differences in sound.
      Biblio graphy
 Allred, R. 2003. Second-order IIR ﬁ lters will support cascade implementations: 5 part digital audio application
tutorial.  EE Times Design Guide .  http://www.eetimes.com/design/audio-design/4009473/Second-order-IIR-
Filters-will-support-cascade-implementations—5-Part-digital-audio-application-tutorial. Accessed August 15,
2012.
 Berners, D. P. and Abel, J. S. 2003. Discrete-time shelf ﬁ lter design for analog modeling.  Journal of the Audio
Engineering Society , preprint 5939.
 Bohn, D. 1986. Constant-Q graphic equalizers.  Journal of the Audio Engineering Society  34(9): 611–25.
 Bohn, D. 2008. Bandwidth in octaves versus Q in band-pass ﬁ lters. Application Note 170. Mukilteo,
WA: Rane Corp.
 Dodge, C. and Jerse, T. 1997.  Computer Music Synthesis, Composition and Performance , Chapter 6. New York:
Schirmer.
 Giles, M., ed. 1980.  The Audio/Radio Handbook . Santa Clara, CA: National Semiconductor Corp.
 Kwakernaak, H. and Sivan, R. 1991.  Modern Signals and Systems , Chapter 9. Englewood Cliffs, NJ:
Prentice-Hall.
 Lane, J. et al. 2001.  DSP Filters , Chapters 4–10 and 20. Carmel, IN: Howard W. Sams & Co.
 Lane, J. and Hillman, G. 1991.  Implementing IIR/FIR Filters with Motorola’s DSP56000/DSP56001 . APR7/D
Rev1. Schomberg, ON: Motorola, Inc.
 Leach, M. 1999.  Introduction to Electroacoustics and Audio Ampliﬁ er Design , Chapter 6. Dubuque, IA:
Kendall-Hunt.
 Lindquist, C. 1977.  Active Network Design , Chapter 2. Miami: Steward & Sons.
 Lindquist, C. 1999.  Adaptive and Digital Signal Processing , Chapter 5. Miami: Steward & Sons.
 Massberg, M. 2011. Digital low-pass ﬁ lter design with analog-matched magnitude response.  Journal of the Audio
Engineering Society , preprint 8551 (Massberg 2011).
 Moore, R. 1990.  Elements of Computer Music , Chapter 2. Englewood Cliffs, NJ: Prentice-Hall.

Audio Filter Designs: IIR Filters  205
   Motorola, Inc. 1991.  Digital Stereo 10-Band Graphic Equalizer Using the DSP56001 . APR2/D. Schomberg, ON:
Motorola, Inc.
 Oppenheim, A. V. and Schafer, R. W. 1999.  Discrete-Time Signal Processing , 2nd ed., Chapter 7. Englewood
Cliffs, NJ: Prentice-Hall.
   Smith, J. O. and Angell, J. B. 1982. A constant gain digital resonator tuned by a single coefﬁ cient.  Computer
Music Journal  4(4): 36–40.
 Zöler, U. 2011.  Digital Audio Effects , 2nd ed., Chapter 2. West Sussex, U.K.: John Wiley & Sons.
References
Ifeachor, Emmanuel C. and Jervis, Barrie W. 1993. Digital Signal Processing: A Practical Approach. Menlo Park:
Addison-Wesley. pp. 398–400.
Orfanidis, Sophacles. 1997. Digital Parametric Equalizer Design with Prescribed Nyquist-Frequency Gain.
Journal of the Audio Engineering Society 45(6): 444–55.

CHAP TE R 7

Delay Effects and
Circular Buffers
    Before we can start looking at some ﬁ nite impulse response (FIR) algorithms, we need to
deal with the concept of long delay lines or circular buffers. Not only are they used for the
delay effects, but also they are needed to make long FIR ﬁ lters. In this chapter we’ll take a
break from the DSP ﬁ lter algorithms and develop some digital delays. If you think back to the
inﬁ nite impulse response (IIR) ﬁ lters you’ve worked on so far you will remember that after
implementing the difference equation, you need to shufﬂ e the  z  −1  delay element values. You
do this by overwriting the delays backwards, like this:
  m_f_z2 5 m_f_z1;
  m_f_z1 5  xn ;
   //  xn  is the input sample
 Suppose you had a ﬁ lter that was higher than a second-order one and you had to implement
 z  −4  using discrete variables for each delay element. You might wind up writing something like
this to do the shufﬂ ing:
  m_f_z4 5 m_f_z3;
  m_f_z3 5 m_f_z2;
  m_f_z2 5 m_f_z1;
  m_f_z1 5  xn ;
   //  xn  is the input sample
 But what happens when the delay line gets really long, like  z  −1024  or, for a 1-second digital
delay,  z  −44100 ? It’s going to be difﬁ cult to implement the delay shufﬂ ing this way. Not only
would it be tedious to code, it would also be very inefﬁ cient to have to implement all those
read/write operations each sample period. The answer to the problem of long delay lines is
called circular buffering.
 Digital signal processors (DSPs) have an interesting feature in the address generation unit
(AGU), which allows you to declare a buffer of data to be addressed circularly. This kind of
addressing is not built into the C++ programming language’s array access operation. We will
have to code the mechanism ourselves. When you declare a buffer, the addressing is linear.
207

208  Chapter 7
This means that the pointer will add or subtract the offset you provide and move linearly to
the next location. Suppose you declare a buffer of ﬂ oats like this:
  ﬂ oat Buffer[1024];
 and a pointer to the buffer:
    ﬂ oat* pBuffer = &Buffer[0];
 and then you enter a loop which accesses the data, incrementing by ﬁ ve addresses each time
through the loop:
    for(int n=0; n<someNumber; n+=5)
    {
             ﬂ oat data = pBuffer[n];
    }
 What happens if the pointer is accessed outside the buffer, as shown in  Figure 7.1 ? Usually
a crash or debug halt. You always have to be careful when setting up a loop like that one
to make sure the pointer never accesses outside the buffer. But what if you are stuck with
a certain sized buffer and a different sized loop so that you are never sure if the pointer is
going to go outside the buffer on the next iteration? In a DSP chip, when you declare a buffer
to be circular, you are setting up a pointer access mechanism. Reads and writes are made
with a pointer, which is incremented or decremented by some amount to generate the next
access location. If the amount of offset causes the pointer to go outside the bounds of the
buffer, it automatically wraps to the other side, including the amount of offset, as depicted
in  Figure 7.2 .
START  LOOP
Increment pointer by +5 samples
Buffer
Buffer
Buffer
Buffer
pBuffer[n]
pBuffer[n + 5]
pBuffer[n + 10]
pBuffer[n + 15]
???
 Figure 7.1:     After several accesses, the pointer goes outside the buffer into an unknown location,
usually resulting in a crash.
Delay Effects and Circular Buffers   209
START  LOOP
Increment  pointer by +5  samples
Buffer
Buffer
Buffer
Buffer
pBuffer[n]
pBuffer[n + 5]
pBuffer[n + 15]
pBuffer[n + 10]
 Figure 7.2:     In a circular buffer, the pointer is automatically wrapped back to the top and
offset by the proper amount to continue the access-by-ﬁ ve-samples loop.
 Circular buffers are useful in audio signal processing. You can create circular buffers of audio
samples or buffers of coefﬁ cients and loop through and access them automatically. Let’s
start with the most obvious use and make a digital delay effect.
  7.1    The Basic Digital Delay
 The digital delay effect or DDL (digital delay line) consists of a long buffer for storing
audio samples. The samples enter one end of the buffer and come out the other end after  D
samples of delay, which corresponds to  D  sample periods. A feedback path allows for delay
regeneration, or repeated echoes, as shown in  Figure 7.3 . With the feedback control  fb  set
to 0, there is only a single delayed sample. With any other value, repeated echoes will form
at the rate corresponding to the length of the delay line. The block diagram is shown in
 Figure 7.3 .
 The difference equation is as follows:
  y(n) 5 x(n2D) 1 fb * y(n2D)
(7.1)
x(n)
x(n)+(fb)*y(n)
Z –D
x(n– D)+(fb)*y(n–D)
y(n)
(fb)*y(n)
fb
 Figure 7.3:     Basic DDL.
210  Chapter 7
 From the difference equation in  Equation 7.1 , you can see that the output consists of an input
sample delayed by  D  samples plus a scaled version of the output at that time,  fb  *  y ( n 2 D ). The
sequence of accessing the delay line during the processAudioFrame() function is as follows:
1.
2.
3.
   Read the output value, y(n), from the DDL and write it to the output buffer.
   Form the product  fb  *  y ( n ).
   Write the input value, x(n) 1 fb * y(n), into the delay line.
 You might notice that  s o m ethi ng   i s missing here: the shufﬂ ing of the samples through the
delay. If we use a circular buffer as the delay line, then we don’t have to shufﬂ e data around,
but we do have to keep track of the read and write access locations in the buffer and wrap the
pointers as needed. In order to understand how the buffer operates to make a delay, consider
a circular buffer that we’ve been writing samples into each sample period and automatically
wrapping the pointer (or index) back to the top of the buffer as needed. The code would look
something like this:
  // buffer size, 1 second of audio data
  int nBufferLength = 44100;
  ﬂ oat Buffer[nBufferLength];
  .
  .
  .
  . // somewhere else in code:
  int nIndex = 0;
  .
  .
  // inside a loop of some kind:
    pBuffer[nIndex] = audioSample;
    nIndex++;
    // if we go outside the buffer
    if(nIndex >= nBufferLength) // if we hit nBufferLength, we are one sample outside
             nIndex = 0;      // wrap the pointer back to the top for the next iteration
    .
    .
 Suppose this has been going on for some time, and we are left with the buffer looking
like  Figure 7.4   after  the last write access and just before we increment the pointer index.
If pBuffer is pointing to the current sample value  x ( n )
•
•
   Where is the  x ( n 21) sample (the youngest delayed value)?
   Where is the oldest sample in the delay?
 In  Figure 7.5  the youngest sample,  x ( n 21), is in the location just before pBuffer[i], that is
pBuffer[i21]. The oldest sample is found by working backwards to the top of the buffer,
