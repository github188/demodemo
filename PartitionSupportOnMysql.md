1. check partition available
mysql> SHOW VARIABLES LIKE '%partition%';
+-------------------+-------+
| Variable\_name     | Value |
+-------------------+-------+
| have\_partitioning | YES   |
+-------------------+-------+
mysql> SHOW PLUGINS;
+------------+----------+----------------+---------+---------+
| Name       | Status   | Type           | Library | License |
+------------+----------+----------------+---------+---------+
| binlog     | ACTIVE   | STORAGE ENGINE | NULL    | GPL     |
| partition  | ACTIVE   | STORAGE ENGINE | NULL    | GPL     |

2. re-build partition.

ALTER TABLE tbl\_name
> REORGANIZE PARTITION partition\_list
> INTO (partition\_definitions);

explain select count(**) from rpt\_test\_case\_execution where build\_release\_name="A10";**

+----+-------------+-------------------------+------+--------------------+--------------------+---------+-------+-------+--------------------------+
| id | select\_type | table                   | type | possible\_keys      | key                | key\_len | ref   | rows  | Extra                    |
+----+-------------+-------------------------+------+--------------------+--------------------+---------+-------+-------+--------------------------+
|  1 | SIMPLE      | rpt\_test\_case\_execution | ref  | build\_release\_name | build\_release\_name | 13      | const | 32406 | Using where; Using index |
+----+-------------+-------------------------+------+--------------------+--------------------+---------+-------+-------+--------------------------+
1 row in set (0.00 sec)

### Convert the release name to integer. the partition expression only support integer.
drop function if exists release\_partition;
delimiter 
create function release\_partition(s varchar(10))
returns integer
begin
> declare r integer;
> SELECT FIND\_IN\_SET(s,'A10,A11,A11TOP,A10TOP,A6,A8,A9,Q3') into r;
> return r;
end
delimiter;

### check the release\_partition
select release\_partition('A10');

-- unknown release partition number is 0

alter table rpt\_test\_case\_execution ADD p\_num integer(12) default 0;
update rpt\_test\_case\_execution set p\_num = release\_partition(build\_release\_name)



ERROR 1064 (42000): Constant/Random expression in (sub)partitioning function is not allowed near
ERROR 1491 (HY000): A PRIMARY KEY must include all columns in the table's partitioning function
---http://dev.mysql.com/doc/refman/5.1/en/partitioning-limitations-partitioning-keys-unique-keys.html

### resolve ERROR 1491
ALTER TABLE rpt\_test\_case\_execution ADD PRIMARY KEY(p\_num, external\_id);
show index from rpt\_test\_case\_execution;
DROP INDEX external\_id ON rpt\_test\_case\_execution;

select substring(max(concat(aa.log\_execution\_datetime, aa.external\_id)), 27) as external\_id
from exchange\_table\_all as aa
where aa.build\_release\_name='A10'
group by aa.test\_case\_code, aa.build\_name, aa.log\_analysis\_result;

explain select substring(max(concat(aa.log\_execution\_datetime, aa.external\_id)), 27) as external\_id
from rpt\_test\_case\_execution as aa
where aa.p\_num=1
group by aa.test\_case\_code, aa.build\_name, aa.log\_analysis\_result;

27309 rows in set (0.41 sec)

ALTER TABLE rpt\_test\_case\_execution add index (external\_id);

ALTER TABLE rpt\_test\_case\_execution ANALYZE PARTITION a10;
ALTER TABLE rpt\_test\_case\_execution PARTITION BY RANGE(length(build\_release\_name))
(
> PARTITION unknown VALUES LESS THAN (1),
> PARTITION a10 VALUES LESS THAN (2),
> PARTITION a11 VALUES LESS THAN (3),
> PARTITION a11Top VALUES LESS THAN (4),
> PARTITION others VALUES LESS THAN MAXVALUE
);
ALTER TABLE rpt\_test\_case\_execution REBUILD PARTITION a10; , p1;

explain select count(**) from rpt\_test\_case\_execution where p\_num<2;**


---

2. ##############
drop table if exists rpt\_test\_case\_execution\_temp;
create table rpt\_test\_case\_execution\_temp as
select b.external\_id, b.build\_release\_name, b.build\_name, b.build\_rpt\_build\_type, b.log\_env\_code, b.log\_phase\_name, b.log\_req\_area,
> b.log\_dev\_area, b.log\_team\_name, b.log\_execution\_type, b.test\_case\_code, b.log\_analysis\_result, b.log\_execution\_datetime
from exchange\_table\_all as b;
