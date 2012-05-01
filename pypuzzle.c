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

    self = (PuzzleObject *)PyObject_NEW(PuzzleObject, &Puzzle_Type);
    if (self != NULL) {
        puzzle_init_context(&(self->context));
    }
    
    return self;
}

static PyMethodDef PyPuzzleMethods[] = {
    {"Puzzle", (PyCFunction)puzzle_new, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

void
puzzle_dealloc(PuzzleObject *self)
{
    puzzle_free_context(&(self->context));
    PyMem_DEL(self);
}

static PyObject *
puzzle_get_attr(PuzzleObject *self, char *attrname)
{
    PyObject *value = NULL;
    if (value == NULL & self->dict !=NULL) {
        value = PyDict_GetItemString(self->dict, attrname);
        if (value != NULL ) {
            Py_INCREF(value);
            return value;
        }
    }
    return Py_FindMethod(PuzzleObjectMethods, self, attrname);
}

static int
puzzle_set_attr(PuzzleObject *self, char *attrname, PyObject *value)
{
    if (value == NULL) {
        int rv = -1;
        if (self->dict != NULL) {
            rv = PyDict_DelItemString(self->dictm attrname);
        }
        if (rv < 0) {
            PyErr_SetString(PyExc_AttributeError, "Non-existing attribute");
        }
        return rv;
    }
    if (self->dict == NULL) {
        self->dict = PyDict_NEW();
        if (self->dict == NULL) {
            return -1;
        }
    }
    return PyDict_SetItemString(self->dict, attrname, value);
}

PyTypeObject Puzzle_Type = {
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


static PyMethodDef PuzzleObjectMethods[] = {
    {"get_distance", get_distance, METH_VARARGS, "Get the distance between two images."},
    {"set_max_width", set_max_width, METH_VARARGES, "Set the max width of images. Default is 3000 pixels."},
    {"set_max_height", set_max_height, METH_VARARGES, "Set the max height of images. Default is 3000 pixels."},
    {"set_lambdas", set_lambdas, METH_VARARGES, "Set the lambdas value. Images are divided in lambda x lambda blocks. Default is 9."},
    {"set_p_ratio", set_p_ratio, METH_VARARGES, "Set the p ratio. The p ratio determines the size of the centered zone which the average intensity of each block is based upon. Default is 2.0."},
    {"set_noise_cutoff", set_noise_cutoff, METH_VARARGES, "Set the noise cutoff value. If you raise that value, more zones with little difference of intensity will be considered as similar. Default is 2."},
    {"set_contrast_barrier_for_cropping", set_contrast_barrier_for_cropping, METH_VARARGES, "Set the tolerance of auto-cropping borders. Default is 5."},
    {"set_max_cropping_ratio", set_max_cropping_ratio, METH_VARARGES, "Set the safe-guard value againt unwanted excessive auto-cropping. Defalut is 0.25."},
    {"set_autocrop", set_autocrop, METH_VARARGES, "Any value except 0 will enable auto-cropping. It is enabled by default."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initpypuzzle(void)
{
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
