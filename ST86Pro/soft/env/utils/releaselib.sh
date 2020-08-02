#!/bin/bash

echo ======== build target=========
echo "-------Dual SIM GSM only--------"
rm ./build/8853_gh021/platform/stack/lib/libstack_debug.a
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_dualsim/lib/libstack_dualsim_debug.a

rm ./build/8853_gh021/platform/stack/lib/libstack_release.a
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_dualsim/lib/libstack_dualsim_release.a

echo "-------Three SIM GSM only--------"
rm ./build/8853_gh021/platform/stack/lib/libstack_debug.a
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_threesim/lib/libstack_threesim_debug.a

rm ./build/8853_gh021/platform/stack/lib/libstack_release.a
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_threesim/lib/libstack_threesim_release.a

echo "-------Dual SIM GPRS--------"
rm ./build/8853_hk603/platform/stack/lib/libstack_debug.a
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_dualsim_gprs/lib/libstack_dualsim_gprs_debug.a

rm ./build/8853_hk603/platform/stack/lib/libstack_release.a
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_dualsim_gprs/lib/libstack_dualsim_gprs_release.a

echo "-------Three SIM GPRS--------"
rm ./build/8853_hk603/platform/stack/lib/libstack_debug.a
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_threesim_gprs/lib/libstack_threesim_gprs_debug.a

rm ./build/8853_hk603/platform/stack/lib/libstack_release.a
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_threesim_gprs/lib/libstack_threesim_gprs_release.a

echo ======== compile platform finish =========
echo "Done!!"

