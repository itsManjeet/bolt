#!/bin/sh

echo "" > training.txt
for i in plugins/*/sentences.list ; do
    # shellcheck disable=SC2046
    echo $(basename $(dirname $i)) >> training.txt
    cat $i | sed 's#^#- #g' >> training.txt
    echo >> training.txt
done
if [[ -e data/responses.txt ]] ; then
  echo "%%" >> training.txt
  cat data/responses.txt >> training.txt
  echo >> training.txt
fi