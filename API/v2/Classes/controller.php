<?php
	include 'mysql_setup.php';
	include 'tables.php';
	include 'tables/user_details.php';
	include 'tables/floors.php';
	include 'tables/rooms.php';
	include 'tables/floor_room_groups.php';
	include 'tables/room_consumptions.php';
	include 'tables/room_devices.php';
	include 'tables/room_schedules.php';
	include 'tables/room_device_schedules.php';

	class Controller{

		public $user_details;	// user_details
		public $floors;			// floors
		public $rooms;			// rooms
		public $frg;			// floor_room_groups
		public $rc;				// room_consumptions
		public $rd;				// room_devices
		public $rs;				// room_schedules
		public $rds;			// room_device_schedules

		public function __construct($hostname, $username, $password, $dbname){
			$this->user_details = new User_details($hostname, $username, $password, $dbname);
			$this->floors = new Floors($hostname, $username, $password, $dbname);
			$this->rooms = new Rooms($hostname, $username, $password, $dbname);
			$this->frg = new Floor_room_groups($hostname, $username, $password, $dbname);
			$this->rc = new Room_consumptions($hostname, $username, $password, $dbname);
			$this->rd = new Room_devices($hostname, $username, $password, $dbname);
			$this->rs = new Room_schedules($hostname, $username, $password, $dbname);
			$this->rds = new Room_device_schedules($hostname, $username, $password, $dbname);
		}

		public function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

	}
?>