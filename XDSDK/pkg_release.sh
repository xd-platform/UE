#!/bin/sh

plugins=("TapBootstrap" "TapCommon" "TapDB" "TapLogin" "TapMoment" "XDGCommon" "XDGAccount" "XDGPayment" "XDGSDK" "AntiAddiction")

function DeleteBuild()
{
    rm -rf Plugins/$1/Binaries
    rm -rf Plugins/$1/Intermediate
}

for ((i=0;i<${#plugins[@]};i++));do
    DeleteBuild ${plugins[$i]}
done  

