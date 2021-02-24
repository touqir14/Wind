#include "src/basicHttp.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    PyObject_HEAD
    Listener* listener;
    Scheduler* scheduler;
} windObj;


responseMap_t create_responseMap(PyObject* dict) {
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	PyErr_Clear();
	responseMap_t map;
	std::string yo;
	while (PyDict_Next(dict, &pos, &key, &value)) {
		Py_ssize_t k_len, v_len;
	    const char* k = PyUnicode_AsUTF8AndSize(key, &k_len);
	    const char* v = PyUnicode_AsUTF8AndSize(value, &v_len);
	    if ((k==NULL) || (v==NULL)) {
	        continue;
	    }
	    map[std::string(k, k_len)] = std::string(v, v_len);
	}
	if (PyErr_Occurred() != NULL) {
		PyErr_Clear();
		PyErr_SetString(PyExc_ValueError, "1 or more uri endpoint value and respose body could not be parsed");
	}
	return map;
}

static void custom_dealloc(windObj* self) {
    /*
    The destructor
    */

	delete self->listener;
	delete self->scheduler;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* custom_new(PyTypeObject *type, PyObject *args) {
    /*
    Allocates the windObj
    */

    auto self = (windObj*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

static int custom_init(windObj* self, PyObject *args) {
    /*
    Constructor for allocating and initializing the hashtable along with the iterators.
    */

	int64_t port;
	Py_ssize_t name_len;
	char* server_name;

	if (!PyArg_ParseTuple(args, "Ls#", &port, &server_name, &name_len)) {
		return -1;
	}

	if (!(0 <= port <= 65535)) {
		PyErr_SetString(PyExc_ValueError, "Port must be a value in the range [0, 65535]");
		return -1; 
	}

	if (basicHttp::init(port, &(self->listener), &(self->scheduler), std::string(server_name, name_len)) != 0) {
		// This whole thing needs some modification for proper error message catching.
		return -1;
	}

    return 0;    
}

static PyObject* set_responses(windObj* self, PyObject* args) {
	PyObject* uri_responses;

	if (!PyArg_ParseTuple(args, "O!", &PyDict_Type, &uri_responses)) {
		return NULL;
	}	

	auto map = create_responseMap(uri_responses);
	if (PyErr_Occurred() != NULL) {
		return NULL;
	}

	basicHttp::assign_uri_responses(map, self->listener);
	Py_RETURN_NONE;
}

static PyObject* run_server(windObj* self) {
	basicHttp::run_scheduler(self->scheduler, self->listener);
	Py_RETURN_NONE;
}

static PyMethodDef methods_wind[] = {
    {"set_responses", set_responses, METH_VARARGS, "This method initializes the uri handlers based on the input dictionary such that for a (key, value) pair, key is the uri and value is the response body to be returned."},
    {"run_server", run_server, METH_VARARGS, "Calling this method puts the server in a listening state."},
    {NULL, NULL, 0, NULL}
};


static PyTypeObject windType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "wind backend",
    .tp_basicsize = sizeof(windObj),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) custom_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "wind backend ...",
    .tp_methods = methods_wind,
    .tp_init = (initproc) custom_init,
    .tp_new = custom_new,
};


static struct PyModuleDef moduleDef_wind =
{
    PyModuleDef_HEAD_INIT,
    "_wind", /* name of module */ 
    NULL, // Documentation of the module
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
};

PyMODINIT_FUNC PyInit__wind(void)
{
    PyObject *obj;

    if (PyType_Ready(&windType) < 0)
        return NULL;

    obj = PyModule_Create(&moduleDef_wind);
    if (obj == NULL)
        return NULL;

    Py_INCREF(&windType);
    if (PyModule_AddObject(obj, "create_server", (PyObject *) &windType) < 0) {
        Py_DECREF(&windType);
        Py_DECREF(obj);
        return NULL;
    }

    return obj;
}