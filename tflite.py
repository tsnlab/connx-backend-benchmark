#!/usr/bin/env -S python3 -u

import decimal
import glob
import sys
from os import path
import timeit

import onnx
from onnx import numpy_helper
import tensorflow as tf


def load_tensor(fname):
    tensor = onnx.TensorProto()
    with open(fname, 'rb') as f:
        tensor.ParseFromString(f.read())
    return numpy_helper.to_array(tensor)


if len(sys.argv) != 4:
    print(f'Usage: {sys.argv[0]} <model_path> <dataset_path> <count>')
    exit(1)

model_path, dataset_path, count = sys.argv[1:]
count = int(count)

input_files = glob.glob(path.join(dataset_path, 'input_*.pb'))
output_files = glob.glob(path.join(dataset_path, 'output_*.pb'))

print(f'Input files: {len(input_files)}', file=sys.stderr)
print(f'Output files: {len(output_files)}', file=sys.stderr)

input_tensors = [load_tensor(fname) for fname in input_files]
output_tensors = [load_tensor(fname) for fname in output_files]

interpreter = tf.lite.Interpreter(path.join(model_path, 'model.tflite'))
interpreter.allocate_tensors()
for idx, input_tensor in enumerate(input_tensors):
    input_idx = interpreter.get_input_details()[idx]['index']
    interpreter.set_tensor(input_idx, input_tensor)

result = timeit.timeit(interpreter.invoke, number=count)

result_d = decimal.Decimal(result) * 1_000_000_000  # ns
print(f'{count} invocations took {result_d} ns')
