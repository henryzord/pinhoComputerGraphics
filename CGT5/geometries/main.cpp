//
//  Matematica.cpp
//  Testes
//
//  Created by Márcio Sarroglia Pinho on 19/03/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "polygon.h"
#include "map.h"

#include <stdio.h>
#include <cstdlib>
#include <Python.h>
#include "structmember.h"  // for PyMemberDef


//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//


// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef module_methods[] = {
{
    "rotatePoint", (PyCFunction)Py_rotatePoint, METH_VARARGS | METH_KEYWORDS,
    "Rotates a point given a pivot"
    },
    {NULL, NULL, 0, NULL}
};

// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef module_definition = {
    PyModuleDef_HEAD_INIT,
    "geometries",
    "A Python module to compute whether points are inside or outside geometries.",
    -1,
    module_methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_geometries(void) {
    PyObject* module_py;

    PyPolygonType.tp_new = PyType_GenericNew;
    PyPolygonType.tp_basicsize = sizeof(PyPolygon);
    PyPolygonType.tp_dealloc=(destructor) PyPolygon_dealloc;
    PyPolygonType.tp_flags=Py_TPFLAGS_DEFAULT;
    PyPolygonType.tp_doc="Describes a Polygon. A polygon is composed by a given number of vertices, where each vertex is a tuple with three values.";
    PyPolygonType.tp_methods=PyPolygon_methods;
    PyPolygonType.tp_members=PyPolygon_members;
    PyPolygonType.tp_str=(reprfunc)PyPolygon_str;
    PyPolygonType.tp_repr=(reprfunc)PyPolygon_str;
    PyPolygonType.tp_init=(initproc)PyPolygon_init;

    if (PyType_Ready(&PyPolygonType) < 0) {
        return NULL;
    }

    PyMapType.tp_new = PyType_GenericNew;
    PyMapType.tp_basicsize = sizeof(PyMap);
    PyMapType.tp_dealloc=(destructor)PyMap_dealloc;
    PyMapType.tp_flags=Py_TPFLAGS_DEFAULT;
    PyMapType.tp_doc="Describes a Map. A map is composed by a set of polygons.";
    PyMapType.tp_methods=PyMap_methods;
    PyMapType.tp_members=PyMap_members;
//    PyMapType.tp_str=(reprfunc)PyMap_str;
//    PyMapType.tp_repr=(reprfunc)PyMap_str;
    PyMapType.tp_init=(initproc)PyMap_init;

    if (PyType_Ready(&PyMapType) < 0) {
        return NULL;
    }

    module_py = PyModule_Create(&module_definition);
    if (!module_py) {
        return NULL;
    }

    Py_INCREF(&PyPolygonType);
    Py_INCREF(&PyMapType);
    PyModule_AddObject(module_py, "Polygon", (PyObject*)&PyPolygonType);
    PyModule_AddObject(module_py, "Map", (PyObject*)&PyMapType);
    return module_py;
}