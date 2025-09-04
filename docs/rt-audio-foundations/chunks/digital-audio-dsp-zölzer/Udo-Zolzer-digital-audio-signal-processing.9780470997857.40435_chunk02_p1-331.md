# Digital Audio DSP (Zölzer) — Chunk 2/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 9005

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Digital Audio Signal Processing
Second Edition
Udo Zölzer
Helmut Schmidt University, Hamburg, Germany
A  John Wiley & Sons, Ltd, Publication
This edition ﬁrst published 2008
© 2008 John Wiley & Sons Ltd
First edition published under the title Digitale Audiosignalverarbeitung © B. G. Teubner
Verlag, Stuttgart, 1995. Digital Audio Signal Processing was then published in 1997 by
John Wiley & Sons Ltd.
Registered ofﬁce
John Wiley & Sons Ltd, The Atrium, Southern Gate, Chichester, West Sussex, PO19 8SQ,
United Kingdom
For details of our global editorial ofﬁces, for customer services and for information about
how to apply for permission to reuse the copyright material in this book please see our
website at www.wiley.com.
The right of the author to be identiﬁed as the author of this work has been asserted in
accordance with the Copyright, Designs and Patents Act 1988.
All rights reserved. No part of this publication may be reproduced, stored in a retrieval
system, or transmitted, in any form or by any means, electronic, mechanical,
photocopying, recording or otherwise, except as permitted by the UK Copyright, Designs
and Patents Act 1988, without the prior permission of the publisher.
Wiley also publishes its books in a variety of electronic formats. Some content that
appears in print may not be available in electronic books.
Designations used by companies to distinguish their products are often claimed as
trademarks. All brand names and product names used in this book are trade names, service
marks, trademarks or registered trademarks of their respective owners. The publisher is
not associated with any product or vendor mentioned in this book. This publication is
designed to provide accurate and authoritative information in regard to the subject matter
covered. It is sold on the understanding that the publisher is not engaged in rendering
professional services. If professional advice or other expert assistance is required, the
services of a competent professional should be sought.
Library of Congress Cataloging-in-Publication Data
Zölzer, Udo.
Digital Audio Signal Processing / Udo Zölzer. 2nd ed.
p. cm.
Includes bibliographical reference and index.
ISBN 978-0-470-99785-7 (cloth)
1. Sound–Recording and reproducing–Digital techniques. 2. Signal processing–Digital
techniques. I. Title.
TK7881.4.Z65 2008
621.382’2–dc22
A catalogue record for this book is available from the British Library.
ISBN 978-0-470-99785-7 (HB)
Set in 10/12pt Times by Sunrise Setting Ltd, Torquay, England.
Printed in Great Britain by Antony Rowe Ltd, Chippenham, England.
2008006095
Contents
Preface to the Second Edition
Preface to the First Edition
1
Introduction
. . . .
1.1 Studio Technology .
. .
1.2 Digital Transmission Systems
1.3 Storage Media . . . .
. . . .
1.4 Audio Components at Home . . .
. . . .
References .
. . . .
. . . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
2 Quantization
. . .
. . . .
2.2 Dither . . . .
2.1 Signal Quantization .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
2.2.1 Basics . . . .
2.2.2
2.2.3 Examples . .
2.1.1 Classical Quantization Model
2.1.2 Quantization Theorem . .
2.1.3
. . . .
. . . .
. . . .
. .
. . . .
. . . .
. . . .
Statistics of Quantization Error . .
. . . .
. . . .
. . .
. . . .
. . . .
. . .
. . . .
. . . .
Implementation . .
. . . .
. . . .
. . .
2.3 Spectrum Shaping of Quantization – Noise Shaping . . .
. . . .
2.4 Number Representation . .
. . . .
. . . .
Fixed-point Number Representation . . .
2.4.1
2.4.2
. . . .
Floating-point Number Representation . .
2.4.3 Effects on Format Conversion and Algorithms . .
Java Applet – Quantization, Dither, and Noise Shaping .
. . . .
. . . .
2.5
2.6 Exercises
References .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. .
. . . .
. . .
. . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
3 AD/DA Conversion
3.1 Methods . . .
. . .
. . . .
. . . .
3.1.1 Nyquist Sampling . . . .
3.1.2 Oversampling . . .
. . . .
3.1.3 Delta-sigma Modulation .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
ix
xi
1
1
. . .
. . .
2
. . . 10
. . . 13
. . . 17
21
. . . 21
. . . 21
. . . 24
. . . 30
. . . 36
. . . 36
. . . 39
. . . 39
. . . 42
. . . 47
. . . 47
. . . 53
. . . 56
. . . 58
. . . 59
. . . 61
63
. . . 63
. . . 63
. . . 65
. . . 66
vi
Contents
3.3 DA Converters
3.2 AD Converters
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . . .
. . .
Speciﬁcations .
3.2.1
Parallel Converter . .
. . . .
3.2.2
Successive Approximation .
3.2.3
3.2.4 Counter Methods . .
. . . .
3.2.5 Delta-sigma AD Converter .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
Speciﬁcations .
. . . .
3.3.1
Switched Voltage and Current Sources . . .
3.3.2
. . . .
3.3.3 Weighted Resistors and Capacitors .
. . . .
. . . .
3.3.4 R-2R Resistor Networks . .
3.3.5 Delta-sigma DA Converter .
. . . .
. . . .
Java Applet – Oversampling and Quantization . . .
. . . .
. . .
. . . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . . .
. . .
. . .
3.4
3.5 Exercises
References . .
4 Audio Processing Systems
4.1.1
4.1.2
. . . .
. . . .
. . . .
. . . .
4.2 Digital Audio Interfaces
4.1 Digital Signal Processors . .
Fixed-point DSPs . .
Floating-point DSPs
. .
. . . .
. . . .
. . . .
. . . .
4.2.1 Two-channel AES/EBU Interface .
. . . .
4.2.2 MADI Interface . . .
. . . .
4.3 Single-processor Systems . .
. . . .
. . .
. . . .
. . .
. . . .
4.4 Multi-processor Systems . .
. . . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
4.4.1 Connection via Serial Links
4.4.2 Connection via Parallel Links
. . . .
4.4.3 Connection via Standard Bus Systems . . .
. . . .
4.4.4
. . . .
References . .
Scalable Audio System . . .
. . . .
4.3.1
4.3.2 Control
Peripherals . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
5 Equalizers
. . . .
. . . .
5.3 Nonrecursive Audio Filters .
. . . .
. . .
5.1 Basics .
. . . .
5.2 Recursive Audio Filters . . .
. . . .
5.2.1 Design . . . .
. . .
5.2.2
Parametric Filter Structures
5.2.3 Quantization Effects . . . .
. . . .
5.3.1 Basics of Fast Convolution .
5.3.2
5.3.3
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
Fast Convolution of Long Sequences
. . .
Filter Design by Frequency Sampling . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
5.4.1
. . . .
5.4.2 Example: Eight-band Multi-complementary Filter Bank . . . .
5.4 Multi-complementary Filter Bank .
. . . .
Principles . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . .
. . 79
. . 79
. . 82
. . 83
. . 84
. . 85
. . 86
. . 86
. . 89
. . 89
. . 92
. . 92
. . 92
. . 94
. . 95
97
. . 98
. . 98
. . 100
. . 101
. . 101
. . 102
. . 107
. . 107
. . 107
. . 109
. . 110
. . 111
. . 112
. . 113
. . 113
115
. . 115
. . 119
. . 119
. . 128
. . 138
. . 157
. . 158
. . 161
. . 167
. . 168
. . 168
. . 175
Contents
vii
5.5
5.6 Exercises
References .
Java Applet – Audio Filters . . . .
. . . .
. . . .
. . . .
. . . .
. .
. . . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . 180
. . . 181
. . . 185
6 Room Simulation
. . .
. . . .
. . .
.
. . . .
. . . .
. . . .
. . . .
6.1 Basics . . . .
6.2 Early Reﬂections . .
. . . .
6.1.1 Room Acoustics
. . . .
6.1.2 Model-based Room Impulse Responses .
6.1.3 Measurement of Room Impulse Responses
Simulation of Room Impulse Responses .
6.1.4
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
6.4 Approximation of Room Impulse Responses . . .
. . . .
6.5
. . . .
6.6 Exercises
. . . .
References .
. . . .
. . . .
. . . .
6.2.1 Ando’s Investigations . . .
. . . .
6.2.2 Gerzon Algorithm . . . .
. . . .
6.3 Subsequent Reverberation . . . .
6.3.1
. . . .
Schroeder Algorithm . . .
6.3.2 General Feedback Systems . . . .
Feedback All-pass Systems . . . .
6.3.3
Java Applet – Fast Convolution . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. .
. . . .
. . .
. . .
7 Dynamic Range Control
7.1 Basics . . . .
7.2 Static Curve .
7.3 Dynamic Behavior
7.4
. . .
. . .
. . .
. . . .
. . . .
.
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
7.3.1 Level Measurement . . . .
. . . .
7.3.2 Gain Factor Smoothing . .
. . . .
. . . .
7.3.3 Time Constants . .
. . . .
. . . .
. . .
Implementation . . .
. . . .
. . . .
. . .
. . .
7.4.1 Limiter
7.4.2 Compressor, Expander, Noise Gate
. . . .
7.4.3 Combination System . . .
. . . .
. . . .
. . .
Sampling Rate Reduction . . . .
7.5.1
. . . .
7.5.2 Curve Approximation . .
7.5.3
. . . .
. . . .
Stereo Processing .
Java Applet – Dynamic Range Control . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. .
. . . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
7.6
7.7 Exercises
References .
7.5 Realization Aspects .
. . . .
. . . .
. . . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
8 Sampling Rate Conversion
. . . .
8.1 Basics . . . .
. . . .
. . . .
. . .
8.1.1 Upsampling and Anti-imaging Filtering .
. . . .
8.1.2 Downsampling and Anti-aliasing Filtering . . . .
. . . .
8.2 Synchronous Conversion .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
191
. . . 191
. . . 191
. . . 192
. . . 193
. . . 194
. . . 195
. . . 195
. . . 195
. . . 200
. . . 200
. . . 208
. . . 212
. . . 213
. . . 217
. . . 217
. . . 219
225
. . . 225
. . . 226
. . . 228
. . . 228
. . . 230
. . . 230
. . . 231
. . . 231
. . . 231
. . . 233
. . . 234
. . . 234
. . . 236
. . . 237
. . . 237
. . . 238
. . . 239
241
. . . 241
. . . 241
. . . 242
. . . 244
viii
Contents
8.4
. . . .
. . . .
. . . .
8.3 Asynchronous Conversion .
. . . .
Single-stage Methods . . . .
. . . .
. . . .
. . . .
8.3.1
8.3.2 Multistage Methods
. . . .
8.3.3 Control of Interpolation Filters . . . . . . .
. . . .
. . . .
Interpolation Methods .
. . .
. . . .
Polynomial Interpolation . .
8.4.1
. . . .
8.4.2 Lagrange Interpolation . . .
. . . .
. . . .
Spline Interpolation .
8.4.3
. . . .
. . . .
. . .
8.5 Exercises
. . . .
. . . .
. . .
References . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . . .
9 Audio Coding
9.4
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
9.1 Lossless Audio Coding . . .
. . . .
. . .
9.2 Lossy Audio Coding .
. . . .
. . .
9.3 Psychoacoustics . . . .
9.3.1 Critical Bands and Absolute Threshold . .
. . . .
9.3.2 Masking . . .
. . . .
. . .
. . . .
ISO-MPEG-1 Audio Coding . . . .
. . . .
. . . .
Filter Banks . .
9.4.1
9.4.2
. . . .
Psychoacoustic Models . . .
9.4.3 Dynamic Bit Allocation and Coding . . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
9.5 MPEG-2 Audio Coding . . .
. . . .
9.6 MPEG-2 Advanced Audio Coding .
. . . .
9.7 MPEG-4 Audio Coding . . .
9.8 Spectral Band Replication .
. . . .
Java Applet – Psychoacoustics . . .
9.9
. . . .
9.10 Exercises
. . . .
References . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . .
. . . .
. . .
. . .
. . .
Index
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . . .
. . 246
. . 250
. . 252
. . 253
. . 257
. . 257
. . 260
. . 261
. . 269
. . 270
273
. . 273
. . 275
. . 277
. . 277
. . 279
. . 284
. . 285
. . 287
. . 290
. . 291
. . 292
. . 304
. . 306
. . 308
. . 310
. . 311
317
Preface to the Second Edition
This second edition represents a revised and extended version and offers an improved
description besides new issues and extended references. The contents of this book are
the basis of a lecture on Digital Audio Signal Processing at the Hamburg University of
Technology (TU Hamburg-Harburg) and a lecture on Multimedia Signal Processing at the
Helmut Schmidt University, Hamburg. For further studies you can ﬁnd interactive audio
demonstrations, exercises and Matlab examples on the web site
http://ant.hsu-hh.de/dasp/
Besides the basics of digital audio signal processing introduced in this second edition,
further advanced algorithms for digital audio effects can be found in the book DAFX –
Digital Audio Effects (Ed. U. Zölzer) with the related web site
http://www.dafx.de
My thanks go to Professor Dieter Leckschat, Dr. Gerald Schuller, Udo Ahlvers,
Mijail Guillemard, Christian Helmrich, Martin Holters, Dr. Florian Keiler, Stephan Möller,
Francois-Xavier Nsabimana, Christian Ruwwe, Harald Schorr, Dr. Oomke Weikert,
Catja Wilkens and Christian Zimmermann.
Udo Zölzer
Hamburg, June 2008

Preface to the First Edition
Digital audio signal processing is employed in recording and storing music and speech
signals, for sound mixing and production of digital programs, in digital transmission to
broadcast receivers as well as in consumer products like CDs, DATs and PCs. In the latter
case, the audio signal is in a digital form all the way from the microphone right up to the
loudspeakers, enabling real-time processing with fast digital signal processors.
This book provides the basis of an advanced course in Digital Audio Signal Processing
which I have been giving since 1992 at the Technical University Hamburg-Harburg.
It is directed at students studying engineering, computer science and physics and also
for professionals looking for solutions to problems in audio signal processing like in
the ﬁelds of studio engineering, consumer electronics and multimedia. The mathematical
and theoretical fundamentals of digital audio signal processing systems will be presented
and typical applications with an emphasis on realization aspects will be discussed. Prior
knowledge of systems theory, digital signal processing and multirate signal processing is
taken as a prerequisite.
The book is divided into two parts. The ﬁrst part (Chapters 1–4) presents a basis
for hardware systems used in digital audio signal processing. The second part (Chapters
5–9) discusses algorithms for processing digital audio signals. Chapter 1 describes the
course taken by an audio signal from its recording in a studio up to its reproduction
at home. Chapter 2 contains a representation of signal quantization, dither techniques
and spectral shaping of quantization errors used for reducing the nonlinear effects of
quantization. In the end, a comparison is made between the ﬁxed-point and ﬂoating-
point number representations as well as their associated effects on format conversion and
algorithms. Chapter 3 describes methods for AD/DA conversion of signals, starting with
Nyquist sampling, methods for oversampling techniques and delta-sigma modulation. The
chapter closes with a presentation of some circuit design of AD/DA converters. After an
introduction to digital signal processors and digital audio interfaces, Chapter 4 describes
simple hardware systems based on a single- and multiprocessor solutions. The algorithms
introduced in the following Chapters 5–9 are, to a great extent, implemented in real-time
on hardware platforms presented in Chapter 4. Chapter 5 describes digital audio equalizers.
Apart from the implementation aspects of recursive audio ﬁlters, nonrecursive linear phase
ﬁlters based on fast convolution and ﬁlter banks are introduced. Filter designs, parametric
ﬁlter structures and precautions for reducing quantization errors in recursive ﬁlters are dealt
with in detail. Chapter 6 deals with room simulation. Methods for simulation of artiﬁcial
room impulse response and methods for approximation of measured impulse responses
xii
Preface to the First Edition
are discussed. In Chapter 7 the dynamic range control of audio signals is described.
These methods are applied at several positions in the audio chain from the microphone
up to the loudspeakers in order to adapt to the dynamics of the recording, transmission
and listening environment. Chapter 8 contains a presentation of methods for synchronous
and asynchronous sampling rate conversion. Efﬁcient algorithms are described which are
suitable for real-time processing as well as off-line processing. Both lossless and lossy
audio coding are discussed in Chapter 9. Lossless audio coding is applied for storing of
higher word-lengths. Lossy audio coding, on the other hand, plays a signiﬁcant role in
communication systems.
I would like to thank Prof. Fliege (University of Mannheim), Prof. Kammeyer
(University of Bremen) and Prof. Heute (University of Kiel) for comments and support.
I am also grateful to my colleagues at the TUHH and especially Dr. Alfred Mertins,
Dr. Thomas Boltze, Dr. Bernd Redmer, Dr. Martin Schönle, Dr. Manfred Schusdziarra,
Dr. Tanja Karp, Georg Dickmann, Werner Eckel, Thomas Scholz, Rüdiger Wolf, Jens
Wohlers, Horst Zölzer, Bärbel Erdmann, Ursula Seifert and Dieter Gödecke. Apart from
these, I would also like to say a word of gratitude to all those students who helped me in
carrying out this work successfully.
Special thanks go to Saeed Khawaja for his help during translation and to Dr. Anthony
Macgrath for proof-reading the text. I also would like to thank Jenny Smith, Colin
McKerracher, Ian Stoneham and Christian Rauscher (Wiley).
My special thanks are directed to my wife Elke and my daughter Franziska.
Udo Zölzer
Hamburg, July 1997
Chapter 1
Introduction
It is hardly possible to make a start in the ﬁeld of digital audio signal processing without
having a ﬁrst insight into the variety of technical devices and systems of audio technology.
In this introductory chapter, the ﬁelds of application for digital audio signal processing are
presented. Starting from recording in a studio or in a concert hall, the whole chain of signal
processing is shown, up to the reproduction at home or in a car (see Fig. 1.1). The ﬁelds of
application can be divided into the following areas:
• studio technology;
• digital transmission systems;
• storage media;
• audio components for home entertainment.
The basic principles of the above-mentioned ﬁelds of application will be presented as an
overview in order to exhibit the uses of digital signal processing. Special technical devices
and systems are outside the focus of this chapter. These devices and systems are strongly
driven by the development of the computer technology with yearly changes and new
devices based on new technologies. The goal of this introduction is a trend-independent
presentation of the entire processing chain from the instrument or singer to the listener and
consumer of music. The presentation of signal processing techniques and their algorithms
will be discussed in the following chapters.
1.1 Studio Technology
While recording speech or music in a studio or in a concert hall, the analog signal from a
microphone is ﬁrst digitized, fed to a digital mixing console and then stored on a digital
storage medium. A digital sound studio is shown in Fig. 1.2. Besides the analog sources
(microphones), digital sources are fed to the digital mixing console over multichannel
MADI interfaces [AES91]. Digital storage media like digital multitrack tape machines
have been replaced by digital hard disc recording systems which are also connected via
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
2
Introduction
Concert hall
Audio studio
l
e
o
s
n
o
C
g
n
x
M
i
i
l
a
t
i
g
D
i
i
a
d
e
M
e
g
a
r
o
t
S
Digital
Transmission
Audio System
(CD,DVD,
Radio,PC)
Living room
Automobile
PC
l
a
t
i
g
D
i
Figure 1.1 Signal processing for recording, storage, transmission and reproduction.
multichannel MADI interfaces to the mixing console. The ﬁnal stereo mix is stored via a
two-channel AES/EBU interface [AES92] on a two-channel MASTER machine. External
appliances for effects or room simulators are also connected to the mixing console via
a two-channel AES/EBU interface. All systems are synchronized by a MASTER clock
reference. In digital audio technology, the sampling rates1 fS = 48 kHz for professional
studio technology, fS = 44.1 kHz for compact disc and fS = 32 kHz for broadcasting
applications are established. In addition, multiples of these sampling frequencies such as
88.2, 96, 176.4, and 192 kHz are used. The sound mixing console plays a central role
in a digital sound studio. Figure 1.3 shows the functional units. The N input signals are
processed individually. After level and panorama control, all signals are summed up to
give a stereo mix. The summation is carried out several times so that other auxiliary stereo
and/or mono signals are available for other purposes. In a sound channel (see Fig. 1.4), an
equalizer unit (EQ), a dynamic unit (DYN), a delay unit (DEL), a gain element (GAIN)
and a panorama element (PAN) are used. In addition to input and output signals in an audio
channel, inserts as well as auxiliary or direct outputs are required.
1.2 Digital Transmission Systems
In this section digital transmission will be brieﬂy explained. Besides the analog wireless
broadcasting systems based on amplitude and frequency modulation, DAB2 (Digital Audio
Broadcasting) has been introduced in several countries [Hoe01]. On the other hand, the
internet has pushed audio/video distribution, internet radio and video via cable networks.
Terrestrial Digital Broadcasting (DAB)
With the introduction of terrestrial digital broadcasting, the quality standards of a compact
disc will be achieved for mobile and stationary reception of radio signals [Ple91].
Therefore, the data rate of a two-channel AES/EBU signal from a transmitting studio
is reduced with the help of a source coder [Bra94] (see Fig. 1.5). Following the source
coder (SC), additional information (AI) like the type of program (music/speech) and
1Data rate 16 bit × 48 kHz = 768 kbit/s; data rate (AES/EBU signal) 2 × (24 + 8) bit × 48 kHz =
3.072 Mbit/s; data rate (MADI signal) 56 × (24 + 8) bit × 48 kHz = 86.016 Mbit/s.
2http://www.worlddab.org/.
1.2 Digital Transmission Systems
3
DIGITAL MULTITRACK
HARD DISC RECORDER
DIGITAL STEREO
MASTER MACHINE
MADI
AES/EBU
ROOM SIMULATOR
EFFECTS
AES/EBU
AES/EBU
DIGITAL MIXING CONSOLE
DIGITAL MIXING CONSOLE
MASTER
MASTER
SYNC
SYNC
GENERATOR
GENERATOR
MADI
MADI
AES/EBU -> MADI
CONVERSION
ANALOG-to-DIGITAL
CONVERSION
DIGITAL SIGNALS
ANALOG SIGNALS
Figure 1.2 Digital sound studio.
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)(cid:14)(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:18)(cid:14)(cid:14)(cid:14)(cid:14)(cid:22)(cid:29)(cid:14)(cid:29)(cid:14)(cid:24)
(cid:22)
(cid:6)(cid:1)
(cid:22)
(cid:23)
(cid:6)(cid:1)
(cid:23)
(cid:24)
(cid:6)(cid:1)
(cid:24)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:9)(cid:4)(cid:14)(cid:1)(cid:8)(cid:11)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)(cid:10)(cid:3)(cid:25)
(cid:30)(cid:31)(cid:13)(cid:9)(cid:4)(cid:16) (cid:2)(cid:8)(cid:14)!(cid:10)(cid:16)(cid:7)
(cid:26)"(cid:10)(cid:9)(cid:12)(cid:16)(cid:20)(cid:18)(cid:14)!(cid:10)(cid:16)(cid:7)
(cid:26)(cid:2)(cid:4)(cid:9)"(cid:14)!(cid:10)(cid:16)(cid:7)
(cid:6)(cid:7)(cid:2)(cid:8)(cid:2)(cid:11)(cid:14)&(cid:13)’(cid:16)(cid:4)(cid:16)(cid:9)(cid:8)(cid:16)(cid:2)(cid:18)
(cid:1)(cid:2)(cid:3)(cid:2)(cid:4)(cid:5)(cid:6)(cid:7)(cid:2)(cid:4)(cid:4)(cid:2)(cid:8)
#(cid:9)(cid:16)(cid:10)(cid:14)$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
(cid:1)(cid:9)(cid:10)(cid:11)(cid:8)(cid:9)(cid:12)(cid:9)(cid:5)(cid:6)(cid:7)(cid:2)(cid:4)(cid:4)(cid:2)(cid:8)
(cid:1)(cid:9)(cid:10)(cid:14)$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
(cid:6)(cid:13)(cid:12)(cid:12)(cid:2)(cid:14)(cid:15)(cid:16)(cid:10)(cid:17)(cid:18)
(cid:15)(cid:2)%(cid:7)(cid:14)$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)
(cid:6)(cid:13)(cid:12)(cid:12)(cid:2)(cid:14)(cid:19)(cid:2)(cid:20)(cid:21)(cid:7)(cid:18)(cid:14)
(cid:6)(cid:7)(cid:2)(cid:8)(cid:2)(cid:11)(cid:14)O(cid:13)(cid:7)(cid:28)(cid:13)(cid:7)
(cid:19)(cid:16)(cid:3)(cid:21)(cid:7)(cid:14)$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)
Figure 1.3 N-channel sound mixing console.
trafﬁc information is added. A multicarrier technique is applied for digital transmission
to stationary and mobile receivers. At the transmitter, several broadcasting programs are
combined in a multiplexer (MUX) to form a multiplex signal. The channel coding and
4
Introduction
modulation is carried out by a multi-carrier transmission technique (Coded Orthogonal
Frequency Division Multiplex, [Ala87, Kam92, Kam93, Tui93]).
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)(cid:10)
(cid:30))
(cid:26)*(cid:24)
(cid:26)(cid:30)(cid:15)
#&(cid:27)(cid:24)
(cid:1)&(cid:24)
(cid:15)
(cid:19)
(cid:27)(cid:10)(cid:18)(cid:2)(cid:8)(cid:7)
(cid:27)(cid:10)(cid:18)(cid:2)(cid:8)(cid:7)
((cid:13)(cid:7)(cid:28)(cid:13)(cid:7)
((cid:13)(cid:7)(cid:28)(cid:13)(cid:7)
Figure 1.4 Sound channel.
,(cid:8)(cid:11)(cid:9)-(cid:20)(cid:9)(cid:18)(cid:7)(cid:14)(cid:18)(cid:7)(cid:13)-(cid:16)(cid:11)
(cid:26)(cid:16)(cid:18)(cid:7)(cid:8)(cid:16).(cid:13)(cid:7)(cid:16)(cid:11)(cid:10)
+(cid:8)(cid:9)(cid:10)(cid:18)(cid:12)(cid:16)(cid:7)(cid:7)(cid:2)(cid:8)
&(cid:30)(cid:6)1(cid:30),!
2(cid:29)34(cid:23)(cid:14)/.(cid:16)(cid:7)1(cid:18)
(cid:6)$
(cid:27)(cid:6)(1/(cid:1)(cid:30)#(cid:22)
(cid:6)(cid:7)(cid:8)(cid:2)(cid:9)(cid:12)
&(cid:27)
/!0
/((cid:26)
(cid:1)(cid:8)(cid:11)(cid:3)(cid:8)(cid:9)(cid:12)(cid:14)$(cid:11)(cid:10)(cid:7)(cid:2)(cid:10)(cid:7)(cid:18)
+(cid:8)(cid:9)%%(cid:16)(cid:20)(cid:14)(cid:27)(cid:10)%(cid:11)(cid:8)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:27)(cid:10)%(cid:11)(cid:8)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
Figure 1.5 DAB transmitter.
The DAB receiver (Fig. 1.6) consists of the demodulator (DMOD), the demultiplexer
(DMUX) and the source decoder (SD). The SD provides a linearly quantized PCM signal
(Pulse Code Modulation). The PCM signal is fed over a Digital-to-Analog Converter (DA
Converter) to an ampliﬁer connected to loudspeakers.
(cid:26)/((cid:26)
(cid:26)/!0
&(cid:27)
(cid:6)(cid:26)
(cid:26)&$
&/(cid:1)
Figure 1.6 DAB receiver.
For a more detailed description of the DAB transmission technique, an illustration
based on ﬁlter banks is presented (see Fig. 1.7). The audio signal at a data rate of 768 kbit/s
is decomposed into subbands with the help of an analysis ﬁlter bank (AFB). Quantization
and coding based on psychoacoustic models are carried out within each subband. The data
reduction leads to a data rate of 96–192 kbit/s. The quantized subband signals are provided
with additional information (header) and combined together in a frame. This so-called ISO-
MPEG1 frame [ISO92] is ﬁrst subjected to channel coding (CC). Time-interleaving (TIL)
follows and will be described later on. The individual transmitting programs are combined
in frequency multiplex (frequency-interleaving FIL) with a synthesis ﬁlter bank (SFB) into
1.2 Digital Transmission Systems
5
one broad-band transmitting signal. The synthesis ﬁlter bank has several complex-valued
input signals and one complex-valued output signal. The real-valued band-pass signal is
obtained by modulating with ej ωct and taking the real part. At the receiver, the complex-
valued base-band signal is obtained by demodulation followed by low-pass ﬁltering.
The complex-valued analysis ﬁlter bank provides the complex-valued band-pass signals
from which the ISO-MPEG1 frame is formed after frequency and time deinterleaving
and channel decoding. The PCM signal is combined using the synthesis ﬁlter bank after
extracting the subband signals from the frame.
ω
ω
ω
ω
− ω
+ ω
ω
ω
ω
Figure 1.7 Filter banks within DAB.
DAB Transmission Technique. The special problems of mobile communications are dealt
with using a combination of the OFDM transmission technique with DPSK modulation
and time and frequency interleaving. Possible disturbances are minimized by consecutive
channel coding. The schematic diagram in Fig. 1.8 shows the relevant subsystems.
For example, the transmission of a program P1 which is delivered as an ISO-MPEG1
stream is shown in Fig. 1.8. The channel coding doubles the data rate. The typical
characteristics of a mobile communication channel like time and frequency selectivity are
handled by using time and frequency interleaving with the help of a multicarrier technique.
6
Introduction
Figure 1.8 DAB transmission technique.
The burst disturbances of consecutive bits are reduced to single bit errors by spreading
the bits over a longer period of time. The narrow-band disturbances affect only individual
carriers by spreading the transmitter program P1 in the frequency domain, i.e. distribution
of transmitter programs of carrier frequencies at a certain displacement. The remaining
disturbances of the mobile channel are suppressed with the help of channel coding, i.e. by
adding redundancy, and decoding with a Viterbi decoder. The implementation of an OFDM
transmission is discussed in the following.
1.2 Digital Transmission Systems
7
OFDM Transmission. The OFDM transmission technique is shown in Fig. 1.9. The
technique stands out owing to its simple implementation in the digital domain. The data
sequence ct (k) which is to be transmitted, is written blockwise into a register of length 2M.
The complex numbers from d1(m) to dM (m) are formed from two consecutive bits (dibits).
Here the ﬁrst bit corresponds to the real part and the second to the imaginary part. The
signal space shows the four states for the so-called QPSK [Kam92a, Pro89]. The vector
d(m) is transformed with an inverse FFT (Fast Fourier Transform) into a vector e(m)
which describes the values of the transmitted symbol in the time domain. The transmitted
symbol xt (n) with period Tsym is formed by the transmission of the M complex numbers
ei (m) at sampling period TS. The real-valued band-pass signal is formed at high frequency
after DA conversion of the quadrature signals, modulation by ej ωct and by taking the real
part. At the receiver, the transmitted symbol becomes a complex-valued sequence xr (n)
by demodulation with e−j ωct and AD conversion of the quadrature signal. M samples of
the received sequence xr (n) are distributed over the M input values fi (m) and transformed
into the frequency domain with the help of FFT. The resulting complex numbers gi(m) are
again converted to dibits and provide the received sequence cr (k). Without the inﬂuence of
the communication channel, the transmitted sequence can be reconstructed exactly.
OFDM Transmission with a Guard Interval. In order to describe the OFDM trans-
mission with a guard interval, the schematic diagram in Fig. 1.10 is considered. The
transmission of a symbol of length M over a channel with impulse response h(n) of length
L leads to a received signal y(n) of length M + L − 1. This means that the received
symbol is longer than the transmitted signal. The exact reconstruction of the transmitted
symbol is disturbed because of the overlapping of received symbols. Reconstruction of
the transmitted symbol is possible by cyclic continuation of the transmitted symbol. Here,
the complex numbers from the vector e(m) are repeated so as to give a symbol period of
Tsym = (M + L)TS. Each of the transmitted symbols is, therefore, extended to a length
of M + L. After transmission over a channel with impulse response of length L, the
response of the channel is periodic with length M. After the initial transient state of the
channel, i.e. after the L samples of the guard interval, the following M samples are written
into a register. Since a time delay occurs between the start of the transmitted symbol
and the sampling shifted by L displacements, it is necessary to shift the sequence of
length M cyclically by L displacements. The complex values gi (m) do not correspond
to the exact transmitted values di(m) because of the transmission channel h(n). However,
there is no inﬂuence of neighboring carrier frequencies. Every received value gi(m) is
weighted with the corresponding magnitude and phase of the channel at the speciﬁc
carrier frequency. The inﬂuence of the communication channel can be eliminated by
differential coding of consecutive dibits. The decoding process can be done according to
zi (m) = gi (m)g∗
i (m − 1). The dibit corresponds to the sign of the real and imaginary parts.
The DAB transmission technique presented stands out owing to its simple implementation
with the help of FFT algorithms. The extension of the transmitted symbol by a length L
of the channel impulse response and the synchronization to collect the M samples out of
the received symbol have still to be carried out. The length of the guard interval must be
matched to the maximum echo delay of the multipath channel. Owing to differential coding
of the transmitted sequence, an equalizer at the receiver is not necessary.
8
t
Introduction
Figure 1.9 OFDM transmission.
Digital Radio Mondiale (DRM)
In the interest of national and international broadcasting stations a more widespread
program delivery across regional or worldwide regions is of speciﬁc importance. This
is accomplished by analog radio transmission in the frequency range below 30 MHz.
The limited audio quality of the amplitude modulation technique (channel bandwidth 9–
10 kHz) with an audio bandwidth of 4.5 kHz leads to a low acceptance rate for such kind
of audio broadcasting. The introduction of the digital transmission technique Digital Radio
Mondiale3 will replace the existing analog transmission systems. The digital transmission
is based on OFDM and the audio coding MPEG4-AAC in combination with SBR (Spectral
Band Replication).
3http://www.drm.org.
1.2 Digital Transmission Systems
9
Figure 1.10 OFDM transmission with a guard interval.
Internet Audio
The growth of the internet offers new distribution possibilities for information, but
especially for audio and video signals. The distribution of audio signals is mainly driven by
the MP3 format (MPEG-1 Audio Layer III [Bra94]) or in proprietary formats of different
companies. The compressed transmission is used because the data rate of home users is
10
Introduction
still low compared to lossless audio and video formats. Since the transmission is based on
ﬁle transfer of packets, the data rates strongly depend on the providing server, the actual
internet trafﬁc and the access point of the home computer. A real-time transfer of high-
quality music is still not possible. If the audio compression is high enough to achieve a just
acceptable audio quality, a real-time transfer with a streaming technology is possible, since
the ﬁle size is small and a transmission needs less time (see Fig. 1.11). For this a receiver
needs a double memory ﬁlled with incoming packets of a coded audio ﬁle and a parallel
running audio decoding. After decoding of a memory with a sufﬁciently long audio portion
the memory is transferred to the sound card of the computer. During sound playback of
the decoded audio signal further incoming packets are received and decoded. Packet loss
can lead to interrupts in the audio signal. Several techniques for error concealment and
protocols allow the transfer of coded audio.
Audio
Audio
compression
WEB Server
Internet
Intranet
WEB-
Browser
Media-Player
Figure 1.11 Audio streaming via the internet.
1.3 Storage Media
Compact Disc
The technological advances in the semiconductor industry have led to economical storage
media for digitally encoded information. Independently of developments in the computer
industry, the compact disc system was introduced by Philips and Sony in 1982. The storage
of digital audio data is carried out on an optical storage medium. The compact disc operates
at a sampling rate of fS = 44.1 kHz.4 The essential speciﬁcations are summarized in
Table 1.1.
R-DAT (Rotary-head Digital Audio on Tape)
The R-DAT system makes use of the heliscan method for two-channel recording. The
available devices enable the recording of 16-bit PCM signals with all three sampling rates
(Table 1.2) on a tape. R-DAT recorders are used in studio recording as well as in consumer
applications.
MiniDisc and MP3 Format
Advanced coding techniques are based on psychoacoustics for data reduction. A widespread
storage system is the MiniDisc by Sony. The Mini Disc system operates with the ATRAC
technique (Adaptive Transform Acoustic Coding, [Tsu92]) and has a data rate of about
43 × 490 × 30 Hz (NTSC) = 3 × 588 × 25 Hz (CCIR) = 44.1 kHz.
1.3 Storage Media
11
Table 1.1 Speciﬁcations of the CD system [Ben88].
Type of recording
Signal recognition
Storage density
Audio speciﬁcation
Number of channels
Duration
Frequency range
Dynamic range
THD
Signal format
Sampling rate
Quantization
Pre-emphasis
Error Correction
Data rate
Modulation
Channel bit rate
Redundancy
Optical
682 Mbit/in2
2
Approx. 60 min.
20–20 000 Hz
>90 dB
<0.01%
44.1 kHz
16-bit PCM (2’s complement)
None or 50/15 µs
CIRC
2.034 Mbit/s
EFM
4.3218 Mbit/s
30%
Mechanical speciﬁcation
Diameter
Thickness
Diameter of the inner hole
Program range
Reading speed
120 mm
1.2 mm
15 mm
50–116 mm
1.2–1.4 m/s
500–200 r/min.
2 · 140 kbit/s for a stereo channel. A magneto-optical storage medium is used for recording.
The MP3 format was developed simultaneously, but the availability of recording and
playback systems has taken a longer time. Simple MP3 recorders and playback systems
are now available for the consumer market.
Super Audio Compact Disc (SACD)
The SACD was speciﬁed by Philips and Sony in 1999 as a further development of the
compact disc with the objective of improved sound quality. The audio frequency range of
20 kHz is perceived as a limiting audio quality factor by some human beings, and the anti-
aliasing and reconstruction ﬁlters may lead to ringing resulting from linear phase ﬁlters.
This effect follows from short audio pulses leading to audible transients of the ﬁlters.
In order to overcome these problems the audio bandwidth is extended to 100 kHz and
the sampling frequency is increased to 2.8224 MHz (64 × 44.1 kHz). With this the ﬁlter
speciﬁcations can be met with simple ﬁrst-order ﬁlters. The quantization of the samples is
based on a 1-bit quantizer within a delta-sigma converter structure which uses noise shaping
12
Introduction
Table 1.2 Speciﬁcations of the R-DAT system [Ben88].
Type of recording
Signal recognition
Storage capacity
Audio speciﬁcation
Number of channels
Duration
Frequency range
Dynamic range
THD
Signal format
Sampling rate
Quantization
Error correction
Channel coding
Data rate
Channel bit rate
Magnetic
2 GB
2
Max. 120 min.
20–20 000 Hz
>90 dB
<0.01%
48, 44.1, 32 kHz
16-bit PCM (2’s complement)
CIRC
8/10 modulation
2.46 Mbit/s
9.4 Mbit/s
Mechanical speciﬁcation
Tapewidth of magnet
Thickness
Diameter of head drum 3 cm
Revolutions per min.
Rel. track speed
3.8 mm
13 µm
2000 r/min.
3.133 m/s
500–200 r/min.
(see Fig. 1.12). The 1-bit signal with 2.8224 MHz sampling frequency is denoted a DSD
signal (Direct Stream Digital). The DA conversion of a DSD signal into an analog signal
is accomplished with a simple analog ﬁrst-order low-pass. The storage of DSD signals is
achieved by a special compact disc (Fig. 1.13) with a CD layer in PCM format and an HD
layer (High Density) with a DVD 4.38 GByte layer. The HD layer stores a stereo signal
in 1-bit DSD format and a 6-channel 1-bit signal with a lossless compression technique
(Direct Stream Transfer DST) [Jan03]. The CD layer of the SACD can be replayed with
a conventional CD player, whereas special SACD players can replay the HD layer. An
extensive discussion of 1-bit delta-sigma techniques can be found in [Lip01a, Lip01b,
Van01, Lip02, Van04].
x(t)
Analog-
DSD
1-bi t
64.fS
1-bi t
DSD
Memory
1-bi t
64.fS
DSD-
Analog
y(t)
Figure 1.12 SACD system.
1.4 Audio Components at Home
13
Protection layer
CD Layer
Plastic
HD Layer
Plastic
Laser Scanner
Figure 1.13 Layer of the SACD.
Digital Versatile Disc – Audio (DVD-A)
To increase the storage capacity of the CD the Digital Versatile Disc (DVD) was developed.
The physical dimensions are identical to the CD. The DVD has two layers with one or
two sides, and the storage capacity per side has been increased. For a one-sided version
for audio applications the storage capacity is 4.7 GB. A comparison of speciﬁcations for
different disc media is shown in Table 1.3. Besides stereo signals with different sampling
rates and word-lengths a variety of multi-channel formats can be stored. For data reduction
a lossless compression technique, MLP (Meridian Lossless Packing), is applied. The
improved audio quality compared to the CD audio is based on the higher sampling rates
and word-lengths and the multichannel features of the DVD-A.
Table 1.3 Speciﬁcations of CD, SACD and DVD-A.
Parameter
CD
SACD
DVD-A
Coding
Sampling rate
Channels
Compression
Recording time
Frequency range
Dynamic range
Copy protection
16-bit PCM
44.1 kHz
2
No
74 min.
20–20 000 Hz
96 dB
No
1-bit DSD
2.8224 MHz
2–6
Yes (DST)
70–80 min.
20–100 000 Hz
120 dB
Yes
16-/20-/24-bit PCM
44.1/48/88.2/96/176.4/192 kHz
1–6
Yes (MLP)
62–843 min.
20–96 000 Hz
144 dB
Yes
1.4 Audio Components at Home
Domestic digital storage media are already in use, like compact discs, personal computers
and MP3 players, which have digital outputs, and can be connected to digital post-
processing systems right up to the loudspeakers. The individual tone control consists of
the following processing.
14
Equalizer
Introduction
Spectral modiﬁcation of the music signal in amplitude and phase and the automatic
correction of the frequency response from loudspeaker to listening environment are desired.
Room Simulation
The simulation of room impulse responses and the processing of music signals with special
room impulse response are used to give an impression of a room like a concert hall, a
cathedral or a jazz club.
Surround Systems
Besides the reproduction of stereo signals from a CD over two frontal loudspeakers, more
than two channels will be recorded in the prospective digital recording systems [Lin93].
This is already illustrated in the sound production for cinema movies where, besides the
stereo signal (L, R), a middle channel (M) and two additional room signals (LB, RB )
are recorded. These surround systems are also used in the prospective digital television
systems. The ambisonics technique [Ger85] is a recording technique that allows three-
dimensional recording and reproduction of sound.
Digital Ampliﬁer Concepts
The basis of a digital ampliﬁer is pulse width modulation as shown in Fig. 1.14. With the
help of a fast counter, a pulse width modulated signal is formed out of the w-bit linearly
quantized signal. Single-sided and double-sided modulated conversion are used and they
are represented by two and three states, respectively. Single-sided modulation (2 states, −1
and +1) is performed by a counter which counts upward from zero with multiples of the
sampling rate. The number range of the PCM signal from −1 to+ 1 is directly mapped
onto the counter. The duration of the pulse width is controlled by a comparator. For pulse
width modulation with three states (−1, 0, +1), the sign of the PCM signal determines the
state. The pulse width is determined by a mapping of the number range from 0 to 1 onto
a counter. For double-sided modulation, an upward/downward counter is needed which
has to be clocked at twice the rate compared with single-sided modulation. The allocation
of pulse widths is shown in Fig. 1.14. In order to reduce the clock rate for the counter,
pulse width modulation is carried out after oversampling (Oversampling) and noise shaping
(Noise Shaping) of the quantization error (see Fig. 1.15, [Gol90]). Thus the clock rate of
the counter is reduced to 180.6 MHz. The input signal is ﬁrst upsampled by a factor of
16 and then quantized to 8-bits with third-order noise shaping. The use of pulse shaping
with delta-sigma modulation is shown in Fig. 1.16 [And92]. Here a direct conversion of the
delta-sigma modulated 1-bit signal is performed. The pulse converter shapes the envelope
of the serial data bits. The low-pass ﬁlter reconstructs the analog signal. In order to reduce
nonlinear distortion, the output signal is fed back (see Fig. 1.17, [Klu92]). New methods
for the generation of pulse width modulation try to reduce the clock rates and the high
frequency components [Str99, Str01].
1.4 Audio Components at Home
15
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23);
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
(cid:7)
(cid:16)
.
-
;
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23);(cid:5)(cid:22)
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
(cid:7)
(cid:16)
.
-
;
3
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
(cid:18)(cid:16)(cid:10)(cid:3)(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)7(cid:23)(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)8(cid:5)(cid:22):
(cid:18)(cid:16)(cid:10)(cid:3)(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)72(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)838(cid:5)(cid:22):
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23);
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
(cid:7)
(cid:16)
.
-
;
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23) ;(cid:5)(cid:22)
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
(cid:7)
(cid:16)
.
-
;
3
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
-(cid:11)(cid:13).(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)7(cid:23)(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)8(cid:5)(cid:22):
-(cid:11)(cid:13).(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)72(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)838(cid:5)(cid:22):
Figure 1.14 Pulse width modulation.
(cid:22)?3(cid:29)>(cid:14)/=
’7(cid:10):
<<(cid:29)(cid:22)(cid:14)(cid:17)=
(cid:22)>-.(cid:16)(cid:7)
((cid:6)1(cid:24)(cid:6)
435(cid:29)>(cid:14)(cid:17)=
?-.(cid:16)(cid:7)
(cid:1)6/
(cid:1)6/
&(cid:12)(cid:28)(cid:4)(cid:16)%(cid:16)(cid:2)(cid:8)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
Figure 1.15 Pulse width modulation with oversampling and noise shaping.
16
Introduction
(cid:22)-.(cid:16)(cid:7)(cid:14)(cid:26)&$
’7(cid:10):
<<(cid:29)(cid:22)(cid:14)(cid:17)=
(cid:22)>-.(cid:16)(cid:7)
((cid:6)1(cid:24)(cid:6)
(cid:22)(cid:22)(cid:29)2(cid:14)/=
(cid:22)-.(cid:16)(cid:7)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
=@(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
(cid:22)-.(cid:16)(cid:7)
(cid:1)(cid:13)(cid:4)(cid:18)(cid:2)
=@(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
$(cid:11)(cid:10)A(cid:2)(cid:8)(cid:7)(cid:2)(cid:8) (cid:22)-.(cid:16)(cid:7)(cid:14)(cid:1)(cid:13)(cid:4)(cid:18)(cid:2)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
=@(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
(cid:22)-.(cid:16)(cid:7)(cid:14)(cid:26)(cid:9)(cid:7)(cid:9)
(cid:22)
(cid:22)
3
3
(cid:22)
3
3
(cid:22)-.(cid:16)(cid:7)(cid:14)(cid:1)(cid:13)(cid:4)(cid:18)(cid:2)
"7(cid:7):
Figure 1.16 Pulse shaping after delta-sigma modulation.
% (cid:6)
’7(cid:10):
=7 :
&(cid:12)(cid:28)(cid:4)(cid:16)%(cid:16)(cid:2)(cid:8)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
&(cid:26)$
% (cid:6)
Figure 1.17 Delta-sigma modulated ampliﬁer with feedback.
Digital Crossover
In order to perform digital crossovers for loudspeakers, a linear phase decomposition of the
signal with a special ﬁlter bank [Zöl92] is done (Fig. 1.18). In a ﬁrst step, the input signal
is decomposed into its high-pass and low-pass components and the high-pass signal is fed
to a DAC over a delay unit. In the next step, the low-pass signal is further decomposed.
The individual band-pass signals and the low-pass signal are then fed to the respective
loudspeaker. Further developments for the control of loudspeakers can be found in [Kli94,
Kli98a, Kli98b, Mül99].
References
17
+$(cid:22)
+$(cid:23)
+$2
+$(cid:24)
’
7(cid:10) :(cid:22)
%(cid:6)(cid:22)
@(cid:6)(cid:22)
%(cid:6)(cid:22)
%(cid:6)(cid:22)
@(cid:6)(cid:23)
%(cid:6)(cid:23)
%(cid:6)(cid:23)
@(cid:6)2
%
(cid:6)2
%(cid:6)2
%(cid:6)<
% (cid:6)(cid:24)
@(cid:6)(cid:24)
%(cid:6)(cid:24)
(cid:26)(cid:30)(cid:15)(cid:22)
(cid:26)(cid:30)(cid:15)(cid:23)
(cid:26)(cid:30)(cid:15)2
(cid:26)(cid:30)(cid:15)(cid:24)
%(cid:6)(cid:23)
%(cid:6)2
" 7(cid:10)(cid:22)
:(cid:22)
" 7(cid:10) :
(cid:23)
(cid:23)
" 7(cid:10) :
2
2
=(cid:16)(cid:3)(cid:21)(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:14),(cid:9)(cid:10)-(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)(cid:14)(cid:22)
,(cid:9)(cid:10)-(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)(cid:14)(cid:23)
%(cid:6)(cid:24)
" 7(cid:10) :(cid:24)(cid:24)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
Figure 1.18 Digital crossover (FSi frequency splitting, TCi transition bandwidth control, DELi
delay).
References
[AES91] AES10-1991 (ANSI S4.43-1991): AES Recommended Practice for Digital
Audio Engineering – Serial Multichannel Audio Digital Interface (MADI).
[AES92] AES3-1992 (ANSI S4.40-1992): AES Recommended Practice for Digital
Audio Engineering – Serial Transmission Format for Two-Channel Linearly
Represented Digital Audio.
[Ala87] M. Alard, R. Lasalle: Principles of Modulation and Channel Coding for Digital
Broadcasting for Mobile Receivers, EBU Review, No. 224, pp. 168–190,
August 1987.
[And92] M. Andersen: New Principle for Digital Audio Power Ampliﬁers, Proc. 92nd
AES Convention, Preprint No. 3226, Vienna, 1992.
[Ben88] K. B. Benson: Audio Engineering Handbook, McGraw-Hill, New York, 1988.
[Bra94]
K. Brandenburg, G. Stoll: ISO/MPEG-1 Audio: A Generic Standard for Coding
of High Quality Digital Audio, J. Audio Eng. Soc., Vol. 42, No. 10, pp. 780–
792, October 1994.
[Ger85] M. A. Gerzon: Ambisonics in Multichannel Broadcasting and Video, J. Audio
Eng. Soc., Vol. 33, No. 11, pp. 859–871, November 1985.
[Gol90]
J. M. Goldberg, M. B. Sandler: New Results in PWM for Digital Power
Ampliﬁcation, Proc. 89th AES Convention, Preprint No. 2959, Los Angeles,
1990.
[Hoe01] W. Hoeg, T. Lauterbach: Digital Audio Broadcasting, John Wiley & Sons Ltd,
Chichester, 2001.
18
Introduction
[ISO92]
ISO/IEC 11172-3: Coding of Moving Pictures and Associated Audio for Digital
Storage Media at up to 1.5 Mbits/s – Audio Part, International Standard, 1992.
[Jan03]
E. Janssen, D. Reefman: Super Audio CD: An Introduction, IEEE Signal
Processing Magazine, pp. 83–90, July 2003.
[Kam92] K. D. Kammeyer, U. Tuisel, H. Schulze, H. Bochmann: Digital Multicarrier-
Transmission of Audio Signals over Mobile Radio Channels, Europ. Trans. on
Telecommun. ETT, Vol. 3, pp. 243–254, May–June 1992.
[Kam93] K. D. Kammeyer, U. Tuisel: Synchronisationsprobleme in digitalen Multi-
trägersystemen, Frequenz, Vol. 47, pp. 159–166, May 1993.
[Kli94] W. Klippel: Das nichtlineare Übertragungsverhalten elektroakustischer Wand-
ler, Habilitationsschrift, Technische Universität Dresden, 1994.
[Kli98a] W. Klippel: Direct Feedback Linearization of Nonlinear Loudspeaker Systems,
J. Audio Eng. Soc., Vol. 46, No. 6, pp. 499–507, 1998.
[Kli98b] W. Klippel: Adaptive Nonlinear Control of Loudspeaker Systems, J. Audio Eng.
Soc., Vol. 46, No. 11, pp. 939–954, 1998.
[Klu92]
J. Klugbauer-Heilmeier: A Sigma Delta Modulated Switching Power Amp,
Proc. 92nd AES Convention, Preprint No. 3227, Vienna, 1992.
[Lec92]
D. Leckschat: Verbesserung der Wiedergabequalität von Lautsprechern mit
Hilfe von Digitalﬁltern, Dissertation, RWTH Aachen, 1992.
[Lin93]
B. Link, D. Mandell: A DSP Implementation of a Pro Logic Surround Decoder,
Proc. 95th AES Convention, Preprint No. 3758, New York, 1993.
[Lip01a]
S. P. Lipshitz, J. Vanderkooy: Why 1-Bit Sigma-Delta Conversion is
Unsuitable for High-Quality Applications, Proc. 110th Convention of the
Audio Engineering Society, Preprint No. 5395, Amsterdam, 2001.
[Lip01b]
S. P. Lipshitz. J. Vanderkooy: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 2, Proc. 111th Convention of the Audio Engineering
Society, Preprint No. 5477, New York, 2001
[Lip02]
S. P. Lipshitz, J. Vanderkooy: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 3, Proc. 112th Convention of the Audio Engineering
Society, Preprint No. 5620, Munich, 2002.
[Mül99]
S. Müller: Digitale Signalverarbeitung für Lautsprecher, Dissertation, RWTH
Aachen, 1999.
[Ple91]
G. Plenge: DAB – Ein neues Hörrundfunksystem – Stand der Entwicklung und
Wege zu seiner Einführung, Rundfunktechnische Mitteilungen, Vol. 35, No. 2,
pp. 46–66, 1991.
References
[Str99]
[Str01]
[Tsu92]
[Tui93]
[Van01]
[Van04]
[Zöl92]
19
M. Streitenberger, H. Bresch, W. Mathis: A New Concept for High Performance
Class-D Audio Ampliﬁcation, Proc. AES 106th Convention, Preprint No. 4941,
Munich, 1999.
M. Streitenberger, F. Felgenhauer, H. Bresch, W. Mathis: Zero Position Coding
(ZePoC) – A Generalised Concept of Pulse-Length Modulated Signals and its
Application to Class-D Audio Power Ampliﬁers, Proc. AES 110th Convention,
Preprint No. 5365, Amsterdam, 2001.
K. Tsutsui, H. Suzuki, O. Shimoyoshi, M. Sonohara, K. Akagiri, R. Heddle:
ATRAC: Adaptive Transform Coding for MiniDisc, Proc. 91st AES Convention,
Preprint No. 3216, New York, 1991.
U. Tuisel: Multiträgerkonzepte für die digitale,
funkübertragung, Dissertation, TU Hamburg-Harburg, 1993.
terrestrische Hörrund-
J. Vanderkooy, S. P. Lipshitz: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 1, Proc. 110th Convention of the Audio Engineering
Society, Preprint No. 5398, Amsterdam, 2001.
J. Vanderkooy, S. P. Lipshitz: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 4, Proc. 116th Convention of the Audio Engineering
Society, Preprint No. 6093, Berlin, 2004.
U. Zölzer, N. Fliege: Logarithmic Spaced Analysis Filter Bank for Multiple
Loudspeaker Channels, Proc. 93rd AES Convention, Preprint No. 3453, San
Francisco, 1992.
