<?php
	header('Content-Type: application/json');
	include 'Classes/operations.php';

	/* START DB CREDINTIALS */
	$hostname = "localhost";
	$username = "root";
	$password = "";
	$dbname = "thesisdb";
	/* END DB CREDINTIALS */

	/* START CALLING NECESSARY CLASSES */
	$operation = new Operations($hostname, $username, $password, $dbname);
	/* END CALLING NECESSARY CLASSES */

	/* START GET WHAT TO DO */
	if(isset($_GET['function'])){
		switch($_GET['function']){
			case "get_auth":
				if(isset($_GET['password'])){
					echo $operation->get_auth($_GET['password']);
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "add_auth":
				if(isset($_GET['password'])){
					echo $operation->add_auth($_GET['password']);
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "del_auth":
				if(isset($_GET['password'])){
					echo $operation->del_auth($_GET['password']);
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "add_room":
				if(isset($_GET['rooms_name'])){
					if(isset($_GET['rooms_port'])){
						if(isset($_GET['rooms_status'])){
							echo $operation->add_room($_GET['rooms_name'], $_GET['rooms_port'], $_GET['rooms_status']);
						}else{
							exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
						}
					}else{
						exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
					}
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "get_rooms":
				$values = array();
				if(isset($_GET['rooms_name'])){
					$values['rooms_name'] = $_GET['rooms_name'];
				}
				if(isset($_GET['rooms_port'])){
					$values['rooms_port'] = $_GET['rooms_port'];
				}
				if(isset($_GET['rooms_status'])){
					$values['rooms_status'] = $_GET['rooms_status'];
				}
				echo $operation->get_room($values);
				break;
			case "del_rooms":
				$values = array();
				if(isset($_GET['rooms_name'])){
					$values['rooms_name'] = $_GET['rooms_name'];
				}
				if(isset($_GET['rooms_port'])){
					$values['rooms_port'] = $_GET['rooms_port'];
				}
				if(isset($_GET['rooms_id'])){
					$values['rooms_id'] = $_GET['rooms_id'];
				}
				echo $operation->del_room($values);
				break;
			case "upd_rooms":
				$where_values = array();
				$new_values = array();
				if(isset($_GET['rooms_name'])){
					$where_values['rooms_name'] = $_GET['rooms_name'];
				}
				if(isset($_GET['rooms_port'])){
					$where_values['rooms_port'] = $_GET['rooms_port'];
				}
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['new_rooms_name'])){
					$new_values['rooms_name'] = $_GET['new_rooms_name'];
				}
				if(isset($_GET['new_rooms_port'])){
					$new_values['rooms_port'] = $_GET['new_rooms_port'];
				}
				if(isset($_GET['new_rooms_status'])){
					$new_values['rooms_status'] = $_GET['new_rooms_status'];
				}
				echo $operation->upd_room(array($new_values, $where_values));
				break;
			case "add_floor":
				if(isset($_GET['floors_name'])){
					echo $operation->add_floor($_GET['floors_name']);
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "del_floor":
				$where_values = array();
				if(isset($_GET['floors_name'])){
					$where_values['floors_name'] = $_GET['floors_name'];
				}
				if(isset($_GET['floors_id'])){
					$where_values['floors_id'] = $_GET['floors_id'];
				}
				echo $operation->del_floor($where_values);
				break;
			case "get_floor":
				$where_values = array();
				if(isset($_GET['floors_name'])){
					$where_values['floors_name'] = $_GET['floors_name'];
				}
				if(isset($_GET['floors_id'])){
					$where_values['floors_id'] = $_GET['floors_id'];
				}
				echo $operation->get_floor($where_values);
				break;
			case "upd_floor":
				$where_values = array();
				$new_values = array();
				if(isset($_GET['floors_name'])){
					$where_values['floors_name'] = $_GET['floors_name'];
				}
				if(isset($_GET['floors_id'])){
					$where_values['floors_id'] = $_GET['floors_id'];
				}
				if(isset($_GET['new_floors_name'])){
					$new_values['floors_name'] = $_GET['new_floors_name'];
				}
				echo $operation->upd_floor(array($new_values, $where_values));
				break;
			case "get_frg":
				$where_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['floors_id'])){
					$where_values['floors_id'] = $_GET['floors_id'];
				}
				if(isset($_GET['floor_room_groups_id'])){
					$where_values['floor_room_groups'] = $_GET['floor_room_groups'];
				}
				echo $operation->get_frg($where_values);
				break;
			case "add_frg":
				$add_values = array();
				if(!isset($_GET['rooms_id']) || !isset($_GET['floors_id'])){
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}else{
					$add_values['rooms_id'] = $_GET['rooms_id'];
					$add_values['floors_id'] = $_GET['floors_id'];
				}
				echo $operation->add_frg($add_values);
				break;
			case "del_frg":
				if(isset($_GET['floor_room_groups_id'])){
					echo $operation->del_frg(array('floor_room_groups_id'=>$_GET['floor_room_groups_id']));
				}else if(isset($_GET['floors_id']) && isset($_GET['rooms_id'])){
					echo $operation->del_frg(array('floors_id'=>$_GET['floors_id'], 'rooms_id'=>$_GET['rooms_id']));
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "add_rc":
				$add_values = array();
				if(isset($_GET['rooms_id']) && is_numeric($_GET['rooms_id'])){
					$add_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_consumptions'])){
					if(is_float(floatval($_GET['room_consumptions']))){
						$add_values['room_consumptions'] = $_GET['room_consumptions'];
					}else{
						exit($operation->encode_output(array('status'=>0, 'message'=>"Parameter should be float!", 'data'=>NULL)));
					}
				}
				if(count($add_values) == 2){
					echo $operation->add_rc($add_values);
				}else{
					exit($operation->encode_output(array('status'=>0, 'message'=>"Lack of parameters!", 'data'=>NULL)));
				}
				break;
			case "get_rc":
				$where_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_consumptions_id'])){
					$where_values['room_consumptions_id'] = $_GET['room_consumptions_id'];
				}
				echo $operation->get_rc($where_values);
				break;
			case "del_rc":
				$where_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_consumptions_id'])){
					$where_values['room_consumptions_id'] = $_GET['room_consumptions_id'];
				}
				echo $operation->del_rc($where_values);
				break;
			case "upd_rc":
				$where_values = array();
				$new_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_consumptions_id'])){
					$where_values['room_consumptions_id'] = $_GET['room_consumptions_id'];
				}
				if(isset($_GET['new_room_consumptions'])){
					$new_values['room_consumptions'] = $_GET['new_room_consumptions'];
				}
				if(isset($_GET['new_rooms_id'])){
					$new_values['rooms_id'] = $_GET['new_rooms_id'];

				}
				echo $operation->upd_rc(array($new_values, $where_values));
				break;
			case "add_rd":
				$add_values = array();
				if(isset($_GET['rooms_id'])){
					$add_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_devices_name'])){
					$add_values['room_devices_name'] = $_GET['room_devices_name'];
				}
				if(isset($_GET['room_devices_port'])){
					$add_values['room_devices_port'] = $_GET['room_devices_port'];
				}
				if(isset($_GET['room_devices_status'])){
					$add_values['room_devices_status'] = $_GET['room_devices_status'];
				}
				echo $operation->add_rd($add_values);
				break;
			case "get_rd":
				$where_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_devices_id'])){
					$where_values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_devices_port'])){
					$where_values['room_devices_port'] = $_GET['room_devices_port'];
				}
				if(isset($_GET['room_devices_status'])){
					$where_values['room_devices_status'] = $_GET['room_devices_status'];
				}
				echo $operation->get_rd($where_values);
				break;
			case "del_rd":
				$where_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_devices_id'])){
					$where_values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_devices_port'])){
					$where_values['room_devices_port'] = $_GET['room_devices_port'];
				}
				echo $operation->del_rd($where_values);
				break;
			case "upd_rd":
				$where_values = array();
				$new_values = array();
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_devices_id'])){
					$where_values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_devices_port'])){
					$where_values['room_devices_port'] = $_GET['room_devices_port'];
				}
				if(isset($_GET['new_rooms_id'])){
					$new_values['rooms_id'] = $_GET['new_rooms_id'];
				}
				if(isset($_GET['new_room_devices_name'])){
					$new_values['room_devices_name'] = $_GET['new_room_devices_name'];
				}
				if(isset($_GET['new_room_devices_port'])){
					$new_values['room_devices_port'] = $_GET['new_room_devices_port'];
				}
				if(isset($_GET['new_room_devices_status'])){
					$new_values['room_devices_status'] = $_GET['new_room_devices_status'];
				}
				echo $operation->upd_rd(array($new_values, $where_values));
				break;
			case "add_rs":
				$values = array();
				if(isset($_GET['rooms_id'])){
					$values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_schedules_day'])){
					$values['room_schedules_day'] = ucwords($_GET['room_schedules_day']);
				}
				if(isset($_GET['room_schedules_date'])){
					$values['room_schedules_date'] = date("Y-m-d", strtotime($_GET['room_schedules_date']));
				}
				if(isset($_GET['room_schedules_time'])){
					$values['room_schedules_time'] = intval($_GET['room_schedules_time']);
				}
				echo $operation->add_rs($values);
				break;
			case "get_rs":
				$where_values = array();
				if(isset($_GET['room_schedules_id'])){
					$where_values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_schedules_day'])){
					$where_values['room_schedules_day'] = ucwords($_GET['room_schedules_day']);
				}
				if(isset($_GET['room_schedules_date'])){
					$where_values['room_schedules_date'] = date("Y-m-d", strtotime($_GET['room_schedules_date']));
				}
				if(isset($_GET['room_schedules_time'])){
					$where_values['room_schedules_time'] = intval($_GET['room_schedules_time']);
				}
				echo $operation->get_rs($where_values);
				break;
			case "del_rs":
				$where_values = array();
				if(isset($_GET['room_schedules_id'])){
					$where_values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				if(isset($_GET['rooms_id'])){
					$where_values['rooms_id'] = $_GET['rooms_id'];
				}
				if(isset($_GET['room_schedules_day'])){
					$where_values['room_schedules_day'] = ucwords($_GET['room_schedules_day']);
				}
				if(isset($_GET['room_schedules_date'])){
					$where_values['room_schedules_date'] = date("Y-m-d", strtotime($_GET['room_schedules_date']));
				}
				if(isset($_GET['room_schedules_time'])){
					$where_values['room_schedules_time'] = intval($_GET['room_schedules_time']);
				}
				echo $operation->del_rs($where_values);
				break;
			case "upd_rs":
				$where_values = array();
				$new_values = array();
				if(isset($_GET['room_schedules_id'])){
					$where_values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				if(isset($_GET['new_rooms_id'])){
					$new_values['rooms_id'] = $_GET['new_rooms_id'];
				}
				if(isset($_GET['new_room_schedules_day'])){
					$new_values['room_schedules_day'] = ucwords($_GET['new_room_schedules_day']);
				}
				if(isset($_GET['new_room_schedules_date'])){
					$new_values['room_schedules_date'] = date("Y-m-d", strtotime($_GET['new_room_schedules_date']));
				}
				if(isset($_GET['new_room_schedules_time'])){
					$new_values['room_schedules_time'] = intval($_GET['new_room_schedules_time']);
				}
				echo $operation->upd_rs(array($new_values, $where_values));
				break;
			case "add_rds":
				$values = array();
				if(isset($_GET['room_devices_id'])){
					$values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_schedules_id'])){
					$values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				echo $operation->add_rds($values);
				break;
			case "get_rds":
				$where_values = array();
				if(isset($_GET['room_device_schedules_id'])){
					$where_values['room_device_schedules_id'] = $_GET['room_device_schedules_id'];
				}
				if(isset($_GET['room_devices_id'])){
					$where_values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_schedules_id'])){
					$where_values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				echo $operation->get_rds($where_values);
				break;
			case "del_rds":
				$where_values = array();
				if(isset($_GET['room_device_schedules_id'])){
					$where_values['room_device_schedules_id'] = $_GET['room_device_schedules_id'];
				}
				if(isset($_GET['room_devices_id'])){
					$where_values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_schedules_id'])){
					$where_values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				echo $operation->del_rds($where_values);
				break;
			case "upd_rds":
				$where_values = array();
				$new_values = array();
				if(isset($_GET['room_device_schedules_id'])){
					$where_values['room_device_schedules_id'] = $_GET['room_device_schedules_id'];
				}
				if(isset($_GET['room_devices_id'])){
					$where_values['room_devices_id'] = $_GET['room_devices_id'];
				}
				if(isset($_GET['room_schedules_id'])){
					$where_values['room_schedules_id'] = $_GET['room_schedules_id'];
				}
				if(isset($_GET['new_room_devices_id'])){
					$new_values['room_devices_id'] = $_GET['new_room_devices_id'];
				}
				if(isset($_GET['new_room_schedules_id'])){
					$new_values['room_schedules_id'] = $_GET['new_room_schedules_id'];
				}
				echo $operation->upd_rds(array($new_values, $where_values));
				break;
			default:
				exit($operation->encode_output(array('status'=>0, 'message'=>"Call of non-existing function!", 'data'=>NULL)));
		}
	}else{
		exit($operation->encode_output(array('status'=>0, 'message'=>"No function is called!", 'data'=>NULL)));
	}
	/* END GET WHAT TO DO */
?>