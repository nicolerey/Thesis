-- MySQL dump 10.16  Distrib 10.1.12-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: ThesisDB
-- ------------------------------------------------------
-- Server version	10.1.12-MariaDB

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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `floor_room_groups`
--

LOCK TABLES `floor_room_groups` WRITE;
/*!40000 ALTER TABLE `floor_room_groups` DISABLE KEYS */;
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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_consumptions`
--

LOCK TABLES `room_consumptions` WRITE;
/*!40000 ALTER TABLE `room_consumptions` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=53 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_device_schedules`
--

LOCK TABLES `room_device_schedules` WRITE;
/*!40000 ALTER TABLE `room_device_schedules` DISABLE KEYS */;
INSERT INTO `room_device_schedules` VALUES (1,1,1),(2,2,1),(3,1,2),(4,2,2),(5,1,3),(6,2,3),(7,1,4),(8,2,4),(9,1,5),(10,2,5),(11,1,6),(12,2,6),(13,1,7),(14,2,7),(15,1,16),(16,2,16),(17,3,8),(18,4,8),(19,3,9),(20,4,9),(21,3,10),(22,4,10),(23,3,11),(24,4,11),(25,3,12),(26,4,12),(27,3,13),(28,4,13),(29,3,14),(30,4,14),(31,3,15),(32,4,15),(33,5,17),(34,6,17),(35,5,18),(36,6,18),(37,5,19),(38,6,19),(39,5,20),(40,6,20),(41,5,21),(42,6,21),(43,5,22),(44,6,22),(45,5,23),(46,6,23),(47,5,24),(48,6,24),(49,5,25),(50,6,25),(51,3,26),(52,4,26);
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
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_devices`
--

LOCK TABLES `room_devices` WRITE;
/*!40000 ALTER TABLE `room_devices` DISABLE KEYS */;
INSERT INTO `room_devices` VALUES (1,1,'Light 1',6,0),(2,1,'Light 2',8,0),(3,2,'Light 1',6,0),(4,2,'Light 2',7,0),(5,3,'Light 1',NULL,0),(6,3,'Light 2',NULL,0),(7,4,'Light 1',NULL,0),(8,4,'Light 2',NULL,0);
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
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_schedules`
--

LOCK TABLES `room_schedules` WRITE;
/*!40000 ALTER TABLE `room_schedules` DISABLE KEYS */;
INSERT INTO `room_schedules` VALUES (1,'room 364 day 1.1',1,'','2016-04-04',630,725,NULL,0),(2,'room 364 day 1.2',1,'','2016-04-04',840,905,NULL,0),(3,'room 364 day 1.3',1,'','2016-04-04',990,1085,NULL,0),(4,'room 364 day 2.1',1,'','2016-04-05',450,605,NULL,0),(5,'room 364 day 2.2',1,'','2016-04-05',660,725,NULL,0),(6,'room 364 day 2.3',1,'','2016-04-05',750,815,NULL,0),(7,'room 364 day 2.4',1,'','2016-04-05',960,1115,NULL,0),(8,'room 364 day 3.1',2,'','2016-04-04',570,605,NULL,0),(9,'room 364 day 3.2',2,'','2016-04-04',630,725,NULL,0),(10,'room 364 day 3.3',2,'','2016-04-04',810,905,NULL,0),(11,'room 364 day 3.4',2,'','2016-04-04',930,965,NULL,0),(12,'room 364 day 3.5',2,'','2016-04-04',1020,1085,NULL,0),(13,'room 364 day 4.1',2,'','2016-04-05',570,605,NULL,0),(14,'room 364 day 4.2',2,'','2016-04-05',660,815,NULL,0),(15,'room 364 day 4.3',2,'','2016-04-05',900,1115,NULL,0),(16,'room 364 day 5.1',1,'','2016-04-06',840,965,NULL,0),(17,'room 365 day 1.1',3,NULL,'2016-03-31',570,630,NULL,0),(18,'room 365 day 1.2',3,NULL,'2016-03-31',660,720,NULL,0),(19,'room 365 day 1.3',3,NULL,'2016-03-31',750,810,NULL,0),(20,'room 365 day 1.4',3,NULL,'2016-03-31',840,960,NULL,0),(21,'room 365 day 2.1',3,NULL,'2016-04-01',450,510,NULL,0),(22,'room 365 day 2.2',3,NULL,'2016-04-01',570,600,NULL,0),(23,'room 365 day 2.3',3,NULL,'2016-04-01',660,690,NULL,0),(24,'room 365 day 2.4',3,NULL,'2016-04-01',900,960,NULL,0),(25,'room 365 day 2.5',3,NULL,'2016-04-01',1050,1110,NULL,0),(26,'room 365 day 3.1',4,NULL,'2016-03-31',570,630,NULL,0),(27,'room 365 day 3.2',4,NULL,'2016-03-31',660,810,NULL,0),(28,'room 365 day 3.3',4,NULL,'2016-03-31',840,960,NULL,0),(29,'room 365 day 3.4',4,NULL,'2016-03-31',1020,1080,NULL,0),(30,'room 365 day 4.1',4,NULL,'2016-04-01',450,510,NULL,0),(31,'room 365 day 4.2',4,NULL,'2016-04-01',660,690,NULL,0),(32,'room 365 day 4.3',4,NULL,'2016-04-01',840,870,NULL,0),(33,'room 365 day 4.4',4,NULL,'2016-04-01',900,990,NULL,0),(34,'room 365 day 4.5',4,NULL,'2016-04-01',1050,1110,NULL,0),(35,'room 365 day 5.1',3,NULL,'2016-04-02',870,990,NULL,0);
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
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rooms`
--

LOCK TABLES `rooms` WRITE;
/*!40000 ALTER TABLE `rooms` DISABLE KEYS */;
INSERT INTO `rooms` VALUES (1,'Room 364 A','00 13 A2 00 40 9B AF 8A',9,'','',0),(2,'Room 364 B','00 13 A2 00 40 E7 94 9E',9,'','',0),(3,'Room 365 A',NULL,9,'','',0),(4,'Room 365 B',NULL,9,'','',0);
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
  `rooms_id` int(11) NOT NULL,
  `status_logs_status` int(11) NOT NULL,
  `status_logs_datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`status_logs_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `status_logs`
--

LOCK TABLES `status_logs` WRITE;
/*!40000 ALTER TABLE `status_logs` DISABLE KEYS */;
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

-- Dump completed on 2016-03-31 22:31:41
