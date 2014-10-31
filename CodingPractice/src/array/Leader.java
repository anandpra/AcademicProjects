package array;
/*Write a program to print all the LEADERS in the array. 
 * An element is leader if it is greater than all the elements to its right side. 
 * And the rightmost element is always a leader. For example int the array {16, 17, 4, 3, 5, 2},
 *  leaders are 17, 5 and 2.
Let the input array be arr[] and size of the array be size*/



public class Leader {

	public static void main(String args[]){
		
		int arr[] = {21,16, 17, 4, 3,19, 5, 2};
		
		int greatestNumSeenSoFar = -1;
		
		for(int i = arr.length - 1 ;i >= 0 ; i-- ){
			if(arr[i] > greatestNumSeenSoFar){
				System.out.println(arr[i]);
				greatestNumSeenSoFar = arr[i];
			}
		}
		
	}
}
