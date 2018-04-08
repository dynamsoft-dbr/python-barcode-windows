#include <Python.h>
#include "DynamsoftBarcodeReader.h"
#include <ndarraytypes.h>

#if PY_MAJOR_VERSION >= 3
#ifndef IS_PY3K
#define IS_PY3K 1
#endif
#endif

struct module_state {
    PyObject *error;
};

#if defined(IS_PY3K)
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *
error_out(PyObject *m) {
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
    return NULL;
}

#define DBR_NO_MEMORY 0
#define DBR_SUCCESS   1

// #define LOG_OFF

#ifdef LOG_OFF

    #define printf(MESSAGE, __VA_ARGS__)

#endif

// Barcode reader handler
void* hBarcode = NULL; 

/**
 * Create DBR instance
 */
static int createDBR() 
{
    if (!hBarcode) {
        hBarcode = DBR_CreateInstance();
        if (!hBarcode)
        {
            printf("Cannot allocate memory!\n");
            return DBR_NO_MEMORY;
        }
    }

    return DBR_SUCCESS;
}

/**
 * Destroy DBR instance
 */
static void destroyDBR()
{
    if (hBarcode) {
        DBR_DestroyInstance(hBarcode);
    }
}

static PyObject *
create(PyObject *self, PyObject *args)
{
    int ret = createDBR();
    return Py_BuildValue("i", ret);
}

static PyObject *
destroy(PyObject *self, PyObject *args)
{
    destroyDBR();
    return Py_BuildValue("i", 0);
}

/**
 * Set Dynamsoft Barcode Reader license.  
 * To get valid license, please contact support@dynamsoft.com
 * Invalid license is acceptable. With an invalid license, SDK will return an imcomplete result.
 */
static PyObject *
initLicense(PyObject *self, PyObject *args)
{
    if (!createDBR()) 
    {
        return NULL;
    }

    char *pszLicense;
    if (!PyArg_ParseTuple(args, "s", &pszLicense)) {
        return NULL;
    }

	int ret = DBR_InitLicense(hBarcode, pszLicense);
    return Py_BuildValue("i", ret);
}

// Load settings from template files.
static PyObject *
loadSettings(PyObject *self, PyObject *args)
{
    if (!createDBR()) 
    {
        return NULL;
    }

    char *pszSettingFile;
    if (!PyArg_ParseTuple(args, "s", &pszSettingFile)) {
        return NULL;
    }

    char szErrorMsg[256];
	int ret = DBR_LoadSettingsFromFile(hBarcode, pszSettingFile, szErrorMsg, 256);
    return Py_BuildValue("i", ret);
}

static PyObject *createPyResults(STextResultArray *paryResult)
{
    if (!paryResult)
    {
        printf("No barcode detected\n");
        return NULL;
    }
    // Get barcode results
    int count = paryResult->nResultsCount;

    // Create a Python object to store results
    PyObject* list = PyList_New(count); 
    printf("count: %d\n", count);
    PyObject* result = NULL;
    for (int i = 0; i < count; i++)
    {
        PyList_SetItem(list, i, Py_BuildValue("ss", paryResult->ppResults[i]->pszBarcodeFormatString, paryResult->ppResults[i]->pszBarcodeText)); // Add results to list
    }

    // Release memory
    DBR_FreeTextResults(&paryResult);

    return list;
}

/**
 * Decode barcode from a file 
 */
static PyObject *
decodeFile(PyObject *self, PyObject *args)
{
    if (!createDBR()) 
    {
        return NULL;
    }

    char *pFileName; // File name
    int iFormat;     // Barcode formats
    char *pszTemplateName; // template name
    if (!PyArg_ParseTuple(args, "sis", &pFileName, &iFormat, &pszTemplateName)) {
        return NULL;
    }

    STextResultArray *paryResult = NULL;

    // Barcode detection
    int ret = DBR_DecodeFile(hBarcode, pFileName, pszTemplateName);
    DBR_GetAllTextResults(hBarcode, &paryResult);

    // Wrap results
    PyObject *list = createPyResults(paryResult);
    return list;
}

/**
 * Decode barcode from an image buffer. 
 */
static PyObject *
decodeBuffer(PyObject *self, PyObject *args)
{
    if (!createDBR()) 
    {
        return NULL;
    }

    PyObject *o;
    int iFormat;
    char *pszTemplateName; // template name
    if (!PyArg_ParseTuple(args, "Ois", &o, &iFormat, &pszTemplateName))
        return NULL;

    #if defined(IS_PY3K)
    //Refer to numpy/core/src/multiarray/ctors.c
    Py_buffer *view;
    int nd;
    PyObject *memoryview = PyMemoryView_FromObject(o);
    if (memoryview == NULL) {
        PyErr_Clear();
        return -1;
    }

    view = PyMemoryView_GET_BUFFER(memoryview);
    char *buffer = (char*)view->buf;
    nd = view->ndim;
    int len = view->len;
    int stride = view->strides[0];
    int width = view->strides[0] / view->strides[1];
    int height = len / stride;
    #else

    PyObject *ao = PyObject_GetAttrString(o, "__array_struct__");

    if ((ao == NULL) || !PyCObject_Check(ao)) {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        return NULL;
    }

    PyArrayInterface *pai = (PyArrayInterface*)PyCObject_AsVoidPtr(ao);
    
    if (pai->two != 2) {
        PyErr_SetString(PyExc_TypeError, "object does not have array interface");
        Py_DECREF(ao);
        return NULL;
    }

    // Get image information
    char *buffer = (char*)pai->data; // The address of image data
    int width = pai->shape[1];       // image width
    int height = pai->shape[0];      // image height
    int size = pai->strides[0] * pai->shape[0]; // image size = stride * height
    #endif

    // Initialize Dynamsoft Barcode Reader
    STextResultArray *paryResult = NULL;

    // Detect barcodes
    int depth = 24;
    int iStride = ((width * depth + 31) >> 5) << 2;

    PyObject *list = NULL;
    int iRet = DBR_DecodeBuffer(hBarcode, buffer, width, height, iStride, IPF_RGB_888, pszTemplateName);
    // Wrap results
    DBR_GetAllTextResults(hBarcode, &paryResult);
    list = createPyResults(paryResult);
    
    #if defined(IS_PY3K)
    Py_DECREF(memoryview);
    #else
    Py_DECREF(ao);
    #endif

    return list;
}

static PyMethodDef dbr_methods[] =
{
    {"create", create, METH_VARARGS, NULL},
    {"destroy", destroy, METH_VARARGS, NULL},
    {"initLicense", initLicense, METH_VARARGS, NULL},
    {"loadSettings", loadSettings, METH_VARARGS, NULL},
    {"decodeFile", decodeFile, METH_VARARGS, NULL},
    {"decodeBuffer", decodeBuffer, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

#if defined(IS_PY3K)

static int dbr_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int dbr_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "dbr",
    NULL,
    sizeof(struct module_state),
    dbr_methods,
    NULL,
    dbr_traverse,
    dbr_clear,
    NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_dbr(void)

#else
#define INITERROR return
void
initdbr(void)
#endif
{
#if defined(IS_PY3K)
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("dbr", dbr_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("dbr.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if defined(IS_PY3K)
    return module;
#endif
}