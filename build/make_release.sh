#! /bin/bash

# MAKE ALL
# make_release.sh
#
# MAKE JUST ONE
# make_release.sh atom
# or
# make_release.sh 0

# Color-echo.
# arg $1 = message
# arg $2 = Color
cecho() {
  echo -e "${2}${1}"
  tput sgr0
  # Reset # Reset to normal.
  return
}
# Set the colours you can use
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'


BUILD_DIR=$(cd "$(dirname "$0")"; pwd)
if [ -z $1 ]; then
	MAKE_RELEASE_PLATFORM="all"
else
	MAKE_RELEASE_PLATFORM="$1"
fi

cecho "attempting to release $MAKE_RELEASE_PLATFORM" $yellow 

if [ $MAKE_RELEASE_PLATFORM == "atom" ] || [ $MAKE_RELEASE_PLATFORM == "0" ]; then
	./release.sh 0 atom
elif [ $MAKE_RELEASE_PLATFORM == "neutron" ] || [ $MAKE_RELEASE_PLATFORM == "1" ]; then
	./release.sh 1 neutron
elif [ $MAKE_RELEASE_PLATFORM == "electron" ] || [ $MAKE_RELEASE_PLATFORM == "4" ]; then
	./release.sh 4 electron
elif [ $MAKE_RELEASE_PLATFORM == "all" ]; then
	./release.sh 0 atom
	./release.sh 1 neutron
	#./release.sh 4 electron
elif [ $MAKE_RELEASE_PLATFORM == "clean" ]; then
    cecho "Clean the intorobot-online dir" $yellow
    rm -rf $BUILD_DIR/release/intorobot-online/atom/*
    rm -rf $BUILD_DIR/release/intorobot-online/neutron/*
    rm -rf $BUILD_DIR/release/intorobot-online.tar.gz
    exit 0
else
	echo "ERROR, $MAKE_RELEASE_PLATFORM not valid!!" $red
    exit 1
fi

# cp makeflies
#cp $BUILD_DIR/makefile_release $BUILD_DIR/release/intorobot-online/makefile

# make release server pack
cd $BUILD_DIR/release
tar -czf intorobot-online.tar.gz intorobot-online
#tar -czvf intorobot-online.tar.gz intorobot-online

if [[ $2 == "publish" ]]; then
    # publish to the cloud
    cecho "Please select the number which cloud platform to publish:"
    select yn in "no" "intoyun" "intorobot"; do
          case $yn in
            no )
                cecho "Not publish to any cloud platform!! " $blue
                break;;
            intoyun )
                cecho "Publish to the intoyun platform" $blue
                scp $BUILD_DIR/release/intorobot-online.tar.gz root@112.124.117.64:/tmp/
                ssh root@112.124.117.64
                break;;
            intorobot )
                cecho "Publish to the intorobot platform " $blue
                scp $BUILD_DIR/release/intorobot-online.tar.gz root@115.29.193.81:/tmp/
                cecho "Tell zhongjin upload！" $yellow
                break;;
          esac
    done
fi

cecho "All Done!!!!!" $green
exit 0



