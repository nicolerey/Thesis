<?php
	include 'mysql_setup.php';

	class Operations extends SQL_Setup{

		private $user_details = "user_details";
		private $rooms = "rooms";
		private $floors = "floors";
		private $frg = "floor_room_groups";
		private $rc = "room_consumptions";
		private $rd = "room_devices";
		private $rs = "room_schedules";
		private $rds = "room_device_schedules";

		function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		function add_auth($new_password){
			if(empty($new_password)){
				$result = array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL);
			}else{
				$check = json_decode($this->get_auth($new_password));
				if(count($check->data)){
					$result = array('status'=>0, 'message'=>"Password is existing in the database!", 'data'=>NULL);
				}else{
					$result = $this->mysql->insert($this->user_details, array('user_details_password'=>sha1($new_password)));
				}
			}
			return $this->encode_output($result);
		}

		function get_auth($password){
			$result = $this->mysql->select($this->user_details, array('user_details_password'=>sha1($password)));
			if(count($result['data']) === 0){
				$result = array('status'=>0, 'message'=>"Wrong password!", 'data'=>NULL);
			}
			return $this->encode_output($result);
		}

		function del_auth($password){
			$check = json_decode($this->get_auth($password));
			if(count($check->data)){
				$result = $this->mysql->delete($this->user_details, array('user_details_password'=>sha1($password)));
				return $this->encode_output($result);
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Password to delete does not exist!", 'data'=>NULL));
			}
		}

		function add_room($name, $port, $status){
			if(!empty($name) || !empty($port) || !empty($status)){
				$check_port = $this->mysql->select($this->rooms, array('rooms_port'=>$port));
				$check_name = $this->mysql->select($this->rooms, array('rooms_name'=>$name));
				if(count($check_port['data']) !== 0 || count($check_name['data']) !== 0){
					$result = array('status'=>0, 'message'=>"A room must have a unique port or name!", 'data'=>NULL);
				}else{
					$result = $this->mysql->insert($this->rooms, array('rooms_name'=>$name, 'rooms_port'=>$port, 'rooms_status'=>$status));
				}
			}else{
				$result = array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL);
			}
			return $this->encode_output($result);
		}

		function get_room($values){
			return $this->encode_output($this->mysql->select($this->rooms, $values));
		}

		function del_room($values){
			if(empty($values)){
				return $this->encode_output(array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->delete($this->rooms, $values));
			}
		}

		function upd_room($values){
			if(empty($values)){
				return $this->encode_output(array('status'=>0, 'message'=>'Empty field is prohibited!', 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->update($this->rooms, $values));
			}
		}

		function add_floor($value){
			if(empty($value)){
				return $this->encode_output(array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL));
			}else{
				$check_floor = $this->mysql->select($this->floors, array('floors_name'=>$value));
				if(count($check_floor['data']) === 0){
					return $this->encode_output($this->mysql->insert($this->floors, array('floors_name'=>$value)));
				}else{
					return $this->encode_output(array('status'=>0, 'message'=>"A floor must have a unique name!", 'data'=>NULL));
				}
			}
		}

		function del_floor($value){
			if(empty($value)){
				return $this->encode_output(array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->delete($this->floors, $value));
			}
		}

		function get_floor($value){
			return $this->encode_output($this->mysql->select($this->floors, $value));
		}

		function upd_floor($value){
			if(empty($value)){
				return $this->encode_output(array('status'=>0, 'message'=>'Empty field is prohibited!', 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->update($this->floors, $value));
			}
		}

		function get_frg($where_values){
			return $this->encode_output($this->mysql->select_query($this->frg, $where_values));
		}

		function add_frg($add_values){
			if(empty($add_values) || count($add_values) == 1){
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}else{
				$check_room = $this->mysql->select($this->rooms, array('rooms_id'=>$add_values['rooms_id']));
				$check_floor = $this->mysql->select($this->floors, array('floors_id'=>$add_values['floors_id']));
				$check_data = $this->mysql->select($this->frg, $add_values);
				$check_room_exist_in_frg = $this->mysql->select($this->frg, array('rooms_id'=>$add_values['rooms_id']));
				if(!count($check_room['data']) || !count($check_floor['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>'One of the parameters does not exist in the database!', 'data'=>NULL));
				}else if(count($check_data['data']) || count($check_room_exist_in_frg['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>'Parameters already exist in the table!', 'data'=>NULL));
				}else{
					return $this->encode_output($this->mysql->insert($this->frg, $add_values));
				}
			}
		}

		function del_frg($where_values){
			return $this->encode_output($this->mysql->delete($this->frg, $where_values));
		}

		function add_rc($add_values){
			if(!empty($add_values) && count($add_values) == 2){
				$check_room_rooms = $this->mysql->select($this->rooms, array('rooms_id'=>$add_values['rooms_id']));
				$check_room_rc = $this->mysql->select($this->rc, array('rooms_id'=>$add_values['rooms_id']));
				if(!count($check_room_rooms['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"Room ID does not exist in the 'room' table!", 'data'=>NULL));
				}else if(count($check_room_rc['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"Room ID is already in the 'rc' table!", 'data'=>NULL));
				}else{
					return $this->encode_output($this->mysql->insert($this->rc, $add_values));
				}
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function get_rc($where_values){
			return $this->encode_output($this->mysql->select_query($this->rc, $where_values));
		}

		function del_rc($where_values){
			return $this->encode_output($this->mysql->delete($this->rc, $where_values));
		}

		function upd_rc($values){
			if(is_array($values) && !empty($values)){
				if(isset($values[0]['rooms_id'])){
					$check_room_rooms = $this->mysql->select($this->rooms, array('rooms_id'=>$values[0]['rooms_id']));
					$check_room_rc = $this->mysql->select($this->rc, array('rooms_id'=>$values[0]['rooms_id']));
					if(!count($check_room_rooms['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"Room ID does not exist in the 'room' table!", 'data'=>NULL));
					}else if(count($check_room_rc['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"Room ID is already in the 'rc' table!", 'data'=>NULL));
					}else{
						return $this->encode_output($this->mysql->update($this->rc, $values));
					}
				}else{
					return $this->encode_output($this->mysql->update($this->rc, $values));
				}
				
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function add_rd($add_values){
			if(!empty($add_values) && count($add_values) == 4){
				$check_room_rooms = $this->mysql->select($this->rooms, array('rooms_id'=>$add_values['rooms_id']));
				$check_port_rd = $this->mysql->select($this->rd, array('room_devices_port'=>$add_values['room_devices_port']));
				if(!count($check_room_rooms['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"Room ID does not exist in the 'room' table!", 'data'=>NULL));
				}else if(count($check_port_rd['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"A device must have a unique port!", 'data'=>NULL));
				}else{
					return $this->encode_output($this->mysql->insert($this->rd, $add_values));
				}
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function get_rd($where_values){
			return $this->encode_output($this->mysql->select_query($this->rd, $where_values));
		}

		function del_rd($where_values){
			return $this->encode_output($this->mysql->delete($this->rd, $where_values));
		}

		function upd_rd($values){
			if(is_array($values) && !empty($values)){
				$flag_room = 0;
				$flag_port = 0;
				if(isset($values[0]['rooms_id'])){
					$check_room_rooms = $this->mysql->select($this->rooms, array('rooms_id'=>$values[0]['rooms_id']));
					if(!count($check_room_rooms['data'])){
						$flag_room = 1;
					}
				}
				if(isset($values[0]['room_devices_port'])){
					$check_port_rd = $this->mysql->select($this->rd, array('room_devices_port'=>$values[0]['room_devices_port']));
					if(count($check_port_rd['data'])){
						$flag_port = 1;
					}
				}
				if($flag_room || $flag_port){
					return $this->encode_output(array('status'=>0, 'message'=>"Either room does not exist or port already exist!", 'data'=>NULL));
				}
				else{
					return $this->encode_output($this->mysql->update($this->rd, $values));
				} 
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function add_rs($values){
			if(!empty($values) && count($values) == 4){
				$check_room = $this->mysql->select($this->rooms, array('rooms_id'=>$values['rooms_id']));
				$check_data = $this->mysql->select($this->rs, array('rooms_id'=>$values['rooms_id'], 'room_schedules_time'=>$values['room_schedules_time'], 'room_schedules_day'=>$values['room_schedules_day']));
				if(!count($check_room['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"Room ID does not exist in the 'room' table!", 'data'=>NULL));
				}
				if(count($check_data['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"The schedule is in conflict with other schedule!", 'data'=>$check_data['data']));
				}
				return $this->encode_output($this->mysql->insert($this->rs, $values));
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function get_rs($where_values){
			return $this->encode_output($this->mysql->select_query($this->rs, $where_values));
		}

		function del_rs($where_values){
			return $this->encode_output($this->mysql->delete($this->rs, $where_values));
		}

		function upd_rs($values){
			if(!empty($values) && is_array($values)){
				if(isset($values[0]['rooms_id'])){
					$check_room_rooms = $this->mysql->select($this->rooms, array('rooms_id'=>$values[0]['rooms_id']));
					if(!count($check_room_rooms['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"Room ID does not exist in the 'room' table!", 'data'=>NULL));
					}
				}
				if(isset($values[0]['rooms_id']) && isset($values[0]['room_schedules_time']) && isset($values[0]['room_schedules_day'])){
					$check_data = $this->mysql->select($this->rs, array('rooms_id'=>$values[0]['rooms_id'], 'room_schedules_time'=>$values[0]['room_schedules_time'], 'room_schedules_day'=>$values[0]['room_schedules_day']));
					if(count($check_data['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"The schedule is in conflict with other schedule!", 'data'=>$check_data['data']));
					}
				}
				$get_data = $this->mysql->select($this->rs, array('room_schedules_id'=>$values[1]['room_schedules_id']));
				if(isset($values[0]['room_schedules_time']) && isset($values[0]['room_schedules_day'])){
					$check_data_again = $this->mysql->select($this->rs, array('rooms_id'=>$get_data['data'][0]['rooms_id'], 'room_schedules_time'=>$values[0]['room_schedules_time'], 'room_schedules_day'=>$values[0]['room_schedules_day']));
					if(count($check_data_again['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"The schedule is in conflict with other schedule!", 'data'=>$check_data_again['data']));
					}
				}
				return $this->encode_output($this->mysql->update($this->rs, $values));
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function add_rds($values){
			if(!empty($values) && count($values) == 2){
				$check_data = $this->mysql->select($this->rds, $values);
				if(count($check_data['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"Data is already in the database!", 'data'=>NULL));
				}
				$check_rd = $this->mysql->select($this->rd, array('room_devices_id'=>$values['room_devices_id']));
				$check_rs = $this->mysql->select($this->rs, array('room_schedules_id'=>$values['room_schedules_id']));
				if(!count($check_rd['data']) || !count($check_rs['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"One of the data passed does not exist!", 'data'=>NULL));
				}
				if($check_rd['data'][0]['rooms_id'] != $check_rs['data'][0]['rooms_id']){
					return $this->encode_output(array('status'=>0, 'message'=>"Device and schedule should be in the same room!", 'data'=>NULL));
				}
				return $this->encode_output($this->mysql->insert($this->rds, $values));
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

		function get_rds($where_values){
			return $this->encode_output($this->mysql->select_query($this->rds, $where_values));
		}

		function del_rds($where_values){
			return $this->encode_output($this->mysql->delete($this->rds, $where_values));
		}

		function upd_rds($values){
			if(!empty($values) && is_array($values)){
				$check_id_rd = $this->mysql->select($this->rd, array('room_devices_id'=>$values[1]['room_devices_id']));
				$check_id_rs = $this->mysql->select($this->rs, array('room_schedules_id'=>$values[1]['room_schedules_id']));
				if(!count($check_id_rd['data']) || !count($check_id_rs['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"One of the data passed does not exist!", 'data'=>NULL));
				}
				if($check_id_rs['data'][0]['rooms_id'] != $check_id_rd['data'][0]['rooms_id']){
					return $this->encode_output(array('status'=>0, 'message'=>"Device and schedule should be in the same room!", 'data'=>NULL));
				}
				$check_data = $this->mysql->select($this->rd, $values[0]);
				if(count($check_data['data'])){
					return $this->encode_output(array('status'=>0, 'message'=>"Data is already in the database!", 'data'=>NULL));
				}
				return $this->encode_output($this->mysql->update($this->rds, $values));
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}
	}
?>