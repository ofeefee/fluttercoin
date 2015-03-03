#!/bin/bash
#Script expects src to be in fluttercoin and gitian in a directory called gitian and both in the same directory
#somedir\fluttercoin
#somedir\gitian
#there is no errorchecking to see if gitian builds or not, if it works there will be two zip files in gitian directory
#with the programs in them.

major=`grep DISPLAY_VERSION_MAJOR src/version.h | awk {'print $3'} | head -n1`
minor=`grep DISPLAY_VERSION_MINOR src/version.h | awk {'print $3'} | head -n1`
revision=`grep DISPLAY_VERSION_REVISION src/version.h | awk {'print $3'} | head -n1`
build=`grep DISPLAY_VERSION_BUILD src/version.h | awk {'print $3'} | head -n1`
cd ..
rm gitian/inputs/fluttercoin-src.zip
rm gitian/fluttercoin-??-windows*
rm -r gitian/themes
zip -vr gitian/inputs/fluttercoin-src.zip fluttercoin
cd gitian
time sudo bin/gbuild ../fluttercoin/contrib/gitian-descriptors/gitian-win.yml
zip -jv fluttercoin-32-windows-v$major.$minor.$revision.$build.zip build/out/32/fluttercoind.exe build/out/32/fluttercoin-qt.exe
zip -jv fluttercoin-64-windows-v$major.$minor.$revision.$build.zip build/out/64/fluttercoind.exe build/out/64/fluttercoin-qt.exe
cp -r ../fluttercoin/src/qt/res/themes .
zip -grv fluttercoin-32-windows-v$major.$minor.$revision.$build.zip themes
zip -grv fluttercoin-64-windows-v$major.$minor.$revision.$build.zip themes
