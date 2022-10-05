#!/bin/bash
set -ex
shopt -s globstar

function process_params {
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
  shift $((OPTIND-1))

  source_file="$1"
  extension="${source_file#*.}"
}

function compile_cpp {
  gpp_opts=()
  $debug && gpp_opts+=( -DLOCAL )

  out_file="$source_file".out
  g++ -o "$out_file" ${gpp_opts[@]} "$source_file"
}

function compile {
  case $extension in
    cpp) compile_cpp ;;
    py) true ;;
    ?) echo "error: extension $extension is not recognized"; exit ;;
  esac
}

function evaluate_inputs {
  dir="$(dirname "$source_file")"
  for in_file in "$dir"/**/*.in; do
    ans_file="${in_file%.*}".ans
    echo $in_file ">>" $ans_file
    case $extension in
      cpp) ./"$out_file" < "$in_file" > "$ans_file" 2>&1 ;;
      py) python3 $source_file < "$in_file" > "$ans_file" 2>&1 ;;
    esac
  done
}

process_params $@
compile
evaluate_inputs
