#!/usr/bin/env bash
set -e # Exit immediately on error.
set -x # Display expanded commands as they are executed.

. build_lib

getAppName || exit -1
getQtVersion || exit $?
getAppVersion || exit $?
modifyApplicationVersionName || exit $?

#if jenkins env. variables are present, use them as build info for the applications about box
if [ ! -z $BUILD_ID ] && [ ! -z $BUILD_NUMBER ]; then
	setBuildString `isoDateTime $BUILD_ID` $(returnGitHash) $BUILD_NUMBER
else
	setBuildString "dev" $(returnGitHash)
fi

#use an alternate debug level if specified
if [ ! -z $DLDEBUGLEVEL ]; then
	setDebugLevel $DLDEBUGLEVEL
fi

platform=`uname | tr '[:upper:]' '[:lower:]'`
case $platform in
	#section for mac os x
	darwin* )
	CONFIGURATION=release-mac
	if [ ! -z $APPNAME ]; then
		TARGET=$CONFIGURATION/$APPNAME.app
	fi
	#compile
	QTDIR="/Users/mshaffer/Qt/5.5/clang_64"
	${QTDIR}/bin/qmake -r -spec macx-g++ CONFIG+=release
	# macdeployqt doesn't like it if you build an app twice, so delete the last one:
	rm -rf "release-mac/$APPNAME.app"
	make clean
	make || exit -1
	#make package
	${QTDIR}/bin/macdeployqt "$TARGET" -no-strip || exit -1
	cp -R 'MacInstaller/Kext Installer/Install Drivers.pkg' $TARGET/Contents/Resources || exit -1
	find $TARGET -exec touch -t `date '+%Y%m%d%H%M.%S'` {} \; || exit -1
	buildDMG || exit -1
	;;
	* )
	echo "Unrecognized OS Type. Try again."
	exit 1
	;;
esac
