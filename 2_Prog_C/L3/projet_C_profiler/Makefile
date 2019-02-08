all: profiler test_target doc_target

profiler:
	$(MAKE) -C bin

test_target:
	$(MAKE) -C test

doc_target:
	doxygen Doxyfile

clean:
	$(MAKE) -C bin clean
	$(MAKE) -C test clean
	rm -rf doc/*

mrproper: clean
	$(MAKE) -C bin mrproper
	$(MAKE) -C test mrproper

