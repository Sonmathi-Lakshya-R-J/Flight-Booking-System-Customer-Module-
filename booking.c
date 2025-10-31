#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "booking.h"
#include "utils.h"

extern char logUser[50]; 

typedef struct {
    char username[50];   
    char name[50];
    int age;
    char gender;
    char flightNum[10];
    int bookingID;
} Passenger;

typedef struct {
    char flightNum[10];
    char source[30];
    char destination[30];
    char artime[10];
    char dptime[10]; 
    char date[15];
    int seatsAvailable;
    float price;
} Flight;


void bookFlight(char username[]);
void cancelBooking(char username[]);
void viewBookings(char username[]);

void bookFlight(char username[]) {
    Passenger p;
    Flight f;
    FILE *fl = fopen("flights.txt", "r");
    FILE *b = fopen("bookings.txt", "a");
    char flightNum[10];
    int flag = 0;
    float fare;
    int seatsAvailable;

    if (fl == NULL || b == NULL) {
        printf("Error: Could not open required files.\n");
        return;
    }
    fclose(fl); 

    printf("\n\t\t--- Book a Flight ---\n");
    printf("Enter flight number: ");
    scanf("%s", flightNum);

    fl = fopen("flights.txt", "r");
    if (fl == NULL) {
        printf("Error: Could not open flights file.\n");
        fclose(b);
        return;
    }

    while(fscanf(fl, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",
                  f.flightNum, f.source, f.destination, f.artime, f.dptime, f.date, &f.seatsAvailable, &f.price) != EOF)  {
        if (strcmp(flightNum, f.flightNum) == 0) {
            flag = 1;
            fare = f.price;
            seatsAvailable = f.seatsAvailable;
            break;
        }
    }
    fclose(fl);

    if (!flag) {
        printf("Error: Flight number not found.\n");
        fclose(b);
        return;
    }

    int seat;
    printf("The fare for this flight is: %.2f\n", fare);
    printf("Seats available: %d\n", seatsAvailable);
    printf("Enter the number of seats to book: ");
    scanf("%d", &seat);

    if (seat > seatsAvailable) {
        printf("Sorry, only %d seat(s) are available. Cannot book %d seat(s).\n", seatsAvailable, seat);
        fclose(b);
        return;
    }

    float totalFare = fare * seat;
    printf("The total fare for %d seat(s) is: %.2f\n", seat, totalFare);
    printf("Do you want to proceed with the payment? (yes/no): ");
    char confirmation[5];
    scanf("%s", confirmation);
    changeCase(confirmation);

    if (strcmp(confirmation, "yes")  && strcmp(confirmation, "y") ) {
        printf("Booking canceled.\n");
        fclose(b);
        return;
    }

    for (int i = 0; i < seat; i++) {
        printf("\nEnter details for passenger %d:\n", i + 1);
        printf("Enter name: ");
        scanf("%s", p.name);
        printf("Enter age: ");
        scanf("%d", &p.age);
        do {
            printf("Enter gender (M/F): ");
            scanf(" %c", &p.gender);
            changeCase(&p.gender);
            if (p.gender != 'M' && p.gender != 'F' && p.gender != 'm' && p.gender != 'f') {
                printf("Invalid input. Please enter 'M' or 'F'.\n");
            }
        } while (p.gender != 'M' && p.gender != 'F' && p.gender != 'm' && p.gender != 'f');

        strncpy(p.username, username, sizeof(p.username) - 1);
        p.username[sizeof(p.username) - 1] = '\0';
        strcpy(p.flightNum, flightNum);
        p.bookingID = (time(NULL) % 100000) + (rand() % 1000);

        fprintf(b, "%s,%s,%d,%c,%s,%d\n",
                p.username, p.name, p.age, p.gender, p.flightNum, p.bookingID);
        printf("Booking successful for passenger %d. ID: %d\n", i + 1, p.bookingID);
    }

    fclose(b);

    FILE *in = fopen("flights.txt", "r");
    FILE *out = fopen("flights_tmp.txt", "w");

    if (in && out) {
        Flight temp;
        while (fscanf(in, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",
                    temp.flightNum, temp.source, temp.destination,
                    temp.artime, temp.dptime, temp.date,
                    &temp.seatsAvailable, &temp.price) == 8) {
            
            if (strcmp(temp.flightNum, flightNum) == 0) {
                temp.seatsAvailable -= seat;
            }

            fprintf(out, "%s,%s,%s,%s,%s,%s,%d,%.2f\n",
                    temp.flightNum, temp.source, temp.destination,
                    temp.artime, temp.dptime, temp.date,
                    temp.seatsAvailable, temp.price);
        }

        fclose(in);
        fclose(out);
        remove("flights.txt");
        rename("flights_tmp.txt", "flights.txt");
    }


    printf("All bookings completed successfully.\n");
}




void cancelBooking(char username[]) {
    int id, found = 0;
    Passenger p;

    printf("Enter booking ID to cancel: ");
    scanf("%d", &id);

    FILE *fp = fopen("bookings.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open bookings file.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error: Could not create temporary file.\n");
        fclose(fp);
        return;
    }

    char canceledFlightNum[10] = "";

    while (fscanf(fp, "%49[^,],%49[^,],%d,%c,%9[^,],%d\n",
                  p.username, p.name, &p.age, &p.gender,
                  p.flightNum, &p.bookingID) == 6) {
        if (p.bookingID == id && strcmp(p.username, username) == 0) {
            found = 1;
            strcpy(canceledFlightNum, p.flightNum); // Save for later seat update
        } else {
            fprintf(temp, "%s,%s,%d,%c,%s,%d\n",
                    p.username, p.name, p.age, p.gender,
                    p.flightNum, p.bookingID);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("bookings.txt");
    rename("temp.txt", "bookings.txt");

    if (found) {
        FILE *in = fopen("flights.txt", "r");
        FILE *out = fopen("flights_tmp.txt", "w");

        if (in == NULL || out == NULL) {
            printf("Error: Could not open flights files for updating.\n");
            if (in) fclose(in);
            if (out) fclose(out);
            return;
        }

        Flight f;
        while (fscanf(in, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",
                      f.flightNum, f.source, f.destination,
                      f.artime, f.dptime, f.date,
                      &f.seatsAvailable, &f.price) == 8) {

            if (strcmp(f.flightNum, canceledFlightNum) == 0) {
                f.seatsAvailable += 1; // Increase seat count
            }

            fprintf(out, "%s,%s,%s,%s,%s,%s,%d,%.2f\n",
                    f.flightNum, f.source, f.destination,
                    f.artime, f.dptime, f.date,
                    f.seatsAvailable, f.price);
        }

        fclose(in);
        fclose(out);
        remove("flights.txt");
        rename("flights_tmp.txt", "flights.txt");

        printf("Booking canceled successfully.\n");
    } else {
        printf("Booking ID not found or does not belong to your account.\n");
    }
}


void viewBookings(char username[]) {
    FILE *fp = fopen("bookings.txt", "r");
    Passenger p;
    int found = 0;

    if (fp == NULL) {
        printf("Error: Could not open bookings file.\n");
        return;
    }

    printf("\n--- Your Bookings ---\n");

     while (fscanf(fp, "%49[^,],%49[^,],%d,%c,%9[^,],%d\n",
                  p.username, p.name, &p.age, &p.gender, p.flightNum, &p.bookingID) == 6) {
        if (strcmp(p.username, username) == 0) {
            printf("Passenger: %s | Age: %d | Gender: %c | Flight: %s | Booking ID: %d\n",
                   p.name, p.age, p.gender, p.flightNum, p.bookingID);
            found = 1;
        }
    }

    if (!found) {
        printf("No bookings found for user: %s\n", username);
    }

    fclose(fp);
}
