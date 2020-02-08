## Submission
This is a maintenance release fixing warnings with libraries deprecation. 
Compiled using
CXX11FLAGS = -Wdeprecated-declarations -Wsign-compare -pedantic
No warnings were thrown.

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
