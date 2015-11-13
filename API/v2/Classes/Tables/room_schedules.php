<?php
	class Room_schedules extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		public function add($values){
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

		public function get($where_values){
			return $this->encode_output($this->mysql->select_query($this->rs, $where_values));
		}

		public function del($where_values){
			return $this->encode_output($this->mysql->delete($this->rs, $where_values));
		}

		public function upd($values){
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

	}
?>