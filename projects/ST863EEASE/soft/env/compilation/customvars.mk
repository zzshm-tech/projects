## Extra flags exported to all makefiles.
GLOBAL_EXPORT_FLAG += 

CT_PRODUCT ?= ST86

GLOBAL_EXPORT_FLAG += CT_PRODUCT=${CT_PRODUCT}
-include ${SOFT_WORKDIR}/${CT_PRODUCT}.mk

## This will define the default target for which you will compile,
## if you don't specify a CT_TARGET on the command line.
CT_TARGET := sl_app

## This defines the release mode in which you compile.
## release  for product  
## debug for development and test
CT_RELEASE ?= release

## Genernate SVN version information
## 0 : no SVN tool
## 1 : Using SVN version manager tool
WITH_SVN :=0

## Used for select stack compile 
## ADMIN : compile the stack source code
## FAE   : not compile stack source code, just using stack lib directly
CT_USER :=FAE


CT_MODEM := 1

ifeq "${TTS}" "1"
COMPRESS = 1
endif

ifeq "${BLUETOOTH}" "1"
SL_PLATFORM=ST87
endif

MODE = api

ifeq "${APART}" "1"
CFLAGS += -DSL_APART=1

APP_LOD_GEN_SCRIPT_PATH=${SOFT_WORKDIR}/env/utils/
APP_LOD_GEN_SCRIPT=gen_lod.pl
else
CFLAGS += -DSL_APART=0
endif
