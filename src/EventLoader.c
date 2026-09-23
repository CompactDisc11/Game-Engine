#include "EventLoader.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
void  StringCopy(char **String1, char **String2){

    if (strlen(*String1) > strlen(*String2))
    {
        printf("\n String 1 longer than String 2\n%s\n", *String2);

        char *tmp = (char*)malloc(strlen(*String1));
        if (tmp == NULL){
            printf("Failed to reallocate memory\n");
        }
        else{
            *String2 = tmp;
        }
        
    }
    int CharacterIndex = 0;
    memcpy(*String2, *String1, strlen(*String1));
    printf("\n%s", *String2);
    printf("\n");

}


void CreateEvent(Event* ev, int Day, int Month, int Year, int AllDay, char Name[], char Location[], char PeopleInvovled[]){
    ev->Day = Day;
    ev->Month = Month;
    ev->Year = Year;
    ev->AllDay = AllDay;
    
    
}