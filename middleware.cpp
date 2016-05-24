#include <stdio.h>
#include <algorithm>
#include <python2.7/Python.h>
#include <mysql.h>
#include <time.h>
#include <stdlib.h>

#include "roi.hpp"
#include "features.hpp"
#include "utilities.hpp"

#define RESULTS 20

PyObject* testFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory);

extern "C" {
    PyObject* TestFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory)
        {
            testFunction(fileName, colorPriority, texturePriority, shapePriority, productCategory);
        }
}

long getCurrentTime() {
    struct timeval t;
    gettimeofday(&t, 0);
    return 1000000 * t.tv_sec + t.tv_usec;
}

std::vector<std::string> searchImages(char* fileName, double colorWeight, double textureWeight, double shapeWeight, char* productCategory) {

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    std::string server = "shuttle.cfs9lab4dyj4.us-west-2.rds.amazonaws.com";
    std::string user = "psp2133";
    std::string password = "horcrux1";
    std::string database = "AdvancePotionMaking";
    int port = 3306;

    long time1 = getCurrentTime();
    // Connect to the Database
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), 
                            database.c_str(), port, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
 
    std::string colorMeasure = " " + std::to_string(colorWeight) + "*distColor ";
    std::string shapeMeasure = " " + std::to_string(shapeWeight) + "*distShape ";
    std::string textureMeasure;
    if (std::string(productCategory) == "NeckTies") {
        textureMeasure = " " + std::to_string(textureWeight) + "*distTexture ";
    }
    else {
        textureMeasure = " " + std::to_string(textureWeight) + "*(1 - (surfMatches/maxSurfMatches)) ";
    }

    // Query the database for the best matches
    std::string query = "SELECT file2 FROM (SELECT file2, (" + colorMeasure + "+" + shapeMeasure + "+" + textureMeasure + ") as totalMeasure FROM " + productCategory + ", (SELECT max(surfMatches) AS maxSurfMatches FROM " + productCategory + ") Temp WHERE CONCAT(\"/home/parita/Github/AdvancedPotionMaking/\", file1) LIKE \"" + fileName + "\" ORDER BY totalMeasure LIMIT 10) TempAnother";
    if (mysql_query(conn, query.c_str())) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);
    std::vector<std::string> results;
    while ((row = mysql_fetch_row(res)) != NULL) {
        results.push_back(row[0]);
    }

    // Time taken to process the query
    std::cout << "Time taken: " << getCurrentTime() - time1 << std::endl;

    return results;
}

PyObject* testFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory) {

    PyObject *result = PyList_New(0);

    printf("File: %s | Color Priority: %d | Texture Priority: %d | Shape Priority: %d | Product Category: %s\n", fileName, colorPriority, texturePriority, shapePriority, productCategory);

    int sumPriorities = colorPriority + texturePriority + shapePriority;
    double colorWeight = (double)colorPriority / (double)sumPriorities;
    double textureWeight = (double)texturePriority / (double)sumPriorities;
    double shapeWeight = (double)shapePriority / (double)sumPriorities;

    vector<string> resultImages = searchImages(fileName, colorWeight, textureWeight, shapeWeight, productCategory);

    for (int i = 0; i < resultImages.size(); i++) {
        PyList_Append(result, PyString_FromFormat(resultImages[i].c_str()));
    }

    std::cout << "Done\n";
    return result;
}
