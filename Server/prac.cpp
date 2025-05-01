#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdio>  
#include <ctime>
#include <iomanip>
#include <vector>

using namespace std;
const int MAX_ITEMS = 100;
struct Item {
    int id;
    string name;
    int quantity;
}; 

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
struct User {
    int id;
    string username;
    string password;
    string name;
    string dept;
    string userAccess;
}; 

struct RequestItem {
	int reqId;
    int itemId;
    string itemName;
    int itemQuan;
    string borrowerName;
    int borrowerId;
    string date;
}; 

struct BorrowedItem {
	int reqId;
    int itemId;
    string itemName;
    int itemQuan;
    string borrowerName;
    int borrowerId;
    string borrowDate;
    
};
struct ReturnedItem{
        	int itemId;
        	string itemName;
        	int itemQuan;
        	int borrowerId;
        	string borrowerName;
        	string borrowDate;
        	string dateReturn;
}; 






void adminDashboard(int id, string name ,string username, string department, string userAccess,string password);
void userDashboard(int id, string name ,string username, string department, string userAccess,string password);
void loginUser();

void addUser(const User& user);
void addItem(const Item&item);

void readUsers();
void readItems();

void readAllUserRequest();
void readMyRequest(int id);
void readMyCancelled(int id);

void readMyBorrow(int id);
void readAllBorrowedItem();

void updateUser(int idToUpdate, const User& updatedUser);

void deleteItem(int idToDelete);
void deleteUser(int idToDelete);
void changePass(int id, string confirmedPass);

void requestItems();
void approveRequest(int requestIdToApprove);
void cancelItems(int idToCancel);

std::string getCurrentDate() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    std::ostringstream oss;
    oss << (1900 + localTime->tm_year) << "-"
        << std::setw(2) << std::setfill('0') << (1 + localTime->tm_mon) << "-"
        << std::setw(2) << std::setfill('0') << localTime->tm_mday;

    return oss.str();
}
//User ID getter
int getLastUserId() {
    ifstream file("data.txt");
    string line;
    int lastId = 0;

    while (getline(file, line)) {
        istringstream iss(line);
        int id;
        if (iss >> id) {
            lastId = id;
        }
    }

    return lastId;
}


//Utem ID Getter
int getLastItemId() {
    ifstream file("item.txt");
    string line;
    int lastId = 0;

    while (getline(file, line)) {
        istringstream iss(line);
        int id;
        if (iss >> id) {
            lastId = id;
        }
    }

    return lastId;
}
//Request Id getter
int getLastReqId() {
    ifstream file("requestItem.txt");
    string line;
    int lastId = 0;

    while (getline(file, line)) {
        istringstream iss(line);
        int id;
        if (iss >> id) {
            lastId = id;
        }
    }

    return lastId;
}

//Authentication// user dashboard
void loginUser() {
	clearScreen();
    string username, password;

    cout << "\n--- Login ---\n";
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;
	cin.ignore();
    ifstream file("data.txt");
    if (!file.is_open()) {
        cout << "Unable to open user database.\n";
        return;
    }

    User user;
    bool loginSuccess = false;

    while (file >> user.id >> user.name >> user.username >> user.password >> user.dept >> user.userAccess) {
        if (user.username == username && user.password == password) {
            loginSuccess = true;
            break;
        }
    }

    file.close();

    if (loginSuccess) {
    	if(user.userAccess == "admin" || user.userAccess == "ADMIN"|| user.userAccess == "Admin"){
    		adminDashboard(user.id, user.name ,user.username ,user.dept , user.userAccess, user.password);
		}else if(user.userAccess == "user"|| user.userAccess == "USER"|| user.userAccess == "User"){
			userDashboard(user.id ,user.name ,user.username ,user.dept , user.userAccess,user.password);
		}else{
			return;
		}
		
		
        
    } else {
        cout << "Invalid username or password.\n";
    }
}

//User adder
void addUser(const User& user) {
	clearScreen();
    ofstream file("data.txt", ios::app);
    if (file.is_open()) {
        file << user.id << " " << user.name << " " << user.username << " " << user.password << " " << user.dept << " "
		<<user.userAccess<< endl;

        file.close();
        cout << "User added.\n";
    }
}
//Item ADder
void addItem(const Item& item) {
	clearScreen();
    ofstream file("item.txt", ios::app);
    if (file.is_open()) {
        file << item.id << " " << item.name << " "<< item.quantity << endl;

        file.close();
        cout << "Item added.\n";
    }
}

//user requesting an item to borrow
void requestItems(int idToBorrow,int quantity,string name, int id , string date) {
    clearScreen();
	int lastId = getLastReqId() +1;
    ifstream inFile("item.txt");
    if (!inFile) {
        cerr << "Error: Unable to open item.txt for reading.\n";
        return;
    }

    ofstream outFile("requestItem.txt", ios::app);
    if (!outFile) {
        cerr << "Error: Unable to open requestItem.txt for writing.\n";
        inFile.close();
        return;
    }

    RequestItem req;
    bool found = false;
	
    while (inFile >> req.itemId >> req.itemName >> req.itemQuan) {
        if (req.itemId == idToBorrow) {
        	if(req.itemQuan >= quantity){
        	outFile << lastId<< " "<<req.itemId << " " << req.itemName << " " << quantity <<" "<< id<< " "<<
				name <<" "<< date << endl;
            found = true;	
            cout << "Request for " << quantity << " units of " << req.itemName << " has been added.\n";
			}else{
				cout << "Insufficient stock for " << req.itemName << ". Only " << req.itemQuan << " units are available.\n";
                return;
			}
			
            
        }
    }

    if (!found) {
        cout << "Item with ID " << idToBorrow << " not found." << endl;
    }

    inFile.close();
    outFile.close();
}


//retorning aytim
void returnItem(int idToReturn) {
	clearScreen();
    // Open the necessary files
    string dateReturn = getCurrentDate();
    
    ifstream borrowedFile("borrowedItem.txt");
    ifstream itemFile("item.txt");
    ofstream tempItemFile("tempItem.txt", ios::app);
    ofstream tempBorrowedFile("tempBorrowedItem.txt", ios::app);
    ofstream returnRecord("returnRecord.txt", ios::app);

    if (!borrowedFile || !itemFile) {
        cerr << "Error: Unable to open files for reading.\n";
        return;
    }

    Item item;
    BorrowedItem borrowedItem;
    bool found = false;

    while (borrowedFile >> borrowedItem.reqId >> borrowedItem.itemId >> borrowedItem.itemName 
                        >> borrowedItem.itemQuan >> borrowedItem.borrowerId >> borrowedItem.borrowerName 
                        >> borrowedItem.borrowDate) {
        if (borrowedItem.reqId == idToReturn) {
            found = true;
			returnRecord <<borrowedItem.itemId<<" " << borrowedItem.itemName <<" "
                        << borrowedItem.itemQuan << " "<< borrowedItem.borrowerId <<" "<< borrowedItem.borrowerName <<" "
                        << borrowedItem.borrowDate<<" " << dateReturn<<endl;
                        
            while (itemFile >> item.id >> item.name >> item.quantity) {
                if (item.id == borrowedItem.itemId) {
                    item.quantity += borrowedItem.itemQuan;
                    tempItemFile << item.id << " " << item.name << " " << item.quantity << endl;
                } else {
                    tempItemFile << item.id << " " << item.name << " " << item.quantity << endl;
                }
            }

            continue;
        } else {
           
            tempBorrowedFile << borrowedItem.reqId << " " << borrowedItem.itemId << " " 
                             << borrowedItem.itemName << " " << borrowedItem.itemQuan << " "
                             << borrowedItem.borrowerId << " " << borrowedItem.borrowerName << " "
                             << borrowedItem.borrowDate << endl;
        }
    }
    borrowedFile.close();
    itemFile.close();
    tempItemFile.close();
    tempBorrowedFile.close();

    if (found) {
        remove("borrowedItem.txt");
        remove("item.txt");
        rename("tempItem.txt", "item.txt");
        rename("tempBorrowedItem.txt", "borrowedItem.txt");

        cout << "Item with ID " << idToReturn << " has been returned and stock updated.\n";
    } else {
        remove("tempItem.txt");
        remove("tempBorrowedItem.txt");
        cout << "Item with ID " << idToReturn << " not found in borrowed list.\n";
    }
}

//approbing
void approveRequest(int idToBorrow) {
    clearScreen();
    string borrowDate = getCurrentDate();
    int toDeduct;

    ifstream inFile("requestItem.txt");
    ifstream inFile2("item.txt");
    if (!inFile || !inFile2) {
        cerr << "Error: Unable to open requestItem.txt or item.txt for reading.\n";
        return;
    }

    ofstream tempFile("temp.txt");
    ofstream tempItemFile("tempItem.txt");
    ofstream borrowedFile("borrowedItem.txt", ios::app);

    if (!tempFile || !tempItemFile || !borrowedFile) {
        cerr << "Error: Unable to open temp or borrowedItem.txt for writing.\n";
        return;
    }

    Item item;
    RequestItem req;
    bool found = false;
    bool insufficientStock = false;

    
    while (inFile >> req.reqId >> req.itemId >> req.itemName >> req.itemQuan >> req.borrowerId >> req.borrowerName >> req.date) {
        if (req.reqId == idToBorrow) {
            found = true;

            vector<Item> items;

            while (inFile2 >> item.id >> item.name >> item.quantity) {
                if (item.id == req.itemId) {
                    if (req.itemQuan > item.quantity) {
                        cout << "Insufficient stocks.\n";
                        insufficientStock = true;
                    } else {
                        toDeduct = item.quantity - req.itemQuan;
                        items.push_back({item.id, item.name, toDeduct});
                    }
                } else {
                    items.push_back(item);
                }
            }

            if (insufficientStock) {
                break;
            }

            
            for (const auto& it : items) {
                tempItemFile << it.id << " " << it.name << " " << it.quantity << endl;
            }

           
            borrowedFile << req.reqId << " " << req.itemId << " " << req.itemName << " " << req.itemQuan << " "
                         << req.borrowerId << " " << req.borrowerName << " " << borrowDate << endl;

        } else {
            
            tempFile << req.reqId << " " << req.itemId << " " << req.itemName << " " << req.itemQuan << " "
                     << req.borrowerId << " " << req.borrowerName << " " << req.date << endl;
        }
    }

    inFile.close();
    inFile2.close();
    tempFile.close();
    tempItemFile.close();
    borrowedFile.close();

    if (found && !insufficientStock) {
        remove("requestItem.txt");
        remove("item.txt");
        rename("temp.txt", "requestItem.txt");
        rename("tempItem.txt", "item.txt");
        cout << "Item with request ID " << idToBorrow << " has been borrowed and moved to borrowedItem.txt.\n";
    } else if (found && insufficientStock) {
        remove("temp.txt");
        remove("tempItem.txt");
        cout << "Borrowing failed due to insufficient stocks.\n";
    } else {
        remove("temp.txt");
        cout << "Request ID " << idToBorrow << " not found.\n";
    }
}

void cancelItems(int idToCancel) {
    clearScreen();
	string cancelDate = getCurrentDate();
	
    ifstream inFile("requestItem.txt");
    if (!inFile) {
        cerr << "Error: Unable to open requestItem.txt for reading.\n";
        return;
    }

    ofstream tempFile("temp.txt", ios::app);
    ofstream outFile("cancelItem.txt", ios::app);

    if (!tempFile || !outFile) {
        cerr << "Error: Unable to open file(s) for writing.\n";
        inFile.close();
        return;
    }

    RequestItem req;
    bool found = false;

    while (inFile >>req.reqId>>req.itemId >> req.itemName >> req.itemQuan >>  req.borrowerId >>req.borrowerName >>req.date) {
    if (req.reqId == idToCancel) { 
        outFile << req.reqId<< " "<<req.itemId << " " << req.itemName << " " << req.itemQuan << " "
                << req.borrowerId << " "<<req.borrowerName<<" " << cancelDate << endl;
        found = true;
    } else {
        tempFile<< req.reqId<<" "<<req.itemId << " " << req.itemName << " " << req.itemQuan << " "<<
                  req.borrowerId <<" "<<req.borrowerName<< " " << req.date << endl;
    }
}

    inFile.close();
    outFile.close();
    tempFile.close();

    if (found) {
        remove("requestItem.txt");
        rename("temp.txt", "requestItem.txt");
        cout << "Item with ID " << idToCancel << " has been canceled and removed.\n";
    } else {
        remove("temp.txt"); 
        cout << "Item with ID " << idToCancel << " not found.\n";
    }
}


void readUsers() {
    clearScreen();
        ifstream file("data.txt"); 
        User user;
        if (file.is_open()) {
            while (file >> user.id >> user.name >> user.username >> user.password >> user.dept>> user.userAccess) {
            		cout << "ID: " << user.id << ", Name: " << user.name << ", Username: " << user.username << ", Password: " 
				<< user.password << ", dept: " << user.dept << ", Access: "<< user.userAccess<< endl;   
            }
           file.close();
        }
        cout <<"--------------------------------"<< endl;
    } 
//changing password
void changePass(int id, string confirmedPass){
	clearScreen();
        ifstream file("data.txt"); 
        ofstream temp("tempData.txt",ios::app );
        User user;
        if (file.is_open()) {
            while (file >> user.id >> user.name >> user.username >> user.password >> user.dept>> user.userAccess) {
            	if(id == user.id){
            		temp <<user.id<<" "<< user.name<<" "<< user.username<<" "<< confirmedPass<<" "<< user.dept<<" "<<user.userAccess<<endl;
				}else{
					temp <<user.id<<" "<< user.name<<" "<< user.username<<" "<< user.password<<" "<< user.dept<<" "<<user.userAccess<<endl;
				}
            }
            
           file.close();
           temp.close();
        }
        
        remove("data.txt");
        rename("tempData.txt","data.txt");
        cout << "Password Successfully Changed\n";
}
        
// show returned Itemsss
void readReturnItems() {
    clearScreen();
        ifstream file("returnRecord.txt"); 
        ReturnedItem returned;
        if (file.is_open()) {
            while (file >> returned.itemId >> returned.itemName >> returned.itemQuan >> 
			returned.borrowerId >> returned.borrowerName>> returned.borrowDate>>returned.dateReturn) {
            		cout << "ID: " <<  returned.itemId << 
					", Item Name: " << returned.itemName << 
					", Quantity: " << returned.itemQuan << 
					", Name: " << returned.borrowerName << 
					", Borrower ID: " << returned.borrowerId << 
					", Date of borrow: " << returned.borrowDate << 
					", Return Date: "<< returned.dateReturn<< endl;   
            }
           file.close();
        }
        cout <<"--------------------------------"<< endl;
} 

 //return all items
void readItems() {
	clearScreen();
    ifstream file("item.txt");
    Item item;
    if (file.is_open()) {
        while (file >> item.id >> item.name>> item.quantity) {
            cout << "ID: " << item.id << ", Name: " << item.name << " Quantity: " << item.quantity << endl;

        }
        file.close();
    }
    cout <<"--------------------------------"<< endl;
}
//user's all requests only
void readMyRequest(int id) {
    clearScreen();
    cout << "Pending Request" << endl;
    
    ifstream file("requestItem.txt");
    RequestItem req;
    
    if (file.is_open()) {
        while (file >> req.reqId>> req.itemId >> req.itemName >> req.itemQuan 
                     >> req.borrowerId >> req.borrowerName>> req.date) {
            if (id == req.borrowerId) {
                cout << "Req. ID: " << req.reqId
					 << ", Item ID: " << req.itemId 
                     << ", Item Name: " << req.itemName 
                     << ", Quantity: " << req.itemQuan 
                     << ", Date: " << req.date << endl;
            }
        }
        file.close(); 
    } else {
        cout << "Failed to open requestItem.txt" << endl;
    }
    
    cout << "--------------------------------" << endl;
}
void readMyBorrow(int id) {
    clearScreen();
    cout << "All Borrowed Item" << endl;
    
    ifstream file("borrowedItem.txt");
    RequestItem req;
    
    if (file.is_open()) {
        while (file >> req.reqId>>req.itemId >> req.itemName >> req.itemQuan 
                     >> req.borrowerId >> req.borrowerName>> req.date) {
            if (id == req.borrowerId) {
                cout << "Item ID: " << req.itemId 
                     << ", Item Name: " << req.itemName 
                     << ", Quantity: " << req.itemQuan 
                     << ", Date: " << req.date << endl;
            }
        }
        file.close(); 
    } else {
        cout << "Failed to open requestItem.txt" << endl;
    }
    
    cout << "--------------------------------" << endl;
}
//admin side
void readAllUserRequest(){
	clearScreen();
	cout << "All User Request"<< endl;
	ifstream file("requestItem.txt");
	int num = 0;
	RequestItem req;
    if (file.is_open()) {
        while (file >>req.reqId >>req.itemId >> req.itemName>> req.itemQuan>> req.borrowerId >>req.borrowerName>> req.date) {
        	num++;
            cout <<  "Req. ID# "<<req.reqId<<", Item ID: " << req.itemId << ", Item Name: " << req.itemName << " Quantity: " 
			<< req.itemQuan << ", Borrower's Name: "<<req.borrowerName<< ", Borrower's ID: "<< req.borrowerId<< ", Date: "<< req.date << endl;
		}
	}cout <<"--------------------------------"<< endl;
}
void readAllBorrowedItem(){
	clearScreen();
	cout << "All Borrowed Item"<< endl;
	ifstream file("borrowedItem.txt");
	int num = 0;
	RequestItem req;
    if (file.is_open()) {
        while (file >>req.reqId>> req.itemId >> req.itemName>> req.itemQuan>> req.borrowerId >>req.borrowerName>> req.date) {
        	num++;
            cout <<  "Req. ID# "<<req.reqId<<", Item ID: " << req.itemId << ", Item Name: " << req.itemName << ", Quantity: " 
			<< req.itemQuan << ", Borrower's Name: "<<req.borrowerName<< ", Borrower's ID: "<< req.borrowerId<< ", Date: "<< req.date << endl;
		}
	}cout <<"--------------------------------"<< endl;
}
//user's all cancelled items only
void readMyCancelled(int id) {
    clearScreen();
    cout << "Cancel Request" << endl;
    
    ifstream file("cancelItem.txt");
    RequestItem cancelled;

    if (file.is_open()) {
        while (file >> cancelled.reqId >> cancelled.itemId >> cancelled.itemName>> cancelled.itemQuan>> cancelled.borrowerId >>
		cancelled.borrowerName>> cancelled.date) {
            if (id == cancelled.borrowerId) {
                cout <<  "Req. ID "<<cancelled.reqId<<", Item ID: " << cancelled.itemId << ", Item Name: " << cancelled.itemName << ", Quantity: " 
			<< cancelled.itemQuan << ", Borrower's Name: "<<cancelled.borrowerName<< ", Borrower's ID: "<< cancelled.borrowerId<< ", Date: "<< cancelled.date << endl;
            }
        }
        file.close();
    } else {
        cout << "Failed to open cancelItem.txt" << endl;
    }
    
    cout << "--------------------------------" << endl;
}

//deleting user(admin side)
void deleteUser(int idToDelete) {
    clearScreen();

    ifstream inFile("data.txt");
    if (!inFile) {
        cerr << "Error: Unable to open data.txt for reading.\n";
        return;
    }

    ofstream outFile("temp.txt");
    if (!outFile) {
        cerr << "Error: Unable to open temp.txt for writing.\n";
        inFile.close();
        return;
    }

    User user;
    bool found = false;

   	while (inFile >> user.id >> user.name >> user.username >> user.password >> user.dept >> user.userAccess) {
	    if (user.id == idToDelete) {
	        if (user.userAccess == "admin" || user.userAccess == "ADMIN" || user.userAccess == "Admin") {
	            cout << "Can't delete Admin user: " << endl;
	            return;
	        }
	        found = true; 
	    } else {
	        outFile << user.id << " " << user.name << " " << user.username << " "
	                << user.password << " " << user.dept << " " << user.userAccess << endl;
	    }
	}


    inFile.close();
    outFile.close();
    
	ifstream check("data.txt");
	check.close();
	if (check.is_open()) {
	    cout << "data.txt is still open somehow before remove().\n";
	} else {
	    cout << "data.txt appears to be closed.\n";
	}
	check.close();
	
    if (found) {
    
    if (remove("data.txt") != 0) {
        perror("Error deleting original data file");
        return;
    }
    
    if (rename("temp.txt", "data.txt") != 0) {
        perror("Error renaming temp file to data.txt");
        return;
    }
    cout << "User deleted successfully.\n";
}
	 else {
        cout << "User not found.\n";
        if (remove("temp.txt") != 0) {
            perror("Error deleting temporary file");
        }
    }
}
//deleting item(admin side)
void deleteItem(int idToDelete) {
    clearScreen();

    ifstream inFile("item.txt");
    if (!inFile) {
        cerr << "Error: Unable to open data.txt for reading.\n";
        return;
    }

    ofstream outFile("tempItem.txt");
    if (!outFile) {
        cerr << "Error: Unable to open temp.txt for writing.\n";
        inFile.close();
        return;
    }

    Item item;
    bool found = false;

    while (inFile >> item.id >> item.name >> item.quantity ) {
        if (item.id != idToDelete) {
            outFile << item.id << " " << item.name << " " << item.quantity << endl;
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();
    
	ifstream check("item.txt");
	check.close();
	if (check.is_open()) {
	    cout << "item.txt is still open somehow before remove().\n";
	} else {
	    cout << "item.txt appears to be closed.\n";
	}
	check.close();
	
    if (found) {
    if (remove("item.txt") != 0) {
        perror("Error deleting original data file");
        return;
    }
    if (rename("tempItem.txt", "item.txt") != 0) {
        perror("Error renaming temp file to data.txt");
        return;
    }
    cout << "Item deleted successfully.\n";
}
	 else {
        cout << "Item not found.\n";
        if (remove("temp.txt") != 0) {
            perror("Error deleting temporary file");
        }
    }
}


//asking for inputs
bool userExists(const string& username) {
    ifstream file("data.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open data.txt\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, name, storedUsername, password, dept, userAccess;

        getline(ss, idStr, ' ');      
        getline(ss, name, ' ');          
        getline(ss, storedUsername, ' ');
        getline(ss, password, ' ');       
        getline(ss, dept, ' ');            
        getline(ss, userAccess, ' ');     

        if (storedUsername == username) {
            cout << "Username already exists.\n";
            return true;
        }
    }

    return false;
}


void registerUser() {
    User user;
    user.id = getLastUserId() + 1;

    cout << "\n--- Register ---\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover input

    cout << "Enter first name (use dash '-' if multiple names): ";
    getline(cin, user.name);

    cout << "Enter username: ";
    getline(cin, user.username);

    if (userExists(user.username)) {
        return;
    }

    cout << "Enter password: ";
    getline(cin, user.password);

    cout << "Enter department (use dash '-' if multiple words): ";
    getline(cin, user.dept);

    cout << "Enter User Access (admin, user): ";
    getline(cin, user.userAccess);

    addUser(user);

    cout << "Registration successful!\n";
}

//Admin dashboard nakalagay naman HAHAHAH
void adminDashboard(int id,string name ,string username, string department, string userAccess,string password) {
	clearScreen();
    int action;
    int action2;
	int approveId;
    int choice = -1;  // Initializings
    int deleteId;
    int returnId;
    Item item;
	cout << "Login successful! Welcome,"<<endl;
    while (choice != 0) {
    	cout << "Name: " << name << " (" << username << ").\n";
        cout << "Department: " << department << " Access: "<< userAccess<<endl;
        cout << "\nADMIN DASHBOARD\n";
        cout << "Navigate it (1-9 | 0 - Logout)\n\n";
        cout << "1. Show all Items\n";
        cout << "2. Show all Users\n";
        cout << "3. Add Item\n";
    	cout << "4. Show Borrow Requests\n";
    	cout << "5. Show Borrowed Items\n";
    	cout << "6. Show Returned Items\n";
        cout << "7. Delete Item\n";
        cout << "8. Delete User\n";
        cout << "9. Settings\n";
        cout << "0. Log Out\n";
        cout << "Action: ";

        cin >> choice;
        switch (choice) {
            case 1:
                clearScreen();
                readItems();
                break;
            case 2:
                clearScreen();
                readUsers();
                break;
            case 3:
                clearScreen();
                item.id = getLastItemId() + 1;
                cout << "Name of Item (dash separated if multiple words): ";
                cin.ignore();
                getline(cin, item.name);
                cout << "Enter quantity: ";
                cin >> item.quantity;
                addItem(item);
                break;
            case 4:
                readAllUserRequest();
                cout << "Enter Operation\n1- Approve 0- DashBoard\nAction: ";
                    cin>> action;
                    if(action ==1){
                    	cout << "Enter Req. ID to Approve: ";
                    	cin>> approveId;
                    	approveRequest(approveId);
                    	break;
		
					}else if(action == 0){
						clearScreen();
                		break;
					}
					else{
						break;
					}
                break;
            case 5:
                readAllBorrowedItem();
                cout << "Enter Operation\n1- Return Item  0-DashBoard\nAction: ";
                cin >> action2;
                if(action2 == 1){
                	cout << "Enter Req. ID to Return: ";
                	cin >> returnId;
                	returnItem(returnId);
                	break;
                }else if(action2 == 0){
                	clearScreen();
                	break;
				}else{
					break;	
				}
            case 6:
            	readReturnItems();
            	break;
            case 7:
                clearScreen();
                readItems();
                cout << "Enter ID to delete Item: ";
                cin >> deleteId;
                deleteItem(deleteId);
                break;
            case 8:
                clearScreen();
                readUsers();
                cout << "Enter ID to delete user: ";
                cin >> deleteId;
                deleteUser(deleteId);
                break;
            case 9:
            	clearScreen();
                	cout << "1. Change password\n0. back\nAction: ";
                	cin >>action;
                	if(action == 1){
                		clearScreen();
                		cin.ignore();
                		string currentPass, newPass, confirmPass;
                		cout << "Enter current Password: ";
                		getline(cin, currentPass);
                		cout << "Enter new Password: ";
                		getline(cin,newPass);
                		cout << "Confirm Password: ";
                		getline(cin,confirmPass);
                		
                		if(currentPass == password){
							if(newPass == confirmPass){
	                			changePass(id,confirmPass);
	                			break;
							}else{
								cout << "Passwords not Match\n";
								break;
							}
						}else{
							cout<< "Password do not match in our Database\n";
						}
					}else if(action == 0){
						break;
					}else{
						break;
					}
        
            case 0:
                clearScreen();
                cout << "Logging out of admin dashboard...\n";
                break;
            default:
                cout << "Invalid input.\n";
        }
    }
}

void userDashboard(int id, string name ,string username, string department, string userAccess ,string password){
	 	
		clearScreen();
		int idToBorrow;
		int quantity;
		int choice;
		int idToCancel;
		int cancelationChoice ;
		int action;
		

		string date = getCurrentDate();
        
        do {
            cout << "Login successful! Welcome, " << name << " (" << username << ").\n";
            cout << "Department: " << department << " Access: "<< userAccess;
            cout << "\nOptions after login:\n";
            cout << "1. Show all Items available\n";
            cout << "2. Show Pending Request Item\n";
            cout << "3. Show Borrowed Item\n";
            cout << "4. Show Cancelled Items\n";
            cout << "5. Settings\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    readItems();
                    cout << "Enter ID to request: ";
                    cin >> idToBorrow;
                    cout << "Enter Quantity: ";
                    cin >> quantity;
                    requestItems(idToBorrow,quantity,name,id,date);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                case 2:
                    readMyRequest(id);
                    cout << "1 - Cancel Request , 0 to Return\nAction: ";
                    cin >> cancelationChoice;
                    
                    if(cancelationChoice == 1){
                    	cout << "Enter Req. ID to cancel: ";
                    	cin >> idToCancel;
                    	cancelItems(idToCancel);
                    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    	break;
					}else if(cancelationChoice == 2){
						clearScreen();
						return;
					}else{
						break;
					}
                    
                case 3:
                	readMyBorrow(id);
                	break;
                case 4:
                    clearScreen();
                    readMyCancelled(id);
                    break;
                case 5:
                	clearScreen();
                	cout << "1. Change password\n0. back\nAction: ";
                	cin >>action;
                	if(action == 1){
                		clearScreen();
                		cin.ignore();
                		string currentPass, newPass, confirmPass;
                		cout << "Enter current Password: ";
                		getline(cin, currentPass);
                		cout << "Enter new Password: ";
                		getline(cin,newPass);
                		cout << "Confirm Password: ";
                		getline(cin,confirmPass);
                		cin.ignore();
                		if(currentPass == password){
							if(newPass == confirmPass){
	                			changePass(id,confirmPass);
	                			break;
							}else{
								cout << "Passwords not Match\n";
								break;
							}
						}else{
							cout<< "Password do not match in our Database\n";
						}
					}else if(action == 0){
						break;
					}else{
						break;
					}
                case 0:
                    clearScreen();
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 0);
}
int main() {
	clearScreen();
    int choice;

    do {
        cout << "\n=== Supply Sync ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
            	clearScreen();
                registerUser();
                break;
            case 2:
            	clearScreen();
                loginUser();
                break;
            case 0:
                cout << "Exiting the program.\n";
                break;
            default:
            	clearScreen();
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}


