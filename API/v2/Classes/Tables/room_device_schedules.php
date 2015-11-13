<?php
	class Room_device_schedules extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		function add($values){
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

		function get($where_values){
			return $this->encode_output($this->mysql->select_query($this->rds, $where_values));
		}

		function del($where_values){
			return $this->encode_output($this->mysql->delete($this->rds, $where_values));
		}

		function upd($values){
			if(!empty($values) && is_array($values)){
				if(isset($values[0]['room_devices_id'])){
					$check_id_rd = $this->mysql->select($this->rd, array('room_devices_id'=>$values[0]['room_devices_id']));
					if(!count($check_id_rd['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"Device does not exist!", 'data'=>NULL));
					}
				}
				if(isset($values[0]['room_schedules_id'])){
					$check_id_rs = $this->mysql->select($this->rs, array('room_schedules_id'=>$values[0]['room_schedules_id']));
					if(!count($check_id_rs['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"Schedule does not exist!", 'data'=>NULL));
					}
				}
				$check_data = $this->mysql->select($this->rds, $values[1]);
				if(!isset($check_id_rd) && !isset($check_id_rs)){
					return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
				}else if(!isset($check_id_rd) && isset($check_id_rs)){
					$check_rd = $this->mysql->select($this->rd, array('room_devices_id'=>$check_data['data'][0]['room_devices_id']));
					if($check_rd['data'][0]['rooms_id'] != $check_id_rs['data'][0]['rooms_id']){
						return $this->encode_output(array('status'=>0, 'message'=>"Device and schedule should be in the same room!", 'data'=>NULL));
					}
				}else if(isset($check_id_rd) && !isset($check_id_rs)){
					$check_rs = $this->mysql->select($this->rs, array('room_schedules_id'=>$check_data['data'][0]['room_schedules_id']));
					if($check_rs['data'][0]['rooms_id'] != $check_id_rd['data'][0]['rooms_id']){
						return $this->encode_output(array('status'=>0, 'message'=>"Device and schedule should be in the same room!", 'data'=>NULL));
					}
				}else{
					if($check_id_rd['data'][0]['rooms_id'] != $check_id_rs['data'][0]['rooms_id']){
						return $this->encode_output(array('status'=>0, 'message'=>"Device and schedule should be in the same room!", 'data'=>NULL));
					}
					$check_data_exist = $this->mysql->select($this->rds, $values[0]);
					if(count($check_data_exist['data'])){
						return $this->encode_output(array('status'=>0, 'message'=>"Data is existing!", 'data'=>$check_data_exist['data']));
					}
				}
				return $this->encode_output($this->mysql->update($this->rds, $values));
			}else{
				return $this->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL));
			}
		}

	}
?>