package org.notebook.cache;

import java.util.Collection;
import java.util.Comparator;
import java.util.SortedSet;

public abstract class Comparators {
	public final static Comparator<Category> createDateComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2)) 
				return 0;
			else {
				return o1.createDate.getTime() > o2.createDate.getTime() ? 1 : -1;
			}
		}
	};
	
	public final static Comparator<Category> updateDateComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2)) 
				return 0;
			else {
				return o1.lastUpdated.getTime() > o2.lastUpdated.getTime() ? 1 : -1;
			}
		}
	};
	
	public final static Comparator<Category> nameComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2)) 
				return 0;
			else {
				int r = o1.name.compareTo(o2.name);
				return r != 0 ? r : 1;
			}
		}
	};
	
	public final static Comparator<Category> keyComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2))
				return 0;
			else {
				int r = o1.order - o2.order;
				return r != 0 ? r : 1;
			}
		}
	};	
	
	public static Comparator<Category> reverse(Comparator<Category> c){
		return new ReverseComparator(c);
		
	}
	
	public static SortedSet<Category> sort(Collection<Category> list, String orderBy){
		return null;
	}
	
	static class ReverseComparator implements Comparator<Category>{
		public Comparator<Category> proxy = null;
		public ReverseComparator(Comparator<Category> c){
			this.proxy = c;
		}
		
		@Override
		public int compare(Category o1, Category o2) {
			return proxy.compare(o1, o2) * -1;
		}
		
		public boolean equals(Object c){
			if(c instanceof ReverseComparator){
				return ((ReverseComparator)c).proxy.equals(this.proxy);
			}
			return false;			
		}
	}
	
	public Comparator<Category> getByName(String name){
		Comparator<Category> cmp = createDateComparator;
		if(name.startsWith("createDate")){
			cmp = createDateComparator;
		}else if(name.startsWith("updateDate")){
			cmp = updateDateComparator;
		}else if(name.startsWith("name")){
			cmp = nameComparator;
		}else if(name.startsWith("key")){
			cmp = keyComparator;
		}
		if(name.endsWith("-")){
			cmp = reverse(cmp);
		}
		return cmp;
	}
	
	
}
