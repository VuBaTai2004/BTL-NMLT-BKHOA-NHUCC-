#include <stdio.h>
#include <stdlib.h>
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
    char temp[strlen(command) + 1]; //temp[do dai command + 1]
    strcpy(temp, command);  //chuoi temp = command
    char *token =  strtok(temp, " ");   //token = cat chuoi temp tai vi tri co dau cach

    for(int i =0 ; i < strlen(token);i++){ //cho i chay tu 0 toi lenght(token) - 1
        if(token[i] >= 'a'){    //neu token[i] la ki tu in thuong 
            token[i] -= 32;     //in hoa
        }
    }
    // printf("%s \n", token);
    // enum CommandType type = SHOW;
    if(strcmp(token,"ADD") == 0){   //neu (token = "ADD") 
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

void getTitleFromAdd(char * command, char* out_title){
        char temp[strlen(command) + 1];
    strcpy(temp, command);
    char *token=  strtok(temp, "[]");
    int i = 1;
    while(i != 0){
        token = strtok(NULL, "[]");
        i--;
    }
    strcpy(out_title, token);
}
void getDescriptionFromAdd(char * command, char * out_description){
    char temp[strlen(command) + 1];
    strcpy(temp, command);
    char *token=  strtok(temp, "[]");
    int i = 3;
    while(i != 0){
        token = strtok(NULL, "[]");
        i--;
    }
    strcpy(out_description, token);
}
void getTimeFromAdd(char * command, char * out_time){
    char temp[strlen(command) + 1];
    strcpy(temp, command);
    char *token=  strtok(temp, "[]");
    while(token!=NULL){
        strcpy(out_time, token);
        token = strtok(NULL, "[]");
    }
}
// Other functions
int checkTitle(char* raw_title){
    char temp[strlen(raw_title) + 1];
    strcpy(temp, raw_title);

    if(strlen(temp) > 100){
        return strlen(temp);
    }
    if(temp[0] == ' ') return 0;

    for(int i = 0 ;i < strlen(temp) - 2;i++){
        if(!(temp[i] >= '0' && temp[i] <= '9') &&
            !(temp[i] >= 'a' && temp[i] <= 'z') &&
            !(temp[i] >= 'A' && temp[i] <= 'Z') &&
            !(temp[i] == ' ') && !(temp[i] == ',') &&
            !(temp[i] == '.') && !(temp[i] == '-') &&
            !(temp[i] == ':') && !(temp[i] == '|') &&
            !(temp[i] == '/'))
        // regex
        return i;
    }
    if(strcmp(&temp[strlen(raw_title) - 1], " ") == 0) return strlen(temp) - 1;

    return -1;
}

int checkDescription(char * raw_description){
    char temp[strlen(raw_description) + 1];
    strcpy(temp, raw_description);

    if(strlen(temp) > 200){
        return strlen(temp);
    }
    if(temp[0] == ' ') return 0;

    for(int i = 0 ;i < strlen(temp) - 2;i++){
        if(!(temp[i] >= '0' && temp[i] <= '9') &&
            !(temp[i] >= 'a' && temp[i] <= 'z') &&
            !(temp[i] >= 'A' && temp[i] <= 'Z') &&
            !(temp[i] == ' ') && !(temp[i] == ',') &&
            !(temp[i] == '.') && !(temp[i] == '-') &&
            !(temp[i] == ':') && !(temp[i] == '|') &&
            !(temp[i] == '/'))
        // regex
        return i;
    }
    if(strcmp(&temp[strlen(raw_description) - 1], " ") == 0) return strlen(temp) - 1;

    return -1;
}
// todo
int checkPositiveInt(int num){
    return 1;
}
int checkLeapYear(int year){
    if (year % 400 == 0 || year % 4 == 0) {
      return 1;
   }
   return 0;
}
int checkHour(char * hour){
    char temp[strlen(hour) + 1];
    strcpy(temp,  hour);
    char * token = strtok(temp, ":");
    char * _hour = NULL;
    char * minute = NULL;
    while(token != NULL){
        if(_hour == NULL){
            _hour = token;
        }
        else{
            minute = token;
        }
        token = strtok(NULL, ":");
    }
    // todo: check hour and minute is int
    return (atoi(hour) >= 0 && atoi(hour) <= 23) && (atoi(minute) >= 0 && atoi(minute) <= 59) && checkPositiveInt(atoi(hour)) && checkPositiveInt(atoi(minute));
}
int dayInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 30, 30, 31, 30, 31};
int checkDmy(char * dmy, char* _day, char*_month, char*_year){
    char temp[strlen(dmy) + 1];
    strcpy(temp,  dmy);
    char * token = strtok(temp, "/");
    char * month = NULL;
    char * day = NULL;
    char * year = NULL;

    while(token != NULL){
        if(month == NULL){
            month = token;
        }
        else if(day == NULL){
            day = token;
        }
        else{
            year = token;
        }
        token = strtok(NULL, "/");
    }
    if(checkLeapYear(atoi(year))){
        dayInMonth[1] = 29;
    }
    else{
        dayInMonth[1] =  28;
    }
    strcpy(_day,day);
    strcpy(_month,month);
    strcpy(_year,year);
    return (atoi(day) <= dayInMonth[atoi(month)-1]) && checkPositiveInt(atoi(year));
}
int checkDay(char* day, char* _day, char*_month, char*_year){
    char temp[strlen(day) + 1];
    strcpy(temp,  day);
    char * token = strtok(temp, "|");
    char * hm = NULL;
    char * dmy = NULL;

    while(token != NULL){
        if(hm == NULL){
            hm = token;
        }
        else{
            dmy = token;
        }
        token = strtok(NULL, "|");
    }
    printf("%s \n", hm);
    printf("%s \n", dmy);
    return checkHour(hm) && checkDmy(dmy,_day,_month,_year);
}
int checkTime(char * raw_time){
    char temp[strlen(raw_time) + 1];
    strcpy(temp,  raw_time);
    
    // split day
    char * token = strtok(temp, "-");
    char * date1 = NULL;
    char * date2 = NULL;
     char* day1 = NULL;
    char* month1 = NULL;
    char* year1 = NULL;
    char* day2 = NULL;
    char* month2 = NULL;
    char* year2 = NULL;
    while(token != NULL){
        if(date1 == NULL){
            date1 = token;
        }
        else{
            date2 = token;
        }
        token = strtok(NULL, "-");
    }
    if(!checkDay(date1, day1, month1, year1)) return 0;
    if(!checkDay(date2, day2, month2, year2)) return 0;
    
// todo : check day1 day2, month1 month2, year1 year2
    if(year1 > year2) return 0;
    if(month1 > month2) return 0;
    if(day1 > day2) return 0;

    printf("%s \n", day1);
    printf("%s \n", day2);
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

int main() {
    // runTodoApp();
    int num = getCommandType("Add !0");
    printf("%d",num);
    // char command[] = "Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]";
    // char raw_title[200];
    // char raw_description[200];
    // char raw_time[200];
    // getTitleFromAdd(command, raw_title);
    // getDescriptionFromAdd(command, raw_description);
    // getTimeFromAdd(command, raw_time);
    // // printf("%s \n", raw_title);
    // // printf("%s \n", raw_description);
    // // printf("%s \n", raw_time);
    // // int failTitle = checkTitle(raw_title);
    // // printf("%d\n", failTitle);
    // checkTime(raw_time);
    return 0;
}