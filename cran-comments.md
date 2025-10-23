## Submission
This is addressing project deletion following your email.
===
Please see the problems shown on
<https://cran.r-project.org/web/checks/check_results_jmotif.html>.
Please correct before 2025-11-15 to safely retain your package on CRAN.
Note that this will be the *final* reminder.
===

I've fixed RcppArmadillo-related issues by removing hard requirement for C++ 11.

Some bugs popped up and I fixed those as well.

Tests pass and checks as well.

Have a great day! Thank you!


## Changes

Build routines and some sources.

## Test environments
* Ubuntu LTS 22.04.05 (localhost), R 4.5.1
* win-builder (devel and release)
* r-actions -- ubuntu-, macos-, windows- latest 

## R CMD check results

There were no ERRORs or WARNINGs, one NOTE

## Downstream dependencies

`stats::as.dist`
