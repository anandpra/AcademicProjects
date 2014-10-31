package array;

import java.util.Arrays;

public class Sort0123 {
	/*Given an array A[] consisting 0s, 1s and 2s, write a function that sorts A[]. 
	 * The functions should put all 0s first, then all 1s and all 2s in last.

	Example
	Input = {0, 1, 1, 0, 1, 2, 1, 2, 0, 0, 0, 1};
	Output = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2}

	The problem is similar to our old post Segregate 0s and 1s in an array, and 
	both of these problems are variation of famous Dutch national flag problem.
	 */	


	static void swap(int input[], int i , int j){
		int temp ;
		temp = input[i];
		input[i] = input[j];
		input[j] = temp;
	}
	public static void main(String[] args) {

		int input[] = {0, 1, 1,3, 0,3, 1, 2, 1,3,3, 2, 0, 0, 0, 1};

		int zeroIndex= 0, oneIndex = 0, twoIndex = 0, threeIndex = input.length;

		System.out.println("Before:\n"+Arrays.toString(input));
		
		while(twoIndex < threeIndex){

			if(input[twoIndex] == 0){
			
				swap(input,oneIndex,twoIndex);
				swap(input,zeroIndex,oneIndex);				
				zeroIndex++;
				oneIndex++;
				twoIndex++;
					
			}else if(input[twoIndex] == 1){
				swap(input,twoIndex,oneIndex);				
				oneIndex++;
				twoIndex++;
						
			}else if(input[twoIndex] == 2){
				twoIndex++;
						
			}else if(input[twoIndex] == 3){
				threeIndex--;
				swap(input,twoIndex,threeIndex);
				
			}

		}

		System.out.println("After:\n"+Arrays.toString(input));





	}

}
