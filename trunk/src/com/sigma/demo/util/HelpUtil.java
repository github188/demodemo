package com.sigma.demo.util;

public class HelpUtil {
	
	public static long parseTime(String period){
		if(period == null || "".equals(period.trim())){
			return -1;
		}
		long pt = 0;
		try{
			period = period.trim();
			pt = Long.parseLong(period.replaceAll("\\D+", ""));
		} catch(Exception e){
			pt = 0;
		}
		if(period.endsWith("s")){
			pt *= 1000;
		}
		
		return pt;
	}
	
	public static String stringTime(long time, String unit){
		String re = null;
		unit = (unit == null) ? "ms" : unit;
		if(time == -1){
			return "0" +unit;
		}
		if(unit.equals("ms")){
			re = time + unit;
		}else if(unit.equals("s")) {
			re = (time / 1000) + unit;
		}
		
		return re;
	}

	public static int addNumberString(String number, int i){
		int data = parseInteger(number);
		if(data < 0) data = 0;
		return data + i;
	}

	
	public static int parseInteger(String _int){
		
		if(_int == null || "".equals(_int.trim())) return -1;
		if(_int.matches("\\d+")){
			try{
				return Integer.parseInt(_int.trim());
			}catch(Exception e){
				return -1;
			}
		}
		
		return -1;
	}
	
	public static boolean eq(String a, String b) {
		if ((a == null && b == null) || (a != null && b != null && a.equals(b))) {
			return true;
		} else {
			return false;
		}
	}
	
	public static String join(String flag, String[] list) {
		StringBuffer buf = new StringBuffer();
		if(list != null) {
			int i = 0;
			for(; i < list.length - 1; i++) {
				buf.append(list[i]);
				buf.append(flag);
			}
			buf.append(list[i]);
		}
		return buf.toString();
	}
}
