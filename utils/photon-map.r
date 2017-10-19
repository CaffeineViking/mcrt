#!/usr/bin/env Rscript
# Plots the Photon Map

library(rgl)

args <- commandArgs(trailingOnly=TRUE)
data <- read.csv(args[1], header=TRUE)
with(data, plot3d(x, y, z, col=rgb(r, g, b, 1,
                  maxColorValue=1.0)))
while (rgl.cur() > 0) Sys.sleep(1)
