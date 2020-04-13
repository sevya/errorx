# ErrorX

[![GitHub Release](https://img.shields.io/github/release/sevya/errorx.svg)](https://github.com/sevya/errorx/releases/latest)  
[![Build Status](https://travis-ci.com/sevya/errorx.svg?branch=master)](https://travis-ci.com/sevya/errorx)
[![Documentation Status](https://readthedocs.org/projects/errorx/badge/?version=latest)](https://errorx.readthedocs.io/en/latest/?badge=latest)
[![Twitter Follow](https://img.shields.io/twitter/follow/EndeavorBio.svg?style=social)](https://twitter.com/EndeavorBio)  

ErrorX is a software package for correction of DNA sequencing errors. In next-generation sequencing, it's common that many of the output sequences are actually errors, different from the input DNA sequence. This can be caused by the way the DNA was prepared, amplification before sequencing, and errors introduced by the sequencing machine itself. Unfortunately, these errors can be very costly to a next-generation sequencing pipeline, as you can never be sure which sequences are true and which are mistakes.

ErrorX solves this problem by using deep neural networks to predict sites where an error has been introduced. Trained on a dataset of tens of millions of sequences, ErrorX can pinpoint with extremely high accuracy positions where an error was likely introduced, saving valuable time and energy by removing these bases from further analysis. In benchmark studies, ErrorX was able to identify errors with an accuracy of 99.9% and a false positive rate of only 0.1%.

ErrorX currently supports processing of antibody and T-cell repertoire sequencing data from human and mice, gathered on Illumina HiSeq and MiSeq instruments.

For more information on how the algorithm works, check out the ErrorX manuscript ([Sevy bioRxiv 2020](https://www.biorxiv.org/content/10.1101/2020.02.17.952408v2)).
See the [documentation page](https://errorx.readthedocs.io) for more information on how to set up error correction.

Copyright (c) 2019-2020, Alex Sevy. All Rights Reserved. For commercial inquiries contact us at alex@endeavorbio.com.
