
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[255];
    char surname[255];
    char email[255];
    char department[255];
    char birthday[255];
    int id;
} Patient;

Patient *PatientList;
int patientCount = 0;
static int idCounter = 0;

void deleteRecord();
void searchPatient();
void updateRecord();
void idSetter(FILE *x);
void newRecord();
void showList();
void endProgram();
void createNewPatient(char* name, char* surname, char* birthday, char* email, char* department);
void savePatientsToFile();
void loadPatientsFromFile();
int isValidString(char *name);
int isValidBirthday(char *birthday);
int isValidEmail(char *email);

int main() {
    FILE *idCount = fopen("id.txt", "r");
    idSetter(idCount);
    loadPatientsFromFile();

    while (1) {
        printf("\n----------Welcome to the hospital management system-----------\n");
        printf("--------------------------------------------------------------\n\n");
        printf("0: Exit    |\t1: Show all records\t|\t2: Search patient\t|\t3: New patient\t\n");
        printf("Please select an option: ");
        int input;
         if (scanf("%d", &input) != 1) {
            printf("Invalid input, please enter a number.\n\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (input) {
            case 0:
                endProgram();
                exit(0);
            case 1:
                showList();
                printf("\n0: Return to main menu\t|   1:Update record\t|   2:Delete record\n");
                printf("Please select an option: ");
                int choice;
                scanf("%d", &choice);
                switch (choice) {
                    case 0:
                        break;
                    case 1:
                        updateRecord();
                        break;
                    case 2:
                        deleteRecord();
                        break;
                    default:
                        printf("Invalid option, please try again.\n");
                        break;
                }
                break;
            case 2:
                searchPatient();
                break;
            case 3:
                newRecord();
                break;
            default:
                printf("Invalid option, please try again.\n");
                break;
        }
    }

    return 0;
}

void createNewPatient(char* name, char* surname, char* birthday, char* email, char* department) {
    Patient newPatient;
    strcpy(newPatient.name, name);
    strcpy(newPatient.surname, surname);
    strcpy(newPatient.birthday, birthday);
    strcpy(newPatient.email, email);
    strcpy(newPatient.department, department);
    newPatient.id = idCounter;

    PatientList = realloc(PatientList, sizeof(Patient) * (patientCount + 1));
    PatientList[patientCount] = newPatient;
    patientCount++;
    idCounter++;
}

void deleteRecord() {
    int id, found = 0;
    printf("Enter the ID of the patient to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < patientCount; i++) {
        if (PatientList[i].id == id) {
            found = 1;
            for (int j = i; j < patientCount - 1; j++) {
                PatientList[j] = PatientList[j + 1];
            }
            patientCount--;
            PatientList = realloc(PatientList, sizeof(Patient) * patientCount);
            printf("\nPatient record with ID:%d deleted successfully...\n\n", id);
            savePatientsToFile();
            break;
        }
    }

    if (!found) {
        printf("\nNo patient found with ID:%d...\n\n", id);
    }
}

void searchPatient() {
    printf("\n0:Return to main menu\t|   1:Search by name\t|   2:Search by surname\t|   3:Search by ID\n");
    printf("Please select an option: ");
    int option;
    scanf("%d", &option);

    char input[255];
    int id, found = 0;

    switch (option) {
        case 0:
            return;
        case 1:
            printf("Patient name: ");
            scanf("%s", input);
            for (int i = 0; i < patientCount; i++) {
                if (strcmp(PatientList[i].name, input) == 0) {
                    found = 1;
                    printf("\n%-5d %-20s %-20s %-15s %-30s %-15s\n", PatientList[i].id, PatientList[i].name, PatientList[i].surname, PatientList[i].birthday, PatientList[i].email, PatientList[i].department);

                }
            }
            break;
        case 2:
            printf("Patient surname: ");
            scanf("%s", input);
            for (int i = 0; i < patientCount; i++) {
                if (strcmp(PatientList[i].surname, input) == 0) {
                    found = 1;
                    printf("\n%-5d %-20s %-20s %-15s %-30s %-15s\n", PatientList[i].id, PatientList[i].name, PatientList[i].surname, PatientList[i].birthday, PatientList[i].email, PatientList[i].department);
                }
            }
            break;
        case 3:
            printf("Patient ID: ");
            scanf("%d", &id);
            for (int i = 0; i < patientCount; i++) {
                if (PatientList[i].id == id) {
                    found = 1;
                    printf("\n%-5d %-20s %-20s %-15s %-30s %-15s\n", PatientList[i].id, PatientList[i].name, PatientList[i].surname, PatientList[i].birthday, PatientList[i].email, PatientList[i].department);
                }
            }
            break;
        default:
            printf("Invalid option, returning to main menu.\n");
            return;
    }

    if (!found) {
        printf("\nNo patient found with the given criteria.\n\n");
    }
}

void updateRecord() {
    int id, found = 0;
    char newName[255], newSurname[255], newBirthday[255], newEmail[255], newDepartment[255];

    printf("Enter the ID of the patient to update: ");
    scanf("%d", &id);

    for (int i = 0; i < patientCount; i++) {
        if (PatientList[i].id == id) {
            found = 1;
            printf("\nEnter a new name: (Old: %s)",PatientList[i].name);
            scanf("%s", newName);
            while (!isValidString(newName)) {
                printf("Invalid input. Enter a valid name: (Old: %s)",PatientList[i].name);
                scanf("%s", newName);
            }
            printf("\nEnter a new surname: (Old: %s)",PatientList[i].surname);
            scanf("%s", newSurname);
            while (!isValidString(newSurname)) {
                printf("Invalid input. Enter a valid surname: (Old: %s)",PatientList[i].surname);
                scanf("%s", newSurname);
            }
            printf("\nEnter a new birthday (dd/mm/yyyy): (Old: %s)",PatientList[i].birthday);
            scanf("%s", newBirthday);
            while (!isValidBirthday(newBirthday)) {
                printf("Invalid input. Enter a valid birthday (dd/mm/yyyy): (Old: %s)",PatientList[i].birthday);
                scanf("%s", newBirthday);
            }
            printf("\nEnter a new email: (Old: %s)",PatientList[i].email);
            scanf("%s", newEmail);
            while (!isValidEmail(newEmail)) {
                printf("Invalid input. Enter a valid Email: (Old: %s)",PatientList[i].email);
                scanf("%s", newEmail);
            }
            printf("\nEnter a new department: (Old: %s)",PatientList[i].department);
            scanf("%s", newDepartment);

            strcpy(PatientList[i].name, newName);
            strcpy(PatientList[i].surname, newSurname);
            strcpy(PatientList[i].birthday, newBirthday);
            strcpy(PatientList[i].email, newEmail);
            strcpy(PatientList[i].department, newDepartment);

            printf("\nPatient record with ID:%d updated successfully...\n\n", id);
            savePatientsToFile();
            break;
        }
    }

    if (!found) {
        printf("\nNo patient found with ID:%d...\n\n", id);
    }
}

void newRecord() {
    char tempname[50], tempsurname[50], tempbirthday[50], tempemail[50], tempdepartment[50];
    printf("Enter name: ");
    scanf("%s", tempname);
    while (!isValidString(tempname)) {
        printf("Invalid input. Enter a valid name: ");
        scanf("%s", tempname);
    }
    printf("\nEnter surname: ");
    scanf("%s", tempsurname);
    while (!isValidString(tempsurname)) {
        printf("Invalid input. Enter a valid surname: ");
        scanf("%s", tempsurname);
    }
    printf("\nEnter email: ");
    scanf("%s", tempemail);
    while (!isValidEmail(tempemail)) {
        printf("Invalid input. Enter a valid Email: ");
        scanf("%s", tempemail);
    }
    printf("\nEnter birthday (dd/mm/yyyy): ");
    scanf("%s", tempbirthday);
    while (!isValidBirthday(tempbirthday)) {
        printf("Invalid input. Enter a valid birthday (dd/mm/yyyy): ");
        scanf("%s", tempbirthday);
    }
    printf("\nEnter department: ");
    scanf("%s", tempdepartment);
    while (!isValidString(tempdepartment)) {
        printf("Invalid input. Enter a valid department: ");
        scanf("%s", tempdepartment);
    }
    createNewPatient(tempname, tempsurname, tempbirthday, tempemail, tempdepartment);
    printf("\nNew patient has been recorded...\n\n");
    savePatientsToFile();
}

void showList() {
    printf("\n%-5s %-20s %-20s %-15s %-30s %-15s\n", "ID", "Name", "Surname", "Birthday", "Email", "Department");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patientCount; i++) {
        printf("%-5d %-20s %-20s %-15s %-30s %-15s\n", PatientList[i].id, PatientList[i].name, PatientList[i].surname, PatientList[i].birthday, PatientList[i].email, PatientList[i].department);
    }
    printf("\n");
}

// void showList() {
//     printf("\nPATIENT RECORDS:\n\n");
//     for (int i = 0; i < patientCount; i++) {
//         printf("%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", PatientList[i].id, PatientList[i].name, PatientList[i].surname, PatientList[i].birthday, PatientList[i].email, PatientList[i].department);
//     }
// }

void idSetter(FILE *x) {
    if (fscanf(x, "%d", &idCounter) == EOF) {
        FILE *idFile = fopen("id.txt", "w");
        fprintf(idFile, "%d\n", 0);
        idCounter = 1;
        fclose(idFile);
    }
    fclose(x);
}

void endProgram() {
    FILE *x = fopen("id.txt", "w");
    fprintf(x, "%d", idCounter);
    fclose(x);
    free(PatientList);
}

void savePatientsToFile() {
    FILE *file = fopen("hospital.txt", "w");
    for (int i = 0; i < patientCount; i++) {
        fprintf(file, "%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", PatientList[i].id, PatientList[i].name, PatientList[i].surname, PatientList[i].birthday, PatientList[i].email, PatientList[i].department);
    }
    fclose(file);

     FILE *idFile = fopen("id.txt", "w");
    fprintf(idFile, "%d\n", idCounter);
    fclose(idFile);
}

void loadPatientsFromFile() {
    FILE *file = fopen("hospital.txt", "r");
    if (file == NULL) {
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        Patient tempPatient;
        sscanf(buffer, "%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s", &tempPatient.id, tempPatient.name, tempPatient.surname, tempPatient.birthday, tempPatient.email, tempPatient.department);
        PatientList = realloc(PatientList, sizeof(Patient) * (patientCount + 1));
        PatientList[patientCount] = tempPatient;
        patientCount++;
    }

    fclose(file);
}

int isValidString(char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidBirthday(char *birthday) {
    if (strlen(birthday) != 10) return 0;
    if (birthday[2] != '/' || birthday[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(birthday[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidEmail(char *email) {
    int atIndex = -1, dotIndex = -1;
    int length = strlen(email);

    for (int i = 0; i < length; i++) {
        if (email[i] == '@') atIndex = i;
        else if (email[i] == '.' && atIndex != -1) dotIndex = i;
    }

    return (atIndex > 0 && dotIndex > atIndex && dotIndex < length - 1);
}