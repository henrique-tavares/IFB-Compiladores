#!/bin/bash

make -C ./compiler

exec "$@"