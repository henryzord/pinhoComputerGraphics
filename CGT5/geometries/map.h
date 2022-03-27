#ifndef MAP_H
#define MAP_H

#define N_VERTICES_SLAB 150

#include "twod_vectors.h"
#include "polygon.h"

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <math.h>
#include <vector>
#include <Python.h>
#include "structmember.h"  // for PyMemberDef

typedef struct {
    PyObject_HEAD
    int n_polygons;
    int n_slabs;
    double *slabs;
    PyObject *py_slabs;
    PyObject *py_polygons;
    std::vector<std::vector<int>> slabsIndices;
} PyMap;

static PyTypeObject PyMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Map"   /* tp_name */
};

static int PyMap_init(PyMap *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"polygons", NULL};
    PyObject *py_polygons;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyList_Type, &py_polygons)) {
        return NULL;
    }

    self->n_polygons = (int)PyList_Size(py_polygons);
    self->py_polygons = PyList_New(self->n_polygons);

    std::vector<Vetor> limits;
    double absoluteLower, absoluteUpper;
    for(int i = 0; i < self->n_polygons; i++) {
        PyPolygon *ref = (PyPolygon*)PyList_GetItem(py_polygons, i);
        Py_INCREF((PyObject*)ref);
        PyList_SetItem(self->py_polygons, i, (PyObject*)PyPolygon_copy(ref));

        if(i == 0) {
            absoluteLower = ref->vertices[0].y;
            absoluteUpper = ref->vertices[0].y;
        }
        double upper, lower;
        for(int j = 0; j < ref->n_vertices; j++) {
            if(j == 0) {
                upper = ref->vertices[j].y;
                lower = ref->vertices[j].y;
            } else {
                lower = fmin(lower, ref->vertices[j].y);
                upper = fmax(upper, ref->vertices[j].y);
            }
        }
        absoluteLower = fmin(absoluteLower, lower);
        absoluteUpper = fmax(absoluteUpper, upper);
        limits.push_back(Vetor(lower, upper));
        Py_DECREF((PyObject*)ref);
    }

    self->slabs = new double [N_VERTICES_SLAB];
    self->py_slabs = PyList_New(N_VERTICES_SLAB);
    self->n_slabs = N_VERTICES_SLAB;

    double increment = (absoluteUpper - absoluteLower)/self->n_slabs;
    for(int i = 0; i < N_VERTICES_SLAB; i++) {
        self->slabs[i] = absoluteLower + i * increment;
        PyList_SetItem(self->py_slabs, i, Py_BuildValue("d", self->slabs[i]));
    }

    for(int i = 0; i < self->n_slabs; i++) {
        std::vector<int> thisIndices;
        for(int j = 0; j < self->n_polygons; j++) {
            // limits[j].x is lower y bound for polygon j
            // limits[j].y is upper y bound for polygon j
            if((self->slabs[i] >= limits[j].x) && (self->slabs[i] <= limits[j].y)) {
                thisIndices.push_back(j);
            }
        }
        self->slabsIndices.push_back(thisIndices);
    }
    return 1;
}

static void PyMap_dealloc(PyMap * self) {
    Py_DECREF(self->py_slabs);
    Py_DECREF(self->py_polygons);
    free(self->slabs);
    Py_TYPE(self)->tp_free(self);
}

/**
 * Checa se um ponto está dentro de um polígono desse mapa.
 * @param point Ponto a ser checado
 * @return -1 se o ponto não está dentro de nenhum polígono; ou o índice do polígono ao qual esse ponto
 * está inserido
 */
PyObject *PyMap_checkInside(PyMap *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"point", NULL};
    PyObject *py_point;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyTuple_Type, &py_point)) {
        return NULL;
    }
    Py_INCREF(py_point);
    Vetor point = readPoint(py_point);
    Py_DECREF(py_point);

    int index = -1;

    for(int i = 0; i < self->n_slabs; i++) {
        if(point.y <= self->slabs[i]) {
            index = i;
            break;
        }
    }
    if(index == -1) {
        return Py_BuildValue("i", index);
    }


    // indices has the set of indices of polygons in that region
    std::vector<int> indices = self->slabsIndices[index];
    PyObject *res;
    PyObject *other_args = PyTuple_New(1);
    PyTuple_SetItem(other_args, 0, py_point);
    PyObject *other_kwargs = PyDict_New();

    int return_index = -1;

    // for each polygon in that region
    for(int j = 0; j < indices.size(); j++) {
        PyPolygon *ref = (PyPolygon*)PyList_GetItem(self->py_polygons, indices[j]);
        Py_INCREF(ref);
        res = PyPolygon_isInside(ref, other_args, other_kwargs);
        Py_DECREF(ref);
        if(PyObject_IsTrue(res)) {
            return_index = indices[j];
            break;
        }
    }
    Py_DECREF(res);
//    Py_DECREF(other_args);  // seg fault
    Py_DECREF(other_kwargs);

    return Py_BuildValue("i", return_index);
}

static PyMethodDef PyMap_methods[] = {
    {
        "checkInside",
        (PyCFunction)PyMap_checkInside, METH_VARARGS | METH_KEYWORDS,
        "Checks whether a point is inside this map. If true, returns the index of the polygon; -1 if it is outside"
    },
//    {
//        "toConvexHull",
//        (PyCFunction)PyPolygon_toConvexHull, METH_NOARGS,
//        "Converts a concave polygon to a convex one. Idempotent on convex polygons"
//    },
    {NULL, NULL, 0, NULL}
};

//int n_polygons;
//PyPolygon **polygons;
//int n_slabs;
//double *slabs;

static PyMemberDef PyMap_members[] = {
    {"n_polygons", T_INT, offsetof(PyMap, n_polygons), 0, "number of polygons in this map"},
    {"polygons", T_OBJECT, offsetof(PyMap, py_polygons), 0, "polygons of this map"},
    {"slabs", T_OBJECT, offsetof(PyMap, py_slabs), 0, "slabs of this map"},
//    {"isThisConvex", T_BOOL, offsetof(PyPolygon, isThisConvex), 0, "whether this polygon is convex or not"},
    {NULL}  /* Sentinel */
};


#endif // MAP_H