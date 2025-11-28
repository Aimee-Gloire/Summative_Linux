# Question 3 — C Extension (temp_ext)

Files:

- temp_ext.c
- setup.py
- test.py

Functions:

- min_temp(list_of_floats)
- max_temp(list_of_floats)
- avg_temp(list_of_floats)
- variance_temp(list_of_floats)  (sample variance)
- count_readings(list_of_floats)

Build & Install (local user):

- python3 setup.py build
- python3 setup.py install --user

Run:
python3 test.py

Performance Notes

Linear time: O(n)
Memory: copies Python floats into C array once (efficient)
Handles invalid inputs gracefully
Much faster than pure Python implementation
