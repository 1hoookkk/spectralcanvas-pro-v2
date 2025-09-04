# Digital Audio DSP (Zölzer) — Chunk 1/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1445

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
