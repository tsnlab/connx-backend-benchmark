#!/usr/bin/env python3

import csv
from collections import defaultdict


results = defaultdict(dict)

# Read tsv file
with open('engine_results.tsv', 'r') as tsvfile:
    reader = csv.reader(tsvfile, delimiter='\t')
    next(reader)  # Skip header
    for row in reader:
        engine, test_name, dataset_name, result = row
        results[(test_name, dataset_name)].update({engine: result})


# Write tsv file
with open('engine_results_reformed.csv', 'w') as tsvfile:
    writer = csv.writer(tsvfile, delimiter='\t')
    writer.writerow(['test_name', 'dataset_name', 'connx', 'tflite'])
    for (test_name, dataset_name), engine_results in results.items():
        connx_result = engine_results.get('connx', 'Missing')
        tflite_result = engine_results.get('tflite', 'Missing')
        writer.writerow([test_name, dataset_name, connx_result, tflite_result])
