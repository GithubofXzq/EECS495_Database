//
//  main.cpp
//  sh
//
//  Created by 孙昊 on 16/11/9.
//  Copyright © 2016年 孙昊. All rights reserved.
//

#include <mysql.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
MYSQL *conn;

void login ();
void Studentmanu(string);
void Transcript(string);
void Detailinfo(string,string);
void Enroll(string,string*);
void Withdraw(string);
string* PersonalDetails(string id);
void checkpre(string,string,string,int);


int main (int argc, const char * argv[])
{
    conn=mysql_init(NULL);
    //open connection
    mysql_real_connect(conn, "localhost", "root", "Tywzsh19931105+", "project3-nudb", 0, 0, 0);
    while(1){
        login();
    }
}


void login (){                                         //login
    string id, password;
    cout<<"***********************************"<<endl;
    cout<<"***********************************"<<endl;
    cout<<"*please input your id and password*"<<endl;
    cout<<"***********************************"<<endl;
    cout<<"***********************************"<<endl;
    cout<<"***enter id****"<<endl;                     //enter id
    cin>>id;
    cout<<"***enter password***"<<endl;                //enter password
    cin>>password;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    mysql_query(conn,"SELECT Id, Password FROM Student;");
    res_set=mysql_store_result(conn);
    int numrows=(int)mysql_num_rows(res_set);
    //display results
    for(int i=0;i<numrows;i++){
        row=mysql_fetch_row(res_set);
        if(row!=NULL){
            if((row[0]==id)&&(row[1]==password)){
                cout<<"welcome to Caesar"<<endl;         //enter into Caesar
                string *p;
                p=new string[2];
                p[0]="250";                            //set the default value for max number of students preference
                p[1]="NULL";   //set the default value for nonprefered classroom type
                MYSQL_RES *res_set3;
                MYSQL_ROW row3;
                string str3="SELECT NonPreferredClassroomType FROM student where Id="+id+";";
                mysql_query(conn,str3.c_str());
                res_set3=mysql_store_result(conn);
                if(res_set3!=NULL){
                    int numrows3=(int)mysql_num_rows(res_set3);
                    //display results
                    for(int i=0;i<numrows3;i++){
                        row3=mysql_fetch_row(res_set3);
                        if(row3!=NULL){
                            if(row3[0]==NULL){
                                p[1]="NULL";
                            }else
                                p[1]=row3[0];
                        }
                    }
                }
                while(1){
                    Studentmanu(id);                         //display Studentmanu
                    cout<<"what operations do you what to do"<<endl;
                    string operation;                        //choose the operation
                    cin>>operation;
                    if(operation=="Transcript"){         //Transcript operation
                        Transcript(id);
                        continue;
                    }
                    if(operation=="Enroll"){             // Enroll operation
                        Enroll(id,p);
                        continue;
                    }
                    if(operation=="Withdraw"){           // Withdraw operation
                        Withdraw(id);
                        continue;
                    }
                    if(operation=="PersonalDetails"){       // look for personal info
                        p=PersonalDetails(id);
                        continue;
                    }
                    if(operation=="logout")              //logout
                        break;
                }
                return;
            }
        }
    }
    cout<<"sorry, your password is wrong"<<endl;         // if the password is wrong
}

// studentmanu function
void  Studentmanu(string stuid){
    MYSQL_RES *res_set1;
    MYSQL_ROW row1;
    time_t timeSec=time (0);
    struct tm * timeinfo= localtime ( &timeSec );
    int year1=timeinfo->tm_year+1900;
    string year=to_string(year1);                            //year
    int month=timeinfo->tm_mon+1;                            //month
    string quarter;                                          //quarter
    if((month>=9)&&(month<=12)){
        quarter="Q1";
    }
    if((month>=1)&&(month)<=3){
        quarter="Q2";
    }
    if((month>=4)&&(month<=6)){
        quarter="Q3";
    }
    if((month>=7)&&(month<=8)){
        quarter="Q4";
    }
    cout<<"****************************"<<endl;
    cout<<"The current course is below."<<endl;
    cout<<"****************************"<<endl;
    mysql_query(conn,"SELECT * FROM transcript;");
    res_set1=mysql_store_result(conn);
    int numrows=(int)mysql_num_rows(res_set1);
    //display results
    for(int i=0;i<numrows;i++){
        row1=mysql_fetch_row(res_set1);
        if(row1!=NULL){
            if((stuid==row1[0])&&(quarter==row1[2])&&(year==row1[3])&&(row1[4]==NULL)){
                cout<<"the course id is: "<<row1[1]<<endl;             //output current course
            }
        }
    }
}
// end studentmanu

//display the transcript and some information about courses
void  Transcript(string stuid){
    cout<<"************************"<<endl;
    cout<<"This is full transcript."<<endl;                           //display full transcript
    cout<<"************************"<<endl;
    MYSQL_RES *res_set1;
    MYSQL_ROW row1;
    string str="SELECT * FROM transcript where StudId="+stuid+";";
    mysql_query(conn,str.c_str());
    res_set1=mysql_store_result(conn);
    if(res_set1!=NULL){
        int numrows=(int)mysql_num_rows(res_set1);
        //display results
        for(int i=0;i<numrows;i++){
            row1=mysql_fetch_row(res_set1);
            if(row1!=NULL){
                cout<<"the course id is: "<<row1[1];
                if(row1[4]==NULL){
                    cout<<"  the grade is: "<<"NULL"<<endl;
                }else
                    cout<<"  the grade is: "<<row1[4]<<endl;
            }
        }
    }
    while(1){
        cout<<"What additional options do you want to do? "<<endl;        //additional options
        string opt;
        cin>>opt;
        if(opt=="Detailinfo"){                                            //lookfor detail info
            cout<<"please input the course code"<<endl;
            string cd;
            cin>>cd;
            Detailinfo(stuid,cd);
        }
        if(opt=="back"){                                              //back
            break;
        }
    }
}
//Detailinfo function
void Detailinfo(string id,string cd){
    MYSQL_RES *res_set1;
    MYSQL_ROW row1;
    string str="select StudId, UoSName, UoSCode, Semester, Year, Enrollment, MaxEnrollment, Name, Grade from (select StudId, UoSName, UoSCode, Semester, Year, Enrollment, MaxEnrollment, InstructorId as Id, Grade from (select StudId, UoSCode, Semester, Year, Enrollment, MaxEnrollment, InstructorId, Grade from uosoffering natural join transcript) A natural join unitofstudy) B natural join faculty    WHERE StudId="+id+" and UoSCode='"+cd+"';";
    mysql_query(conn,str.c_str());
    res_set1=mysql_store_result(conn);
    if(res_set1!=NULL){
        int numrows=(int)mysql_num_rows(res_set1);
        //display results
        for(int i=0;i<numrows;i++){
            row1=mysql_fetch_row(res_set1);
            if(row1!=NULL){
                cout<<"the course id is: "<<row1[2]<<endl;
                cout<<"the course title is: "<<row1[1]<<endl;
                cout<<"the semester is: "<<row1[3]<<endl;
                cout<<"the year is: "<<row1[4]<<endl;
                cout<<"the erollment is: "<<row1[5]<<endl;
                cout<<"the maxenrollment is: "<<row1[6]<<endl;
                cout<<"the lecturer name is: "<<row1[7]<<endl;
                if(row1[8]==NULL){
                    cout<<"  the grade is: "<<"NULL"<<endl;
                }else
                    cout<<"  the grade is: "<<row1[8]<<endl;
            }
        }
    }
}

//Enroll function
void Enroll(string id, string *p){
    MYSQL_RES *res_set1;
    MYSQL_ROW row1;
    time_t timeSec=time (0);
    struct tm * timeinfo= localtime ( &timeSec );
    int year1=timeinfo->tm_year+1900;
    int year2=year1+1;
    string year=to_string(year1);//year
    string year3=to_string(year2);
    int month=timeinfo->tm_mon+1;                            //month
    string quarter1,quarter2;                                //quarter
    if((month>=9)&&(month<=12)){
        quarter1="Q1";
        quarter2="Q2";
    }
    if((month>=1)&&(month)<=3){
        quarter1="Q2";
        quarter2="Q3";
    }
    if((month>=4)&&(month<=6)){
        quarter1="Q3";
        quarter2="Q4";
    }
    if((month>=7)&&(month<=8)){
        quarter1="Q4";
        quarter2="Q1";
    }
    cout<<"****************************"<<endl;
    cout<<"The course to be selected is below."<<endl;
    cout<<"****************************"<<endl;
    
    mysql_query(conn,"select UoSCode,Semester,Year,Textbook,Enrollment,MaxEnrollment,Type from (select * from uosoffering natural join lecture) A natural join classroom;");
    res_set1=mysql_store_result(conn);
    int numrows=(int)mysql_num_rows(res_set1);
    //display results
    for(int i=0;i<numrows;i++){
        row1=mysql_fetch_row(res_set1);
        if(row1!=NULL){
            int numberset=atoi(p[0].c_str());   //transfer string to int
            int numbermax=atoi(row1[5]);        //transfer char* to int
            
            if((((quarter1==row1[1])&&(year==row1[2]))||((quarter2==row1[1])&&(year3==row1[2])))&&(row1[6]!=p[1])&&(numbermax<=numberset)){
                cout<<"the course id is: "<<row1[0]<<endl;
                cout<<"the quarter is: "<<row1[1]<<endl;
                cout<<"the year is: "<<row1[2]<<endl;//output current course
            }
        }
    }
    string selectcourse, selectquarter, selectyear;
    cout<<"please inpiut the course"<<endl;
    cin>>selectcourse;
    cout<<"please inpiut the quarter"<<endl;
    cin>>selectquarter;
    cout<<"please inpiut the year"<<endl;
    cin>>selectyear;
    MYSQL_RES *res_set2;
    MYSQL_ROW row2;
    string str1="call Enrollcourse12('"+selectcourse+"','"+selectquarter+"','"+selectyear+"','"+id+"',@output);";
    if(mysql_query(conn,str1.c_str()) == 0)//to handel the returened value of the ENROLL procedure
    {
        do
        {
            if(mysql_field_count(conn) > 0)
            {
                res_set2 = mysql_store_result(conn);
                //free resources
                mysql_free_result(res_set2);
            }
            
        } while(mysql_next_result(conn) == 0);
    }
    row2 = mysql_fetch_row(res_set2);
                cout<<"**************************************"<<endl;
                cout<<"**the information of result is below**"<<endl;
                cout<<"**************************************"<<endl;
                cout<<row2[0]<<endl;
                string s="lackpre";
                if(row2[0]==s){
                    int numberset1=atoi(selectyear.c_str());
                    cout<<"************************************************"<<endl;
                    cout<<"***you need to take these prerequisite course***"<<endl;
                    cout<<"************************************************"<<endl;
                    checkpre(selectcourse,id,selectquarter,numberset1);
                }
                string s1= "course enrolled";
                if(row2[0]==s1){
                    MYSQL_RES *res_set0;
                    string warning = "select signal0 from flag;";
                    const char* warning_sent = warning.c_str();
                    if (mysql_query(conn, warning_sent)) {
                        cout << "query failed in trigger: mysql_query\n";
                        //mysql_free_result(res_set);
                        exit(EXIT_FAILURE);
                    }
                    if ((res_set0 = mysql_store_result(conn)) == 0) {
                        mysql_free_result(res_set0);
                        cout << "mysql_store_result falied in trigger\n";
                        //mysql_free_result(res_set_temp);
                        exit(EXIT_FAILURE);
                    }
                    int numrows = (int)mysql_num_rows(res_set0);
                    mysql_free_result(res_set0);
                    if (numrows >= 1) {
                        cout << "below 50%!!!!\n";
                    }
                }
            }


void Withdraw(string stuid){
    MYSQL_RES *res_set2;
    MYSQL_ROW row2;
    string selectcourse;
    cout<<"************************************************"<<endl;
    cout<<"**Please input the course you want to withdraw**"<<endl;
    cout<<"************************************************"<<endl;
    cin>>selectcourse;
    string str1="call Withdraw('"+selectcourse+"','"+stuid+"', @output);";
    if(mysql_query(conn,str1.c_str()) == 0)//to handel the returened value of the ENROLL procedure
    {
        do
        {
            if(mysql_field_count(conn) > 0)
            {
                res_set2 = mysql_store_result(conn);
                //free resources
                mysql_free_result(res_set2);
            }
            
        } while(mysql_next_result(conn) == 0);
    }
    if(res_set2!=NULL){
    row2 = mysql_fetch_row(res_set2);
            cout<<"**************************************"<<endl;
            cout<<"**the information of result is below**"<<endl;
            cout<<"**************************************"<<endl;
            cout<<row2[0]<<endl;
    }
    
    MYSQL_RES *res_set0;
    string warning = "select signal0 from flag;";
    const char* warning_sent = warning.c_str();
    if (mysql_query(conn, warning_sent)) {
        cout << "query failed in trigger: mysql_query\n";
        //mysql_free_result(res_set);
        exit(EXIT_FAILURE);
    }
    if ((res_set0 = mysql_store_result(conn)) == 0) {
        mysql_free_result(res_set0);
        cout << "mysql_store_result falied in trigger\n";
        //mysql_free_result(res_set_temp);
        exit(EXIT_FAILURE);
    }
    int numrows = (int)mysql_num_rows(res_set0);
    mysql_free_result(res_set0);
    if (numrows >= 1) {
        cout << "below 50%!!!!\n";
    }
}
string* PersonalDetails(string id){
    string *p;
    p=new string[2];
    cout<<"please input your preference about maximum number of students"<<endl;
    cin>>p[0];
    cout<<"please input your non-preferred classroom type"<<endl;
    cin>>p[1];
    return p;              //return the preference about maximum number of students and non-preferred classroom type/
}

void checkpre(string uoscode, string id,string chosenquarter,int chosenyear) {
    string query_transcript = "select allpre.prerequoscode from (select distinct r.prerequoscode from requires r where r.UoSCode in('" + uoscode + "')) as allpre left join (select distinct t.uoscode,t.grade from transcript t where t.studid = " + id + ") as taken on allpre.prerequoscode = taken.uoscode where taken.uoscode is NULL or taken.grade is  null or taken.grade ='D';";
    const char* query_checkpre_sent = query_transcript.c_str();
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    if (mysql_query(conn, query_checkpre_sent)) {
        cout << "query failed in checkpre: mysql_query\n";
        //mysql_free_result(res_set);
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    
    if ((res_set = mysql_store_result(conn)) == 0) {
        mysql_free_result(res_set);
        cout << "mysql_store_result falied in checkrpe()\n";
        exit(EXIT_FAILURE);
    }
    int numrows = (int)mysql_num_rows(res_set);
    if(numrows==0){
        cout << "enrolling\n";
        
        string query_transcript = "call enrolling("+id+",'"+uoscode+"','"+chosenquarter+"',"+to_string(chosenyear)+");";
        const char* query_enrolling_sent = query_transcript.c_str();
        if (mysql_query(conn, query_enrolling_sent)) {
            cout << "query failed in checkpre: mysql_query\n";
            //mysql_free_result(res_set);
            exit(EXIT_FAILURE);
        }
        if ((res_set = mysql_store_result(conn)) == 0) {
            mysql_free_result(res_set);
            //cout << "mysql_store_result falied in enrolling()\n";
            //mysql_free_result(res_set_temp);
            cout << "go back check update\n";
            return;
        }
        cout << "enrolling failed\n";
    }
    else {
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row(res_set);
            if (row != NULL) {
                cout << "need UoSCode : " << row[0] << endl;
            }
        }
    }
    mysql_free_result(res_set);
    return ;
}
