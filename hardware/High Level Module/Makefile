SVFILES = vga.sv

lint :
	for file in $(SVFILES); do \
	verilator --lint-only -Wall $$file; done


