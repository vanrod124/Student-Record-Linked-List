// Defines pre-processor directives.
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <iomanip>

using namespace std;

// Declares and initializes file-scope variables.
const int MAX_STRING_LENGTH = 64;

// Declares the filename of the record
const char *DATABASE_FILENAME = "student record.txt";
// Declares the delimiter for file input/output.
const char DELIMITER = '|';

// Sets menu items as elements of an array for ease of printing using for loops.
const char MAIN_MENU_ITEMS[][MAX_STRING_LENGTH] = {"Add New Record", "Search Record", "Display All Records",
                                      "Display Specific Record", "Delete Record", "Exit"};
// Gets the number of rows of an array using the operator "sizeof."
const int MAIN_MENU_ITEMS_SIZE = (sizeof(MAIN_MENU_ITEMS) / sizeof(MAIN_MENU_ITEMS[0]));

// 
const char SEARCH_STUDENT_MENU_ITEMS[][MAX_STRING_LENGTH] = {"ID Number", "Full Name", "Birthday", "Address", "Gender",
                                              "Degree Program", "Year Level"};
const int SEARCH_STUDENT_MENU_ITEMS_SIZE = (sizeof(SEARCH_STUDENT_MENU_ITEMS) / sizeof(SEARCH_STUDENT_MENU_ITEMS[0]));

// Declate the item of genders are Male and Female in an array form
const char GENDER_ITEMS[][MAX_STRING_LENGTH] = {"Male", "Female"};

const int GENDER_ITEMS_SIZE = (sizeof(GENDER_ITEMS) / sizeof(GENDER_ITEMS[0]));

// Declare the year level in an array style
const char YEAR_LEVEL_ITEMS[][MAX_STRING_LENGTH] = {"Freshman", "Sophomore", "Junior", "Senior", "Terminal"};

const int YEAR_LEVEL_ITEMS_SIZE = (sizeof(YEAR_LEVEL_ITEMS) / sizeof(YEAR_LEVEL_ITEMS[0]));

// Defines the structure "Student."
typedef struct {
    int IDNumber;
    char fullName[MAX_STRING_LENGTH];
    char birthday[9];
    char address[MAX_STRING_LENGTH];
    int gender;
    char degreeProgram[MAX_STRING_LENGTH];
    int yearLevel;
} Student;

// Defines the structure "Node."
struct Node {
    Student student;
    Node *next;
};

// Declares and initializes file-scope variables.
char *filename;
int totalStudents = 0;
Node *head = NULL;

void insertNode(Student student, int type) {
    Node *newNode = new Node();

    // Inserts a node at the front.
    if (type == 0) {
        // Swaps pointers
        newNode->student = student;
        newNode->next = head;
        head = newNode;
    // Inserts a node at the back.
    } else if (type == 1) {
        Node *tail = head;

        newNode->student = student;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;

            return;
        }

        while (tail->next != NULL) {
            tail = tail->next;
        }

        tail->next = newNode;
    }
}

// Inserts a node after a given node.
void insertNode(Node *previousNode, Student student) {
    if (previousNode == NULL) {
        return;
    }

    Node *newNode = new Node();

    newNode->student = student;
    newNode->next = previousNode->next;
    previousNode->next = newNode;
}

// Deletes a node.
void deleteNode(int index) {
    if (head == NULL) {
        return;
    }

    Node *temp = head;

    if (index == 0) {
        head = temp->next;

//            free(temp);

        return;
    }

    for (int i = 0; (temp != NULL) && (i < index - 1); i++) {
        temp = temp->next;
    }

    if ((temp == NULL) || (temp->next == NULL)) {
        return;
    }

    Node *next = temp->next->next;

//        free(temp->next);
    temp->next = next;
}

// Defines a function prototype.
void tokenize(FILE *file, char temp[]);

// Reads the database.
void readFile() {
    // Opens a file for reading.
    FILE *file = fopen(filename, "r");
    char temp[MAX_STRING_LENGTH] = "";

    if (file != NULL) {
        // Reads the total number of students and sets it as value of the integer "totalStudents."
        tokenize(file, temp);
        // Transforms the data type of the array "temp" from character to integer.
        totalStudents = atoi(temp);
        // Sets the value of all of the elements of the array "temp" as "\0" for reuse.
        memset(temp, '\0', sizeof(temp));

        for (int i = 0; i < totalStudents; i++) {
            Student student;

            // Reads a student number and stores it in the structure "student."
            tokenize(file, temp);
            student.IDNumber = atoi(temp);
            memset(temp, '\0', sizeof(temp));

            // Reads a full name and stores it in the structure "student."
            tokenize(file, temp);
            strcpy(student.fullName, temp);
            memset(temp, '\0', sizeof(temp));

            // Reads a birthday and stores it in the structure "student."
            tokenize(file, temp);
            strcpy(student.birthday, temp);
            memset(temp, '\0', sizeof(temp));

            // Reads an address and stores it in the structure "student."
            tokenize(file, temp);
            strcpy(student.address, temp);
            memset(temp, '\0', sizeof(temp));

            // Reads a gender and stores it in the structure "student."
            tokenize(file, temp);
            student.gender = atoi(temp);
            memset(temp, '\0', sizeof(temp));

            // Reads a degree program and stores it in the structure "student."
            tokenize(file, temp);
            strcpy(student.degreeProgram, temp);
            memset(temp, '\0', sizeof(temp));

            // Reads a year level and stores it in the structure "student."
            tokenize(file, temp);
            student.yearLevel = atoi(temp);
            memset(temp, '\0', sizeof(temp));

            // Inserts the structure "student" as a node at the back.
            insertNode(student, 1);
        }
    }

    // Closes a file.
    fclose(file);
}

// Writes the database.
void writeFile() {
    // Opens a file for writing.
    FILE *file = fopen(filename, "w");

    if (file != NULL) {
        Node *node = head;

        fprintf(file, "%d\n", totalStudents);

        while (node != NULL) {
            // Writes student attributes, which are separated by a delimiter, on the file.
            fprintf(file, "%d%c", node->student.IDNumber, DELIMITER);
            fprintf(file, "%s%c", node->student.fullName, DELIMITER);
            fprintf(file, "%s%c", node->student.birthday, DELIMITER);
            fprintf(file, "%s%c", node->student.address, DELIMITER);
            fprintf(file, "%d%c", node->student.gender, DELIMITER);
            fprintf(file, "%s%c", node->student.degreeProgram, DELIMITER);

            if (node->next != NULL) {
                fprintf(file, "%d%c", node->student.yearLevel, '\n');
            } else {
                fprintf(file, "%d", node->student.yearLevel);
            }

            node = node->next;
        }
    }

    // Closes a file.
    fclose(file);
}

// Tokenizes strings in a file.
void tokenize(FILE *file, char temp[]) {
    int character;

    for (int i = 0; !(feof(file)); i++) {
        character = fgetc(file);

        if ((character != '|') && (character != '\n') && (character != EOF)) {
            temp[i] = (char) character;
        } else {
            break;
        }
    }
}

// Defines function prototypes.
void printAddStudentMenu();
void printSearchStudentMenu();
void printStudents();
void printGetStudentMenu();
void printDeleteStudentMenu();

void printMainMenu() {
    int selectedMainMenuItemIndex;

    cout << "Welcome to the Student Information System!" << endl;

    do {
        cout << endl;

        for (int i = 0; i < MAIN_MENU_ITEMS_SIZE; i++) {
            cout << "[" << (i + 1) << "] " << MAIN_MENU_ITEMS[i] << endl;
        }
        
        // Take the initial input of the user
        do {
            cout << "\nWhat is the number that corresponds to the action that you would like to take?" << endl;
            cout << "Please enter the number: ";
            cin >> selectedMainMenuItemIndex;
            
            
            if (!((selectedMainMenuItemIndex >= 1) && (selectedMainMenuItemIndex <= MAIN_MENU_ITEMS_SIZE))) { // To handle error
                cout << "\nYou have entered an invalid number. Please try again." << endl;
            }
        } while (!((selectedMainMenuItemIndex >= 1) && (selectedMainMenuItemIndex <= MAIN_MENU_ITEMS_SIZE)));
        
        // Assign each condition to their respective functions in the main menu
        if (selectedMainMenuItemIndex == 1) { 
            printAddStudentMenu(); 
        } else if (selectedMainMenuItemIndex == 2) { 
            printSearchStudentMenu(); 
        } else if (selectedMainMenuItemIndex == 3) { 
            printStudents();
        } else if (selectedMainMenuItemIndex == 4) { 
            printGetStudentMenu();
        } else if (selectedMainMenuItemIndex == 5) { 
            printDeleteStudentMenu();
        }
    } while (selectedMainMenuItemIndex != MAIN_MENU_ITEMS_SIZE);
}

// It ask for user for the informations of the students
void printAddStudentMenu() {
    int selectedPrintAddStudentMenuItemIndex;

    Student student;

    cout << "\nID Number: ";
    cin >> student.IDNumber;

    // Clears the output buffer and prevents unexpected behavior that may be caused by an input that has a whitespace.
    fflush(stdin);
    cout << "Full Name: ";
    gets(student.fullName);
    fflush(stdin);

    cout << "Birthday (e.g., 01312021): ";
    cin >> student.birthday;

    fflush(stdin);
    cout << "Address: ";
    gets(student.address);
    fflush(stdin);

    for (int i = 0; i < GENDER_ITEMS_SIZE; i++) {
        cout << "\n" << "[" << (i + 1) << "] " << GENDER_ITEMS[i];
    }
    
    cout << "\n\nNote: In file /0/ means Male and /1/ means Female" << endl;
    cout << "\n\nGender: ";
    cin >> selectedPrintAddStudentMenuItemIndex;

    if ((selectedPrintAddStudentMenuItemIndex >= 1) &&
    (selectedPrintAddStudentMenuItemIndex <= GENDER_ITEMS_SIZE)) {
        student.gender = selectedPrintAddStudentMenuItemIndex - 1;
    }

    fflush(stdin);
    cout << "\n\nNote: In file /0/ means Male and /1/ means Female" << endl;
    cout << "Degree Program: ";
    gets(student.degreeProgram);
    fflush(stdin);

    for (int i = 0; i < YEAR_LEVEL_ITEMS_SIZE; i++) {
        cout << "\n" << "[" << (i + 1) << "] " << YEAR_LEVEL_ITEMS[i];
    }

    cout << "\n\nYear Level: ";
    cin >> selectedPrintAddStudentMenuItemIndex;

    if ((selectedPrintAddStudentMenuItemIndex >= 1) &&
    (selectedPrintAddStudentMenuItemIndex <= YEAR_LEVEL_ITEMS_SIZE)) {
        student.yearLevel = selectedPrintAddStudentMenuItemIndex - 1;
    }

    insertNode(student, 1);
    totalStudents++;
}

// Defines a function prototype.
void printStudent(Student *student);

// Search a specific student base on their attributes
void searchStudent(int type) {
    char studentAttribute[MAX_STRING_LENGTH];
    int totalOccurrences = 0;

    Node *temp = head;

    fflush(stdin);

    if (type == 2) {
        cout << "\nBirthday (e.g., 01312021): ";
    } else if (type == 4) {
        for (int i = 0; i < GENDER_ITEMS_SIZE; i++) {
            cout << "\n" << "[" << (i + 1) << "] " << GENDER_ITEMS[i];
        }

        cout << "\n\nGender: ";
    } else if (type == 6) {
        for (int i = 0; i < YEAR_LEVEL_ITEMS_SIZE; i++) {
            cout << "\n" << "[" << (i + 1) << "] " << YEAR_LEVEL_ITEMS[i];
        }

        cout << "\n\nYear Level: ";
    } else {
        cout << "\n" << SEARCH_STUDENT_MENU_ITEMS[type] << ": ";
    }

    gets(studentAttribute);
    fflush(stdin);

    while (temp != NULL) {
        if (((type == 0) && (atoi(studentAttribute) == temp->student.IDNumber)) ||
        ((type == 1) && (strcasecmp(studentAttribute, temp->student.fullName) == 0)) ||
        ((type == 2) && (strcasecmp(studentAttribute, temp->student.birthday) == 0)) ||
        ((type == 3) && (strcasecmp(studentAttribute, temp->student.address) == 0)) ||
        ((type == 4) && ((atoi(studentAttribute) - 1) == temp->student.gender)) ||
        ((type == 5) && (strcasecmp(studentAttribute, temp->student.degreeProgram) == 0)) ||
        ((type == 6) && ((atoi(studentAttribute) - 1) == temp->student.yearLevel))) {
            cout << endl;
            printStudent(&temp->student);
            totalOccurrences++;
        }

        temp = temp->next;
    }

    if (totalOccurrences == 0) {
        cout << "\n0 students found." << endl;
    }
}

void printSearchStudentMenu() {
    if (totalStudents == 0) {
        return;
    }

    int selectedSearchStudentMenuItemIndex;

    cout << endl;

    for (int i = 0; i < SEARCH_STUDENT_MENU_ITEMS_SIZE; i++) {
        cout << "[" << (i + 1) << "] " << SEARCH_STUDENT_MENU_ITEMS[i] << endl;
    }

    do {
        cout << "\nWhat is the number that corresponds to the action that you would like to take?" << endl;
        cout << "Please enter the number: ";
        cin >> selectedSearchStudentMenuItemIndex;

        if (!((selectedSearchStudentMenuItemIndex >= 1) &&
              (selectedSearchStudentMenuItemIndex <= SEARCH_STUDENT_MENU_ITEMS_SIZE))) {
            cout << "\nYou have entered an invalid number. Please try again." << endl;
        }
    } while (!((selectedSearchStudentMenuItemIndex >= 1) &&
    (selectedSearchStudentMenuItemIndex <= SEARCH_STUDENT_MENU_ITEMS_SIZE)));

    searchStudent((selectedSearchStudentMenuItemIndex - 1));
}

// Gets the gender that corresponds to a given index.
static const char *getGender(int gender) {
    if (gender == 0) {
        return "Male";
    } else if (gender == 1) {
        return "Female";
    } else {
        return NULL;
    }
}

// Gets the year level that corresponds to a given index.
static const char *getYearLevel(int yearLevel) {
    if (yearLevel == 0) {
        return "Freshman";
    } else if (yearLevel == 1) {
        return "Sophomore";
    } else if (yearLevel == 2) {
        return "Junior";
    } else if (yearLevel == 3) {
        return "Senior";
    } else if (yearLevel == 4) {
        return "Terminal";
    } else {
        return NULL;
    }
}

// Gets a substring from a given string using leftmost and rightmost indices.
void getSubstring(char string[], int leftmostIndex, int rightmostIndex) {
    printf("%.*s", (rightmostIndex - leftmostIndex) + 1, (string + leftmostIndex));
}


// Prints the attributes of a student.
void printStudent(Student *student) {
	cout << "=========================================" << endl;
    cout << "ID Number: " << student->IDNumber << endl;
    cout << "Full Name: " << student->fullName << endl;
    cout << "Birthday: ";
    getSubstring(student->birthday, 0, 1);
    cout << "/";
    getSubstring(student->birthday, 2, 3);
    cout << "/";
    getSubstring(student->birthday, 4, 7);
    cout << endl;
    cout << "Address: " << student->address << endl;
    cout << "Gender: " << getGender(student->gender) << endl;
    cout << "Degree Program: " << student->degreeProgram << endl;
    cout << "Year Level: " << getYearLevel(student->yearLevel) << endl;
    cout << "=========================================" << endl;
}

// Prints the respective attributes of all students in the database.
void printStudents() {
    if (totalStudents == 0) {
        return;
    }

    Node *temp = head;

    while (temp != NULL) {
        cout << endl;
        printStudent(&temp->student);

        temp = temp->next;
    }
}

// Prints the get student menu.
void printGetStudentMenu() {
    if (totalStudents == 0) {
        return;
    }

    int selectedPrintGetStudentMenuItemIndex;
    int i = 0;

    Node *temp = head;

    cout << endl;

    while (temp != NULL) {
        cout << "[" << (i + 1) << "] " << temp->student.fullName << endl;

        temp = temp->next;
        i++;
    }

    do {
        cout << "\nWhat is the number that corresponds to the action that you would like to take?" << endl;
        cout << "Please enter the number: ";
        cin >> selectedPrintGetStudentMenuItemIndex;

        if (!((selectedPrintGetStudentMenuItemIndex >= 1) &&
        (selectedPrintGetStudentMenuItemIndex <= totalStudents))) {
            cout << "\nYou have entered an invalid number. Please try again." << endl;
        }
    } while (!((selectedPrintGetStudentMenuItemIndex >= 1) &&
    (selectedPrintGetStudentMenuItemIndex <= totalStudents)));

    i = 0;
    temp = head;

    cout << endl;

    while (i != (selectedPrintGetStudentMenuItemIndex - 1)) {
        temp = temp->next;
        i++;
    }

    printStudent(&temp->student);
}

// Prints the delete student menu.
void printDeleteStudentMenu() {
    if (totalStudents == 0) {
        return;
    }

    int selectedPrintDeleteStudentMenuItemIndex;
    int i = 0;

    Node *temp = head;

    cout << endl;

    while (temp != NULL) {
        cout << "[" << (i + 1) << "] " << temp->student.fullName << endl;

        temp = temp->next;
        i++;
    }

    do {
        cout << "\nWhat is the number that corresponds to the action that you would like to take?" << endl;
        cout << "Please enter the number: ";
        cin >> selectedPrintDeleteStudentMenuItemIndex;

        if (!((selectedPrintDeleteStudentMenuItemIndex >= 1) &&
              (selectedPrintDeleteStudentMenuItemIndex <= totalStudents))) {
            cout << "\nYou have entered an invalid number. Please try again." << endl;
        }
    } while (!((selectedPrintDeleteStudentMenuItemIndex >= 1) &&
               (selectedPrintDeleteStudentMenuItemIndex <= totalStudents)));

    deleteNode((selectedPrintDeleteStudentMenuItemIndex - 1));
    totalStudents--;
}

// Defines the main function.
int main() {
    filename = (char *) DATABASE_FILENAME;

    readFile(); // Read the file first
    printMainMenu(); // Display the main menu secondly
    writeFile(); // display the input record to the file of "student record.txt"
    
    // Display the name of the group and its members
    cout << "\nSuper Talong Group\n" << endl;
    cout << "Programmers: \nLeader: Honey Rose Suliva  \nNatahalie Boco \nKing Sun Jaquilmo \nAaron Cano\n";
    cout << "\n\nThe record is stored at \"student record.txt\". Kindly search your files for that txt file.";
    cout << "\nThank you for using our program!" << endl;


    return 0;
}

