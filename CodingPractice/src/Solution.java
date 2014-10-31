import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

import org.w3c.dom.traversal.NodeIterator;

/**
 * For the graph traversing algorithm.
 */
public class Solution {

	/**
	 * A node in a graph
	 */
	private static class Node {
		private final Set<Node> neighbors;

		Node() {
			this.neighbors = new HashSet<>();
		}
	}

	/** @return a map from degree to number of nodes of that degree */
	private static Map<Integer, Integer> calculateDegreeCount(Node node) {
		// YOUR CODE HERE
		Map<Integer, Integer> resultMap = new HashMap<Integer,Integer>();
		
		if(node == null)
			return null;
		
		Map<Node,Integer> nodeToInt = new HashMap<Node,Integer>();
		Queue<Node> nodeQueue = new LinkedList<Node>();
		
		nodeQueue.offer(node);
		int count;
		nodeToInt.put(node, 0);
		int nodeCount ;
		while(!nodeQueue.isEmpty()){
			Node currentNode = nodeQueue.poll();
			System.out.println("New Node");

			if(currentNode == null)
				continue;
			if(nodeToInt.containsKey(currentNode))
				nodeCount = nodeToInt.get(currentNode);
			else{
				nodeCount = 0;
				nodeToInt.put(currentNode, nodeCount);
			}

			Iterator<Node> neighborsList = currentNode.neighbors.iterator();
			while(neighborsList.hasNext()){
				Node adjacentNode = neighborsList.next();
				if(adjacentNode == null){
					continue;
				}
				
				decrementResultMap(nodeCount,resultMap);
				nodeCount++;
				incrementResultMap(nodeCount, resultMap);
				
				if(!nodeToInt.containsKey(adjacentNode)){
					System.out.println("Current Node:"+currentNode+" Node added: "+adjacentNode);
					nodeQueue.offer(adjacentNode);
					nodeToInt.put(adjacentNode, 1);
					incrementResultMap(1, resultMap);
				}else{
					count = nodeToInt.get(adjacentNode);
					decrementResultMap(count,resultMap);
					count++;
					incrementResultMap(count, resultMap);
					nodeToInt.put(adjacentNode,count);
				}

				
			}
			nodeToInt.put(currentNode, nodeCount);
			System.out.println(resultMap);
		}



		return resultMap;
	}

	public static void decrementResultMap(int count, Map<Integer,Integer> resultMap){
		int resultCount;
		if(resultMap.containsKey(count)){
			resultCount = resultMap.get(count);
			resultCount--;
			resultMap.put(count,resultCount);
			System.out.println("Decrementing "+count+" new val:"+resultCount);

		}
	}

	public static void incrementResultMap(int count, Map<Integer,Integer> resultMap){
		int resultCount;
		if(resultMap.containsKey(count)){
			resultCount = resultMap.get(count);
			resultCount++;
			resultMap.put(count,resultCount);
			System.out.println("Incrementing "+count+" new val:"+resultCount);

		}else{
			resultMap.put(count, 1);
			System.out.println("Adding "+count+" new val:"+1);

		}

	}
	/*private static Map<Integer, Integer> calculateDegreeCount(Node node) {
		// YOUR CODE HERE
		Map<Integer, Integer> map = new HashMap<Integer,Integer>();
		ArrayList<Node> list = new ArrayList<Node>();
		Map<Node, Integer> visited = new HashMap<Node, Integer>();

		Iterator<Node> iter = list.iterator();
		list.add(node);

		Node newNode;
		while(iter.hasNext()){
			newNode = (Node)iter.next();
			if(!visited.containsKey(newNode)){
				visited.put(newNode,1);
			}
			for(Node n : newNode.neighbors){
				if(!visited.containsKey(n))
					list.add(n);
			}

			int degree = newNode.neighbors.size();
			if(map.containsKey(degree)){
				int count = map.get(degree);
				count++;
				map.put(degree, count);
			}else{
				map.put(degree,1);
			}
		}
		return map;
	}*/

	public static void main(String[] args) {
		String line;
		Scanner stdin = new Scanner(System.in);
		Map<Integer, Node> graph = new HashMap<>();
		Node firstNode = null;
		int iCount = 0;
		//while(stdin.hasNextLine() && !( line = stdin.nextLine() ).equals( "" ))
		while(iCount<5 && !( line = stdin.nextLine() ).equals( "" ))
		{iCount++;
		String[] tokens = line.split(" ");
		// Edge list can only be one or two nodes per line.
		if (tokens.length != 1 && tokens.length != 2) {
			throw new RuntimeException("Unknown format");
		}

		for (int i = 0; i < tokens.length; i++) {
			int id = Integer.parseInt(tokens[i]);
			if (!graph.containsKey(id)) {
				graph.put(id, new Node());
			}
		}

		Node node0 = graph.get(Integer.parseInt(tokens[0]));
		if (firstNode == null) {
			firstNode = node0;
		}

		if (tokens.length == 2) {
			Node node1 = graph.get(Integer.parseInt(tokens[1]));

			node0.neighbors.add(node1);
			node1.neighbors.add(node0);
		}

		}
		stdin.close();

		TreeMap treeMap = new TreeMap(calculateDegreeCount(firstNode));
		System.out.println(treeMap);
	}
}