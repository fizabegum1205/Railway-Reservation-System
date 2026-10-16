
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAX_PASSENGERS 100
#define MAX_TRAINS 5
#define CANCELLATION_CHARGE 50.0f

struct Passenger
{
    int passengerID;
    char name[50];
    int age;
    char gender[10];
    char phone[11];
    int booked;
    int ticketNo;
    int trainNo;
    int seats;
    float fare;
};

struct Train
{
    int tnumber;
    char tname[30];
    char Source[30];
    char Destination[30];
    char Date[15];
    char Timing[15];
    int capacity;
    float seatFare;
};

struct Passenger passengers[MAX_PASSENGERS];
int passengerCount = 0;
int nextTicketNo = 1001;

void readLine(char str[], int size);
int readInt(const char *message);
void registerPassenger(void);
void displayTrains(struct Train t[], int n);
void ticketBooking(struct Train t[], int n);
void cancelTicket(void);
void calculatePassengerFare(void);
void saveData(void);

void readLine(char str[], int size)
{
    if (fgets(str, size, stdin) != NULL)
    {
        str[strcspn(str, "\n")] = '\0';
    }
    else
    {
        str[0] = '\0';
    }
}

int readInt(const char *message)
{
    char buffer[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", message);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            return 0;

        value = strtol(buffer, &end, 10);

        while (isspace((unsigned char)*end))
            end++;

        if (end != buffer && *end == '\0' &&
            value >= 0 && value <= 2147483647L)
            return (int)value;

        printf("Invalid input! Enter a valid number.\n");
    }
}

/* MODULE 1: PASSENGER REGISTRATION */
void registerPassenger(void)
{
    struct Passenger p;
    int choice, valid, i, len;

    if (passengerCount >= MAX_PASSENGERS)
    {
        printf("Passenger list is full!\n");
        return;
    }

    p.passengerID = passengerCount + 1;
    p.booked = 0;
    p.ticketNo = 0;
    p.trainNo = 0;
    p.seats = 0;
    p.fare = 0.0f;

    printf("\n===== PASSENGER REGISTRATION =====\n");
    printf("Passenger ID: %d\n", p.passengerID);

    do
    {
        printf("Enter name: ");
        readLine(p.name, sizeof(p.name));

        if (strlen(p.name) == 0)
            printf("Name cannot be empty!\n");

    } while (strlen(p.name) == 0);

    do
    {
        p.age = readInt("Enter age (1-80): ");

        if (p.age < 1 || p.age > 80)
            printf("Age must be between 1 and 80.\n");

    } while (p.age < 1 || p.age > 80);

    do
    {
        printf("\n1. Male\n2. Female\n3. Others\n");
        choice = readInt("Select gender: ");

        if (choice == 1)
        {
            strcpy(p.gender, "Male");
            valid = 1;
        }
        else if (choice == 2)
        {
            strcpy(p.gender, "Female");
            valid = 1;
        }
        else if (choice == 3)
        {
            strcpy(p.gender, "Others");
            valid = 1;
        }
        else
        {
            printf("Invalid choice!\n");
            valid = 0;
        }

    } while (!valid);

    do
    {
        printf("Enter 10-digit phone number: ");
        readLine(p.phone, sizeof(p.phone));

        len = strlen(p.phone);
        valid = (len == 10);

        for (i = 0; i < len && valid; i++)
        {
            if (!isdigit((unsigned char)p.phone[i]))
                valid = 0;
        }

        if (!valid)
            printf("Enter exactly 10 digits.\n");

    } while (!valid);

    passengers[passengerCount++] = p;

    printf("\nPassenger registered successfully!\n");
    printf("ID: %d\nName: %s\nAge: %d\nGender: %s\nPhone: %s\n",
           p.passengerID, p.name, p.age, p.gender, p.phone);
}

/* MODULE 2: TRAIN DETAILS */
void displayTrains(struct Train t[], int n)
{
    char source[30], destination[30], date[15];
    int i, found = 0;

    printf("\n===== TRAIN DETAILS =====\n");
    printf("Enter Source: ");
    readLine(source, sizeof(source));

    printf("Enter Destination: ");
    readLine(destination, sizeof(destination));

    printf("Enter Date (DD-MM-YYYY): ");
    readLine(date, sizeof(date));

    for (i = 0; i < n; i++)
    {
        if (strcmp(t[i].Source, source) == 0 &&
            strcmp(t[i].Destination, destination) == 0 &&
            strcmp(t[i].Date, date) == 0)
        {
            printf("\nTrain Number: %d\n", t[i].tnumber);
            printf("Train Name: %s\n", t[i].tname);
            printf("Source: %s\n", t[i].Source);
            printf("Destination: %s\n", t[i].Destination);
            printf("Date: %s\n", t[i].Date);
            printf("Timing: %s\n", t[i].Timing);
            printf("Fare per seat: Rs. %.2f\n", t[i].seatFare);
            printf("Capacity: %d\n", t[i].capacity);
            found = 1;
        }
    }

    if (!found)
        printf("No trains available for this route and date.\n");
}

/* MODULE 3: TICKET BOOKING */
void ticketBooking(struct Train t[], int n)
{
    int id, tno, seats, i;
    int passengerIndex = -1, trainIndex = -1;
    int bookedSeats = 0;

    if (passengerCount == 0)
    {
        printf("Please register a passenger first.\n");
        return;
    }

    printf("\n===== TICKET BOOKING =====\n");
    id = readInt("Enter passenger ID: ");

    for (i = 0; i < passengerCount; i++)
    {
        if (passengers[i].passengerID == id)
        {
            passengerIndex = i;
            break;
        }
    }

    if (passengerIndex == -1)
    {
        printf("Passenger not found.\n");
        return;
    }

    if (passengers[passengerIndex].booked)
    {
        printf("This passenger already has a booking.\n");
        return;
    }

    printf("\nAvailable Trains:\n");

    for (i = 0; i < n; i++)
    {
        printf("%d - %s | %s to %s | %s | %s | Rs. %.2f\n",
               t[i].tnumber, t[i].tname,
               t[i].Source, t[i].Destination,
               t[i].Date, t[i].Timing, t[i].seatFare);
    }

    tno = readInt("Enter train number: ");

    for (i = 0; i < n; i++)
    {
        if (t[i].tnumber == tno)
        {
            trainIndex = i;
            break;
        }
    }

    if (trainIndex == -1)
    {
        printf("Invalid train number!\n");
        return;
    }

    seats = readInt("Enter number of seats (1-6): ");

    if (seats < 1 || seats > 6)
    {
        printf("Invalid number of seats!\n");
        return;
    }

    for (i = 0; i < passengerCount; i++)
    {
        if (passengers[i].booked &&
            passengers[i].trainNo == tno)
        {
            bookedSeats += passengers[i].seats;
        }
    }

    if (bookedSeats + seats > t[trainIndex].capacity)
    {
        printf("Not enough seats available!\n");
        return;
    }

    passengers[passengerIndex].ticketNo = nextTicketNo++;
    passengers[passengerIndex].trainNo = tno;
    passengers[passengerIndex].seats = seats;
    passengers[passengerIndex].fare =
        seats * t[trainIndex].seatFare;
    passengers[passengerIndex].booked = 1;

    saveData();

    printf("\nTicket booked successfully!\n");
    printf("Passenger: %s\n", passengers[passengerIndex].name);
    printf("Passenger ID: %d\n", id);
    printf("Ticket Number: %d\n",
           passengers[passengerIndex].ticketNo);
    printf("Train: %s\n", t[trainIndex].tname);
    printf("Train Number: %d\n", tno);
    printf("Seats: %d\n", seats);
    printf("Total Fare: Rs. %.2f\n",
           passengers[passengerIndex].fare);
}

/* MODULE 4: TICKET CANCELLATION AND REFUND */
void cancelTicket(void)
{
    int ticketNo, i, found = -1;
    char confirm;

    float ticketAmount;
    float cancellationCharge;
    float refundAmount;

    printf("\n===== TICKET CANCELLATION =====\n");

    ticketNo = readInt("Enter ticket number: ");

    for (i = 0; i < passengerCount; i++)
    {
        if (passengers[i].booked == 1 &&
            passengers[i].ticketNo == ticketNo)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\nTicket not found or already cancelled.\n");
        return;
    }

    ticketAmount = passengers[found].fare;

    printf("\n----- TICKET DETAILS -----\n");
    printf("Passenger ID       : %d\n",
           passengers[found].passengerID);
    printf("Passenger Name     : %s\n",
           passengers[found].name);
    printf("Ticket Number      : %d\n",
           passengers[found].ticketNo);
    printf("Train Number       : %d\n",
           passengers[found].trainNo);
    printf("Number of Seats    : %d\n",
           passengers[found].seats);
    printf("Ticket Amount      : Rs. %.2f\n", ticketAmount);
    printf("Cancellation Charge: Rs. %.2f\n",
           CANCELLATION_CHARGE);

    refundAmount = ticketAmount - CANCELLATION_CHARGE;

    if (refundAmount < 0.0f)
        refundAmount = 0.0f;

    printf("Refund Amount      : Rs. %.2f\n", refundAmount);

    printf("\nDo you want to cancel this ticket? (Y/N): ");

    if (scanf(" %c", &confirm) != 1)
    {
        printf("Input error. Cancellation stopped.\n");
        return;
    }

    {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }
    }

    if (confirm == 'Y' || confirm == 'y')
    {
        passengers[found].booked = 0;
        passengers[found].ticketNo = 0;
        passengers[found].trainNo = 0;
        passengers[found].seats = 0;
        passengers[found].fare = 0.0f;

        saveData();

        printf("\nTicket cancelled successfully!\n");
        printf("Original Ticket Amount: Rs. %.2f\n", ticketAmount);
        printf("Cancellation Charge   : Rs. %.2f\n",
               ticketAmount < CANCELLATION_CHARGE
                   ? ticketAmount
                   : CANCELLATION_CHARGE);
        printf("Refund Amount         : Rs. %.2f\n",
               refundAmount);
        printf("Please collect your refund.\n");
    }
    else
    {
        printf("\nCancellation stopped. Ticket remains active.\n");
    }
}

/* MODULE 5: PASSENGER LIST AND FARE CALCULATION */
void calculatePassengerFare(void)
{
    int n, i;
    float fare, total = 0.0f;
    char names[5][50];

    printf("\n===== PASSENGER FARE CALCULATION =====\n");

    n = readInt("Enter number of passengers (1-5): ");

    if (n < 1 || n > 5)
    {
        printf("Invalid passenger count!\n");
        return;
    }

    for (i = 0; i < n; i++)
    {
        do
        {
            printf("Enter passenger %d name: ", i + 1);
            readLine(names[i], sizeof(names[i]));

            if (strlen(names[i]) == 0)
                printf("Name cannot be empty!\n");

        } while (strlen(names[i]) == 0);
    }

    do
    {
        char buffer[100];
        char *end;

        printf("Enter fare per passenger: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            return;

        fare = strtof(buffer, &end);

        while (isspace((unsigned char)*end))
            end++;

        if (end == buffer || *end != '\0' ||
            !isfinite(fare) || fare < 0.0f)
        {
            printf("Invalid fare! Enter a non-negative amount.\n");
            fare = -1.0f;
        }

    } while (fare < 0.0f);

    printf("\n----- PASSENGER FARE LIST -----\n");

    for (i = 0; i < n; i++)
    {
        printf("%d. %s - Rs. %.2f\n",
               i + 1, names[i], fare);

        total += fare;
    }

    printf("--------------------------------\n");
    printf("Total Fare = Rs. %.2f\n", total);
}

/* DATA SAVE FUNCTION */
void saveData(void)
{
    /* Passenger records remain in memory until program exits. */
}

/* MAIN FUNCTION */
int main(void)
{
    int choice;

    struct Train trains[MAX_TRAINS] =
    {
        {101, "Express", "Guntur", "Chennai",
         "15-10-2026", "09:00AM", 100, 150.0f},

        {102, "Superfast", "Guntur", "Chennai",
         "15-10-2026", "02:00PM", 100, 200.0f},

        {103, "Passenger", "Guntur", "Chennai",
         "16-10-2026", "06:00AM", 100, 100.0f},

        {104, "Express", "Vijayawada", "Hyderabad",
         "15-10-2026", "10:00AM", 100, 150.0f},

        {105, "Superfast", "Guntur", "Hyderabad",
         "15-10-2026", "05:00PM", 100, 200.0f}
    };

    do
    {
        printf("\n====================================\n");
        printf("    RAILWAY RESERVATION SYSTEM\n");
        printf("====================================\n");
        printf("1. Passenger Registration\n");
        printf("2. Train Details\n");
        printf("3. Ticket Booking\n");
        printf("4. Ticket Cancellation and Refund\n");
        printf("5. Passenger List and Fare Calculation\n");
        printf("6. Exit\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                registerPassenger();
                break;

            case 2:
                displayTrains(trains, MAX_TRAINS);
                break;

            case 3:
                ticketBooking(trains, MAX_TRAINS);
                break;

            case 4:
                cancelTicket();
                break;

            case 5:
                calculatePassengerFare();
                break;

            case 6:
                printf("Thank you for using the Railway Reservation System!\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}
