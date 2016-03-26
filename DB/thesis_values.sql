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
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `check_devices`
--

LOCK TABLES `check_devices` WRITE;
/*!40000 ALTER TABLE `check_devices` DISABLE KEYS */;
/*!40000 ALTER TABLE `check_devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `check_rooms`
--

LOCK TABLES `check_rooms` WRITE;
/*!40000 ALTER TABLE `check_rooms` DISABLE KEYS */;
INSERT INTO `check_rooms` VALUES (47,0,'view'),(48,0,'manual'),(49,0,'update'),(50,0,'manage');
/*!40000 ALTER TABLE `check_rooms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `floor_room_groups`
--

LOCK TABLES `floor_room_groups` WRITE;
/*!40000 ALTER TABLE `floor_room_groups` DISABLE KEYS */;
INSERT INTO `floor_room_groups` VALUES (1,1,7);
/*!40000 ALTER TABLE `floor_room_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `floors`
--

LOCK TABLES `floors` WRITE;
/*!40000 ALTER TABLE `floors` DISABLE KEYS */;
INSERT INTO `floors` VALUES (1,'1st Floor');
/*!40000 ALTER TABLE `floors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `frames`
--

LOCK TABLES `frames` WRITE;
/*!40000 ALTER TABLE `frames` DISABLE KEYS */;
/*!40000 ALTER TABLE `frames` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `room_consumptions`
--

LOCK TABLES `room_consumptions` WRITE;
/*!40000 ALTER TABLE `room_consumptions` DISABLE KEYS */;
/*!40000 ALTER TABLE `room_consumptions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `room_device_schedules`
--

LOCK TABLES `room_device_schedules` WRITE;
/*!40000 ALTER TABLE `room_device_schedules` DISABLE KEYS */;
INSERT INTO `room_device_schedules` VALUES (1,63,1),(2,64,1),(3,65,1),(4,66,1),(5,63,2),(6,64,2),(7,65,2),(8,66,2),(9,63,3),(10,64,3),(11,65,3),(12,66,3),(13,63,4),(14,64,4),(15,65,4),(16,66,4),(17,63,5),(18,64,5),(19,65,5),(20,66,5),(21,63,6),(22,64,6),(23,65,6),(24,66,6),(25,63,7),(26,64,7),(27,65,7),(28,66,7),(29,63,8),(30,64,8),(31,65,8),(32,66,8),(33,63,9),(34,64,9),(35,65,9),(36,66,9),(37,63,10),(38,64,10),(39,65,10),(40,66,10),(41,63,11),(42,64,11),(43,65,11),(44,66,11);
/*!40000 ALTER TABLE `room_device_schedules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `room_devices`
--

LOCK TABLES `room_devices` WRITE;
/*!40000 ALTER TABLE `room_devices` DISABLE KEYS */;
INSERT INTO `room_devices` VALUES (63,7,'Lights',8,1),(64,7,'Aircon',7,1),(65,7,'Electric Fan',6,1),(66,7,'Projector',5,1);
/*!40000 ALTER TABLE `room_devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `room_schedules`
--

LOCK TABLES `room_schedules` WRITE;
/*!40000 ALTER TABLE `room_schedules` DISABLE KEYS */;
INSERT INTO `room_schedules` VALUES (1,'EM 22',7,'Mon,Wed',NULL,630,720,NULL,0),(2,'EM 123',7,'Mon,Wed',NULL,750,810,NULL,0),(3,'EM 121',7,'Mon,Tue,Wed,Thu',NULL,810,900,NULL,0),(4,'EM 122',7,'Mon,Wed',NULL,930,1080,NULL,0),(5,'EM 211',7,'Mon,Wed',NULL,1080,1230,NULL,0),(6,'EM 123',7,'Tue,Thu',NULL,450,570,NULL,0),(7,'EM 122',7,'Tue,Thu',NULL,570,720,NULL,0),(8,'EM 121',7,'Tue,Thu',NULL,900,990,NULL,0),(9,'EM 22',7,'Tue,Thu',NULL,990,1080,NULL,0),(10,'EM 31',7,'Tue,Thu',NULL,1080,1170,NULL,0),(11,'NSTP 2',7,'Sat',NULL,750,1110,NULL,0);
/*!40000 ALTER TABLE `room_schedules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `rooms`
--

LOCK TABLES `rooms` WRITE;
/*!40000 ALTER TABLE `rooms` DISABLE KEYS */;
INSERT INTO `rooms` VALUES (7,'LB167TC','00 13 A2 00 40 9B AF 8A',9,'','',0);
/*!40000 ALTER TABLE `rooms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `rooms_routing_table`
--

LOCK TABLES `rooms_routing_table` WRITE;
/*!40000 ALTER TABLE `rooms_routing_table` DISABLE KEYS */;
/*!40000 ALTER TABLE `rooms_routing_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `system_pan`
--

LOCK TABLES `system_pan` WRITE;
/*!40000 ALTER TABLE `system_pan` DISABLE KEYS */;
/*!40000 ALTER TABLE `system_pan` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `triggers`
--

LOCK TABLES `triggers` WRITE;
/*!40000 ALTER TABLE `triggers` DISABLE KEYS */;
/*!40000 ALTER TABLE `triggers` ENABLE KEYS */;
UNLOCK TABLES;

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
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-03-23 21:22:57
