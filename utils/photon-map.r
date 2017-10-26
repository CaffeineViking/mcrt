#!/usr/bin/env Rscript
# Plots the Photon Map

library(rgl)

args <- commandArgs(trailingOnly=TRUE)
data <- read.csv(args[1], header=TRUE)
bg3d(color=c("darkgrey", "darkgrey"))
with(data, plot3d(x, z, y, col=rgb(r, g, b, 1,
                  maxColorValue=1.0),
                  size=4, alpha=0.7))
while (rgl.cur() > 0) Sys.sleep(0.5)
