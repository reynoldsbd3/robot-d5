

all:
	$(MAKE) -f EWL_C.$(VENDOR).mak -C EWL_C
	$(MAKE) -f EWL_C++.$(VENDOR).mak -C EWL_C++
	$(MAKE) -f EWL_Runtime.$(VENDOR).mak -C EWL_Runtime

cleanobj:
	$(MAKE) -f EWL_C.$(VENDOR).mak -C EWL_C cleanobj 
	$(MAKE) -f EWL_C++.$(VENDOR).mak -C EWL_C++ cleanobj
	$(MAKE) -f EWL_Runtime.$(VENDOR).mak  -C EWL_Runtime cleanobj

clean:
	$(MAKE) -f EWL_C.$(VENDOR).mak -C EWL_C clean 
	$(MAKE) -f EWL_C++.$(VENDOR).mak -C EWL_C++ clean
	$(MAKE) -f EWL_Runtime.$(VENDOR).mak  -C EWL_Runtime clean
	@rm -fr doc

doc:
	doxygen Doxyfile