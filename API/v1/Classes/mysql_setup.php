<?php
	class SQL_Setup{

		private $hostname;
		private $username;
		private $password;
		private $dbname;

		private $con;

		public $mysql;

		function __construct($hostname, $username, $password, $dbname){
			$this->hostname = $hostname;
			$this->username = $username;
			$this->password = $password;
			$this->dbname = $dbname;
			$this->connect();
		}

		function connect(){
			$this->con = mysqli_connect($this->hostname, $this->username, $this->password, $this->dbname) or exit(json_encode(array('status'=>0, 'message'=>"Unable to connect to database!", 'data'=>NULL), JSON_PRETTY_PRINT));
		}

		function insert($table, $values){
			if(is_array($values)){
				$query = "INSERT INTO ".$table." (".$table."_id, ";
				$sub_query = "('', ";
				$x=0;
				$to_return=0;
				$message = "Error in Query = ";
				$status=0;
				foreach($values as $key=>$value){
					if(($x+1) != count($values)){
						$query = $query." ".$key.", ";
						$sub_query = $sub_query." '".$value."', ";
					}else{
						$query = $query." ".$key.") VALUES ";
						$sub_query = $sub_query." '".$value."')";
					}
					$x++;
				}
				$result = mysqli_query($this->con, $query.$sub_query);
				if($result){
					$to_return = mysqli_insert_id($this->con);
					$message = "Success!";
					$status = 1;
				}else{
					$message = array('MYSQL Error'=>mysqli_error($this->con), 'MYSQL Query'=>$query.$sub_query);
				}
				return array('status'=>$status, 'message'=>$message, 'data'=>$to_return);
			}else{
				return array('status'=>0, 'message'=>"Value passed is not an array!", 'data'=>NULL);
			}
		}

		function select($table, $values = NULL){
			if(is_array($values)){
				$query = "SELECT * FROM ".$table;
				if(!empty($values)){
					$query = $query." WHERE ";
					$x=0;
					foreach($values as $key=>$value){
						if(($x+1) != count($values)){
							$query = $query." ".$key." = '".$value."' AND ";
						}else{
							$query = $query." ".$key." = '".$value."'";
						}
						$x++;
					}
				}
				$result = mysqli_query($this->con, $query);
				if($result){
					if(!mysqli_affected_rows($this->con)){
						return array('status'=>0, 'message'=>"Success but no row is affected! Query = ".$query, 'data'=>NULL);
					}else{
						return array('status'=>1, 'message'=>"Success!", 'data'=>mysqli_fetch_all($result, MYSQLI_ASSOC));
					}
				}else{
					return array('status'=>0, 'message'=>array('MYSQL Error'=>mysqli_error($this->con), 'MYSQL Query'=>$query), 'data'=>NULL);
				}
			}else{
				return array('status'=>0, 'message'=>"Value passed is not an array!", 'data'=>NULL);
			}
		}

		function delete($table, $values){
			if(is_array($values)){
				$query = "DELETE FROM ".$table." WHERE ";
				$x=0;
				foreach($values as $key=>$value){
					if(($x+1) != count($values)){
						$query = $query." ".$key." = '".$value."' AND ";
					}else{
						$query = $query." ".$key." = '".$value."'";
					}
					$x++;
				}
				$result = mysqli_query($this->con, $query);
				if($result){
					if(!mysqli_affected_rows($this->con)){
						return array('status'=>0, 'message'=>"Success but no row is affected! Query = ".$query, 'data'=>NULL);
					}else{
						return array('status'=>1, 'message'=>"Delete success!", 'data'=>NULL);
					}
				}else{
					return array('status'=>0, 'message'=>array('MYSQL Error'=>mysqli_error($this->con), 'MYSQL Query'=>$query), 'data'=>NULL);
				}
			}else{
				return array('status'=>0, 'message'=>"Value passed is not an array!", 'data'=>NULL);
			}
		}

		function update($table, $values){
			if(is_array($values)){
				if(!is_array($values[0]) || !is_array($values[1])){
					return array('status'=>0, 'message'=>"Value passed should be a 2D array!", 'data'=>NULL);
				}else{
					$query = "UPDATE ".$table." SET ";
					$sub_query = " WHERE ";
					$x=0;
					foreach($values[0] as $key=>$value){
						if(($x+1) != count($values[0])){
							$query = $query.$key."='".$value."', ";
						}else{
							$query = $query.$key."='".$value."' WHERE ";
						}
						$x++;
					}
					$x=0;
					foreach($values[1] as $key=>$value){
						if(($x+1) != count($values[1])){
							$query = $query.$key."='".$value."' AND ";
						}else{
							$query = $query.$key."='".$value."'";
						}
						$x++;
					}
				}
				$result = mysqli_query($this->con, $query);
				if($result){
					if(!mysqli_affected_rows($this->con)){
						return array('status'=>0, 'message'=>"Success but no row is affected! Query = ".$query, 'data'=>NULL);
					}else{
						return array('status'=>1, 'message'=>"Update success!", 'data'=>NULl);
					}
				}else{
					return array('status'=>0, 'message'=>array('MYSQL Error'=>mysqli_error($this->con), 'MYSQL Query'=>$query), 'data'=>NULL);
				}
			}else{
				return array('status'=>0, 'message'=>"Value passed is not an array!", 'data'=>NULL);
			}
		}

		function select_query($function, $where_values){
			$where_query = "";
			$x=0;
			if(!empty($where_values)){
				$where_query = " WHERE ";
				foreach($where_values as $key=>$value){
					if(($x+1) != count($where_values)){
						$where_query = $where_query.$function.".".$key."='".$value."' AND ";
					}else{
						$where_query = $where_query.$function.".".$key."='".$value."'";
					}
					$x++;
				}
			}
			switch($function){
				case "floor_room_groups":
					$query = "SELECT floor_room_groups.floor_room_groups_id, floors.floors_id, floors.floors_name, rooms.rooms_id, rooms.rooms_name, rooms.rooms_port, rooms.rooms_status FROM floor_room_groups INNER JOIN floors ON floor_room_groups.floors_id=floors.floors_id INNER JOIN rooms ON floor_room_groups.rooms_id=rooms.rooms_id";
					break;
				case "room_consumptions":
					$query = "SELECT room_consumptions.room_consumptions_id, room_consumptions.room_consumptions, rooms.rooms_id, rooms.rooms_name, rooms.rooms_port, rooms.rooms_status FROM room_consumptions INNER JOIN rooms ON room_consumptions.rooms_id=rooms.rooms_id";
					break;
				case "room_devices":
					$query = "SELECT room_devices.room_devices_id, room_devices.room_devices_name, room_devices.room_devices_port, room_devices.room_devices_status, rooms.rooms_id, rooms.rooms_name, rooms.rooms_port, rooms.rooms_status FROM room_devices INNER JOIN rooms ON room_devices.rooms_id=rooms.rooms_id";
					break;
				case "room_schedules":
					$query = "SELECT room_schedules.room_schedules_id, room_schedules.room_schedules_day, room_schedules.room_schedules_date, room_schedules.room_schedules_time, rooms.rooms_id, rooms.rooms_name, rooms.rooms_port, rooms.rooms_status FROM room_schedules INNER JOIN rooms ON room_schedules.rooms_id=rooms.rooms_id";
					break;
				 case "room_device_schedules":
				 	$query = "SELECT room_device_schedules.room_device_schedules_id, room_devices.room_devices_id, room_schedules.room_schedules_id, room_devices.rooms_id, room_devices.room_devices_name, room_devices.room_devices_port, room_devices.room_devices_status, room_schedules.room_schedules_day, room_schedules.room_schedules_date, room_schedules.room_schedules_time, rooms.rooms_name, rooms.rooms_port, rooms.rooms_status FROM room_device_schedules INNER JOIN room_devices ON room_device_schedules.room_devices_id=room_devices.room_devices_id INNER JOIN room_schedules ON room_device_schedules.room_schedules_id=room_schedules.room_schedules_id INNER JOIN rooms ON rooms.rooms_id=room_devices.rooms_id";
				 	break;
				default:
					$query = NULL;
			}
			if(!$query){
				$query = $query.$where_query;
				return array('status'=>0, 'message'=>"Error in function parameter!", 'data'=>NULL);
			}else{
				$result = mysqli_query($this->con, $query);
				if($result){
					if(!mysqli_affected_rows($this->con)){
						return array('status'=>0, 'message'=>"No data.", 'data'=>NULL);
					}else{
						return array('status'=>1, 'message'=>"Success!", 'data'=>mysqli_fetch_all($result, MYSQLI_ASSOC));
					}
				}else{
					return array('status'=>0, 'message'=>array('MYSQL Error'=>mysqli_error($this->con), 'MYSQL Query'=>$query), 'data'=>NULL);
				}
			}
		}
	}
?>