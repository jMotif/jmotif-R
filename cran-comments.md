## Submission
This yet another C++11 fix, fixing clang warning: 
  Result: WARN
    Found the following significant warnings:
     znorm.cpp:44:59: warning: 'bind2nd<std::__1::minus<double>, double>' is deprecated [-Wdeprecated-declarations]

The old code:
std::transform(ts.begin(), ts.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));

The new code:
 for(unsigned i=0; i<ts.size(); i++)
    diff[i] = ts[i]-mean;

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
