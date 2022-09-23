#!/bin/bash
#export LANG="en_US.UTF-8"
export LANG="C"

PROJECT_DIR=$(pwd)

doBuild() {
    export LANG="C"
    sh /Users/Shared/Epic\ Games/UE_$1/Engine/Build/BatchFiles/RunUAT.sh BuildCookRun -project=$2 -package -ue4exe=/Users/Shared/Epic\ Games/UE_$1/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor -targetplatform=$3 -cookflavor=ETC2 -stage -archive -archivedirectory=$4 -clientconfig=Development -distribution -target=XDSDK -build -nocompile -pak -cook -allmaps -noP4 -clean -uft8output
    export LANG="en_US.UTF-8"
}

cleanPlugin() {
    cd $1
    rm -rf Binaries
    rm -rf Intermediate
    cd ..
    cd ..
}

java -jar ./.ci/release.jar message --title="${CI_PROJECT_TITLE} build " --body="<${CI_JOB_URL}|Package Start>"

PRODUCT_DIR=./Product

mkdir -p $PRODUCT_DIR

cd $PRODUCT_DIR

cd ..

doBuild 4.26 $PROJECT_DIR/XDSDK/XDSDK.uproject IOS $PROJECT_DIR/Product

doBuild 4.26 $PROJECT_DIR/XDSDK/XDSDK.uproject Android $PROJECT_DIR/Product

resultContent=$(java -jar ./.ci/release.jar message --title="${CI_PROJECT_TITLE} build " --body="<${CI_JOB_URL}|Package Start>")
echo $resultContent
THREAD=${resultContent#*threadTs=}
echo $THREAD

 sh ./pkg_uploader.sh $PRODUCT_DIR/IOS/Distro_XDSDK.ipa com.xd.sdkdemo1 $THREAD
 
 sh ./pkg_uploader.sh $PRODUCT_DIR/Android_ETC2/XDSDK-armv7.apk com.tds.sdkdemopro $THREAD

