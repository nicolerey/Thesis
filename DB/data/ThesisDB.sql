-- MySQL dump 10.16  Distrib 10.1.9-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: ThesisDB
-- ------------------------------------------------------
-- Server version	10.1.9-MariaDB-log

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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `floors`
--

LOCK TABLES `floors` WRITE;
/*!40000 ALTER TABLE `floors` DISABLE KEYS */;
/*!40000 ALTER TABLE `floors` ENABLE KEYS */;
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
  `room_consumptions` double NOT NULL,
  PRIMARY KEY (`room_consumptions_id`),
  KEY `fk_room_consumption_rooms1_idx` (`rooms_id`),
  CONSTRAINT `fk_room_consumption_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
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
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_device_schedules`
--

LOCK TABLES `room_device_schedules` WRITE;
/*!40000 ALTER TABLE `room_device_schedules` DISABLE KEYS */;
INSERT INTO `room_device_schedules` VALUES (1,1,3),(2,5,23),(3,8,11),(4,9,20),(5,2,7),(6,11,21),(7,3,4);
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
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_devices`
--

LOCK TABLES `room_devices` WRITE;
/*!40000 ALTER TABLE `room_devices` DISABLE KEYS */;
INSERT INTO `room_devices` VALUES (1,1,'Lights',4,0),(2,1,'Projector',3,0),(3,1,'Wall Sockets',7,0),(4,2,'Lights',4,0),(5,2,'Projector',3,0),(6,2,'Wall Sockets',7,0),(7,3,'Lights',4,0),(8,3,'Projector',3,0),(9,3,'Wall Sockets',7,0),(10,4,'Lights',4,0),(11,4,'Projector',3,0),(12,4,'Wall Sockets',7,0);
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
  PRIMARY KEY (`room_schedules_id`),
  KEY `fk_room_schedule_rooms1_idx` (`rooms_id`),
  CONSTRAINT `fk_room_schedule_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room_schedules`
--

LOCK TABLES `room_schedules` WRITE;
/*!40000 ALTER TABLE `room_schedules` DISABLE KEYS */;
INSERT INTO `room_schedules` VALUES (1,'',1,'Sat',NULL,480,600),(2,'',1,'Mon, Wed, Fri',NULL,510,630),(3,'',1,'Mon, Wed',NULL,630,690),(4,'',1,'Fri',NULL,630,750),(5,'',1,'Tue, Thu',NULL,540,630),(6,'',1,'Tue, Thu',NULL,630,690),(7,'',2,'Mon, Wed',NULL,450,510),(8,'',2,'Mon, Wed',NULL,720,810),(9,'',2,'Tue, Thu',NULL,450,540),(10,'',2,'Tue, Thu',NULL,540,630),(11,'',2,'Tue, Thu',NULL,720,780),(12,'',3,'Mon, Wed',NULL,780,870),(13,'',3,'Mon, Wed',NULL,870,990),(14,'',3,'Mon, Wed',NULL,1020,1110),(15,'',3,'Tue, Thu',NULL,780,1080),(16,'',4,'Mon, Wed',NULL,810,870),(17,'',4,'Mon, Wed',NULL,870,1050),(18,'',4,'Tue, Thu',NULL,900,960),(19,'',4,'Tue, Thu',NULL,990,1080),(20,'',1,NULL,'2015-12-15',810,840),(21,'',2,NULL,'2015-12-15',80,255),(22,'',3,NULL,'2015-12-17',20,120),(23,'',4,NULL,'2015-12-19',300,330),(24,'',2,NULL,'2015-12-16',209,267);
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
  `rooms_address` int(11) DEFAULT NULL,
  `rooms_port` int(11) DEFAULT NULL,
  `rooms_status` int(11) NOT NULL,
  PRIMARY KEY (`rooms_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rooms`
--

LOCK TABLES `rooms` WRITE;
/*!40000 ALTER TABLE `rooms` DISABLE KEYS */;
INSERT INTO `rooms` VALUES (1,'LB286',8,5,0),(2,'LB285',7,6,0),(3,'LB265',2,6,0),(4,'LB264',3,6,0);
/*!40000 ALTER TABLE `rooms` ENABLE KEYS */;
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
  PRIMARY KEY (`user_details_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_details`
--

LOCK TABLES `user_details` WRITE;
/*!40000 ALTER TABLE `user_details` DISABLE KEYS */;
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

-- Dump completed on 2015-12-13 19:25:48
