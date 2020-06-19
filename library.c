#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

void menuPage();
int nameCheck(const char* name);
#define fileName "library.bin"
#define fileHeadSize sizeof(fileHeader)
#define maxUserName 20
#define maxPassWord 20

//------------------------------------------------------------------------------------------------------------------------------------------

//  Structure to store the data
typedef struct DateDetails {
    int yyyy;   //  Year
    int mm;     //  Month
    int dd;     //  Date
}date;
//  Structure for the authentication in the file
typedef struct AuthenticationDetails {
    char userName [maxUserName];
    char password [maxPassWord];
}fileHeader;
//  Structure for details
typedef struct StorageDetails {
    unsigned int bookID;        //  the book id
    char bookName[100];         //  the book name
    char authorName[100];       //  the book author name
    char customerName[100];     //  the customers name
    char customerContact[100];   //  Customer Phone number
    date bookIssue;             //  book issue date
}bookInfo;

//----------------------------------------------------------------------------------------------------------------------------------------


//  FILE OPERATIONS -----------------------------------------------------------------------------------------------------------------------


//  Verifies if a file is created
int verifyFile(const char *path) {
    //  Opening the file
    FILE *fs = fopen(path, "rb");
    int status = 0;

    //  If the file doesn't exists
    if(fs!= NULL) {
        status = 1;
        //  File exists hence close file
        fclose(fs);
    }
    return status;
}

//  If the file doesn't exist it must be created
void createFile() {
    FILE *fs = NULL;
    int status = 0;
    const char dUserName[] = "admin\n";   //  default username
    const char dPassWord[] = "pass\n";    //  default password

    fileHeader fh = {0};
    status = verifyFile(fileName);

    if(status == 0) {
        //  Create a binary file
        fs = fopen(fileName,"wb");      //  Write into the library file in binarymode
        if(fs!= NULL) {
            //  Copy default username and pass
            strncpy(fh.userName, dUserName, sizeof(dUserName));
            strncpy(fh.password, dPassWord, sizeof(dPassWord));
            fwrite(&fh, fileHeadSize, 1, fs);       //  Write the username and pass in the file
            fclose(fs);
        }
    }
}   


//  DISPLAY OF THE PROGRAM ------------------------------------------------------------------------------------------------------------


//  Prints the message in the centre

void printCentre (const char* message) {
    int len = 0;
    //  Calculates the amount of spaces needed to print
    len = (78 - strlen(message))/2;
    printf("\t\t\t");
    for(int i=0;i<len; i++) {
        //  space printing
        printf(" ");
    }
    printf("%s",message);
}

//  Prints the software name

void softName(const char* message) {
    system("cls");      //  Clear screen
    printf(" ");
    printf("\n\n\n\t\t\t\t*******************************************************************************");
    printf("\n\n\t\t\t\t------------------------------------------------------------------------------");
    printf("\n\t\t\t\t------------------      LIBRARY BOOKS MANAGEMENT SYSTEM      -----------------");
    printf("\n\t\t\t\t------------------------------------------------------------------------------");
    printf("\n\n");
    printCentre(message);
    printf("\n\n\t\t\t\t*******************************************************************************");
}

//  Prints the greeting

void greeting() {
    softName("Created By Debanjan Paul \t On 2020");
    printf("\n\n");
    printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t\t\t|                                   WELCOME                                   |");
    printf("\n\t\t\t\t|                                TO THE SYSTEM                                |");
    printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\n\n\n\t\t\t\t...PRESS ANY KEY TO MOVE FURTHER...");
    getchar();
}



//  LOGIN -----------------------------------------------------------------------------------------------------------------------------

void loginPage() {
    unsigned char userName[maxUserName] = {0};
    unsigned char passWord[maxPassWord] = {0};
    int attempts=0;
    fileHeader fh = {0};
    FILE *fs =NULL;
    softName("LOGIN PAGE!");
    fs = fopen(fileName, "rb");
    if(fs == NULL) {        //  Checks if file is empty
        printf("File is not open.");
        exit(1);
    }
    fread(&fh, fileHeadSize, 1, fs);        //  reads from the file
    fclose(fs);                                         //  close the file 
    do {
        printf("\n\n\n\n\t\t\t\tPlease enter your User Name: ");
        fgets(userName, sizeof(userName), stdin);

        printf("\n\t\t\t\tPlease Enter the Password: ");
        fgets(passWord, sizeof(passWord), stdin);

        if((strncmp(userName, fh.userName, sizeof(userName)) == 0) && (strncmp(passWord, fh.password, sizeof(passWord)) == 0) )
        {
            menuPage();
        }
        else {
            printf("\n\t\t\t\tUsername or password entered is incorrect. Please try again!");
            attempts++;
        }
    }
    while(attempts<=3);
    if(attempts>3) {
        softName("LOGIN FAILED!");
        printf("\n\n\t\t\t\tYou have exceeded your login attempts!");
        getchar();
        system("cls");
    }
}


//  BOOK DETAILS AND CUSTOMER DETAILS -----------------------------------------------------------------------------------------------------


//  Add book

void bookAdd() {
    int day, status = 0;
    bookInfo addBook = {0};
    FILE *fs = NULL;
    fs = fopen(fileName,"ab+");              //  opening the binary file in append mode
    if(fs == NULL) {
        printf("File cannot be opened.");
        exit(1);
    }
    softName("ADD NEW BOOKS");
    printf("\n\n\t\t\t\tEnter Book Details\n");
    printf("\n\t\t\t\t*******************************************************************************");
    
    printf("\n\n\t\t\t\tBook ID Number: ");
    scanf("%u", &addBook.bookID);        // unsigned integer
    
    LOOP2:
    printf("\n\n\t\t\t\tBook Name: ");
    fflush(stdin);
    fgets(addBook.bookName, sizeof(addBook.bookName), stdin);
    status = nameCheck(addBook.bookName);
    if(!status) {
        printf("\n\n\n\t\t\t\tThe Book name contains an invalid character. Please Enter the name again.");
        goto LOOP2;
    }

    LOOP3:
    printf("\n\n\t\t\t\tEnter author name: ");
    fflush(stdin);
    fgets(addBook.authorName, sizeof(addBook.authorName), stdin);
    status = nameCheck(addBook.authorName);
    if(!status) {
        printf("\n\n\n\t\t\t\tThe Author Name contains an invalid character. Please Try again.");
        goto LOOP3;
    }

    LOOP4:
    printf("\n\n\t\t\t\tEnter customer name: ");
    fflush(stdin);
    fgets(addBook.customerName, sizeof(addBook.customerName), stdin);
    status = nameCheck(addBook.customerName);
    if(!status) {
        printf("\n\n\n\t\t\t\tThe Customer Name contains an invalid character. Please Try again.");
        goto LOOP4;
    }

    printf("\n\n\t\t\t\tEnter customer phone number: ");
    fflush(stdin);
    fgets(addBook.customerContact, sizeof(addBook.customerContact), stdin);

    LOOP5:
    printf("\n\n\t\t\t\tEnter date in format (DD/MM/YYYY): ");
    scanf("%d %d %d", &addBook.bookIssue.dd, &addBook.bookIssue.mm, &addBook.bookIssue.yyyy);
    status = dateCheck(&addBook.bookIssue);
    if(!status) {
        printf("\n\n\n\t\t\t\tThe Date format is wrong. Please try again.");
        goto LOOP5;
    }

    fwrite(&addBook, sizeof(addBook), 1, fs);
    fclose(fs);
}

//  Search book

void bookSearch() {
    int count = 0;
    char bookName[100] = {0};
    bookInfo addBook = {0};
    FILE *fs = NULL;
    int status = 0;
    fs = fopen(fileName, "rb");         //  Open in read binary file mode
    if(fs == NULL) {
        printf("\n\t\t\t\tFile cannot be opened.\n");
        exit(1);
    }
    softName("SEARCH FOR BOOK");
    if(fseek(fs, fileHeadSize, SEEK_SET) != 0) {
        fclose(fs);
        printf("\n\n\t\t\t\tSomething went wrong while reading the files.");
        exit(1);
    }
    printf("\n\n\t\t\t\tEnter a Book Name to search for: ");
    fflush(stdin);
    fgets(bookName, sizeof(bookName), stdin);

    while(fread(&addBook, sizeof(addBook), 1, fs)) {
        if(!strcmp(addBook.bookName, bookName)) {
            count = 1;
            break;
        }
    }
    if(count == 1) {
        printf("\n\t\t\t\tBook ID: %u\n", addBook.bookID);
        printf("\n\t\t\t\tBook name: %s\n", addBook.bookName);
        printf("\n\t\t\t\tAuthor Name: %s\n", addBook.authorName);
        printf("\n\t\t\t\tBook Issue Date: %d / %d / %d\n", addBook.bookIssue.dd, addBook.bookIssue.mm, addBook.bookIssue.yyyy);
    }
    else {
        printf("\n\n\t\t\t\tBook not found.");
    }
    fclose(fs);
    printf("\n\n\n\n\t....Press any key to go to the main menu....");
    getchar();
}

//  View book

void bookView() {
    int count = 0, status = 0;
    char bookName[100] = {0};
    bookInfo addBook = {0};
    FILE *fs = NULL;
    unsigned int bCount = 1;

    softName("VIEW BOOKS");
    fs = fopen(fileName, "rb");     //  Open in read binary file mode
    if(fs == NULL) {
        printf("File cannot be opened.");
        exit(0);
    }

    if(fseek(fs, fileHeadSize, SEEK_SET) != 0) {
        fclose(fs);
        printf("Something went wrong while reading the file");
        exit(1);
    }
    while (fread(&addBook, sizeof(addBook), 1, fs)) {
        printf("\n\n\t\t\t\tBook count: %d\n", bCount);
        printf("\n\t\t\t\tBook ID: %u\n", addBook.bookID);
        printf("\n\t\t\t\tBook name: %s\n", addBook.bookName);
        printf("\n\t\t\t\tAuthor Name: %s\n", addBook.authorName);
        printf("\n\t\t\t\tIssued to: %s\n", addBook.customerName);
        printf("\n\t\t\t\tCustomer contact number: %s\n", addBook.customerContact);
        printf("\n\t\t\t\tBook Issue Date: %d / %d / %d\n", addBook.bookIssue.dd, addBook.bookIssue.mm, addBook.bookIssue.yyyy);
        count = 1;
        ++bCount;
    }
    fclose(fs);

    if(count == 0) {
        printf("No records as of now!");
    }
    printf("\n\n\t\t\t\t Press any key to go to the main menu.");
    fflush(stdin);
    getchar();
}

//  Delete book

void bookDelete() {
    int count = 0, deleteBook = 0, status = 0;
    fileHeader header = {0};
    char bookName [100] = {0};
    bookInfo addBook = {0};
    FILE *fs = NULL;
    FILE *tfs = NULL;

    softName("DELETE BOOKS");

    fs = fopen(fileName, "rb");     //  read binary file mode
    if(fs == NULL) {
        printf("\n\t\t\t\tFile cannot be opened.");
        exit(1);
    }
    tfs = fopen("tmp.bin","wb");                        //  Open a temporary binary fine in write binary file mode
    if(tfs == NULL) {
        fclose(fs);
        printf("Data cannot be deleted.");
        exit(1);
    }
    fread(&header, fileHeadSize, 1, fs);            //  Read from the main file
    fwrite(&header, fileHeadSize, 1, tfs);            //  Write into the temporary file

    printf("\n\n\t\t\t\tEnter the Book ID to delete: ");
    scanf("%d", &deleteBook);

    while(fread(&addBook, sizeof(addBook), 1, fs)) {
        if(addBook.bookID != deleteBook) {
            fwrite(&addBook, sizeof(addBook), 1, tfs);
        }   
        else {
            count = 1;
        }
    }
    if(count == 1) {
        printf("\n\t\t\t\tRecord deleted succesfully.");
    }
    else {
        printf("Record cannot be deleted.");
    }
    fclose(fs);
    fclose(tfs);
    
    remove(fileName);
    rename("tmp.bin",fileName);
    //  Here all the data is copied into the tmp.bin file except the data to be deleted. Once moved the original file is deleted 
    //  and the tmp file is rename to the fileName
}

//  Update login

void userUpdate(void) {
    fileHeader fh = {0};
    FILE *fs = NULL;
    unsigned char userName[maxUserName] = {0};
    unsigned char password[maxPassWord] = {0};
    softName("Update Credentials");

    fs = fopen(fileName, "rb+");        //  open binary file for update
    if(fs == NULL) {
        printf("File Cannot be opened.");
        exit(1);
    }
    fread(&fh, fileHeadSize, 1, fs);
    if (fseek(fs, 0, SEEK_SET) != 0) {
        fclose(fs);
        printf("\n\n\t\t\t\tError occured while updating password.");
        exit(0);
    }
    
    printf("\n\n\t\t\t\tEnter the new username: ");
    fflush(stdin);
    fgets(userName, sizeof(userName), stdin);

    printf("\n\n\t\t\t\tEnter the new password: ");
    fflush(stdin);
    fgets(password, sizeof(password), stdin);

    strncpy(fh.userName, userName, sizeof(userName));
    strncpy(fh.password, password, sizeof(password));
    fwrite(&fh, fileHeadSize, 1, fs);

    fclose(fs);

    printf("\n\n\n\t\t\t\t Credentials updated successfully. Please login again!");
    fflush(stdin);
    getchar();
    exit(1);
}



//  AUTHENTICATION --------------------------------------------------------------------------------------------------------------------



//  validity for username, authorname, customername

int nameCheck(const char *name) {
    int vName = 1;
    
    for(int i=0;i<strlen(name);++i) {
        if((isalpha(name[i]) == 0) && (name[i] != '\n') && (name[i] != ' ')) {            //  Checks if the name does not contains alphanumeric, next line and spaces
            vName = 0;
            break;
        }          
    }
    return vName;
}

//  validity of the date and time

//  Leap year check
int leapYearCheck(int year) {
    if(((year % 4 == 0) && (year % 100 == 0)) || (year % 400 == 0) ) {
        return 1;
    }
    else {
        return 0;
    }
}

//  Valid date check
int dateCheck(date *vDate) {
    //  Checks the range of date, month and year
    if(vDate->yyyy > 9999 || vDate->yyyy < 1000) {
        return 0;
    }
    if(vDate->mm >12 || vDate->mm <1) {
        return 0;
    }
    if(vDate->dd > 31 || vDate->yyyy < 1) {
        return 0;
    }
    
    //  For Feb in leap year
    if(vDate->mm == 2) {
        if(leapYearCheck(vDate->yyyy)) {
            return(vDate->dd <=29);
        }
        else {
            return(vDate->dd <=28);
        }
    }
    
    //  For months with 30 days 
    if((vDate->mm == 4) || (vDate->mm == 6) || (vDate->mm == 9) || (vDate->mm == 11)) {
        return (vDate->dd <=30);
    }
    return 1;
}


//  MENU OF THE APPLICATION ---------------------------------------------------------------------------------------------------------------


void menuPage() {  
    int ch = 0;
    do {
        softName("MAIN MENU");
        printf("\n\n\t\t\t\t1. Add Book");
        printf("\n\t\t\t\t2. Search Book");
        printf("\n\t\t\t\t3. View Books");
        printf("\n\t\t\t\t4. Delete Book");
        printf("\n\t\t\t\t5. Update Password");
        printf("\n\t\t\t\t0. Exit");
        printf("\n\n Enter the choice: ");
        scanf("%d",&ch);
        switch(ch) {
            case 1:
                //  Add book            
                bookAdd();
                break;
            case 2:
                //  Search book         
                bookSearch();
                break;
            case 3:
                //  View book           
                bookView();
                break;
            case 4:
                //  Delete book         
                bookDelete();
                break;
            case 5:
                //  Update login       
                userUpdate();
                break;
            case 0:
                system("cls");
                printf("\n\n\n\n\t\t\t\tThank you for using this application.");
                exit(1);
                break;
            default:
                printf("\n\n\n\t\t\t\t Something went wrong. Try again.");
        
        }
    }
    while(ch!=0);
}




//  MAIN FUNCTION WHERE IT ALL STARTS -------------------------------------------------------------------------------------------------

int main() {
    createFile();
    greeting();
    loginPage(); 
    return 0;
}