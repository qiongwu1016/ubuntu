#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <cstring>

#include <mpir.h>

using namespace std;

//CONSTANTS
const int ITERATIONS = 5;

//reference: example solution by Prof.Mak
void root4(mpf_t& root4, const mpf_t& x)
{
    mpf_t root2;
    mpf_init(root2);

    mpf_sqrt(root2, x);
    mpf_sqrt(root4, root2);
}

void power4(mpf_t& xxxx, const mpf_t& x)
{
    mpf_t xx;
    mpf_init(xx);

    mpf_mul(xx, x, x);
    mpf_mul(xxxx, xx, xx);
}


int main(void) {

	mpf_t one, two, four, six, pi, sqrt2, four_sqrt2, a, a_prev, y, y_prev, powers2;

	mpf_set_default_prec(4004);

	auto start = chrono::steady_clock::now();

	  // Multiple-precision constants.
	mpf_init(one);
	mpf_set_str(one, "1", 10);
	mpf_init(two);
	mpf_set_str(two, "2", 10);
	mpf_init(four);
	mpf_set_str(four, "4", 10);
	mpf_init(six);
	mpf_set_str(six, "6", 10);



	//set value for sqrt(2) and 4*sqrt(2)
	mpf_init(sqrt2);
	mpf_init(four_sqrt2);
	mpf_sqrt(sqrt2, two);
	mpf_mul(four_sqrt2, four, sqrt2);

	//set value for a0 and y0
	mpf_init(a);
	mpf_init(a_prev); mpf_sub(a_prev, six, four_sqrt2);
	mpf_init(y);
	mpf_init(y_prev); mpf_sub(y_prev, sqrt2, one);

	//set value for 2^(2 * i + f) when i == 0
	mpf_init(powers2); mpf_set_str(powers2, "2", 10);


	//loop to calcuate ai and yi
	for (int i = 1; i <= ITERATIONS; i++) {
		mpf_t y4, temp1, temp2, yroot4, aP1, aP2, y2;
		mpf_init(temp1);
		mpf_init(temp2);
		mpf_init(yroot4);
		mpf_init(aP1);
		mpf_init(aP2);
		mpf_init(y2);

		//y4 = power(y_prev,4)
		mpf_init(y4);
		power4(y4, y_prev);
		//temp1 = 1- y4
		mpf_sub(temp1, one, y4);
		//yroot4 = root4(1-power(y_prev, 4))
		root4(yroot4, temp1);

		//compute y
		mpf_sub(temp1, one, yroot4);
		mpf_add(temp2, one, yroot4);
		mpf_div(y, temp1, temp2);

		//compute aP1 = a_prev*power((1+y), 4)
		mpf_add(temp1, one, y);
		power4(temp2, temp1);
		mpf_mul(aP1, a_prev, temp2);

		//compute aP2 = power(2, 2i + 1) * y * (1+ y + power(y, 2))
		mpf_mul(y2, y, y);
		mpf_add(temp1, y2, y);
		//temp2 = 1 + y + y2
		mpf_add(temp2, temp1, one);
		//temp1 = y*(1 + y + y2)
		mpf_mul(temp1, temp2, y);

		//compute 2^(2 * i + 1)
		mpf_mul(powers2, four, powers2);

		//compute aP2
		mpf_mul(aP2, powers2, temp1);

		//compute a

		mpf_sub(a, aP1, aP2);

		mpf_set(a_prev, a);
		mpf_set(y_prev, y);

	}






	mpf_init(pi);
	mpf_div(pi, one, a);
	auto end = chrono::steady_clock::now();

	//gmp_printf("C API: pi to 1,000 places: %.1000Ff \n", pi);
	mp_exp_t exp; 
    char *str = NULL;
    char *s = mpf_get_str(str, &exp, 10, 1001, pi);

    char *p = s + 1;  // skip the 3 before the decimal point

    cout << endl;
    cout << "3.";

    char block[11];  // +1 for the ending \0

    // Loop for each line.
    for (int i = 1; i <= 10; i++)
    {
        // Loop to print blocks of digits in each line.
        for (int j = 0; j < 100; j += 10)
        {
            strncpy(block, p+j, 10);
            block[10] = '\0';
            cout << block << " ";
        }

        cout << endl << "  ";

        // Print a blank line for grouping.
        if (i%5 == 0) cout << endl << "  ";

        p += 100;
    }

    free(s);
	

	cout << "Elapsed time: "
		<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
		<< " nsecs    = " <<chrono::duration_cast<chrono::nanoseconds>(end - start).count()*10e-9
		<< " seconds" << endl;




	return 0;
}

