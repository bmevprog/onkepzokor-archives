#!/bin/bash
set -e
shopt -s globstar

# Process options.
debug=false
while getopts ":d" option; do
  case $option in
    h) echo "usage: $0 [-d] [source].cpp ..."; exit ;;
    d) debug=true ;;
    ?) echo "error: option -$OPTARG is not implemented"; exit ;;
  esac
done

# Remove the options from the positional parameters.
shift $(( OPTIND - 1 ))

# Compile
gpp_opts=()
$debug && gpp_opts+=( -DLOCAL )

cpp_file="$1"
out_file="${cpp_file%.*}".out
g++ -o "${out_file}" ${gpp_opts[@]} "${cpp_file}"

# Run on all input files
dir="$(dirname "${cpp_file}")"

for in_file in "$dir"/**/*.in; do
  ans_file="${in_file%.*}".ans
  echo $in_file ">>" $ans_file
  ./"${out_file}" < "${in_file}" > "${ans_file}"
done
