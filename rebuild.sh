#!/bin/bash
            cd build/;
            rm -rf *;
            cmake -S ../ -B ./;
            make