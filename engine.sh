#!/bin/bash
set -eo pipefail

CONNX_EXE="./connx"
TFLITE_EXE="./tflite.py"
TEST_PATH="converted_tests"
BATCH_COUNT=1000000

RESULTS_PATH="engine_results"

filename_for_dataset() {
  local p=$1
  p="${p##${TEST_PATH}/}"
  echo "${p//\//_}"
}


run_connx() {
    # Check connx file exists and executable
    if [ ! -x "$CONNX_EXE" ]; then
        echo "connx executable not found"
        exit 1
    fi

    mkdir -p "$RESULTS_PATH"

    for testcase in "$TEST_PATH"/*; do
        echo "Running testcase $testcase"
        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
                continue
            fi

            echo "Running dataset $dataset"
            fname=$(filename_for_dataset "${dataset}")
            $CONNX_EXE "$testcase" "$dataset"/input_*.data -p $BATCH_COUNT > "$RESULTS_PATH/connx_$fname"
        done
    done
}

run_tflite() {
    mkdir -p "$RESULTS_PATH"

    for testcase in "$TEST_PATH"/*; do
        echo "Running testcase $testcase"
        if [ ! -e "$testcase/model.tflite" ]; then
            echo "No tflite model found for $testcase"
            continue
        fi

        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
              continue
            fi

            echo "Running dataset $dataset"
            fname=$(filename_for_dataset "${dataset}")
            $TFLITE_EXE "$testcase" "$dataset" $BATCH_COUNT 2>/dev/null | tee "$RESULTS_PATH/tflite_$fname" || echo "TFLite failed"
        done
    done
}

if [ $# -ne 1 ]; then
  echo "Usage: $0 <engine>"
  exit 1
fi

engine=$1
shift

case $engine in
  "connx")
      run_connx "$@"
    ;;
  "tflite")
      run_tflite "$@"
    ;;
  *)
    echo "Unknown engine: $engine"
    exit 1
    ;;
esac
