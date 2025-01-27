#include <bits/stdc++.h>
using namespace std;

class vehicle {
private:
    int rown; // Row number of the parking spot
    int spotn; // Column number of the parking spot
    chrono::time_point<chrono::system_clock> start_time; // Time when the vehicle was parked

public:
    vehicle() {
        rown = -1;
        spotn = -1;
    }

    void occupy(int r, int c) {
        start_time = chrono::system_clock::now(); // Log the current time as the start time
        rown = r;
        spotn = c;
    }

    void vacate() {
        rown = -1;
        spotn = -1;
    }

    double duration() {
        auto now = chrono::system_clock::now(); // Get the current time
        auto dura = chrono::duration_cast<chrono::seconds>(now - start_time); // Calculate duration in seconds
        return dura.count() / 3600.0; // Return duration in fractional hours
    }

    virtual int calculate() = 0; // Pure virtual method for calculating parking fee
    virtual string getType() = 0; // Pure virtual method to return vehicle type
};

class car : public vehicle {
public:
    string getType() { return "Car"; }
    int calculate() { return 5 * duration(); } // Fee is 5 units per hour
};

class bike : public vehicle {
public:
    string getType() { return "Bike"; }
    int calculate() { return 2 * duration(); } // Fee is 2 units per hour
};

class Parking {
private:
    int rows;                     // Total rows in the parking lot
    int spotperrow;               // Spots per row
    vector<vector<vehicle*>> grid; // 2D grid to represent parking spots
    int preferredSpotFee = 100;     // Additional fee for a preferred spot

public:
    void create(int r, int c) {
        rows = r;
        spotperrow = c;
        grid.resize(r); // Resize grid to have r rows
        for (int i = 0; i < r; i++) {
            grid[i].resize(c, nullptr); // Resize each row to have c columns initialized to nullptr
        }
    }

    bool park(vehicle* v, int rown, int spotn, bool force = false) {
        if (rown < 0 || rown >= rows || spotn < 0 || spotn >= spotperrow) {
            cout << "Invalid parking spot: " << rown << ", " << spotn << "\n";
            return false;
        }

        if (grid[rown][spotn] != nullptr) {
            if (!force) {
                cout << "Sorry!! Spot " << rown << "," << spotn << " is already occupied.\n";
                return false;
            } else {
                cout << "Preferred spot is occupied. Overriding with additional fee of Rs " << preferredSpotFee << ".\n";
                grid[rown][spotn]->vacate(); // Vacate the current vehicle
            }
        }

        v->occupy(rown, spotn); // Assign the spot to the vehicle
        grid[rown][spotn] = v; // Update the grid
        cout << "Thanks!! for parking at " << rown << "," << spotn << " spot.\n";
        return true;
    }

    void leave(vehicle* v) {
        bool found = false;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < spotperrow; j++) {
                if (grid[i][j] == v) {
                    grid[i][j] = nullptr; // Clear the spot
                    int fee = v->calculate(); // Calculate the fee
                    cout << "Fee is Rs " << fee << endl;
                    v->vacate();
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            cout << "Vehicle not found in the parking lot.\n";
        }
    }

    void print() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < spotperrow; j++) {
                cout << (grid[i][j] ? grid[i][j]->getType() : "Empty") << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};


int main() {
    Parking p;
    p.create(3, 2); // Create a parking lot with 3 rows and 2 spots per row

    car c1, c2;
    bike b1;

    // Normal parking
    p.park(&c1, 1, 1);
    p.print();

    // Try to park another car in the same spot without force
    p.park(&c2, 1, 1);

    // Park another car in the same spot with force (preferred parking)
    p.park(&c2, 1, 1, true);
    p.print();

    // Park a bike in an available spot
    p.park(&b1, 0, 0);
    p.print();

    // Leave a car
    p.leave(&c2);
    p.print();

    return 0;
}

