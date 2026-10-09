/*
email_priotitizer.cpp


This program takes a file as input and reads it line by line. It is designed to read
email headers from the file and sort them in order of importance using a max heap.
The file will also contain commands like NEXT which will list the info of the next
email on the heap, READ which will remove the top email from the heap, and COUNT which
will tell the user how many emails they have left to read. Once the program reaches
the end of the file, the program will end.


Inputs:
    main: 
        A text file to be read from

    Class Email:
        getCategoryPriority(): none
        getFormattedDate(): none
        Email(): none
        Email(string cat, string subj, string d):
            3 strings, a sender category, a subject, and a date
        getSenderCategory(): none
        getSubject(): none
        getDate(): none
        getSenderRank(): none
        getDateKey(): none
        operator<(const Email& other):
            Another Email object to be compared against
        operator>(const Email& other):
            Another Email object to be compared against

    Class MaxHeap:
        heapifyUp(int index):
            An index in the heap
        heapifyDown(int index):
            An index in the heap
        MaxHeap(): none
        insert(const Email& email):
            An Email object to add to the heap
        removeMax(): none
        peekMax(): none
        isEmpty(): none
        size(): none

    Class PriorityQueue:
        enqueue(const Email& email):
            An Email object to be added to the priority queue
        dequeue(): none
        peek(): none
        isEmpty(): none
        size(): none

    processCommands(const string& filename):
        A filepath to read commands from


Output:
    Output comes from the NEXT and COUNT commands.
    Example output:

        Next email:
            Sender: Boss
            Subject: Quarterly review prep
            Date: 01-09-2025

        There are 9 emails to read.


No collaborators


Author: Jacob Spry
*/

//Necessary libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//Used to reduce clutter by removing "::std"
using namespace std;

//Class to represent an Email object
class Email {
private:
    //Private Email variables
    string senderCategory; //The sender category (Boss, Subordinate)
    string subject; //Subject line
    string date; // Format: MM-DD-YYYY
    int senderRank; //The rank of the sender
    int dateKey; //Int version of date (MM-DD-YYYY -> int YYYYMMDD)

    //Helper to return numerical category priority (higher number = higher priority)
    int getCategoryPriority() const {
        if (senderCategory == "Boss") return 5;
        if (senderCategory == "Subordinate") return 4;
        if (senderCategory == "Peer") return 3;
        if (senderCategory == "ImportantPerson") return 2;
        if (senderCategory == "OtherPerson") return 1;
        return 0;
    }

    //Converts MM-DD-YYYY to YYYYMMDD string for simple chronological comparison
    int getFormattedDate() const {
        if (date.length() == 10) {
            string month = date.substr(0, 2);
            string day = date.substr(3, 2);
            string year = date.substr(6, 4);
            return stoi(year + month + day);
        }
        return 0;
    }

public:
    Email() : senderRank(0), dateKey(0) {} //Default Email constructor
    //Normal Email constructor, assigns all private Email variables
    Email(string cat, string subj, string d)
        : senderCategory(move(cat)), subject(move(subj)), date(move(d)), 
            senderRank(getCategoryPriority()), dateKey(getFormattedDate()) {}

    //Public facing getter functions for pivate Email variables
    string getSenderCategory() const {return senderCategory;} //Gets senderCategory
    string getSubject() const {return subject;} //Gets subject
    string getDate() const {return date;} //Gets date
    int getSenderRank() const {return senderRank;} //Gets senderRank
    int getDateKey() const {return dateKey;} //Gets dateKey

    //Comparison operator for MaxHeap ordering
    //Priority rule:
    //1. Sender Category priority (Boss > Subordinate > Peer > ImportantPerson > OtherPerson)
    //2. Date priority (Newest first)
    bool operator<(const Email& other) const {
        //Stores the other Email's senderRank
        int otherSenderRank = other.getSenderRank();

        //Makes sure the ranks are different
        if (senderRank != otherSenderRank) {
            //Returns a bool based on which rank is larger
            return senderRank < otherSenderRank;
        }

        //Stores the other Email's dateKey
        int otherDateKey = other.getDateKey();
        // If categories are equal, newer date takes higher priority
        return dateKey < otherDateKey;
    }

    //Opposite comparison from the above one
    bool operator>(const Email& other) const {
        //Returns the result using the < operation
        //Works because A > B = B < A
        return other < *this;
    }
};

//Custom MaxHeap implementation using a dynamic list (std::vector)
class MaxHeap {
private:
    vector<Email> heap; //Vector to represent the heap

    //Heapifies up from a starting index
    void heapifyUp(int index) {
        //Goes as long as it aren't at the top index
        while (index > 0) {
            int parentIndex = (index - 1) / 2; //Parent index of the current index
            //Checks to see if the parent is less than the child
            if (heap[parentIndex] < heap[index]) {
                //Swaps them and updates the index to the parent if true
                swap(heap[index], heap[parentIndex]);
                index = parentIndex;
            } else {
                //Ends the loop otherwise
                break;
            }
        }
    }

    //Heapifies down from a starting index
    void heapifyDown(int index) {
        int size = static_cast<int>(heap.size()); //Size of the heap
        while (true) {
            int leftChild = 2 * index + 1; //Calculates the left child
            int rightChild = 2 * index + 2; //Calculates the right child
            int largest = index; //Keeps track of the largest value's index

            //Checks to see if the left child is a valid index and
            //If it is greater than the current largest
            if (leftChild < size && heap[largest] < heap[leftChild]) {
                //Sets largest to the left child if true
                largest = leftChild;
            }

            //Checks to see if the right child is a valid index and
            //If it is greater than the current largest
            if (rightChild < size && heap[largest] < heap[rightChild]) {
                //Sets largest to the right child if true
                largest = rightChild;
            }

            //Checks to see iff the main index is not the same as the largest index
            if (largest != index) {
                //Swaps the main index and the largest index and
                //sets the main index to the largest index if true
                swap(heap[index], heap[largest]);
                index = largest;
            } else {
                //Ends the loop otherwise
                break;
            }
        }
    }

public:
    MaxHeap() = default; //Default constructor for the heap

    //Inserts a new Email object to the end of the heap
    void insert(const Email& email) {
        heap.push_back(email);
        //Call heapifyUp to move the new entry into the correct spot
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    //Removes the top Email from the heap
    void removeMax() {
        //Checks to see if the heap is empty
        if (heap.empty()) {
            //Throws a runtime error if true
            throw runtime_error("Heap is empty.");
        }
        //Assigns the last entry in the list to the first spot
        heap[0] = heap.back();
        heap.pop_back(); //Removes the last spot in the heap
        //Checks to see if the heap is empty
        if (!heap.empty()) {
            //Heapifies down from the starting index if true
            heapifyDown(0);
        }
    }

    //Extracts the top Email in the heap without removing it
    const Email& peekMax() const {
        //Checks to see if the heap is empty
        if (heap.empty()) {
            //Throws a runtime error if true
            throw runtime_error("Heap is empty.");
        }
        return heap[0]; //Returns the top Email in the heap
    }

    //Checks to see if the heap is empty
    bool isEmpty() const {
        //Returns a boolean based on the .empty() call from the vector library
        return heap.empty();
    }

    //Checks the size of the heap
    size_t size() const {
        //Returns the size of the heap from the .size() call from the vector libary
        return heap.size();
    }
};

//Priority Queue wrapper utilizing the MaxHeap
class PriorityQueue {
private:
    MaxHeap maxHeap; //Max heap to represent the priority queue

public:
    //Adds an Email to the priority queue using the .insert() heap function
    void enqueue(const Email& email) {
        maxHeap.insert(email);
    }

    //Removes the top Email from the priority queue
    void dequeue() {
        //Checks to see if the priority queue is empty
        if (!maxHeap.isEmpty()) {
            //Removes the item using the .removeMax() heap function if true
            maxHeap.removeMax();
        }
    }

    //Returns the top Email from the priority queue without removing it
    //Does so using the .peekMax() heap function
    Email peek() const {
        return maxHeap.peekMax();
    }

    //Checks to see if the priority queue is empty
    //Does so using the .isEmpty() heap function
    bool isEmpty() const {
        return maxHeap.isEmpty();
    }

    //Returns the size of the priority queue using the .size() heap function
    size_t size() const {
        return maxHeap.size();
    }
};

//Goes through the file containing the commands line by line until it reaches
//the end of the file
void processCommands(const string& filename) {
    ifstream inputFile(filename); //Represents the input file
    //Checks to see if the file opened correctly
    if (!inputFile.is_open()) {
        //Throws an error if true
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    PriorityQueue emailQueue; //The priority queue to store and sort the emails
    string line; //Stores the current line of the file

    //Loops as long as getLine is able to fetch a line
    while (getline(inputFile, line)) {
        //Skips the rest of the current loop if the current line is empty
        if (line.empty()) continue;

        //Checks to see if the command was EMAIL
        if (line.rfind("EMAIL", 0) == 0) {
            //If true ...
            // Parse EMAIL line: "EMAIL <sender category>,<subject line>,<date>"
            string content = line.substr(6); // Skip "EMAIL "
            stringstream ss(content); //Stores the content of the email

            //Stores the category, subject, and date of the email
            string category, subject, date;
            getline(ss, category, ','); //Extracts the category
            getline(ss, subject, ','); //Extracts the subject
            getline(ss, date); //Extracts the date

            //Adds the email to the priority queue as a new Email object
            emailQueue.enqueue(Email(category, subject, date));
        
        //Checks to see if the command was COUNT
        } else if (line == "COUNT") {
            //Prints the count to the terminal if true
            cout << "There are " << emailQueue.size() << " emails to read.\n\n";
        
        //Checks to see if the command was NEXT
        } else if (line == "NEXT") {
            //If true ...
            //Checks to see if the queue is empty
            if (!emailQueue.isEmpty()) {
                //Gets the top email and prints its info if true
                Email topEmail = emailQueue.peek();
                cout << "Next email:\n";
                cout << "    Sender: " << topEmail.getSenderCategory() << "\n";
                cout << "    Subject: " << topEmail.getSubject() << "\n";
                cout << "    Date: " << topEmail.getDate() << "\n\n";
            } else {
                //Otherwise prints the below message
                cout << "No emails to read.\n\n";
            }
        
        //Checks to see if the command was READ
        } else if (line == "READ") {
            //Checks to see if the queue is empty if true
            if (!emailQueue.isEmpty()) {
                //Removes the top email from the queue if true
                emailQueue.dequeue();
            }
        }
    }

    inputFile.close(); //Closes the file once the loop is done
}

int main(int argc, char* argv[]) {
    string filename = "input.txt"; //Stores the filename
    //Checks to see if a filename was actually passed
    if (argc > 1) {
        filename = argv[1]; //Assigns it to filename if true
    }

    processCommands(filename); //Processes the command file
    return 0;
}