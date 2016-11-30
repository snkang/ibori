/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string.h>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"
#include "dbconn.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

// =================== Private variables ====================
int idpw_cnt = 0, ck_cnt = 0;
int sn, cn;

dbconn::dbconn(){}

dbconn::~dbconn(){}


bool dbconn::insertData2DB(match::DATA *dataset, uint data_cnt) {

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt1, *pstmt2;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("localhost:3306", "root", "toor");

        /* Connect to the MySQL ibory database */
        con->setSchema("ibory");


        pstmt1 = con->prepareStatement("INSERT INTO ibory.getpasswd (sn, url, id, passwd, cdate) VALUES (?, ?, ?, ?, ?)");

        for (int i=0; i<data_cnt; i++) {

            printf("\n dataset[%d].url : %s, empty() : %d", i, dataset[i].url.c_str(), dataset[i].url.empty());
            printf("\n dataset[%d].id : %s, empty() : %d", i, dataset[i].id.c_str(), dataset[i].id.empty());
            printf("\n dataset[%d].password : %s, empty() : %d\n", i, dataset[i].password.c_str(), dataset[i].password.empty());

            if ( dataset[i].url.empty() || dataset[i].id.empty() || dataset[i].password.empty() ) continue;

            pstmt1->setInt(1, dataset[i].cdate);        // sn
            pstmt1->setString(2, dataset[i].url);       // url
            pstmt1->setString(3, dataset[i].id);        // id
            pstmt1->setString(4, dataset[i].password);  // passwd

            if ( dataset[i].cdate != 0 ) {
                char *text = (char*)malloc(sizeof(char)*20);
                getStringDate(dataset[i].cdate, text);
                pstmt1->setDateTime(5, text);           // date
            } else {
                pstmt1->setDateTime(5, "");
            }
            pstmt1->executeUpdate();
        }


        pstmt2 = con->prepareStatement("INSERT INTO ibory.getcookie (cn, url, cookie, cdate) VALUES (?, ?, ?, ?)");

        for (int i=0; i<data_cnt; i++) {

            if ( dataset[i].url.empty() || dataset[i].cookie.empty() ) continue;

            pstmt2->setInt(1, dataset[i].cdate);      // cn
            pstmt2->setString(2, dataset[i].url);     // url
            pstmt2->setString(3, dataset[i].cookie);  // cookie

            if ( dataset[i].cdate != 0 ) {
                char *text = (char*)malloc(sizeof(char)*20);
                getStringDate(dataset[i].cdate, text);
                pstmt2->setDateTime(4, text);        // date
            } else {
                pstmt2->setDateTime(4, "");
            }
            pstmt2->executeUpdate();
        }
        delete pstmt1, pstmt2;
        delete con;

    } catch (sql::SQLException &e) {
        printf("# ERR: SQLException in %s ( %s ) on line %s\n", __FILE__, __FUNCTION__, __LINE__);
        printf("# ERR: $s %s\n", e.what());
        printf("  (MySQL error code: $s, SQLState: %s )\n", e.getErrorCode(), e.getSQLState());
    }
}

void dbconn::getStringDate(time_t cdate, char* text) {
    strftime(text, 20, "%Y-%m-%d %H:%M:%S", localtime(&cdate));
//    printf("Date : %s\n", text);
}

int dbconn::genSerialNumber(int *num_cnt, int *data_cnt, time_t cdate) {
    tm *t = localtime(&cdate);
    char* num, buffer;

    sprintf(&buffer, "%d", t->tm_year-100);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_mon+1);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_mday);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_hour);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_min);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_sec);
    num += buffer;

    return atoi(num);
}


