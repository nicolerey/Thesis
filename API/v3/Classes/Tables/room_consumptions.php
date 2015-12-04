<?php
	class Room_consumptions extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		public function add($add_values){
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

		public function get($where_values){
			return $this->encode_output($this->mysql->select_query($this->rc, $where_values));
		}

		public function del($where_values){
			return $this->encode_output($this->mysql->delete($this->rc, $where_values));
		}

		public function upd($values){
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

	}
?>