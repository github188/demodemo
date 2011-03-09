-- MySQL dump 10.13  Distrib 5.1.41, for debian-linux-gnu (i486)
--
-- Host: localhost    Database: goku
-- ------------------------------------------------------
-- Server version	5.1.41-3ubuntu12.7

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `alarm_code_list`
--

DROP TABLE IF EXISTS `alarm_code_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alarm_code_list` (
  `alarmCode` varchar(32) COLLATE utf8_bin NOT NULL,
  `alarmName` varchar(50) COLLATE utf8_bin NOT NULL,
  `alarmLevel` varchar(10) COLLATE utf8_bin NOT NULL,
  `alarmCategory` varchar(4) COLLATE utf8_bin NOT NULL,
  `alarmStatus` varchar(4) COLLATE utf8_bin NOT NULL,
  `reActiveTime` int(11) NOT NULL,
  `alarmDesc` longtext COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`alarmCode`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alarm_code_list`
--

LOCK TABLES `alarm_code_list` WRITE;
/*!40000 ALTER TABLE `alarm_code_list` DISABLE KEYS */;
INSERT INTO `alarm_code_list` VALUES ('1003','动态检测','1','1','2',5,'全局告警-所有设备都启用; 策略告警-需要设备定义后生效; 禁用告警-在所有设备中禁用.'),('1004','硬盘丢失','1','3','2',1440,'全局告警-所有设备都启用; 策略告警-需要设备定义后生效; 禁用告警-在所有设备中禁用.'),('1001','外部报警','1','1','1',5,''),('1002','视频丢失','1','3','1',5,''),('2002','认证错误','1','3','1',5,''),('2003','时间和服务器不同步','1','3','1',5,''),('2001','连接超时','1','3','1',5,''),('4001','刷新基站列表','1','3','1',5,'');
/*!40000 ALTER TABLE `alarm_code_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `alarm_record`
--

DROP TABLE IF EXISTS `alarm_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alarm_record` (
  `uuid` varchar(32) COLLATE utf8_bin NOT NULL,
  `baseStation` varchar(10) COLLATE utf8_bin NOT NULL,
  `channelId` varchar(10) COLLATE utf8_bin NOT NULL,
  `alarmCode` varchar(32) COLLATE utf8_bin NOT NULL,
  `alarmLevel` varchar(10) COLLATE utf8_bin NOT NULL,
  `alarmCategory` varchar(10) COLLATE utf8_bin NOT NULL,
  `alarmStatus` varchar(10) COLLATE utf8_bin NOT NULL,
  `user` varchar(20) COLLATE utf8_bin DEFAULT NULL,
  `videoPath` varchar(1024) COLLATE utf8_bin DEFAULT NULL,
  `startTime` datetime DEFAULT NULL,
  `endTime` datetime DEFAULT NULL,
  `lastUpdateTime` datetime DEFAULT NULL,
  `comfirmTime` datetime DEFAULT NULL,
  PRIMARY KEY (`uuid`),
  KEY `alarm_record_baseStation` (`baseStation`),
  KEY `alarm_record_alarmCode` (`alarmCode`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alarm_record`
--

LOCK TABLES `alarm_record` WRITE;
/*!40000 ALTER TABLE `alarm_record` DISABLE KEYS */;
INSERT INTO `alarm_record` VALUES ('100206676864100','1002','0','2001','1','3','1','',NULL,'2011-03-09 02:41:04',NULL,'2011-03-09 02:41:04',NULL),('100106676864265','1001','0','2001','1','3','1','',NULL,'2011-03-09 02:41:04',NULL,'2011-03-09 02:41:04',NULL),('100206677305496','1002','0','2001','1','3','1','',NULL,'2011-03-09 02:48:25',NULL,'2011-03-09 02:48:25',NULL),('100106677305565','1001','0','2001','1','3','1','',NULL,'2011-03-09 02:48:25',NULL,'2011-03-09 02:48:25',NULL),('100106683231033','1001','0','2001','1','3','1','',NULL,'2011-03-09 04:27:10',NULL,'2011-03-09 04:27:10',NULL),('100206683231077','1002','0','2001','1','3','1','',NULL,'2011-03-09 04:27:11',NULL,'2011-03-09 04:27:11',NULL),('100106683799346','1001','0','2001','1','3','1','',NULL,'2011-03-09 04:36:39',NULL,'2011-03-09 04:36:39',NULL),('100206683799449','1002','0','2001','1','3','1','',NULL,'2011-03-09 04:36:39',NULL,'2011-03-09 04:36:39',NULL);
/*!40000 ALTER TABLE `alarm_record` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_group`
--

DROP TABLE IF EXISTS `auth_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(80) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_group`
--

LOCK TABLES `auth_group` WRITE;
/*!40000 ALTER TABLE `auth_group` DISABLE KEYS */;
INSERT INTO `auth_group` VALUES (1,'monitor_admin');
/*!40000 ALTER TABLE `auth_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_group_permissions`
--

DROP TABLE IF EXISTS `auth_group_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_group_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `group_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `group_id` (`group_id`,`permission_id`)
) ENGINE=MyISAM AUTO_INCREMENT=61 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_group_permissions`
--

LOCK TABLES `auth_group_permissions` WRITE;
/*!40000 ALTER TABLE `auth_group_permissions` DISABLE KEYS */;
INSERT INTO `auth_group_permissions` VALUES (54,1,58),(53,1,52),(52,1,57),(51,1,37),(50,1,51),(49,1,59),(48,1,50),(47,1,49),(46,1,33),(45,1,47),(44,1,53),(43,1,48),(42,1,27),(57,1,38),(41,1,36),(40,1,34),(60,1,39),(39,1,56),(38,1,26),(37,1,41),(36,1,40),(35,1,25),(59,1,54),(34,1,42),(33,1,55),(32,1,31),(31,1,46),(56,1,35),(55,1,60),(58,1,32);
/*!40000 ALTER TABLE `auth_group_permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_message`
--

DROP TABLE IF EXISTS `auth_message`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_message` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `message` longtext COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  KEY `auth_message_user_id` (`user_id`)
) ENGINE=MyISAM AUTO_INCREMENT=66 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_message`
--

LOCK TABLES `auth_message` WRITE;
/*!40000 ALTER TABLE `auth_message` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_message` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_permission`
--

DROP TABLE IF EXISTS `auth_permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_permission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) COLLATE utf8_bin NOT NULL,
  `content_type_id` int(11) NOT NULL,
  `codename` varchar(100) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `content_type_id` (`content_type_id`,`codename`),
  KEY `auth_permission_content_type_id` (`content_type_id`)
) ENGINE=MyISAM AUTO_INCREMENT=64 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_permission`
--

LOCK TABLES `auth_permission` WRITE;
/*!40000 ALTER TABLE `auth_permission` DISABLE KEYS */;
INSERT INTO `auth_permission` VALUES (1,'Can add permission',1,'add_permission'),(2,'Can change permission',1,'change_permission'),(3,'Can delete permission',1,'delete_permission'),(4,'Can add group',2,'add_group'),(5,'Can change group',2,'change_group'),(6,'Can delete group',2,'delete_group'),(7,'Can add user',3,'add_user'),(8,'Can change user',3,'change_user'),(9,'Can delete user',3,'delete_user'),(10,'Can add message',4,'add_message'),(11,'Can change message',4,'change_message'),(12,'Can delete message',4,'delete_message'),(13,'Can add content type',5,'add_contenttype'),(14,'Can change content type',5,'change_contenttype'),(15,'Can delete content type',5,'delete_contenttype'),(16,'Can add session',6,'add_session'),(17,'Can change session',6,'change_session'),(18,'Can delete session',6,'delete_session'),(19,'Can add site',7,'add_site'),(20,'Can change site',7,'change_site'),(21,'Can delete site',7,'delete_site'),(22,'Can add log entry',8,'add_logentry'),(23,'Can change log entry',8,'change_logentry'),(24,'Can delete log entry',8,'delete_logentry'),(25,'Can add base station',9,'add_basestation'),(26,'Can change base station',9,'change_basestation'),(27,'Can delete base station',9,'delete_basestation'),(60,'Can delete bts category',20,'delete_btscategory'),(59,'Can change bts category',20,'change_btscategory'),(58,'Can add bts category',20,'add_btscategory'),(31,'Can add user',11,'add_user'),(32,'Can change user',11,'change_user'),(33,'Can delete user',11,'delete_user'),(34,'Can add user group',12,'add_usergroup'),(35,'Can change user group',12,'change_usergroup'),(36,'Can delete user group',12,'delete_usergroup'),(37,'Can add user group relation',13,'add_usergrouprelation'),(38,'Can change user group relation',13,'change_usergrouprelation'),(39,'Can delete user group relation',13,'delete_usergrouprelation'),(40,'Can add station group relation',14,'add_stationgrouprelation'),(41,'Can change station group relation',14,'change_stationgrouprelation'),(42,'Can delete station group relation',14,'delete_stationgrouprelation'),(57,'Can delete alarm define',19,'delete_alarmdefine'),(56,'Can change alarm define',19,'change_alarmdefine'),(55,'Can add alarm define',19,'add_alarmdefine'),(46,'Can add alarm record',16,'add_alarmrecord'),(47,'Can change alarm record',16,'change_alarmrecord'),(48,'Can delete alarm record',16,'delete_alarmrecord'),(49,'Can add system log',17,'add_systemlog'),(50,'Can change system log',17,'change_systemlog'),(51,'Can delete system log',17,'delete_systemlog'),(52,'Can add location',18,'add_location'),(53,'Can change location',18,'change_location'),(54,'Can delete location',18,'delete_location'),(61,'Can add 计划任务',21,'add_videotask'),(62,'Can change 计划任务',21,'change_videotask'),(63,'Can delete 计划任务',21,'delete_videotask');
/*!40000 ALTER TABLE `auth_permission` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_user`
--

DROP TABLE IF EXISTS `auth_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) COLLATE utf8_bin NOT NULL,
  `first_name` varchar(30) COLLATE utf8_bin NOT NULL,
  `last_name` varchar(30) COLLATE utf8_bin NOT NULL,
  `email` varchar(75) COLLATE utf8_bin NOT NULL,
  `password` varchar(128) COLLATE utf8_bin NOT NULL,
  `is_staff` tinyint(1) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `is_superuser` tinyint(1) NOT NULL,
  `last_login` datetime NOT NULL,
  `date_joined` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_user`
--

LOCK TABLES `auth_user` WRITE;
/*!40000 ALTER TABLE `auth_user` DISABLE KEYS */;
INSERT INTO `auth_user` VALUES (1,'admin','','','admin@nsn.com','sha1$38454$54eec228fa356d5813fb30817f3d35ee7ff56f48',1,1,1,'2011-03-06 15:06:01','2011-03-01 15:10:55'),(2,'dgm','','','','sha1$31755$a9f351122287b6c96ccffa24eac7b8df61a6c3a6',1,1,0,'2011-03-04 22:04:00','2011-03-02 13:29:33');
/*!40000 ALTER TABLE `auth_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_user_groups`
--

DROP TABLE IF EXISTS `auth_user_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user_groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`group_id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_user_groups`
--

LOCK TABLES `auth_user_groups` WRITE;
/*!40000 ALTER TABLE `auth_user_groups` DISABLE KEYS */;
INSERT INTO `auth_user_groups` VALUES (1,2,1);
/*!40000 ALTER TABLE `auth_user_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_user_user_permissions`
--

DROP TABLE IF EXISTS `auth_user_user_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user_user_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`permission_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_user_user_permissions`
--

LOCK TABLES `auth_user_user_permissions` WRITE;
/*!40000 ALTER TABLE `auth_user_user_permissions` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_user_user_permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `base_station`
--

DROP TABLE IF EXISTS `base_station`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `base_station` (
  `uuid` varchar(10) COLLATE utf8_bin NOT NULL,
  `name` varchar(50) COLLATE utf8_bin NOT NULL,
  `connectionStatus` varchar(10) COLLATE utf8_bin NOT NULL,
  `groupName` varchar(50) COLLATE utf8_bin NOT NULL,
  `routeServer` varchar(50) COLLATE utf8_bin DEFAULT NULL,
  `locationId` varchar(50) COLLATE utf8_bin NOT NULL,
  `alarmStatus` varchar(50) COLLATE utf8_bin DEFAULT NULL,
  `channels` varchar(150) COLLATE utf8_bin DEFAULT NULL,
  `devType` int(11) NOT NULL,
  `lastActive` datetime DEFAULT NULL,
  `lastUpdate` datetime NOT NULL,
  `createDate` datetime NOT NULL,
  `locationUUID` varchar(32) COLLATE utf8_bin NOT NULL,
  `btsCategory` varchar(20) COLLATE utf8_bin NOT NULL,
  `supportAlarm` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`uuid`),
  KEY `base_station_locationUUID` (`locationUUID`),
  KEY `base_station_btsCategory` (`btsCategory`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `base_station`
--

LOCK TABLES `base_station` WRITE;
/*!40000 ALTER TABLE `base_station` DISABLE KEYS */;
INSERT INTO `base_station` VALUES ('1001','信诚路','timeout','default',NULL,'192.168.1.1:9001',NULL,'1:通道1,2:通道2',1,'2011-03-09 04:36:39','2011-03-09 10:23:58','2011-03-02 15:07:57','001','33','[]'),('1002','test','timeout','default',NULL,'192.168.1.1:9002',NULL,'1:通道1,2:通道2',1,'2011-03-09 04:36:39','2011-03-09 10:38:50','2011-03-09 10:32:07','001','33','[]');
/*!40000 ALTER TABLE `base_station` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bts_category_code`
--

DROP TABLE IF EXISTS `bts_category_code`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bts_category_code` (
  `uuid` varchar(20) COLLATE utf8_bin NOT NULL,
  `name` varchar(50) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`uuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bts_category_code`
--

LOCK TABLES `bts_category_code` WRITE;
/*!40000 ALTER TABLE `bts_category_code` DISABLE KEYS */;
INSERT INTO `bts_category_code` VALUES ('33','33');
/*!40000 ALTER TABLE `bts_category_code` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_admin_log`
--

DROP TABLE IF EXISTS `django_admin_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_admin_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `action_time` datetime NOT NULL,
  `user_id` int(11) NOT NULL,
  `content_type_id` int(11) DEFAULT NULL,
  `object_id` longtext COLLATE utf8_bin,
  `object_repr` varchar(200) COLLATE utf8_bin NOT NULL,
  `action_flag` smallint(5) unsigned NOT NULL,
  `change_message` longtext COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  KEY `django_admin_log_user_id` (`user_id`),
  KEY `django_admin_log_content_type_id` (`content_type_id`)
) ENGINE=MyISAM AUTO_INCREMENT=74 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_admin_log`
--

LOCK TABLES `django_admin_log` WRITE;
/*!40000 ALTER TABLE `django_admin_log` DISABLE KEYS */;
INSERT INTO `django_admin_log` VALUES (11,'2011-03-02 14:21:35',2,19,'001','中文2<001>',3,''),(10,'2011-03-02 14:21:27',2,19,'33','中文<33>',3,''),(9,'2011-03-02 14:09:22',1,2,'1','monitor_admin',2,'已修改 permissions 。'),(5,'2011-03-01 15:19:43',1,18,'001','浙江<001>',1,''),(6,'2011-03-02 13:29:18',1,2,'1','monitor_admin',1,''),(7,'2011-03-02 13:29:33',1,3,'2','dgm',1,''),(8,'2011-03-02 13:30:11',1,3,'2','dgm',2,'已修改 is_staff 和 groups 。'),(12,'2011-03-02 14:25:23',2,19,'4001','刷新基站列表<4001>',3,''),(13,'2011-03-02 14:25:23',2,19,'2003','时间和服务器不同步<2003>',3,''),(14,'2011-03-02 14:25:23',2,19,'2002','认证错误<2002>',3,''),(15,'2011-03-02 14:25:23',2,19,'2001','连接超时<2001>',3,''),(16,'2011-03-02 14:25:23',2,19,'1004','动态检测<1004>',3,''),(17,'2011-03-02 14:25:23',2,19,'1003','动态检测<1003>',3,''),(18,'2011-03-02 14:25:23',2,19,'1002','视频丢失<1002>',3,''),(19,'2011-03-02 14:25:23',2,19,'1001','外部报警<1001>',3,''),(20,'2011-03-02 14:30:28',2,17,'','11-1',1,''),(21,'2011-03-02 14:57:41',2,12,'11','11',1,''),(22,'2011-03-02 14:57:59',2,11,'22','22',1,''),(23,'2011-03-02 14:58:01',2,13,'1','UserGroupRelation object',1,''),(24,'2011-03-02 15:00:12',2,11,'22','22',2,'已修改 status 。'),(25,'2011-03-02 15:00:58',2,18,'22','22<22>',1,''),(26,'2011-03-02 15:01:15',2,20,'33','33<33>',1,''),(27,'2011-03-02 15:07:57',2,9,'1001','1001',1,''),(28,'2011-03-02 15:12:14',2,16,'','1001-刷新基站列表<4001>',1,''),(29,'2011-03-02 15:15:30',2,16,'','1001-刷新基站列表<4001>',1,''),(30,'2011-03-02 15:18:14',2,16,'1','1001-刷新基站列表<4001>',1,''),(31,'2011-03-02 15:19:10',2,14,'1','StationGroupRelation object',1,''),(32,'2011-03-04 22:05:02',2,19,'2003','时间和服务器不同步<2003>',2,'已修改 alarmLevel 和 alarmCategory 。'),(33,'2011-03-06 15:11:13',1,21,'1','33<3>',1,''),(34,'2011-03-06 15:12:45',1,21,'1','33<3>',2,'已修改 userName 。'),(35,'2011-03-06 17:42:29',2,9,'1001','1001',2,'已修改 channels 和 supportAlarm 。'),(36,'2011-03-06 17:42:44',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(37,'2011-03-06 17:47:32',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(38,'2011-03-06 17:52:09',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(39,'2011-03-06 17:58:00',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(40,'2011-03-06 18:02:19',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(41,'2011-03-06 18:06:58',2,9,'1001','1001',2,'没有字段被修改。'),(42,'2011-03-06 18:07:10',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(43,'2011-03-06 18:18:09',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(44,'2011-03-06 18:55:02',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(45,'2011-03-06 18:55:16',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(46,'2011-03-06 18:59:18',2,9,'1001','1001',2,'已修改 supportAlarms 。'),(47,'2011-03-07 08:32:53',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(48,'2011-03-07 08:39:46',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(49,'2011-03-07 08:46:57',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(50,'2011-03-07 08:58:43',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(51,'2011-03-07 09:06:18',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(52,'2011-03-07 09:12:07',2,9,'1001','1001',2,'已修改 supportAlarm 。'),(53,'2011-03-07 13:38:58',2,19,'1004','硬盘丢失',2,'已修改 alarmStatus 和 alarmDesc 。'),(54,'2011-03-07 13:39:10',2,19,'1003','动态检测',2,'已修改 alarmStatus 和 alarmDesc 。'),(55,'2011-03-07 13:41:14',2,19,'4001','刷新基站列表',2,'没有字段被修改。'),(56,'2011-03-09 09:32:44',2,11,'deon','deon',1,''),(57,'2011-03-09 10:04:19',2,12,'test2','test2',1,''),(58,'2011-03-09 10:04:25',2,12,'222222222','222222222',1,''),(59,'2011-03-09 10:04:41',2,11,'deon','deon',2,'已修改 user_groups 。'),(60,'2011-03-09 10:07:28',2,11,'deon','deon',2,'已修改 user_groups 。'),(61,'2011-03-09 10:09:43',2,11,'deon','deon',2,'已修改 user_groups 。'),(62,'2011-03-09 10:12:54',2,11,'deon','deon',2,'已修改 user_groups 。'),(63,'2011-03-09 10:16:58',2,11,'deon','deon',2,'已修改 user_groups 。'),(64,'2011-03-09 10:23:47',2,9,'1001','1001',2,'已修改 supportAlarm 和 user_groups 。'),(65,'2011-03-09 10:23:58',2,9,'1001','1001',2,'已修改 supportAlarm 和 user_groups 。'),(66,'2011-03-09 10:26:15',2,12,'11','11<*>',2,'已修改 isAdmin 。'),(67,'2011-03-09 10:26:31',2,11,'deon','deon',2,'已修改 user_groups 。'),(68,'2011-03-09 10:32:07',2,9,'1002','1002',1,''),(69,'2011-03-09 10:38:22',2,9,'1002','1002',2,'已修改 channels 和 supportAlarm 。'),(70,'2011-03-09 10:38:50',2,9,'1002','1002',2,'已修改 supportAlarm 和 user_groups 。'),(71,'2011-03-09 10:43:02',2,11,'deon','deon',2,'已修改 user_groups 。'),(72,'2011-03-09 10:43:21',2,11,'deon','deon',2,'已修改 user_groups 。'),(73,'2011-03-09 10:43:33',2,11,'deon','deon',2,'已修改 user_groups 。');
/*!40000 ALTER TABLE `django_admin_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_content_type`
--

DROP TABLE IF EXISTS `django_content_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_content_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) COLLATE utf8_bin NOT NULL,
  `app_label` varchar(100) COLLATE utf8_bin NOT NULL,
  `model` varchar(100) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `app_label` (`app_label`,`model`)
) ENGINE=MyISAM AUTO_INCREMENT=22 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_content_type`
--

LOCK TABLES `django_content_type` WRITE;
/*!40000 ALTER TABLE `django_content_type` DISABLE KEYS */;
INSERT INTO `django_content_type` VALUES (1,'permission','auth','permission'),(2,'group','auth','group'),(3,'user','auth','user'),(4,'message','auth','message'),(5,'content type','contenttypes','contenttype'),(6,'session','sessions','session'),(7,'site','sites','site'),(8,'log entry','admin','logentry'),(9,'base station','coreapp','basestation'),(21,'计划任务','coreapp','videotask'),(11,'user','coreapp','user'),(12,'user group','coreapp','usergroup'),(13,'user group relation','coreapp','usergrouprelation'),(14,'station group relation','coreapp','stationgrouprelation'),(19,'alarm define','sysparam','alarmdefine'),(16,'alarm record','coreapp','alarmrecord'),(17,'system log','coreapp','systemlog'),(18,'location','coreapp','location'),(20,'bts category','sysparam','btscategory');
/*!40000 ALTER TABLE `django_content_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_session`
--

DROP TABLE IF EXISTS `django_session`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_session` (
  `session_key` varchar(40) COLLATE utf8_bin NOT NULL,
  `session_data` longtext COLLATE utf8_bin NOT NULL,
  `expire_date` datetime NOT NULL,
  PRIMARY KEY (`session_key`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_session`
--

LOCK TABLES `django_session` WRITE;
/*!40000 ALTER TABLE `django_session` DISABLE KEYS */;
INSERT INTO `django_session` VALUES ('8a6598f11d83e6158770789e21aa5fca','gAJ9cQEoVRJfYXV0aF91c2VyX2JhY2tlbmRxAlUpZGphbmdvLmNvbnRyaWIuYXV0aC5iYWNrZW5k\ncy5Nb2RlbEJhY2tlbmRxA1UNX2F1dGhfdXNlcl9pZHEESwF1LjMyMjI3YmVjNjA5ZTdkNWFiN2I5\nOWIzMDZiNTEzMWQ5\n','2011-03-20 15:06:01'),('b0c9845e1d608188876eb3ae804793a5','gAJ9cQEoVRJfYXV0aF91c2VyX2JhY2tlbmRxAlUpZGphbmdvLmNvbnRyaWIuYXV0aC5iYWNrZW5k\ncy5Nb2RlbEJhY2tlbmRxA1UNX2F1dGhfdXNlcl9pZHEESwJ1LmRjMjY1ZDc3ZGIyNzU1MTM4MjJk\nNWZlNGVkODYzMWRj\n','2011-03-18 22:04:01'),('c41d09e27662f89dd8020ea4764c758e','gAJ9cQEoVRJfYXV0aF91c2VyX2JhY2tlbmRxAlUpZGphbmdvLmNvbnRyaWIuYXV0aC5iYWNrZW5k\ncy5Nb2RlbEJhY2tlbmRxA1UNX2F1dGhfdXNlcl9pZHEESwJ1LmRjMjY1ZDc3ZGIyNzU1MTM4MjJk\nNWZlNGVkODYzMWRj\n','2011-03-18 12:57:24');
/*!40000 ALTER TABLE `django_session` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_site`
--

DROP TABLE IF EXISTS `django_site`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_site` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `domain` varchar(100) COLLATE utf8_bin NOT NULL,
  `name` varchar(50) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_site`
--

LOCK TABLES `django_site` WRITE;
/*!40000 ALTER TABLE `django_site` DISABLE KEYS */;
INSERT INTO `django_site` VALUES (1,'example.com','example.com');
/*!40000 ALTER TABLE `django_site` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `goku_system_log`
--

DROP TABLE IF EXISTS `goku_system_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goku_system_log` (
  `uuid` varchar(32) COLLATE utf8_bin NOT NULL,
  `actionOwner` varchar(50) COLLATE utf8_bin NOT NULL,
  `actionObject` varchar(50) COLLATE utf8_bin NOT NULL,
  `actionType` varchar(50) COLLATE utf8_bin NOT NULL,
  `description` varchar(1024) COLLATE utf8_bin DEFAULT NULL,
  `createDate` datetime DEFAULT NULL,
  PRIMARY KEY (`uuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `goku_system_log`
--

LOCK TABLES `goku_system_log` WRITE;
/*!40000 ALTER TABLE `goku_system_log` DISABLE KEYS */;
INSERT INTO `goku_system_log` VALUES ('','11','22','1','22','2011-03-02 14:30:26'),('a52b7a8cf78343b43be47d7bbc4dad6','22','master','login_fail','127.0.0.1','2011-03-09 09:31:31'),('bf4d62451d81b8d48129d85f572','22','master','login_fail','127.0.0.1','2011-03-09 09:31:49'),('26c8ae220474ceedd337c6011781b6','deon','master','login_ok','127.0.0.1','2011-03-09 09:33:15'),('b66a2ace603032bf298570dbffebf9e4','deon','master','login_ok','127.0.0.1','2011-03-09 10:37:30');
/*!40000 ALTER TABLE `goku_system_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `location`
--

DROP TABLE IF EXISTS `location`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `location` (
  `uuid` varchar(32) COLLATE utf8_bin NOT NULL,
  `name` varchar(50) COLLATE utf8_bin NOT NULL,
  `parent` varchar(32) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`uuid`),
  KEY `location_parent` (`parent`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `location`
--

LOCK TABLES `location` WRITE;
/*!40000 ALTER TABLE `location` DISABLE KEYS */;
INSERT INTO `location` VALUES ('001','浙江',NULL),('22','22','001');
/*!40000 ALTER TABLE `location` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `relation_station_group`
--

DROP TABLE IF EXISTS `relation_station_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `relation_station_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `base_station_id` varchar(10) COLLATE utf8_bin NOT NULL,
  `user_group_id` varchar(20) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `base_station_id` (`base_station_id`,`user_group_id`),
  KEY `relation_station_group_base_station_id` (`base_station_id`),
  KEY `relation_station_group_user_group_id` (`user_group_id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `relation_station_group`
--

LOCK TABLES `relation_station_group` WRITE;
/*!40000 ALTER TABLE `relation_station_group` DISABLE KEYS */;
INSERT INTO `relation_station_group` VALUES (3,'1001','11'),(2,'1001','test2'),(4,'1002','222222222');
/*!40000 ALTER TABLE `relation_station_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `relation_user_group`
--

DROP TABLE IF EXISTS `relation_user_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `relation_user_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` varchar(20) COLLATE utf8_bin NOT NULL,
  `user_group_id` varchar(20) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`user_group_id`),
  KEY `relation_user_group_user_id` (`user_id`),
  KEY `relation_user_group_user_group_id` (`user_group_id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `relation_user_group`
--

LOCK TABLES `relation_user_group` WRITE;
/*!40000 ALTER TABLE `relation_user_group` DISABLE KEYS */;
INSERT INTO `relation_user_group` VALUES (1,'22','11'),(10,'deon','11');
/*!40000 ALTER TABLE `relation_user_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_account`
--

DROP TABLE IF EXISTS `user_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_account` (
  `name` varchar(20) COLLATE utf8_bin NOT NULL,
  `password` varchar(50) COLLATE utf8_bin NOT NULL,
  `display` varchar(50) COLLATE utf8_bin NOT NULL,
  `lastActive` datetime DEFAULT NULL,
  `status` varchar(10) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_account`
--

LOCK TABLES `user_account` WRITE;
/*!40000 ALTER TABLE `user_account` DISABLE KEYS */;
INSERT INTO `user_account` VALUES ('22','new','2',NULL,'locked'),('deon','123','deon',NULL,'ok');
/*!40000 ALTER TABLE `user_account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_group`
--

DROP TABLE IF EXISTS `user_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_group` (
  `name` varchar(20) COLLATE utf8_bin NOT NULL,
  `isAdmin` int(11) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_group`
--

LOCK TABLES `user_group` WRITE;
/*!40000 ALTER TABLE `user_group` DISABLE KEYS */;
INSERT INTO `user_group` VALUES ('11',1),('test2',0),('222222222',0);
/*!40000 ALTER TABLE `user_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `video_task`
--

DROP TABLE IF EXISTS `video_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `video_task` (
  `taskID` int(11) NOT NULL,
  `name` varchar(50) COLLATE utf8_bin NOT NULL,
  `userName` varchar(50) COLLATE utf8_bin NOT NULL,
  `status` varchar(4) COLLATE utf8_bin NOT NULL,
  `startDate` varchar(16) COLLATE utf8_bin NOT NULL,
  `endDate` varchar(16) COLLATE utf8_bin NOT NULL,
  `weekDays` varchar(20) COLLATE utf8_bin NOT NULL,
  `startTime` varchar(10) COLLATE utf8_bin NOT NULL,
  `endTime` varchar(10) COLLATE utf8_bin NOT NULL,
  `uuid` varchar(12) COLLATE utf8_bin NOT NULL,
  `channel` varchar(5) COLLATE utf8_bin NOT NULL,
  `windowID` int(11) NOT NULL,
  `minShowTime` varchar(8) COLLATE utf8_bin NOT NULL,
  `showOrder` int(11) NOT NULL,
  PRIMARY KEY (`taskID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `video_task`
--

LOCK TABLES `video_task` WRITE;
/*!40000 ALTER TABLE `video_task` DISABLE KEYS */;
INSERT INTO `video_task` VALUES (1,'33','3','2','','','1,2,3,4','08:30','08:30','3','3',1,'10',43);
/*!40000 ALTER TABLE `video_task` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-09 20:02:53
