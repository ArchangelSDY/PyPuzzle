#include <Python.h>
#include <puzzle.h>

#undef UNUSED
#define UNUSED(var)     ((void)&var)

#define DEFAULT_MAX_WIDTH           (3000)
#define DEFAULT_MAX_HEIGHT          (3000)
#define DEFAULT_LAMBDAS             (9)
#define DEFAULT_P_RATIO             (2.0)
#define DEFAULT_NOISE_CUTOFF        (2)
#define DEFAULT_CONTRAST_BARRIER   (5)
#define DEFAULT_MAX_CROPPING_RATIO  (0.25)
#define DEFAULT_AUTOCROP            (1)

/* Safe XDECREF for object states that handles nested deallocations */
#define ZAP(v) do {\
        PyObject *tmp = (PyObject *)(v); \
        (v) = NULL; \
        Py_XDECREF(tmp); \
} while (0)

static PyTypeObject *p_Puzzle_Type = NULL;

typedef struct {
    PyObject_HEAD
    PyObject *dict;
    PuzzleContext context;
    unsigned int max_width;
    unsigned int max_height;
    unsigned int lambdas;
    double p_ratio;
    double noise_cutoff;
    double contrast_barrier_for_cropping;
    double max_cropping_ratio;
    int autocrop;
} PuzzleObject;

static PuzzleObject *
puzzle_new(PyObject *dummy)
{
    UNUSED(dummy);

    PuzzleObject *self = NULL;

    self = (PuzzleObject *)PyObject_GC_New(PuzzleObject, p_Puzzle_Type);
    if (self == NULL) {
        return NULL;
    }
    PyObject_GC_Track(self);

    puzzle_init_context(&self->context);
    self->dict = NULL;

    return self;
}

static void
puzzle_dealloc(PuzzleObject *self)
{
    PyObject_GC_UnTrack(self);
    Py_TRASHCAN_SAFE_BEGIN(self);

    ZAP(self->dict);
    puzzle_free_context(&self->context);
    
    PyObject_GC_Del(self);
    Py_TRASHCAN_SAFE_END(self);
}


static PyObject *
get_distance(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;

    const char *file_path_1;
    const char *file_path_2;
    
    if (!PyArg_ParseTuple(args, "ss", &file_path_1, &file_path_2)){
        return NULL;
    }
    
    PuzzleCvec cvec_1, cvec_2;

    puzzle_init_cvec(&po->context, &cvec_1);
    puzzle_init_cvec(&po->context, &cvec_2);

    puzzle_fill_cvec_from_file(&po->context, &cvec_1, file_path_1);
    puzzle_fill_cvec_from_file(&po->context, &cvec_2, file_path_2);

    double distance = puzzle_vector_normalized_distance(&po->context, &cvec_1, &cvec_2, 1);
    
    puzzle_free_cvec(&po->context, &cvec_1);
    puzzle_free_cvec(&po->context, &cvec_2);

    return Py_BuildValue("d", distance);
}

static PyObject *
set_max_width(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    unsigned int max_width = DEFAULT_MAX_WIDTH;

    if (!PyArg_ParseTuple(args, "I", &max_width)) {
        return NULL;
    }

    int result = puzzle_set_max_width(&po->context, max_width);
    
    return Py_BuildValue("i", result);
}

static PyObject *
set_max_height(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    unsigned int max_height = DEFAULT_MAX_HEIGHT;

    if (!PyArg_ParseTuple(args, "I", &max_height)) {
        return NULL;
    }

    int result = puzzle_set_max_height(&po->context, max_height);

    return Py_BuildValue("i", result);
}

static PyObject *
set_lambdas(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    unsigned int lambdas = DEFAULT_LAMBDAS;

    if (!PyArg_ParseTuple(args, "I", &lambdas)) {
        return NULL;
    }

    int result = puzzle_set_lambdas(&po->context, lambdas);

    return Py_BuildValue("i", result);
}

static PyObject *
set_p_ratio(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    double p_ratio = DEFAULT_P_RATIO;

    if (!PyArg_ParseTuple(args, "d", &p_ratio)) {
        return NULL;
    }

    int result = puzzle_set_p_ratio(&po->context, p_ratio);

    return Py_BuildValue("i", result);
}

static PyObject *
set_noise_cutoff(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    double noise_cutoff = DEFAULT_NOISE_CUTOFF;

    if (!PyArg_ParseTuple(args, "d", &noise_cutoff)) {
        return NULL;
    }
    
    int result = puzzle_set_noise_cutoff(&po->context, noise_cutoff);

    return Py_BuildValue("i", result);
}

static PyObject *
set_contrast_barrier_for_cropping(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    double barrier = DEFAULT_CONTRAST_BARRIER;

    if (!PyArg_ParseTuple(args, "d", &barrier)) {
        return NULL;
    }

    int result = puzzle_set_contrast_barrier_for_cropping(&po->context, barrier);

    return Py_BuildValue("i", result);
}

static PyObject *
set_max_cropping_ratio(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    double ratio = DEFAULT_MAX_CROPPING_RATIO;

    if (!PyArg_ParseTuple(args, "d", &ratio)) {
        return NULL;
    }

    int result = puzzle_set_max_cropping_ratio(&po->context, ratio);

    return Py_BuildValue("i", result);
}

static PyObject *
set_autocrop(PyObject *self, PyObject *args)
{
    PuzzleObject *po = (PuzzleObject *)self;
    int enable = DEFAULT_AUTOCROP;

    if (!PyArg_ParseTuple(args, "i", &enable)) {
        return NULL;
    }

    int result = puzzle_set_autocrop(&po->context, enable);

    return Py_BuildValue("i", result);
}

static PyMethodDef PyPuzzleMethods[] = {
    {"Puzzle", (PyCFunction)puzzle_new, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef PuzzleObjectMethods[] = {
    {"get_distance", get_distance, METH_VARARGS, "Get the distance between two images."},
    {"set_max_width", set_max_width, METH_VARARGS, "Set the max width of images. Default is 3000 pixels."},
    {"set_max_height", set_max_height, METH_VARARGS, "Set the max height of images. Default is 3000 pixels."},
    {"set_lambdas", set_lambdas, METH_VARARGS, "Set the lambdas value. Images are divided in lambda x lambda blocks. Default is 9."},
    {"set_p_ratio", set_p_ratio, METH_VARARGS, "Set the p ratio. The p ratio determines the size of the centered zone which the average intensity of each block is based upon. Default is 2.0."},
    {"set_noise_cutoff", set_noise_cutoff, METH_VARARGS, "Set the noise cutoff value. If you raise that value, more zones with little difference of intensity will be considered as similar. Default is 2."},
    {"set_contrast_barrier_for_cropping", set_contrast_barrier_for_cropping, METH_VARARGS, "Set the tolerance of auto-cropping borders. Default is 5."},
    {"set_max_cropping_ratio", set_max_cropping_ratio, METH_VARARGS, "Set the safe-guard value againt unwanted excessive auto-cropping. Defalut is 0.25."},
    {"set_autocrop", set_autocrop, METH_VARARGS, "Any value except 0 will enable auto-cropping. It is enabled by default."},
    {NULL, NULL, 0, NULL}
};

static PyObject *
puzzle_get_attr(PuzzleObject *self, char *attrname)
{
    PyObject *value = NULL;
    if (value == NULL && self->dict !=NULL) {
        value = PyDict_GetItemString(self->dict, attrname);
        if (value != NULL ) {
            Py_INCREF(value);
            return value;
        }
    }
    return Py_FindMethod(PuzzleObjectMethods, (PyObject *)self, attrname);
}

static int
puzzle_set_attr(PuzzleObject *self, char *attrname, PyObject *value)
{
    if (value == NULL) {
        int rv = -1;
        if (self->dict != NULL) {
            rv = PyDict_DelItemString(self->dict, attrname);
        }
        if (rv < 0) {
            PyErr_SetString(PyExc_AttributeError, "Non-existing attribute");
        }
        return rv;
    }
    if (self->dict == NULL) {
        self->dict = PyDict_New();
        if (self->dict == NULL) {
            return -1;
        }
    }
    return PyDict_SetItemString(self->dict, attrname, value);
}

static PyTypeObject Puzzle_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                          /* ob_size */
    "pypuzzle.Puzzle",          /* tp_name */
    sizeof(PuzzleObject),       /* tp_basicsize */
    0,                          /* tp_itemsize */
    /* Methods */
    (destructor)puzzle_dealloc, /* tp_dealloc */
    0,                          /* tp_print */
    (getattrfunc)puzzle_get_attr,   /* tp_getattr */
    (setattrfunc)puzzle_set_attr,   /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    0,                          /* tp_flags */
    0,                          /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
};

PyMODINIT_FUNC
initpypuzzle(void)
{
    p_Puzzle_Type = &Puzzle_Type;
    (void) Py_InitModule("pypuzzle", PyPuzzleMethods);
}

int
main(int argc, char *argv[])
{
    // Pass argv[0] to the Python interpreter.
    Py_SetProgramName(argv[0]);
    
    // Initialize the Python interpreter.
    Py_Initialize();

    // Add a static moduel.
    initpypuzzle();

    return 0;
}
