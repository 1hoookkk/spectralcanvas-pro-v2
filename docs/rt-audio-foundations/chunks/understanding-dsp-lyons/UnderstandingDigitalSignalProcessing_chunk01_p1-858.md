# Understanding DSP (Lyons) — Chunk 1/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1799

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Understanding Digital Signal Processing
Third Edition
Richard G. Lyons
Upper Saddle River, NJ • Boston • Indianapolis • San Francisco
New York • Toronto • Montreal • London • Munich • Paris • Madrid
Capetown • Sydney • Tokyo • Singapore • Mexico City
Many of the designations used by manufacturers and sellers to distinguish their products are claimed
as  trademarks.  Where  those  designations  appear  in  this  book,  and  the  publisher  was  aware  of  a
trademark claim, the designations have been printed with initial capital letters or in all capitals.
The author and publisher have taken care in the preparation of this book, but make no expressed or
implied  warranty  of  any  kind  and  assume  no  responsibility  for  errors  or  omissions.  No  liability  is
assumed for incidental or consequential damages in connection with or arising out of the use of the
information or programs contained herein.
The publisher offers excellent discounts on this book when ordered in quantity for bulk purchases or
special sales, which may include electronic versions and/or custom covers and content particular to
your business, training goals, marketing focus, and branding interests. For more information, please
contact:
U.S. Corporate and Government Sales
(800) 382-3419
corpsales@pearsontechgroup.com
For sales outside the United States please contact:
International Sales
international@pearson.com
Visit us on the Web: informit.com/ph
Library of Congress Cataloging-in-Publication Data
Lyons, Richard G., 1948-
    Understanding digital signal processing / Richard G. Lyons.—3rd ed.
        p.    cm.
    Includes bibliographical references and index.
    ISBN 0-13-702741-9 (hardcover : alk. paper)
  1.    Signal processing—Digital techniques.    I.  Title.
    TK5102.9.L96 2011
    621.382’2—dc22                                                                          2010035407
Copyright © 2011 Pearson Education, Inc.
All  rights  reserved.  Printed  in  the  United  States  of  America.  This  publication  is  protected  by
copyright, and permission must be obtained from the publisher prior to any prohibited reproduction,
storage  in  a  retrieval  system,  or  transmission  in  any  form  or  by  any  means,  electronic,  mechanical,
photocopying, recording, or likewise. For information regarding permissions, write to:
Pearson Education, Inc.
Rights and Contracts Department
501 Boylston Street, Suite 900
Boston, MA 02116
Fax: (617) 671-3447
ISBN-13: 978-0-13-702741-5
ISBN-10:        0-13-702741-9
Text printed in the United States on recycled paper at Edwards Brothers in Ann Arbor, Michigan.
Second printing, April 2011
I dedicate this book to my daughters, Julie and Meredith—I wish I could go with you; to my mother,
Ruth, for making me finish my homework; to my father, Grady, who didn’t know what he started when
he built that workbench in the basement; to my brother Ray for improving us all; to my brother Ken
who succeeded where I failed; to my sister Nancy for running interference for us; and to the skilled
folks  on  the  USENET  newsgroup  comp.dsp.  They  ask  the  good  questions  and  provide  the  best
answers.  Finally,  to  Sigi  Pardula  (Batgirl):  Without  your  keeping  the  place  running,  this  book
wouldn’t exist.

Contents
PREFACE
ABOUT THE AUTHOR
1 DISCRETE SEQUENCES AND SYSTEMS
1.1 Discrete Sequences and Their Notation
1.2 Signal Amplitude, Magnitude, Power
1.3 Signal Processing Operational Symbols
1.4 Introduction to Discrete Linear Time-Invariant Systems
1.5 Discrete Linear Systems
1.6 Time-Invariant Systems
1.7 The Commutative Property of Linear Time-Invariant Systems
1.8 Analyzing Linear Time-Invariant Systems
References
Chapter 1 Problems
2 PERIODIC SAMPLING
2.1 Aliasing: Signal Ambiguity in the Frequency Domain
2.2 Sampling Lowpass Signals
2.3 Sampling Bandpass Signals
2.4 Practical Aspects of Bandpass Sampling
References
Chapter 2 Problems
3 THE DISCRETE FOURIER TRANSFORM
3.1 Understanding the DFT Equation
3.2 DFT Symmetry
3.3 DFT Linearity
3.4 DFT Magnitudes
3.5 DFT Frequency Axis
3.6 DFT Shifting Theorem
3.7 Inverse DFT
3.8 DFT Leakage
3.9 Windows
3.10 DFT Scalloping Loss
3.11 DFT Resolution, Zero Padding, and Frequency-Domain Sampling
3.12 DFT Processing Gain
3.13 The DFT of Rectangular Functions
3.14 Interpreting the DFT Using the Discrete-Time Fourier Transform
References
Chapter 3 Problems
4 THE FAST FOURIER TRANSFORM
4.1 Relationship of the FFT to the DFT
4.2 Hints on Using FFTs in Practice
4.3 Derivation of the Radix-2 FFT Algorithm
4.4 FFT Input/Output Data Index Bit Reversal
4.5 Radix-2 FFT Butterfly Structures
4.6 Alternate Single-Butterfly Structures
References
Chapter 4 Problems
5 FINITE IMPULSE RESPONSE FILTERS
5.1 An Introduction to Finite Impulse Response (FIR) Filters
5.2 Convolution in FIR Filters
5.3 Lowpass FIR Filter Design
5.4 Bandpass FIR Filter Design
5.5 Highpass FIR Filter Design
5.6 Parks-McClellan Exchange FIR Filter Design Method
5.7 Half-band FIR Filters
5.8 Phase Response of FIR Filters
5.9 A Generic Description of Discrete Convolution
5.10 Analyzing FIR Filters
References
Chapter 5 Problems
6 INFINITE IMPULSE RESPONSE FILTERS
6.1 An Introduction to Infinite Impulse Response Filters
6.2 The Laplace Transform
6.3 The z-Transform
6.4 Using the z-Transform to Analyze IIR Filters
6.5 Using Poles and Zeros to Analyze IIR Filters
6.6 Alternate IIR Filter Structures
6.7 Pitfalls in Building IIR Filters
6.8 Improving IIR Filters with Cascaded Structures
6.9 Scaling the Gain of IIR Filters
6.10 Impulse Invariance IIR Filter Design Method
6.11 Bilinear Transform IIR Filter Design Method
6.12 Optimized IIR Filter Design Method
6.13 A Brief Comparison of IIR and FIR Filters
References
Chapter 6 Problems
7 SPECIALIZED DIGITAL NETWORKS AND FILTERS
7.1 Differentiators
7.2 Integrators
7.3 Matched Filters
7.4 Interpolated Lowpass FIR Filters
7.5 Frequency Sampling Filters: The Lost Art
References
Chapter 7 Problems
8 QUADRATURE SIGNALS
8.1 Why Care about Quadrature Signals?
8.2 The Notation of Complex Numbers
8.3 Representing Real Signals Using Complex Phasors
8.4 A Few Thoughts on Negative Frequency
8.5 Quadrature Signals in the Frequency Domain
8.6 Bandpass Quadrature Signals in the Frequency Domain
8.7 Complex Down-Conversion
8.8 A Complex Down-Conversion Example
8.9 An Alternate Down-Conversion Method
References
Chapter 8 Problems
9 THE DISCRETE HILBERT TRANSFORM
9.1 Hilbert Transform Definition
9.2 Why Care about the Hilbert Transform?
9.3 Impulse Response of a Hilbert Transformer
9.4 Designing a Discrete Hilbert Transformer
9.5 Time-Domain Analytic Signal Generation
9.6 Comparing Analytical Signal Generation Methods
References
Chapter 9 Problems
10 SAMPLE RATE CONVERSION
10.1 Decimation
10.2 Two-Stage Decimation
10.3 Properties of Downsampling
10.4 Interpolation
10.5 Properties of Interpolation
10.6 Combining Decimation and Interpolation
10.7 Polyphase Filters
10.8 Two-Stage Interpolation
10.9 z-Transform Analysis of Multirate Systems
10.10 Polyphase Filter Implementations
10.11 Sample Rate Conversion by Rational Factors
10.12 Sample Rate Conversion with Half-band Filters
10.13 Sample Rate Conversion with IFIR Filters
10.14 Cascaded Integrator-Comb Filters
References
Chapter 10 Problems
11 SIGNAL AVERAGING
11.1 Coherent Averaging
11.2 Incoherent Averaging
11.3 Averaging Multiple Fast Fourier Transforms
11.4 Averaging Phase Angles
11.5 Filtering Aspects of Time-Domain Averaging
11.6 Exponential Averaging
References
Chapter 11 Problems
12 DIGITAL DATA FORMATS AND THEIR EFFECTS
12.1 Fixed-Point Binary Formats
12.2 Binary Number Precision and Dynamic Range
12.3 Effects of Finite Fixed-Point Binary Word Length
12.4 Floating-Point Binary Formats
12.5 Block Floating-Point Binary Format
References
Chapter 12 Problems
13 DIGITAL SIGNAL PROCESSING TRICKS
13.1 Frequency Translation without Multiplication
13.2 High-Speed Vector Magnitude Approximation
13.3 Frequency-Domain Windowing
13.4 Fast Multiplication of Complex Numbers
13.5 Efficiently Performing the FFT of Real Sequences
13.6 Computing the Inverse FFT Using the Forward FFT
13.7 Simplified FIR Filter Structure
13.8 Reducing A/D Converter Quantization Noise
13.9 A/D Converter Testing Techniques
13.10 Fast FIR Filtering Using the FFT
13.11 Generating Normally Distributed Random Data
13.12 Zero-Phase Filtering
13.13 Sharpened FIR Filters
13.14 Interpolating a Bandpass Signal
13.15 Spectral Peak Location Algorithm
13.16 Computing FFT Twiddle Factors
13.17 Single Tone Detection
13.18 The Sliding DFT
13.19 The Zoom FFT
13.20 A Practical Spectrum Analyzer
13.21 An Efficient Arctangent Approximation
13.22 Frequency Demodulation Algorithms
13.23 DC Removal
13.24 Improving Traditional CIC Filters
13.25 Smoothing Impulsive Noise
13.26 Efficient Polynomial Evaluation
13.27 Designing Very High-Order FIR Filters
13.28 Time-Domain Interpolation Using the FFT
13.29 Frequency Translation Using Decimation
13.30 Automatic Gain Control (AGC)
13.31 Approximate Envelope Detection
13.32 A Quadrature Oscillator
13.33 Specialized Exponential Averaging
13.34 Filtering Narrowband Noise Using Filter Nulls
13.35 Efficient Computation of Signal Variance
13.36 Real-time Computation of Signal Averages and Variances
13.37 Building Hilbert Transformers from Half-band Filters
13.38 Complex Vector Rotation with Arctangents
13.39 An Efficient Differentiating Network
13.40 Linear-Phase DC-Removal Filter
13.41 Avoiding Overflow in Magnitude Computations
13.42 Efficient Linear Interpolation
13.43 Alternate Complex Down-conversion Schemes
13.44 Signal Transition Detection
13.45 Spectral Flipping around Signal Center Frequency
13.46 Computing Missing Signal Samples
13.47 Computing Large DFTs Using Small FFTs
13.48 Computing Filter Group Delay without Arctangents
13.49 Computing a Forward and Inverse FFT Using a Single FFT
13.50 Improved Narrowband Lowpass IIR Filters
13.51 A Stable Goertzel Algorithm
References
A THE ARITHMETIC OF COMPLEX NUMBERS
A.1 Graphical Representation of Real and Complex Numbers
A.2 Arithmetic Representation of Complex Numbers
A.3 Arithmetic Operations of Complex Numbers
A.4 Some Practical Implications of Using Complex Numbers
B CLOSED FORM OF A GEOMETRIC SERIES
C TIME REVERSAL AND THE DFT
D MEAN, VARIANCE, AND STANDARD DEVIATION
D.1 Statistical Measures
D.2 Statistics of Short Sequences
D.3 Statistics of Summed Sequences
D.4 Standard Deviation (RMS) of a Continuous Sinewave
D.5 Estimating Signal-to-Noise Ratios
D.6 The Mean and Variance of Random Functions
D.7 The Normal Probability Density Function
E DECIBELS (DB AND DBM)
E.1 Using Logarithms to Determine Relative Signal Power
E.2 Some Useful Decibel Numbers
E.3 Absolute Power Using Decibels
F DIGITAL FILTER TERMINOLOGY
G FREQUENCY SAMPLING FILTER DERIVATIONS
G.1 Frequency Response of a Comb Filter
G.2 Single Complex FSF Frequency Response
G.3 Multisection Complex FSF Phase
G.4 Multisection Complex FSF Frequency Response
G.5 Real FSF Transfer Function
G.6 Type-IV FSF Frequency Response
H FREQUENCY SAMPLING FILTER DESIGN TABLES
I COMPUTING CHEBYSHEV WINDOW SEQUENCES
I.1 Chebyshev Windows for FIR Filter Design
I.2 Chebyshev Windows for Spectrum Analysis
INDEX
