# Case study: elimination of false clonal variants

## Introduction

A common problem in antibody repertoire sequencing is false clonal lineages created by PCR and sequencing error. In spike-in sequencing studies, clonal lineages of up to 200 clones can be found from sequencing a single starting template.

ErrorX addresses this problem by identifying and eliminating miscalled base pairs, eliminating positions, before they can be confused for somatic variants. In a case study, we applied ErrorX to a set of barcoded sequences corrected by MAF from a public dataset (Khan, Reddy Sci Adv 2016 - SRA run # SRR3175015).

In the case below, we show a false clonal lineage of 11 variants all based on the same starting DNA molecule. We run these variants through ErrorX correction and, by eliminating the miscalled bases, reduce the lineage from 11 to 3 variant.

## Running ErrorX

All the input files needed to run this case study are in the folder `test_case` within the ErrorX distribution. Navigate to this folder with the command `cd test_case`.

The 19 sequences that all came from the same RID are shown in the file `example_cluster15.fastq`, along with their quality scores. This is all the information you need to run ErrorX prediction.

Run the following command to predict errors in this dataset:
`/path/to/ErrorX/bin/errorx --species mouse --format fastq --out cluster15_corrected.tsv --allow-nonproductive example_cluster15.fastq`

This will process the sequences and output the results to `cluster15_corrected.tsv`. Once it's done, open up `cluster15_corrected.tsv` and look at the corrected sequences. The input sequences can be found in the column `Full_NT_sequence`, and the corrected sequences are in the `Full_NT_sequence_corrected` column. 

## Analyzing results
You can now take the uncorrected and corrected sequences and visualize the difference in a multiple sequence alignment. To illustrate, below is a portion of the sequence highlighted, where ErrorX was able to successfully identify four separate cases of sequencing error.

![Uncorrected sequences](cluster_uncorrected.pdf) ![Corrected sequences](cluster_corrected.pdf)

Highlighted in red are positions where a sequence error was introduced, either by PCR error or base miscalling on the sequencer. 

In all four cases shown above, ErrorX was able to identify which bases were errors, and change them to an N to reflect that they are inaccurate.
