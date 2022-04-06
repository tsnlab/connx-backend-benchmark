#!/bin/bash
set -eo pipefail

BASEPATH=$(dirname "$0")

CONNX_EXE="connx-run"
TFLITE_EXE="./tflite.py"
BATCH_COUNT=${BATCH_COUNT:-100000}

RESULTS_PATH="engine_results"

TEST_PATH=$(mktemp -d)
trap 'rm -rf "$TESTS_DIR"' EXIT


onnx_path() {
    python -c 'import os; import onnx; print(os.path.dirname(onnx.__file__))'
}

make_tests() {
    local test_pattern="$1"

    # Convert tests
    find "$(onnx_path)/backend/test/data/node" -maxdepth 1 -type d \( -name "test_${test_pattern}" -or -name "test_${test_pattern}_*" \) | while read -r test_dir; do
        test_name=$(basename "$test_dir")
        echo "Copying $test_name"

        # Copy onnx
        rsync -srtulH "${test_dir}/" "$TEST_PATH/$test_name/"

    done

    TEST_PATH=$TEST_PATH "$BASEPATH/convert_tests.py" >/dev/null
}

filename_for_dataset() {
  local p=$1
  p="${p##"${TEST_PATH}"/}"
  echo "${p//\//_}"
}


run_connx() {
    # Check connx file exists and executable
    if ! command -v $CONNX_EXE >/dev/null 2>&1; then
        echo "connx executable not found"
        exit 1
    fi

    mkdir -p "$RESULTS_PATH"

    for testcase in "$TEST_PATH"/test_*; do
        echo "Running testcase $testcase"
        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
                continue
            fi


            echo "Running dataset $dataset"
            fname=$(filename_for_dataset "${dataset}")
            $CONNX_EXE -p "$BATCH_COUNT" "$testcase/model.onnx" "$dataset"/input_*.pb 2>&1 | grep total | awk "{print \"$BATCH_COUNT invocations took \" \$2 \" ns\"}" | tee "$RESULTS_PATH/connx_$fname"
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

        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
              continue
            fi

            echo "Running dataset $dataset"
            fname=$(filename_for_dataset "${dataset}")
            $TFLITE_EXE "$testcase" "$dataset" "$BATCH_COUNT" 2>/dev/null | tee "$RESULTS_PATH/tflite_$fname" || echo "TFLite failed"
        done
    done
}

if [ $# -ne 1 ]; then
  echo "Usage: $0 <operator>"
  exit 1
fi

operator=$1
shift

make_tests "$operator"

echo "Running connx"
run_connx "$@"
echo "Running tflite"
run_tflite "$@"
