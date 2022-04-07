#!/bin/bash
set -eo pipefail

BASEPATH=$(dirname "$0")

CONNX_EXE="connx-run"
TFLITE_EXE="./tflite.py"
BATCH_COUNT=${BATCH_COUNT:-100000}

RESULTS_TSV="engine_results.tsv"

TEST_PATH=$(mktemp -d)
trap 'rm -rf "$TESTS_DIR"' EXIT

check_dependencies() {
    installed_packages=$(pip list 2>/dev/null | cut -d' ' -f1)
    failed=0
    for package in {'onnx-connx','onnx','tensorflow','onnx-tf'}; do
        if ! echo "$installed_packages" | grep -q -E "^$package$"; then
            echo "Please install $package before running this script"
            failed=1
        fi
    done

    if [ $failed -eq 1 ]; then
        exit 1
    fi
}


onnx_path() {
    python -c 'import os; import onnx; print(os.path.dirname(onnx.__file__))'
}

make_tests() {
    local test_pattern="$1"

    if [ "$test_pattern" == "ALL" ]; then
        find_pattern="*"
    else
        find_pattern="$test_pattern"
    fi

    # Convert tests
    find "$(onnx_path)/backend/test/data/node" -maxdepth 1 -type d \( -name "test_${test_pattern}" -or -name "test_${find_pattern}_*" \) | while read -r test_dir; do
        test_name=$(basename "$test_dir")
        echo "Copying $test_name"

        # Copy onnx
        rsync -srtulH "${test_dir}/" "$TEST_PATH/$test_name/"

    done

    if [ "$test_pattern" == "ALL" ]; then
        # "ALL" is highly causes segfalt, So ignore it
        TEST_PATH=$TEST_PATH "$BASEPATH/convert_tests.py" >/dev/null || true
    else
        TEST_PATH=$TEST_PATH "$BASEPATH/convert_tests.py" >/dev/null
    fi
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

    for testcase in "$TEST_PATH"/test_*; do
        echo "Running testcase $testcase"
        for dataset in "$testcase"/test_data_set_*; do
            if [ ! -d "$dataset" ]; then
                continue
            fi

            test_name=$(basename "$testcase")
            dataset_name=$(basename "$dataset")

            echo "Running dataset $dataset"
            result=$($CONNX_EXE -p "$BATCH_COUNT" "$testcase/model.onnx" "$dataset"/input_*.pb 2>&1 | grep total | awk '{print $2}' || echo "Failed")

            printf "connx\t%s\t%s\t%s\n" "$test_name" "$dataset_name" "$result" | tee -a "$RESULTS_TSV"

        done
    done
}

run_tflite() {
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

            test_name=$(basename "$testcase")
            dataset_name=$(basename "$dataset")

            echo "Running dataset $dataset"
            result=$($TFLITE_EXE "$testcase" "$dataset" "$BATCH_COUNT" 2>/dev/null || echo "Failed")
            printf "tflite\t%s\t%s\t%s\n" "$test_name" "$dataset_name" "$result" | tee -a "$RESULTS_TSV"
        done
    done
}

if [ $# -ne 1 ]; then
  echo "Usage: $0 <operator>"
  echo "Example: $0 add"
  echo "To run all tests (most will failing): $0 ALL"
  exit 1
fi

operator=$1
shift

check_dependencies

make_tests "$operator"

printf "engine\ttest_name\tdataset_name\tresult(ns)\n" > "$RESULTS_TSV"

echo "Running connx"
run_connx "$@"
echo "Running tflite"
run_tflite "$@"

cat "$RESULTS_TSV"
