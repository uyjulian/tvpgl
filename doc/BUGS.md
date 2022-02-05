# Bugs

This document will document differing behavior between the different variants of tvpgl

## C

Most of the non-HDA variants of the function will set the alpha channel to 0  
May be off-by-one from the MMX variant  

## MMX

* TVPScreenBlend\_HDA\_o - G and A channels are cleared out to 0
* TVPDarkenBlend\_HDA - Off-by-one error
* TVPAlphaBlend - If source alpha channel is 0xff, it will be skipped
