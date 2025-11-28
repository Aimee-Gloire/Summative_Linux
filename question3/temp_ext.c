#include <Python.h>

/*
    This C extension implements fast temperature statistics.

    All functions operate on a Python list of floats.
    We convert the list to a C array internally for speed.

    Time complexity:
        - All operations are O(n)
    Memory usage:
        - C array allocated once, freed before return
*/

///////////////////////////////////////////////////////////////
// Helper: Convert Python list → C double array
///////////////////////////////////////////////////////////////
static int convert_list(PyObject *listObj, double **arr, Py_ssize_t *size) {
    if (!PyList_Check(listObj)) {
        PyErr_SetString(PyExc_TypeError, "Expected a Python list");
        return 0;
    }

    *size = PyList_Size(listObj);
    *arr = (double*) malloc(sizeof(double) * (*size));

    if (*arr == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
        return 0;
    }

    for (Py_ssize_t i = 0; i < *size; i++) {
        PyObject *item = PyList_GetItem(listObj, i);
        (*arr)[i] = PyFloat_AsDouble(item);
    }
    return 1;
}

///////////////////////////////////////////////////////////////
// min_temp
///////////////////////////////////////////////////////////////
static PyObject* min_temp(PyObject *self, PyObject *args) {
    PyObject *listObj;
    if (!PyArg_ParseTuple(args, "O", &listObj)) return NULL;

    double *arr;
    Py_ssize_t n;

    if (!convert_list(listObj, &arr, &n)) return NULL;
    if (n == 0) {
        free(arr);
        PyErr_SetString(PyExc_ValueError, "Empty temperature list");
        return NULL;
    }

    double min = arr[0];
    for (Py_ssize_t i = 1; i < n; i++)
        if (arr[i] < min) min = arr[i];

    free(arr);
    return PyFloat_FromDouble(min);
}

///////////////////////////////////////////////////////////////
// max_temp
///////////////////////////////////////////////////////////////
static PyObject* max_temp(PyObject *self, PyObject *args) {
    PyObject *listObj;
    if (!PyArg_ParseTuple(args, "O", &listObj)) return NULL;

    double *arr;
    Py_ssize_t n;

    if (!convert_list(listObj, &arr, &n)) return NULL;
    if (n == 0) {
        free(arr);
        PyErr_SetString(PyExc_ValueError, "Empty temperature list");
        return NULL;
    }

    double max = arr[0];
    for (Py_ssize_t i = 1; i < n; i++)
        if (arr[i] > max) max = arr[i];

    free(arr);
    return PyFloat_FromDouble(max);
}

///////////////////////////////////////////////////////////////
// avg_temp
///////////////////////////////////////////////////////////////
static PyObject* avg_temp(PyObject *self, PyObject *args) {
    PyObject *listObj;
    if (!PyArg_ParseTuple(args, "O", &listObj)) return NULL;

    double *arr;
    Py_ssize_t n;

    if (!convert_list(listObj, &arr, &n)) return NULL;
    if (n == 0) {
        free(arr);
        PyErr_SetString(PyExc_ValueError, "Empty temperature list");
        return NULL;
    }

    double sum = 0;
    for (Py_ssize_t i = 0; i < n; i++)
        sum += arr[i];

    free(arr);
    return PyFloat_FromDouble(sum / n);
}

///////////////////////////////////////////////////////////////
// variance_temp (sample variance)
///////////////////////////////////////////////////////////////
static PyObject* variance_temp(PyObject *self, PyObject *args) {
    PyObject *listObj;
    if (!PyArg_ParseTuple(args, "O", &listObj)) return NULL;

    double *arr;
    Py_ssize_t n;

    if (!convert_list(listObj, &arr, &n)) return NULL;
    if (n < 2) {
        free(arr);
        PyErr_SetString(PyExc_ValueError, "Variance requires at least 2 values");
        return NULL;
    }

    double sum = 0;
    for (Py_ssize_t i = 0; i < n; i++)
        sum += arr[i];

    double mean = sum / n;

    double var = 0;
    for (Py_ssize_t i = 0; i < n; i++)
        var += (arr[i] - mean) * (arr[i] - mean);

    var /= (n - 1);

    free(arr);
    return PyFloat_FromDouble(var);
}

///////////////////////////////////////////////////////////////
// count_readings
///////////////////////////////////////////////////////////////
static PyObject* count_readings(PyObject *self, PyObject *args) {
    PyObject *listObj;
    if (!PyArg_ParseTuple(args, "O", &listObj)) return NULL;

    Py_ssize_t n = PyList_Size(listObj);
    return PyLong_FromSsize_t(n);
}

///////////////////////////////////////////////////////////////
// Method table
///////////////////////////////////////////////////////////////
static PyMethodDef TempMethods[] = {
    {"min_temp", min_temp, METH_VARARGS, "Get minimum temperature"},
    {"max_temp", max_temp, METH_VARARGS, "Get maximum temperature"},
    {"avg_temp", avg_temp, METH_VARARGS, "Get average temperature"},
    {"variance_temp", variance_temp, METH_VARARGS, "Get variance"},
    {"count_readings", count_readings, METH_VARARGS, "Count readings"},
    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////
// Module definition
///////////////////////////////////////////////////////////////
static struct PyModuleDef tempmodule = {
    PyModuleDef_HEAD_INIT,
    "temp_ext",
    "Temperature statistics extension",
    -1,
    TempMethods
};

PyMODINIT_FUNC PyInit_temp_ext(void) {
    return PyModule_Create(&tempmodule);
}
