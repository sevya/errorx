# ErrorX DNA correction software

ErrorX is a revolutionary software for correction of DNA sequencing errors. In next-generation sequencing, it's common that many of the output sequences are actually errors, different from the input DNA sequence. This can be caused by the way the DNA was prepared, amplification before sequencing, and errors introduced by the sequencing machine itself. Unfortunately, these errors can be very costly to a next-generation sequencing pipeline, as you can never be sure which sequences are true and which are mistakes.

ErrorX solves this problem by using deep neural networks to predict sites where an error has been introduced. Trained on a dataset of tens of millions of sequences, ErrorX can pinpoint with extremely high accuracy positions where an error was likely introduced, saving valuable time and energy by removing these bases from further analysis. In benchmark studies, ErrorX was able to identify errors with an accuracy of 99.9% and a false positive rate of only 0.1%. 

ErrorX currently supports processing of antibody repertoire sequencing data from human and mice, gathered on Illumina HiSeq and MiSeq instruments.

To install ErrorX, simply extract the `tar.gz` archive, which contains binaries for running the application and libraries for building the ErrorX platform into a C++, Python, or Java application.


