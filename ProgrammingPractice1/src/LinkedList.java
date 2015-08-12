/*
	Isidro Gomez - July 25, 2015
	Doubly Connected Linked List
*/

public class LinkedList<E> 
{
	//Node class for the DCEL
	public class Node<T>
	{
		T data;
		Node<T> next = null;
		Node<T> prev = null;
		public Node(T data, Node<T> prev, Node<T> next)
		{
			this.data = data;
			this.next = next;
			this.prev = prev;
		}
		public Node(T data)
		{
			this.data = data;
		}
		
	}
	
	Node<E> head, tail;
	int size;
	
	public LinkedList()
	{
		head = null;
		tail = head;
		size = 0;
	}
	
	public void add(E newData)
	{
		if(size==0)
		{
			head = new Node<E>(newData, null, null);
			tail = head;
		}
		else
			addToEnd(newData);
				
		size++;
	}
	
	public void add(E newData, int location)
	{
		//if the add wants to be added further than n + 1
		//simply don't add the item and move on.
		if(location > size + 1)
			return;
		//if the data added is in the beginning call the 
		//helper function addToFront
		else if(location - 1 == 0)
			addToFront(newData); 
		//if the data wants to be added to very end
		//call the helper function addToEnd
		else if(location == size + 1)
			addToEnd(newData);
		else
		{
			//by having a head and a tail you can optimize a bit by never having to iterate 
			//more than n/2 objects when adding simply by checking if there the location is 
			//in the first or second half of the list
			if(location < size / 2)
			{
				Node<E> temp = head;
				for(int x = 1; x < location - 1; x++)
				{
					temp = temp.next;
				}
				Node<E> newNode = new Node<E>(newData, temp, temp.next);					//Overall O(n/2)
				newNode.prev.next = newNode;												//but still considered O(n) to 
			}																				//add into the list
			else
			{
				Node<E> temp = tail;
				for(int x = size - 1; x > location - 1; x--)
				{
					temp = temp.prev;
				}
				Node<E> newNode = new Node<E>(newData, temp, temp.next);
				newNode.prev.next = newNode;
			}
		}
		
		size++;
	}
	
	//Helper method to add to front
	//O(1) runtime
	private void addToFront(E newData)
	{
		head = new Node<E>(newData, null, head);
	
	}
	
	//Helper method to add to the end
	//O(1) runtime since we always keep track of the end
	//its an improvement from the O(n) runtime of adding 
	//to the end of a single connected linked list
	private void addToEnd(E newData)
	{
		Node<E> newNode = new Node<E>(newData, tail, null);
		newNode.prev.next = newNode;
		tail = newNode;
	}
	
	
	//O(n) worst case for checking if an item exists
	//in a DCEL
	public boolean exists(E data)
	{
		for(Node<E> temp = head; temp != null; temp = temp.next)   
			if(temp.data.toString().equals(data.toString()))
				return true;
		return false;
	}
	
	
	
	public E remove(E data)
	{
		if(exists(data))
		{
			
			E dataRemoved = null;
			if(head.data.toString().equals(data.toString()))
			{
				
				dataRemoved = head.data;
				head = head.next;
				
			}
			else if(tail.data.toString().equals(data.toString()))
			{
				
				dataRemoved = tail.data;
			    tail = tail.prev;
				tail.next = null;
			}
			else
			{
			
				Node<E> temp = head;
				while(!temp.data.toString().equals(data.toString()))
				{
					
					temp = temp.next;
				}
				dataRemoved = temp.data;
				temp.prev.next = temp.next;
				temp.next.prev = temp.prev;
				
			}
			size--;
			return dataRemoved;
		}
		return null;
	}
	
	public E removeAt (int location)
	{
		E dataRemoved = null;
		if(location == 1)
		{
			dataRemoved = head.data;
			head = head.next;
		}
		else if(location == size)
		{
			dataRemoved = tail.data;
			tail = tail.prev;
			tail.next = null;
		}
		else if(location > 1 && location < size)
		{
			
			Node<E> temp = head;
			for(int x = 1; x < location; x++)
				temp = temp.next;
			
			dataRemoved = temp.data;
			temp.prev.next = temp.next;
			temp.next.prev = temp.prev;
		}
		
		size--;
		return dataRemoved;
	}
	
	public E get(int location)
	{
		Node<E> temp = head;
		for(int x = 0; x < location; x++)
			temp = temp.next;
		return temp.data;
	}
	
	public E set(int location, E newData)
	{
		Node<E> temp = head;
		E changedValue;
		for(int x = 1; x < location; x++)
			temp = temp.next;
		changedValue = temp.data;
		temp.data = newData;
		return changedValue;
	}
	
	public String toString()
	{
		String ss = "" + head.data;
		for(Node<E> temp = head.next; temp != null; temp = temp.next)
		{
			ss += "<->" + temp.data;
		}
		return ss;
	}
}
