#include <mysql.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "utilities.hpp"
#include "features.hpp"

using namespace std;

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    string server = "shuttle.cfs9lab4dyj4.us-west-2.rds.amazonaws.com";
    string user = "psp2133";
    //set the password for mysql server here
    string password = "horcrux1"; /* set me first */
    string database = "AdvancePotionMaking";
    int port = 3306;

    /* Connect to database */
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), 
                            database.c_str(), port, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    // A Table for each Category
    string types[5] = {"NeckTies", "Watches", "Shoes", "Bags", "TShirts"};
    for (int iType = 0; iType < numTypes; iType++) {
        vector<string> files;
        getDir(types[iType], files);
        // Location of the image files
        string loc = "/home/parita/Github/AdvancedPotionMaking/";
        // Specify the table for the INSERT
        string insertTable = "insert into " + types[iType] + " values";
        
        // Compare every two file for every category */
        for (int iFile = files.size()-1; iFile >= 0; iFile--) {
            cout << "File: " << files[iFile] << endl;
            for (int jFile = iFile - 1; jFile >= 0; jFile--) {
                // Check if the images are compared already
                string checkExisting = "SELECT 1 from " + types[iType] + " WHERE file1 LIKE \'" + files[iFile] + "\' AND file2 LIKE \'" + files[jFile] + "\'";
                if (mysql_query(conn, checkExisting.c_str())) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                }
                res = mysql_use_result(conn);
                // Abort if the comparison is already done
                if ((row = mysql_fetch_row(res)) != NULL) {
                    if (stoi(row[0]) == 1) {
                        cout << "Breaking\n";
                        continue;
                    }
                }
                cout << "File: " << files[jFile] << endl;
                // Specify both permutation of file names combination for the INSERT
                string insertQuery1 = insertTable + "(\'" + files[iFile] + "\', ";
                insertQuery1 += "\'" + files[jFile] + "\', ";
                string insertQuery2 = insertTable + "(\'" + files[jFile] + "\', \'" + files[iFile] + "\', ";

                float distColor = matchHist(&files[iFile], &files[jFile]);
                float distTexture = matchTexture(&files[iFile], &files[jFile]);
                float distShape = matchShape(&files[iFile], &files[jFile]);
                float surfMatches = matchSURF(&files[iFile], &files[jFile]);

                // Update the measurements for the combination                
                insertQuery1 += to_string(distColor) + ", " + to_string(distTexture) + ", " + to_string(distShape) + ", " + to_string(surfMatches) + ")";
                insertQuery2 += to_string(distColor) + ", " + to_string(distTexture) + ", " + to_string(distShape) + ", " + to_string(surfMatches) + ")";

                // Execute INSERTs to the table
                if (mysql_query(conn, insertQuery1.c_str())) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                }

                if (mysql_query(conn, insertQuery2.c_str())) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                }
            }
            mysql_free_result(res);
        }
    }
        
    /* close connection */
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
