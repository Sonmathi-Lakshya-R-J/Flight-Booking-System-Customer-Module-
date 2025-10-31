#include <stdio.h>
#include "auth.h"
#include "flight.h"
#include "booking.h"

char logUser[50]; 

int main() {
    int choice;
    char username[50];
    while (1) {
        printf("\n--- Flight Booking System ---\n");
        printf("1. Register\n2. Login\n3. Exit\nChoose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerUser(); break;
            case 2:
                if (login(username)) {
                    int option;
                    do {
                        printf("\n1. Search Flights\n2. Book Flight\n3. Cancel Booking\n4. View Booking\n5. Logout\nChoose an option: ");
                        scanf("%d", &option);
                        switch (option) {
                            case 1: searchFlights(); break;
                            case 2: bookFlight(logUser); break;
                            case 3: cancelBooking(logUser); break;
                            case 4: viewBookings(logUser); break;
                        }
                    } while (option != 5);
                }
                break;
            case 3: return 0;
        }
    }
    return 0;
}
