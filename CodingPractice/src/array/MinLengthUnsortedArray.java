package array;

public class MinLengthUnsortedArray {


	/*Find the Minimum length Unsorted Subarray, 
	 * sorting which makes the complete array sorted.
	 * Given an unsorted array arr[0..n-1] of size n, 
	 * find the minimum length subarray arr[s..e] 
	 * such that sorting this subarray makes the whole array sorted.
	Examples:
	1) If the input array is [10, 12, 20, 30, 25, 40, 32, 31, 35, 50, 60], 
	your program should be able to find that the subarray lies between the indexes 3 and 8.

	2) If the input array is [0, 1, 15, 25, 6, 7, 30, 40, 50], 
	your program should be able to find that the subarray lies between the indexes 2 and 5.
	 */	


	public static void main(String[] args) {


		int arr[] = {0, 1, 15, 25, 6, 7, 30, 40, 50};

		boolean leftFlag = false;
		boolean rightFlag = false;

		int leftIndex = -1;
		int rightIndex =  -1;
		int left = 999;
		int right = -1;
		for(int i = 1; i < arr.length ; i++){
			if(arr[i-1] > arr[i]){
				if(left > arr[i]){
					left = arr[i]; 
				}
				rightIndex = i ;


			}

		}
		for(int i = 0; i <=rightIndex ; i++){
			if(right < arr[i]){
				right = arr[i]; 
			}
		}
		
		System.out.println("right:"+right);
		for(int i = 0; i < arr.length ; i++){
			if(arr[i] > left){
				if(!leftFlag ){
					leftFlag = true;
					leftIndex = i;
				}
			}
			if(arr[i] > right){
				if(!rightFlag){
					rightIndex = i-1;
					rightFlag = true;
				}
			}

		}

		System.out.println("LeftIndex:"+leftIndex+" rightIndex:"+rightIndex);






	}

}
