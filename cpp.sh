#!/bin/bash

set -e

debug=false

while getopts ":d" option; do
  case $option in
    h) echo "usage: $0 [-d] [source].cpp ..."; exit ;;
    d) debug=true ;;
    ?) echo "error: option -$OPTARG is not implemented"; exit ;;
  esac
done

gpp_opts=()
$debug && gpp_opts+=( -DLOCAL )

# remove the options from the positional parameters
shift $(( OPTIND - 1 ))

cpp_file="$1"

file="${cpp_file%.*}"

in_file="$file".in
out_file="$file".out
exec_file="$file"

set -x

g++ -o ${exec_file} ${gpp_opts[@]} "${cpp_file}"
./${exec_file} < ${in_file} | tee ${out_file}
