# Makefile commun. Usage dans examples/<module>/<nom>/Makefile :
#   include ../../common/example.mk
# Variables : CSFML=3.0|2.6 (défaut 3.0), CSFML_PREFIX (défaut ~/csfml-$(CSFML), "system" = paquet système).
CSFML ?= 3.0
CSFML_PREFIX ?= $(HOME)/csfml-$(CSFML)
COMMON := $(dir $(lastword $(MAKEFILE_LIST)))
NAME := example
BUILD := build/$(CSFML)
CC ?= gcc
CFLAGS += -std=c11 -Wall -Wextra -Werror -I$(COMMON)
# CSFML 2.6 : le header Event.h est marqué deprecated (sfMouseWheelEvent) -> avertissement même sans l'utiliser.
ifeq ($(CSFML),2.6)
CFLAGS += -Wno-error=deprecated-declarations
endif
ifneq ($(CSFML_PREFIX),system)
CFLAGS += -I$(CSFML_PREFIX)/include
LDFLAGS += -L$(CSFML_PREFIX)/lib -Wl,-rpath,$(CSFML_PREFIX)/lib
endif
LDLIBS += -lcsfml-graphics -lcsfml-window -lcsfml-audio -lcsfml-network -lcsfml-system -lm

all: $(BUILD)/$(NAME)
$(BUILD)/$(NAME): $(CSFML)/main.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)
clean:
	rm -rf build
re: clean all
.PHONY: all clean re
