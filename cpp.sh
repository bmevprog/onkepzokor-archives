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
ans_file="$file".ans
out_file="$file".out

set -x

g++ -o ${out_file} ${gpp_opts[@]} "${cpp_file}"
./${out_file} < ${in_file} | tee ${ans_file}
