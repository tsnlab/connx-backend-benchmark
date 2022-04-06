#!/usr/bin/env python3

import glob
import os
import onnx
from onnx_tf.backend import prepare
import tensorflow as tf

TEST_PATH = os.environ['TEST_PATH']

for test in glob.glob(os.path.join(TEST_PATH, 'test_*')):
    try:
        print('Converting {}'.format(test))
        model_path = os.path.join(test, 'model.onnx')
        pb_path = os.path.join(test, 'model.pb')
        tflite_path = os.path.join(test, 'model.tflite')

        model = onnx.load(model_path)
        tf_rep = prepare(model)
        tf_rep.export_graph(pb_path)

        converter = tf.lite.TFLiteConverter.from_saved_model(pb_path)
        tflite_rep = converter.convert()
        with open(tflite_path, 'wb') as f:
            f.write(tflite_rep)
    except Exception:
        print('Failed to convert {}'.format(test))
    else:
        print('Successfully converted {}'.format(test))
