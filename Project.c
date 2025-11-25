#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

// Structure to hold vehicle information
struct Vehicle {
    int id;
    char name[20];
    char type[20]; // Car, Bike, Bus
    float rentPerDay;
    int available; // 1 = available, 0 = rented
};

// Function prototypes
void addVehicle();
void viewVehicles();
void rentVehicle();
void returnVehicle();
void updateVehicle();
void deleteVehicle();
void generateReport();
void saveVehicle(struct Vehicle v);
int loadVehicles(struct Vehicle vList[]);

int main() {
    int choice;
    while(1) {
        printf("\n===== Vehicle Rental System =====\n");
        printf("1. Add Vehicle\n");
        printf("2. View Vehicles\n");
        printf("3. Rent Vehicle\n");
        printf("4. Return Vehicle\n");
        printf("5. Update Vehicle\n");
        printf("6. Delete Vehicle\n");
        printf("7. Generate Report\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addVehicle(); break;
            case 2: viewVehicles(); break;
            case 3: rentVehicle(); break;
            case 4: returnVehicle(); break;
            case 5: updateVehicle(); break;
            case 6: deleteVehicle(); break;
            case 7: generateReport(); break;
            case 0: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Add new vehicle
void addVehicle() {
    struct Vehicle v;
    printf("\nEnter Vehicle ID: ");
    scanf("%d", &v.id);
    printf("Enter Vehicle Name: ");
    scanf("%s", v.name);
    printf("Enter Vehicle Type (Car/Bike/Bus): ");
    scanf("%s", v.type);
    printf("Enter Rent per Day: ");
    scanf("%f", &v.rentPerDay);
    v.available = 1; // Vehicle is available by default

    saveVehicle(v);
    printf("Vehicle added successfully!\n");
}

// Save vehicle to file
void saveVehicle(struct Vehicle v) {
    FILE *fp = fopen("vehicles.dat", "ab");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&v, sizeof(struct Vehicle), 1, fp);
    fclose(fp);
}

// Load vehicles into array, returns count
int loadVehicles(struct Vehicle vList[]) {
    FILE *fp = fopen("vehicles.dat", "rb");
    if(fp == NULL) return 0;
    int i = 0;
    while(fread(&vList[i], sizeof(struct Vehicle), 1, fp) == 1) {
        i++;
    }
    fclose(fp);
    return i;
}

// View all vehicles
void viewVehicles() {
    struct Vehicle vList[MAX];
    int count = loadVehicles(vList);
    if(count == 0) {
        printf("No vehicles found!\n");
        return;
    }

    printf("\nID\tName\tType\tRent\tStatus\n");
    for(int i=0; i<count; i++) {
        printf("%d\t%s\t%s\t%.2f\t%s\n", vList[i].id, vList[i].name, vList[i].type, vList[i].rentPerDay, vList[i].available ? "Available" : "Rented");
    }
}

// Rent a vehicle
void rentVehicle() {
    int id, days;
    struct Vehicle vList[MAX];
    int count = loadVehicles(vList);
    if(count == 0) {
        printf("No vehicles available!\n");
        return;
    }

    printf("Enter Vehicle ID to rent: ");
    scanf("%d", &id);
    for(int i=0; i<count; i++) {
        if(vList[i].id == id) {
            if(vList[i].available) {
                printf("Enter number of days: ");
                scanf("%d", &days);
                vList[i].available = 0;
                printf("Vehicle rented successfully! Total rent: %.2f\n", days * vList[i].rentPerDay);
                
                // Update file
                FILE *fp = fopen("vehicles.dat", "wb");
                for(int j=0; j<count; j++)
                    fwrite(&vList[j], sizeof(struct Vehicle), 1, fp);
                fclose(fp);
                return;
            } else {
                printf("Sorry, vehicle is already rented.\n");
                return;
            }
        }
    }
    printf("Vehicle not found!\n");
}

// Return a vehicle
void returnVehicle() {
    int id;
    struct Vehicle vList[MAX];
    int count = loadVehicles(vList);
    if(count == 0) {
        printf("No vehicles in system!\n");
        return;
    }

    printf("Enter Vehicle ID to return: ");
    scanf("%d", &id);
    for(int i=0; i<count; i++) {
        if(vList[i].id == id) {
            if(!vList[i].available) {
                vList[i].available = 1;
                printf("Vehicle returned successfully!\n");
                
                // Update file
                FILE *fp = fopen("vehicles.dat", "wb");
                for(int j=0; j<count; j++)
                    fwrite(&vList[j], sizeof(struct Vehicle), 1, fp);
                fclose(fp);
                return;
            } else {
                printf("Vehicle is already available!\n");
                return;
            }
        }
    }
    printf("Vehicle not found!\n");
}

// Update vehicle info
void updateVehicle() {
    int id;
    struct Vehicle vList[MAX];
    int count = loadVehicles(vList);
    if(count == 0) {
        printf("No vehicles found!\n");
        return;
    }

    printf("Enter Vehicle ID to update: ");
    scanf("%d", &id);
    for(int i=0; i<count; i++) {
        if(vList[i].id == id) {
            printf("Enter new name: ");
            scanf("%s", vList[i].name);
            printf("Enter new type: ");
            scanf("%s", vList[i].type);
            printf("Enter new rent per day: ");
            scanf("%f", &vList[i].rentPerDay);

            FILE *fp = fopen("vehicles.dat", "wb");
            for(int j=0; j<count; j++)
                fwrite(&vList[j], sizeof(struct Vehicle), 1, fp);
            fclose(fp);

            printf("Vehicle updated successfully!\n");
            return;
        }
    }
    printf("Vehicle not found!\n");
}

// Delete a vehicle
void deleteVehicle() {
    int id;
    struct Vehicle vList[MAX];
    int count = loadVehicles(vList);
    if(count == 0) {
        printf("No vehicles found!\n");
        return;
    }

    printf("Enter Vehicle ID to delete: ");
    scanf("%d", &id);

    int found = 0;
    FILE *fp = fopen("vehicles.dat", "wb");
    for(int i=0; i<count; i++) {
        if(vList[i].id != id)
            fwrite(&vList[i], sizeof(struct Vehicle), 1, fp);
        else
            found = 1;
    }
    fclose(fp);

    if(found)
        printf("Vehicle deleted successfully!\n");
    else
        printf("Vehicle not found!\n");
}

// Generate simple report
void generateReport() {
    struct Vehicle vList[MAX];
    int count = loadVehicles(vList);
    if(count == 0) {
        printf("No vehicles in system!\n");
        return;
    }

    int rented = 0, available = 0;
    float totalIncome = 0;
    for(int i=0; i<count; i++) {
        if(vList[i].available)
            available++;
        else {
            rented++;
            // Assume 1 day rent for simplicity in report
            totalIncome += vList[i].rentPerDay;
        }
    }

    printf("\n===== Report =====\n");
    printf("Total Vehicles: %d\n", count);
    printf("Available Vehicles: %d\n", available);
    printf("Rented Vehicles: %d\n", rented);
    printf("Estimated Total Income (1 day each rented): %.2f\n", totalIncome);
}

