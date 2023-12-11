#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FLIGHT_MAX_COUNT 20
#define MAX_LUGGAGE_COUNT 2
#define MEDIUM_PLAIN_ROW_COUNT 23
#define MEDIUM_PLAIN_SEAT_COUNT_IN_ROW 6
#define MEDIUM_PLAIN_SEATS_COUNT (MEDIUM_PLAIN_ROW_COUNT * MEDIUM_PLAIN_SEAT_COUNT_IN_ROW)
#define SMALL_PLAIN_ROW_COUNT 15
#define SMALL_PLAIN_SEAT_COUNT_IN_ROW 4
#define SMALL_PLAIN_SEATS_COUNT (SMALL_PLAIN_ROW_COUNT * SMALL_PLAIN_SEAT_COUNT_IN_ROW)
#define HAND_LUG_MAX_WEIGHT_KG 5
#define HAND_LUG_MAX_HAIGHT_CM 56
#define HAND_LUG_MAX_LENGHT_CM 45
#define HAND_LUG_MAX_WIDTH_CM 25
#define LUG_MAX_WEIGHT_KG 20
#define LUG_MAX_HAIGHT_CM 81
#define LUG_MAX_LENGHT_CM 54
#define LUG_MAX_WIDTH_CM 30
#define AIRCRAFT_TYPE_SMALL 0
#define AIRCRAFT_TYPE_MEDIUM 1


typedef struct person_s{
    char ID[10];
    char name[10];
    char surname[15];
}person_t;

typedef struct luggage_s {
    int length;
    int hight;
    int width;
    int weight;
    bool isHand;
}luggage_t;

typedef struct registrationInfo_s {
    person_t person;
    luggage_t luggages[MAX_LUGGAGE_COUNT];
    int currLugCount;
    int seat;
}registrationInfo_t;

typedef struct flight_s {
	char airline[10];
    char dest[15];
    char fligh_code[11];
    char start_time[12];
    char window[6];
    int aircraft_type;
    int gate;
    registrationInfo_t  *regInfos;
    int currRegCount;
}flight_t;


/// Reads flught's information from the flight.txt file.
/// 
/// f      File's pointer
/// pData  Pointer to flight srtucture 
/// Return Error when any paramerts missed or incorrect format in file
int readFlightInfo(FILE* f, flight_t* pData) { 
    int fResult;

    fResult = fscanf(f, "airline: %9s\n", pData->airline);
    if (fResult != 1) {
        printf("Incorrect airline format\n");
        return 1;
    }

    fResult = fscanf(f, "dest: %14s\n", pData->dest);
    if (fResult != 1) {
        printf("Incorrect dest format\n");
        return 1;
    }

    fResult = fscanf(f, "fligh_code: %10s\n", pData->fligh_code);
    if (fResult != 1) {
        printf("Incorrect fligh_code format\n");
        return 1;
    }

    fResult = fscanf(f, "start_time: %11s\n", pData->start_time);
    if (fResult != 1) {
        printf("Incorrect start_time format\n");
        return 1;
    }

    fResult = fscanf(f, "window: %5s\n", pData->window);
    if (fResult != 1) {
        printf("Incorrect window format\n");
        return 1;
    }
    char temp[10] = {0};
    fResult = fscanf(f, "aircraft_type: %9s\n", temp);
    if (fResult != 1) {
        printf("Incorrect aircraft_type format\n");
        return 1;
    }
    if (!strcmp(temp, "small")) {
        pData->aircraft_type = AIRCRAFT_TYPE_SMALL;
    }
    else if (!strcmp(temp, "medium")) {
        pData->aircraft_type = AIRCRAFT_TYPE_MEDIUM;
    }
    else {
        printf("Unknown aircraft type %s", temp);
        return 1;
    }
    fResult = fscanf(f, "gate: %d\n", &pData->gate);
    if (fResult != 1) {
        printf("Incorrect gate format\n");
        return 1;
    }
    // Allocate memory for registrationInfo_t  array based on aircraft type.
    int regCount = pData->aircraft_type == AIRCRAFT_TYPE_SMALL ? SMALL_PLAIN_SEATS_COUNT : MEDIUM_PLAIN_SEATS_COUNT;
    pData->regInfos = (registrationInfo_t*)malloc(sizeof(registrationInfo_t) * regCount);
    return 0;
}

/// This function reads flights information from the file and copy into the memory 
///
/// flightArr  Memory pointer for reading data into this array
/// flightNum  Actuall numbefr of flights read from the file
/// return     0 success, other value in case of error
int initializeFlights(flight_t* flightArr, int* flightNum) { 
   
    FILE* f;
 
    if (fopen_s(&f, "flights.txt", "r") != 0){
        printf("Cannot open the file flights.txt\n");
        return 1;
    }
    int i = 0;
    int result;
    do{
       result = readFlightInfo(f, flightArr + i);
       if (result != 0) {
           break;
       }
     
        i++;
    // break cycle either file EOF or max flight count is read 
    } while (!feof(f) && i < FLIGHT_MAX_COUNT);

    fclose(f);

    if (result == 0) {
        *flightNum = i;
    }

    return result;
}

/// This function releases any allocated memory
/// 
/// flightArr  Memory pointer contains flight information array
/// flightNum  Flight count
void  uninitializeFlights(flight_t* flightArr, int flightNum)
{
    for (int i = 0; i < flightNum; i++)
    {
        if ((flightArr + i)->regInfos != NULL) {
            free((flightArr + i)->regInfos);
        }
    }
}

/// This function clears characters from the input in order to do not pass them next input
void clearInput(void) 
{
    char newLine;
    while ((newLine = getchar()) != '\n' && newLine != EOF);

}

/// This function displays check in status: Personal info, luggage info, seat number. 
/// 
/// pFlight  Pointer to flight information structure
void displayCheckInStatus(flight_t* pFlight) {
    int maxSeatInRow = (pFlight->aircraft_type == AIRCRAFT_TYPE_SMALL) ? SMALL_PLAIN_SEAT_COUNT_IN_ROW : MEDIUM_PLAIN_SEAT_COUNT_IN_ROW;
    printf("\nThe checking status of %s flight\n", pFlight->fligh_code);
    printf("Num  ID         Name       Surname         Seat  Luggage Count\n");
   
    for (int i = 0; i < pFlight->currRegCount; i++)
    {
        char seatStr[4];
        sprintf(seatStr, "%d%c", (i / maxSeatInRow) + 1, i % maxSeatInRow + 'A'); // convert index into the seat format(11A).
        printf("%3d  %-9s  %-9s  %-14s  %-4s         %d\n", 
            i + 1, 
            pFlight->regInfos[i].person.ID, 
            pFlight->regInfos[i].person.name, 
            pFlight->regInfos[i].person.surname, 
            seatStr,
            pFlight->regInfos[i].currLugCount);
        
    }
    char temp;
    printf("Press any key to continue\n");
    (void)scanf("%c", &temp);
}

/// This function returns weather provided seat is occupied or no
/// 
/// pRegInfos  Pointer to registration array
/// infoCount  Registration array's size
/// seat       Seat number to be checked
/// returns    True if occupied, otherwise false
bool isSeatOccupied(registrationInfo_t* pRegInfos, int infoCount, int seat) {
    for (int i = 0; i < infoCount; i++) {    
        if (seat == pRegInfos[i].seat) {
            return true;
        }
    }
     
    return false;
}

/// This function displays seats status, O - occupied, F - free
/// 
/// pRegInfos  Pointer to registration array
/// infoCount  Registration array's size
/// aircraftType Type of aircraft that indicates max seat's number
void displaySeatStatus(registrationInfo_t* pRegInfos, int infoCount, int aircraftType) {
    int maxSeatIndex;
    int maxSeatRow;

    printf("Airplain seat status:\n");
    printf("F - Free seat\n");
    printf("O - Occupied seat\n");
    
    if (aircraftType == AIRCRAFT_TYPE_SMALL) {
        maxSeatIndex = SMALL_PLAIN_SEAT_COUNT_IN_ROW;
        maxSeatRow = SMALL_PLAIN_ROW_COUNT;
    }
    else {
        maxSeatIndex = MEDIUM_PLAIN_SEAT_COUNT_IN_ROW;
        maxSeatRow = MEDIUM_PLAIN_ROW_COUNT;
    }
    for (int i = 0; i < maxSeatRow; i++) {
        for (int j = 0; j < maxSeatIndex; j++) {
            printf("%d%c (%c)%s", i + 1, j + 'A', isSeatOccupied(pRegInfos, infoCount, i * maxSeatIndex + j) ? 'O' : 'F', (i < 9) ? "   " : "  ");
        }
        printf("\n");
    }
   
}

/// This function gets person info from the input
/// 
/// pPerson  Pointer to the person structure
void registarPersonInfo(person_t* pPerson) {

    printf("Please enter your ID: \n");
    (void)scanf("%9s", pPerson->ID);
    clearInput();

    printf("Please enter your name: \n");
    (void)scanf("%9s", pPerson->name);
    clearInput();

    printf("Please enter your surname: \n");
    (void)scanf("%14s", pPerson->surname);
    clearInput();

}

/// This function gets luggage info from the input
/// 
/// pLug      Pointer to the luggage array
/// pLugCount Returns actuall inputed luggage's count
void registarLuggageInfo(luggage_t* pLug, int *pLugCount) {

    char temp[4] = {0};
    int lugCount = 0;

    printf("Do you have a hendluggage?(Yes/No) \n");
    (void)scanf("%3s", temp);
    clearInput();

    if (!_stricmp(temp, "Yes")) { 
        printf("Please enter hendluggage's  size(Max 56 x 45 x 25 cm)\n");
        (void)scanf("%d %d %d", &pLug[lugCount].hight , &pLug[lugCount].length, &pLug[lugCount].width);

        printf("Please enter hendluggage's  weight(Max 5 kg)\n");
        (void)scanf("%d", &pLug[lugCount].weight);

        if (pLug[lugCount].hight > HAND_LUG_MAX_HAIGHT_CM ||
            pLug[lugCount].length > HAND_LUG_MAX_LENGHT_CM ||
            pLug[lugCount].width > HAND_LUG_MAX_WIDTH_CM ||
            pLug[lugCount].weight > HAND_LUG_MAX_WEIGHT_KG) {

            printf("Sorry but your handluggage's parametrs are not fit standards, we will consider it as a luggage\n"); //TBD luggage no hand

            pLug[lugCount].isHand = false;
        }
        else 
        {
            pLug[lugCount].isHand = true;
        
        } 
        lugCount++;
    }
    printf("Do you have a checked baggage?(Yes/No)\n");
    (void)scanf("%3s", temp);
    clearInput();

    if (!_stricmp(temp, "Yes")) {
        printf("Please enter checked baggage's  size(Max 81 x 54 x 30 cm)\n");
        (void)scanf("%d %d %d", &pLug[lugCount].hight, &pLug[lugCount].length, &pLug[lugCount].width);

        printf("Please enter checked baggage's  weight(Max 20 kg)\n");
        (void)scanf("%d", &pLug[lugCount].weight);

        if (pLug[lugCount].hight > LUG_MAX_HAIGHT_CM ||
            pLug[lugCount].length > LUG_MAX_LENGHT_CM ||
            pLug[lugCount].width > LUG_MAX_WIDTH_CM ||
            pLug[lugCount].weight > LUG_MAX_WEIGHT_KG) {

            printf("Your checked baggage's parametrs are not fit standards, you have to overpay \n"); //TBD calculate overpay and damp
           
        }
        pLug[lugCount].isHand = false;
        lugCount++;
    }
    *pLugCount = lugCount;
}

/// This function converts sear format to index
/// 
/// seat    String of seat's format
/// type    Type of aircraft
/// returns Returns seat's index or -1 in case of incorrect format
int seatString2Num(char* seat, int type) {
    int len = (int)strlen(seat);
    int ret = -1;
    int seatRow = 0;
    int seatIndex = 0;

    if (len == 2) {
        seatRow = seat[0] - '0';
        seatIndex = seat[1] - 'A';
    }
    else if(len == 3) {
        seatRow = seat[0] - '0';
        seatRow *= 10;
        seatRow += seat[1] - '0';
        seatIndex = seat[2] - 'A';
    }
    else {
        return 0;
    }

    if(type == AIRCRAFT_TYPE_SMALL) {
        if(seatIndex > SMALL_PLAIN_SEAT_COUNT_IN_ROW || seatRow > SMALL_PLAIN_ROW_COUNT)
            return -1;
        ret = (seatRow - 1) * SMALL_PLAIN_SEAT_COUNT_IN_ROW + seatIndex;

    }
    else if (type == AIRCRAFT_TYPE_MEDIUM) {
        if (seatIndex > MEDIUM_PLAIN_SEAT_COUNT_IN_ROW || seatRow > MEDIUM_PLAIN_ROW_COUNT)
            return -1;
        ret = (seatRow - 1) * MEDIUM_PLAIN_SEAT_COUNT_IN_ROW + seatIndex;
    }

    return ret;
}

/// This function registor seat's number either mannualy or automatically 
/// 
/// pRegInfos    Pointer to the registration info array
/// infoCount    Registration array size
/// aircraftType Aircraft type for getting max seat's count
void registorSeat(registrationInfo_t* pRegInfos, int infoCount, int aircraftType) {
    char temp[4] = {0};
    printf("Do you want to select seat mannualy? (Yes/No)\n");
    (void)scanf("%3s", temp);
    clearInput();
 
    if (!_stricmp(temp, "Yes")) { // mannual selection of seat
        int seatNum;
        do
        {
            displaySeatStatus(pRegInfos, infoCount, aircraftType);
            printf("Select the seat\n");
            (void)scanf("%3s", temp);
            clearInput();
            seatNum = seatString2Num(temp, aircraftType);
            if (seatNum == -1) {
                printf("Incorrect seat number entered\n");
            }
        } while (seatNum == -1);
        pRegInfos[infoCount].seat = seatNum;
    }
    else { // automatic selection of seat, it finds first free seat
        int i;
        int maxSeatInRow = (aircraftType == AIRCRAFT_TYPE_SMALL) ? SMALL_PLAIN_SEAT_COUNT_IN_ROW : MEDIUM_PLAIN_SEAT_COUNT_IN_ROW;
        int maxSeatCount = (aircraftType == AIRCRAFT_TYPE_SMALL) ? SMALL_PLAIN_SEATS_COUNT : MEDIUM_PLAIN_SEATS_COUNT;
        for ( i = 0; i < maxSeatCount; i++) {
            if (!isSeatOccupied(pRegInfos, infoCount, i)) {
                break;
            }
        }
        pRegInfos[infoCount].seat = i;
        printf("Selected seat is %d%c\n", (i / maxSeatInRow) + 1, i % maxSeatInRow + 'A');
    }
}

/// This function gets check in info from input(person, luggage, seat)
/// 
/// pFlight Pointer to the flight structure
void performCheckIn(flight_t* pFlight) {
    char temp[4] = {0};

    do
    {
        if ((pFlight->aircraft_type == AIRCRAFT_TYPE_SMALL && pFlight->currRegCount >= SMALL_PLAIN_SEATS_COUNT) ||
            (pFlight->aircraft_type == AIRCRAFT_TYPE_MEDIUM && pFlight->currRegCount >= MEDIUM_PLAIN_SEATS_COUNT)) {

            printf("Aircraf is full, no further registration can be done\n");
            break;
        }
        registarPersonInfo(&pFlight->regInfos[pFlight->currRegCount].person);
        registarLuggageInfo(pFlight->regInfos[pFlight->currRegCount].luggages, &pFlight->regInfos[pFlight->currRegCount].currLugCount);
        registorSeat(pFlight->regInfos, pFlight->currRegCount, pFlight->aircraft_type);
        pFlight->currRegCount++;

        printf("Do you want to continue registration (Yes/No)\n");
        (void)scanf("%3s", temp);
        clearInput();

        if (!_stricmp(temp, "No")){
            break;
        }
    } while (true);
    
}

/// This function displays check in flight menu 
/// 
/// pFlight Pointer to the flight structure
void displayFlightMenu(flight_t* pFlight) {

    int menuChoice;
    do
    {
        printf("\nCheck in flight %s\n", pFlight->fligh_code);
        printf("1.Check in status\n");
        printf("2.Seat status\n");
        printf("3.Perform check in\n");
        printf("4.Exit\n");
        (void)scanf("%d", &menuChoice);

        switch (menuChoice)
        {
        case 1:
            displayCheckInStatus(pFlight);
            break;
        case 2:
            displaySeatStatus(pFlight->regInfos, pFlight->currRegCount, pFlight->aircraft_type);
            break;
        case 3:
            performCheckIn(pFlight);
            break;
        case 4:
            return;
        default:
            printf("There is not such a choic!\n");
            break;
        }
    } while (true); 
   
}

/// This function displays flights menu
/// 
/// flightArr  Pointer to flight array
/// flightNum  Flight array size
void displayFlightsMenu(flight_t* flightArr, int flightNum) {
 
    do
    {
        int flightIndex;
        printf("Num  Airline    Destination     Flight code  Start time   Window  Gate \n");
        for (int i = 0; i < flightNum; i++) {

            printf("%3d  %-9s  %-14s  %-10s   %-11s  %-5s    %d\n", 
                i + 1,
                flightArr[i].airline,
                flightArr[i].dest,
                flightArr[i].fligh_code,
                flightArr[i].start_time,
                flightArr[i].window,
                flightArr[i].gate);
        }
 
        printf("Please select flight number for check in or 0 to exit...\n");

        (void)scanf("%d", &flightIndex);

        if (flightIndex > flightNum) {
            printf("Incorrect flight number, please enter again\n");
            continue;
        }

        if (flightIndex == 0)
            break;
     
        displayFlightMenu(&flightArr[flightIndex-1]);
    } while (true);
}

int main()
{
    flight_t flightArr[FLIGHT_MAX_COUNT];
    int flightNum = 0;
    int ret;

    memset(flightArr, 0, sizeof(flightArr)); 

    ret = initializeFlights(flightArr, &flightNum);
    if (ret == 0)
    {
        displayFlightsMenu(flightArr, flightNum);
    }
   
    uninitializeFlights(flightArr, flightNum);
    return ret;
}

