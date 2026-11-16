#include <Python.h>
#include <Windows.h>
#include <cmath>

class test_obj {
public:
    test_obj();
    ~test_obj();

    bool init(int v1, int v2);
    void release();

	int get_v1() const { return v1; }
private:
    int v1;
    int v2;
    int* ar;
};

test_obj::test_obj() : v1(0), v2(0), ar(nullptr) {
    ;
} 
test_obj::~test_obj() {
	release();
}
bool test_obj::init(int v1, int v2) {
    this->v1 = v1;
    this->v2 = v2;
    ar = new int[2];
    ar[0] = v1;
    ar[1] = v2;
    return true;
}
void test_obj::release() {
	if (ar != nullptr) {
		delete[] ar;
		ar = nullptr;
	}
    v1 = 0;
    v2 = 0;
}
static void test_obj_del(PyObject* capsule) {
	void* ptr = PyCapsule_GetPointer(capsule, "test_obj_ptr");
	test_obj* test_obj_inst = static_cast<test_obj*>(ptr);
	delete test_obj_inst;
}
static PyObject* test_obj_new(PyObject* self, PyObject* args) {
	test_obj* test_obj_inst = new test_obj();
	return PyCapsule_New(static_cast<void*>(test_obj_inst), "test_obj_ptr", test_obj_del);
}
static PyObject* test_obj_init(PyObject* self, PyObject* args) {
	PyObject* capsule;
	if (!PyArg_ParseTuple(args, "O", &capsule)) {
		return nullptr;
	}
	test_obj* test_obj_inst = static_cast<test_obj*>(PyCapsule_GetPointer(capsule, "test_obj_ptr"));
	if (!test_obj_inst) {
		PyErr_SetString(PyExc_RuntimeError, "Invalid capsule");
		return nullptr;
	}
    test_obj_inst->init(3, 4);
	Py_RETURN_NONE;
}

static PyObject* test_obj_read(PyObject* self, PyObject* args) {
	PyObject* capsule;
	if (!PyArg_ParseTuple(args, "O", &capsule)) {
		return nullptr;
	}
	test_obj* test_obj_inst = static_cast<test_obj*>(PyCapsule_GetPointer(capsule, "test_obj_ptr"));
	if (!test_obj_inst) {
		PyErr_SetString(PyExc_RuntimeError, "Invalid capsule");
		return nullptr;
	}
	int v1 = test_obj_inst->get_v1();
	return PyLong_FromLong(v1);
}

static PyMethodDef TestCaps_methods[] = {
	{ "new_test", (PyCFunction)test_obj_new, METH_VARARGS, nullptr},
	{ "init_test", (PyCFunction)test_obj_init, METH_VARARGS, nullptr},
	{ "read_test", (PyCFunction)test_obj_read, METH_VARARGS, nullptr},
	{ nullptr, nullptr, 0, nullptr }
};
static struct PyModuleDef TestCaps_module = {
	PyModuleDef_HEAD_INIT,
	"TestCaps",                        // Module name to use with Python import statements
	"test of using objects without python deleting them",  // Module description
	-1,
	TestCaps_methods                   // Structure that defines the methods of the module
};

PyMODINIT_FUNC PyInit_TestCaps() {
    return PyModule_Create(&TestCaps_module	);
}