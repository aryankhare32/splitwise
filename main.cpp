#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include <sstream>


using namespace std;

class User {
private:
    string user_id;
    string name;
    string email;
    string mobile_number;

public:
    User(string id, string n, string e, string m) : user_id(id), name(n), email(e), mobile_number(m) {}

    string getUserId() const {
        return user_id;
    }

    string getName() const {
        return name;
    }
};

enum ExpenseType {
    EQUAL,
    EXACT,
    PERCENT
};

class Expense {
private:
    string paid_by;
    vector<string> participants;
    ExpenseType type;
    double total_amount;
    vector<double> shares;

public:
    Expense(string payer, vector<string> participants, ExpenseType t, double amount, vector<double> s)
        : paid_by(payer), participants(participants), type(t), total_amount(amount), shares(s) {}

    string getPaidBy() const { ///The const at the end of the function declaration indicates that this member function is a "const member function". It means that this function does not modify the state of the object it is called on. In this case, it implies that calling getUserId() will not change any of the member variables of the User object.
        return paid_by;
    }

    vector<string> getParticipants() const {
        return participants;
    }

    ExpenseType getType() const {
        return type;
    }

    double getTotalAmount() const {
        return total_amount;
    }

    vector<double> getShares() const {
        return shares;
    }
};

class ExpenseManager {
private:
    vector<User> users;
    vector<Expense> expenses;
    map<pair<string, string>, double> balances;  // (Ower, Owed) -> Amount

public:
    void addUser(User user) {
        users.push_back(user);
    }

    void addExpense(Expense expense) {
        expenses.push_back(expense);
        updateBalances(expense);
    }

    void showBalances(string user_id = "") {
        if (balances.empty()) {
            cout << "No balances" << endl;
            return;
        }

        for (const auto& balance : balances) {
            string ower = balance.first.first;
            string owed = balance.first.second;
            double amount = balance.second;

            if(amount>0)
            if (user_id.empty() || ower == user_id || owed == user_id) {
                cout << ower << " owes " << owed <<": " << fixed << setprecision(2) << amount << endl;
            }
        }
    }

private:
    void updateBalances(const Expense& expense) {   //Using a reference here (Expense&) means that the function can directly work with the original Expense object passed in, rather than making a copy of it. //Note: If this was just Expense (without the &), it would mean passing the Expense object by value, which would make a copy of it. This can be less efficient, especially for large objects.                                                
        string payer = expense.getPaidBy(); 
        vector<string> participants = expense.getParticipants();
        double total_amount = expense.getTotalAmount();
        vector<double> shares = expense.getShares();

        if (expense.getType() == EQUAL) {
            double share = total_amount / participants.size();
            for (const string& participant : participants) {
                if (participant != payer) {
                    balances[make_pair(participant, payer)] += share;
                    balances[make_pair(payer, participant)] -=share;
                }
            }
        } else if (expense.getType() == EXACT) {
            for (size_t i = 0; i < participants.size(); i++) { // using for loop because we need to traverse both shares and pariticipants vector
                if (participants[i] != payer) {
                    balances[make_pair(participants[i], payer)] += shares[i];
                    balances[make_pair(payer, participants[i])] =-shares[i];
                }
            }
        } else if (expense.getType() == PERCENT) {
            for (size_t i = 0; i < participants.size(); i++) {
                if (participants[i] != payer) {
                    double share = (shares[i] / 100.0) * total_amount;
                    balances[make_pair(participants[i], payer)] += share;
                    balances[make_pair(payer, participants[i])] -= share;
                }
            }
        }
    } 
    //settle balances
};

int main() {
    ExpenseManager manager;

    // Adding users
    manager.addUser(User("u1", "User1", "user1@example.com", "1234567890"));
    manager.addUser(User("u2", "User2", "user2@example.com", "2345678901"));
    manager.addUser(User("u3", "User3", "user3@example.com", "3456789012"));
    manager.addUser(User("u4", "User4", "user4@example.com", "4567890123"));
    cout << "Please enter the inputs:" << endl;
    // Adding expenses
    while(true){
        string s;
        getline(cin, s);
        istringstream iss(s);
        vector<string> inputs;

        string input;
        while (iss >> input) {
            inputs.push_back(input);
        }

        if(inputs[0]== "SHOW"){ // show method inputs
            if(inputs.size()==1){
                manager.showBalances();
            }else{
                manager.showBalances(inputs[1]);
            }
        }else if(inputs[0]== "EXPENSE"){ // taking expense method inputs
            vector<string> participants_array;
            vector<double> shares;
            string payer = inputs[1];
            double amount = stod(inputs[2]);
            int participants = stoi(inputs[3]);
            // int i;
            ExpenseType e_type;
            for(int i = 1;i <= participants; i++){
                participants_array.push_back(inputs[3+i]);
            }
            if(inputs[3+participants+1] == "EQUAL"){
                e_type = EQUAL;
                for( int x = 0; x<participants; x++){
                    shares.push_back(double(1)/participants);
                }
            }else if(inputs[3+participants+1] == "PERCENT"){
                e_type = PERCENT;
                for( int x = 5+participants; x<inputs.size(); x++){
                    shares.push_back(stoi(inputs[x]));
                }
            }else if(inputs[3+participants+1] == "EXACT"){
                e_type = EXACT;
                for( int x = 5+participants; x<inputs.size(); x++){
                    shares.push_back(stoi(inputs[x]));
                }
            }
            manager.addExpense(Expense(payer, participants_array, e_type, amount, shares));
        }else if(inputs[0] == "EXIT"){
            return 0;
        }else{
            return 0;
        }
    }
    return 0;
}
