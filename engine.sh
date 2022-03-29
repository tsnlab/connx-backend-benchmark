#!/bin/bash
set -eo pipefail

CONNX_EXE="./connx"
TFLITE_EXE="./tflite.py"
TEST_PATH="converted_tests"
BATCH_COUNT=1000000

declare -A BATCH_COUNT_OVERRIDES

BATCH_COUNT_OVERRIDES["test_maxpool_2d_default"]=$((BATCH_COUNT / 100))
BATCH_COUNT_OVERRIDES["test_maxpool_2d_pads"]=$((BATCH_COUNT / 100))
BATCH_COUNT_OVERRIDES["test_maxpool_2d_same_lower"]=$((BATCH_COUNT / 100))
BATCH_COUNT_OVERRIDES["test_maxpool_2d_same_upper"]=$((BATCH_COUNT / 100))
BATCH_COUNT_OVERRIDES["test_maxpool_2d_strides"]=$((BATCH_COUNT / 100))
BATCH_COUNT_OVERRIDES["test_maxpool_3d_default"]=$((BATCH_COUNT / 1000))
BATCH_COUNT_OVERRIDES["test_maxpool_3d_pads"]=$((BATCH_COUNT / 1000))
BATCH_COUNT_OVERRIDES["test_maxpool_3d_same_lower"]=$((BATCH_COUNT / 1000))
BATCH_COUNT_OVERRIDES["test_maxpool_3d_same_upper"]=$((BATCH_COUNT / 1000))
BATCH_COUNT_OVERRIDES["test_maxpool_3d_strides"]=$((BATCH_COUNT / 1000))

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

    for testcase in "$TEST_PATH"/test_*; do
        echo "Running testcase $testcase"
        batch_count=${BATCH_COUNT_OVERRIDES[$(basename "$testcase")]:-$BATCH_COUNT}
        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
                continue
            fi


            echo "Running dataset $dataset"
            fname=$(filename_for_dataset "${dataset}")
            $CONNX_EXE "$testcase" "$dataset"/input_*.data -p $batch_count > "$RESULTS_PATH/connx_$fname"
        done
    done
}

run_tflite() {
    mkdir -p "$RESULTS_PATH"

    for testcase in "$TEST_PATH"/test_*; do
        echo "Running testcase $testcase"
        if [ ! -e "$testcase/model.tflite" ]; then
            echo "No tflite model found for $testcase"
            continue
        fi

        batch_count=${BATCH_COUNT_OVERRIDES[$(basename "$testcase")]:-$BATCH_COUNT}

        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
              continue
            fi

            echo "Running dataset $dataset"
            fname=$(filename_for_dataset "${dataset}")
            $TFLITE_EXE "$testcase" "$dataset" $batch_count 2>/dev/null | tee "$RESULTS_PATH/tflite_$fname" || echo "TFLite failed"
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
