<?php
	class Room_devices extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		public function add($add_values){
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

		public function get($where_values){
			return $this->encode_output($this->mysql->select_query($this->rd, $where_values));
		}

		public function del($where_values){
			return $this->encode_output($this->mysql->delete($this->rd, $where_values));
		}

		public function upd($values){
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

	}
?>