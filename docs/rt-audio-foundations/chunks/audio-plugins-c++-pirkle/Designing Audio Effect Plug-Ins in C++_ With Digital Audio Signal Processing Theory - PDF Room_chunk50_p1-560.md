# Audio Plugins C++ (Pirkle) — Chunk 50/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2785

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Reverb Algorithms
    Reverb algorithms might represent the Holy Grail of audio signal processing. They have an
appeal that seems universal, perhaps because we live in a reverberant world. Our ears are
time-integrating devices that use time domain cues and transients for information, so we are
sensitive to anything that manipulates these cues. In this chapter, we discuss reverb algorithms
as applied mainly to room simulation. There are two general ways to create the reverberation
effect:
(cid:129)
(cid:129)
   Reverb by direct convolution—the physical approach.
   Reverb by simulation—the perceptual approach.
 In the physical approach, the impulse response of a room is convolved with the input signal
in a large ﬁ nite impulse response (FIR) ﬁ lter. For large rooms, these impulse responses
might be 5 to 10 seconds. In the mid 1990s, Gardner developed a hybrid system for fast
convolution that combined direct convolution with block fast Fourier transform (FFT)
processing (Gardner 1995). Around the same time, Reilly and McGrath (1995)  described
a new commercially available system that could process 262,144-tap FIR ﬁ lters for
convolving impulses over 5 seconds in length. The required processing power is sometimes
too large to be practical in a plug-in. Aside from the computing expense, another drawback
to this approach is that an impulse response is frozen in time, and measures the room at one
location only, under a certain set of conditions. To create a reverb unit (or  reverberator )
that is general enough to provide many different reverbs of different spaces would require
a large library of impulse response ﬁ les. In addition, its parameters can’t be adjusted in
real time.
 The perceptual approach aims to simulate the reverberation with enough quality to fool the
ears and give the same perception of real reverb but with much less processing power. The
advantages are numerous, from the minimal processing required to the ability to vary many
parameters in real time. Browne (2001) proposed a hybrid system combining a short impulse
response convolution along with recursive ﬁ ltering, combining both approaches. There are
several key engineers who developed much of the theory still in use today. These include
Schroeder’s (1962) initial work in the early 1960s with continued research and contributions
from Moorer, Griesinger, Gerzon, Gardner, Jot, Chaigne, Smith, Roscchesso, and others
357
358  Chapter 11
across the decades. Most of this chapter is owed to their work in the ﬁ eld. We will focus on
the perceptual approach and try to ﬁ nd computationally efﬁ cient algorithms for interesting
reverberation effects.
 Griesinger (1989) states that it is impossible to perfectly emulate the natural reverberation of
a real room and thus the algorithms will always be approximations. It seems that the area of
reverberation design has the most empirically derived or trial-and-error research of just about
any audio signal processing ﬁ eld. There is no single “correct” way to implement a reverb
algorithm, so this chapter focuses on giving you many different reverberator modules to
experiment with.
  11.1    Anatomy of a Room Impulse Response
 The ﬁ rst place to start is by examining impulse responses of actual rooms. There are several
popular methods for capturing the impulse response, from cap pistols and balloons to
deconvolution of chirp signals and pseudo random sequences. The resulting time domain plot
is useful for investigating the properties of reverberation.
  Figure 11.1  shows the impulse response plots for two very different spaces; a large concert
hall and a cathedral. The initial impulse is followed by a brief delay called the  pre-delay . As
the impulse pressure wave expands, it comes into contact with the nearby structures—walls,
ﬂ oor, and ceiling—and the ﬁ rst echoes appear. These initial echoes, called  early reﬂ ections ,
are important to the simulation of reverb because of the auditory cues we get from them. The
pressure wave continues to expand and more reﬂ ections occur, with reﬂ ected signal upon
reﬂ ected signal piling on top of each other while decaying in energy. The resulting reverb
“tail” is called  late reverberation  or  subsequent reverberation .
 The top impulse response in  Figure 11.1  is a modern concert hall designed for a pleasing
reverberant quality. The initial reﬂ ections are from irregularly shaped back and side walls
and they pile up in amplitude; they are also piling up in density, shown as the impulse
gets “fatter” in the middle section. The dense reverberant tail follows, which decays in an
exponential fashion, and the sound pressure energy is absorbed by the seats and acoustical
room treatments. The cathedral is a different story. After a short pre-delay, a few large early
reﬂ ections arrive from the nearby structures but they don’t increase in amplitude or density
in the same way as the hall. The reverb’s decay is also much longer in time as there is little in
the cathedral to absorb the sound pressure.
 The block diagram in  Figure 11.2  shows the three components we observe. However, there
is debate as to whether or not this is a good way to break the problem down. Some feel
that the reverb algorithm should not need to separate out the early reﬂ ections from the late
reverberation. In other words, a good algorithm will create all of the reverberation aspects
at once.
Reverb Algorithms  359
Large  Concert  Hall
0
3087
6174
9261
12348
15435
18522
21609
24696
27783
Cathedral
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
0
3087
6174
9261
12348
15435
18522
21609
24696
27783
Early  Reflections
Late  Reverberation
 Figure 11.1:    The impulse responses of a large hall and cathedral.
Dry
ER
∑
Output
Input
Pre-Delay
Early  Reflections
Late  Reverberation
Wet
 Figure 11.2:    A generalized model of a reverb algorithm.
11.1.1    RT 60 : The Reverb Time
 The most common measurement for reverb is the  RT  60  reverb time. Reverb time is measured
by ﬁ rst stimulating the room into a reverberant state, then turning off the source and plotting
the pressure-squared level as a function of time. The amount of time it takes for this energy
decay curve to drop by 60 dB is the  reverb time , or  RT  60 . Sabine’s (1973) pioneering work in
this area leads to the following formula in  Equation 11.1 :
  RT60 5 0.5
VR
SRARAve
(11.1)
where
VR 5 volume of room in ft3
SR 5 surface area of room in ft2
ARAve 5 average absorption coefficient

360  Chapter 11
 Sabine measured and tabulated the absorption coefﬁ cients for various materials. The units
are given in  Sabines . A room made of several materials is ﬁ rst partitioned to ﬁ nd the partial
surface area of each material type, then the average is found by weighting the areas with the
absorption coefﬁ cients and summed. The reverb time is probably the most common control,
found on just about every kind of reverb plug-in.
    11.2    Echoes and Modes
 In Schroeder’s early work, he postulates that a natural sounding artiﬁ cial reverberator has
both a large echo density and a colorless frequency response. The echo density is simply the
number of echoes per second that a listener experiences in a given position in the reverberant
environment ( Equation 11.2 ).
  ED 5
echoes
second
(11.2)
 If the echo density is too low the ear discerns the individual echoes and a ﬂ uttering sound is
the result. As the echo density increases, the echoes fuse together, producing a wash of sound.
Schroeder (1962) postulated that for a natural-sounding reverb, the echo density needs to
be 1000 echoes/second. Greisinger (1989)  increases this to 10,000 echoes/second or more.
Statistically, the echo density is shown in  Equation 11.3 , which reveals that the echoes build
up as the square of time.
  Echo density ~
4pc3
VR
2
 t
(11.3)
where
c 5 speed of sound
t 5 time
VR 5 volume of room
 Reverb is often modeled statistically as decaying white noise, which implies that ideal
reverberant rooms have ﬂ at frequency responses. A room’s geometry can be used to predict
its frequency response. The dimensions of the room can be used to calculate the frequencies
that will naturally resonate as the wave bounces back and forth between parallel surfaces. An
untreated rectangular room will have multiple resonances and anti-resonances.
 Given a rectangular room with length, width, and height of  l ,  w , and  h , the resonances are
found in  Equation 11.4 , a well-known and useful equation for predicting room resonances,
also called  modes  (Beranek 1986). As the frequency increases, so do the number of
resonances.

Reverb Algorithms  361
fe 5
c
2Å
a
nx
l
2
b
1 a
2
b
ny
w
1 a
2
b
nz
h
where
nx, ny, nz 5 half wave numbers 0, 1, 2, 3...
l, w, h  5  length 1 x2 , width 1 y2  and height 1 z 2  of the room
(11.4)
 Above a certain frequency the resonances overlap and fuse together. Each resonant frequency
has its own envelope whose curve is bell (band-pass ﬁ lter [BPF]) shaped, meaning that it has
a peak resonance at the mode frequency, but is still excitable at frequencies around it and
thus the quality factor ( Q ) of the curve relates to this excitability. The number of resonances
increases with frequency.  Figure 11.3  shows a ﬁ ctitious room example with the bell-shaped
resonances.
 The  modal density  is the number of resonant peaks per Hz. Physicists call the resonant
frequencies or modes  eigenfrequencies  (note this is not an acoustics-speciﬁ c term; an
eigenfrequency is the resonant frequency of any system). Schroeder’s second postulation is that
for a colorless frequency response, the modal density should be 0.15 eigenfrequencies/Hz or
one eigenfrequency every 6.67 Hz or approximately 3000 resonances spread across the audio
spectrum. Thus, it makes sense that good reverberant environments have interesting geometries
with many nonparallel walls. The multitude of resonances is created by the many paths an
impulse can take from the source to the listener. Kuttruff (1991) derived the approximation for
Amplitude
(dB)
0.0
–3.0
–inf
Frequency
 Figure 11.3:    The resonances of a room all contribute to create the
ﬁ nal frequency response. When the bandwidths of the modes overlap,
they fuse together. The dotted line represents the combined frequency response.

362  Chapter 11
 Schroeder’s rules for natural reverb:
 (cid:129)
 (cid:129)
   Echo density: At least 1000 echoes/sec (Greisinger: 10,000 echoes/sec)
   Modal density: At least 0.15 eigenfrequencies/Hz
 In physical rooms we know that:
 (cid:129)
 (cid:129)
   Echo density increases with the square of time.
   Modal density increases with the square of frequency.
the modal density as it relates to the volume of the room and modal frequency.  Equation 11.5
shows that the resonances build up as the square of frequency.
where
Dm 5
4pVR
c3
f 2
m
VR  5 volume of the room
 fm  5 modal frequency in question
(11.5)
 The  energy decay relief  plot (or EDR) shows how the energy decays over both frequency and
time for a given impulse response of a room.  Figure 11.4  shows a very simple ﬁ ctitious EDR.
 In  Figure 11.4 , notice that the frequency axis comes out of the page; low frequencies are in
the back. It also shows that this room has a resonant peak, which forms almost right away
Amplitude
0.0 dB
–20 dB
–40 dB
–60 dB
–80 dB
–100  dB
–120  dB
0.0
A  resonant frequency
DC
Frequency
Time
20 kHz
2.0 sec
 Figure 11.4:    The EDR shows time ( x -axis), frequency ( z -axis) and
amplitude ( y -axis) of the energy decay of a room.

Reverb Algorithms  363
in time. In an EDR, the modal density is shown across the  z -axis (frequency) while the echo
density appears across the  x -axis (time). This simpliﬁ ed plot shows just one resonance and no
echo density build-up for ease of viewing.  Figure 11.5  shows the EDR of an actual room, in
this case a theater. The eigenfrequencies are visible as the ridges that run perpendicularly to
the frequency axis.
  Figure 11.6  shows the EDR for a cathedral. In this case, the echo pile-ups are clearly visible
running perpendicular to the time axis. Comparing both EDRs shows that both rooms have
high echo and modal densities; therefore, they should be good reverberant spaces. Both
EDRs show an initial high frequency roll-off and, especially in the theater’s case, the high
frequencies decay faster than the low frequencies. The high-frequency decay is a property of
the treatment of the room surfaces along with the fact that the air absorbs high frequencies
more easily than low frequencies. The high-frequency energy decay in the theater is caused
by the carpet, seats, and acoustic treatment which the cathedral lacks. Therefore, for a good
quality reverb, we will need to take this high-frequency decay into account along with the
echo and modal density.
 Reverb algorithms are typically made of arrangement of modules called  reverberator
modules . From our basic observations we can tell that good reverberator modules are going
to produce dense echoes along with a large number of resonances. If the resonances are
distributed properly across the spectrum, the reverb will sound ﬂ at. If they are not, there
will be metallic pinging and other annoyances that will color the frequency response in an
–10
–20
–30
–40
–50
–60
–70
–80
–90
)
B
d
(
e
d
u
t
i
n
g
a
M
–100
0.1
0.2
0.3
0.4
0.5
0.6
0.7
Time (sec)
0.8
0.9
104
103
Freq (Hz)
 Figure 11.5:    An EDR for a theater.
2
10

364  Chapter 11
)
B
d
(
e
d
u
t
i
n
g
a
M
-20
-40
-60
-80
-100
-120
-140
1
2
3
4
Time (sec)
5
6
4
10
3
10
Freq (Hz)
 Figure 11.6:    An EDR for a cathedral.
2
10
unnatural way. The majority of the rest of the chapter is devoted to revealing, analyzing, and
explaining these building blocks and the algorithms that use them. The best place to start is
with Schroeder’s reverb modules.
   11.3    The Comb Filter Reverberator
 One of the reverberator modules that Schroeder proposed is a comb ﬁ lter. Remember that we
are looking for modules that produce echoes and the comb ﬁ lter with feedback will do just
that. The comb ﬁ lter reverberator is familiar looking because it’s also the delay with feedback
algorithm you studied in  Chapter 7 . We derived the difference equation and transfer function
for the comb ﬁ lter in  Figure 11.7  in  Chapter 7 :
y(n) 5 x(n2D) 1 gy(n2D)
and
H(z) 5
z2D
12gz2D
(11.6)
 We also performed a pole-zero analysis and generated frequency plots. We showed that the
feedback path caused a series of poles evenly spaced around the inside of the unit circle. The
resulting impulse response is easy to predict as the echo recirculates through the feedback
path, being attenuated by multiplying by  g  each time through the loop.

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
