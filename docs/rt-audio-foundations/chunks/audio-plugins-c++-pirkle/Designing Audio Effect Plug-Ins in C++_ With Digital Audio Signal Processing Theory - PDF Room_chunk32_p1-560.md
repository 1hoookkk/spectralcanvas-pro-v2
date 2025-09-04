# Audio Plugins C++ (Pirkle) — Chunk 32/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2732

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Delay Effects and Circular Buffers   211
Buffer
pBuffer[i]
x(n)
 Figure 7.4:     After many loops through the buffer, pBuffer[i] points to  x ( n ).
Buffer
etc...
x(n –2)
x(n –1)
x(n)
pBuffer[i]
Youngest delayed  sample
x(n –D  +1)
Oldest delayed  sample
 Figure 7.5:     The youngest and oldest samples in the delay line.
wrapping back to the bottom, and locating the oldest sample written; it is at pBuffer[i11].
If the pointer is accessing the samples sequentially from top to bottom, then the youngest
sample is just above  x ( n ) and the oldest is just below it. It is easy to understand that the
youngest sample is  x ( n 21) but why is the oldest sample labeled  x ( n2  D 11) rather than
 x ( n 2 D )?
 The answer to the question is that we overwrote the actual oldest sample,  x ( n 2 D ), when we
wrote in  x ( n ). This is one of the reasons for our rule about always performing reads before
writes in our algorithms. This means that to get the oldest sample in the delay, you must ﬁ rst
read out pBuffer[i] before writing into it. In other words,  before  the write access, the buffer
looks like  Figure 7.6 ; you can see the oldest value  x ( n 2 D ) is actually the location of the
current write operation.
 The steps for creating the delay line in your plug-in are as follows:
1.
2.
   Decide on the maximum amount of delay you need to provide.
   Declare read and write index values.
212  Chapter 7
Buffer
etc...
x(n –2)
x(n –1)
x(n–D)
x(n–D+1)
pBuffer[i]
Youngest delayed  sample
Oldest delayed  sample
 Figure 7.6:     The location of the oldest audio sample  x ( n  –  D ).
3.
4.
   Declare a ﬂ oat buffer for each channel, right and left: for very long delay lines this is
traditionally done with the  new  operator in the constructor of the plug-in.
   Initialize the buffers with 0.0 using the memset() function.
 During the processAudioFrame() function, you will need to
•
•
•
   Use the read and write index values to operate the delay.
   Increment or decrement the indices according to the algorithm you are using.
   Check to see if you need to wrap the index values for the next sample period.
 In order to use the delay line, the user needs to provide a value for the delay amount in samples
(in Section 7.3  we will allow them to enter the delay in milliseconds instead, but we will convert
this to a sample count inside the delay). In your code, there are two basic ways to do this:
1.
2.
   Subtract the delay amount (in samples) from your write index, wrapping the pointer
 backwards if needed, to locate the delayed sample.
   Store a read index and offset the two indices when the user changes the delay amount.
 The second option is better because you only have to offset the index values and perform
the subtraction/wrap when the user changes the delay value, instead of each time through
the processAudioFrame() loop. Suppose the user selects 100 samples of delay time.
You have declared two indices, m_nRead and m_nWrite, to use for buffer. During the
processAudioFrame() function you will need to do the following ﬁ ve steps.
 Step 1: Read out the delayed audio data,  d ( n2  D 2100), which is 100 samples behind the
current  x ( n ) sample time; this value is  y ( n ), the current output value ( Figure 7.7 ).
  ﬂ oat yn = pBuffer[m_nRead];
 Step 2: Form the input combination input + feedback * output:
  ﬂ oat xn = pInputBuffer[0] + m_fFeedBack*yn;
Delay Effects and Circular Buffers   213
 m_fFeedBack is declared in your .h ﬁ le; this example code is for processing the left channel,
pInputBuffer[0].
 Step 3: Write the input data into the delay line at the m_nWrite location ( Figure 7.8 ).
 Step 4: Increment the read and write pointers by one to set up for the next time through the
function; check for wrapping and do that if necessary. Can you think of other ways to do this?
  // inc/wrap
  m_nWrite++;
  if(m_nWrite >= m_ nBufferLength)
         m_nWrite = 0;
  m_nRead++;
  if(m_nRead >= m_ nBufferLength)
         m_nRead = 0;
Buffer
pBuffer[m_nRead]
d(n – D – 100)
100 sample offset
pBuffer[m_nWrite]
m_nRead = m_nWrite –100
 Figure 7.7:     The delayed sample is read at location  d ( n – D –100), 100 samples before
the current write location.
Buffer
pBuffer[m_nRead]
d(n–D–100)
100 sample offset
x(n) + fb*y(n)
pBuffer[m_nWrite]
 Figure 7.8:     The delayed sample plus feedback is written into the current write location.
214  Chapter 7
 Notice that we wrap if the incremented index hits nBufferLength because this references the
location just one sample outside the delay line.
 Step 5: In the event that the user changes the delay time, you need to recalculate the m_nRead
index to accommodate it. Note that once set up, the m_nWrite index is never changed except
to increment it through the line. In this example, the user has selected nSamples of delay in
the plug-in.
  // user selects nSample delay
  // ﬁ rst subtract the index
  m_nRead = m_nWrite - nSamples;
   //  the check and wrap BACKWARDS if the index is negative
   if (m_nRead < 0)
        m_nRead += nBufferLength;  // amount of wrap is Read + Length
   7.2    Digital Delay with Wet/Dry Mix
 Although there may be some instances where you don’t need a wet/dry mix, in general when
you make a delay plug-in, you still want to hear your unaffected signal. This can be done
by using the plug-in as an Auxiliary (AUX) Send effect rather than an insert effect. But, to
provide both capabilities, you need to provide the user with a wet/dry ratio control. The block
diagram is slightly modiﬁ ed as shown in  Figure 7.9 .
 The difference equation is as follows:
  y(n) 5 dry * x(n) 1 wet * 3x(n2D) 1 fb * y(n2D)4
(7.2)
  7.2.1 Frequency and Impulse Responses
 Consider the basic delay with  no  feedback applied and with the wet/dry ratio at 1:1. The
block diagram and difference equation would then reduce down to  Figure 7.10 .
 The difference equation is as follows:
  y(n) 5 x(n) 1 x(n2D)
(7.3)
 To ﬁ nd the frequency response, ﬁ rst take the  z  transform of the difference equation and form
the transfer function:
y(n) 5 x(n) 1 x(n2D)
Y(z) 5 X(z) 1 X(z)z2D
        5  X(z)(11z2D)
Y(z)
X(z)
5 11z2D
H(z) 5
(7.4)

Delay Effects and Circular Buffers   215
Dry
Wet
y(n)
x(n)
Z –D
fb
 Figure 7.9:     A more useful digital delay incorporates wet and dry controls.
x(n)
Z –D
1
1
y(n)
 Figure 7.10:     The simpliﬁ ed DDL of  D  samples delay.
 Next, calculate the poles and zeros of the transfer function. We can see that this is a pure
feed-forward ﬁ lter in its current state so there are only zeros. We need to ﬁ nd the roots of the
equation for  H ( z ):
0 5 1 1 z2D
     5 zD 1 1
  zD 5 21
Let z 5 ejv
  ejDv 5 21
  Apply Euler
  cos(Dv) 1 j sin(Dv) 5 21
(7.5)
 The roots (zeros) of the function lie wherever  D v causes the equation to evaluate to 21 1  j 0,
and we know this will happen when  Equation 7.6  holds true.
cos(Q) 1 j sin(Q) 5 211 j0
if
Q 5 ;p,  ; 3p, ; 5p, etc...
(7.6)

216  Chapter 7
 Notice that both 6p produce the desired result of 21 1  j 0 as well as all the odd
multiples of p: 63p, 65p, and so on. So the actual solution to ﬁ nd the roots becomes
 Equation 7.7 :
cos(Dv) 1 j sin(Dv) 5 21 1 j0
if
Dv 5 ; p,  ; 3p,  ; 5p, ...,  ; Np
until
N . D21
or
zeros at v 5 ; kp
D
   k 5 1, 3, 5, ..., D
 (7.7)
 After  N  .  D  21, the whole mathematical sequence repeats again, cycling through odd
multiples of p. This means that there are  D  zeros spread equally around the unit circle. This
makes sense—the fundamental theorem of algebra predicts  D  roots for a polynomial of
order  D . Now consider the simple case of  D  5 2 samples; we get  Equation 7.8 :
cos(2v) 1 j sin(2v) 5 21
if
v 5 ; kp
D
v 5 ; p
2
  k 5 1, 3, 5, ..., D
(7.8)
 There are two zeros, one at 1p/2 and the other at 2p/2. Plot those on the unit circle in the
 z -plane and you can see what the frequency response will be, shown in  Figure 7.11 . You
can see from  Figure 7.11  that the zeros produce a notch (zero of transmission) at 6p/2. In
fact, when the delay time is very small, your ears hear the effect as a frequency response
change; your ears cannot discriminate two samples that are only 23 uS apart as separate
echoes. Now consider what would happen if we increase the delay amount to four samples,
as in  Figure 7.12 . Finally, what would happen if the delay is an odd value, like  D  5 5
( Figure 7.13 )?
cos(Dv) 1 j sin(Dv) 5 21 1 j0
cos(4v) 1 j sin(4v) 5 21 1 j0
v 5 ; pk
D
,  ; 3p
v 5 ; p
4
4
   k 5 1, 3, 5, ..., D
(7.9)

Delay Effects and Circular Buffers   217
Im
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
Re
2kHz
4kHz
6kHz
8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.11:     The  z -plane pole/zero plot and resulting frequency response.
Im
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
Re
2kHz
4kHz
6kHz
8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.12:     The  z -plane pole/zero plot and resulting frequency response for  D  = 4 samples.
Im
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
Re
2kHz
4kHz
6kHz
8kHz
10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.13:     The  z -plane pole/zero plot and resulting frequency response for  D  = 5 samples.
    k 5 1, 3, 5, ..., D21
cos(Dv) 1 j sin(Dv) 5 21 1 j0
cos(5v) 1 j sin(5v) 5 21 1 j0
v 5 ; kp
5
,  ; 3p
v 5 ; p
5
5
,  ; 3p
v 5 ; p
5
5
,  ; 5p
5
,  ; p
(7.10)
 This kind of frequency response in  Figure 7.13  is called  inverse comb ﬁ ltering . As we add
more and more samples of delay, we add more and more notches to the response. You can use
