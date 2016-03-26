-- MySQL dump 10.13  Distrib 5.5.44, for debian-linux-gnu (armv7l)
--
-- Host: localhost    Database: ThesisDB
-- ------------------------------------------------------
-- Server version	5.5.44-0+deb8u1

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
-- Table structure for table `check_devices`
--

DROP TABLE IF EXISTS `check_devices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `check_devices` (
  `check_devices_id` int(11) NOT NULL AUTO_INCREMENT,
  `rooms_id` int(11) NOT NULL,
  `check_devices_status` int(11) NOT NULL,
  PRIMARY KEY (`check_devices_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `check_devices`
--

LOCK TABLES `check_devices` WRITE;
/*!40000 ALTER TABLE `check_devices` DISABLE KEYS */;
/*!40000 ALTER TABLE `check_devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `check_rooms`
--

DROP TABLE IF EXISTS `check_rooms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `check_rooms` (
  `check_rooms_id` int(11) NOT NULL AUTO_INCREMENT,
  `check_rooms_status` int(11) NOT NULL,
  `check_rooms_for` varchar(32) NOT NULL,
  PRIMARY KEY (`check_rooms_id`)
) ENGINE=InnoDB AUTO_INCREMENT=51 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `check_rooms`
--

LOCK TABLES `check_rooms` WRITE;
/*!40000 ALTER TABLE `check_rooms` DISABLE KEYS */;
INSERT INTO `check_rooms` VALUES (47,0,'view'),(48,0,'manual'),(49,0,'update'),(50,0,'manage');
/*!40000 ALTER TABLE `check_rooms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `floor_room_groups`
--

DROP TABLE IF EXISTS `floor_room_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `floor_room_groups` (
  `floor_room_groups_id` int(11) NOT NULL AUTO_INCREMENT,
  `floors_id` int(11) NOT NULL,
  `rooms_id` int(11) NOT NULL,
  PRIMARY KEY (`floor_room_groups_id`),
  KEY `fk_floor_room_group_floors1_idx` (`floors_id`),
  KEY `fk_floor_room_group_rooms1_idx` (`rooms_id`),
  CONSTRAINT `fk_floor_room_group_floors1` FOREIGN KEY (`floors_id`) REFERENCES `floors` (`floors_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_floor_room_group_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `floor_room_groups`
--

LOCK TABLES `floor_room_groups` WRITE;
/*!40000 ALTER TABLE `floor_room_groups` DISABLE KEYS */;
INSERT INTO `floor_room_groups` VALUES (1,1,7);
/*!40000 ALTER TABLE `floor_room_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `floors`
--

DROP TABLE IF EXISTS `floors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `floors` (
  `floors_id` int(11) NOT NULL AUTO_INCREMENT,
  `floors_name` varchar(45) NOT NULL,
  PRIMARY KEY (`floors_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `floors`
--

LOCK TABLES `floors` WRITE;
/*!40000 ALTER TABLE `floors` DISABLE KEYS */;
INSERT INTO `floors` VALUES (1,'1st Floor');
/*!40000 ALTER TABLE `floors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `frames`
--

DROP TABLE IF EXISTS `frames`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `frames` (
  `frames_id` int(11) NOT NULL AUTO_INCREMENT,
  `frames_status` int(11) NOT NULL,
  PRIMARY KEY (`frames_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `frames`
--

LOCK TABLES `frames` WRITE;
/*!40000 ALTER TABLE `frames` DISABLE KEYS */;
/*!40000 ALTER TABLE `frames` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `room_consumptions`
--

DROP TABLE IF EXISTS `room_consumptions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `room_consumptions` (
  `room_consumptions_id` int(11) NOT NULL AUTO_INCREMENT,
  `rooms_id` int(11) NOT NULL,
  `room_consumptions` float NOT NULL,
  `room_consumptions_datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`room_consumptions_id`),
  KEY `fk_room_consumptions_rooms1_idx` (`rooms_id`),
  CONSTRAINT `fk_room_consumptions_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=70 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_consumptions`
--

LOCK TABLES `room_consumptions` WRITE;
/*!40000 ALTER TABLE `room_consumptions` DISABLE KEYS */;
INSERT INTO `room_consumptions` VALUES (37,7,0,'2016-03-23 23:09:33'),(38,7,0,'2016-03-24 01:51:00'),(39,7,247,'2016-03-24 02:51:27'),(40,7,248,'2016-03-24 03:51:55'),(41,7,44,'2016-03-24 04:52:23'),(42,7,87,'2016-03-24 05:52:52'),(43,7,250,'2016-03-24 06:53:20'),(44,7,249,'2016-03-24 07:53:49'),(45,7,250,'2016-03-24 08:54:16'),(46,7,250,'2016-03-24 09:54:44'),(47,7,251,'2016-03-24 10:55:12'),(48,7,170,'2016-03-24 11:55:40'),(49,7,0,'2016-03-24 11:20:27'),(50,7,2.5,'2016-03-24 12:20:55'),(51,7,0,'2016-03-25 03:04:18'),(52,7,230,'2016-03-25 04:04:46'),(53,7,124,'2016-03-25 05:05:14'),(54,7,249,'2016-03-25 06:05:41'),(55,7,232,'2016-03-25 07:06:10'),(56,7,55,'2016-03-25 08:06:39'),(57,7,253,'2016-03-25 09:07:06'),(58,7,251,'2016-03-25 10:07:35'),(59,7,252,'2016-03-25 11:08:02'),(60,7,239,'2016-03-25 12:08:31'),(61,7,6.7,'2016-03-25 13:08:58'),(62,7,0,'2016-03-25 16:39:21'),(63,7,0,'2016-03-26 04:25:45'),(64,7,232,'2016-03-26 05:26:13'),(65,7,246,'2016-03-26 06:26:41'),(66,7,246,'2016-03-26 07:27:09'),(67,7,245,'2016-03-26 08:27:37'),(68,7,156,'2016-03-26 09:28:05'),(69,7,2.8,'2016-03-26 10:28:33');
/*!40000 ALTER TABLE `room_consumptions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `room_device_schedules`
--

DROP TABLE IF EXISTS `room_device_schedules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `room_device_schedules` (
  `room_device_schedules_id` int(11) NOT NULL AUTO_INCREMENT,
  `room_devices_id` int(11) NOT NULL,
  `room_schedules_id` int(11) NOT NULL,
  PRIMARY KEY (`room_device_schedules_id`),
  KEY `fk_room_device_schedules_room_devices1_idx` (`room_devices_id`),
  KEY `fk_room_device_schedules_room_schedules1_idx` (`room_schedules_id`),
  CONSTRAINT `fk_room_device_schedules_room_devices1` FOREIGN KEY (`room_devices_id`) REFERENCES `room_devices` (`room_devices_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_room_device_schedules_room_schedules1` FOREIGN KEY (`room_schedules_id`) REFERENCES `room_schedules` (`room_schedules_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=65 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_device_schedules`
--

LOCK TABLES `room_device_schedules` WRITE;
/*!40000 ALTER TABLE `room_device_schedules` DISABLE KEYS */;
INSERT INTO `room_device_schedules` VALUES (1,63,1),(2,64,1),(3,65,1),(4,66,1),(5,63,2),(6,64,2),(7,65,2),(8,66,2),(9,63,3),(10,64,3),(11,65,3),(12,66,3),(13,63,4),(14,64,4),(15,65,4),(16,66,4),(17,63,5),(18,64,5),(19,65,5),(20,66,5),(21,63,6),(22,64,6),(23,65,6),(24,66,6),(25,63,7),(26,64,7),(27,65,7),(28,66,7),(29,63,8),(30,64,8),(31,65,8),(32,66,8),(33,63,9),(34,64,9),(35,65,9),(36,66,9),(37,63,10),(38,64,10),(39,65,10),(40,66,10),(41,63,11),(42,64,11),(43,65,11),(44,66,11),(45,63,12),(46,64,12),(47,65,12),(48,66,12),(49,63,13),(50,64,13),(51,65,13),(52,66,13),(53,63,14),(54,64,14),(55,65,14),(56,66,14),(57,63,15),(58,64,15),(59,65,15),(60,66,15),(61,63,16),(62,64,16),(63,65,16),(64,66,16);
/*!40000 ALTER TABLE `room_device_schedules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `room_devices`
--

DROP TABLE IF EXISTS `room_devices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `room_devices` (
  `room_devices_id` int(11) NOT NULL AUTO_INCREMENT,
  `rooms_id` int(11) NOT NULL,
  `room_devices_name` varchar(45) NOT NULL,
  `room_devices_port` int(11) DEFAULT NULL,
  `room_devices_status` int(11) NOT NULL,
  PRIMARY KEY (`room_devices_id`),
  KEY `fk_room_devices_rooms1_idx` (`rooms_id`),
  CONSTRAINT `fk_room_devices_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_devices`
--

LOCK TABLES `room_devices` WRITE;
/*!40000 ALTER TABLE `room_devices` DISABLE KEYS */;
INSERT INTO `room_devices` VALUES (63,7,'Lights',8,1),(64,7,'Aircon',7,1),(65,7,'Electric Fan',6,1),(66,7,'Projector',5,1);
/*!40000 ALTER TABLE `room_devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `room_schedules`
--

DROP TABLE IF EXISTS `room_schedules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `room_schedules` (
  `room_schedules_id` int(11) NOT NULL AUTO_INCREMENT,
  `room_schedules_name` varchar(45) NOT NULL,
  `rooms_id` int(11) NOT NULL,
  `room_schedules_day` varchar(45) DEFAULT NULL,
  `room_schedules_date` date DEFAULT NULL,
  `room_schedules_start_time` int(11) DEFAULT NULL,
  `room_schedules_end_time` int(11) DEFAULT NULL,
  `room_schedules_duration` int(11) DEFAULT NULL,
  `room_schedules_type` int(11) NOT NULL,
  PRIMARY KEY (`room_schedules_id`),
  KEY `fk_room_schedule_rooms1_idx` (`rooms_id`),
  CONSTRAINT `fk_room_schedule_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_schedules`
--

LOCK TABLES `room_schedules` WRITE;
/*!40000 ALTER TABLE `room_schedules` DISABLE KEYS */;
INSERT INTO `room_schedules` VALUES (1,'EM 22',7,'Mon,Wed',NULL,630,720,NULL,0),(2,'EM 123',7,'Mon,Wed',NULL,750,810,NULL,0),(3,'EM 121',7,'Mon,Tue,Wed,Thu',NULL,810,900,NULL,0),(4,'EM 122',7,'Mon,Wed',NULL,930,1080,NULL,0),(5,'EM 211',7,'Mon,Wed',NULL,1080,1230,NULL,0),(6,'EM 123',7,'Tue,Thu',NULL,450,570,NULL,0),(7,'EM 122',7,'Tue,Thu',NULL,570,720,NULL,0),(8,'EM 121',7,'Tue,Thu',NULL,900,990,NULL,0),(9,'EM 22',7,'Tue,Thu',NULL,990,1080,NULL,0),(10,'EM 31',7,'Tue,Thu',NULL,1080,1170,NULL,0),(11,'NSTP 2',7,'Sat',NULL,750,1110,NULL,0),(12,'Klase',7,'Fri','0000-00-00',630,720,0,0),(13,'Klase',7,'Fri','0000-00-00',750,810,0,0),(14,'Klase',7,'Fri','0000-00-00',810,900,0,0),(15,'Klase',7,'Fri','0000-00-00',930,1080,0,0),(16,'Klase',7,'Fri','0000-00-00',1080,1230,0,0);
/*!40000 ALTER TABLE `room_schedules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rooms`
--

DROP TABLE IF EXISTS `rooms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rooms` (
  `rooms_id` int(11) NOT NULL AUTO_INCREMENT,
  `rooms_name` varchar(45) NOT NULL,
  `rooms_address` varchar(32) DEFAULT NULL,
  `rooms_port` int(11) DEFAULT NULL,
  `rooms_ble_password` varchar(32) NOT NULL,
  `rooms_key_address` varchar(32) NOT NULL,
  `rooms_status` int(11) NOT NULL,
  PRIMARY KEY (`rooms_id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rooms`
--

LOCK TABLES `rooms` WRITE;
/*!40000 ALTER TABLE `rooms` DISABLE KEYS */;
INSERT INTO `rooms` VALUES (7,'LB167TC','00 13 A2 00 40 9B AF 8A',9,'','',0);
/*!40000 ALTER TABLE `rooms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rooms_routing_table`
--

DROP TABLE IF EXISTS `rooms_routing_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rooms_routing_table` (
  `rooms_routing_table_id` int(11) NOT NULL AUTO_INCREMENT,
  `rooms_address` int(11) NOT NULL,
  PRIMARY KEY (`rooms_routing_table_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rooms_routing_table`
--

LOCK TABLES `rooms_routing_table` WRITE;
/*!40000 ALTER TABLE `rooms_routing_table` DISABLE KEYS */;
/*!40000 ALTER TABLE `rooms_routing_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `status_logs`
--

DROP TABLE IF EXISTS `status_logs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `status_logs` (
  `status_logs_id` int(11) NOT NULL AUTO_INCREMENT,
  `status_logs_status` int(11) NOT NULL,
  `status_logs_datetime` varchar(45) DEFAULT 'CURRENT_TIMESTAMP',
  PRIMARY KEY (`status_logs_id`)
) ENGINE=InnoDB AUTO_INCREMENT=38 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `status_logs`
--

LOCK TABLES `status_logs` WRITE;
/*!40000 ALTER TABLE `status_logs` DISABLE KEYS */;
INSERT INTO `status_logs` VALUES (1,1,'2016-03-24 07:35:19'),(2,0,'2016-03-24 09:14:39'),(3,1,'2016-03-24 09:14:58'),(4,1,'2016-03-24 09:50:59'),(5,0,'2016-03-24 09:51:00'),(6,1,'2016-03-24 09:51:01'),(7,0,'2016-03-24 10:22:57'),(8,1,'2016-03-24 10:23:17'),(9,0,'2016-03-24 12:02:02'),(10,1,'2016-03-24 13:32:57'),(11,0,'2016-03-24 19:35:32'),(12,1,'2016-03-25 11:04:19'),(13,1,'2016-03-25 11:06:20'),(14,0,'2016-03-25 11:43:47'),(15,1,'2016-03-25 11:44:07'),(16,0,'2016-03-25 12:00:46'),(17,1,'2016-03-25 12:36:15'),(18,0,'2016-03-25 15:02:10'),(19,1,'2016-03-25 15:54:24'),(20,0,'2016-03-25 20:05:28'),(21,1,'2016-03-25 20:33:47'),(22,0,'2016-03-25 20:34:44'),(23,1,'2016-03-25 23:40:35'),(24,1,'2016-03-26 12:30:49'),(25,0,'2016-03-26 12:34:31'),(26,1,'2016-03-26 12:34:51'),(27,0,'2016-03-26 14:22:26'),(28,1,'2016-03-26 14:22:46'),(29,0,'2016-03-26 14:47:13'),(30,1,'2016-03-26 14:47:33'),(31,0,'2016-03-26 15:44:56'),(32,1,'2016-03-26 15:45:15'),(33,0,'2016-03-26 16:21:47'),(34,1,'2016-03-26 16:22:07'),(35,0,'2016-03-26 16:27:00'),(36,1,'2016-03-26 16:27:20'),(37,0,'2016-03-26 17:05:09');
/*!40000 ALTER TABLE `status_logs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `system_pan`
--

DROP TABLE IF EXISTS `system_pan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_pan` (
  `system_pan_id` int(11) NOT NULL AUTO_INCREMENT,
  `system_pan_name` int(11) NOT NULL,
  PRIMARY KEY (`system_pan_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `system_pan`
--

LOCK TABLES `system_pan` WRITE;
/*!40000 ALTER TABLE `system_pan` DISABLE KEYS */;
/*!40000 ALTER TABLE `system_pan` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `triggers`
--

DROP TABLE IF EXISTS `triggers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `triggers` (
  `triggers_id` int(11) NOT NULL AUTO_INCREMENT,
  `triggers_table_id` int(11) NOT NULL,
  `triggers_table` int(11) NOT NULL COMMENT '1-rooms, 2-room_schedules, 3-room_device, 4-pan_id',
  PRIMARY KEY (`triggers_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `triggers`
--

LOCK TABLES `triggers` WRITE;
/*!40000 ALTER TABLE `triggers` DISABLE KEYS */;
/*!40000 ALTER TABLE `triggers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_details`
--

DROP TABLE IF EXISTS `user_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_details` (
  `user_details_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_details_password` varchar(45) NOT NULL,
  `user_details_pan_id` int(11) NOT NULL,
  PRIMARY KEY (`user_details_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_details`
--

LOCK TABLES `user_details` WRITE;
/*!40000 ALTER TABLE `user_details` DISABLE KEYS */;
INSERT INTO `user_details` VALUES (1,'f10e2821bbbea527ea02200352313bc059445190',255);
/*!40000 ALTER TABLE `user_details` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-03-26 20:24:27
