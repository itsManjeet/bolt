#!/bin/sh

echo "" > training.txt
for i in plugins/*/training.txt ; do
    cat ${i} >> training.txt
done