#ifndef POLYGON_H
#define POLYGON_H

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include "twod_vectors.h"
#include <Python.h>
#include "structmember.h"  // for PyMemberDef

/**
 * Verifica se um polígono é côncavo ou convexo.
 * @param num_vert Número de vértices do polígono
 * @param points Pontos do polígono
 * @return 1 se o polígono for convexo, 0 se for côncavo
 */
bool isConvex(int n_points, Vetor *points) {
    int signal = 1;

    for(int i = 0; i < n_points; i++) {
        int index[3] = {
            ((i - 1) < 0) * (n_points - 1) + ((i - 1) > 0) * (i - 1),
            i,
            ((i + 1) < n_points) * (i + 1) + ((i + 1) > n_points) * 0
        };

        Vetor a = points[index[1]] - points[index[0]];
        Vetor b = points[index[1]] - points[index[2]];

        Vetor c1 = a.cross(b);
        int new_signal = c1.z >= 0? 1 : -1;
        if(i == 0) {
            signal = new_signal;
        } else if(new_signal != signal) {
            return false;
        }
    }
    return true;
}


/**
 * Verifica se um Vetor está dentro de um polígono côncavo.
 * @param num_vert Número de vértices do polígono
 * @param Vetors Coordenadas dos vértices
 * @param Vetor Coordenada do Vetor
 * @return 1 se o Vetor estiver dentro do polígono, 0 se não
 */
int isInsideConcave(int num_vert, Vetor *pontos, Vetor ponto) {
    int inside = 0;
    for(int i = 0; i < num_vert; ++i) {
        Vetor v1 = pontos[i];
        Vetor v2 = pontos[(i+1)%num_vert];

        int considered = (v1.y > ponto.y) != (v2.y > ponto.y);
        int in_half_plane = ponto.x < ((((v2.x - v1.x) * (ponto.y - v1.y)) / (v2.y - v1.y)) + v1.x);
        int intersect = considered && in_half_plane;
        if(intersect) inside = !inside;
    }

    return inside;
}

/**
 * Verifica se um Vetor está dentro de um polígono convexo.
 * @param num_vert Número de vértices do polígono
 * @param vetores Coordenadas dos vértices
 * @param Vetor Coordenada do Vetor
 * @return 1 se o Vetor estiver dentro do polígono, 0 se não
 */
int isInsideConvex(int num_vert, Vetor *vetores, Vetor ponto) {
    int signal = 1;

    for(int i = 0; i < num_vert; i++) {
        int index[2] = {
            ((i - 1) < 0) * (num_vert - 1) + ((i - 1) > 0) * (i - 1),
            i
        };

        Vetor a = vetores[index[1]] - vetores[index[0]];
        Vetor b = vetores[index[1]] - ponto;

        Vetor c1 = a.cross(b);
        int new_signal = c1.z >= 0? 1 : -1;
        if(i == 0) {
            signal = new_signal;
        } else if(new_signal != signal) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    PyObject_HEAD
    int n_vertices;
    Vetor *vertices;
    PyObject *py_vertices;
    bool isThisConvex;

    Vetor pivotPoint;
    Vetor bbCenter;
    double bbHalfHeight;
    double bbHalfWidth;
    double allignAngle;

} PyPolygon;

static PyTypeObject PyPolygonType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Polygon"   /* tp_name */
};

Vetor rotatePointArbitrary(Vetor vec, Vetor origin, double radiansAngle) {
    return Vetor(
        (vec.x - origin.x) * cos(radiansAngle) - (vec.y - origin.y) * sin(radiansAngle) + origin.x,
        (vec.x - origin.x) * sin(radiansAngle) + (vec.y - origin.y) * cos(radiansAngle) + origin.y
    );
}

Vetor rotatePointOrigin(Vetor vec, double radiansAngle) {
    return rotatePointArbitrary(vec, Vetor(0, 0), radiansAngle);
}

void PyPolygon_addEnvelope(PyPolygon *self) {
    // finds two farthest points
    int aIndex = 0, bIndex = 1;
    double maxDist = -1;
    for(int i = 0; i < self->n_vertices; i++) {
        for(int j = i + 1; j < self->n_vertices; j++) {
            double dist = Vetor::point_distance(self->vertices[i], self->vertices[j]);
            if(dist > maxDist) {
                aIndex = i;
                bIndex = j;
                maxDist = dist;
            }
        }
    }

    Vetor closest, farthest;
    if(Vetor::point_distance(self->vertices[aIndex], Vetor(0, 0)) < Vetor::point_distance(self->vertices[bIndex], Vetor(0, 0))) {
        closest = self->vertices[aIndex];
        farthest = self->vertices[bIndex];
    } else {
        closest = self->vertices[bIndex];
        farthest = self->vertices[aIndex];
    }

    double adjacent = abs(farthest.x - closest.x);
    double opposite = abs(farthest.y - closest.y);
    double hypotenuse = Vetor::point_distance(closest, farthest);

    double signal;
    if(farthest.x - closest.x > 0) {  // either on first or fourth quadrant
        if(farthest.y - closest.y > 0) {  // first quadrant
            signal = -1;  // should rotate clockwise to put farthest point on x axis
        } else {  // fourth quadrant
            signal = 1;  // should rotate counter-clockwise to put farthest point on x axis
        }
    } else {  // either on second or third quadrant
        if(farthest.y - closest.y > 0) {  // first quadrant
            signal = 1;  // should rotate counter-clockwise to put farthest point on x axis
        } else {  // third quadrant
            signal = -1;  // should rotate clockwise to put farthest point on x axis
        }
    }

    // angle between closest and farthest points
    double rotAngle = acos(adjacent / hypotenuse);  // in radians

    double upperY = 0, lowerY = 0, upperX = 0, lowerX = 0;
    double meanX = 0, meanY = 0;
    for(int i = 0; i < self->n_vertices; i++) {
        Vetor rotated = rotatePointArbitrary(self->vertices[i], closest, signal * rotAngle);

        meanX += rotated.x;
        meanY += rotated.y;

        if(rotated.x > upperX || i == 0) {
            upperX = rotated.x;
        }
        if(rotated.x < lowerX || i == 0) {
            lowerX = rotated.x;
        }
        if(rotated.y > upperY || i == 0) {
            upperY = rotated.y;
        }
        if(rotated.y < lowerY || i == 0) {
            lowerY = rotated.y;
        }
    }
    self->bbHalfHeight = (upperY - lowerY)/2.0;
    self->bbHalfWidth = (upperX - lowerX)/2.0;
    self->bbCenter = Vetor(lowerX + self->bbHalfWidth, lowerY + self->bbHalfHeight);
    self->allignAngle = signal * rotAngle;
    self->pivotPoint = closest;
}

static int PyPolygon_init(PyPolygon *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"vertices", NULL};
    PyObject *thisPolygon;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyList_Type, &thisPolygon)) {
        return NULL;
    }
    Py_INCREF(thisPolygon);

    int num_vert = (int)PyList_Size(thisPolygon);
    self->vertices = new Vetor [num_vert];
    self->py_vertices = PyList_New(num_vert);

    for(int j = 0; j < num_vert; j++) {
        PyObject *vertex = PyList_GetItem(thisPolygon, j);
        Py_INCREF(vertex);

        double x = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 0));
        double y = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 1));
        int tuple_size = (int)PyTuple_Size(vertex);
        double z = 0;
        if(tuple_size > 2) {
            z = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 2));
        }
        Py_DECREF(vertex);
        self->vertices[j] = Vetor(x, y, z);
        PyList_SetItem(self->py_vertices, j, Py_BuildValue("(ddd)", x, y, z));
    }
    self->n_vertices = num_vert;
    self->isThisConvex = isConvex(self->n_vertices, self->vertices);

    PyPolygon_addEnvelope(self);
    Py_DECREF(thisPolygon);

    return 1;
}

static void PyPolygon_dealloc(PyPolygon * self) {
    free(self->vertices);
    Py_DECREF(self->py_vertices);
    Py_TYPE(self)->tp_free(self);
}

static PyObject *PyPolygon_checkPointEnvelopeIntersection(PyPolygon *self, Vetor point) {
    Vetor rotated = rotatePointArbitrary(point, self->pivotPoint, self->allignAngle);
    if((rotated.x >= (self->bbCenter.x - self->bbHalfWidth)) && (rotated.x <= (self->bbCenter.x + self->bbHalfWidth))) {
        if((rotated.y >= (self->bbCenter.y - self->bbHalfHeight)) && (rotated.y <= (self->bbCenter.y + self->bbHalfHeight))) {
            Py_INCREF(Py_True);
            return Py_True;
        }
    }

    Py_INCREF(Py_False);
    return Py_False;
}

static PyObject *PyPolygon_checkEnvelopeIntersection(PyPolygon *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"other", NULL};
    PyPolygon *other;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyPolygonType, &other)) {
        return NULL;
    }
    Py_INCREF(other);

    for(int i = 0; i < other->n_vertices; i++) {
        Vetor rotated = rotatePointArbitrary(other->vertices[i], self->pivotPoint, self->allignAngle);
        if((rotated.x >= (self->bbCenter.x - self->bbHalfWidth)) && (rotated.x <= (self->bbCenter.x + self->bbHalfWidth))) {
            if((rotated.y >= (self->bbCenter.y - self->bbHalfHeight)) && (rotated.y <= (self->bbCenter.y + self->bbHalfHeight))) {
                Py_DECREF(other);
                Py_INCREF(Py_True);
                return Py_True;
            }
        }
    }
    Py_DECREF(other);
    Py_INCREF(Py_False);
    return Py_False;
}

static Vetor readPoint(PyObject *py_point) {
    int tuple_size = (int)PyTuple_Size(py_point);

    PyObject *py_x = PyTuple_GetItem(py_point, 0);
    PyObject *py_y = PyTuple_GetItem(py_point, 1);
    Py_INCREF(py_x);
    Py_INCREF(py_y);
    double xp = (double)PyFloat_AsDouble(py_x);
    double yp = (double)PyFloat_AsDouble(py_y);
    Py_DECREF(py_x);
    Py_DECREF(py_y);
    double zp = 0;

    if(tuple_size > 2) {
        PyObject *py_z = PyTuple_GetItem(py_point, 2);
        Py_INCREF(py_z);
        zp = (double)PyFloat_AsDouble(py_z);
        Py_DECREF(py_z);
    }
    Vetor point(xp, yp, zp);
    return point;
}

PyObject *Py_rotatePoint(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"point", "pivot", "radians", NULL};
    PyObject *py_point, *py_pivot;
    double radians;
    if(!PyArg_ParseTupleAndKeywords(
        args, kwargs, "O!O!d", keywords,
        &PyTuple_Type, &py_point,
        &PyTuple_Type, &py_pivot,
        &radians
    )) {
        return NULL;
    }
    Py_INCREF(py_point);
    Py_INCREF(py_pivot);
    Vetor point = readPoint(py_point);
    Vetor pivot = readPoint(py_pivot);
    Vetor rotated = rotatePointArbitrary(point, pivot, radians);
    Py_DECREF(py_point);
    Py_DECREF(py_pivot);

    return Py_BuildValue("(dd)", rotated.x, rotated.y);
}

/**
 * Método para verificar se um ponto está contido no polígono.
 * Faz uma pré-checagem com bounding box; passando por essa checagem, aplica um algoritmo mais complexo
 * de inserção em polígono, dependendo do formato do polígono (côncavo ou convexo).
 */
static PyObject *PyPolygon_isInside(PyPolygon *self, PyObject *args, PyObject *kwargs) {
    PyObject *py_point;

    static char *keywords[] = {"point", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyTuple_Type, &py_point)) {
        return NULL;
    }
    Py_INCREF(py_point);
    Vetor point = readPoint(py_point);
    Py_DECREF(py_point);

//    PyObject *inside_envelope = PyPolygon_checkPointEnvelopeIntersection(self, point);

//    if(PyObject_IsTrue(inside_envelope)) {
//        Py_DECREF(inside_envelope);
//        Py_INCREF(Py_True);
//        return Py_True;
//    }

    int isInside;
    if(self->isThisConvex) {
        isInside = isInsideConvex(self->n_vertices, self->vertices, point);
    } else {
        isInside = isInsideConcave(self->n_vertices, self->vertices, point);
    }
    if(isInside) {
        Py_INCREF(Py_True);
        return Py_True;
    }
    Py_INCREF(Py_False);
    return Py_False;
}

/**
 * Implementa o algoritmo gift wrapping para calcular o convex hull.
 * Começa pelo ponto mais abaixo, e procura os pontos subsequentes que possuem o menor ângulo com este vetor.
 */
static PyPolygon *PyPolygon_toConvexHull(PyPolygon *self) {
    int lowestY = 2147483647;  // max int value
    int startIndex = -1;
    for(int i = 0; i < self->n_vertices; i++) {
        if(self->vertices[i].y < lowestY) {
            lowestY = i;
            startIndex = i;
        }
    }
    int n_newPoints = 1;
    Vetor *new_points = (Vetor*)malloc(sizeof(Vetor) * self->n_vertices);
    new_points[0] = self->vertices[startIndex];

    int curIndex = startIndex;
    int lastIndex = curIndex;

    do {
        int minAngle = 359;
        int maxIndex = -1;
        Vetor thisPoint = self->vertices[curIndex];
        Vetor thisFarRight(100, 0);

        for(int j = 0; j < self->n_vertices; j++) {
            if(j == curIndex) {
                continue;
            }
            Vetor thatPoint = self->vertices[j];
            Vetor thatTransPoint(thatPoint.x - thisPoint.x, thatPoint.y - thisPoint.y);

            int angle = thisFarRight.angle(thatTransPoint);
            if(thatTransPoint.y < 0) {
                angle += 180;
            }

            if(angle < minAngle) {
                minAngle = angle;
                maxIndex = j;
            }
        }
        lastIndex = curIndex;
        curIndex = maxIndex;
        new_points[n_newPoints] = self->vertices[curIndex];
        n_newPoints += 1;
    } while((curIndex != startIndex) && (n_newPoints < self->n_vertices));

    n_newPoints -= 1;  // last point will be the same as first

    PyPolygon *new_polygon = PyObject_New(PyPolygon, &PyPolygonType);
    new_polygon->n_vertices = n_newPoints;
    new_polygon->vertices = new_points;
    new_polygon->isThisConvex = true;  // it has to be
    new_polygon->pivotPoint = self->pivotPoint;
    new_polygon->bbCenter = self->bbCenter;
    new_polygon->bbHalfHeight = self->bbHalfHeight;
    new_polygon->bbHalfWidth = self->bbHalfWidth;
    new_polygon->allignAngle = self->allignAngle;

    new_polygon->py_vertices = PyList_New(n_newPoints);

    for(int i = 0; i < n_newPoints; i++) {
        PyList_SetItem(
            new_polygon->py_vertices, i,
            Py_BuildValue("(ddd)", new_points[i].x, new_points[i].y, new_points[i].z)
        );
    }

    return new_polygon;
}

static PyObject *PyPolygon_getEnvelope(PyPolygon *self) {
    PyObject *corners = PyList_New(4);

    Vetor lowerLeft = rotatePointArbitrary(self->bbCenter + Vetor(-self->bbHalfWidth, -self->bbHalfHeight), self->pivotPoint, -1 * self->allignAngle);
    Vetor upperLeft = rotatePointArbitrary(self->bbCenter + Vetor(-self->bbHalfWidth, self->bbHalfHeight), self->pivotPoint, -1 * self->allignAngle);
    Vetor lowerRight = rotatePointArbitrary(self->bbCenter + Vetor(self->bbHalfWidth, -self->bbHalfHeight),self->pivotPoint, -1 * self->allignAngle);
    Vetor upperRight = rotatePointArbitrary(self->bbCenter + Vetor(self->bbHalfWidth, self->bbHalfHeight), self->pivotPoint, -1 * self->allignAngle);

    PyList_SetItem(corners, 0, Py_BuildValue("(dd)", lowerLeft.x, lowerLeft.y));
    PyList_SetItem(corners, 1, Py_BuildValue("(dd)", upperLeft.x, upperLeft.y));
    PyList_SetItem(corners, 2, Py_BuildValue("(dd)", upperRight.x, upperRight.y));
    PyList_SetItem(corners, 3, Py_BuildValue("(dd)", lowerRight.x, lowerRight.y));

    return corners;
}

static PyObject *PyPolygon_getOriginalEnvelope(PyPolygon *self) {
    PyObject *corners = PyList_New(4);

    Vetor lowerLeft = self->bbCenter + Vetor(-self->bbHalfWidth, -self->bbHalfHeight);
    Vetor upperLeft = self->bbCenter + Vetor(-self->bbHalfWidth, self->bbHalfHeight);
    Vetor lowerRight = self->bbCenter + Vetor(self->bbHalfWidth, -self->bbHalfHeight);
    Vetor upperRight = self->bbCenter + Vetor(self->bbHalfWidth, self->bbHalfHeight);

    PyList_SetItem(corners, 0, Py_BuildValue("(dd)", lowerLeft.x, lowerLeft.y));
    PyList_SetItem(corners, 1, Py_BuildValue("(dd)", upperLeft.x, upperLeft.y));
    PyList_SetItem(corners, 2, Py_BuildValue("(dd)", upperRight.x, upperRight.y));
    PyList_SetItem(corners, 3, Py_BuildValue("(dd)", lowerRight.x, lowerRight.y));

    return corners;
}

static PyPolygon *PyPolygon_copy(PyPolygon *self) {

    PyPolygon *new_polygon = PyObject_New(PyPolygon, &PyPolygonType);
    new_polygon->n_vertices = self->n_vertices;

    new_polygon->vertices = (Vetor*)malloc(sizeof(Vetor) * new_polygon->n_vertices);
    for(int i = 0; i < new_polygon->n_vertices; i++) {
        new_polygon->vertices[i] = self->vertices[i];
    }

    new_polygon->isThisConvex = self->isThisConvex;
    new_polygon->pivotPoint = self->pivotPoint;
    new_polygon->bbCenter = self->bbCenter;
    new_polygon->bbHalfHeight = self->bbHalfHeight;
    new_polygon->bbHalfWidth = self->bbHalfWidth;
    new_polygon->allignAngle = self->allignAngle;

    new_polygon->py_vertices = PyList_New(self->n_vertices);

    for(int i = 0; i < new_polygon->n_vertices; i++) {
        PyList_SetItem(
            new_polygon->py_vertices, i,
            Py_BuildValue("(ddd)", new_polygon->vertices[i].x, new_polygon->vertices[i].y, new_polygon->vertices[i].z)
        );
    }

    return new_polygon;
}

PyObject *PyPolygon_str(PyPolygon *self) {
    std::string res("[");

    for(int i = 0; i < self->n_vertices; i++) {
        char buffer[50];
        sprintf_s(buffer, "(%.1f, %.1f)", self->vertices[i].x, self->vertices[i].y);
        res += std::string(buffer);
        if(i < (self->n_vertices - 1)) {
            res += std::string(" ");
        }
    }
    return PyUnicode_FromFormat("%s]", res.c_str());
}

static PyMethodDef PyPolygon_methods[] = {
    {
        "copy",
        (PyCFunction)PyPolygon_copy, METH_NOARGS,
        "Makes a copy of this object"
    },
    {
        "getOriginalEnvelope",
        (PyCFunction)PyPolygon_getOriginalEnvelope, METH_NOARGS,
        "Gets the original, un-rotated envelope for this polygon. This envelope is the one used to check collisions"
    },
    {
        "getEnvelope",
        (PyCFunction)PyPolygon_getEnvelope, METH_NOARGS,
        "Get the envelope of this polygon"
    },
    {
        "isInside",
        (PyCFunction)PyPolygon_isInside, METH_VARARGS | METH_KEYWORDS,
        "Checks whether a point is inside this polygon"
    },
    {
        "toConvexHull",
        (PyCFunction)PyPolygon_toConvexHull, METH_NOARGS,
        "Converts a concave polygon to a convex one. Idempotent on convex polygons"
    },
    {
        "checkEnvelopeIntersection", (PyCFunction)PyPolygon_checkEnvelopeIntersection, METH_VARARGS | METH_KEYWORDS,
        "checks whether two polygons intersect, based on envelopes"
    },
    {NULL, NULL, 0, NULL}
};

static PyMemberDef PyPolygon_members[] = {
    {"allignAngle", T_DOUBLE, offsetof(PyPolygon, allignAngle), 0, "Angle used to find bounding box"},
    {"n_vertices", T_INT, offsetof(PyPolygon, n_vertices), 0, "number of vertices of this polygon"},
    {"vertices", T_OBJECT, offsetof(PyPolygon, py_vertices), 0, "vertices of this polygon"},
    {"isThisConvex", T_BOOL, offsetof(PyPolygon, isThisConvex), 0, "whether this polygon is convex or not"},
    {NULL}  /* Sentinel */
};


#endif // POLYGON