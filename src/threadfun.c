#include <pthread.h>
#include <Python.h>


struct thread_args {
    int thread;
    int start;
    int end;
};


static void *run_thread(void *_args) {
    struct thread_args *args = _args;
    for(int i = args->start; i < args->end; ++i) {
        printf("Thread: %d; Iteration: %d\n", args->thread, i);
    }
    return NULL;
}


static PyObject *threadfun(PyObject *self, PyObject *args) {
    int start_val;
    int end_val;
    int thread_count = 4;
    if(!PyArg_ParseTuple(args, "iii", &start_val, &end_val, &thread_count)) {
        return NULL;
    }

    if(thread_count < 1) {
        PyErr_SetString(PyExc_ValueError, "Thread count cannot be less than 1");
        return NULL;
    }

    pthread_t threads[thread_count];
    struct thread_args thread_args[thread_count];

    for(int i = 0; i < thread_count; ++i) {
        thread_args[i].thread = i;
        thread_args[i].start = (i + 1) * start_val;
        thread_args[i].end = (i + 1) * end_val;
        pthread_create(&threads[i], NULL, run_thread, &thread_args[i]);
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    return Py_BuildValue("{sisi}", "very_start", thread_args[0].start, "very_end", thread_args[thread_count - 1].end);
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
