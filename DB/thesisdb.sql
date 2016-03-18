-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Mar 13, 2016 at 09:18 AM
-- Server version: 10.1.9-MariaDB
-- PHP Version: 5.6.15

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `thesisdb`
--

-- --------------------------------------------------------

--
-- Table structure for table `check_devices`
--

CREATE TABLE `check_devices` (
  `check_devices_id` int(11) NOT NULL,
  `rooms_id` int(11) NOT NULL,
  `check_devices_status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `check_rooms`
--

CREATE TABLE `check_rooms` (
  `check_rooms_id` int(11) NOT NULL,
  `check_rooms_status` int(11) NOT NULL,
  `check_rooms_for` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `check_rooms`
--

INSERT INTO `check_rooms` (`check_rooms_id`, `check_rooms_status`, `check_rooms_for`) VALUES
(47, 0, 'view'),
(48, 0, 'manual'),
(49, 0, 'update'),
(50, 0, 'manage');

-- --------------------------------------------------------

--
-- Table structure for table `floors`
--

CREATE TABLE `floors` (
  `floors_id` int(11) NOT NULL,
  `floors_name` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `floors`
--

INSERT INTO `floors` (`floors_id`, `floors_name`) VALUES
(1, '1st Floor'),
(2, '2nd Floor');

-- --------------------------------------------------------

--
-- Table structure for table `floor_room_groups`
--

CREATE TABLE `floor_room_groups` (
  `floor_room_groups_id` int(11) NOT NULL,
  `floors_id` int(11) NOT NULL,
  `rooms_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `floor_room_groups`
--

INSERT INTO `floor_room_groups` (`floor_room_groups_id`, `floors_id`, `rooms_id`) VALUES
(7, 1, 7),
(8, 2, 8),
(9, 1, 9);

-- --------------------------------------------------------

--
-- Table structure for table `frames`
--

CREATE TABLE `frames` (
  `frames_id` int(11) NOT NULL,
  `frames_status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `rooms`
--

CREATE TABLE `rooms` (
  `rooms_id` int(11) NOT NULL,
  `rooms_name` varchar(45) NOT NULL,
  `rooms_address` varchar(32) DEFAULT NULL,
  `rooms_port` int(11) DEFAULT NULL,
  `rooms_ble_password` varchar(32) NOT NULL,
  `rooms_key_address` varchar(32) NOT NULL,
  `rooms_status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `rooms`
--

INSERT INTO `rooms` (`rooms_id`, `rooms_name`, `rooms_address`, `rooms_port`, `rooms_ble_password`, `rooms_key_address`, `rooms_status`) VALUES
(7, 'LB167TC', '11 11 11 11 11 11 11 11', 9, '', '', 0),
(8, 'LB361TC', '11 11 11 11 11 11 11 10', 18, '', '', 0),
(9, 'LB168TC', '11 10 1A 1B 1C 1D 1E 1F', 9, '', '', 0);

-- --------------------------------------------------------

--
-- Table structure for table `rooms_routing_table`
--

CREATE TABLE `rooms_routing_table` (
  `rooms_routing_table_id` int(11) NOT NULL,
  `rooms_address` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `room_consumptions`
--

CREATE TABLE `room_consumptions` (
  `room_consumptions_id` int(11) NOT NULL,
  `rooms_id` int(11) NOT NULL,
  `room_consumptions` float NOT NULL,
  `room_consumptions_datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `room_consumptions`
--

INSERT INTO `room_consumptions` (`room_consumptions_id`, `rooms_id`, `room_consumptions`, `room_consumptions_datetime`) VALUES
(1, 7, 1000, '2016-03-02 16:00:00'),
(2, 7, 1000, '2016-03-02 16:00:00'),
(3, 7, 9000, '2016-03-03 16:00:00');

-- --------------------------------------------------------

--
-- Table structure for table `room_devices`
--

CREATE TABLE `room_devices` (
  `room_devices_id` int(11) NOT NULL,
  `rooms_id` int(11) NOT NULL,
  `room_devices_name` varchar(45) NOT NULL,
  `room_devices_port` int(11) DEFAULT NULL,
  `room_devices_status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `room_devices`
--

INSERT INTO `room_devices` (`room_devices_id`, `rooms_id`, `room_devices_name`, `room_devices_port`, `room_devices_status`) VALUES
(63, 7, 'Lights', 18, 1),
(64, 7, 'Aircon', 19, 1),
(65, 9, 'Aircon', 18, 0);

-- --------------------------------------------------------

--
-- Table structure for table `room_device_schedules`
--

CREATE TABLE `room_device_schedules` (
  `room_device_schedules_id` int(11) NOT NULL,
  `room_devices_id` int(11) NOT NULL,
  `room_schedules_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `room_device_schedules`
--

INSERT INTO `room_device_schedules` (`room_device_schedules_id`, `room_devices_id`, `room_schedules_id`) VALUES
(355, 64, 23),
(357, 63, 24);

-- --------------------------------------------------------

--
-- Table structure for table `room_schedules`
--

CREATE TABLE `room_schedules` (
  `room_schedules_id` int(11) NOT NULL,
  `room_schedules_name` varchar(45) NOT NULL,
  `rooms_id` int(11) NOT NULL,
  `room_schedules_day` varchar(45) DEFAULT NULL,
  `room_schedules_date` date DEFAULT NULL,
  `room_schedules_start_time` int(11) DEFAULT NULL,
  `room_schedules_end_time` int(11) DEFAULT NULL,
  `room_schedules_duration` int(11) DEFAULT NULL,
  `room_schedules_type` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `room_schedules`
--

INSERT INTO `room_schedules` (`room_schedules_id`, `room_schedules_name`, `rooms_id`, `room_schedules_day`, `room_schedules_date`, `room_schedules_start_time`, `room_schedules_end_time`, `room_schedules_duration`, `room_schedules_type`) VALUES
(13, 'Klasi', 9, 'Mon', NULL, 450, 630, NULL, 0),
(23, 'Klasi', 7, 'Mon', NULL, 450, 630, NULL, 0),
(24, 'Pop', 7, 'Tue', NULL, 450, 510, NULL, 0);

-- --------------------------------------------------------

--
-- Table structure for table `system_pan`
--

CREATE TABLE `system_pan` (
  `system_pan_id` int(11) NOT NULL,
  `system_pan_name` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `triggers`
--

CREATE TABLE `triggers` (
  `triggers_id` int(11) NOT NULL,
  `triggers_table_id` int(11) NOT NULL,
  `triggers_table` int(11) NOT NULL COMMENT '1-rooms, 2-room_schedules, 3-room_device, 4-pan_id'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `user_details`
--

CREATE TABLE `user_details` (
  `user_details_id` int(11) NOT NULL,
  `user_details_password` varchar(45) NOT NULL,
  `user_details_pan_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `user_details`
--

INSERT INTO `user_details` (`user_details_id`, `user_details_password`, `user_details_pan_id`) VALUES
(1, 'f10e2821bbbea527ea02200352313bc059445190', 255);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `check_devices`
--
ALTER TABLE `check_devices`
  ADD PRIMARY KEY (`check_devices_id`);

--
-- Indexes for table `check_rooms`
--
ALTER TABLE `check_rooms`
  ADD PRIMARY KEY (`check_rooms_id`);

--
-- Indexes for table `floors`
--
ALTER TABLE `floors`
  ADD PRIMARY KEY (`floors_id`);

--
-- Indexes for table `floor_room_groups`
--
ALTER TABLE `floor_room_groups`
  ADD PRIMARY KEY (`floor_room_groups_id`),
  ADD KEY `fk_floor_room_group_floors1_idx` (`floors_id`),
  ADD KEY `fk_floor_room_group_rooms1_idx` (`rooms_id`);

--
-- Indexes for table `frames`
--
ALTER TABLE `frames`
  ADD PRIMARY KEY (`frames_id`);

--
-- Indexes for table `rooms`
--
ALTER TABLE `rooms`
  ADD PRIMARY KEY (`rooms_id`);

--
-- Indexes for table `rooms_routing_table`
--
ALTER TABLE `rooms_routing_table`
  ADD PRIMARY KEY (`rooms_routing_table_id`);

--
-- Indexes for table `room_consumptions`
--
ALTER TABLE `room_consumptions`
  ADD PRIMARY KEY (`room_consumptions_id`),
  ADD KEY `fk_room_consumptions_rooms1_idx` (`rooms_id`);

--
-- Indexes for table `room_devices`
--
ALTER TABLE `room_devices`
  ADD PRIMARY KEY (`room_devices_id`),
  ADD KEY `fk_room_devices_rooms1_idx` (`rooms_id`);

--
-- Indexes for table `room_device_schedules`
--
ALTER TABLE `room_device_schedules`
  ADD PRIMARY KEY (`room_device_schedules_id`),
  ADD KEY `fk_room_device_schedules_room_devices1_idx` (`room_devices_id`),
  ADD KEY `fk_room_device_schedules_room_schedules1_idx` (`room_schedules_id`);

--
-- Indexes for table `room_schedules`
--
ALTER TABLE `room_schedules`
  ADD PRIMARY KEY (`room_schedules_id`),
  ADD KEY `fk_room_schedule_rooms1_idx` (`rooms_id`);

--
-- Indexes for table `system_pan`
--
ALTER TABLE `system_pan`
  ADD PRIMARY KEY (`system_pan_id`);

--
-- Indexes for table `triggers`
--
ALTER TABLE `triggers`
  ADD PRIMARY KEY (`triggers_id`);

--
-- Indexes for table `user_details`
--
ALTER TABLE `user_details`
  ADD PRIMARY KEY (`user_details_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `check_devices`
--
ALTER TABLE `check_devices`
  MODIFY `check_devices_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `check_rooms`
--
ALTER TABLE `check_rooms`
  MODIFY `check_rooms_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=51;
--
-- AUTO_INCREMENT for table `floors`
--
ALTER TABLE `floors`
  MODIFY `floors_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT for table `floor_room_groups`
--
ALTER TABLE `floor_room_groups`
  MODIFY `floor_room_groups_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT for table `frames`
--
ALTER TABLE `frames`
  MODIFY `frames_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `rooms`
--
ALTER TABLE `rooms`
  MODIFY `rooms_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT for table `rooms_routing_table`
--
ALTER TABLE `rooms_routing_table`
  MODIFY `rooms_routing_table_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `room_consumptions`
--
ALTER TABLE `room_consumptions`
  MODIFY `room_consumptions_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
--
-- AUTO_INCREMENT for table `room_devices`
--
ALTER TABLE `room_devices`
  MODIFY `room_devices_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=66;
--
-- AUTO_INCREMENT for table `room_device_schedules`
--
ALTER TABLE `room_device_schedules`
  MODIFY `room_device_schedules_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=358;
--
-- AUTO_INCREMENT for table `room_schedules`
--
ALTER TABLE `room_schedules`
  MODIFY `room_schedules_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=25;
--
-- AUTO_INCREMENT for table `system_pan`
--
ALTER TABLE `system_pan`
  MODIFY `system_pan_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `triggers`
--
ALTER TABLE `triggers`
  MODIFY `triggers_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `user_details`
--
ALTER TABLE `user_details`
  MODIFY `user_details_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `floor_room_groups`
--
ALTER TABLE `floor_room_groups`
  ADD CONSTRAINT `fk_floor_room_group_floors1` FOREIGN KEY (`floors_id`) REFERENCES `floors` (`floors_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `fk_floor_room_group_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `room_consumptions`
--
ALTER TABLE `room_consumptions`
  ADD CONSTRAINT `fk_room_consumptions_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `room_devices`
--
ALTER TABLE `room_devices`
  ADD CONSTRAINT `fk_room_devices_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `room_device_schedules`
--
ALTER TABLE `room_device_schedules`
  ADD CONSTRAINT `fk_room_device_schedules_room_devices1` FOREIGN KEY (`room_devices_id`) REFERENCES `room_devices` (`room_devices_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `fk_room_device_schedules_room_schedules1` FOREIGN KEY (`room_schedules_id`) REFERENCES `room_schedules` (`room_schedules_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `room_schedules`
--
ALTER TABLE `room_schedules`
  ADD CONSTRAINT `fk_room_schedule_rooms1` FOREIGN KEY (`rooms_id`) REFERENCES `rooms` (`rooms_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
