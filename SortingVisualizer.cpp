#include<SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
using namespace std;

const int SCREEN_WIDTH=910;
const int SCREEN_HEIGHT=750;

const int arrSize=130;
const int rectSize=7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;

bool complete=false;

bool init()
{
    bool success=true;
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        cout<<"Couldn't initialize SDL. SDL_Error: "<<SDL_GetError();
        success=false;
    }
    else
    {
        if(!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout<<"Warning: Linear Texture Filtering not enabled.\n";
        }
        // SDL_WINDOWPOS_CENTERED_DISPLAY(n)
        window=SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window==NULL)
        {
            cout<<"Couldn't create window. SDL_Error: "<<SDL_GetError();
            success=false;
        }
        else
        {
            renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer==NULL)
            {
                cout<<"Couldn't create renderer. SDL_Error: "<<SDL_GetError();
                success=false;
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer=NULL;

    SDL_DestroyWindow(window);
    window=NULL;

    SDL_Quit();
}

void visualize(int x=-1, int y=-1, int z=-1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j=0;
    for(int i=0; i<=SCREEN_WIDTH-rectSize; i+=rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect={i, 0, rectSize, arr[j]};
        if(complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if(j==x || j==z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j==y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void inplaceHeapSort(int* input, int n)
{
    for(int i=1; i<n; i++)
    {
       int childIndex=i;
       int parentIndex=(childIndex-1)/2;

       while(childIndex>0)
       {
           if(input[childIndex]>input[parentIndex])
           {
               int temp=input[parentIndex];
               input[parentIndex]=input[childIndex];
               input[childIndex]=temp;

           }
           else
           {
               break;
           }

           visualize(parentIndex, childIndex);
           SDL_Delay(40);

           childIndex=parentIndex;
           parentIndex=(childIndex-1)/2;
       }
    }

    for(int heapLast=n-1; heapLast>=0 ; heapLast--)
    {
        int temp=input[0];
        input[0]=input[heapLast];
        input[heapLast]=temp;

        int parentIndex=0;
        int leftChildIndex=2*parentIndex + 1;
        int rightChildIndex=2*parentIndex + 2;

        while(leftChildIndex<heapLast)
        {
            int maxIndex=parentIndex;

            if(input[leftChildIndex]>input[maxIndex])
            {
                maxIndex=leftChildIndex;
            }
            if(rightChildIndex<heapLast && input[rightChildIndex]>input[maxIndex])
            {
                maxIndex=rightChildIndex;
            }
            if(maxIndex==parentIndex)
            {
                break;
            }

            int temp=input[parentIndex];
            input[parentIndex]=input[maxIndex];
            input[maxIndex]=temp;

            visualize(maxIndex, parentIndex, heapLast);
            SDL_Delay(40);

            parentIndex=maxIndex;
            leftChildIndex=2*parentIndex + 1;
            rightChildIndex=2*parentIndex + 2;
        }
    }
}

int partition_array(int a[], int si, int ei)
{
    int count_small=0;

    for(int i=(si+1);i<=ei;i++)
    {
        if(a[i]<=a[si])
        {
            count_small++;
        }
    }
    int c=si+count_small;
    int temp=a[c];
    a[c]=a[si];
    a[si]=temp;
    visualize(c, si);

    int i=si, j=ei;

    while(i<c && j>c)
    {
        if(a[i]<= a[c])
        {
            i++;
        }
        else if(a[j]>a[c])
        {
            j--;
        }
        else
        {
            int temp_1=a[j];
            a[j]=a[i];
            a[i]=temp_1;

            visualize(i, j);
            SDL_Delay(70);

            i++;
            j--;
        }
    }
    return c;
}

void quickSort(int a[], int si, int ei)
{
    if(si>=ei)
    {
        return;
    }

    int c=partition_array(a, si, ei);
    quickSort(a, si, c-1);
    quickSort(a, c+1, ei);

}

void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output=(ei-si)+1;
    int* output=new int[size_output];

    int mid=(si+ei)/2;
    int i=si, j=mid+1, k=0;
    while(i<=mid && j<=ei)
    {
        if(a[i]<=a[j])
        {
            output[k]=a[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k]=a[j];
            visualize(i, j);
            j++;
            k++;
        }

    }
    while(i<=mid)
    {
        output[k]=a[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while(j<=ei)
    {
        output[k]=a[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x=0;
    for(int l=si; l<=ei; l++)
    {
        a[l]=output[x];
        visualize(l);
        SDL_Delay(15);
        x++;
    }
    delete []output;
}

void mergeSort(int a[], int si, int ei)
{
    if(si>=ei)
    {
        return;
    }
    int mid=(si+ei)/2;

    mergeSort(a, si, mid);
    mergeSort(a, mid+1, ei);

    merge2SortedArrays(a, si, ei);
}

void bubbleSort()
{
    for(int i=0; i<arrSize-1; i++)
    {
        for(int j=0; j<arrSize-1-i; j++)
        {
            if(arr[j+1]<arr[j])
            {
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;

                visualize(j+1, j, arrSize-i);
            }
            SDL_Delay(1);
        }
    }
}

void insertionSort()
{
    for(int i=1; i<arrSize; i++)
    {
        int j=i-1;
        int temp=arr[i];
        while(j>=0 && arr[j]>temp)
        {
            arr[j+1]=arr[j];
            j--;

            visualize(i, j+1);
            SDL_Delay(5);
        }
        arr[j+1]=temp;
    }
}

void selectionSort()
{
    int minIndex;
    for(int i=0;i<arrSize-1;i++)
    {
        minIndex=i;
        for(int j=i+1;j<arrSize;j++)
        {
            if(arr[j]<arr[minIndex])
            {
                minIndex=j;
                visualize(i, minIndex);
            }
            SDL_Delay(1);
        }
        int temp=arr[i];
        arr[i]=arr[minIndex];
        arr[minIndex]=temp;
    }
}
void quicksort(){
    cout<<"if(si>=ei)\n"
    <<"{\n"
    <<    "return;\n"
    <<"}\n"

    <<"int c=partition_array(a, si, ei);\n"
    <<"quickSort(a, si, c-1);\n"
    <<"quickSort(a, c+1, ei);\n";
}
int partitionarray()
{
    cout<<"int count_small=0;\n"

    <<"for(int i=(si+1);i<=ei;i++);\n"
    <<"{\n"
    <<    "if(a[i]<=a[si])\n"
    <<    "{\n"
    <<        "count_small++;\n"
    <<    "}\n"
    <<"}\n"
    <<"int c=si+count_small;\n"
    <<"int temp=a[c];\n"
    <<"a[c]=a[si];\n"
    <<"a[si]=temp;\n"
    

    <<"int i=si, j=ei;\n"

    <<"while(i<c && j>c)\n"
    <<"{\n"
    <<    "if(a[i]<= a[c])\n"
    <<    "{\n"
    <<        "i++;\n"
    <<    "}\n"
    <<    "else if(a[j]>a[c])\n"
    <<    "{\n"
    <<        "j--;\n"
    <<    "}\n"
    <<    "else\n"
    <<    "{\n"
    <<        "int temp_1=a[j];\n"
    <<        "a[j]=a[i];\n"
    <<        "a[i]=temp_1;\n"

           

    <<        "i++;\n"
    <<        "j--;\n"
    <<    "}\n"
    <<"}\n"
    <<"return c;\n";
}
void inplaceheapsort(int* input, int n)
{
    cout<<"for(int i=1; i<n; i++)\n"
    <<"{\n"
    <<   "int childIndex=i;\n"
    <<   "int parentIndex=(childIndex-1)/2;\n"

    <<   "while(childIndex>0)\n"
    <<   "\n{"
    <<       "if(input[childIndex]>input[parentIndex])\n"
    <<       "{\n"
    <<           "int temp=input[parentIndex];\n"
    <<           "input[parentIndex]=input[childIndex];\n"
    <<           "input[childIndex]=temp;\n"

    <<       "}\n"
    <<       "else\n"
    <<       "{\n"
    <<           "break;\n"
    <<       "}\n"

    

    <<       "childIndex=parentIndex;\n"
    <<       "parentIndex=(childIndex-1)/2;\n"
    <<   "}\n"
    <<"}\n"

    <<"for(int heapLast=n-1; heapLast>=0 ; heapLast--)\n"
    <<"{\n"
    <<    "int temp=input[0];\n"
    <<    "input[0]=input[heapLast];\n"
    <<    "input[heapLast]=temp;\n"

    <<    "int parentIndex=0;\n"
    <<    "int leftChildIndex=2*parentIndex + 1;\n"
    <<    "int rightChildIndex=2*parentIndex + 2;\n"

    <<    "while(leftChildIndex<heapLast)\n"
    <<    "{\n"
    <<        "int maxIndex=parentIndex;\n"

    <<        "if(input[leftChildIndex]>input[maxIndex])\n"
    <<        "{\n"
    <<            "maxIndex=leftChildIndex;\n"
    <<        "}\n"
    <<        "if(rightChildIndex<heapLast && input[rightChildIndex]>input[maxIndex])\n"
    <<        "{\n"
    <<            "maxIndex=rightChildIndex;\n"
    <<        "}\n"
    <<        "if(maxIndex==parentIndex)\n"
    <<        "{\n"
    <<            "break;\n"
    <<        "}\n"

    <<        "int temp=input[parentIndex];\n"
    <<        "input[parentIndex]=input[maxIndex];\n"
    <<        "input[maxIndex]=temp;\n"

    <<        "visualize(maxIndex, parentIndex, heapLast);\n"
    <<        "SDL_Delay(40);\n"

    <<        "parentIndex=maxIndex;\n"
    <<        "leftChildIndex=2*parentIndex + 1;\n"
    <<        "rightChildIndex=2*parentIndex + 2;\n"
    <<    "}\n"
    <<"}\n";
}
void selectionsort(){
    cout<<"int minIndex\n"
        <<"for(int i=0;i<arrSize-1;i++)\n"
        <<"{\n"
        <<"minIndex=i;\n"
        <<" for(int j=i+1;j<arrSize;j++)\n"
        <<"{\n"
        <<    "if(arr[j]<arr[minIndex])\n"
        <<    "{\n"
        <<        "minIndex=j;\n"
        <<    "}\n"
        <<    "}\n"
        <<        "int temp=arr[i];\n"
        <<        "arr[i]=arr[minIndex];\n"
        <<        "arr[minIndex]=temp;\n"
        <<    "}\n"

    <<"}";
}
void bubblesort(){
    cout<<"for(int i=0; i<arrSize-1; i++)\n"
        <<"{\n"
        <<"for(int j=0; j<arrSize-1-i; j++)\n"
        <<"{\n"
        <<    "if(arr[j+1]<arr[j])\n"
        <<    "{\n"
        <<        "int temp=arr[j];\n"
        <<        "arr[j]=arr[j+1];\n"
        <<        "arr[j+1]=temp;\n"

        <<    "}\n"

        <<"}\n"
    <<"}";
}
void insertionsort(){
    cout<<"for(int i=1; i<arrSize; i++)\n"
    <<"{\n"
    <<    "int j=i-1;\n"
    <<    "int temp=arr[i];\n"
    <<    "while(j>=0 && arr[j]>temp)\n"
    <<    "{\n"
    <<        "arr[j+1]=arr[j];\n"
    <<        "j--;\n"
    <<    "}\n"
    <<    "arr[j+1]=temp;\n"
    <<"}\n";
}
void mergesort(){
    cout<<"if(si>=ei)\n"
    <<"{\n"
    <<    "return;\n"
    <<"}\n"
    <<"int mid=(si+ei)/2;\n"

    <<"mergeSort(a, si, mid);\n"
    <<"mergeSort(a, mid+1, ei);\n"

    <<"merge2SortedArrays(a, si, ei);\n";
}
void merge2sortedarrays(){
    cout<<"\n\n mergetwosortedarrays(){"
    <<"\n int size_output=(ei-si)+1;\n"
    <<"int* output=new int[size_output];\n"

    <<"int mid=(si+ei)/2;\n"
    <<"int i=si, j=mid+1, k=0;\n"
    <<"while(i<=mid && j<=ei)\n"
    <<"{\n"
    <<    "if(a[i]<=a[j])\n"
    <<    "{\n"
    <<        "output[k]=a[i];\n"
    <<        "i++;\n"
    <<        "k++;\n"
    <<    "}\n"
    <<    "else\n"
    <<    "{\n"
    <<        "output[k]=a[j];\n"
    <<        "j++;\n"
    <<        "k++;\n"
    <<    "}\n"

    <<"}\n"
    <<"while(i<=mid)\n"
    <<"{\n"
    <<    "output[k]=a[i];\n"
    <<    "i++;\n"
    <<    "k++;\n"
    <<"}\n"
    <<"while(j<=ei)\n"
    <<"{\n"
    <<    "output[k]=a[j];\n"
    <<    "j++;\n"
    <<    "k++;\n"
    <<"}\n"
    <<"int x=0;\n"
    <<"for(int l=si; l<=ei; l++)\n"
    <<"{\n"
    <<    "a[l]=output[x];\n"
        
    <<    "x++;\n"
    <<"}\n"
    <<"delete []output;\n"
<<"}\n";
}
void loadArr()
{
    memcpy(arr, Barr, sizeof(int)*arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed=(unsigned)time(NULL);
    srand(seed);
    for(int i=0; i<arrSize; i++)
    {
        int random=rand()%(SCREEN_HEIGHT);
        Barr[i]=random;
    }
}

void execute()
{
    if(!init())
    {
        cout<<"SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();

        SDL_Event e;
        bool quit=false;
        while(!quit)
        {
            while(SDL_PollEvent(&e)!=0)
            {
                if(e.type==SDL_QUIT)
                {
                    quit=true;
                    complete=false;
                }
                else if(e.type==SDL_KEYDOWN)
                {
                    switch(e.key.keysym.sym)
                    {
                        case(SDLK_q):
                            quit=true;
                            complete=false;
                            cout<<"\nEXITING SORTING VISUALIZER.\n";
                            break;
                        case(SDLK_0):
                            randomizeAndSaveArray();
                            complete=false;
                            loadArr();
                            cout<<"\nNEW RANDOM LIST GENERATED.\n";
                            break;
                        case(SDLK_1):
                            loadArr();
                            cout<<"\nSELECTION SORT STARTED.\n";
                            complete=false;
                            selectionSort();
                            selectionsort();
                            complete=true;
                            cout<<"\nSELECTION SORT COMPLETE.\n BEST CASE TIME COMPLEXITY: O(N^2).\n AVERAGE CASE TIME COMLEXITY: O(N^2).\n WORST CASE TIME COMPLEXITY: O(N^2)\n";
                            break;
                        case(SDLK_2):
                            loadArr();
                            cout<<"\nINSERTION SORT STARTED.";
                            complete=false;
                            insertionSort();
                            insertionsort();
                            complete=true;
                            cout<<"\nINSERTION SORT COMPLETE.\n BEST CASE TIME COMPLEXITY: O(N).\n AVERAGE CASE TIME COMLEXITY: O(N^2).\n WORST CASE TIME COMPLEXITY: O(N^2)\n";
                            break;
                        case(SDLK_3):
                            loadArr();
                            cout<<"\nBUBBLE SORT STARTED.\n";
                            complete=false;
                            bubbleSort();
                            bubblesort();
                            complete=true;
                            cout<<"\nBUBBLE SORT COMPLETE.\n BEST CASE TIME COMPLEXITY: O(N).\n AVERAGE CASE TIME COMLEXITY: O(N^2).\n WORST CASE TIME COMPLEXITY: O(N^2)\n";
                            break;
                        case(SDLK_4):
                            loadArr();
                            cout<<"\nMERGE SORT STARTED.\n";
                            complete=false;
                            mergeSort(arr, 0, arrSize-1);
                            mergesort();
                            merge2sortedarrays();
                            complete=true;
                            cout<<"\nMERGE SORT COMPLETE.\n BEST CASE TIME COMPLEXITY: O(NlogN).\n AVERAGE CASE TIME COMLEXITY: O(NlogN).\n WORST CASE TIME COMPLEXITY: O(NlogN)\n";
                            break;
                        case(SDLK_5):
                            loadArr();
                            cout<<"\nQUICK SORT STARTED.\n";
                            complete=false;
                            quickSort(arr, 0, arrSize-1);
                            quicksort();
                            partitionarray();
                            complete=true;
                            cout<<"\nQUICK SORT COMPLETE.\n BEST CASE TIME COMPLEXITY: O(NlogN).\n AVERAGE CASE TIME COMLEXITY: O(NlogN).\n WORST CASE TIME COMPLEXITY: O(N^2)\n";
                            break;
                        case(SDLK_6):
                            loadArr();
                            cout<<"\nHEAP SORT STARTED.\n";
                            complete=false;
                            inplaceHeapSort(arr, arrSize);
                            complete=true;
                            cout<<"\nHEAP SORT COMPLETE.\n BEST CASE TIME COMPLEXITY: O(NlogN).\n AVERAGE CASE TIME COMLEXITY: O(NlogN).\n WORST CASE TIME COMPLEXITY: O(NlogN)\n";
                            break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout <<"WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
         <<"Available Controls inside Sorting Visualizer:-\n"
         <<"    Use 0 to Generate a different randomized list.\n"
         <<"    Use 1 to start Selection Sort Algorithm.\n"
         <<"    Use 2 to start Insertion Sort Algorithm.\n"
         <<"    Use 3 to start Bubble Sort Algorithm.\n"
         <<"    Use 4 to start Merge Sort Algorithm.\n"
         <<"    Use 5 to start Quick Sort Algorithm.\n"
         <<"    Use 6 to start Heap Sort Algorithm.\n"
         <<"    Use q to exit out of Sorting Visualizer\n\n"
         <<"PRESS ENTER TO START SORTING VISUALIZER...\n\n"
         <<"Or type -1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);
    if(s=="-1")
    {
        return false;
    }
    //else if(s=="\n")
    //{
    //    return true;
    //}
    return true;
}

void intro()
{
    cout<<"==============================Sorting Visualizer==============================\n\n"
        <<"Visualization of different sorting algorithms in C++ with SDL2 Library. A sorting algorithm is an algorithm that puts the elements of a list in a certain order. While there are a large number of sorting algorithms, in practical implementations a few algorithms predominate.\n"
        <<"In this implementation of sorting visualizer, we'll be looking at some of these sorting algorithms and visually comprehend their working.\n"
        <<"The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort, Merge Sort, Quick Sort and Heap Sort.\n"
        <<"The list size is fixed to 130 elements. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order. The sorting time being visualized for an algorithm is not exactly same as their actual time complexities. The relatively faster algorithms like Merge Sort, etc. have been delayed so that they could be properly visualized.\n\n"
        <<"Press ENTER to show controls...";

    string s;
    getline(cin, s);
    if(s=="\n")
    {
        return;
    }
}

int main(int argc, char* args[])
{
    intro();

    while(1)
    {
        cout<<'\n';
        if(controls())
            execute();
        else
        {
            cout<<"\nEXITING PROGRAM.\n";
            break;
        }
    }
     return 0;
}