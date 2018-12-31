#!/bin/bash
find .|grep -E "\.o$"|xargs rm
find .|grep -E "\.d$"|xargs rm

