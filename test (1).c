#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH_COMMAND 300

#define MAX_NO_TASKS 100

#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33

#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20

enum Status {IN_PROGRESS, DONE, ARCHIVED};
char * status_name[] = {"In Progress", "Done", "Archived"};
enum CommandType {ADD, EDIT, SHOW, DELETE, QUIT, INVALID};
char * command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};

struct Task {
    int num;
    char title[MAX_LENGTH_TITLE+1];
    char description[MAX_LENGTH_DESCRIPTION+1];
    char time[MAX_LENGTH_TIME+1];
    enum Status status;
};

void printTask(struct Task * task) {
    printf("--------------------------------------------\n");
    printf("Num: #%d. Title: %s\n", task->num, task->title);
    printf("Description: %s\n", task->description);
    printf("Status: %s\n", status_name[task->status]);
    printf("--------------------------------------------\n");
}

void printUnsupportedTime(struct Task * task) {
    printf("----- Show week view -----\n");
    printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
    printf("In Task:\n");
    printTask(task);
}
// ------ Begin: Student Answer ------
enum CommandType getCommandType(char * command) {
    // TODO
    if(strlen(command) == 0) return INVALID;
    char temp[strlen(command) + 1];
    strcpy(temp, command);
    char *token =  strtok(temp, " ");

    for(int i =0 ; i < strlen(token);i++){
        if(token[i] >= 'a'){
            token[i] -= 32;
        }
    }
    // printf("%s \n", token);
    // enum CommandType type = ADD;
    if(strcmp(token,"ADD") == 0){
        return ADD;
    }
    if(strcmp(token,"EDIT") == 0){
        return EDIT;
    }
    if(strcmp(token,"SHOW") == 0){
        return SHOW;
    }
    if(strcmp(token,"DELETE") == 0){
        return DELETE;
    }
    if(strcmp(token,"QUIT") == 0){
        return QUIT;
    }
    return INVALID;
}

void getSubstring(const char *inputString, char* startIndex, char* endIndex, char *outputSubstring) {
    strncpy(outputSubstring, startIndex + 1, endIndex - startIndex + 1);
    int length = strlen(outputSubstring) - 1;
    while(outputSubstring[length] != ']') length--;
    outputSubstring[length] = '\0';
}

// 3.2

void getInfo(char* command, char* info, int index){
    char * startIndex = strstr(command, "["); // find first square bracket
    if(!startIndex) return;
    char * endIndex = strstr(startIndex, "]"); // find the respective close bracket
    while(--index){
        startIndex = strstr(startIndex + 1, "[");
        if(!startIndex || !endIndex) break;
        endIndex = strstr(startIndex, "]");
    }
    if(index != 0) return;
    getSubstring(command, startIndex, endIndex, info);
}

void getTitleFromAdd(char * command, char* out_title){
    // todo
    getInfo(command,out_title, 1);
}
//3.2
void getDescriptionFromAdd(char * command, char * out_description){
    // todo
    getInfo(command,out_description, 2);
}
//3.2
void getTimeFromAdd(char * command, char * out_time){
    // todo
    getInfo(command,out_time, 3);
}
// Other functions

// 3.3
int checkTitle(char* raw_title){
    if(strlen(raw_title) > MAX_LENGTH_TITLE){
        return strlen(raw_title);
    }
    
    if(raw_title[0] == ' ') return 0;
   
    if(raw_title[strlen(raw_title) - 1] == ' ') return strlen(raw_title) - 1;

    for(int i = 0 ;i < strlen(raw_title);i++){
        if(!(raw_title[i] >= '0' && raw_title[i] <= '9') &&
            !(raw_title[i] >= 'a' && raw_title[i] <= 'z') &&
            !(raw_title[i] >= 'A' && raw_title[i] <= 'Z') &&
            !(raw_title[i] == ' ') && !(raw_title[i] == ',') &&
            !(raw_title[i] == '.') && !(raw_title[i] == '-') &&
            !(raw_title[i] == ':') && !(raw_title[i] == '|') &&
            !(raw_title[i] == '/'))
        // regex
        return i;
    }

    return -1;
}
// 3.4
int checkDescription(char * raw_description){
   if(strlen(raw_description) > MAX_LENGTH_DESCRIPTION){
        return strlen(raw_description);
    }

    if(raw_description[0] == ' ') return 0;
    if(raw_description[strlen(raw_description) - 1] == ' ') return strlen(raw_description) - 1;

    for(int i = 0;i < strlen(raw_description);i++){
        if(!(raw_description[i] >= '0' && raw_description[i] <= '9') &&
            !(raw_description[i] >= 'a' && raw_description[i] <= 'z') &&
            !(raw_description[i] >= 'A' && raw_description[i] <= 'Z') &&
            !(raw_description[i] == ' ') && !(raw_description[i] == ',') &&
            !(raw_description[i] == '.') && !(raw_description[i] == '-') &&
            !(raw_description[i] == ':') && !(raw_description[i] == '|') &&
            !(raw_description[i] == '/'))
        // regex
    
        return i;
    }

    return -1;
}

int dayInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 30, 30, 31, 30, 31};
int checkValidInt(char* num, int minVal, int maxVal){
    char temp[strlen(num) + 1];
    strcpy(temp,  num); //2 dòng copy chuỗi num vào chuỗi temp
    for( int i = 0 ;i < strlen(num);i++){
        if(temp[i] < '0' || temp[i] > '9'){ //ktra temp[i] có phải là số không
            return 0; //có thì trả về giá trị 0, thoát hàm
        } 
    }
    if(minVal != -1 && maxVal != -1){//nếu minVal khác - 1 và maxVal khác -1
        return (atoi(num) >= minVal && atoi(num) <= maxVal); //trả về 1 hoặc 0 tùy theo kết quả ktra num có nằm trong [minVal,maxVal]
    }                                                       // đúng trả về 1, sai trả về 0
    return 1;
}
int checkLeapYear(int year){
    if (year % 400 == 0 || year % 4 == 0) {
      return 1;
   }
   return 0;
}
int checkHourMinute(char * time, char* hour, char* minute,int index){
    char temp[strlen(time) + 1];
    strcpy(temp,  time);
    char * token = strtok(temp, ":");
    int i = 0 ;
    while(token != NULL){
        if(i == 0){
            strcpy(hour, token);
        }
        else{
            strcpy(minute, token);
        }
        i++;
        token = strtok(NULL, ":");
    }
    if (checkValidInt(hour, 0, 23) == 0){
        char error[100] = "11";
        if(index == 2){
            error[1] = '2';
        }
        strcat(error, hour);
        return atoi(error);
    }
    if(checkValidInt(minute, 0 , 59) == 0){
        char error[100] = "21";
        if(index == 2){
            error[1] = '2';
        }
        strcat(error, minute);
        return atoi(error);
    }
    return -1;
}
int checkDate(char * date, char * day, char* month, char* year, int index){
    char temp[strlen(date) + 1];
    strcpy(temp,  date);
    char * token = strtok(temp, "/");
    int i =0 ;
    while(token != NULL){
        if(i == 0){
            strcpy(day, token);
        }
        else if(i == 1){
            strcpy(month, token);
        }
        else{
            strcpy(year, token);
        }
        i++;
        token = strtok(NULL, "/");
    }
    if(checkValidInt(year, 1000, 9999) == 0){
        // fail year
        char error[100] = "51";
        if(index == 2){
            error[1] = '2';
        }
        if(atoi(year) != 0){
            int n_year = atoi(year);
            snprintf( year, 4, "%d", n_year );
        } 
        strcat(error, year);
        return atoi(error);
    }
    if(checkValidInt(month, 1, 12) == 0){
        // fail month
        char error[100] = "41";
        if(index == 2){
            error[1] = '2';
        }
        strcat(error, month);
        return atoi(error);
    }
    if(checkValidInt(day, -1, -1)){
        if(checkLeapYear(atoi(year))){
            dayInMonth[1] = 29;
        }
        if(checkValidInt(day, 1, dayInMonth[atoi(month) - 1]) == 0){
            // fail day
            char error[100] = "31";
            if(index == 2){
                error[1] = '2';
            }
            strcat(error, day);
            return atoi(error);
        }
    }
    else{
        // fail day
        char error[100] = "31";
        if(index == 2){
            error[1] = '2';
        }
        strcat(error, day);
        return atoi(error);
    }
    return -1;
}
int checkDateTime(char* datetime, char* day, char* month, char* year, char* hour, char* minute, int index){
    char temp[strlen(datetime) + 1];
    strcpy(temp,  datetime);

    char * token = strtok(temp, "|");
    char * time = NULL;
    char * date = NULL;

    while(token != NULL){
        if(time == NULL){
            time = token;
        }
        else{
            date = token;
        }
        token = strtok(NULL, "|");
    }
    int checkTime = checkHourMinute(time, hour, minute, index);
    int checkDay = checkDate(date, day, month, year, index);
    if(checkTime != -1)
    {
        return checkTime;
    }
    if(checkDay != -1){
        return checkDay;
    }
    return -1;
}
// 3.5
int checkTime(char * raw_time){
    char temp[strlen(raw_time) + 1];
    strcpy(temp,  raw_time);
    
    // split day
    char * token = strtok(temp, "-");
    char * datetime1 = NULL;
    char * datetime2 = NULL;

    char day1[strlen(raw_time)];
    char month1[strlen(raw_time)];
    char year1[strlen(raw_time)];
    char hour1[strlen(raw_time)];
    char minute1[strlen(raw_time)];

    char day2[strlen(raw_time)];
    char month2[strlen(raw_time)];
    char year2[strlen(raw_time)];
    char hour2[strlen(raw_time)];
    char minute2[strlen(raw_time)];
    // extract datetime
    while(token != NULL){
        if(datetime1 == NULL){
            datetime1 = token;
        }
        else{
            datetime2 = token;
        }
        token = strtok(NULL, "-");
    }
    int checkDay1 = checkDateTime(datetime1, day1, month1, year1, hour1, minute1, 1);
    if(checkDay1 != -1){
        return checkDay1;
    }
    int checkDay2 = checkDateTime(datetime2, day2, month2, year2, hour2, minute2, 2);
    if(checkDay2 != -1){
        return checkDay2;
    }
    if(atoi(year1) > atoi(year2)) return 0;
    if(atoi(month1) > atoi(month2)) return 0;
    if(atoi(day1) > atoi(day2)) return 0;
    if(atoi(hour1) > atoi(hour2)) return 0;
    if(atoi(hour1) == atoi(hour2)){
        if(atoi(minute1) >= atoi(minute2)) return 0;
    }
    else{
        if(atoi(minute1) > atoi(minute2)) return 0;
    }

    return -1;

}

// ===== EDIT ======
// 3.6
// todo
void getTitleFromEdit(char * command, char * out_title){
    getInfo(command, out_title, 1);
}
void getDescriptionFromEdit(char * command, char * out_description){
    getInfo(command, out_description, 1);
}
void getTimeFromEdit(char * command, char * out_time){
    getInfo(command, out_time, 1);
}

// 3.7
int getNumFromCommand(char * command){
    char* temp = new char[strlen(command) + 1]; //
    strcpy(temp,  command);         //2 dòng đầu để copy chuỗi command vào chuỗi temp

    char* token = strtok(temp, "#");    //con trỏ token trỏ đến vị trí kí tự '#' đầu tiên trong chuỗi temp
    char num_part[MAX_LENGTH_TIME + 1]; //khai báo biến num_part có 34 phần tử
    int i = 2;
    while(token != NULL){ //khi token không phải con trỏ NULL thì thực hiện vòng lặp
        if(i == 0) break; //nếu i = 0 thoát vòng lặp
        strcpy(num_part, token);//copy chuỗi token vào chuỗi num_part
        token = strtok(NULL, "#");//token trỏ đến vị trí kí tự '#' tiếp theo
        i--; 
    }
    if(i != 0) return -1; // have # or not 
    if(num_part[0] < '0' || num_part[0] > '9') return 0; // num is valid
    char * edt_num = strtok(num_part, " "); //con trỏ edt_num trỏ đến vị trí khoảng trắng đầu tiên tính từ vị trí con trỏ num_part
    if(checkValidInt(edt_num, -1, -1)){ //nếu kết quả trả về của hàm checkValidInt là 1 thì thực hiện câu lệnh trong if
        return atoi(edt_num); // trả về chuỗi edt_num dưới dạng int
    }
    else return 0;
}

// 3.8
int getFieldFromEdit(char * edit_cmd){
    char * field_array[] = {"title", "description", "time", "status"}; //khai báo mảng field_array gồm các phần tử {....}
    char temp[strlen(edit_cmd) + 1]; //khai báo chuỗi temp chứa (độ dài chuỗi edit_cmd + 1) kí tự
    strcpy(temp, edit_cmd); //copy chuỗi edit_cmd vào chuỗi temp
    char *token=  strtok(temp, " "); //con trỏ token trỏ đến vị trí khoảng trắng đầu tiên tính từ vị trí con trỏ temp
    char field[strlen(edit_cmd) + 1]; //khai báo mảng field chứa (độ dài chuỗi edit_cmd + 1) phần tử
    int i = 3;
    while(token != NULL){ //nếu con trỏ token k phải con trỏ NULL thì thực hiện vòng lặp
        if(i == 0) break; //nếu i = 0 thoát vòng lặp
        strcpy(field, token); //copy chuỗi token vào field
        token = strtok(NULL, " "); //con trỏ token trỏ đến vị trí khoảng trắng đầu tiên tính từ sau con trỏ token
        i--; 
    }
    if(i != 0) return 0; // right position
    char _field[strlen(field) + 1]; //khai báo mảng _field chứa (độ dài chuỗi field + 1) kí tự
    strcpy(_field, strtok(field, ":")); //copy con trỏ field từ vị trí dấu ":" đầu tiên vào mảng _field
    for(int i =0 ;i < sizeof(field_array)/ sizeof(field_array[0]);i++){ 
        if(strcmp(field_array[i], _field) == 0) return i + 1;
    }
    return 0;
}
 //3.9
enum Status getStatusFromEdit(char * edit_cmd){
    char status[200];
    getInfo(edit_cmd,status,1);
    int length = strlen(status);
    if(length > 1) return -1;
    if( status[0] == 'I' || status[0] == 'i') return IN_PROGRESS;
    if( status[0] == 'd' || status[0] == 'D') return DONE;
    if( status[0] == 'A' || status[0] == 'a') return ARCHIVED;
    return -1;
}


// 3.11
void printAllTasks(struct Task * array_tasks, int no_tasks){
    for(int i = 0 ;i < no_tasks;i++){
        printTask(&array_tasks[i]);
    }
}
// 3.12
void printTaskByNum(struct Task * array_tasks, int no_tasks, int num){
    for(int i = 0 ;i < no_tasks;i++){
        if(array_tasks[i].num == num)
            printTask(&array_tasks[i]);
    }
}
// 
void printHeadTasks(struct Task * array_tasks, int no_tasks, int quan){
    int print_num = no_tasks;
    if(quan < no_tasks) print_num = quan;
    for(int i = 0 ;i < print_num;i++){
        printTask(&array_tasks[i]);
    }

}
void printTailTasks(struct Task * array_tasks, int no_tasks, int quan){
    int print_num = no_tasks - quan;
    if(print_num < 0) print_num = 0;
    for(int i = print_num ;i < no_tasks;i++){
        printTask(&array_tasks[i]);
    }
}
int checkSubString(char * command, char* word){
    char temp[strlen(command) + 1];
    strcpy(temp,  command);

    char* token = strtok(temp, " ");

    while(token != NULL){
        if(strcmp(token, word) == 0) return 1;
        token = strtok(NULL, " ");
    }
    return 0;
}
void printFilteredTasksByTitle(struct Task * array_tasks, int no_tasks, char * filter_title){
    for(int i = 0 ;i < no_tasks;i++){
        char * dup = strstr(array_tasks[i].title,filter_title);
        if(dup)
            printTask(&array_tasks[i]);
        // if(checkSubString(array_tasks[i].title, filter_title)){
        //     printTask(&array_tasks[i]);
        // }
    }
}
void printFilteredTasksByDescription(struct Task * array_tasks, int no_tasks, char * filter_description){
    for(int i = 0 ;i < no_tasks;i++){
        char * dup = strstr(array_tasks[i].description,filter_description);
        if(dup)
            printTask(&array_tasks[i]);
        // if(checkSubString(array_tasks[i].description, filter_description)){
        //     printTask(&array_tasks[i]);
        // }
    }
}
void printFilteredTasksByStatus(struct Task * array_tasks, int no_tasks, enum Status filter_status){
    for(int i = 0 ;i < no_tasks;i++){
        if(array_tasks[i].status == filter_status){
            printTask(&array_tasks[i]);
        }
    }
}
// 3.12
// ADD TASK
bool addTask(struct Task * array_tasks, int no_tasks, char * new_title, char * new_description, char * new_time){
    if(no_tasks >= MAX_NO_TASKS) return false;
    struct Task newTask; // create need a const 
    newTask.num = no_tasks + 1;
    strcpy(newTask.title, new_title);
    strcpy(newTask.description, new_description);
    strcpy(newTask.time, new_time);
    newTask.status = 0;
    array_tasks[no_tasks] = newTask;
    return true;
}
bool deleteTask(struct Task * array_tasks, int no_tasks, int num){
    if(no_tasks == 0 || num > no_tasks) return false;
    for(int i = num - 1;i < no_tasks - 1;i++){
        // int temp_num = array_tasks[i].num;
        array_tasks[i] =  array_tasks[i + 1];
        // array_tasks[i].num = temp_num;
    }
    array_tasks[no_tasks - 1].num = 0;
    array_tasks[no_tasks - 1].status = 0;
    strcpy(array_tasks[no_tasks - 1].title,"");
    strcpy(array_tasks[no_tasks - 1].description,"");
    strcpy(array_tasks[no_tasks - 1].time,"");
    return true; 
}
void print_with_indent(int indent, char * s)
{
    printf("%*s%s", indent, "", s);
}
void print_space(int indent)
{
    printf("%*s", indent,"");
}
void print_cell(char* s, int indent){
    int num_space = (indent - strlen(s)) /2 ;
    char first_cell[strlen(s) + indent];
    print_with_indent(num_space, s);
    print_space(num_space);
}
void print_task_in_calendar(int i ,int j, struct Task t){

}
int printWeekTime(struct Task * array_tasks, int no_tasks, char * date){
    char * year = "2023";
    char* days[8] = {"", "MON", "TUE", "WED", "THU", "FRI", "SAT","SUN"};
    char * time[25] = {"", "00:00","01:00","02:00","03:00","04:00","05:00",
                        "06:00","07:00","08:00","09:00","10:00","11:00","12:00","13:00","14:00",
                        "15:00","16:00","17:00","18:00","19:00","20:00","21:00","22:00","23:00"};
    for(int i = 0 ; i < 25;i++){ // row
        for(int j = 0; j < 8;j++){ // col
            if(j == 0 && i == 0){ // first col first row
                print_cell(year, WEEK_CELL_FIRST_COL_WIDTH);
                printf("|");
            }
            else if(i == 0){
                print_cell(days[j], WEEK_CELL_OTHER_COL_WIDTH);
                printf("|");
            }
            else if(j == 0){
                print_cell(time[i], WEEK_CELL_FIRST_COL_WIDTH);
            }
        }
        // printf("\n-----------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("\n");
    }

    return 1;
}
// ------ End: Student Answer ------



void runTodoApp() {
    // Example of command Add
    char command[MAX_LENGTH_COMMAND+1];
    
    while (true) {
        // Sample input:
        // Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]
        fgets(command, MAX_LENGTH_COMMAND+1, stdin);
        command[strlen(command)-1] = '\0';

        enum CommandType commandType = getCommandType(command);
        printf("Command     : %s\n", command);
        printf("Command type: %s\n", command_name[commandType]);

        break;  // only one loop for simple test
                // actual app will break when encounter QUIT command
    }
}
void test_getCommandType(int start, int end)
{
//yeu cau 1
    //! {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};
    char command[10][MAX_LENGTH_COMMAND+1] = 
    {
        "Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]\0",
        "Edit  #2",
        "Show  #2",
        "Delete  #2",
        "    Quit  #2",
        "Add",
        " Add ",
        "A  #2",
        "Add1  #2",
        "",
    };

    enum CommandType result[20] = {ADD, EDIT, SHOW, DELETE, QUIT, ADD, ADD ,INVALID ,INVALID, INVALID , INVALID , INVALID};
   
    for(int i = start; i <= end; i++)
    {
        printf("Test case %d", i);
        enum CommandType commandType = getCommandType(command[i - start]);
        if(result[i-start] != commandType)
        {
            printf(" fail\n");
            printf("    Command              : %s\n", command[i - start]);
            printf("    code you Command type: %s\n", command_name[commandType]);
            printf("    exact Command type   : %s\n", command_name[result[i-start]]);
        }
        else printf(" pass\n");
    }
}

void test_get__FromAdd(int start, int end)
{
    //! {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};
    char command[10][MAX_LENGTH_COMMAND+1] = 
    {
        "Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]\0",
        "Edit  [a] [b] [c]",
        "Show  [a]   [b] [c]",
        "       Show  [a]   [b]   [c]",
        "Add [a] [] []",
        "Add [] [b] []",
        "Add [] [] [c]",
        "Add [] [] []",
        "Add [-] [-] [-]",
        "Add [k1K , . - : | /] [k] [k]",
    };
    char result[20][3][MAX_LENGTH_COMMAND+1] = {
        {"Course Intro to Programming", "Room 701-H6", "07:00|01/10/2023-12:00|01/10/2023"},
        {"a", "b", "c"},
        {"a", "b", "c"},
        {"a", "b", "c"},
        {"a", "", ""},
        {"", "b", ""},
        {"", "", "c"},
        {"", "", ""},
        {"-", "-", "-"},
        {"k1K , . - : | /", "k", "k"}
    };
    char title[MAX_LENGTH_TITLE+1];
    char description[MAX_LENGTH_DESCRIPTION+1];
    char time[MAX_LENGTH_TIME+1];
    for(int i = start; i <= end; i++)
    {
        printf("Test case %d", i);
        getTitleFromAdd(command[i-start], title);
        getDescriptionFromAdd(command[i-start], description);
        char c = description[0];
        getTimeFromAdd(command[i-start], time);
        description[0] = c;
        if(strcmp(title, result[i-start][0]) != 0 || strcmp(description, result[i-start][1]) != 0 || strcmp(time, result[i-start][2]) != 0)
        {
            printf(" fail\n");
            printf("    Command                          : %s\n", command[i - start]);
            printf("    code you title, description, time: %s , %s, %s\n", title, description, time);
            printf("    exact title, description, time   : %s , %s, %s\n", result[i-start][0], result[i-start][1], result[i-start][2]);
        }
        else printf(" pass\n");
    }
}
void test_check__FromAdd(int start, int end)
{
    //! {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};
    char command[10][MAX_LENGTH_COMMAND+1] = 
    {
        "Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]\0",
        "Add [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa] [ a ] [07:00|01/11/2023-12:00|01/10/2023]",
        "Add [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa] [a ] [07:00|02/10/2023-12:00|01/10/2023]",
        "Add [   a ] [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa] [37:00|01/10/2023-37:00|01/10/2023]",
        "Add [a    ] [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa] [07:00|37/10/2023-12:00|37/10/2023]",
        "Add [a ,.-:|/] [a ,.-:|/] [07:00|02/13/2023-12:00|01/10/2023]", 
        "Add [abcdzAZX1239] [abcdzAZX1239] [07:00|02/00/2023-12:00|01/10/2023]",
        "Add [+] [*] [12:00|01/10/2023-12:00|01/10/2023]",
        "Add [!] [=] [12:70|01/10/2023-12:70|01/10/2023]",
        "Add [] [] [12:00|01/10/2023-12:00|01/10/0023]",
    };
    int result[20][3]= {
        {-1, -1, -1},
        {-1, 0, 0},
        {101, 1, 0},
        {0,-1,  1137},
        {4,201,  3137},
        {-1, -1, 4113},
        {-1, -1, 4100},
        {0, 0, 0},
        {0, 0, 2170},
        {-1, -1, 5223}
    };
    char title[500+1];
    char description[500+1];
    char time[500+1];
    for(int i = start; i <= end; i++)
    {
        printf("Test case %d \n", i);
        getTitleFromAdd(command[i-start], title);
        getDescriptionFromAdd(command[i-start], description);
        getTimeFromAdd(command[i-start], time);
        int _checkTitle = checkTitle(title);
        int _checkDescription = checkDescription(description);
        int _checkTime = checkTime(time);
        // printf("\n%s %s %s \n", title, description, time);
        if(_checkTitle == result[i-start][0] && _checkDescription == result[i-start][1] && _checkTime == result[i-start][2]) printf(" pass\n");
        else 
        {
            printf(" fail\n");
            printf("    Command                          : %s\n", command[i - start]);
            printf("    code you check title, description, time: %d , %d, %d\n", _checkTitle, _checkDescription, _checkTime);
            printf("    exact check title, description, time   : %d , %d, %d\n", result[i-start][0], result[i-start][1], result[i-start][2]);
        }
        
    }    
}

void test_check_edt_num(int start, int end ){
    char command[10][MAX_LENGTH_COMMAND+1] = 
    {
        "Edit #1 title:[]",
        "Edit title:[]",
        "Edit # title:[]",
        "Edit #-1 title:[]",
        "Edit #1123 title:[]",
        "Edit #1e title:[]",
        "Edit #2.139 title:[]",
        "Edit #001 title:[]",
        "Edit # 1 title:[]",
        "Edit # a title:[]"
    };
    int result[20]= {
        1,
        -1,
        0,
        0,
        1123,
        0,
        0,
        1,
        0,
        0
    };
    for(int i = start; i <= end; i++)
    {
        printf("Test case %d \n", i);
        int num = getNumFromCommand(command[i]);
        // printf("\n%s %s %s \n", title, description, time);
        if(num == result[i]) printf("pass\n");
        else 
        {
            printf("\t your num: %d \n", num);
            printf("\t right answer: %d \n", result[i]);
        }
    }   
}


void test_check_field_num(int start, int end ){
    char command[10][MAX_LENGTH_COMMAND+1] = 
    {
        "Edit #1 title:[]",
        "Edit description:[]",
        "Edit #2 time:[]",
        "Edit #-1 status:[]",
        "Edit #1123 titlwe:[]",
        "Edit #1e status:[]",
        "Edit #2.139 title:[]",
        "Edit #001 title:[]",
        "Edit # 1 description:[]",
        "Edit # a title:[]"
    };
    int result[20]= {
        1,
        0,
        3,
        4,
        0,
        4,
        1,
        1,
        0,
        0
    };
    for(int i = start; i <= end; i++)
    {
        printf("Test case %d \n", i);
        int num = getFieldFromEdit(command[i]);
        // printf("\n%s %s %s \n", title, description, time);
        if(num == result[i]) printf("pass\n");
        else 
        {
            printf("\t your num: %d \n", num);
            printf("\t right answer: %d \n", result[i]);
        }
    }   
}

void test_check_state_num(int start, int end ){
    char command[10][MAX_LENGTH_COMMAND+1] = 
    {
        "Edit #1 title:[t]",
        "Edit description:[ddd]",
        "Edit #2 time:[d]",
        "Edit #-1 status:[a]",
        "Edit #1123 titlwe:[A]",
        "Edit #1e status:[D]",
        "Edit #2.139 title:[T]",
        "Edit #001 title:[aa]",
        "Edit # 1 description:[tt]",
        "Edit # a title:[das]"
    };
    int result[20]= {
        -1,
        -1,
        1,
        2,
        2,
        1,
        -1,
        -1,
        -1,
        -1
    };
    for(int i = start; i <= end; i++)
    {
        printf("Test case %d \n", i);
        int num = getStatusFromEdit(command[i]);
        // printf("\n%s %s %s \n", title, description, time);
        if(num == result[i]) printf("pass\n");
        else 
        {
            printf("\t your num: %d \n", num);
            printf("\t right answer: %d \n", result[i]);
        }
    }   
}

void test_print_all_task(int start, int end){

    int num;
    char title[MAX_LENGTH_TITLE+1];
    char description[MAX_LENGTH_DESCRIPTION+1];
    char time[MAX_LENGTH_TIME+1];
    enum Status status;

    struct Task task1 = {1, "Phong0 12","des1","time1", 0};
    struct Task task2 = {2, "Phong1123","des2","time2", 1};
    struct Task task3 = {3, "Phong2 1234","des3","time3", 2};
    struct Task task4 = {4, "Phong3 aa ","des4","time4", 3};

    struct Task tasks[MAX_NO_TASKS];
    // printAllTasks(tasks, 10);
    // printTaskByNum(tasks,10,3);
    // printHeadTasks(tasks, 10, 4);
    // printTailTasks(tasks, 10, 3);
    // printFilteredTasksByTitle(tasks,10, "123");
    // printFilteredTasksByStatus(tasks, 10, IN_PROGRESS);
    addTask(tasks, 0,"Phong1 12","des1","time1");
    addTask(tasks,1, "Phong2 12","des1","time1");
    addTask(tasks, 2, "Phong3 123","des2","time2");
    addTask(tasks, 3, "Phong4 123","des2","time2");
    addTask(tasks, 4, "Phong5 123","des2","time2");

    // printAllTasks(tasks, 5);
    // deleteTask(tasks, 5, 1);
    // deleteTask(tasks, 4, 1);
    // deleteTask(tasks, 3, 1);

    // deleteTask(tasks, 5, 3);

    // printAllTasks(tasks, 4);
    printWeekTime(tasks, 4, "10/10/2023");

}
int main() {
    // runTodoApp();
    // int num = getCommandType("add !0");
    // need to fix ?
    // char command[] = "Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]\0";
    // char out_title[200];
    // char out_description[200];
    // char out_time[200];
    // //ADD
    // getTitleFromAdd(command, out_title);
    // getDescriptionFromAdd(command, out_description);
    // getTimeFromAdd(command, out_time);
    // printf("%s \n", command_name[num]);
    // EDIT
    // getTitleFromEdit(command, out_title);
    // getDescriptionFromEdit(command, out_description);
    // getTimeFromEdit(command, out_time);
    // printf("%s \n", out_title);
    // printf("%s \n", out_description);
    // printf("%s \n", out_time);
    // printf("%d \n", getStatusFromEdit(command));
    // test_getCommandType(0,9);
    // test_get__FromAdd(11,20);
    // test_check__FromAdd(0,9);
    // test_check_edt_num(0,9);
    // test_check_field_num(0,9);
    // test_check_state_num(0,9);
    test_print_all_task(0,9);
    return 0;
}