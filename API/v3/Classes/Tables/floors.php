<?php
	class Floors extends Tables{

		public function __construct($hostname, $username, $password, $dbname){
			$this->mysql = new SQL_Setup($hostname, $username, $password, $dbname);
		}

		private function encode_output($data){
			return json_encode((object)$data, JSON_PRETTY_PRINT);
		}

		public function add($value){
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

		public function del($value){
			if(empty($value)){
				return $this->encode_output(array('status'=>0, 'message'=>"Empty field is prohibited!", 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->delete($this->floors, $value));
			}
		}

		public function get($value){
			return $this->encode_output($this->mysql->select($this->floors, $value));
		}

		public function upd($value){
			if(empty($value)){
				return $this->encode_output(array('status'=>0, 'message'=>'Empty field is prohibited!', 'data'=>NULL));
			}else{
				return $this->encode_output($this->mysql->update($this->floors, $value));
			}
		}

	}
?>