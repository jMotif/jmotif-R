## Resubmission

This is a re-submission of todays's rejected submission. In this version I have:

* cleaned "RcppArmadillo" reference in the DESCRIPTION "Depends"

This also fixes a previous submission error:

* switched from system's RNG to the Armadillo's implementation.

## Changes

Second release with bug fixes and new functionality added (HOT-SAX algorithm).

## Test environments

* Ubuntu LTS 13.04
* Ubuntu LTS 12.04 (on travis-ci), R 3.2.2

## R CMD check results

There were no ERRORs or WARNINGs

## Downstream dependencies

`stats::as.dist`
