.PHONY: build-tests
.PHONY: clean-tests
.PHONY: run-tests
.PHONY: tests
.PHONY: get-deps
SHELL := /bin/bash
BUILD_DIR := $(shell mktemp -d)
FILES := `ls -1 | grep .cpp | sed 's/\.cpp//'`
test: build-tests run-tests clean-tests
debug-test: build-tests debug-tests clean-tests
get-deps:
	if [ -d googletest ]; then \
		cd googletest/googletest && cmake . && make && cd -; \
	else \
		git clone --depth 1 -b release-1.8.0 https://github.com/google/googletest.git googletest && cd googletest/googletest && cmake . && make && cd -; \
	fi;
build-tests: get-deps
	echo $$BUILD_DIR; \
	for FILE in $(FILES); do\
		TEST_FILE="`echo $$FILE | sed 's/_test//'`";\
		g++ -o $(BUILD_DIR)/$$FILE $$FILE.cpp googletest/googletest/libgtest.a -I googletest/googletest/include/ -lpthread -g -Wall -I.. ../$$TEST_FILE.cpp -DEXCLUDE_QT -std=c++11; \
	done;
run-tests:
	for TEST_EXEC in $(FILES); do\
		$(BUILD_DIR)/$$TEST_EXEC; \
		EXIT_CODE="$$?";\
		if [ "$$EXIT_CODE" != "0" ]; then \
		    exit "$$EXIT_CODE"; \
		fi; \
	done;
debug-tests:
	for TEST_EXEC in $(FILES); do\
		gdb --batch -ex 'run' -ex 'bt' -ex 'quit' --return-child-result --args $(BUILD_DIR)/$$TEST_EXEC --gtest_catch_exceptions=0; \
		EXIT_CODE="$$?";\
		if [ "$$EXIT_CODE" != "0" ]; then \
		    exit "$$EXIT_CODE"; \
		fi; \
	done;
interactive-debug-test:
	if [ -z "$(FILE)" ]; then\
	    echo "make interactive-debug-test FILE=<NAME OF THE TEST TO DEBUG>";\
	else\
	    gdb --return-child-result --args $(BUILD_DIR)/$(FILE) --gtest_catch_exceptions=0;\
	fi;
clean-tests:
	rm -Rf $(BUILD_DIR)

