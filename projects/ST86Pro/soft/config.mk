MAKE_CONFIG ?= Make.config
ifeq "$(wildcard $(MAKE_CONFIG))" ""
config: $(MAKE_CONFIG)

$(MAKE_CONFIG):
	@echo "export SOFT_WORKDIR=$(shell pwd)" > $(MAKE_CONFIG)
	@echo "include $$""(SOFT_WORKDIR)/env/compilation/mmi_compilevars.mk" >> $(MAKE_CONFIG)
	@echo >> $(MAKE_CONFIG)
	@echo "Configure Done!"
	@echo "All rights reserved by SimpLight Nanoelectronics, Ltd."

else
endif

