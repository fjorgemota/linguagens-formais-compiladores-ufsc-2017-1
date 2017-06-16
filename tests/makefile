.PHONY: build-tests
.PHONY: clean-tests
.PHONY: run-tests
.PHONY: tests
.PHONY: get-deps
SHELL := /bin/bash
BUILD_DIR := $(shell mktemp -d /tmp/tmp.XXXXXXXXX)
FILES := `ls -1 | grep .cpp | grep -v gcov | sed 's/\.cpp//'`
DEPS_PATH := deps
COV ?= gcov
CXX ?= g++
GTEST_PATH := $(DEPS_PATH)/googletest
LCOV_PATH := $(shell which lcov)
test: build-tests run-tests clean-tests
debug-test: build-tests debug-tests clean-tests
get-deps: 
	mkdir -p $(DEPS_PATH); \
	if [ ! -d "$(GTEST_PATH)" ]; then \
		git clone --depth 1 -b release-1.8.0 https://github.com/google/googletest.git $(GTEST_PATH) && cd $(GTEST_PATH)/googletest && cmake . && make && cd -; \
	fi;
build-tests: get-deps
	for FILE in $(FILES); do\
		TEST_FILE="`echo $$FILE | sed 's/_test//'`";\
		$(CXX) -o $(BUILD_DIR)/$$FILE $$FILE.cpp $(GTEST_PATH)/googletest/libgtest.a -I $(GTEST_PATH)/googletest/include/ -lpthread -g -Wall -I.. ../$$TEST_FILE.cpp -DEXCLUDE_QT -std=c++11  -fprofile-arcs -ftest-coverage; \
	done;
run-tests:
	mkdir -p ../coverage; \
	for TEST_EXEC in $(FILES); do\
		$(BUILD_DIR)/$$TEST_EXEC; \
		EXIT_CODE="$$?";\
		if [ "$$EXIT_CODE" == "0" ]; then \
		    TEST_FILE="`echo $$TEST_EXEC | sed 's/_test//'`"; \
		    $(COV) $$TEST_FILE.cpp; \
			if [ -n "$(LCOV_PATH)" ]; then\
			    $(LCOV_PATH) --capture --directory . --output-file ../coverage/lcov.info;\
			fi;\
		else \
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
	rm -Rf $(BUILD_DIR) *.gcov *.gcda *.gcno
