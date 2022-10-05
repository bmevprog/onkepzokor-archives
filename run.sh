#!/bin/bash
set -e
shopt -s globstar

function compile_cpp {
  gpp_opts=()
  $debug && gpp_opts+=( -DLOCAL )

  out_file="$source_file".out
  g++ -o "$out_file" ${gpp_opts[@]} "$source_file"

  executable="./$out_file"
}

function set_py_executable {
  executable="python3 $source_file"
}

# Process options.
debug=false
while getopts ":hd" option; do
  case $option in
    h) echo "usage: $0 [-d] [source_file] ..."; exit ;;
    d) debug=true ;;
    ?) echo "error: option -$OPTARG is not implemented"; exit ;;
  esac
done

# Remove the options from the positional parameters.
shift $(( OPTIND - 1 ))

source_file="$1"
extension="${source_file#*.}"

case $extension in
  cpp) compile_cpp ;;
  py) set_py_executable ;;
  ?) echo "error: extension $extension is not recognized"; exit ;;
esac

# Run on all input files
echo $executable
dir="$(dirname "$source_file")"
for in_file in "$dir"/**/*.in; do
  ans_file="${in_file%.*}".ans
  echo $in_file ">>" $ans_file
  eval "$executable" < "${in_file}" > "${ans_file}"
done
