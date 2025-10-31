#include <stdio.h>
#include <string.h>
#include "flight.h"
#include "utils.h"

typedef struct {
    char flightNum[10];
    char source[30];
    char destination[30];
    char artime[10]; // arrival time
    char dptime[10]; // departure time
    char date[15];
    int seatsAvailable;
    float price;
} Flight;


void searchFlights(){
    FILE *fp = fopen ("flights.txt" ,"r");
    if (fp == NULL) {
        printf("Error: Could not open flights database.\n");
        return;
    }

    Flight f[100];
    char source[30], dest[30], date[15];
    int c=0;

    printf("Enter From: ");
    scanf("%s", source);
    changeCase(source);
    printf("Enter To: ");
    scanf("%s", dest);
    changeCase(dest);
    printf("Enter Date of Travel (DD-MM-YYYY): "); 
    scanf("%s", date);

    while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",
                  f[c].flightNum, f[c].source, f[c].destination, f[c].artime, f[c].dptime, f[c].date, &f[c].seatsAvailable, &f[c].price) != EOF) {
        changeCase(f[c].source);
        changeCase(f[c].destination);
        if (strcmp(f[c].source, source) == 0 && strcmp(f[c].destination, dest) == 0 && strcmp(f[c].date, date) == 0) {
            c++;
        }
    }
    fclose(fp);
    if (c == 0) {
        printf("No flights found for the given criteria.\n");
        return;
    } else {
        printf("Available flights are : ");
        for(int i = 0; i < c; i++) {
            printf("\n%d. Flight number: %s\n   %s to %s\n   Departure time: %s\n   Arrival time: %s\n   Fare: %.2f\n   Seats available: %d\n", 
                   i + 1, f[i].flightNum, f[i].source, f[i].destination, f[i].dptime, f[i].artime, f[i].price, f[i].seatsAvailable);
        }
        int choice;
        printf("Sort flights by:\n1. Price (Low to High)\n2. Price (High to Low)\n3. No sort\nChoose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: // Sort by Price (Low to High)
                for (int i = 0; i < c - 1; i++) {
                    for (int j = 0; j < c - i - 1; j++) {
                        if (f[j].price > f[j + 1].price) {
                            Flight temp = f[j];
                            f[j] = f[j + 1];
                            f[j + 1] = temp;
                        }
                    }
                }
                break;
            case 2: // Sort by Price (High to Low)
                for (int i = 0; i < c - 1; i++) {
                    for (int j = 0; j < c - i - 1; j++) {
                        if (f[j].price < f[j + 1].price) {
                            Flight temp = f[j];
                            f[j] = f[j + 1];
                            f[j + 1] = temp;
                        }
                    }
                }
                break;
            case 3:
            default:
                break;
        }
        {
            // Display sorted flights
            printf("Sorted Available Flights on %s:\n", date);
            for (int i = 0; i < c; i++) {
                printf("%d. Flight number: %s\n   %s to %s\n   Departure: %s\n   Arrival: %s\n   Fare: %.2f\n   Seats available: %d\n", 
                       i + 1, f[i].flightNum, f[i].source, f[i].destination, f[i].dptime, f[i].artime, f[i].price, f[i].seatsAvailable);
            }
        }
    }





}
