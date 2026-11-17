#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 50

typedef struct {
    int flight_no;
    char destination[50];
    int seats[MAX_SEATS];
    char (*passenger_list)[50];
    int total_seats;
    int bookings;
    int cancellations;
} Flight;

int total_bookings = 0, total_cancellations = 0;

void initialize(Flight *f) {
    for (int i = 0; i < f->total_seats; i++)
        f->seats[i] = 0;
}

void book_ticket(Flight *f) {
    int seat;
    char name[50];

    printf("Enter passenger name: ");
    scanf(" %49[^\n]", name);6

    printf("Enter seat number (1-%d): ", f->total_seats);
    scanf("%d", &seat);

    if (seat < 1 || seat > f->total_seats) {
        printf("Invalid seat number!\n");
        return;
    }

    if (f->seats[seat - 1] == 1) {
        printf("Seat already booked!\n");
        return;
    }

    f->seats[seat - 1] = 1;
    strcpy(f->passenger_list[seat - 1], name);
    f->bookings++;
    total_bookings++;

    printf("Ticket booked successfully!\n");
}

void cancel_ticket(Flight *f) {
    int seat;

    printf("Enter seat number to cancel: ");
    scanf("%d", &seat);

    if (seat < 1 || seat > f->total_seats ||
        f->seats[seat - 1] == 0) {
        printf("Invalid seat or no booking found!\n");
        return;
    }

    f->seats[seat - 1] = 0;
    f->passenger_list[seat - 1][0] = '\0';
    f->cancellations++;
    total_cancellations++;

    printf("Booking cancelled successfully!\n");
}

void search_passenger(Flight *f) {
    char name[50];
    int found = 0;

    printf("Enter passenger name to search: ");
    scanf(" %49[^\n]", name);

    for (int i = 0; i < f->total_seats; i++) {
        if (f->seats[i] == 1 &&
            strcmp(f->passenger_list[i], name) == 0) {
            printf("Passenger: %s\nFlight: %d\nDestination: %s\nSeat: %d\n",
                   name, f->flight_no, f->destination, i + 1);
            found = 1;
        }
    }

    if (!found)
        printf("Passenger not found on this flight.\n");
}

void view_flight(Flight *f) {
    printf("\nFlight Number: %d\n", f->flight_no);
    printf("Destination: %s\n", f->destination);
    printf("Total Seats: %d\n", f->total_seats);
    printf("Available Seats: %d\n",
           f->total_seats - f->bookings + f->cancellations);

    printf("\nSeat Status:\n");
    for (int i = 0; i < f->total_seats; i++) {
        if (f->seats[i] == 1)
            printf("Seat %d: Booked by %s\n",
                   i + 1, f->passenger_list[i]);
        else
            printf("Seat %d: Available\n", i + 1);
    }
}

void report(Flight *f) {
    int occupied = 0;

    for (int i = 0; i < f->total_seats; i++)
        occupied += f->seats[i];

    printf("\n--- SUMMARY REPORT ---\n");
    printf("Flight Number: %d\n", f->flight_no);
    printf("Total Bookings: %d\n", total_bookings);
    printf("Total Cancellations: %d\n", total_cancellations);
    printf("Occupied Seats: %d\n", occupied);
    printf("Available Seats: %d\n", f->total_seats - occupied);
    printf("Seat Occupancy: %.2f%%\n",
           occupied * 100.0 / f->total_seats);
}

void save_report(Flight *f) {
    FILE *fp = fopen("flight_report.txt", "w");

    if (fp == NULL) {
        printf("Unable to open report file.\n");
        return;
    }

    int occupied = 0;
    for (int i = 0; i < f->total_seats; i++)
        occupied += f->seats[i];

    fprintf(fp, "Flight Number: %d\n", f->flight_no);
    fprintf(fp, "Destination: %s\n", f->destination);
    fprintf(fp, "Total Bookings: %d\n", total_bookings);
    fprintf(fp, "Total Cancellations: %d\n", total_cancellations);
    fprintf(fp, "Occupied Seats: %d\n", occupied);
    fprintf(fp, "Available Seats: %d\n", f->total_seats - occupied);
    fprintf(fp, "Occupancy: %.2f%%\n",
            occupied * 100.0 / f->total_seats);

    fclose(fp);
    printf("Report saved to flight_report.txt\n");
}

int main(void) {
    int n, choice;

    printf("Enter number of flights: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of flights.\n");
        return 1;
    }

    Flight *flights = calloc(n, sizeof(Flight));

    if (flights == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\nEnter flight number: ");
        scanf("%d", &flights[i].flight_no);

        printf("Enter destination: ");
        scanf(" %49[^\n]", flights[i].destination);

        printf("Enter number of seats (1-50): ");
        scanf("%d", &flights[i].total_seats);

        if (flights[i].total_seats < 1 ||
            flights[i].total_seats > MAX_SEATS) {
            printf("Invalid seat count!\n");
            for (int j = 0; j < i; j++)
                free(flights[j].passenger_list);
            free(flights);
            return 1;
        }

        flights[i].passenger_list =
            calloc(flights[i].total_seats, sizeof *flights[i].passenger_list);

        if (flights[i].passenger_list == NULL) {
            printf("Memory allocation failed!\n");
            for (int j = 0; j < i; j++)
                free(flights[j].passenger_list);
            free(flights);
            return 1;
        }

        initialize(&flights[i]);
    }

    do {
        int index;

        printf("\n--- FLIGHT MANAGEMENT SYSTEM ---\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Search Passenger\n");
        printf("4. View Flight Details\n");
        printf("5. Summary Report\n");
        printf("6. Save Report to File\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            break;

        if (choice == 0)
            break;

        if (choice < 1 || choice > 6) {
            printf("Invalid choice!\n");
            continue;
        }

        printf("Enter flight number: ");
        if (scanf("%d", &index) != 1)
            break;

        int found = -1;

        for (int i = 0; i < n; i++) {
            if (flights[i].flight_no == index) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Flight not found!\n");
            continue;
        }

        switch (choice) {
            case 1:
                book_ticket(&flights[found]);
                break;
            case 2:
                cancel_ticket(&flights[found]);
                break;
            case 3:
                search_passenger(&flights[found]);
                break;
            case 4:
                view_flight(&flights[found]);
                break;
            case 5:
                report(&flights[found]);
                break;
            case 6:
                save_report(&flights[found]);
                break;
        }

    } while (choice != 0);

    for (int i = 0; i < n; i++)
        free(flights[i].passenger_list);

    free(flights);

    printf("Program terminated successfully.\n");
    return 0;
}



