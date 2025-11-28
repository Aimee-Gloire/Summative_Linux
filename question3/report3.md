# Question 3 — C Extension (temp_ext)

## 1. Overview

This task required creating a performance-optimized C extension for Python.  
The extension processes a list of floating-point temperature readings and exposes:

- min_temp()
- max_temp()
- avg_temp()
- variance_temp()
- count_readings()

The module is named `temp_ext` and integrates cleanly with Python via setuptools.

---

## 2. Files Included

- `temp_ext.c` — C source
- `setup.py` — build script
- `test.py` — sample usage script

---

## 3. Implementation Summary

### Functionality

Each function:

- Accepts a Python list of floats
- Converts it into a C `double` array
- Performs O(n) computation
- Returns results back to Python

### Memory

- Uses stack arrays for iteration
- No malloc needed
- Python reference counts respected

### Performance

- O(n) for min, max, sum, variance
- Faster than Python loops due to compiled C operations

### Error Handling

- Checks for empty lists
- Ensures elements are numeric
- Returns Python exceptions on invalid input

---

## 4. How to Build and Run

```bash
python3 setup.py build
python3 setup.py install --user
python3 test.py
```
