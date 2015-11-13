<?php
	class Rooms extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		public function add($name, $port, $status){
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
				return $this->encode_output($this->mysql->update($this->rooms, $values));
			}
		}

	}
?>