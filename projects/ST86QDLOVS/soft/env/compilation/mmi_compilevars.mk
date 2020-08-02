##############################################################################
# Include customvars.mk if it exists
##############################################################################
ifneq "$(wildcard $(SOFT_WORKDIR)/env/compilation/customvars.mk)" ""
include ${SOFT_WORKDIR}/env/compilation/customvars.mk
endif

#############################################################################
# Check if CT_TARGET is correctly defined and inlcude corresponding target.def
# FIXME: That section should disappear for the MMI.
#############################################################################

ifeq "$(wildcard $(SOFT_WORKDIR)/lib/target.def)" ""
    $(error "!!!! target.def not found !!!!")
else
# Include the Target definition
include ${SOFT_WORKDIR}/lib/target.def
endif

# Check if CT_ASIC is correctly defined and include corresponding ${CT_ASIC}.def
# FIXME: That section should disappear for the MMI.
#############################################################################
ifeq "$(wildcard $(SOFT_WORKDIR)/lib/$(CT_ASIC)_$(CT_ASIC_CFG).def)" ""
    $(error "!!!! No ASIC named: CT_ASIC=$(CT_ASIC)_$(CT_ASIC_CFG) !!!!")
else
# Include the ASIC definition
include ${SOFT_WORKDIR}/lib/${CT_ASIC}_${CT_ASIC_CFG}.def
endif

# Include app partition memory map 
ifeq "${APART}" "1"
-include ${SOFT_WORKDIR}/lib/app_partition_map.def
endif

#############################################################################
# Include the file which defines which modules must be used as lib.
#############################################################################
include ${SOFT_WORKDIR}/env/compilation/teamvars.mk
GLOBAL_BINARY_LIBS += ${TEAM_BINARY_LIBS}

ifeq ($(MAKELEVEL),0)
	export CT_APP := _
endif
#############################################################################
# Compile all the lists of flags (except for the local list).
#############################################################################
ALL_EXPORT_FLAG := ${GLOBAL_EXPORT_FLAG} ${TARGET_EXPORT_FLAG} ${CHIP_EXPORT_FLAG}

COMPILEVARS_DEFINED:=yes
