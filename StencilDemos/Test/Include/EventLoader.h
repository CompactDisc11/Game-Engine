
typedef struct {
    int Day, Month, Year;
    int TimeStart, TimeEnd;
    int AllDay;
    char *Name; 
    char *Location; 
    char *PeopleInvloved;

} Event;

typedef struct {
    Event Days[31][24]; //one of those cases where I as a designer am having an unintended impact on society by limiting them to 24 events
} day;

void StringCopy(char *String1[100], char *String2[100]);