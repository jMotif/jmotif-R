## Submission
This is a maintenance release fixing warnings with libraries deprecation. 
Compiled using
CXX11FLAGS = -Wdeprecated-declarations -Wsign-compare -pedantic
No warnings were thrown.

The check_rhub() yielded two notes:


* checking CRAN incoming feasibility ... NOTE
Maintainer: 'Pavel Senin <seninp@gmail.com>'

New submission

Package was archived on CRAN

Possibly mis-spelled words in DESCRIPTION:
  RRA (6:94)
  RePair (6:82)
  VSM (6:68, 6:77)
  interpretable (8:53)

CRAN repository db overrides:
  X-CRAN-Comment: Archived on 2019-12-01 as installation warnings were
    not addressed despite reminders.
* checking for non-standard things in the check directory ... NOTE
Found the following files/directories:
  'examples_i386' 'examples_x64' 'jmotif-Ex_i386.Rout'
  'jmotif-Ex_x64.Rout' 'tests_i386' 'tests_x64'

## Changes

All over source code in order to fix any thrown warnings.

## Test environments
* Ubuntu LTS 19.10 (localhost), R 3.6.1
* Ubuntu LTS 16.04 (on travis-ci), R 3.5
* win-builder (devel and release)

## R CMD check results

There were no ERRORs or WARNINGs

## Downstream dependencies

`stats::as.dist`
