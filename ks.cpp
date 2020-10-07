#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <queue>
#include<chrono>


#define Bag_Size_MAX 5000000
#define Item_Num_MAX 1000

using namespace std;
using namespace std::chrono; 


int Bag_Size;  
int Item_count;

struct Item
{
    double weight;//Item weight
    int value;//Item value (value)
};
 
struct Node
{
    int height;//tree height
	int value;//value of current node
	int bound;//(From u to) The upper bound of this node
    double weight;//weight of current node
};
 
//Compare ratio (value/weight)
bool MyCompare(Item i, Item j)
{
    return (double)i.value/i.weight > (double)j.value/j.weight;
}

//Initilization
template<typename T,size_t N>
void Init(T (&ItemList)[N])
{
    //First according to the (value/weight), sort from large to small (take the highest unit value first)
    //If it is not sorted first, it cannot be processed by the greedy methodology 
	sort(ItemList,ItemList+Item_count,MyCompare);
}

//Stuff items into the backpack (according to the higher unit value first)
int Add_Item(int *lower_bound,int *upper_bound,int Item_num,double *weight_sum,Item ItemList[])
{
    while ((Item_num < Item_count) && (*weight_sum + ItemList[Item_num].weight <= Bag_Size))
    {
        *weight_sum = *weight_sum + ItemList[Item_num].weight;
        *upper_bound = *upper_bound + ItemList[Item_num].value;
        Item_num++;
		if(*weight_sum > Bag_Size)
			*lower_bound = 0;
    }

	return Item_num;
}


//Check if the backpack is stuffed with all items
int Check_full(int *lower_bound,int upper_bound,int Item_num,double weight_sum,Item ItemList[])
{

    //If the backpack is full, but not all items
    //Upper bound needs to consider the unit value of the last Item that cannot be plugged    
    if (Item_num < Item_count){
		*lower_bound = upper_bound;
        upper_bound = upper_bound + (Bag_Size - weight_sum) * ItemList[Item_num].value / ItemList[Item_num].weight;
	}
	return upper_bound;
}

//Check if the upper bound is updated
int Check_bound(Node current, Item ItemList[],int *lower_bound)
{
	//First check whether the weight of the backpack exceeds the limit
    if (current.weight >= Bag_Size)
        return 0;

    //initialization
    int upper_bound = current.value;
    int Item_num = current.height + 1;
    double weight_sum = current.weight; 

	//Start to pack the backpack, return the item number that cannot be packed in the last
	Item_num = Add_Item(lower_bound,&upper_bound,Item_num,&weight_sum,ItemList);

	// Update the final upper_bound according to whether the backpack is packed with all items
	upper_bound = Check_full(lower_bound,upper_bound,Item_num,weight_sum,ItemList);

    return upper_bound;
}

//Check if the maximum value is updated
int Check_value(Node current,int Max_value)
{
    if (current.weight <= Bag_Size && current.value > Max_value)//如果符合條件又更有價值
           Max_value = current.value;
	return Max_value;
}
 
//Return maximum value
int knapsack(Item ItemList[])
{ 
    //initialization
    queue<Node> Bag;
    Node current, next;
	int Max_value = 0;
    current.height = -1;//Before the tree is built, it is -1, and the root of the tree is 0 (the index of the matching matrix starts from 0)
    current.value = 0;
	current.weight = 0;
	int lower_bound = 0,lb=0;

    Bag.push(current);
    while (!Bag.empty())
    {
        //Fetch from backpack
        current = Bag.front();
        Bag.pop();
 
        //Check if it is the initial node
        if (current.height == -1)
            next.height = 0;
 
        //Generate the next node and calculate the total value of all its child nodes
        next.height = current.height + 1;//Start from the next level
		next.value = current.value + ItemList[next.height].value;//The original value + the value of the next item
        next.weight = current.weight + ItemList[next.height].weight;//Original weight + weight of next item
        next.bound = Check_bound(next,ItemList,&lower_bound);//Calculate the upper bound of the next node
		if(lower_bound > lb)
			lb = lower_bound;

		//Check whether the weight limit of the backpack is exceeded, and decide whether to update the current maximum value
		Max_value = Check_value(next,Max_value);
		//If the upper bound of the next node is more valuable than the original total value, it means that the fortress is packed into the backpack
        if (next.bound > Max_value && next.bound >= lower_bound)
            Bag.push(next);

		//Calculate the value of the next node itself (use next as current temp)
        next.value = current.value;
        next.weight = current.weight;
        next.bound = Check_bound(next,ItemList,&lower_bound);
		//Check whether the weight limit of the backpack is exceeded, and decide whether to update the current maximum value
		Max_value = Check_value(next,Max_value);
		//If the upper bound of the current node is more valuable than the original total value, it means that the fortress is stuffed into the backpack
        if (next.bound > Max_value && next.bound > lb)
            Bag.push(next);
    }
    return Max_value;
}

int main()
{
    cout << "#########BRANCH AND BOUND METHOD#########" << endl;
/*	
	Item ItemList[] = {{41, 442}, {50, 525}, {49, 511},{59, 593}, {55, 546},{57,564},{60,617}};//物品清單
	Bag_Size = 170; 
	Item_count = sizeof(ItemList) / sizeof(ItemList[0]);
*/	

	Item ItemList[Item_Num_MAX]={0};//Item list

    cout << "Enter bag size(capacity) and then enter number of items(n): " << endl;
	cin >> Bag_Size >> Item_count; 
    srand(time(NULL));

	for(int i=0;i<Item_count;i++)
	{
        cout << "For item " << i+1 << ": " << endl;
        ItemList[i].value = (rand()% (1000-20 +1));
        ItemList[i].weight = (rand() % (Bag_Size + 1));
        cout << "Weight: " << ItemList[i].weight << " | Value: "<<  ItemList[i].value << endl;  ;
		
	}


	//initialization
	Init(ItemList);
	//主function
    auto start = high_resolution_clock::now(); 
    cout << "Total weight in sack: " <<knapsack(ItemList) << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 

    cout << "\nTime taken( microseconds ): "<< duration.count() << endl;

	//system("PAUSE");
    return 0;
}