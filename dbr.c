#include <Python.h>
#include "DynamsoftBarcodeReader.h"
#include <ndarraytypes.h>

#define DBR_NO_MEMORY 0
#define DBR_SUCCESS   1

// #define LOG_OFF

#ifdef LOG_OFF

    #define printf(MESSAGE, __VA_ARGS__)

#endif

// Barcode reader handler
void* hBarcode = NULL; 

// Supported barcode formats
const char * GetFormatStr(int format)
{
	if (format == BF_CODE_39)
		return "CODE_39";
	if (format == BF_CODE_128)
		return "CODE_128";
	if (format == BF_CODE_93)
		return "CODE_93";
	if (format == BF_CODABAR)
		return "CODABAR";
	if (format == BF_ITF)
		return "ITF";
	if (format == BF_UPC_A)
		return "UPC_A";
	if (format == BF_UPC_E)
		return "UPC_E";
	if (format == BF_EAN_13)
		return "EAN_13";
	if (format == BF_EAN_8)
		return "EAN_8";
	if (format == BF_INDUSTRIAL_25)
		return "INDUSTRIAL_25";
	if (format == BF_QR_CODE)
		return "QR_CODE";
	if (format == BF_PDF417)
		return "PDF417";
	if (format == BF_DATAMATRIX)
		return "DATAMATRIX";

	return "UNKNOWN";
}

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

	int ret = DBR_InitLicenseEx(hBarcode, pszLicense);
    return Py_BuildValue("i", ret);
}

static PyObject *createPyResults(SBarcodeResultArray *pResults)
{
    // Get barcode results
    int count = pResults->iBarcodeCount;
	SBarcodeResult** ppBarcodes = pResults->ppBarcodes;
	SBarcodeResult* tmp = NULL;

    // Create a Python object to store results
    PyObject* list = PyList_New(count); 
    PyObject* result = NULL;
    for (int i = 0; i < count; i++)
    {
        tmp = ppBarcodes[i];
        result = PyString_FromString(tmp->pBarcodeData);
        PyList_SetItem(list, i, Py_BuildValue("sN", GetFormatStr(tmp->emBarcodeFormat), result)); // Add results to list
    }

    // Release memory
    DBR_FreeBarcodeResults(&pResults);

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

    char *pFileName;
    int iFormat;
    if (!PyArg_ParseTuple(args, "si", &pFileName, &iFormat)) {
        return NULL;
    }

    // Initialize Dynamsoft Barcode Reader
	int iMaxCount = 0x7FFFFFFF;
	SBarcodeResultArray *pResults = NULL;
	DBR_SetBarcodeFormats(hBarcode, iFormat);
	DBR_SetMaxBarcodesNumPerPage(hBarcode, iMaxCount);

    // Barcode detection
    int ret = DBR_DecodeFileEx(hBarcode, pFileName, &pResults);

    // Wrap results
    PyObject *list = createPyResults(pResults);
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
    if (!PyArg_ParseTuple(args, "Oi", &o, &iFormat))
        return NULL;

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

    // Initialize Dynamsoft Barcode Reader
    int iMaxCount = 0x7FFFFFFF;
	SBarcodeResultArray *pResults = NULL;
	DBR_SetBarcodeFormats(hBarcode, iFormat);
	DBR_SetMaxBarcodesNumPerPage(hBarcode, iMaxCount);

    // Detect barcodes
    int iRet = DBR_DecodeBufferEx(hBarcode, buffer, width, height, width * 3, IPF_RGB_888, &pResults);
    
    // Wrap results
    PyObject *list = createPyResults(pResults);
    
    Py_DECREF(ao);
    return list;
}

static PyMethodDef Methods[] =
{
    {"create", create, METH_VARARGS, NULL},
    {"destroy", destroy, METH_VARARGS, NULL},
    {"initLicense", initLicense, METH_VARARGS, NULL},
    {"decodeFile", decodeFile, METH_VARARGS, NULL},
    {"decodeBuffer", decodeBuffer, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdbr(void)
{
     (void) Py_InitModule("dbr", Methods);
}
