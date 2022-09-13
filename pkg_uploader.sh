#!/bin/sh

IPA_PATH=$1

BUNDLE_ID=$2

THREAD_TS=$3

MASK_ID=""

isiOS() {
  if [ "${IPA_PATH##*.}"x = "ipa"x ];then
      return 0
  else
      return 1
  fi
}

FileName=${IPA_PATH##*/}

MASK_ID=`python3 ./upload_pkg.py $IPA_PATH $FileName`

sleep 10

echo $MASK_ID

Android_Url=""

iOS_Url=""

if isiOS ; then
  iOS_Url="https://npkg.xindong.com/install/${MASK_ID}"
  echo $iOS_Url
  if [ ! -d ".Products/demo.ipa" ]; then
      java -jar ./.ci/release.jar message --title="${CI_PROJECT_TITLE} build iOS Success" --body="<${iOS_Url}|iOS 下载地址>" --thread $THREAD_TS
  else
      java -jar ./.ci/release.jar message --title="${CI_PROJECT_TITLE} build iOS Failed" --thread $THREAD_TS
  fi
else
  Android_Url="https://npkg.xindong.com/install/${MASK_ID}"
  if [ ! -d ".Products/TapSDK2-Unity.apk" ]; then
      java -jar ./.ci/release.jar message --title="${CI_PROJECT_TITLE} build Android Success" --body="<${Android_Url}|Android 下载地址>" --thread $THREAD_TS
  else
      java -jar ./.ci/release.jar message --title="${CI_PROJECT_TITLE} build Android Failed" --thread $THREAD_TS
  fi
fi

