#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <vector>
#include <thread>

using namespace std;
using namespace std::chrono;

const int DATA_SIZE = 100;
bool is_sorted(vector<int>& data);
void quicksort_single(int left, int right, vector<int>& data);
void quicksort_multi(int left, int right, vector<int>& data);
int partition(int left, int right, int pivot, vector<int>& data);
void print_data(vector<int>& data);




int main() {
vector<int> data;

data.reserve(DATA_SIZE);
for (int i = 0; i < DATA_SIZE; i++) data.push_back(i);
random_shuffle(data.begin(), data.end());
print_data(data);



auto start = chrono::steady_clock::now(); //start timing

quicksort_single(0, DATA_SIZE-1, data);

auto end = chrono::steady_clock::now(); //end timing

cout <<"data sorted? 0/1: " <<is_sorted(data) << endl;
cout << "Single thread quicksort eapsed time: "
	 << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
	 << " nsecs    = " <<chrono::duration_cast<chrono::nanoseconds>(end - start).count()*1e-9
	 << " seconds" << endl;


//test multi_thread sorting
start = chrono::steady_clock::now(); //start timing

quicksort_multi(0, DATA_SIZE-1, data);

end = chrono::steady_clock::now(); //end timing
cout <<"data sorted? 0/1: " <<is_sorted(data) << endl;
cout << "Multi thread quicksort Elapsed time: "
	 << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
	 << " nsecs    = " <<chrono::duration_cast<chrono::nanoseconds>(end - start).count()*1e-9
	 << " seconds" << endl;



}




bool is_sorted(vector<int>& data) {
	for (int i = 1; i < data.size(); i++) {
		if (data[i] < data[i-1]) return false;
	}
	return true;
}

void quicksort_single(int left, int right, vector<int>& data)
{
    // Continue the recursive calls until the indexes cross.
    if (left <= right)
    {
        // Choose the pivot and partition this subrange.
        int pivot = data[left];
        int p = partition(left, right, pivot, data);

        quicksort_single(left, p-1, data);  // Sort elements <  pivot
        quicksort_single(p+1, right, data); // Sort elements >= pivot
    }
}

void quicksort_multi(int left, int right, vector<int>& data)
{
    // Continue the recursive calls until the indexes cross.
    if (left <= right)
    {
        // Choose the pivot and partition this subrange.
        int pivot = data[left];
        int p = partition(left, right, pivot, data);

        thread left_thread(quicksort_multi, left, p-1, ref(data));  // Sort elements <  pivot
        thread right_thread(quicksort_multi, p+1, right, ref(data)); // Sort elements >= pivot
        left_thread.join();
        right_thread.join();
    }
}

int partition(int left, int right, int pivot, vector<int>& data)
{

	int i = left;
    for (int j = left + 1; j <= right; j++){
    	if (data[j] < pivot) {
    		i++;
    		if (i != j) {
    			int temp;
    			temp = data[i];
    			data[i] = data[j];
    			data[j] = temp;
    		}
    	}
    }
    int temp;
    temp = data[left];
    data[left] = data[i];
    data[i] = temp;
    return i;
}

void print_data(vector<int>& data) {
	cout << "data in the vector :";
	for (int i = 0; i < data.size(); i++)
		cout << data[i]<<" ";
	cout <<endl;
}