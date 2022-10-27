#!/bin/bash
set -e
shopt -s globstar

function print_usage {
  echo "usage: $0 [-d] <source_file.(py|cpp)>";
  echo
  echo "eval.sh evaluates the code in source_file for all .in"
  echo "files in its directory (recursively) and saves the output"
  echo "in the corresponding .ans file on the same path."
  echo
  echo "  -d  turn on debug-mode (adds -g and -DLOCAL flags during cpp compilation)"
}

function process_params {
  # Process options.
  debug=false
  while getopts ":dh" option; do
    case $option in
      d) debug=true ;;
      h) print_usage; exit ;;
      ?) echo "unknown option: -$OPTARG"; echo; print_usage; exit ;;
    esac
  done

  # Remove the options from the positional parameters.
  shift $((OPTIND-1))

  source_file="$1"
  extension="${source_file#*.}"
}

function compile_cpp {
  gpp_opts=()
  $debug && gpp_opts+=( -g -DLOCAL )

  out_file="$source_file".out
  g++ -o "$out_file" ${gpp_opts[@]} "$source_file"
}

function compile {
  case $extension in
    ("cpp") compile_cpp ;;
    ("py") echo python ;;
    (*) echo "error: extension .$extension is not recognized"; exit ;;
  esac
}

function evaluate_inputs {
  dir="$(dirname "$source_file")"
  for in_file in "$dir"/**/*.in; do
    ans_file="${in_file%.*}".ans
    echo $in_file ">>" $ans_file
    case $extension in
      ("cpp") ./"$out_file" < "$in_file" > "$ans_file" 2>&1 ;;
      ("py") python3 $source_file < "$in_file" > "$ans_file" 2>&1 ;;
    esac
  done
}

process_params $@
compile
evaluate_inputs
