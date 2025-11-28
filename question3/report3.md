# Question 3 — C Extension (temp_ext)

Files:

- temp_ext.c
- setup.py
- test.py

Build & Install (local user):
python3 setup.py build
python3 setup.py install --user

Run test:
python3 test.py

Functions:

- min_temp(list_of_floats)
- max_temp(list_of_floats)
- avg_temp(list_of_floats)
- variance_temp(list_of_floats)  (sample variance)
- count_readings(list_of_floats)

Complexity:

- Each function is O(n) time and uses O(n) memory to copy Python list into a C array.
