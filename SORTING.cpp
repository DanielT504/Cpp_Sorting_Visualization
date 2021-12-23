#include <iostream>
#include "apvector.h"
#include "apstring.h"
#include "apstring.cpp"
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

//declaring allegro text font
ALLEGRO_FONT *arial;

//setting algorithm measurements as globals because they are used in almost every function
int comparisons = 0;
int moves = 0;

//prototyping functions
void random(apvector<int> &data);
void drawData(apvector<int> data, int whichSort);
void drawLine(int val, int spot, int whichSort);
void drawFinal(apvector<int> data, int z, int whichSort);
void mergeSort(apvector<int> &data, int left, int right, int layer);
void order(apvector<int> &subData1, apvector<int> &subData2, apvector<int> &data, int left, int layer);
void selectionSort(apvector<int> &data, int whichSort);
void quickSort(apvector<int> &data, int Left, int Right);
int sorter(apvector<int> &data, int Left, int Right);
void setUpSorts(apvector<int> data, int which);

int main(int argc, char **argv){
    //declaring vector to store base data
    apvector<int> data(960, 0);

    //necessary allegro syntax
    al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_DISPLAY *display = nullptr;
	display = al_create_display(960, 720);
    arial = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", 20, 0);

    //error message for invalid display access
	if (!display) {
    	cerr << "Failed to initialize display";

       	return 1;
	}

    //error message for invalid primitives access
	if(!al_init_primitives_addon()){
        cerr << "Failed to initialize primitives";

       	return 1;
	}

    //error message for invalid font access
	if(!arial){
        cerr << "Allegro font could not be found." << endl;

        return 1;
	}

    //function to randomize the base data
    random(data);

    //drawing out the currently random data for each function
    for(int h = 0; h < 3; h++) {
        drawData(data, h);
    }

    //executing each algorithm sequentially
    for(int z = 0; z < 3; z++) {
        setUpSorts(data, z);
    }

    al_flip_display();

    //resting until user closes the executable file
    al_rest(100000);

    return 0;
}

//function to draw out and execute each sequential sorting algorithm
void setUpSorts(apvector<int> data, int which){
    //recording time of algorithm execution
    float startTime = al_get_time();

    //proceeded accordingly depending on which function is being requested
    if(which == 0){
        al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 0, 0, "Merge Sort");

        mergeSort(data, 0, data.length() * 2 - 1, 1);
    } else if(which == 1){
        al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 240, 0, "Quick Sort");

        quickSort(data, 0, data.length() - 1);
        drawFinal(data, 960, 1);
    } else {
        al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 480, 0, "Selection Sort");

        selectionSort(data, 2);
        drawFinal(data, 960, 2);
    }

    //declaring variables used to measure specs or this algorithm
    char time[100];
    char ncomparison[100];
    char nmove[100];
    int wholeNumber = 0;
    apstring Time;
    float tempTime = al_get_time() - startTime;
    char segment[50];
    char decimal[50];

    //counting how many whole seconds were recorded, and leaving the decimal places
    while(tempTime > 1){
        tempTime--;
        wholeNumber++;
    }

    //manually piecing the whole number together as a string
    itoa(wholeNumber, segment, 10);
    Time += segment;
    Time += '.';

    //adjusting factor of decimal places
    tempTime *= 100000000;

    //manually piecing the decimal together as a string
    itoa(tempTime, decimal, 10);
    Time += decimal;

    //cutting off the number at a given length
    Time = Time.substr(0, 6);

    //converting the apstring to a char array
    for(int w = 0; w < Time.length() - 1; w++){
        time[w] = Time[w];
    }

    //converting the values for recorded comparisons and moves to strings
    itoa(comparisons, ncomparison, 10);
    itoa(moves, nmove, 10);

    //printing specs
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 40 + which * 240, 0, "Time (s):");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 60 + which * 240, 0, "Comparisons:");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 80 + which * 240, 0, "Moves:");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 150, 40 + which * 240, 0, time);
    al_draw_text(arial, al_map_rgb(255, 255, 255), 150, 60 + which * 240, 0, ncomparison);
    al_draw_text(arial, al_map_rgb(255, 255, 255), 150, 80 + which * 240, 0, nmove);
}

//function to randomize data into unique values
void random(apvector<int> &data){
    //randomizing seed
    srand(time(NULL));

    //for each possible value
    for(int a = 0; a < 960; a++) {
        //setting condition to true
        bool y = 1;

        //continuing to search until false
        while(y) {
            //assuming initially that value is not taken
            bool invalid = false;

            //picking a random number within the given range
            int x = rand() % 960 + 1;

            //go through the entire array
            for (int b = 0; b < data.length(); b++) {
                //if the value is found, mark it as invalid
                if(data[b] == x) {
                    invalid = true;
                }
            }

            //if value is valid, add it to the randomized data array and exit loop
            if(!invalid) {
                data[a] = x;

                y = 0;
            }
        }
    }
}

//function to represent the data visually by using red bars
void drawData(apvector<int> data, int whichSort){
    //for each element of the array
    for(int b = 0; b < 960; b++) {
        drawLine(data[b], b, whichSort);
    }

    //printing algorithm titles
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 0, 0, "Merge Sort");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 240, 0, "Quick Sort");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 480, 0, "Selection Sort");

    al_flip_display();
}

//function that draws the actual lines to the screen
void drawLine(int val, int spot, int whichSort){
    //solid black line to reset that particular x value
    al_draw_line(spot, 240 + whichSort * 240, spot, 0 + whichSort * 240, al_map_rgb_f(0, 0, 0), 1);

    //red line representing data value
    al_draw_line(spot, 240 + whichSort * 240, spot, (240 - val / 4) + whichSort * 240, al_map_rgb_f(255, 0, 0), 1);
}

//merge sort algorithm
void mergeSort(apvector<int> &data, int left, int right, int layer){
    //continue until the end of the leftmost completed segment is reached
    if (right > left) {
        //creating a new midpoint
        int midpoint = left + (right - left) / 2;

        //drawing green progress line
        al_draw_line(midpoint, 240, midpoint, 0, al_map_rgb_f(0, 255, 0), 1);

        //recursing leftmost segment
        mergeSort(data, left, midpoint, layer + 1);

        //stopping once all the data has been sorted
        if(layer > 1) {
            //recursing rightmost segment
            mergeSort(data, midpoint + 1, right, layer + 1);

            //creating two sub data arrays to store both sides of the current segment
            apvector<int> subData1(midpoint - left + 1);
            apvector<int> subData2(right - midpoint);

            //copying first side of data to sub array
            for (int i = 0; i < midpoint - left + 1; i++) {
                subData1[i] = data[left + i];
            }

            //copying second side of data to sub array
            for (int j = 0; j < right - midpoint; j++) {
                subData2[j] = data[midpoint + j + 1];
            }

            //ordering sorted halves
            order(subData1, subData2, data, left, layer);

        }
    }
}

//function to merge sorted segments of equivalent size
void order(apvector<int> &subData1, apvector<int> &subData2, apvector<int> &data, int left, int layer){
    //setting arbitrary variables to counters and an indicator for the right side of the leftmost sorted segment
    int x = 0, y = 0, z = left;

    //continue while there is still unsorted data in either sub array
    while (x < subData1.length() && y < subData2.length()) {
        //incrementing comparison and move count
        comparisons++;
        moves++;

        //determining which sub data contains the next smallest data value first
        if(subData1[x] < subData2[y] + 1) {
            data[z] = subData1[x];
            x++;
        } else {
            data[z] = subData2[y];
            y++;
        }

        z++;

        //if recursion is on the final branch, draw the sorted data in green
        if(layer > 2) {
            drawData(data, 0);
        } else {
            drawFinal(data, z, 0);
        }

        //drawing progress line in green
        al_draw_line(z, 240, z, 0, al_map_rgb_f(0, 255, 0), 1);
        al_flip_display();
    }

    //continue while there is still data in the first sub array
    while(x < subData1.length()) {
        //incrementing moves counter and copying appropriate data to final array
        moves++;
        data[z] = subData1[x];

        x++;
        z++;

        //if recursion is on the final branch, draw the sorted data in green
        if(layer > 2) {
            drawData(data, 0);
        } else {
            drawFinal(data, z, 0);
        }

        //drawing progress line in green
        al_draw_line(z, 240, z, 0, al_map_rgb_f(0, 255, 0), 1);
        al_flip_display();
    }

    //continue while there is still data in the second sub array
    while(y < subData2.length()) {
        y++;
        z++;

        //if recursion is on the final branch, draw the sorted data in green
        if(layer > 2) {
            drawData(data, 0);
        } else {
            drawFinal(data, z, 0);
        }

        //drawing progress line in green
        al_draw_line(z, 240, z, 0, al_map_rgb_f(0, 255, 0), 1);
        al_flip_display();
    }
}

//function to redraw data including sorted data in green
void drawFinal(apvector<int> data, int z, int whichSort){
    //drawing up to a specified point in green to represent final data
    for(int b = 0; b < z; b++) {
        al_draw_line(b, 240 + whichSort * 240, b, 0 + whichSort * 240, al_map_rgb_f(0, 0, 0), 1);
        al_draw_line(b, 240 + whichSort * 240, b, (240 - data[b] / 4) + whichSort * 240, al_map_rgb_f(0, 255, 0), 1);
    }

    //drawing from the specified point in red to represent unsorted data
    for(int b = z + 1; b < 960; b++) {
        al_draw_line(b, 240 + whichSort * 240, b, 0 + whichSort * 240, al_map_rgb_f(0, 0, 0), 1);
        al_draw_line(b, 240 + whichSort * 240, b, (240 - data[b] / 4) + whichSort * 240, al_map_rgb_f(255, 0, 0), 1);
    }

    //printing algorithm titles
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 0, 0, "Merge Sort");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 240, 0, "Quick Sort");
    al_draw_text(arial, al_map_rgb(255, 255, 255), 0, 480, 0, "Selection Sort");

    al_flip_display();
}

//selection sort algorithm
void selectionSort(apvector<int> &data, int whichSort){
    //declaring variables
    int counter = 0;
    int space = 959;

    //for each data value
    while(counter != data.length()) {
        //temporarily storing current data value
        int temp = data[counter];

        //for each remaining data value after the current point
        for(int a = counter; a < data.length(); a++) {
            //incrementing comparisons
            comparisons++;

            //if the currently stored data is greater than the current data
            if(temp > data[a]) {
                //drawing green progress line
                al_draw_line(a, 240 + whichSort * 240, a, 0 + whichSort * 240, al_map_rgb_f(0, 255, 0), 1);
                al_flip_display();

                //drawing sorted data in green
                drawFinal(data, counter, 2);

                //recording value location and adjusting current lowest value
                space = a;
                temp = data[a];
            }
        }

        //incrementing moves accordingly
        moves += 3;
        //swapping lowest found data element with leftmost unsorted data value
        int buffer = data[counter];
        data[counter] = temp;
        data[space] = buffer;

        //moving to the next data value
        counter++;
    }
}

//quick sort algorithm
void quickSort(apvector<int> &data, int Left, int Right){
    //determining at what point to switch to bubble sort
    if(Right - Left > 5) {
        //breakpoint
        if (Left < Right) {
            //determining next partition
            int split = sorter(data, Left, Right);

            //recursing for both sides of the partition
            quickSort(data, Left, split - 1);
            quickSort(data, split, Right);
        }
    } else {
        //bubble sort

        //setting condition
        bool done = false;

        //stopping when the last set of comparisons produced no swaps
        while(!done){
            done++;

            //for the specified range
            for(int q = Right; q > Left; q--){
                //incrementing number of comparisons
                comparisons++;

                //swapping data if left is greater than right
                if(data[q] < data[q - 1]){
                    //incrementing moves accordingly
                    moves += 3;
                    //swapping data values using a buffer
                    int tem = data[q];
                    data[q] = data[q - 1];
                    data[q - 1] = tem;

                    done = 0;
                }
            }
            //drawing current progress
            drawFinal(data, Left, 1);
        }
    }
}

//function to sort data into greater and smaller than partition
int sorter(apvector<int> &data, int Left, int Right){
    //declaring variables
    int divider = data[Right];
    int g = (Left - 1);

    //for current segment
    for (int f = Left; f < Right; f++) {
        //incrementing number of comparisons
        comparisons++;

        //determining whether data value is greater or less than partition
        if (data[f] < divider + 1) {
            g++;

            //drawing green progress line at left and right ends of segment
            al_draw_line(f, 480, f, 240, al_map_rgb_f(0, 255, 0), 1);
            al_flip_display();

            //drawing green progress line at partition
            al_draw_line(divider, 480, divider, 240, al_map_rgb_f(0, 255, 0), 1);
            al_flip_display();

            //incrementing number of moves accordingly
            moves += 3;
            //swapping data values using a buffer
            int temp = data[g];
            data[g] = data[f];
            data[f] = temp;

            //redrawing current progress
            drawFinal(data, Left, 1);

            //drawing green progress line at partition again to improve animation
            al_draw_line(divider, 480, divider, 240, al_map_rgb_f(0, 255, 0), 1);
            al_flip_display();
        }
    }

    //incrementing number of moves accordingly
    moves += 3;
    //swapping data values using a buffer
    int ntemp = data[g + 1];
    data[g + 1] = data[Right];
    data[Right] = ntemp;

    //returning current position
    return (g + 1);
}
