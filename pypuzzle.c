#include <Python.h>
#include <puzzle.h>

#undef UNUSED
#define UNUSED(var)     ((void)&var)

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

static PyObject *
get_distance(PyObject *self, PyObject *args)
{
    const char *file_path_1;
    const char *file_path_2;
    
    if (!PyArg_ParseTuple(args, "ss", &file_path_1, &file_path_2)){
        return NULL;
    }
    
    PuzzleCvec cvec_1, cvec_2;

    puzzle_init_cvec(&self->context, &cvec_1);
    puzzle_init_cvec(&self->context, &cvec_2);

    puzzle_fill_cvec_from_file(&self->context, &cvec_1, file_path_1);
    puzzle_fill_cvec_from_file(&self->context, &cvec_2, file_path_2);

    double distance = puzzle_vector_normalized_distance(&self->context, &cvec_1, &cvec_2, 1);
    
    puzzle_free_cvec(&self->context, &cvec_1);
    puzzle_free_cvec(&self->context, &cvec_2);

    return Py_BuildValue("d", distance);
}

static PyMethodDef PyPuzzleMethods[] = {
    {"get_distance", puzzle_get_distance, METH_VARARGS, "Get the distance between two images."},
    {"set_max_width", puzzle_set_max_width, METH_VARARGES, "Set the max width of images. Default is 3000 pixels."},
    {"set_max_height", puzzle_set_max_height, METH_VARARGES, "Set the max height of images. Default is 3000 pixels."},
    {"set_lambdas", puzzle_set_lambdas, METH_VARARGES, "Set the lambdas value. Images are divided in lambda x lambda blocks. Default is 9."},
    {"set_p_ratio", puzzle_set_p_ratio, METH_VARARGES, "Set the p ratio. The p ratio determines the size of the centered zone which the average intensity of each block is based upon. Default is 2.0."},
    {"set_noise_cutoff", puzzle_set_noise_cutoff, METH_VARARGES, "Set the noise cutoff value. If you raise that value, more zones with little difference of intensity will be considered as similar. Default is 2."},
    {"set_contrast_barrier_for_cropping", puzzle_set_contrast_barrier_for_cropping, METH_VARARGES, "Set the tolerance of auto-cropping borders. Default is 5."},
    {"set_max_cropping_ratio", puzzle_set_max_cropping_ratio, METH_VARARGES, "Set the safe-guard value againt unwanted excessive auto-cropping. Defalut is 0.25."},
    {"set_autocrop", puzzle_set_autocrop, METH_VARARGES, "Any value except 0 will enable auto-cropping. It is enabled by default."},
    {"Puzzle", (PyCFunction)puzzle_new, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef PuzzleObjectMethods[] = {
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
typedef struct {
    PyObject_HEAD
    PuzzleContext context,
    unsigned int max_width,
    unsigned int max_height,
    unsigned int lambdas,
    double p_ratio,
    double noise_cutoff,
    double contrast_barrier_for_cropping,
    double max_cropping_ratio,
    int autocrop
} PuzzleObject;

PyTypeObject Puzzle_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                          /* ob_size */
    "pypuzzle.Puzzle",          /* tp_name */
    sizeof(PuzzleObject),       /* tp_basicsize */
    0,                          /* tp_itemsize */
    /* Methods */
    0,                          /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
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

void
puzzle_dealloc(PuzzleObject *self)
{
    puzzle_free_context(&(self->context));
    PyMem_DEL(self);
}

static PyObject *
get_distance(PyObject *self, PyObject *args)
{
    const char *file_path_1;
    const char *file_path_2;
    
    if (!PyArg_ParseTuple(args, "ss", &file_path_1, &file_path_2)){
        return NULL;
    }
    
    PuzzleContext context;
    PuzzleCvec cvec_1, cvec_2;

    puzzle_init_context(&context);
    puzzle_init_cvec(&context, &cvec_1);
    puzzle_init_cvec(&context, &cvec_2);

    puzzle_fill_cvec_from_file(&context, &cvec_1, file_path_1);
    puzzle_fill_cvec_from_file(&context, &cvec_2, file_path_2);

    double distance = puzzle_vector_normalized_distance(&context, &cvec_1, &cvec_2, 1);
    
    puzzle_free_cvec(&context, &cvec_1);
    puzzle_free_cvec(&context, &cvec_2);
    puzzle_free_context(&context);

    return Py_BuildValue("d", distance);
}

static PyMethodDef PyPuzzleMethods[] = {
    {"get_distance", get_distance, METH_VARARGS, "Get the distance between two images."},
    {"Puzzle", (PyCFunction)puzzle_new, METH_NOARGS, ""},
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
