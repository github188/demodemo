# Details #
```
<session-factory>
      <property name="hibernate.connection.driver_class">org.hsqldb.jdbcDriver</property>
      <property name="hibernate.connection.url">jdbc:hsqldb:file:sip_mydb</property>
      <property name="hibernate.connection.username">sa</property>
      <property name="hibernate.connection.password"></property>
      <property name="hibernate.connection.pool_size">10</property>
      <property name="show_sql">true</property>
      <property name="dialect">org.hibernate.dialect.HSQLDialect</property>
      <property name="hibernate.query.factory_class">org.hibernate.hql.classic.ClassicQueryTranslatorFactory</property> 
      <property name="hibernate.hbm2ddl.auto">create</property>
      <!-- Mapping files -->
      <mapping resource="Registration.hbm.xml"/>
</session-factory>
```

如果用户名不是sa
```
Exception in thread "main" java.sql.SQLException: User not found: SAA
	at org.hsqldb.jdbc.Util.sqlException(Unknown Source)
	at org.hsqldb.jdbc.jdbcConnection.<init>(Unknown Source)
	at org.hsqldb.jdbcDriver.getConnection(Unknown Source)
	at org.hsqldb.jdbcDriver.connect(Unknown Source)
	at java.sql.DriverManager.getConnection(Unknown Source)
	at java.sql.DriverManager.getConnection(Unknown Source)
	at org.robot.log.DBTest.main(DBTest.java:14)
```