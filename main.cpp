// main.cpp
#include <iostream>
#include <string>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define byte win_byte_override
#include <windows.h>
#undef byte

#include <sstream>
#include <cctype>
#include <algorithm>
#include <limits>
#include <regex>
#include <cmath>
#include <fstream>
#include <cstdio>  
#include <ctime>
#include <iomanip>
#include <vector>

// Headers
#include "uix.h"
#include "Color.h"
#include "Config.h"
#include "Keybinds.h"
#include "admin_dashboard.h"

using namespace Color;
using namespace Config;
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
void open_settings();

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

void approveRequest(int requestIdToApprove);
void cancelItems(int idToCancel);
void render_color();

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

    system("cls");

    // UI
    char symbol = '_';

    // DISPLAY PAGE
    space();
    print_gradient(line_str(symbol), 89, 93);
    space(4);
    
    // prompt
    print("S I G N  I N", 0);
    space(3);
    print_input_box(20, 0, Config::color_theme, "username", false, "left");
    space(2);
    print_input_box(20, 0, Config::color_theme, "password", false, "left");

    space(3);
    print_gradient(line_str(symbol), 89, 93);
    space(2);
    cout.flush();

    string ext_color = Color::gray;
    cout << ext_color << "\nExit: \e" << "\e[0m";
    for(string key : exit_keybinds()){
        cout << ext_color << key <<  " \e[0m";
    }
    cout << "\r";

    // USER INPUT
    set_cursor(31, 8);
    
    // ask user for username
    string username;
    cin >> username;

    set_cursor(31, 12);

    if(back_key(username) || exit_key(username)){
        return;
    }

    string password;
    cin >> password;
	cin.ignore();

    if(back_key(password) || exit_key(password)){
        return;
    }

    ifstream file("./data.txt");
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
    	if(convert_case(user.userAccess, "lower") == "admin"){
            space(2);
            print("      Account Verified      ", 0, { Color::bg_light_green, Color::black});
            Sleep(3000);
            system("cls");
    		adminDashboard(user.id, user.name ,user.username ,user.dept , user.userAccess, user.password);
		}
        else if(convert_case(user.userAccess, "lower") == "user"){
            space(2);
            print("      Account Verified      ", 0, { Color::bg_light_green, Color::black });
            Sleep(3000);
            system("cls");
			userDashboard(user.id ,user.name ,user.username ,user.dept , user.userAccess,user.password);
		}
        else{
            print("     User dont have access     ", 0, { Color::bg_red, Color::white });
			return;
		}
		
    } 
    else {
        space(2);
        print("     Login Unsuccessful     ", 0, { Color::bg_red, Color::white });
        Sleep(2000);
        loginUser();
        return;
    }
}

//User adder
void addUser(const User& user) {
    ofstream file("data.txt", ios::app);
    if (file.is_open()) {
        file << user.id << " " << user.name << " " << user.username << " " << user.password << " " << user.dept << " "
		<<user.userAccess<< endl;

        file.close();
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
    cout << " <<" << string(term_width * 0.40, ' ') << " All Items " << string(term_width * 0.38, ' ') << " >>\n";

    vector<Item> items;
    ifstream file("item.txt");
    Item item;

    // Read all items into vector
    while (file >> item.id >> item.name >> item.quantity) {
        items.push_back(item);
    }
    file.close();

    const int ITEMS_PER_PAGE = 7;
    int currentPage = 0;
    int totalPages = (items.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    string command;

    do {
        clearScreen();
        cout << " <<" << string(term_width * 0.40, ' ') <<" All Items " << string(term_width * 0.38, ' ') << " >>\n";
        print_gradient(line_str('~'), 35, 40);

        // Display items for current page
        int start = currentPage * ITEMS_PER_PAGE;
        int end = min(start + ITEMS_PER_PAGE, (int)items.size());

        for (int i = start; i < end; i++) {
            space();
            cout << "\t  ";
            cout << left << setw(20) << ("ID: " + to_string(items[i].id)) 
                 << setw(30) << ("Name: " + items[i].name)
                 << setw(20) << ("Quantity: " + to_string(items[i].quantity));
            space();
            print_gradient(line_str('~'), 35, 40);
        }

        
        space();
        cout << Color::gray << "Page " << (currentPage + 1) << " of " << totalPages << Color::reset;
        space();
        print_input_box(20, 0, {Config::color_theme}, "command", false, "left");

        cin >> command;

        // Handle navigation
        if (right_key(command)) {
            if (currentPage < totalPages - 1) currentPage++;
        }
        else if (db_right_key(command)) {
            currentPage = min(currentPage + 2, totalPages - 1);
        }
        else if (tri_right_key(command)) {
            currentPage = totalPages - 1;
        }
        else if (left_key(command)) {
            if (currentPage > 0) currentPage--;
        }
        else if (db_left_key(command)) {
            currentPage = max(currentPage - 2, 0);
        }
        else if (tri_left_key(command)) {
            currentPage = 0;
        }
    }
    while (!exit_key(command) && !back_key(command));
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
            return true;
        }
    }

    return false;
}

void registerUser() {
    system("cls");

    User user;
    user.id = getLastUserId() + 1;

    // UI Configuration
    char symbol = '_';
    int delay = 2500;
    int gradient[] = {89, 93};
    string input_box_pos = "left";

    do{
       
        // DISPLAY PAGE
        print_gradient(line_str(symbol), gradient[0], gradient[1]);
        space(3);
        
        // PROMPT
        print("S I G N  U P");
        space(2);
        print_input_box(20, 0, Config::color_theme, "firstname", false, input_box_pos);
        space(2);
        print_input_box(20, 0, Config::color_theme, "username", false, input_box_pos);
        space(2);
        print_input_box(20, 0, Config::color_theme, "password", false, input_box_pos);

        space(2);
        print_gradient(line_str(symbol), gradient[0], gradient[1]);
        cout.flush();

        // Exit Controls
        string control_color = Color::gray;

        cout << control_color << "\nExit: \e" << "\e[0m";
        for(string key : exit_keybinds()){
            cout << control_color << key <<  " \e[0m";
        }
        cout << "\r";

        // Back Controls
        cout << control_color << "\nReset: \e" << "\e[0m";
        for(string key : back_keybinds()){
            cout << control_color << key <<  " \e[0m";
        }
        cout << "\r";

        
        // FIRST NAME
        set_cursor(31, 6);
        std::cin.clear();
        if (cin.peek() == '\n') cin.ignore();
        getline(cin, user.name);
        if(exit_key(user.name)) return;
        if(back_key(user.name)){
            system("cls");
            continue;
        }
        if (user.name.find(' ') != string::npos) {
            set_cursor(0, 19);
            print("  Please use '-' for multiple names  ", 0, {Color::bg_red});
            Sleep(delay);
            system("cls");
            continue;
        }
        cout.flush();

        // USERNAME
        set_cursor(31, 10);
        getline(cin, user.username);
        if(exit_key(user.username)) return;
        if(back_key(user.username)){
            system("cls");
            continue;
        }
        if (user.username.find(' ') != string::npos) {
            set_cursor(0, 19);
            print("  Please use '-' for multiple names  ", 0, {Color::bg_red});
            Sleep(delay);
            system("cls");
            continue;
        }
        if (userExists(user.username)) {
            set_cursor(0, 19);
            print("  Username already exist  ", 0, {Color::bg_red});
            Sleep(delay);
            system("cls");
            continue;
        }
        cout.flush();


        set_cursor(31, 14);
        getline(cin, user.password);
        if(exit_key(user.password)) return;
        if(back_key(user.password)){
            system("cls");
            continue;
        }
        if (user.password.find(' ') != string::npos) {
            set_cursor(0, 19);
            print("  Cannot add spaces to your password (use '-')  ", 0, {Color::bg_red});
            Sleep(delay);
            system("cls");
            continue;
        }

        system("cls");

        // DISPLAY PAGE 2
        print_gradient(line_str(symbol), gradient[0], gradient[1]);
        space(4);
        
        // PROMPT
        print("U S E R - T Y P E");
        space(3);
        print_input_box(20, 0, Config::color_theme, "department", false, input_box_pos);
        space(2);
        print_input_box(20, 0, Config::color_theme, "access-control", false, input_box_pos);
        space(4);
        print_gradient(line_str(symbol), gradient[0], gradient[1]);

        // Exit Controls
        cout << control_color << "\nExit: \e" << "\e[0m";
        for(string key : exit_keybinds()){
            cout << control_color << key <<  " \e[0m";
        }
        cout << "\r";
        
        // Back Controls
        cout << control_color << "\nReset: \e" << "\e[0m";
        for(string key : back_keybinds()){
            cout << control_color << key <<  " \e[0m";
        }
        cout << "\r";

        cout.flush();

        set_cursor(31, 8);
        getline(cin, user.dept);
        if(exit_key(user.dept)) return;
        if(back_key(user.dept)){
            system("cls");
            continue;
        }
        if (user.dept.find(' ') != string::npos) {
            set_cursor(0, 19);
            print("  Cannot add spaces to your Department (use '-')  ", 0, {Color::bg_red});
            Sleep(delay);
            system("cls");
            continue;
        }

        set_cursor(31, 12);
        getline(cin, user.userAccess);
        if(exit_key(user.userAccess)) return;
        if(back_key(user.userAccess)){
            system("cls");
            continue;
        }
        string access = convert_case(user.userAccess, "lower");
        if(access != "admin" && access != "user") {
            set_cursor(0, 19);
            print("  Access control must be 'admin' or 'user'  ", 0, {Color::bg_red});
            Sleep(delay);
            system("cls"); 
            continue;
        }

        // Commit Transaction
        addUser(user);

        space(2);
        print("      Registration successful!      ", 0, { Color::bg_blue });
        Sleep(3000);
        system("cls");
        return;
    }
    while(true);
}

//Admin dashboard nakalagay naman HAHAHAH
void adminDashboard(int id,string name ,string username, string department, string userAccess,string password) {
	clearScreen();
    int action;
    int action2;
	int approveId;
    string choice;  // Initializings
    int deleteId;
    int returnId;
    Item item;

    do{
        print_gradient(line_str('='), 203, 207);

        print_triple_text("Name: " + name + "(" + username + ")", "Access: " + userAccess, "Department: " + department, 2);
 
        print_gradient(line_str('='), 203, 207);
        
        space(2);
        print_gradient("ADMIN DASHBOARD", 221, 223);
        space(2);

        print_triple_input_box("Show Items", "Show Users", "Add Item", 20, 3, 1, Color::light_yellow);
        space();
        print_triple_input_box("Borrow Request", "Borrowed Items", "Returned Items", 20, 3, 1, Color::light_yellow);
        space();
        print_triple_input_box("Delete Item", "Delete User", "Account Settings", 20, 3, 1, Color::light_yellow);
        space(2);
        print_gradient(line_str('='), 203, 207);
        space(1);
        
        print_input_box(20, 0, Config::color_theme, "command", false);
        set_cursor(31, 18);

        /* 

            TODO: Logout function will be moved to the account settings as there is no more space here
            TODO: Some functions need its own page, a command input for user so the page stays (all other similar ones too)
            TODO: A UI for displaying datas from the database

        */

        getline(cin, choice);

        if(show_items_key(choice)) {
            clearScreen();
            readItems();
            continue;
        }

        else if(show_users_key(choice)) {
            clearScreen(); 
            readUsers();
            continue;
        }

        else if(add_item_key(choice)) {
            clearScreen();
            item.id = getLastItemId() + 1;
            cout << "Name of Item (dash separated if multiple words): ";
            cin.ignore();
            getline(cin, item.name);
            cout << "Enter quantity: ";
            cin >> item.quantity;
            addItem(item);
            continue;
        }


        else if(borrow_request_key(choice)) {

            readAllUserRequest();
            cout << "Enter Operation\n1- Approve 0- DashBoard\nAction: ";

            cin>> action;
            if(action ==1){
                cout << "Enter Req. ID to Approve: ";
                cin>> approveId;
                approveRequest(approveId);
            }
            else if(action == 0){
                clearScreen();
            }

            continue;
        } 

        else if(borrowed_items_key(choice)) {

            readAllBorrowedItem();
            cout << "Enter Operation\n1- Return Item  0-DashBoard\nAction: ";
            cin >> action2;

            if(action2 == 1){
                cout << "Enter Req. ID to Return: ";
                cin >> returnId;
                returnItem(returnId);
            }

            else if(action2 == 0){
                clearScreen();
            }

            continue;

        }

        else if(returned_items_key(choice)) {

            readReturnItems();
            continue;
        }

        else if(delete_items_key(choice)) {

            clearScreen();
            readItems();
            cout << "Enter ID to delete Item: ";
            cin >> deleteId;
            deleteItem(deleteId);
            continue;

        }

        else if(delete_user_key(choice)) {

            clearScreen();
            readUsers();
            cout << "Enter ID to delete user: ";
            cin >> deleteId;
            deleteUser(deleteId);
            continue;

        }

        else if(account_settings_key(choice)) {

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
                        continue;
                    }

                    else{
                        cout << "Passwords not Match\n";
                        continue;
                    }

                }

                else{
                    cout<< "Password do not match in our Database\n";
                }

            }

            else if(action == 0){
                continue;
            }

            cout << "Invalid";
        }

        else if(exit_key(choice) || back_key(choice)) {

            clearScreen();
            cout << "Logging out of admin dashboard...\n";

        }

        else {

            cout << "Invalid input.\n";
            Sleep(2500); 
            system("cls");
            continue;

        }
    }
    while(exit_key(choice) || back_key(choice));
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
    system("chcp 65001 > nul");
    set_terminal_size();
    
    clearScreen();

    adminDashboard(12, "Kc-Sean", "Sean-Brix", "IT-DEPT", "admin", "121802");
    
    string line_color = "\e[47m";
    
    string user_prompt;
    
    do{
        space(2);
        print_gradient(line_str(' '), 227, 231, true);
        print(line_title_str("SUPPLYSYNC",'='));
        print_gradient(line_str(' '), 227, 231, true);
        space(4);
        
        print("LOGIN", -24);
        print("REGISTER", -27);
        print("SETTINGS", -27);
        print("EXIT", -27);
        
        space(4);

        print_gradient(line_str(' '), 228, 231, true);
        print(line_str('='));
        print_gradient(line_str(' '), 228, 231, true);

        space(2);
        print_input_box(15, 0, Config::color_theme, "input", false);
        set_cursor(33, 17);
        cout.flush();
        
        cin >> user_prompt;
        user_prompt = convert_case(user_prompt, "lower");

        if(login_key(user_prompt)){
            // ENTER LOGIN
            loginUser();
            system("cls");
            cout.flush();
            continue;
        }
        else if(register_key(user_prompt)){
            // ENTER REGISTER
            registerUser();
            system("cls");
            cout.flush();
            continue;
        }
        else if(setting_key(user_prompt)){
            // ENTER SETTINGS
            open_settings();
            system("cls");
            cout.flush();
            continue;
        }
        else if(exit_key(user_prompt)){
            space(1);
            char exit_line = '~';
            string exit_color = Color::light_red;

            set_cursor(0, 3);
            line_title("SHUTTING DOWN", exit_line, exit_color, Config::color_theme);
            
            set_cursor(0, 13);
            print_line(exit_line, exit_color);
            
            set_cursor(37, 17);
            Sleep(3000);
            system("cls");
            exit(0);         
        }
        else{
            // INVALID
            space(2);
            print_error("Invalid Input");
            Sleep(1000);
            system("cls");
            cout.flush();

            continue;
        }
        
    }
    while(!exit_key(user_prompt));

    return 0;
}

// Split function definition 
vector<string> split(const string &s, char delimiter){ 
    vector<string> tokens; string token; 
    stringstream ss(s); 
    while (getline(ss, token, delimiter)) { 
        tokens.push_back(token); 
    } 
    return tokens;
}

// SETTINGS PAGE
void open_settings() {
    system("cls");
    cin.ignore();
    
    while(true) {
        // Display header
        space();
        print_gradient(line_str('='), 35, 40);
        space(2);
        print("SETTINGS", 0, {"\e[1;36m"});
        space(2);
        
        // Display current keybinds
        print("CURRENT KEYBINDS:", 1, {"\e[0;33m"});
        space(2);
        
        print("Exit: ", -5, {"\e[1;36m"});
        for(string key : exit_keybinds()) cout << key << " ";
        space();
        
        print("Back: ", -5, {"\e[1;36m"});
        for(string key : back_keybinds()) cout << key << " ";
        space();
        
        print("Right Page: ", -8, {"\e[1;36m"});
        for(string key : right_keybinds()) cout << key << " ";
        space();
        
        print("Left Page: ", -7, {"\e[1;36m"});
        for(string key : left_keybinds()) cout << key << " ";
        space();
        
        print("Double Right: ", -9, {"\e[1;36m"});
        for(string key : db_right_keybinds()) cout << key << " ";
        space();
        
        print("Double Left: ", -8, {"\e[1;36m"});
        for(string key : db_left_keybinds()) cout << key << " "; 
        space(2);
        
        // Display commands
        print_gradient(line_str('-'), 35, 40);
        space();
        
        space();
        print("'add'/'remove' + 'category' + 'key'", 0, {Color::gray});
        space(2);

        // Input box
        print_input_box(30, 0, Config::color_theme, "command", false);
        set_cursor(26, 18);
        
        string command;
        getline(cin, command);

        if(exit_key(command)) return;
        if(back_key(command)) return;
        
        vector<string> cmds = split(command, ' ');
        if(cmds.size() < 3) {
            set_cursor(0, 15);
            print("                  Invalid Command                  ", 0, {Color::bg_red, Color::white});
            Sleep(2000);
            system("cls");
            continue;
        }

        string action = convert_case(cmds[0], "lower");
        string category = convert_case(cmds[1], "lower"); 
        string key = cmds[2];

        if(action == "add") {
            if(category == "exit") exit_add(key);
            else if(category == "back") back_add(key);
            else if(category == "right") right_add(key);
            else if(category == "left") left_add(key);
            else if(category == "db_right") db_right_add(key);
            else if(category == "db_left") db_left_add(key);
            else {

                set_cursor(0, 15);
                print("                  Invalid category                  ", 0, {Color::bg_red, Color::white});
                Sleep(2000);
                system("cls");
                continue;
            }

            set_cursor(0, 15);
            print("               Key Added Successfully               ", 0, {Color::bg_light_blue, Color::black});
        }
        else if(action == "remove") {
            if(category == "exit") exit_remove(key);
            else if(category == "back") back_remove(key); 
            else if(category == "right") right_remove(key);
            else if(category == "left") left_remove(key);
            else if(category == "db_right") db_right_remove(key);
            else if(category == "db_left") db_left_remove(key);
            else {

                set_cursor(0, 15);
                print("                  Invalid category                  ", 0, {Color::bg_red, Color::white});
                Sleep(2000);
                system("cls");
                continue;
            }
            set_cursor(0, 15);
            print("              Key Removed Successfully              ", 0, {Color::bg_light_blue, Color::black});
        }
        
        Sleep(2000);
        system("cls");
    }
}