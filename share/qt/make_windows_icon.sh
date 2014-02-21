#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/GingerCoin.png
ICON_DST=../../src/qt/res/icons/GingerCoin.ico
convert ${ICON_SRC} -resize 16x16 GingerCoin-16.png
convert ${ICON_SRC} -resize 32x32 GingerCoin-32.png
convert ${ICON_SRC} -resize 48x48 GingerCoin-48.png
convert GingerCoin-16.png GingerCoin-32.png GingerCoin-48.png ${ICON_DST}

