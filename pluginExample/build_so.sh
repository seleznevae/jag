#!/bin/bash

gcc plugin.c -shared -fPIC -Wall -Wextra -Werror -lpthread -o plugin.so
