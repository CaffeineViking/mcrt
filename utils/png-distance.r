#!/usr/bin/env Rscript
# Absolute Difference.

library(png)

args <- commandArgs(trailingOnly=TRUE)
first <- readPNG(args[1],native=FALSE)
other <- readPNG(args[2],native=FALSE)
diff <- abs(other - first)
writePNG(diff, args[3])
