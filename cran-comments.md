## Submission

This is a new submission. In this version I have:

* added a new functionality: HOT SAX time series dicord discovery algorithm;

* fixed few bugs with sliding window discretization;

* added citations for the papers of the original algorithm authors.

## Changes

Second release with bug fixes and new functionality added (HOT-SAX algorithm)

## Test environments

* Ubuntu LTS 13.04
* Ubuntu LTS 12.04 (on travis-ci), R 3.2.2

## R CMD check results

There were no ERRORs or WARNINGs

There was a note about using random generators:
* checking compiled code ... NOTE
File ‘jmotif/libs/jmotif.so’:
  Found ‘rand’, possibly from ‘rand’ (C)
    Object: ‘jmotif.o’
  Found ‘srand’, possibly from ‘srand’ (C)
    Object: ‘jmotif.o’

Compiled code should not call entry points which might terminate R nor
write to stdout/stderr instead of to the console, nor the system RNG.

## Downstream dependencies

stats::as.dist
