#include <stdio.h>
#include <python2.7/Python.h>

PyObject* testFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory);

extern "C" {
    PyObject* TestFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory)
        {
            testFunction(fileName, colorPriority, texturePriority, shapePriority, productCategory);
        }
}

PyObject* testFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory) {

    PyObject *result = PyList_New(0);

    printf("File: %s | Color Priority: %d | Texture Priority: %d | Shape Priority: %d | Product Category: %s\n", fileName, colorPriority, texturePriority, shapePriority, productCategory);

    for (int i = 0; i < 10; i++) {
        PyList_Append(result, PyString_FromFormat("/home/viral/AdvancedPotionMaking/test.jpg"));
    }

    return result;
}
