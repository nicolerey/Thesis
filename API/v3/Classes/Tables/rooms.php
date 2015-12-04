<?php
	class Rooms extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		public function add($name, $port, $status, $address){
			if(!empty($name) || !empty($port) || !empty($status)){
				$check_port = $this->mysql->select($this->rooms, array('rooms_port'=>$port));
				$check_name = $this->mysql->select($this->rooms, array('rooms_name'=>$name));
				$check_address = $this->mysql->select($this->rooms, array('rooms_address'=>$address));
				if(count($check_port['data']) !== 0 || count($check_name['data']) !== 0 || count($check_address['data']) !== 0){
					$result = array('status'=>0, 'message'=>"A room must have a unique port or name or address!", 'data'=>NULL);
				}else{
					$result = $this->mysql->insert($this->rooms, array('rooms_name'=>$name, 'rooms_port'=>$port, 'rooms_status'=>$status, 'rooms_address'=>$address));
				}
			}else{
				$result = array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL);
			}
			return $this->encode_output($result);
		}

		public function get($values){
			return $this->encode_output($this->mysql->select($this->rooms, $values));
		}

		public function del($values){
			if(empty($values)){
				return $this->encode_output(array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->delete($this->rooms, $values));
			}
		}

		public function upd($values){
			if(empty($values)){
				return $this->encode_output(array('status'=>0, 'message'=>'Empty field is prohibited!', 'data'=>NULL));
			}else{
				$check_port = $this->mysql->select($this->rooms, array('rooms_port'=>$values[0]['rooms_port']));
				$check_name = $this->mysql->select($this->rooms, array('rooms_name'=>$values[0]['rooms_name']));
				$check_address = $this->mysql->select($this->rooms, array('rooms_address'=>$values[0]['rooms_address']));
				if(count($check_port['data']) !== 0 || count($check_name['data']) !== 0 || count($check_address['data']) !== 0){
					return $this->encode_output(array('status'=>0, 'message'=>"A room must have a unique port or name or address!", 'data'=>NULL));
				}else{
					return $this->encode_output($this->mysql->update($this->rooms, $values));
				}
			}
		}

	}
?>