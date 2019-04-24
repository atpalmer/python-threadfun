#include <pthread.h>
#include <Python.h>


static struct thread_args {
    int iter_count;
};


static void *run_thread(void *_args) {
    struct thread_args *args = _args;
    printf("Iteration: %d\n", args->iter_count);
    return NULL;
}


static PyObject *threadfun(PyObject *self, PyObject *args) {
    int thread_start;
    int thread_end;
    if(!PyArg_ParseTuple(args, "ii", &thread_start, &thread_end)) {
        return NULL;
    }

    int thread_count = thread_end - thread_start;
    if(thread_count < 1) {
        PyErr_SetString(PyExc_ValueError, "Thread count cannot be less than 1");
        return NULL;
    }

    pthread_t threads[thread_count];
    struct thread_args thread_args[thread_count];

    for (int i = thread_start; i < thread_end; ++i) {
        thread_args[i].iter_count = i;
        pthread_create(&threads[i], NULL, run_thread, &thread_args[i]);
    }

    for (int i = thread_start; i < thread_end; ++i) {
        thread_args[i].iter_count = i * 2;
        pthread_create(&threads[i], NULL, run_thread, &thread_args[i]);
    }

    for (int i = thread_start; i < thread_end; ++i) {
        pthread_join(threads[i], NULL);
    }

    return Py_BuildValue("{sisi}", "start", thread_args[thread_start].iter_count, "end", thread_args[thread_end - 1].iter_count);
}


static PyMethodDef module_methods[] = {
    { "threadfun", threadfun, METH_VARARGS, "" },
    { NULL, NULL, 0, NULL },
};


static struct PyModuleDef module_def = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "threadfun",
    .m_doc = "",
    .m_size = 0,
    .m_methods = module_methods,
};


PyMODINIT_FUNC PyInit_threadfun(void) {
    return PyModule_Create(&module_def);
}
