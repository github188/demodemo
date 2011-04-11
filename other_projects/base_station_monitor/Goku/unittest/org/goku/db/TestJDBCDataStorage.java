package org.goku.db;

import static org.junit.Assert.assertEquals;

import org.goku.TestUtils;
import org.goku.settings.Settings;
import org.junit.Before;
import org.junit.Test;

public class TestJDBCDataStorage {
	private JDBCDataStorage testObj = null;
	
	@Test
	public void test_toOrderBy(){		
		assertEquals(invokeToOrderBy(testObj, "name"), "name");
		assertEquals(invokeToOrderBy(testObj, "+name"), "name asc");
		
		assertEquals(invokeToOrderBy(testObj, "name,age"), "name,age");
		assertEquals(invokeToOrderBy(testObj, "name,+age"), "name,age asc");
		assertEquals(invokeToOrderBy(testObj, "+name,+age"), "name asc,age asc");
		
		assertEquals(invokeToOrderBy(testObj, "name,-age"), "name,age desc");
	}
	
	@Before
	public void initTestObject(){
		Settings s = new Settings("test.conf");
		testObj = new JDBCDataStorage(s);
	}

	private String invokeToOrderBy(Object obj, String param){
		return (String)TestUtils.invokePrivateMethod(obj, "toOrderBy", new String[]{param});		
	}
}
