# tests/Makefile — Build and run PuttyAlt test suites
#
# Usage:
#   make            — build all test binaries
#   make test       — build and run all tests
#   make clean      — remove binaries

CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -std=c99 -g -I..
LDFLAGS ?= -lm

SRCDIR  = ..
TESTS   = test_modules test_security test_terminal test_network \
          test_workflow test_automation test_ui_components \
          test_session_mgmt test_config

# Source files needed for linking (module implementations)
SECURITY_SRCS = $(SRCDIR)/puttyalt_credstore.c \
                $(SRCDIR)/puttyalt_ratelimit.c \
                $(SRCDIR)/puttyalt_inputfilter.c \
                $(SRCDIR)/puttyalt_hostverify.c

TERMINAL_SRCS = $(SRCDIR)/puttyalt_termemu.c \
                $(SRCDIR)/puttyalt_ansiparse.c \
                $(SRCDIR)/puttyalt_selection.c \
                $(SRCDIR)/puttyalt_scrollstate.c

NETWORK_SRCS  = $(SRCDIR)/puttyalt_keepalive.c \
                $(SRCDIR)/puttyalt_proxychain.c \
                $(SRCDIR)/puttyalt_connpool.c \
                $(SRCDIR)/puttyalt_tunnelmon.c

.PHONY: all test clean

all: $(TESTS)

test_modules: test_modules.c
	$(CC) $(CFLAGS) -c -o $@.o $<

test_workflow: test_workflow.c
	$(CC) $(CFLAGS) -c -o $@.o $<

test_automation: test_automation.c
	$(CC) $(CFLAGS) -c -o $@.o $<

test_ui_components: test_ui_components.c
	$(CC) $(CFLAGS) -c -o $@.o $<

test_session_mgmt: test_session_mgmt.c
	$(CC) $(CFLAGS) -c -o $@.o $<

test_config: test_config.c
	$(CC) $(CFLAGS) -c -o $@.o $<

test_security: test_security.c $(SECURITY_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_terminal: test_terminal.c $(TERMINAL_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_network: test_network.c $(NETWORK_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: all
	@echo "=== Running test suites ==="
	@for t in test_security test_terminal test_network; do \
		echo ""; \
		echo "--- $$t ---"; \
		./$$t || exit 1; \
	done
	@echo ""
	@echo "=== All tests passed ==="

clean:
	rm -f $(TESTS) *.o
