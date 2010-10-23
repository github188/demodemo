package org.notebook.cache;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.List;
import java.util.TreeSet;

public abstract class Comparators {
	public static List<Category> sort(Collection<Category> list, String orderBy){
		TreeSet<Category> dataList = new TreeSet<Category>(getByName(orderBy));
		dataList.addAll(list);
		return new ArrayList<Category>(dataList);
	}
	
	public static Comparator<Category> getByName(String name){
		Comparator<Category> cmp = keyComparator;
		if(name == null || "".equals(name.trim())){
			name = "position";
		}else if(name.endsWith("create_date")){
			cmp = createDateComparator;
		}else if(name.endsWith("update_date")){
			cmp = updateDateComparator;
		}else if(name.endsWith("name")){
			cmp = nameComparator;
		}else if(name.endsWith("position")){
			cmp = keyComparator;
		}
		if(name.charAt(0) == '-'){
			cmp = new ReverseComparator(cmp);
		}
		return cmp;
	}
	
	public static char shortName(String name){
		char n = 'P';
		if(name == null || "".equals(name.trim())){
			name = "P";
		}else if(name.endsWith("create_date")){
			n = 'C';
		}else if(name.endsWith("update_date")){
			n = 'U';
		}else if(name.endsWith("name")){
			n = 'N';
		}else if(name.endsWith("position")){
			n = 'P';
		}
		if(name.charAt(0) == '-'){
			n = (char)((int)n + (int)'a' - (int)'A');
		}
		return n;
	}
	
	private final static Comparator<Category> createDateComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2)) 
				return 0;
			else {
				return o1.createDate.getTime() > o2.createDate.getTime() ? 1 : -1;
			}
		}
	};
	
	private final static Comparator<Category> updateDateComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2)) 
				return 0;
			else {
				return o1.lastUpdated.getTime() > o2.lastUpdated.getTime() ? 1 : -1;
			}
		}
	};
	
	private final static Comparator<Category> nameComparator = new Comparator<Category>(){
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
	
	private final static Comparator<Category> keyComparator = new Comparator<Category>(){
		@Override
		public int compare(Category o1, Category o2) {
			if(o1.equals(o2))
				return 0;
			else {
				int r = o1.position - o2.position;
				return r != 0 ? r : 1;
			}
		}
	};	
	
	private static class ReverseComparator implements Comparator<Category>{
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
}
