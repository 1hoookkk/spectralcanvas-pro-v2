# Audio Plugins C++ (Pirkle) — Chunk 1/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 543

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
W I L L   P I R K L E

DE SIG NING
AUDI O EF FECT
PLUG-I NS IN C++
WI TH DIGI TAL AUDIO  SIGNAL  PR O C ESS I N G T HEO RY
bool __stdcall CReverrbb::proccessAAAuuudioFrame(ﬂoat* pInnnpppuuutttBBBuuuffffffeeerr,,
bbooooll
ool __stdcal
ooooll  ____ssttddccaall
ssttddccalll CReverb::processAuddiiiooFFrrame(ﬂoat* pppppppIIInnnpppuuu
p
p
UUINT N mmInputChannels, UINT uNumOutpuuttCChhhannels)
t
tBuffeerr
ppuppuutBuffeerr,,
tBuffee
tBuffeerr,,
ttBBuuffffeerr
ttBBBuuffffffeeerrr
ppuutBuff
,
A i F
A ddii FF
(ﬂ
(ﬂ
*
III
  UUUIIINNTTTT uuNNuummOOOOOuuuuuu
{
// output = input -- change thhiiss ffoorr mmeeaanniinnggffuull ppprroocccceeeesssssiinngg
//
// DDoo LLEEFFTT ((MMOONO)) CChhannel; thheeree iss aallwwwwaaaayyys aatt lleeaaast one input/one
////  (INSERT EEffeecctt)
DDoo LL FFT (MONO) Channel; there is
DDoo  LLEEEEFFFT (MONO) Channnneel; theree iis
// Do LEFT ((
//// DDoo LLEEFFTT (
////// DDDDoooo LLLEEFFTT ((MMOONNOO)) CChhaannnneell;; tthherree iiss aaalllwwwwaayyss att lleastt
ppppppOutputBufffffffeeeer[0] = ppInppuuttBBuuffer[[00]] mm_fVolume;
))
///////// (INSERRRTTTTT EEEEEEfffffffffffffeeeeeeect)))
S TT
tt)))
SEERRRRRTTT Eff
ifff((uuNNNuuummmIIInnnppppuutttCCCCCCCChhhhhhhhaaaaaaaaannnnnnnnnnnnnnnnnnnnnnnneeeeeeelssss ======= 1 && uNNuummmmmOOOOutpppuuutChhanne
  // MM pOuuttppuutttBBBuufffffeerrr[[[11]] = pIInnnppuutttttttttBBBBBBBBBufffffffffffeeeeeer[0]*m_fVVooollluuuuummmmmeeeee;
no Inn SSStereeoo OOOut (AUUUUXXXXX EEffffect))
// MMMoonnoo--Inn, SSSteereeeooo---OOOuuutt (AAUUXXXXXXXX EEEEEEEEEEfffffffffffffffffffeeeeeeecccccccctttttttt))))))))
no Inn SSStereeeooo OOOuuutt (AUUUUUUUUUUXXXXXXXXXX EEEEEEEffffffffffffffect)))
1 && NNN OOOOO t
l
iiif((( N III
ttChhh
ttCh
ll
     Designing Audio Effect
Plug-Ins in C++

Designing Audio Effect
Plug-Ins in C++
 With Digital Audio Signal Processing Theory
   Will Pirkle
 First published 2013
by Focal Press
 70 Blanchard Road, Suite 402, Burlington, MA 01803
 Simultaneously published in the UK
by Focal Press
 2 Park Square, Milton Park, Abingdon, Oxon OX14 4RN
 Focal Press is an imprint of the Taylor and Francis Group, an Informa business
 © 2013 Taylor and Francis
 The right of Will Pirkle to be identiﬁ ed as author of this work has been asserted by him/her in accordance with
sections 77 and 78 of the Copyright, Designs and Patents Act 1988.
 All rights reserved. No part of this book may be reprinted or reproduced or utilised in any form or by any elec-
tronic, mechanical, or other means, now known or hereafter invented, including photocopying and recording, or in
any information storage or retrieval system, without permission in writing from the publishers.
  Notices
 Knowledge and best practice in this ﬁ eld are constantly changing. As new research and
 experience broaden our understanding, changes in research methods, professional  practices, or medical treatment
may become necessary.
 Practitioners and researchers must always rely on their own experience and knowledge in evaluating and using
any information, methods, compounds, or experiments described herein. In using such information or methods
they should be mindful of their own safety and the safety of others, including parties for whom they have a profes-
sional responsibility.
 Product or corporate names may be trademarks or registered trademarks, and are used only for identiﬁ cation and
explanation without intent to infringe.
 Library of Congress Cataloging-in-Publication Data
 Pirkle, William C., author.
  Designing audio effect plug-ins in C++ with digital audio signal processing theory/Will Pirkle.
       pages cm
  Includes bibliographical references and index.
  ISBN 978-0-240-82515-1 (paperback)
1. Computer sound processing. 2. Plug-ins (Computer programs)
3. C++ (Computer program language) I. Title.
  MT723.P57 2013
  006.4’5—dc23
                                                            2012024241
 ISBN: 978-0-240-82515-1 (pbk)
 ISBN: 978-0-123-97882-0 (ebk)
 Typeset in Times
 Project Managed and Typeset by: diacriTech

 Dedicated to
my father and mother
C.H. Pirkle
and
J.V. Pirkle
