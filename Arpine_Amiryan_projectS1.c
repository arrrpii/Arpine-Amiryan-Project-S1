#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Username "user"
#define Password "pass"


//Declaring type struct Metalprof for linked list nodes, that will contain data of the product 

struct Detail {
    char serial_number[20];
    int quantity;
    char location[10];
    char name[20];
    struct Detail* next;   //and pointer to the next node
};

//Declaring type struct Rack for storing data about what details from storage do we need for constructing certain product 

struct Rack{
    int columns;
    int legs;
    int shelf;
}rack1, rack2, rack3, final_rack;

//Function for checking whether username,password are correct for having access to the system
//they are already defined in the beginning

int login(){
    char username[10];
    char password[10];
    int trial = 5;

    do {
        printf("Enter the username: ");
        scanf("%s", username);
        printf("Enter the password: ");
        scanf("%s", password);
        if (strcmp(username, Username) == 0 && strcmp(password, Password) == 0 ){
            printf("You have succesfully logged in !\n ");
            return 1;
        }
            else {
                printf("Wrong username or password.\nTry again");
                printf("\n");
                trial = trial - 1;
            }
        } while (trial > 0);


    printf("Login failed");
    return 0;
};

//Function that at the beginning of the program will take all data from file and place it into linked list for further operations

void loadfromfile(struct Detail** head) {
    // Checking if the linked list is empty
    if (*head != NULL) {
        return;
    }
    //opening file from what will data be loaded
    FILE* fptr = fopen("storage_data.txt", "r");
    if (fptr == NULL) {
        printf("Error while opening file.\n");
        exit(1);
    }
    //Checking wether file is reached its end or not for placing data into nodes
    while (!feof(fptr)) {
        struct Detail* new = (struct Detail*)malloc(sizeof(struct Detail));
        if (new == NULL) {
            printf("Failed memory allocation.\n");
            exit(1);
        }

        if (fscanf(fptr, "%9s | %d | %39s | %19s", new->serial_number, &new->quantity, new->location, new->name) != 4) {
            free(new);
            break;
        }

        new->next = *head;
        *head = new;
    }

    fclose(fptr);
}

//Function that adds products and their data into the file of storage  

void addelement(struct Detail** head) {
    
    struct Detail* new = (struct Detail*)malloc(sizeof(struct Detail));
    if (new == NULL) {
        exit(1);
    }
    //Creating "current" pointer with value of the pointer *head, for traversing linked list and repeating through linked list while working with nodes
    struct Detail* current = *head;
    printf("Enter the serial number: ");
    scanf("%9s", new->serial_number);
    //Checking whether the added product is new or not
    while(current != NULL){
        if(strcmp(current->serial_number,new->serial_number) == 0){
            printf("This item already exists.\n");
            return;
        } current = current->next;
    }     
    //Entering all information about certain product
    printf("Enter the quantity: ");
    scanf("%d", &(new->quantity));

    printf("Enter the location: ");
    scanf("%39s", new->location);

    printf("Enter the name: ");
    scanf("%s", new->name);


    new->next = *head;  
    *head = new;  

    printf("Product added successfully!\n");
    //Opening file for appending new product into it 
    FILE* fptr = fopen("storage_data.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file for writing.\n");
        exit(1);
    }
    fprintf(fptr, "%s | %d | %s | %s\n", new->serial_number, new->quantity, new->location, new->name); 
    fclose(fptr);
}

//Function for searching products in linked list and printing all existing information  

void searchelement(struct Detail* head, const char* searchname) {
   struct Detail *current = head;

    while (current != NULL) {
        if (strcmp(current->name, searchname) == 0) {
            printf("\nProduct Information:\n");
            printf("\n");
            printf("Serial number: %s\n", current->serial_number);
            printf("Quantity: %d\n", current->quantity);
            printf("Location: %s\n", current->location);
            printf("Name: %s\n", current->name);

            return;  
        }
        current = current->next;
    }

    
    printf("Product with name %s not found.\n", searchname);
}

//Function for modifyng quantities of products 

void modifyquantity(struct Detail** head, const char* serialnum, int addquantity) {
    struct Detail* current = *head;
    
    //Opening file in both write and read modes, for rewriting all information after changing quantity
        FILE* fptr = fopen("storage_data.txt", "r+");
            if(fptr == NULL){
                printf("Error while opening file");
                exit(1);
            }

        while(current != NULL){   
            if(strcmp(current->serial_number, serialnum) == 0){
                current->quantity = current->quantity + addquantity;

                rewind(fptr);
            
                while(current != 0){
                    fprintf(fptr, "%s | %d | %s | %s\n", current->serial_number, current->quantity, current->location,current->name);
                    current = current->next;
                }
                fclose(fptr);
                printf("Qyantity succesfully updated !");
                return;
            }
                current = current->next;
        } 
        fclose(fptr);
        printf("Such product does not exist");
}

//Function for deallocating memory that was dynamically allocated for linked list
//This is done for avoiding several errors, such as memory leak 

void deallocating_memory(struct Detail* head) {
    struct Detail* current = head;
    struct Detail* next;

    while (current != NULL) {                       //Traversing linked list, saving every next node before freeing the current 
        next = current->next;     
        free(current);                              //Deallocating memory for this current node and moving into the next node
        current = next;            
    }
}

//Function for doing optimized and beneficial calculations for making product from existing details  
//This function is just representig calculations for one type of product - Rack, which is constracted from columns, legs and shelves

void Rackcalculate(int length){
    rack1.columns = 2;            
    rack1.legs = 2;              //First type of rack (1 meter rack ), needs this much details from storage
    rack1.shelf = 4;                

    rack2.columns = 3;
    rack2.legs = 3;              //Second type of rack ( 2 meter rack ), need this much details
    rack2.shelf = 8;

    rack3.columns = 4;
    rack3.legs = 4;              //Third type of rack (3 meter rack ), need this much details
    rack3.shelf = 12; 

    
    printf("Enter the lenght of needed rack: ");         //how meter rack does the employee need to construct
    scanf("%d", &length);

    if (length <= 0){
        printf("Wrong length !");                        
        exit(1);
    }
    else if(length == 1){
         final_rack = rack1;                            //Checking whether required meter of rack is standard, if yes, do not need to do calculations 
    }else if(length == 2){
        final_rack = rack2;
    }

    int rack2_quantity;                               //Main calculations, that count the most preferable way of choosing details
    int rack1_quantity;                               //The most beneficial way to construct rack, will contain have most details with type 3 as possible
    int rack3_quantity = length / 3;                 
    int residual = length % 3;
    if( residual == 2){
         rack2_quantity = 1;
         rack1_quantity = 0;

    } else  if( residual == 1){
            rack2_quantity = 0;
            rack1_quantity = 1;

    } else{
            rack2_quantity = 0;
            rack1_quantity = 0;
    };                                        //Finally giving information how much of certain details do employee need to take from storage for this current rack
    
    final_rack.columns = (rack1.columns * rack1_quantity) + (rack2.columns * rack2_quantity) + (rack3.columns * rack3_quantity);
    final_rack.legs = (rack1.legs * rack1_quantity) + (rack2.legs * rack2_quantity) + (rack3.legs * rack3_quantity);
    final_rack.shelf = (rack1.shelf * rack1_quantity) + (rack2.shelf * rack2_quantity) + (rack3.shelf * rack3_quantity);
    
    printf("For %d meter rack, you need:\n%d -  Column-c1000 \n%d - Leg-c150 \n%d -  shelf-w1000", length, final_rack.columns, final_rack.legs, final_rack.shelf);   
}


int main(){
    if (!login()) {
        return 1;
    }
    int lenght;

    struct Detail* head = NULL;  //list is empty
    loadfromfile(&head); //load from file data into linked list

    int menu_options;
    do {
        printf("\nMetal Production Management System\n");
        printf("\n");
        printf(" Press '1' to enter a new product:\n ");
        printf("Press '2' to search a product:\n ");
        printf("Press '3' to modify quantity:\n ");
        printf("Press '4' to do optimized calculation:\n ");
        printf("Press '5' to exit:\n ");
        printf("\n");
        scanf("%d", &menu_options);

        switch(menu_options){
            case 1:
                addelement(&head);
                break;
            case 2:
                printf("Enter the name of the product: ");
                char searchname[10];
                scanf("%s", searchname);
                searchelement(head, searchname);
                break;
            case 3:
                printf("Enter serial number to modify the quantity: ");
                char serialnum[20];
                scanf("%s", serialnum);

                int addquantity;
                printf("Enter quantity to add: ");
                scanf("%d", &addquantity);
                modifyquantity(&head, serialnum, addquantity);
                break;
            case 4:
                Rackcalculate(lenght);
                break;
            case 5:
                printf("Closing the program...\n");
                break;  
            default:
                printf("That option does not exist.\n");
                return 0;
        }
    
      } while (menu_options != 5);
    
    deallocating_memory(head); //freeing memory
    return 0;

}