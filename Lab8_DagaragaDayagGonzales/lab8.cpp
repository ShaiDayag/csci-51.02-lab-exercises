/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

April 6, 2026
*/


/*
We have not discussed the C++ language code and Bash scripting in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code and Bash scripting obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code and Bash scripting or documentation of either were used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include <iostream>
#include <unistd.h>
#include <vector>
#include <random>
#include <iomanip>
using namespace std;

void *monte_carlo_pi_calc(void* ptr);

/*
struct ThreadParams{
    int trialcount;
    int progressreport; 
    
}
*/

int main(int argc, char* argv[]){
    /*
    int threadcount = atoi(argv[1]);
    int trialcount = atoi(argv[2]);
    int progressreport = atoi(argv[3]);
    */

    //Random generator code
    /*
    std::random_device rd;
    std::default_random_engine generator(rd()); // rd() provides a random seed
    std::uniform_real_distribution<double> distribution(-1.0,1.0);

    double result = distribution(generator);
    cout << fixed << setprecision(18) << result;
    cout << fixed << setprecision(18) << result;
    result = distribution(generator);
    cout << fixed << setprecision(18)  << result;
    */

    int threadcount = atoi(argv[1]);
    int trialcount = atoi(argv[2]);
    int progressreport = atoi(argv[3]);

    vector<int>

    //Thread code
    /*
    long double x,y,r,estpi;
    std::random_device rd;
    std::default_random_engine generator(rd()); // rd() provides a random seed
    std::uniform_real_distribution<long double> distribution(-1.0,1.0);
    int currenttrial = 0;
    int trialwork = 0;
    while(currenttrial != trialcount){
        x = distribution(generator);
        y = distribution(generator);
        r = pow(x,2) + pow(y,2);
        ///
        cout << "x: " << fixed << setprecision(18) << x << endl;
        cout << "y: " << fixed << setprecision(18) << y << endl;
        cout << "r: " << fixed << setprecision(18) << r << endl;
        ///
        if(r<=1){
            trialwork++;
        }
        currenttrial++;
        if(int(currenttrial) % progressreport == 0){
            estpi = (long double)(trialwork)/(long double)(currenttrial)*4;
            cout << trialwork << "/" << currenttrial << " (Estimated pi is: " << fixed << setprecision(18) << estpi << ")" << endl;
        }
    }
    */
}

